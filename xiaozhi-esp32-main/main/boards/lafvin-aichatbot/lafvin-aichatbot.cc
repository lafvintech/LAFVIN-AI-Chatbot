#include "wifi_board.h"
#include "codecs/box_audio_codec.h"
#include "display/lcd_display.h"
#include "display/emote_display.h"
#include "application.h"
#include "button.h"
#include "config.h"
#include "i2c_device.h"
#include "mcp_server.h"

#include <esp_log.h>
#include <esp_lcd_panel_vendor.h>
#include <driver/i2c_master.h>
#include <driver/spi_common.h>
#include <esp_lcd_touch_ft5x06.h>
#include <esp_lvgl_port.h>
#include <lvgl.h>

#define TAG "LichuangDevBoard"

// class Pca9557 : public I2cDevice {
// public:
//     Pca9557(i2c_master_bus_handle_t i2c_bus, uint8_t addr) : I2cDevice(i2c_bus, addr) {
//         WriteReg(0x01, 0x03);
//         WriteReg(0x03, 0xf8);
//     }

//     void SetOutputState(uint8_t bit, uint8_t level) {
//         uint8_t data = ReadReg(0x01);
//         data = (data & ~(1 << bit)) | (level << bit);
//         WriteReg(0x01, data);
//     }
// };

class CustomAudioCodec : public BoxAudioCodec {
private:
    gpio_num_t pa_en_pin_;

public:
    CustomAudioCodec(i2c_master_bus_handle_t i2c_bus, gpio_num_t pa_en_pin) 
        : BoxAudioCodec(i2c_bus, 
                       AUDIO_INPUT_SAMPLE_RATE, 
                       AUDIO_OUTPUT_SAMPLE_RATE,
                       AUDIO_I2S_GPIO_MCLK, 
                       AUDIO_I2S_GPIO_BCLK, 
                       AUDIO_I2S_GPIO_WS, 
                       AUDIO_I2S_GPIO_DOUT, 
                       AUDIO_I2S_GPIO_DIN,
                        pa_en_pin, // Use our own PA enable pin
                       AUDIO_CODEC_ES8311_ADDR, 
                       AUDIO_CODEC_ES7210_ADDR, 
                       AUDIO_INPUT_REFERENCE),
          pa_en_pin_(pa_en_pin) {
    }

    virtual void EnableOutput(bool enable) override {
        BoxAudioCodec::EnableOutput(enable);
        // Control PA enable directly via GPIO
        gpio_set_level(pa_en_pin_, enable ? 1 : 0);
    }
};

class LichuangDevBoard : public WifiBoard {
private:
    i2c_master_bus_handle_t i2c_bus_;
    // i2c_master_dev_handle_t pca9557_handle_;
    Button boot_button_;
    Display* display_;
    Camera* camera_ = nullptr;
    // Pca9557* pca9557_;

    const gpio_num_t lcd_cs_pin_ = GPIO_NUM_47;    // Choose an available GPIO pin as LCD chip select
    const gpio_num_t pa_en_pin_ = GPIO_NUM_48;     // Choose an available GPIO pin as PA enable


    void InitializeI2c() {
        // Initialize I2C peripheral
        i2c_master_bus_config_t i2c_bus_cfg = {
            .i2c_port = (i2c_port_t)1,
            .sda_io_num = AUDIO_CODEC_I2C_SDA_PIN,
            .scl_io_num = AUDIO_CODEC_I2C_SCL_PIN,
            .clk_source = I2C_CLK_SRC_DEFAULT,
            .glitch_ignore_cnt = 7,
            .intr_priority = 0,
            .trans_queue_depth = 0,
            .flags = {
                .enable_internal_pullup = 1,
            },
        };
        ESP_ERROR_CHECK(i2c_new_master_bus(&i2c_bus_cfg, &i2c_bus_));
        
        // Initialize LCD CS pin and PA enable pin
        gpio_config_t io_conf = {};
        io_conf.intr_type = GPIO_INTR_DISABLE;
        io_conf.mode = GPIO_MODE_OUTPUT;
        io_conf.pin_bit_mask = (1ULL << lcd_cs_pin_) | (1ULL << pa_en_pin_);
        io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
        io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
        gpio_config(&io_conf);
        
        // Set LCD CS pin high by default (deselected)
        gpio_set_level(lcd_cs_pin_, 1);
        
        // Set PA enable pin low by default (disable speaker output)
        gpio_set_level(pa_en_pin_, 0);
        // Initialize PCA9557
    }

