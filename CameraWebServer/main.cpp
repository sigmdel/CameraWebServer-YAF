// Content of CameraWebServer.ino
// Using a .cpp extension removes warning about C/C++ IntelliSense not supporting
// .ino files in PlatformIO

#include <Arduino.h>
#include "esp_camera.h"
#include <WiFi.h>
#include "config.h"
#include "camera_pins.h"
#if defined(CONFIG_MDNS_ADVERTISE_ENABLED)
  #include "ESPmDNS.h"
#endif

void startCameraServer();

void setup() {
  Serial.begin(CONFIG_BAUD);
  Serial.setDebugOutput(true);
  Serial.println();
  Serial.printf("SDK version: %s\n", ESP.getSdkVersion());

  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
#if !defined(NO_GRAB_MODE)
  config.grab_mode = CAMERA_GRAB_LATEST;   // https://github.com/espressif/arduino-esp32/issues/5805#issuecomment-951861112
  Serial.println("Camera buffer grab mode set to latest image");
#endif

  // if PSRAM IC present, init with UXGA resolution and higher JPEG quality
  //                      for larger pre-allocated frame buffer.
  if(psramFound()){
    config.frame_size = FRAMESIZE_UXGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }

#if defined(CAMERA_MODEL_ESP_EYE)
  pinMode(13, INPUT_PULLUP);
  pinMode(14, INPUT_PULLUP);
#endif

  // camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  sensor_t * s = esp_camera_sensor_get();
  // initial sensors are flipped vertically and colors are a bit saturated
  if (s->id.PID == OV3660_PID) {
    s->set_vflip(s, 1); // flip it back
    s->set_brightness(s, 1); // up the brightness just a bit
    s->set_saturation(s, -2); // lower the saturation
  }

/*
// moved to config.h
#if defined(CAMERA_MODEL_M5STACK_WIDE) || defined(CAMERA_MODEL_M5STACK_ESP32CAM)
  s->set_vflip(s, 1);
  s->set_hmirror(s, 1);
#endif
*/

#if defined(CONFIG_V_FLIP)
  s->set_vflip(s, 1);
#endif
#if defined(CONFIG_H_MIRROR)
  s->set_hmirror(s, 1);
#endif

#if defined(CONFIG_ESP_FACE_DETECT_ENABLED)
  #if defined(CONFIG_DEFAULT_RESOLUTION) && (CONFIG_DEFAULT_RESOLUTION < FRAMESIZE_CIF)
  s->set_framesize(s, CONFIG_DEFAULT_RESOLUTION);
  #else
  s->set_framesize(s, FRAMESIZE_QVGA);
  #endif
#elif defined(CONFIG_DEFAULT_RESOLUTION)
  s->set_framesize(s, CONFIG_DEFAULT_RESOLUTION);
#endif
#if defined(CONFIG_DEFAULT_QUALITY)
  s->set_quality(s, CONFIG_DEFAULT_QUALITY);
#endif

#if defined(CONFIG_STATIC_IP_ENABLED)
  IPAddress staticIP;
  IPAddress subnet;
  IPAddress gateway;
  IPAddress dns1;
  IPAddress dns2;

  int iperr = 0;
  if (!staticIP.fromString(CONFIG_STATICIP)) {
    Serial.println("Invalid CONFIG_STATICIP");
    iperr++;
  }
  if (!subnet.fromString(CONFIG_SUBNET)) {
    Serial.println("Invalid CONFIG_SUBNET");
    iperr++;
  }
  if (!gateway.fromString(CONFIG_GATEWAY)) {
    Serial.println("Invalid CONFIG_GATEWAY");
    iperr++;
  }
#if defined(CONFIG_DNS1)
  if (!dns1.fromString(CONFIG_DNS1)) {
    Serial.println("Invalid CONFIG_DNS1");
    iperr++;
  }
#else
  dns1 = gateway;
#endif
#if defined(CONFIG_DNS2)
  if (!dns2.fromString(CONFIG_DNS2)) {
    Serial.println("Invalid CONFIG_DNS2");
    iperr++;
  }
#else
  dns2 = gateway;
#endif

  if (!iperr) {
   if (!WiFi.config(staticIP, gateway, subnet, dns1, dns2)) {
      Serial.println("Static IP configuration failed.");
   }
  }
#endif // defined(CONFIG_STATIC_IP_ENABLED)

  WiFi.begin(CONFIG_WIFI_SSID, CONFIG_WIFI_PWD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

#if defined(CONFIG_MDNS_ADVERTISE_ENABLED)
  String mdnsHostname;
  #if defined(CONFIG_LOCAL_HOSTNAME)
    if (MDNS.begin(CONFIG_LOCAL_HOSTNAME)) {
     mdnsHostname = CONFIG_LOCAL_HOSTNAME;
  #else
    if (MDNS.begin("esp32-cam")) {
     mdnsHostname = "esp32-cam";
  #endif
    MDNS.addService("http", "tcp", 80);
    Serial.println("mDNS advertising started");
  } else {
    Serial.println("Error starting mDNS");
  }
#endif // defined(CONFIG_MDNS_ADVERTISE_ENABLED)

  startCameraServer();

#if defined(CONFIG_LED_ILLUMINATOR_ENABLED)
  ledcSetup(CONFIG_LED_LEDC_CHANNEL, CONFIG_FLASH_PWM_FREQ, CONFIG_FLASH_PWM_BITS);
  ledcAttachPin(CONFIG_FLASH_LED, CONFIG_LED_LEDC_CHANNEL);
  Serial.println("Flash LED configured.");
#endif // defined(CONFIG_LED_ILLUMINATOR_ENABLED)

  Serial.println("Camera Ready!");
  Serial.print("Use 'http://");
  Serial.print(WiFi.localIP());
#if defined(CONFIG_MDNS_ADVERTISE_ENABLED)
  if (mdnsHostname.length() > 0) {
    Serial.print("' or 'http://");
    Serial.print(mdnsHostname);
    Serial.print(".local");
  }
#endif // defined(CONFIG_MDNS_ADVERTISE_ENABLED)
  Serial.println("' to connect.");

#if defined(CONFIG_SHOW_NETWORK_PARAMS)
  Serial.print("Gateway: ");
  Serial.println(WiFi.gatewayIP());
  Serial.print("Subnet mask: ");
  Serial.println(WiFi.subnetMask());
  Serial.print("DNS server 1: ");
  Serial.println(WiFi.dnsIP(0));
  Serial.print("DNS server 2: ");
  Serial.println(WiFi.dnsIP(1));
#endif // defined(CONFIG_SHOW_NETWORK_PARAMS)

}

void loop() {
  // put your main code here, to run repeatedly:
  delay(10000);
}
