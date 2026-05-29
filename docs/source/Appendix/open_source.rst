.. _open_source:

Open Source and Secondary Development
=====================================

The Xiaozhi AI solution used in the LAFVIN ESP32S3 AIChatBot is open source and suitable for secondary development.

If you only want to use the device, you do not need to build the source code yourself. You can simply follow the online flashing tutorial and complete the network configuration steps.

Who This Is For
---------------

This section is useful for:

* developers who want to modify the firmware behavior
* makers who want to connect additional hardware modules
* students who want to learn from a complete ESP32-S3 AI voice project
* users who want to customize prompts, interaction logic, or other features

Source Code Entry
-----------------

You can start from the following resources:

* LAFVIN open source branch: `LAFVIN-AI-Chatbot (src branch) <https://github.com/lafvintech/LAFVIN-AI-Chatbot/tree/src>`_
* Xiaozhi upstream project: `xiaozhi-esp32 <https://github.com/78/xiaozhi-esp32>`_

The LAFVIN ``src`` branch contains the open source code currently provided for this project. The Xiaozhi upstream project is the main reference for source-level development and feature expansion.

What You Can Customize
----------------------

Depending on your development goals, you can extend or modify items such as:

* wake word behavior
* display content and UI flow
* prompt logic and assistant behavior
* audio interaction flow
* peripheral support and hardware integration
* backend or service integration

Recommended Path
----------------

We recommend the following workflow:

1. First, complete the standard online flashing process and confirm that the device works normally.
2. After the hardware, network, and backend are working, review the source code and development documentation.
3. Then begin your own feature modification or secondary development.

.. note::
   The ChatGPT setup described in this documentation is intended primarily for ready-to-use configuration. The open source and secondary development path in this project currently focuses on the Xiaozhi AI solution.
