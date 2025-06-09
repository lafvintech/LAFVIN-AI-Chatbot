.. _troubleshooting:

Troubleshooting
===============

This section will help you solve common problems that you may encounter when using the ESP32S3 AI Chatbot.

Connection Issues
------------------------------------------

**Device Cannot Connect to Wi-Fi**

* **Symptom**: OLED screen displays Wi-Fi icon with a diagonal line through it or prompts to enter Wi-Fi configuration mode
* **Possible Causes**:
  
  1. Incorrect Wi-Fi password
  2. Wi-Fi signal is too weak
  3. Wi-Fi network is a 5GHz network
  
* **Solutions**:
  
  1. Re-enter the web pairing page and confirm that the Wi-Fi password entered is correct
  2. Move the device closer to the router
  3. Ensure your router supports 2.4GHz Wi-Fi (ESP32S3 does not support 5GHz networks)

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
  
  1. Microphone connection error
  2. Excessive environmental noise
  
* **Solutions**:
  
  1. Check if the microphone module connection is correct and secure
  2. Increase microphone sensitivity in the backend settings
  3. Move to a quieter environment for use
  4. Speak closer to the microphone

**Speaker Has No Sound**

* **Symptom**: No sound output when the robot responds
* **Possible Causes**:
  
  1. Speaker connection issues
  2. Volume set to zero or too low
  3. Damaged speaker
  
* **Solutions**:
  
  1. Check the speaker module connection
  2. Increase speaker volume in the backend settings
  3. Replace the speaker module

Display Issues
------------------------------------------

**OLED Screen Does Not Display**

* **Symptom**: OLED screen is completely black
* **Possible Causes**:
  
  1. Screen connection issues
  2. Damaged screen
  
* **Solutions**:
  
  1. Check if the OLED module connections are correct
  2. Restart the device
  3. If the above methods are ineffective, you may need to replace the OLED module

**Abnormal Display Content**

* **Symptom**: Screen displays garbled characters or incomplete content
* **Possible Causes**:
  
  1. Unstable screen connection
  2. Insufficient power supply
  
* **Solutions**:
  
  1. Check the OLED module connection
  2. Replace the power adapter

System Issues
------------------------------------------

**Device Frequently Restarts**

* **Symptom**: Device restarts unexpectedly or cycles through restarts
* **Possible Causes**:
  
  1. Unstable power supply or low voltage
  2. Circuit connection causing a short circuit
  
* **Solutions**:
  
  1. Check if the USB power supply is stable, try replacing with a 5V1A or higher power adapter
  2. Update to the latest firmware version
  3. Confirm that the device is wired correctly

**Device Responds Slowly**

* **Symptom**: Long response time, noticeable operation delay
* **Possible Causes**:
  
  1. Slow network connection
  
* **Solutions**:
  
  1. Check network connection quality

Reset Device
------------------------------------------

If you encounter problems that cannot be resolved, you can try resetting the device:

**Network Reconfiguration**

1. Press the "boot" button when the device is powered on, and the device will re-enter network configuration mode

**Reflash Firmware**

1. Follow the steps in :ref:`firmware_upload` to reflash the firmware. After flashing is complete, you need to reconfigure the network (if you did not unbind the device in the backend before flashing, the device will automatically connect to the previously bound agent)

Contact Support
------------------------------------------

If the above methods cannot solve your problem:

1. Send an email to technical support: `hosyond_service@163.com <mailto:hosyond_service@163.com>`_

When seeking help, please provide the following information:

* Detailed description of the problem
* Steps when the problem occurred
* Solutions you have already tried 