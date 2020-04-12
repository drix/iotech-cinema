#include <ESP8266HTTPClient.h>

int ewelink_toggle(const char *device) {
  WiFiClient client;
  HTTPClient http;
  int httpCode = 0;
  String param = String(device);
  param.replace(" ", "%20");
  
  Mosquito_log("[HTTP] begin... http://192.168.1.117:3000/toggle?device=" + param + "\n");
  if (http.begin(client, "http://192.168.1.117:3000/toggle?device=" + param)) {  // HTTP

    // start connection and send HTTP header
    httpCode = http.GET();

    // httpCode will be negative on error
    if (httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      Serial.printf("[HTTP] GET... code: %d\n", httpCode);

      // file found at server
      if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
        String payload = http.getString();
        Serial.println(payload);
      }
    } else {
      Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
  } else {
    Serial.printf("[HTTP} Unable to connect\n");
  }
  return httpCode ;
}
