#include <ESP8266WiFi.h>

namespace {
const char* ssid = "SHL6d30";
const char* password = "dg9ZCefJ";
const char* host = "pasasap.ovh"; //  "MacBook-Pro-Kamil.local";
// const char* host = "146.59.3.135"; //  "MacBook-Pro-Kamil.local";
// const char* host = "MacBook-Pro-Kamil.local"; //  "MacBook-Pro-Kamil.local";
const uint16_t port = 8069; // common for both rover and base
}  // anonymous namespace

class WifiConnect {
 public:
  void setup() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    clientCert.append(
#include "client.crt.h"
    );
    clientKey.parse(
#include "client.key.h"
    );
    // client.setInsecure();
    static const uint8_t fingerprint[20] = {0x75, 0x27, 0x82, 0xFD, 0x92, 0x32, 0xFE, 0xAB, 0xBA, 0x63, 0x53, 0x7B, 0xDE, 0x16, 0xAF, 0xBF, 0x97, 0xF2, 0xDA, 0x84};
    client.setFingerprint(fingerprint);
    client.setClientRSACert(&clientCert, &clientKey);
  }

  bool loop() {
    if (!waitForWifi()) {
      return false;
    }

    if (!connect()) {
      return false;
    }

    xDebugLn("sending...");
    String str = String(millis());
    send(0xA2, (uint8_t const*)str.c_str(), str.length());
    delay(500);
  
    return true;
  }

  size_t send(uint32_t msgId, uint8_t const* arr, size_t size) {
    auto result = client.write((uint8_t const*)&msgId, sizeof(msgId));
    result += client.write((uint8_t const*)&size, sizeof(size));
    result += client.write(arr, size);
    return result;
  }
  
 private:
  BearSSL::WiFiClientSecure client;
  BearSSL::X509List clientCert;
  BearSSL::PrivateKey clientKey;
  bool wifiOK = false;
  uint8_t buffer[256];

  bool waitForWifi() {
    // Wait for connection
    if (WiFi.status() != WL_CONNECTED) {
      xDebug(".");
      wifiOK = false;
      delay(250);
      return false;
    }
    if (!wifiOK) {
      wifiOK = true;
      debugPrintConnected();
    }
    return true;
  }

  bool connect() {
    return connect(client, host, port);
  }

  bool connect(BearSSL::WiFiClientSecure& client_, char const* host_, uint16_t port_) {
    if (!client_.connected()) {
      // Use WiFiClient class to create TCP connections
      if (!client_.connect(host_, port_)) {
        xDebug("connection failed to:"); xDebug(host_); xDebug(":"); xDebug(port_); xDebugLn("");
        delay(250);
        return false;
      }
    }
    return true;
  }

  void closeConnection() {
    client.stop();
  }

  void debugPrintConnected() {
    xDebugLn("");
    xDebug("Connected to ");
    xDebugLn(ssid);
    xDebug("IP address: ");
    xDebugLn(WiFi.localIP());
  }
};

WifiConnect wifiConnect;

void wifiSetup() { wifiConnect.setup(); }

bool wifiLoop() { return wifiConnect.loop(); }

size_t wifiSend(uint32_t msgId, uint8_t const* arr, size_t size) {
  return wifiConnect.send(msgId, arr, size); 
}