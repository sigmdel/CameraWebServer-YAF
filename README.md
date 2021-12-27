# ESP32 CameraWebServer *YAF*

### What everyone was waiting for: *Y*et *A*nother *F*ork of the [CameraWebServer](https://github.com/espressif/arduino-esp32/tree/master/libraries/ESP32/examples/Camera/CameraWebServer) example by Espressif.

This (December 26, 2021) rebuild version of the repository is better structured to make it easier to support both the PlatformIO and Arduino programming environments.

The web server, capable of streaming video from an OV2640 or OV7670 OmniVision Technologies camera, runs on an ESP32. To use the higher resolutions supported by the camera, PSRAM must be available. Supported devices include the Espressif ESP-EYE, some M5Stack and TTGO models with camera, and the Ai Thinker ESP32-CAM and its many clones.

# Table of Contents 
<!-- TOC -->

- [1. Changes to the Original Example](#1-changes-to-the-original-example)
  - [1.1. Warning](#11-warning)
- [2. The PlatformIO Project Configuration File](#2-the-platformio-project-configuration-file)
  - [2.1. Setting the Correct Wi-Fi Credentials - Mandatory](#21-setting-the-correct-wi-fi-credentials---mandatory)
  - [2.2. Choosing the Correct Board - Mandatory](#22-choosing-the-correct-board---mandatory)
  - [2.3. Changing the Log Level - Optional](#23-changing-the-log-level---optional)
  - [2.4. Assigning a Static IP Address  - Optional](#24-assigning-a-static-ip-address----optional)
  - [2.5. Enabling a mDNS Local Host Name - Optional](#25-enabling-a-mdns-local-host-name---optional)
  - [2.6. Enabling the Flash LED - Optional](#26-enabling-the-flash-led---optional)
  - [2.7. Enabling Face Detection and Recognition - Optional](#27-enabling-face-detection-and-recognition---optional)
  - [2.8. Notes](#28-notes)
- [3. Arduino Project](#3-arduino-project)
  - [3.1. A Configuration File for the Arduino Project](#31-a-configuration-file-for-the-arduino-project)
  - [3.2. Note](#32-note)
- [4. Usage](#4-usage)
- [5. References](#5-references)
- [6. License](#6-license)

<!-- /TOC -->


## 1. Changes to the Original Example

The Oct. 11, 2021 [source from Espressif](https://github.com/espressif/arduino-esp32/tree/master/libraries/ESP32/examples/Camera/CameraWebServer) has been slightly changed.

  1. Optionally the log level can be changed to see more messages.
  1. Optionally a static IP address can be set.
  1. Optionally the IP address of the gateway, subnet mask and dns servers can be displayed in addition to the IP address of the ESP32.
  1. Optionally mDNS advertising can be enabled.
  1. Optionally the flash LED can be enabled.
  1. Optionally face detection and face recognition can be enabled.
  1. Added a `CAMERA_MODEL_CUSTOM_CAM` definition in `camera_pins.h` to handle new ESP32 camera boards.
  1. Added a configuration file `ini.hpp` for the Arduino IDE environment that mimics the `platformio.ini` file.
  1. No editing of the program source is necessary to compile it. All options and settings including the camera model and Wi-Fi credentials are done in the project configuration file which will be 
       1. `platformio.ini` in PlaformIO, or
       1. `ini.hpp`in the Arduino IDE.
  
Two changes not directly related to the above were made 
 
  1. Set the `grab_mode` to `CAMERA_GRAB_LATEST` in the camera configuration struct in `CameraWebServer.ino`. 
  1. The addition of an `#include "stdlib_noniso.h"` line at the start of the `app_httpd.cpp` to get rid of a spurious error report in **PlatformIO** about `itoa` being an unknown function.

The second of these changes is not necessary, but the first is important. It will avoid a lot of frustration with ESP32-CAM users wanting to use the flash function and it will remove the unreliability of the still image capture function. In other words, it should clear the problem incorrectly attributed to a bug introduced in the newer versions of the SDK in section [2.6. Enabling the flash LED - Optional](#26-enabling-the-flash-led---optional).


### 1.1. Warning

The `grab_mode` parameter was added to the camera configuration structure only in the latest version of the ESP32 Arduino core (version 2.0.2) which is not yet used in the stable PlatformIO espressif32 Arduino platform. A test for version of the ESP32 core is therefore done before setting its value.

```c++
#if defined(ARDUINO_ESP32_RELEASE_2_0_2)
  config.grab_mode = CAMERA_GRAB_LATEST;   // https://github.com/espressif/arduino-esp32/issues/5805#issuecomment-951861112
#endif
```

This is brittle and is sure to break when a newer version of the core is used in the Arduino esp32 core or in the PlatformIO espressif32 Arduino platform.



## 2. The PlatformIO Project Configuration File

Before compiling the program, at least three build flags in the project configuration file `platformio.ini` must be set correctly and an environment definition for the board being used must be provided. The following file has two board definitions, one for the AI Thinker ESP32-CAM and another for a custom board.

```
; PlatformIO Project Configuration File

; Supported camera models
;
; WARNING! A PSRAM IC is required for UXGA resolution and high JPEG quality
; otherwise only partial images will be transmitted if they exceed the buffer size
;
; CAMERA_MODEL_WROVER_KIT         has PSRAM
; CAMERA_MODEL_ESP_EYE            has PSRAM
; CAMERA_MODEL_AI_THINKER         has PSRAM
; CAMERA_MODEL_M5STACK_PSRAM      has PSRAM (M5Camera version A?)
; CAMERA_MODEL_M5STACK_V2_PSRAM   has PSRAM (M5Camera version B?)
; CAMERA_MODEL_M5STACK_WIDE       has PSRAM (M5CameraF? )
; CAMERA_MODEL_M5STACK_ESP32CAM   no PSRAM
; CAMERA_MODEL_M5STACK_UNITCAM    no PSRAM
; CAMERA_MODEL_TTGO_T_JOURNAL     no PSRAM
; CAMERA_MODEL_CUSTOM_CAM         must also define pin assignments, see [env:custom]
;
; The camera model must be specified in the build_flags of each board.


[platformio]
default_envs = esp32cam

[extra]
baud=115200
build_flags =
  -D CONFIG_BAUD=${extra.baud}                 ; Mandatory.
  -D CONFIG_WIFI_SSID=\"my\-ssid\              ; Mandatory.
  -D CONFIG_WIFI_PWD=\"my\-password\"          ; Mandatory
  ;-D CORE_DEBUG_LEVEL=ARDUHAL_LOG_LEVEL_INFO  ; Optional. Default is ARDUHAL_LOG_LEVEL_ERROR see https://thingpulse.com/esp32-logging/
  ;-D CONFIG_STATIC_IP_ENABLED                 ; Optional. If not defined the IP is obtained from the DHCP server
  ;-D CONFIG_STATICIP=\"192.168.1.27\"         ;  - Mandatory if CONFIG_STATIC_IP_ENABLED is defined
  ;-D CONFIG_SUBNET=\"255.255.255.0\"          ;  - Mandatory if CONFIG_STATIC_IP_ENABLED is defined
  ;-D CONFIG_GATEWAY=\"192.168.1.1\"           ;  - Mandatory if CONFIG_STATIC_IP_ENABLED is defined
  ;-D CONFIG_DNS1=\"8.8.8.8\"                  ;  - Optional DNS server if CONFIG_STATIC_IP_ENABLED. The default is the GATEWAY
  ;-D CONFIG_DNS2=\"8.8.8.4\"                  ;  - Optional DNS server if CONFIG_STATIC_IP_ENABLED. The default is the GATEWAY
  ;-D CONFIG_SHOW_NETWORK_PARAMS               ; Optional.
  ;-D CONFIG_MDNS_ADVERTISE_ENABLED            ; Optional. If mDNS is enabled, the default hostname is "esp32-cam.local"
  ;-D CONFIG_LOCAL_HOSTNAME=\"esp32\-cam\-01\" ; Optional. Custom local hostname. Here "esp32-cam-01.local"  
  ;-D CONFIG_LED_ILLUMINATOR_ENABLED           ; Optional. Enables the flash LED
  ;-D CONFIG_FLASH_PWM_FREQ=50000              ;  - Mandatory if CONFIG_LED_ILLUMINATOR_ENABLED defined. Flash LED PWM frequency
  ;-D CONFIG_FLASH_PWM_BITS=9                  ;  - Mandatory if CONFIG_LED_ILLUMINATOR_ENABLED defined. Resolution of duty cycle counter
  ;-D CONFIG_LED_MAX_INTENSITY=100             ;  - Mandatory if CONFIG_LED_ILLUMINATOR_ENABLED defined. A percentage (0..100) of full intensity  
  ;-D CONFIG_LED_LEDC_CHANNEL=LEDC_CHANNEL_7   ;  - Mandatory if CONFIG_LED_ILLUMINATOR_ENABLED defined. Channel < 8 and > 0 used by camera
  ;-D CONFIG_ESP_FACE_DETECT_ENABLED           ; Optional. Works at low resolution <= 320x240
  ;-D CONFIG_ESP_FACE_RECOGNITION_ENABLED      ; Optional. Works at low resolution <= 320x240

[env]
platform = espressif32
framework = arduino
monitor_speed = ${extra.baud}

[env:esp32cam]
board = esp32cam
build_flags = ${extra.build_flags}
  -D CAMERA_MODEL_AI_THINKER
  -D CONFIG_FLASH_LED=4

[env:custom]
board = esp32dev                  ; A generic ESP32 board
build_flags = ${extra.build_flags}
  -D BOARD_HAS_PSRAM              ; with PSRAM on board, but there is a problem with PSRAM on pre-revision 3 ESP32 chips, add the following fix
  -mfix-esp32-psram-cache-issue   ; Not needed with ESP32 revision 3. See https://github.com/espressif/esp-idf/issues/2892#issuecomment-754547748
  -D CAMERA_MODEL_CUSTOM_CAM
  -D CONFIG_CAMERA_PIN_PWDN=32    ; These macros are mandatory with the CAMERA_MODEL_CUSTOM
  -D CONFIG_CAMERA_PIN_RESET=-1   ; They map GPIO pins to the camera pins
  -D CONFIG_CAMERA_PIN_XCLK=0     ; - ditto
  -D CONFIG_CAMERA_PIN_SIOD=26    ; - ditto
  -D CONFIG_CAMERA_PIN_SIOC=27    ; - ditto
  -D CONFIG_CAMERA_PIN_Y9=35      ; - ditto
  -D CONFIG_CAMERA_PIN_Y8=34      ; - ditto
  -D CONFIG_CAMERA_PIN_Y7=39      ; - ditto
  -D CONFIG_CAMERA_PIN_Y6=36      ; - ditto
  -D CONFIG_CAMERA_PIN_Y5=21      ; - ditto
  -D CONFIG_CAMERA_PIN_Y4=19      ; - ditto
  -D CONFIG_CAMERA_PIN_Y3=18      ; - ditto
  -D CONFIG_CAMERA_PIN_Y2=5       ; - ditto
  -D CONFIG_CAMERA_PIN_VSYNC=25   ; - ditto
  -D CONFIG_CAMERA_PIN_HREF=23    ; - ditto
  -D CONFIG_CAMERA_PIN_PCLK=22    ; - ditto
```

There should be no problem with the default `monitor_speed` of 115200 bits/second. If another baud is desired then change its value in 

```
[extra]
baud=115200
```
and the ESP32 serial port will be initialized with the correct speed.

The default environment is for the AI Thinker ESP32-CAM. It can be changed in the `[platformio]` section at the start of the configuration file.

### 2.1. Setting the Correct Wi-Fi Credentials - Mandatory

The first two build flags define the credentials of the Wi-Fi network to which the ESP32 will connect. The name (SSID) and password (PWD) are quoted strings. See `[env:string_defines]` in the Examples section of [Build Options](https://docs.platformio.org/en/latest/projectconf/section_env_build.html) for indications about the correct way of formatting string defines.

### 2.2. Choosing the Correct Board - Mandatory

An environment for the ESP32 board being used must be defined in the configuration file. In addition to identifying the board, the CAMERA_MODEL_xxxx must be specified. This selects a predefined assignment of GPIO pins to the camera pins found in `camera_pins.h`. That is all there is to using any one of the predefined boards. As an example, this is the AI Thinker ESP32-CAM definition:

```
[env:esp32cam]
board = esp32cam
build_flags = ${env.build_flags}
  -D CAMERA_MODEL_AI_THINKER
  -D CONFIG_FLASH_LED=4  
```

Note how most of the build flags are common to all boards so they are defined in the common environment `[env]`, but they must also be explicitly added to the specific board build flags. 

The list of supported ESP32 camera modules is almost out of date, with a number of models no longer being produced, while newly available boards are not in it. For such a board, choose the custom environment. 

```
[platformio]
default_envs = custom
```

and then adjust the GPIO pin assignment in `[env:custom]` section. It would be nice to have a newer ESP32 camera module to thoroughly test this approach. As set up, this custom definition corresponds to the ESP32-CAM and it does work with that board.

### 2.3. Changing the Log Level - Optional

The firmware prints a log to the serial monitor. By default only messages with a log level equal to `ARDUHAL_LOG_LEVEL_ERROR` or highter are shown. It can be useful to set the log level to a lower level. Setting the level to `ARDUHAL_LOG_LEVEL_INFO` helped in working out how the LED illuminator worked.

### 2.4. Assigning a Static IP Address  - Optional

To assign a static IP address to the ESP32 camera module, uncomment the `CONFIG_STATIC_IP_ENABLED` flag and set the static device IP address, gateway IP address, subnet mask and IP address of the DNS servers. If either of those is not defined then the gateway IP address will be used instead.

If the default gateway, subnet mask and current DNS servers are not known, leave `CONFIG_STATIC_IP_ENABLED` commented, uncomment the  `CONFIG_SHOW_NETWORK_PARAMS` flag and upload the firmware to the ESP32 board. The current values will be displayed on the serial monitor.

### 2.5. Enabling a mDNS Local Host Name - Optional      

Uncomment the `CONFIG_MDNS_ADVERTISE_ENABLED` flag to enable mDNS. The web server may then be reachable with the following URL `http://esp32-cam.local/`. The local host name can be changed by defining the `CONFIG_LOCAL_HOSTNAME` macro. Do not append `.local` to the value. 

Either one of the  `CONFIG_STATIC_IP_ENABLES` and `CONFIG_STATIC_IP_ENABLED` directives makes it easier to connect to the web server when there's no longer a serial connection to the ESP32 which displays its IP address. They can be enabled together. 

### 2.6. Enabling the Flash LED - Optional

Uncomment the `CONFIG_LED_ILLUMINATOR_ENABLED` build flag to enable the high intensity flash LED found on some ESP32 camera boards. The intensity of the flash is controlled by a pulse-width modulation (PWM) signal from one of the eight high-speed hardware LEDC channels. The `CONFIG_LED_LEDC_CHANNEL=LEDC_CHANNEL_7` macro selects which channel to use, but remember that channel 0 is used by the camera.  The `CONFIG_FLASH_PWM_FREQ` macro defines the frequency of the PWM signal.

The resolution of the duty cycle counter is set with the `CONFIG_FLASH_BITS` macro. A 9-bit value means that the duty cycle can be any value from 0 to 511 (=2^9-1). At full intensity, the flash LED is quite bright and generates a considerable amount of heat. The `CONFIG_LED_MAX_INTENSITY` macro defines a percentage used to reduce the actual length of the duty cycle. Setting this value  lower than 100 will reduce the risk of damaging the LED.

The GPIO pin used to drive the flash LED must be defined in the board's environment. The flash LED is controlled by GPIO 4 on the AI Thinker ESP32-CAM. That is why the `[env:esp32cam]` contains the `-D CONFIG_FLASH_LED=4` build flag.

~~Currently, this addition does not work in the Arduino IDE. It appears to be a problem with the initialization of the LEDC channel. Is this because the ESP32 SDK used in Arduino is version v4.4-dev-3569-g6a7d83af19-dirty while PlatformIO is still at v3.3.5-1-g85c43024c?~~ There was no problem with the initialization of the LEDC channel, the problem was with the choice of the function to set the duty cycle. However, it is necessary to set the grab_mode to CAMERA_GRAB_LATEST in the camera configuration struct in CameraWebServer.ino as explained in [1. Changes to the Original Example](#1-changes-to-the-original-example). Otherwise, the flash will appear not to work at all and the capture of still images will not work reliably in newer versions of the SDK as found in the current Arduino esp32 core. 

### 2.7. Enabling Face Detection and Recognition - Optional

The last two directives merely make visible compile options found in the `app_httpd.cpp` source file. Face recognition requires that face detection be enabled. The web interface states that these functions work when the resolution of the video stream is CIF (400x296) or lower. With the AI Thinker ESP32-CAM, it turns out that these functions only work at QVGA (320x240) or lower resolution.

To recycle a much abused Samuel Johnson quote: “Sir, [an ESP32's face recognition] is like a dog's walking on his hind legs. It is not done well; but you are surprised to find it done at all.” 

### 2.8. Notes

If no optional build flag is set, then compiling the project will produce the same firmware as the original Arduino project by Espressif.

If the project is built in the PlatformIO environment, it is best to rename `CameraWebServer.ino` to `CameraWebServer.cpp` or even `main.cpp`.

## 3. Arduino Project

The `CameraWebServer/src` directory, containing the following five files

    app_httpd.cpp
    camera_pins.h
    camera_index.h
    CameraWebServer.ino
    ini.hpp,
  
is a complete Arduino project. The first four files are modified versions of the original example project, while the new file `ini.hpp` mimics the `platformio.ini` configuration file.

### 3.1. A Configuration File for the Arduino Project

```
#ifndef ARDUINO_INI
#define ARDUINO_INI

#if !defined(CONFIG_IS_PLATFORMIO)

// Select one and only one board definition
//
#define ESP32_CAM
//#define CUSTOM_CAM


// common

#define CONFIG_BAUD 115200                        // Mandatory.
#define CONFIG_WIFI_SSID "my-ssid"                // Mandatory.
#define CONFIG_WIFI_PWD  "my-password"            // Mandatory
//#define CORE_DEBUG_LEVEL ARDUHAL_LOG_LEVEL_INFO // Optional. Default is ARDUHAL_LOG_LEVEL_ERROR see https://thingpulse.com/esp32-logging/
//#define CONFIG_STATIC_IP_ENABLED                // Optional. If not defined the IP is obtained from the DHCP server
//#define CONFIG_SHOW_NETWORK_PARAMS              // Optional.
#define CONFIG_MDNS_ADVERTISE_ENABLED             // Optional. If mDNS is enabled, the default hostname is "esp32-cam.local"
#define CONFIG_LED_ILLUMINATOR_ENABLED            // Optional. Enables the flash LED. Do not define if there is no flash LED
//#define CONFIG_ESP_FACE_DETECT_ENABLED          // Optional. Works at low resolution <= 320x240
//#define CONFIG_ESP_FACE_RECOGNITION_ENABLED     // Optional. Works at low resolution <= 320x240

#if defined(CONFIG_STATIC_IP_ENABLED)
#define CONFIG_STATICIP "192.168.1.27"            //  - Mandatory if CONFIG_STATIC_IP_ENABLED is defined
#define CONFIG_SUBNET "255.255.255.0"             //  - Mandatory if CONFIG_STATIC_IP_ENABLED is defined
#define CONFIG_GATEWAY "192.168.1.1"              //  - Mandatory if CONFIG_STATIC_IP_ENABLED is defined
#define CONFIG_DNS1 "8.8.8.8"                     //  - Optional DNS server if CONFIG_STATIC_IP_ENABLED. The default is the GATEWAY
#define CONFIG_DNS2  "8.8.8.4"                    //  - Optional DNS server if CONFIG_STATIC_IP_ENABLED. The default is the GATEWAY
#endif

#if defined(CONFIG_MDNS_ADVERTISE_ENABLED)
//#define CONFIG_LOCAL_HOSTNAME esp32-cam-01"     // Optional. Custom local hostname. Here "esp32-cam-01.local"
#endif

#if defined(CONFIG_LED_ILLUMINATOR_ENABLED)
#define CONFIG_FLASH_PWM_FREQ 50000               //   - Mandatory if CONFIG_LED_ILLUMINATOR_ENABLED defined. Flash LED PWM frequency
#define CONFIG_FLASH_PWM_BITS 9                   //   - Mandatory if CONFIG_LED_ILLUMINATOR_ENABLED defined. Resolution of duty cycle counter
#define CONFIG_LED_MAX_INTENSITY 100              //   - Mandatory if CONFIG_LED_ILLUMINATOR_ENABLED defined. A percentage (0..100) of full intensity
#define CONFIG_LED_LEDC_CHANNEL LEDC_CHANNEL_7    //   - Mandatory if CONFIG_LED_ILLUMINATOR_ENABLED defined. Channel < 8 and > 0 used by camera
#endif

// Board specific configuration

#if defined(ESP32_CAM)
#define CAMERA_MODEL_AI_THINKER
#define CONFIG_FLASH_LED 4
#endif

#if defined(CUSTOM_CAM)
//#define BOARD_HAS_PSRAM            // with PSRAM on board, but there is a problem with PSRAM on pre-revision 3 ESP32 chips, add the following fix
//-mfix-esp32-psram-cache-issue      // Not needed with ESP32 revision 3. See https://github.com/espressif/esp-idf/issues/2892#issuecomment-754547748
#define CAMERA_MODEL_CUSTOM_CAM
#define CONFIG_CAMERA_PIN_PWDN=32    // These macros are mandatory with the CAMERA_MODEL_CUSTOM,
#define CONFIG_CAMERA_PIN_RESET=-1   // they map GPIO pins to the camera pins
#define CONFIG_CAMERA_PIN_XCLK=0     // - ditto
#define CONFIG_CAMERA_PIN_SIOD=26    // - ditto
#define CONFIG_CAMERA_PIN_SIOC=27    // - ditto
#define CONFIG_CAMERA_PIN_Y9=35      // - ditto
#define CONFIG_CAMERA_PIN_Y8=34      // - ditto
#define CONFIG_CAMERA_PIN_Y7=39      // - ditto
#define CONFIG_CAMERA_PIN_Y6=36      // - ditto
#define CONFIG_CAMERA_PIN_Y5=21      // - ditto
#define CONFIG_CAMERA_PIN_Y4=19      // - ditto
#define CONFIG_CAMERA_PIN_Y3=18      // - ditto
#define CONFIG_CAMERA_PIN_Y2=5       // - ditto
#define CONFIG_CAMERA_PIN_VSYNC=25   // - ditto
#define CONFIG_CAMERA_PIN_HREF=23    // - ditto
#define CONFIG_CAMERA_PIN_PCLK=22    // - ditto
#if defined(CONFIG_LED_ILLUMINATOR_ENABLED)
#define CONFIG_FLASH_LED 4
#endif  //
#endif  // defined(CUSTOM_CAM)

#endif // !defined(CONFIG_IS_PLATFORMIO)

#endif // #ifndef ARDUINO_INI
```

Instead of defining build flags for each board, this file is included at the start of two files `CameraWebServer.ino` and `app_https.cpp`. The various macros defined within are basically the same as the build flags in `platformio.ini` and do not need to be explained any furhter.

### 3.2. Note



The `CameraWebServer-arduino-1-1-0.zip` archive found in release 1.1.0 contains just the files needed for the Arduino project which is the content of the `CameraWebServer` directory.


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


## 6. License

Since this project is clearly derivative work, it can be assumed that the license under which Espressif released **CameraWebServer** applies to the [four downloaded files](https://github.com/espressif/arduino-esp32/tree/master/libraries/ESP32/examples/Camera/CameraWebServer). The only copyright notice in those four files is in `app_httpd.cpp`. It explicitly states that the **Apache License, Version 2.0** applies. However, there is no mention of the other files. The only other mention of a license is in the root directory of the [Arduino core for the ESP32, ESP32-S2 and ESP32-C](https://github.com/espressif/arduino-esp32) repository containing the four source files. There it states that the **LGPL-2.1 License** applies. So **which of the two licenses applies to each source file of this project is not entirely clear**. 

As for any original work found in this repository, it is released under the **BSD Zero Clause** [SPDX](https://spdx.dev/): [0BSD](https://spdx.org/licenses/0BSD.html)) licence.