    void InitializeSpi() {
        spi_bus_config_t buscfg = {};
        buscfg.mosi_io_num = GPIO_NUM_40;
        buscfg.miso_io_num = GPIO_NUM_NC;
        buscfg.sclk_io_num = GPIO_NUM_41;
        buscfg.quadwp_io_num = GPIO_NUM_NC;
        buscfg.quadhd_io_num = GPIO_NUM_NC;
        buscfg.max_transfer_sz = DISPLAY_WIDTH * DISPLAY_HEIGHT * sizeof(uint16_t);
        ESP_ERROR_CHECK(spi_bus_initialize(SPI3_HOST, &buscfg, SPI_DMA_CH_AUTO));
    }

    void InitializeButtons() {
        boot_button_.OnClick([this]() {
            auto& app = Application::GetInstance();
            // During startup (before connected), pressing BOOT button enters Wi-Fi config mode without reboot
            if (app.GetDeviceState() == kDeviceStateStarting) {
                EnterWifiConfigMode();
                return;
            }
            app.ToggleChatState();
        });

#if CONFIG_USE_DEVICE_AEC
        boot_button_.OnDoubleClick([this]() {
            auto& app = Application::GetInstance();
            if (app.GetDeviceState() == kDeviceStateIdle) {
                app.SetAecMode(app.GetAecMode() == kAecOff ? kAecOnDeviceSide : kAecOff);
            }
        });
#endif
    }

    void InitializeSt7789Display() {
        esp_lcd_panel_io_handle_t panel_io = nullptr;
        esp_lcd_panel_handle_t panel = nullptr;
        // Initialize LCD panel IO
        ESP_LOGD(TAG, "Install panel IO");
        esp_lcd_panel_io_spi_config_t io_config = {};
        io_config.cs_gpio_num = lcd_cs_pin_; // Use our own CS pin
        io_config.dc_gpio_num = GPIO_NUM_39;
        io_config.spi_mode = 0;
        io_config.pclk_hz = 40 * 1000 * 1000;
        io_config.trans_queue_depth = 10;
        io_config.lcd_cmd_bits = 8;
        io_config.lcd_param_bits = 8;
        ESP_ERROR_CHECK(esp_lcd_new_panel_io_spi(SPI3_HOST, &io_config, &panel_io));

        // Initialize ST7789 LCD driver
        ESP_LOGD(TAG, "Install LCD driver");
        esp_lcd_panel_dev_config_t panel_config = {};
        panel_config.reset_gpio_num = GPIO_NUM_4;
        panel_config.rgb_ele_order = LCD_RGB_ELEMENT_ORDER_RGB;
        panel_config.bits_per_pixel = 16;
        ESP_ERROR_CHECK(esp_lcd_new_panel_st7789(panel_io, &panel_config, &panel));
        
        esp_lcd_panel_reset(panel);
        // pca9557_->SetOutputState(0, 0);

        esp_lcd_panel_init(panel);
        esp_lcd_panel_invert_color(panel, true);
        esp_lcd_panel_swap_xy(panel, DISPLAY_SWAP_XY);
        esp_lcd_panel_mirror(panel, DISPLAY_MIRROR_X, DISPLAY_MIRROR_Y);
        esp_lcd_panel_disp_on_off(panel, true);

#if CONFIG_USE_EMOTE_MESSAGE_STYLE
        display_ = new emote::EmoteDisplay(panel, panel_io, DISPLAY_WIDTH, DISPLAY_HEIGHT);
#else
        display_ = new SpiLcdDisplay(panel_io, panel,
            DISPLAY_WIDTH, DISPLAY_HEIGHT, DISPLAY_OFFSET_X, DISPLAY_OFFSET_Y, DISPLAY_MIRROR_X, DISPLAY_MIRROR_Y, DISPLAY_SWAP_XY);
#endif
    }

