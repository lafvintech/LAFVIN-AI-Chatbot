.. _troubleshooting:

Troubleshooting
===============

This section helps you resolve common issues you may encounter while using the LAFVIN ESP32S3 AIChatBot.

Connection Issues
------------------------------------------

**Device Cannot Connect to Wi-Fi**

* **Symptom**: The LCD screen shows a Wi-Fi icon with a diagonal line, or the device repeatedly returns to Wi-Fi configuration mode.
* **Possible Causes**:

  1. Incorrect Wi-Fi password
  2. Weak Wi-Fi signal or unstable internet access
  3. The Wi-Fi network is 5 GHz only

* **Solutions**:

  1. Re-enter the Wi-Fi configuration page and confirm that the password is correct
  2. Move the device closer to the router
  3. Make sure your router provides a 2.4 GHz Wi-Fi network, since ESP32-S3 does not support 5 GHz Wi-Fi
  4. Use another device on the same network to confirm that the internet connection is working properly
  5. If you are using the ChatGPT firmware, make sure the network can access OpenAI's servers

**Cannot Connect to AI Service**

* **Symptom**: After startup, the device asks you to add the device in the control panel or enter a verification code.
* **Possible Causes**:

  1. The device has not been added in the backend
  2. The verification code was entered incorrectly
  3. The ChatGPT firmware has not been configured completely

* **Solutions**:

  1. Follow :ref:`xiaozhi_backend_setup` to complete Xiaozhi account registration, device binding, and backend setup
  2. Follow :ref:`chatgpt_conf` to complete Wi-Fi and API key configuration for the ChatGPT firmware

Voice Issues
------------------------------------------

**Microphone Cannot Recognize Speech**

* **Symptom**: When you speak, the LCD screen does not show any recognized text.
* **Possible Causes**:

  1. Incorrect microphone connection
  2. Excessive background noise
  3. The audio module is loose or not connected securely

* **Solutions**:

  1. Check that the audio module is connected correctly and firmly
  2. Move to a quieter environment and speak closer to the microphone

**Speaker Has No Sound**

* **Symptom**: The device responds, but there is no sound output.
* **Possible Causes**:

  1. The speaker is not connected to the audio module
  2. The volume is too low or muted
  3. The speaker is damaged

* **Solutions**:

  1. Check the speaker module connection
  2. On the ChatGPT firmware, double-click the ``UP`` button to increase the volume
  3. On the Xiaozhi firmware, you can also use a voice command such as ``Set the volume to 60 percent``

Display Issues
------------------------------------------

**LCD Screen Does Not Display**

* **Symptom**: The LCD screen is completely black or shows nothing.
* **Possible Causes**:

  1. Incorrect screen connection
  2. The firmware was not flashed successfully

* **Solutions**:

  1. Check that the LCD screen connections are correct
  2. Restart the device
  3. Reflash the firmware by following the :doc:`Online Flasher </Appendix/online_flasher>` guide

**Abnormal Display Content**

* **Symptom**: The screen flickers, shows unstable content, or the device keeps restarting.
* **Possible Causes**:

  1. Unstable LCD connection
  2. Insufficient power supply

* **Solutions**:

  1. Check the LCD module connection carefully
  2. Use a stable power source, with a recommended minimum of 5V/2A
  3. If needed, connect another powered Type-C cable to the second Type-C port so both ports can provide power at the same time

System Issues
------------------------------------------

**Device Frequently Restarts**

* **Symptom**: The device restarts unexpectedly or enters a repeated reboot cycle.
* **Possible Causes**:

  1. Unstable power supply or low voltage
  2. A short circuit caused by incorrect wiring

* **Solutions**:

  1. Check that the USB power supply is stable, with a recommended minimum of 5V/2A
  2. Verify that the firmware was flashed correctly
  3. Confirm that all wiring is correct

**Device Responds Slowly**

* **Symptom**: The response time is long and the device feels noticeably delayed.
* **Possible Causes**:

  1. Slow or unstable network connection

* **Solutions**:

  1. Check the network quality
  2. Try a different Wi-Fi network

Reset Device
------------------------------------------

If the issue cannot be resolved, you can try resetting the device.

**Network Reconfiguration (Xiaozhi Version)**

1. Press the ``boot`` button briefly during the initial startup and initialization process to re-enter network configuration mode.

**Reflash Firmware**

1. Follow the steps in :ref:`firmware_upload` to reflash the firmware
2. After reflashing, configure the network again
3. If the device was not unbound in the backend before reflashing, it may reconnect automatically to the previously bound agent

Contact Support
------------------------------------------

If the methods above do not solve your problem:

1. Send an email to technical support: `tech_edu_service@outlook.com <mailto:tech_edu_service@outlook.com>`_

When asking for help, please provide:

* a detailed description of the problem
* the steps that led to the problem
* the solutions you have already tried
