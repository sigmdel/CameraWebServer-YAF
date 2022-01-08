# ESP32 CameraWebServer *YAF*

### What everyone was waiting for: *Y*et *A*nother *F*ork of the [CameraWebServer](https://github.com/espressif/arduino-esp32/tree/master/libraries/ESP32/examples/Camera/CameraWebServer) example by Espressif.

### Version 1.2.2

This version of the `CameraWebServer`, based on the October 21, 2021 Espressif code, will compile in the Arduino and the PlatformIO environments. It is compatible with versions 1.0.6, 2.0.0, 2.0.1 and 2.0.2 of the [arduino-esp32](https://github.com/espressif/arduino-esp32) core. Note that facial detection and recognition will only work with versions 1.0.6 and 2.0.0 of the core.

The web server, capable of streaming video from an OV2640 or OV7670 OmniVision Technologies camera, runs on an ESP32. To use the higher resolutions supported by the camera, PSRAM must be available. Supported devices include the Espressif ESP-EYE, some M5Stack and TTGO models with camera, and the Ai Thinker ESP32-CAM and its many clones.

# Table of Contents 
<!-- TOC -->

- [1. Changes to the Original Example](#1-changes-to-the-original-example)
- [2. The `config.h` Configuration File](#2-the-configh-configuration-file)
  - [2.1. Choosing the Correct Board - Mandatory](#21-choosing-the-correct-board---mandatory)
  - [2.2. Setting the Correct Wi-Fi Credentials - Mandatory](#22-setting-the-correct-wi-fi-credentials---mandatory)
  - [2.3. Disabling BMP Image Capture - Optional](#23-disabling-bmp-image-capture---optional)
  - [2.4. Changing the Log Level - Optional](#24-changing-the-log-level---optional)
  - [2.5. Assigning a Static IP Address  - Optional](#25-assigning-a-static-ip-address----optional)
  - [2.6. Enabling a mDNS Local Host Name - Optional](#26-enabling-a-mdns-local-host-name---optional)
  - [2.7. Enabling Face Detection and Recognition - Optional](#27-enabling-face-detection-and-recognition---optional)
  - [2.8. Flash LED Options](#28-flash-led-options)
- [3. Further Notes](#3-further-notes)
- [4. Usage](#4-usage)
- [5. References](#5-references)
- [6. License](#6-license)

<!-- /TOC -->


## 1. Changes to the Original Example

The original intent was to change the Oct. 11, 2021 [source from Espressif](https://github.com/espressif/arduino-esp32/tree/master/libraries/ESP32/examples/Camera/CameraWebServer) in the least amount possible. But in the end quite a few little things were modified.

  1. Optionally the log level can be changed to see more information messages.
  1. Optionally a static IP address can be set.
  1. Optionally the IP address of the gateway, subnet mask and DNS servers can be displayed in addition to the IP address of the ESP32.
  1. Optionally mDNS advertising can be enabled.
  1. Optionally the flash LED can be enabled.
  1. Optionally face detection and face recognition can be enabled (with some restrictions).
  1. Optionally the ability to capture still images to the BMP format can be disabled.
  1. Added using the flash LED when capturing a still image to the BMP format.
  1. Added a `CAMERA_MODEL_CUSTOM_CAM` definition in `camera_pins.h` to handle new ESP32 camera boards.
  1. Added a configuration file `config.h` to manage all these changes. All options and settings including the camera model and Wi-Fi credentials are done in that file so that there is no need to edit the program source.
  
Other changes not directly related to the above were made. Here are the more substantial ones.
 
  1. `CameraWebServer.ino` was gutted and its content moved to `main.cpp`. That makes both the Arduino IDE and PlatformIO environment happy.
  1. Set the `grab_mode` to `CAMERA_GRAB_LATEST` in the camera configuration struct in `CameraWebServer.ino` when appropriate (see [4. Further Notes](#4-further-notes)). 
  1. The addition of an `#include "stdlib_noniso.h"` line at the start of the `app_httpd.cpp` to get rid of a spurious error report in **PlatformIO** about `itoa` being an unknown function.
  1. Use of the flash was added to image capture to the .BMP format in `app_httpd.cpp`.
  1. For some unknown reason, all the `#if SOME_DIRECTIVE` in `app_httpd.cpp` had to be changed to either `#ifdef(SOME_DIRECTIVE)` or `#ifdef SOME_DIRECTIVE`. The latter was chosen.


## 2. The `config.h` Configuration File

Here is a partial listing of the configuration file with those aspects that are most important. 

```
#ifndef CONFIG_H
#define CONFIG_H

#include <esp_camera.h>
#include "core_version.h" // to set grab_mode in latest ESP32 arduino core

... 

// Select one and only one module
// ------------------------------

//#define CAMERA_MODEL_WROVER_KIT         //has PSRAM
//#define CAMERA_MODEL_ESP_EYE            //has PSRAM
#define CAMERA_MODEL_AI_THINKER           //has PSRAM
//#define CAMERA_MODEL_M5STACK_PSRAM      //has PSRAM (M5Camera version A?)
//#define CAMERA_MODEL_M5STACK_V2_PSRAM   //has PSRAM (M5Camera version B?)
//#define CAMERA_MODEL_M5STACK_WIDE       //has PSRAM (M5CameraF? )
//#define CAMERA_MODEL_M5STACK_ESP32CAM   //no PSRAM
//#define CAMERA_MODEL_M5STACK_UNITCAM    //no PSRAM
//#define CAMERA_MODEL_TTGO_T_JOURNAL     //no PSRAM
//#define CAMERA_MODEL_CUSTOM_CAM

// Board specific configuration
// ----------------------------

#if defined(CAMERA_MODEL_AI_THINKER)
  // Flash LED configuration
  #define CONFIG_FLASH_LED 4
  #define CONFIG_LED_LEDC_CHANNEL  LEDC_CHANNEL_7   // Channel 0 is used by camera

  // camera settings at startup
  #define CONFIG_DEFAULT_RESOLUTION FRAMESIZE_SVGA  // 800x600
  #define CONFIG_DEFAULT_QUALITY 4
  //#define CONFIG_V_FLIP
  //#define CONFIG_H_MIRROR
#endif

#if defined(CAMERA_MODEL_CUSTOM_CAM)
  // custom camera pin configuration
... 
#endif

// Common configuration
// --------------------

#define CONFIG_WIFI_SSID "your-ssid"              // Mandatory
#define CONFIG_WIFI_PWD  "your-password"          // Mandatory
#define CONFIG_BAUD 115200                        // PlatformIO: set monitor_speed to the same value

//#define CORE_DEBUG_LEVEL ARDUHAL_LOG_LEVEL_INFO // Optional. Default is ARDUHAL_LOG_LEVEL_ERROR see https://thingpulse.com/esp32-logging/
//#define CONFIG_BMP_CAPTURE_DISABLED             // Optional. Default is to enable single shot image capture to .BMP format
//#define CONFIG_STATIC_IP_ENABLED                // Optional. If not defined the IP is obtained from the DHCP server
//#define CONFIG_SHOW_NETWORK_PARAMS              // Optional
//#define CONFIG_MDNS_ADVERTISE_ENABLED           // Optional. If mDNS is enabled, the default hostname is "esp32-cam.local"
//#define CONFIG_ESP_FACE_DETECT_ENABLED          // Optional. Works at low resolution <= 320x240
//#define CONFIG_ESP_FACE_RECOGNITION_ENABLED     // Optional. Works at low resolution <= 320x240
... 
```

With this default configuration, the project will compile in the PlatformIO and the Arduino development environments. However it will not run correctly.

### 2.1. Choosing the Correct Board - Mandatory

The correct ESP32 camera module must be chosen. For most boards that will mean uncommenting the correct `CAMERA_MODEL_XXXX` directive. Of course, only one such directive can be defined. 

The ESP32-CAM has a flash LED. To enable its use the I/O pin used to control the flash and the LEDC channel used to generate the PMW signal are defined in its board specific configuration block. One could also want to change the initial camera resolution and image quality and control if the image is flipped horizontally and vertically. Again this is done in the board-specific configuration block. 

Currently the file does not have many board-specific configuration blocks, but it is always possible to add another board-specific block or to modify one. Here is the block for the ESP32-CAM by AI Thinker.

```
#if defined(CAMERA_MODEL_AI_THINKER)
  // Camera settings at startup
  #define CONFIG_DEFAULT_RESOLUTION FRAMESIZE_SVGA  // 800x600
  #define CONFIG_DEFAULT_QUALITY 4
  //#define CONFIG_V_FLIP
  //#define CONFIG_H_MIRROR

  // Flash LED configuration
  #define CONFIG_FLASH_LED 4
  #define CONFIG_LED_LEDC_CHANNEL  LEDC_CHANNEL_7   // Channel 0 is used by camera
#endif
```

It is possible to change the default values of four camera settings as can be seen above. If, like the ESP32-CAM, the board has a high intensity LED that can be used as a flash then the GPIO pin connected to the LED and the LEDC channel used to generate the PWM signal must be specified here. There are more details about the [optional flash LED below](#27-flash-led-options).


The list of supported boards is rather old, many of the boards in it are no longer being produced. There may be new boards on the market with different connections to the camera. If that's the case, select the `CAMERA_MODEL_CUSTOM_CAM` model and modify the assignment of I/O pin to the camera as needed in the custom camera.

```
#if defined(CAMERA_MODEL_CUSTOM_CAM) // custom camera pin configuration
  #define CONFIG_CAMERA_PIN_PWDN  32
  #define CONFIG_CAMERA_PIN_RESET -1
  #define CONFIG_CAMERA_PIN_XCLK  0
  #define CONFIG_CAMERA_PIN_SIOD  26
  #define CONFIG_CAMERA_PIN_SIOC  27
  #define CONFIG_CAMERA_PIN_Y9    35
  #define CONFIG_CAMERA_PIN_Y8    34
  #define CONFIG_CAMERA_PIN_Y7    39
  #define CONFIG_CAMERA_PIN_Y6    36
  #define CONFIG_CAMERA_PIN_Y5    21
  #define CONFIG_CAMERA_PIN_Y4    19
  #define CONFIG_CAMERA_PIN_Y3    18
  #define CONFIG_CAMERA_PIN_Y2    5
  #define CONFIG_CAMERA_PIN_VSYNC 25
  #define CONFIG_CAMERA_PIN_HREF  23
  #define CONFIG_CAMERA_PIN_PCLK  22
#endif
```

**WARNINGS** 

  1. Choosing the correct camera module here is not all that is required. In the Arduino IDE, the correct ESP32 board must be selected. The equivalent in PlatformIO is to specify the correct board in the `platformio.ini` file.

  2. It would be nice to have newer ESP32 camera modules to thoroughly test the `CAMERA_MODEL_CUSTOM_CAM` approach to handling them. As set up, this custom definition corresponds to the ESP32-CAM and it does work with that board.

  3. This project has been tested with a single ESP32-CAM, and one probably not made by AI Thinker at that.

### 2.2. Setting the Correct Wi-Fi Credentials - Mandatory

The Wi-Fi macros `CONFIG_WIFI_SSID` and `CONFIG_WIFI_PWD` must be correctly defined otherwise, the ESP32 will be forever trying to connect to a non-existent Wi-Fi network. That will be obvious in the serial monitor where all that will be happening is the continuous printing of '.' at each connection attempt.

### 2.3. Disabling BMP Image Capture - Optional

Some may want to remove the ability to capture single shot images in .BMP format by defining the `CONFIG_BMP_CAPTURE_DISABLED` directive. 

Note that capturing .BMP images can only be done with HTML queries and cannot be done the web interface. Furthermore, the implementation is not the same as the .JPEG capture; even if enabled, face detection and recognition rectangles will not be performed. This implementation does add use of the flash during .BMP capture just as done during .JPEG capture.


### 2.4. Changing the Log Level - Optional

The firmware prints a log to the serial monitor. By default only messages with a log level equal to `ARDUHAL_LOG_LEVEL_ERROR` or highter are shown. It can be useful to set the log level to a lower level. Setting the level to `ARDUHAL_LOG_LEVEL_INFO` helped in working out how the LED illuminator worked.

### 2.5. Assigning a Static IP Address  - Optional

To assign a static IP address to the ESP32 camera module, uncomment the `CONFIG_STATIC_IP_ENABLED` directive. Further down in the `config.h` file, set the desired static device IP address and the correct gateway IP address, subnet mask and IP address of the DNS servers. If either of those is not defined then the gateway IP address will be used instead.

```
#if defined(CONFIG_STATIC_IP_ENABLED)
#define CONFIG_STATICIP "192.168.0.77"
#define CONFIG_SUBNET "255.255.255.0"
#define CONFIG_GATEWAY "192.168.0.1"
#define CONFIG_DNS1 "8.8.8.8"       //   Optional first DNS server, the default is the GATEWAY
#define CONFIG_DNS2 "8.8.8.4"       //   Optional second DNS server, the default is the GATEWAY
#endif
```

If the default gateway, subnet mask and current DNS servers are not known, leave `CONFIG_STATIC_IP_ENABLED` commented, uncomment the  `CONFIG_SHOW_NETWORK_PARAMS` and upload the firmware to the ESP32 board. The current values will be displayed on the serial monitor.

### 2.6. Enabling a mDNS Local Host Name - Optional      

Uncomment the `CONFIG_MDNS_ADVERTISE_ENABLED` directive to enable mDNS. The web server may then be reachable with the following URL `http://esp32-cam.local/`. The local host name can be changed by further down in the configuration file defining the `CONFIG_LOCAL_HOSTNAME` macro. 

```
#if defined(CONFIG_MDNS_ADVERTISE_ENABLED)
  //#define CONFIG_LOCAL_HOSTNAME "esp32-cam-01"     // Replaces the default "esp32-cam"
#endif
```

This would be useful if there is more than one ESP32 camera module running this software. Do not append `.local` to the value. 

Either one of the  `CONFIG_STATIC_IP_ENABLES` and `CONFIG_STATIC_IP_ENABLED` directives makes it easier to connect to the web server when there's no longer a serial connection to the ESP32 which displays its IP address. They can be enabled together. 

### 2.7. Enabling Face Detection and Recognition - Optional

*“Sir, [an ESP32's face recognition] is like a dog's walking on his hind legs. It is not done well; but you are surprised to find it done at all.”*  Samuel Johnson (1709-1784).

That is a rather unfair, as both face recognition and identification do work well, but only at QVGA (320x240) or lower resolution, which does considerably reduce the practical use of that functionality. These are enabled by uncommenting the `CONFIG_ESP_FACE_DETECT_ENABLED` and `CONFIG_ESP_FACE_RECOGNITION_ENABLED` directives.

**WARNING** 

Currently, face detection and hence face recognition will not work with the most recent versions of the [`arduino-esp32`](https://github.com/espressif/arduino-esp32/) core. The `fd_forward.h` and `fr_forward.h` headers and other required files are no longer included, starting with version 2.0.1 of the core. They have been replaced with a completely different facial recognition system, but the `app_httpd.cpp` in the `CameraWebServer` example has not been updated to use the new system. Instead, Espressif added  tests for the directives `CONFIG_ESP_FACE_DETECT_ENABLED` and `CONFIG_ESP_FACE_RECOGNITION_ENABLED` while not defining them. In other words, facial detection and recognition are disabled.

In short, face detection and hence face recognition can be enabled in the PlatformIO environment because it still uses version 1.0.6 of the ESP32 Arduino core. In the Arduino environment, it will be necessary to use version 2.0.0. or 1.0.6 of the `esp32 boards` definition.


### 2.8. Flash LED Options

When a board has a high intensity LED that can be used as a photographic flash then its  intensity is controlled by a pulse-width modulation (PWM) signal from one of the eight high-speed hardware LEDC channels of the ESP32. Two macros, `CONFIG_FLASH_LED` and `CONFIG_LED_LEDC_CHANNEL` define the I/O port connected to the LED and the LEDC channel used to generate PWM signal. These are defined in the board specific block because these could be different on other boards. More parameters must be defined to configure the LEDC channel. When `CONFIG_FLASH_LED` macro is defined, then another four macros are defined.

```
#if defined(CONFIG_FLASH_LED)
  #define CONFIG_LED_ILLUMINATOR_ENABLED
  #define CONFIG_FLASH_PWM_FREQ    50000            // Flash LED PWM frequency
  #define CONFIG_FLASH_PWM_BITS    9                // Resolution of duty cycle counter
  #define CONFIG_LED_MAX_INTENSITY 100              // A percentage (0..100) of full intensity
  //#define CONFIG_EASYTARGET_INTENSITY_SCALING     // https://github.com/easytarget/esp32-cam-webserver
  ... 
```

The `CONFIG_LED_ILLUMINATOR_ENABLED` directive ensures that the flash is correctly configured in the `setup()` function. It also enables the automatic use of the flash when capturing still images or streaming video in `app_httpd.cpp`. That LEDC configuration requires setting the PWM frequency (here 50KHz) and the number of bits in the duty cycle counter (here 9). These default values can be changed if desired.

At full intensity, the flash LED is quite bright and generates a considerable amount of heat. The `CONFIG_LED_MAX_INTENSITY` macro defines a percentage used to reduce the actual length of the duty cycle. Setting this value lower than 100 will reduce the risk of damaging the LED.


## 3. Further Notes

The `CameraWebServer-arduino-x-x-x.zip` archive found in the corresponding  x.x.x release contains just the files needed for the Arduino project. In other words, it's a zip archive of the the `CameraWebServer` directory.

In version 2.0.0 of the ESP32 Arduino core, a new field was added to the `camera_config_t` structure defined in `esp_camera.h`. Called `grab_mode`, it controls how images are pulled from the image buffer. By default it is set to `CAMERA_GRAP_WHEN_EMPTY` which presumably means that an image capture is done only once the buffer is empty. Since the buffer holds two images by default, it could take up to three clicks of the `Get Still` button in the web interface before a picture is taken. The value is changed to `CAMERA_GRAB_LATEST` in `setup()` and now the `Get Still` button works as expected. Setting the grab mode is controlled by the `NO_GRAB_MODE` directive defined in `config.h`.

```
#if defined(ARDUINO_ESP32_RELEASE_1_0_6)     // Assuming ESP32-Arduino core version 1.0.6 or newer is used
  #define NO_GRAB_MODE
#endif
```

This sketch will be compatible with version 1.0.6 of the ESP32-Arduino core currently used in the stable Espressif 32 v3.4.0 platform in PlatformIO. At the same time, the project will be compatible with version 2.0.0 and newer of the ESP32-Arduino cores available in the Arduino IDE. It is assumed that a version of the ESP32-Arduino core older than version 1.0.6 will not be used.

## 4. Usage

Once the firmware has been uploaded to the ESP32 board, restart the board. If the Wi-Fi credentials are correctly defined, the web server will be on line after a short time and its IP address will be displayed in the serial terminal. However if a static IP address has been assigned to the board or if mDNS has been enabled, it should be easy to find the board even when there is no serial connection with the board.

Here are the various URLs used to enter the web admin page, view a video stream, capture an image and so on.

- Web control interface: `http://<cam-ip>:80/`
- View video stream: `http://<cam-ip>:81/stream` - only one client can be connected at a time.
- Capture JPEG snapshot: `http://<cam-ip>:80/capture`
- Capture BMP snapshot: `http://<cam-ip>:80/bmp`
- Obtain camera settings: `http://<cam-ip>:80/status`
- Change a single camera setting: `http://<cam-ip>:80/control?var=param&val=value` where `param` is any setting (`framesize`, `quality`, etc.) and `value` is its desired setting. See the output of the `status` request for a list of all the parameters.

Here `<cam-ip>` is the IP address of the ESP32 or its `.local` hostname if the OS and web browser support mDNS and the `CONFIG_MDNS_ADVERTISE_ENABLED` build flag was defined. Of course, the default `:80` HTTP port does not need to be explicitly included in the above URLs, but port `:81` must be included in the video stream URL.

## 5. References

There are simply too many to list. Just on [Github](https://github.com), 1,255 repositories are found when searching for *esp32 cam* and 21 when searching for *CameraWebServer* (as of 2021-12-13). Here is a small sample.

- [CameraWebServer](https://github.com/bertrik/CameraWebServer) is a similar project by Bertrik Sikken (bertrik). It uses WiFiSettings instead of hard-wired Wi-Fi credentials.

- [ESP32-CameraWebServer](https://github.com/thorsten-l/ESP32-CameraWebServer) is a "refactored version" of the project by Thorsten Ludewig (thorsten-l).

- [TinkerCameraWebServer](https://github.com/pmcg31/TinkerCameraWebServer) is a similar project by Rich (pmcg31) with the web server html files stored in SPIFFS instead of being zipped binary blobs. It also enables mDNS.

- [ESP32-cam-webserver-improved](https://github.com/Xitee1/ESP32-cam-webserver-improved) for a similar project by Xitee (Xitee1) with automatic WiFi reconnection, static IP, OTA and rudimentary control of the flash LED while removing face recognition.

- [ESP32-CAM example revisited](https://github.com/easytarget/esp32-cam-webserver) by Owen Carter (easytarget) is similar to the previous project. This author implements a working flash LED, but independently of the way the recent version of `app_http.cpp` from Espressif does it. The author does use a logarithmic scale to set the flash intensity which inspired the one used in this repository. Indeed, **many thanks to Owen Carter** for showing the way to handle the intensity of the flash with a  [LED Control (LEDC)](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/ledc.html").

There is a post about these modifications to the `CameraWebServer` example: [Le ESP32-CAM comme serveur vidéo](https://sigmdel.ca/michel/ha/esp8266/ESP32-CAM_02_fr.html). Currently it is only available in French. 

## 6. License

Since this project is clearly derivative work, it can be assumed that the license under which Espressif released **CameraWebServer** applies to the [four downloaded files](https://github.com/espressif/arduino-esp32/tree/master/libraries/ESP32/examples/Camera/CameraWebServer). The only copyright notice in those four files is in `app_httpd.cpp`. It explicitly states that the **Apache License, Version 2.0** applies. However, there is no mention of the other files. The only other mention of a license is in the root directory of the [Arduino core for the ESP32, ESP32-S2 and ESP32-C](https://github.com/espressif/arduino-esp32) repository containing the four source files. There it states that the **LGPL-2.1 License** applies. So **which of the two licenses applies to each source file of this project is not entirely clear**. 

As for any original work found in this repository, it is released under the **BSD Zero Clause** [SPDX](https://spdx.dev/): [0BSD](https://spdx.org/licenses/0BSD.html)) licence.
