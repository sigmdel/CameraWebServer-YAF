#ifndef ARDUINO_INI
#define ARDUINO_INI

#if !defined(PLATFORMIO)

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
//#define CONFIG_MDNS_ADVERTISE_ENABLED           // Optional. If mDNS is enabled, the default hostname is "esp32-cam.local"
//#define CONFIG_LED_ILLUMINATOR_ENABLED          // Optional. Enables the flash LED. Do not define if there is no flash LED
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

#endif // !defined(PLATFORMIO)

#endif // #ifndef ARDUINO_INI
