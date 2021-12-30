#ifndef CONFIG_H
#define CONFIG_H

#include <esp_camera.h>

/*********************************************************************************
Supported camera models

 WARNING! A PSRAM IC is required for UXGA resolution and high JPEG quality
 otherwise only partial images will be transmitted if they exceed the buffer size
*********************************************************************************/

// Select one and only one module
//
//#define CAMERA_MODEL_WROVER_KIT         //has PSRAM
//#define CAMERA_MODEL_ESP_EYE            //has PSRAM
//#define CAMERA_MODEL_AI_THINKER           //has PSRAM
//#define CAMERA_MODEL_M5STACK_PSRAM      //has PSRAM (M5Camera version A?)
//#define CAMERA_MODEL_M5STACK_V2_PSRAM   //has PSRAM (M5Camera version B?)
//#define CAMERA_MODEL_M5STACK_WIDE       //has PSRAM (M5CameraF? )
//#define CAMERA_MODEL_M5STACK_ESP32CAM   //no PSRAM
//#define CAMERA_MODEL_M5STACK_UNITCAM    //no PSRAM
//#define CAMERA_MODEL_TTGO_T_JOURNAL     //no PSRAM
#define CAMERA_MODEL_CUSTOM_CAM


// Board specific configuration

#if defined(CAMERA_MODEL_AI_THINKER)
  #define CONFIG_LED_ILLUMINATOR_ENABLED // Enables the flash LED. Do not define if there is no flash LED
  #if defined(CONFIG_LED_ILLUMINATOR_ENABLED)
    #define CONFIG_FLASH_LED 4
    #define CONFIG_LED_LEDC_CHANNEL  LEDC_CHANNEL_7  // Channel 1 to 7 possible, 0 is used by camera
  #endif
  #define CONFIG_DEFAULT_RESOLUTION FRAMESIZE_SVGA   // 800x600
  #define CONFIG_DEFAULT_QUALITY 4
  //#define CONFIG_V_FLIP
  //#define CONFIG_H_MIRROR
#endif

#if defined(CAMERA_MODEL_CUSTOM_CAM)
  //#define BOARD_HAS_PSRAM            // with PSRAM on board, but there is a problem with PSRAM on pre-revision 3 ESP32 chips, add the following fix
  //-mfix-esp32-psram-cache-issue      // Not needed with ESP32 revision 3. See https://github.com/espressif/esp-idf/issues/2892#issuecomment-754547748
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
  #if defined(CONFIG_LED_ILLUMINATOR_ENABLED)
    #define CONFIG_FLASH_LED 4
    #define CONFIG_LED_LEDC_CHANNEL  LEDC_CHANNEL_7  // Channel 1 to 7 possible, 0 is used by camera
  #endif
  //#define CONFIG_DEFAULT_RESOLUTION FRAMESIZE_SVGA   // 800x600
  //#define CONFIG_DEFAULT_QUALITY 4
  //#define CONFIG_V_FLIP
  //#define CONFIG_H_MIRROR
#endif


// common configuration

#define CONFIG_BAUD 115200                        // Mandatory.
#define CONFIG_WIFI_SSID "your-ssid"              // Mandatory.
#define CONFIG_WIFI_PWD  "your-password"          // Mandatory

//#define CORE_DEBUG_LEVEL ARDUHAL_LOG_LEVEL_INFO // Optional. Default is ARDUHAL_LOG_LEVEL_ERROR see https://thingpulse.com/esp32-logging/
//#define CONFIG_STATIC_IP_ENABLED                // Optional. If not defined the IP is obtained from the DHCP server
//#define CONFIG_SHOW_NETWORK_PARAMS              // Optional.
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

#if defined(CONFIG_LED_ILLUMINATOR_ENABLED)
#define CONFIG_FLASH_PWM_FREQ    50000            //   - Mandatory if CONFIG_LED_ILLUMINATOR_ENABLED defined. Flash LED PWM frequency
#define CONFIG_FLASH_PWM_BITS    9                //   - Mandatory if CONFIG_LED_ILLUMINATOR_ENABLED defined. Resolution of duty cycle counter
#define CONFIG_LED_MAX_INTENSITY 100              //   - Mandatory if CONFIG_LED_ILLUMINATOR_ENABLED defined. A percentage (0..100) of full intensity
#endif

#endif // #ifndef CONFIG_H
