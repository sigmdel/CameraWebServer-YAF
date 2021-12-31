# Change Log

# CameraServer-YAF

## December 31, 2021

### current

-
- Added Change log

## (146ac75, Release v1.2.0)

- Updated documentation for a unified PlatformIO and Arduino Project
- Fixed bug with V-flip and H-mirror settings at startup.


## December 30, 2021

### (06cf283)

- The configuration file `ini.hpp` renamed `config.h` and it is now used in the PlatformIO project as well as the Arduino sketch which means that build flags in `platformio.ini` have been removed.

## December 27, 2021

### (ba7c3e4)

- Documented that the problem with the flash in Arduino was actually a problem with the way images were pulled from the image buffer in newer versions of the camera library. 

## December 26, 2021

## (c63c0c9, Release v1.1.0)

- Initial commit of the project reorganized to be compatible with both the Arduino and PlatormIO environments.
- Added the `ini.hpp` to mimic the `platformio.ini` configuration in the Arduino IDE.

----

# CameraServer-YAF-deprecated

## December 26, 2021

### (0f7e753)

- Repository renamed `CameraServer-YAF-deprecated`. It is replaced with 
a better structured repository name `CameraServer-YAF`.

## December 23, 2021

### (f12a284)

- Report that the flash does not work when compiled in the Arduino IDE

## December 19, 2021


## (5c75e1f, Release v1.0.0)

- PWM duty cycle now set with an exponential transformation of the 0-255 LED Intensity web inferface 

## December 17, 2021

### (1dcb6e6)

- Changed mode of `CONFIG_LED_MAX_INTENSITY` from using it to set a ceiling to PWM duty cycle to a scaling of the duty cycle. Now `CONFIG_LED_MAX_INTENSITY` is a percentage.

## December 16, 2021

### (c4bb30d)

- Modified `enable_led()` in `app_httpd.cpp` so that the flash intensity is subject to `CONFIG_LED_MAX_INTENSITY` ceiling when taking still images as well as when streaming.
- Added `CONFIG_LED_ILLUMINATOR_ENABLED` flag and other needed flags to `platformio.ini` and LEDC configuration in `setup()`. The flash works.

### (d7bb6b4)

- Added first version of README.md.
- Face detection and recognition work, added  `CONFIG_ESP_FACE_DETECT_ENABLED` and `CONFIG_ESP_FACE_RECOGNITION_ENABLED` build flags to `platformio.ini`.
  

## December 15, 2021

### (6175647)

- Added optional static IP addressing and mDNS advertising.
- Moved WiFi settings and camera model selection to `platformio.ini`.
- Added `#include "stdlib_noniso.h"` to `app_httpd.cpp` to get rid of a spurious error message in PIO.

### (82e38e8)

- Created basic PlatformIO project.
- Copied original CameraWebServer source files into `src/` directory.
- Added basic `esp32cam` environment to simple `platformio.ini`.
- The project compiles and runs.

### (c5eac02)

- Creating bare GitHub repository.
