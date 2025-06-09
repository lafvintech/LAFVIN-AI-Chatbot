.. _troubleshooting:

Troubleshooting
===============

This section will help you solve common problems that you may encounter when using the ESP32S3 AI Chatbot.

Connection Issues
------------------------------------------

**Device Cannot Connect to Wi-Fi**

* **Symptom**: OLED screen displays Wi-Fi icon with a diagonal line through it or prompts to enter Wi-Fi configuration mode
* **Possible Causes**:
  
  1. Wi-Fi密码不正确
  2. Wi-Fi信号太弱或者无法上网
  3. Wi-Fi网络是5GHz网络
  
* **Solutions**:
  
  1. 重新烧录进入网页配对页面，确认输入的Wi-Fi密码正确
  2. 将设备移到更靠近路由器的位置
  3. 确保您的路由器支持2.4GHz Wi-Fi（ESP32S3不支持5GHz网络）
  4. 使用连接到同网络的设备看下是否能访问网络以及openai网站

**Cannot Connect to AI Service**

* **Symptom**: After the device starts, it displays "Please log into the control panel to add the device, enter the verification code xxxxxx."
* **Possible Causes**:
  
  1. Device has not been added in the backend
  2. Incorrect verification code entered
  
* **Solutions**:
  
  1. Re-verify and enter the verification code in the backend management system

Voice Issues
------------------------------------------

**Microphone Cannot Recognize Speech**

* **Symptom**: When speaking, the OLED screen does not display any recognized text
* **Possible Causes**:
  
  1. 麦克风连接错误
  2. 环境噪音过大
  3. 音频模块没有连接正确或者不牢固
  
* **Solutions**:
  
  1. 检查音频模块连接是否正确和牢固
  2. 在后台设置中增加麦克风灵敏度
  3. 移到更安静的环境中使用靠近麦克风说话

**扬声器没有声音**

* **症状**：机器人回应时没有声音输出
* **可能原因**：

  1. 扬声器没有连接到音频模块上
  2. 音量设置为零或太低
  3. 扬声器损坏
  
* **Solutions**:
  
  1. Check the speaker module connection
  2. 调整播放音量

Display Issues
------------------------------------------

**LCD Screen Does Not Display**

* **症状**：LCD屏幕完全黑屏或不显示
* **可能原因**：
  
  1. 屏幕连接问题
  2. 系统没有烧录成功
  
* **Solutions**:
  
  1. 检查LCD屏幕连接是否正确
  2. 重启设备

**Abnormal Display Content**

* **症状**：屏幕显示闪烁或者不断重启
* **可能原因**：

  1. Unstable screen connection
  2. Insufficient power supply
  
* **Solutions**:
  
  1. 检查LCD模块连接
  2. 更换充足电源(建议最少5V2A的电源适配器)

System Issues
------------------------------------------

**Device Frequently Restarts**

* **Symptom**: Device restarts unexpectedly or cycles through restarts
* **Possible Causes**:
  
  1. Unstable power supply or low voltage
  2. Circuit connection causing a short circuit
  
* **Solutions**:
  
  1. 检查USB电源是否稳定(建议最少5V2A的电源适配器)
  2. 确定固件烧录正确
  3. 确认设备接线正确

**Device Responds Slowly**

* **Symptom**: Long response time, noticeable operation delay
* **Possible Causes**:
  
  1. Slow network connection
  
* **Solutions**:
  
  1. Check network connection quality
  2. 更换网络

Reset Device
------------------------------------------

If you encounter problems that cannot be resolved, you can try resetting the device:

**Network Reconfiguration**(xiaozhi version)

1. 在设备通电时长按"boot"按钮，设备将重新进入网络配置模式

**Reflash Firmware**

1. Follow the steps in :ref:`firmware_upload` to reflash the firmware. After flashing is complete, you need to reconfigure the network (if you did not unbind the device in the backend before flashing, the device will automatically connect to the previously bound agent)

Contact Support
------------------------------------------

If the above methods cannot solve your problem:

1. 发送电子邮件至技术支持：`tech_edu_service@outlook.com <mailto:tech_edu_service@outlook.com>`_

When seeking help, please provide the following information:

* Detailed description of the problem
* Steps when the problem occurred
* Solutions you have already tried 