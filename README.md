@ -0,0 +1,43 @@
# LAFVIN AI Chatbot

An AI voice assistant powered by ESP32-S3 and large language models (Qwen / DeepSeek).

## Branches

| Branch | Content |
|--------|---------|
| **[main](https://github.com/LAFVIN-AI-Chatbot/LAFVIN-AI-Chatbot/tree/main)** | Prebuilt firmware binaries, USB drivers, and flashing tools |
| **[src](https://github.com/LAFVIN-AI-Chatbot/LAFVIN-AI-Chatbot/tree/src)** | Complete source code of the Xiaozhi ESP32 firmware |

## Quick Start

### Flash prebuilt firmware (main branch)

1. Switch to the `main` branch and download `flash_download_tool.zip`
2. Install the USB driver (`CP210X`) for your operating system
3. Use the flash download tool to flash `Xiaozhi.bin` to your ESP32-S3 board

### Build from source (src branch)

1. Switch to the `src` branch
2. Follow the build instructions in [xiaozhi-esp32-main/README.md](xiaozhi-esp32-main/README.md)

## Features

- Offline wake word detection (ESP-SR)
- Streaming ASR + LLM + TTS voice conversation
- OPUS audio codec
- Voiceprint recognition (3D-Speaker)
- LCD display with emoji expressions
- Wi-Fi & 4G connectivity
- MCP-based device control
- Multi-language support

## Hardware

Designed for the **LAFVIN AI Chatbot** (ESP32-S3 + ST7789 2.0" LCD).  
See `src` branch → `main/boards/lafvin-aichatbot/` for board configuration details.

## License

Refer to [LICENSE](xiaozhi-esp32-main/LICENSE) in the source tree.
