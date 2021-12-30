#ifndef CONFIG_H
#define CONFIG_H

#include <esp_camera.h>
#include "core_version.h" // to set grab_mode in latest ESP32 arduino core

/*********************************************************************************
Supported camera models

 WARNING! Select the correct board definition in the Arduino IDE or
          set the correct default_envs in platformio.ini and define the correct
          [env:xxx] if needed.

 WARNING! PSRAM is required for UXGA resolution and high JPEG quality otherwise
          only partial images will be transmitted if they exceed the buffer size.
*********************************************************************************/

// Select one and only one module
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
  #define CONFIG_DEFAULT_RESOLUTION FRAMESIZE_SVGA   // 800x600
  #define CONFIG_DEFAULT_QUALITY 4
  //#define CONFIG_V_FLIP
  //#define CONFIG_H_MIRROR
#endif


// common configuration

#define CONFIG_BAUD 115200                        // PlatformIO: set monitor_speed to the same value
#define CONFIG_WIFI_SSID "your-ssid"              // Mandatory
#define CONFIG_WIFI_PWD  "your-password"          // Mandatory

//#define CORE_DEBUG_LEVEL ARDUHAL_LOG_LEVEL_INFO // Optional. Default is ARDUHAL_LOG_LEVEL_ERROR see https://thingpulse.com/esp32-logging/
//#define CONFIG_STATIC_IP_ENABLED                // Optional. If not defined the IP is obtained from the DHCP server
//#define CONFIG_SHOW_NETWORK_PARAMS              // Optional
//#define CONFIG_MDNS_ADVERTISE_ENABLED           // Optional. If mDNS is enabled, the default hostname is "esp32-cam.local"
//#define CONFIG_ESP_FACE_DETECT_ENABLED          // Optional. Works at low resolution <= 320x240
//#define CONFIG_ESP_FACE_RECOGNITION_ENABLED     // Optional. Works at low resolution <= 320x240

#if defined(ARDUINO_ESP32_RELEASE_2_0_0) || defined(ARDUINO_ESP32_RELEASE_2_0_2)
#define HAS_GRAB_MODE
#endif

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
#if !defined(CONFIG_LED_LEDC_CHANNEL)
  #error "Must specify LEDC channel"
#endif
#endif

// sanity checks
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

#endif // #ifndef CONFIG_H
