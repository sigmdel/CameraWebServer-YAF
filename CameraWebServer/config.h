#ifndef CONFIG_H
#define CONFIG_H

#include "core_version.h" // to get the installed ESP32 arduino core version
#include "sensor.h"       // for framesize_t enumerated type to set the default camera resolution

/*********************************************************************************
Supported camera models

 WARNING! Select the correct board definition in the Arduino IDE or
          set the correct default_envs in platformio.ini and define the correct
          [env:xxx] if needed.

 WARNING! PSRAM is required for UXGA resolution and high JPEG quality otherwise
          only partial images will be transmitted if they exceed the buffer size.

  OV2640 supported image sizes           Additional sizes for 3MP Sensors
    FRAMESIZE_96X96,    // 96x96           FRAMESIZE_FHD,      // 1920x1080
    FRAMESIZE_QQVGA,    // 160x120         FRAMESIZE_P_HD,     //  720x1280
    FRAMESIZE_QCIF,     // 176x144         FRAMESIZE_P_3MP,    //  864x1536
    FRAMESIZE_HQVGA,    // 240x176         FRAMESIZE_QXGA,     // 2048x1536
    FRAMESIZE_240X240,  // 240x240
    FRAMESIZE_QVGA,     // 320x240       Additional sizes for 3MP Sensors
    FRAMESIZE_CIF,      // 400x296         FRAMESIZE_QHD,      // 2560x1440
    FRAMESIZE_HVGA,     // 480x320         FRAMESIZE_WQXGA,    // 2560x1600
    FRAMESIZE_VGA,      // 640x480         FRAMESIZE_P_FHD,    // 1080x1920
    FRAMESIZE_SVGA,     // 800x600         FRAMESIZE_QSXGA,    // 2560x1920
    FRAMESIZE_XGA,      // 1024x768
    FRAMESIZE_HD,       // 1280x720
    FRAMESIZE_SXGA,     // 1280x1024
    FRAMESIZE_UXGA,     // 1600x1200
*********************************************************************************/

// Select one and only one module         // Mandatory
//
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

#if defined(CAMERA_MODEL_WROVER_KIT)
  // Camera settings at startup
  //#define CONFIG_DEFAULT_RESOLUTION FRAMESIZE_SVGA  // 800x600
  //#define CONFIG_DEFAULT_QUALITY 4
  //#define CONFIG_V_FLIP
  //#define CONFIG_H_MIRROR
#endif

#if defined(CAMERA_ESP_EYE)
  // Camera settings at startup
  //#define CONFIG_DEFAULT_RESOLUTION FRAMESIZE_SVGA  // 800x600
  //#define CONFIG_DEFAULT_QUALITY 4
  //#define CONFIG_V_FLIP
  //#define CONFIG_H_MIRROR
#endif

#if defined(CAMERA_MODEL_M5STACK_WIDE) || defined(CAMERA_MODEL_M5STACK_ESP32CAM)
  // Camera settings at startup
  //#define CONFIG_DEFAULT_RESOLUTION FRAMESIZE_SVGA  // 800x600
  //#define CONFIG_DEFAULT_QUALITY 4
  #define CONFIG_V_FLIP
  #define CONFIG_H_MIRROR
#endif

#if defined(CAMERA_MODEL_AI_THINKER)
  #define LED_BUILTIN     33
  #define LED_BUILTIN_ON  LOW
  // Flash LED configuration
  #define CONFIG_FLASH_LED 4
  #define CONFIG_LED_LEDC_CHANNEL  LEDC_CHANNEL_7   // Channel 0 is used by camera

  // Camera settings at startup
  #define CONFIG_DEFAULT_RESOLUTION FRAMESIZE_SVGA  // 800x600
  #define CONFIG_DEFAULT_QUALITY 4
  //#define CONFIG_V_FLIP
  //#define CONFIG_H_MIRROR
#endif

#if defined(CAMERA_MODEL_CUSTOM_CAM)
  // custom camera pin configuration, ESP32-CAM configuration shown
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

  // Flash LED configuration
  //#define CONFIG_FLASH_LED 4
  //#define CONFIG_LED_LEDC_CHANNEL  LEDC_CHANNEL_7  // channel 0 is used by camera

  // camera settings at startup
  //#define CONFIG_DEFAULT_RESOLUTION FRAMESIZE_SVGA   // 800x600
  //#define CONFIG_DEFAULT_QUALITY 4
  //#define CONFIG_V_FLIP
  //#define CONFIG_H_MIRROR
#endif


// common configuration

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

#if defined(CONFIG_STATIC_IP_ENABLED)
  #define CONFIG_STATICIP "192.168.1.27"
  #define CONFIG_SUBNET "255.255.255.0"
  #define CONFIG_GATEWAY "192.168.1.1"
  #define CONFIG_DNS1 "8.8.8.8"                     //   Optional first DNS server, the default is the GATEWAY
  #define CONFIG_DNS2 "8.8.8.4"                     //   Optional second DNS server, the default is the GATEWAY
#endif

#if defined(CONFIG_MDNS_ADVERTISE_ENABLED)
  //#define CONFIG_LOCAL_HOSTNAME esp32-cam-01"     // Optional. Custom local hostname. Here "esp32-cam-01.local"
#endif

#if defined(CONFIG_FLASH_LED)
  #define CONFIG_LED_ILLUMINATOR_ENABLED
  #define CONFIG_FLASH_PWM_FREQ    50000            // Flash LED PWM frequency
  #define CONFIG_FLASH_PWM_BITS    9                // Resolution of duty cycle counter
  #define CONFIG_LED_MAX_INTENSITY 100              // A percentage (0..100) of full intensity
  //#define CONFIG_EASYTARGET_INTENSITY_SCALING     // https://github.com/easytarget/esp32-cam-webserver
  #if !defined(CONFIG_LED_LEDC_CHANNEL)
    #error "Must specify LEDC channel"
  #endif
#endif

// Nothing below should need change

#if defined(ARDUINO_ESP32_RELEASE_1_0_6)     // Assuming version 1.0.6 or newer ESP32-Arduino core is used
  #define NO_GRAB_MODE
#endif

// sanity checks
#if defined(CONFIG_STATIC_IP_ENABLED) && defined(CONFIG_SHOW_NETWORK_PARAMS)
  #warning "Do not define CONFIG_STATIC_IP_ENABLED if CONFIG_SHOW_NETWORK_PARAMS was defined to show the default gateway, subnet mask, etc."
#endif

#if defined(CONFIG_LED_LEDC_CHANNEL) && !defined(CONFIG_FLASH_LED)
  #error "Flash LED pin must be defined"
#endif

#if defined(CONFIG_ESP_FACE_RECOGNITION_ENABLED) && !defined(CONFIG_ESP_FACE_DETECT_ENABLED)
  #define CONFIG_ESP_FACE_DETECT_ENABLED
#endif

#if defined(CONFIG_LED_MAX_INTENSITY) && (CONFIG_LED_MAX_INTENSITY > 100)
  #undef CONFIG_LED_MAX_INTENSITY
  #define CONFIG_LED_MAX_INTENSITY 100
#endif

#if !defined(ARDUINO_ESP32_RELEASE_1_0_6) && !defined(ARDUINO_ESP32_RELEASE_2_0_0) && defined(CONFIG_ESP_FACE_DETECT_ENABLED)
  #error "The installed version of the ESP32-Arduino core does not contain the required face detection library"
#endif

#endif // #ifndef CONFIG_H
