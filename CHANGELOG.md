# CameraWebServer-YAF Change Log

## January 6, 2021 (current) - Release v1.2.2

- Updated the documentation 
- Added use of flash in .BMP image capture
- Added `CONFIG_BMP_CAPTURE_DISABLED` flag
- Moved `USE_EASYTARGET_LED_INTENSITY_SCALING` to `config.h` renaming it `CONFIG_EASYTARGET_INTENSITY_SCALING`
- Removed `CameraWebServer-YAF-deprectated` repository

### December 31, 2021 (e1d5ee8) - version 1.2.1 

- Improved setting of the camera `grab_mode` which will work with versions 1.0.6 and up of the ESP32-Arduino core.
- Added Change log.

## December 31, 2021 (146ac75) - Release v1.2.0, 

- Updated documentation for the unified PlatformIO and Arduino Project.
- Fixed bug with V-flip and H-mirror settings at startup.

### December 30, 2021 (06cf283)

- The configuration file `ini.hpp` renamed `config.h` and it is now used in the PlatformIO project as well as the Arduino sketch which means that build flags in `platformio.ini` have been removed.

### December 27, 2021 (ba7c3e4)

- Documented that the problem with the flash in Arduino was actually a problem with the way images were pulled from the image buffer in newer versions of the camera library. 


## December 26, 2021 (c63c0c9) - Release v1.1.0 

- Initial commit of the project reorganized to be compatible with both the Arduino and PlatormIO environments.
- Added the `ini.hpp` to mimic the `platformio.ini` configuration in the Arduino IDE.