    // void InitializeTouch()
    // {
    //     esp_lcd_touch_handle_t tp;
    //     esp_lcd_touch_config_t tp_cfg = {
    //         .x_max = DISPLAY_HEIGHT,
    //         .y_max = DISPLAY_WIDTH,
    //         .rst_gpio_num = GPIO_NUM_NC, // Shared with LCD reset
    //         .int_gpio_num = GPIO_NUM_NC, 
    //         .levels = {
    //             .reset = 0,
    //             .interrupt = 0,
    //         },
    //         .flags = {
    //             .swap_xy = 1,
    //             .mirror_x = 1,
    //             .mirror_y = 0,
    //         },
    //     };
    //     esp_lcd_panel_io_handle_t tp_io_handle = NULL;
    //     esp_lcd_panel_io_i2c_config_t tp_io_config = ESP_LCD_TOUCH_IO_I2C_FT5x06_CONFIG();
    //     tp_io_config.scl_speed_hz = 400000;

    //     esp_lcd_new_panel_io_i2c(i2c_bus_, &tp_io_config, &tp_io_handle);
    //     esp_lcd_touch_new_i2c_ft5x06(tp_io_handle, &tp_cfg, &tp);
    //     assert(tp);

    //     /* Add touch input (for selected screen) */
    //     const lvgl_port_touch_cfg_t touch_cfg = {
    //         .disp = lv_display_get_default(), 
    //         .handle = tp,
    //     };

    //     if(touch_cfg.disp) {
    //         lvgl_port_add_touch(&touch_cfg);
    //     } else {
    //         ESP_LOGE(TAG, "Touch display is not initialized");
    //     }
    // }

    // void InitializeCamera() {
    //     // Open camera power
    //     pca9557_->SetOutputState(2, 0);

    //     camera_config_t config = {};
    //     config.ledc_channel = LEDC_CHANNEL_2;
    //     config.ledc_timer = LEDC_TIMER_2;
    //     config.pin_d0 = CAMERA_PIN_D0;
    //     config.pin_d1 = CAMERA_PIN_D1;
    //     config.pin_d2 = CAMERA_PIN_D2;
    //     config.pin_d3 = CAMERA_PIN_D3;
    //     config.pin_d4 = CAMERA_PIN_D4;
    //     config.pin_d5 = CAMERA_PIN_D5;
    //     config.pin_d6 = CAMERA_PIN_D6;
    //     config.pin_d7 = CAMERA_PIN_D7;
    //     config.pin_xclk = CAMERA_PIN_XCLK;
    //     config.pin_pclk = CAMERA_PIN_PCLK;
    //     config.pin_vsync = CAMERA_PIN_VSYNC;
    //     config.pin_href = CAMERA_PIN_HREF;
    //     config.pin_sccb_sda = -1;
    //     config.pin_sccb_scl = CAMERA_PIN_SIOC;
    //     config.sccb_i2c_port = 1;
    //     config.pin_pwdn = CAMERA_PIN_PWDN;
    //     config.pin_reset = CAMERA_PIN_RESET;
    //     config.xclk_freq_hz = XCLK_FREQ_HZ;
    //     config.pixel_format = PIXFORMAT_RGB565;
    //     config.frame_size = FRAMESIZE_QVGA;
    //     config.jpeg_quality = 12;
    //     config.fb_count = 1;
    //     config.fb_location = CAMERA_FB_IN_PSRAM;
    //     config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;

    //     camera_ = new Esp32Camera(config);
    // }

    void InitializeTools() {
        auto &mcp_server = McpServer::GetInstance();
        mcp_server.AddTool("self.system.reconfigure_wifi",
            "End this conversation and enter WiFi configuration mode.\n"
            "**CAUTION** You must ask the user to confirm this action.",
            PropertyList(), [this](const PropertyList& properties) {
                EnterWifiConfigMode();
                return true;
            });
    }

public:
    LichuangDevBoard() : boot_button_(BOOT_BUTTON_GPIO) {
        InitializeI2c();
        InitializeSpi();
        InitializeSt7789Display();
        // InitializeTouch();
        InitializeButtons();
        // InitializeCamera();
        InitializeTools();

        GetBacklight()->RestoreBrightness();
    }

    virtual AudioCodec* GetAudioCodec() override {
        static CustomAudioCodec audio_codec(
            i2c_bus_, 
            pa_en_pin_);
        return &audio_codec;
    }

    virtual Display* GetDisplay() override {
        return display_;
    }
    
    virtual Backlight* GetBacklight() override {
        static PwmBacklight backlight(DISPLAY_BACKLIGHT_PIN, DISPLAY_BACKLIGHT_OUTPUT_INVERT);
        return &backlight;
    }

    virtual Camera* GetCamera() override {
        return camera_;
    }
};

DECLARE_BOARD(LichuangDevBoard);
