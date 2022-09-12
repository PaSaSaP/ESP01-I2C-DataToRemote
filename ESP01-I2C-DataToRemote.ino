#include <Wire.h>
#include "DebugSerial.h"

void setup() {
  xSerialBegin(115200);

  Wire.begin(2, 0);
  Wire.setTimeout(100);

  wifiSetup();

  xDebugLn("Setup done");
}

uint32_t currentTime, triggerTime = 0;
uint8_t buf[32];

void loop() {
  currentTime = millis();
  if (!wifiLoop()) {
    return;
  }

  if (currentTime - triggerTime < 33) {
    return;
  }
  triggerTime = currentTime;

  auto w = Wire.requestFrom(0x11, sizeof(buf));
  if (w > 0) {
    w = Wire.readBytes(buf, w);
    if ((buf[0] == 0 || buf[0] == 0xFF) && buf[1] == 0xFF && buf[2] == 0xFF && buf[3] == 0xFF) {
      return;
    }
  }
  auto lastIdx = w - 1;
  while (lastIdx > 0 && buf[lastIdx] == 0xFF) {
    lastIdx--;
  }
  if (lastIdx > 0 && buf[lastIdx] == 0x00) {
    lastIdx--;
  }
  if (lastIdx == 0) {
    return;
  }
  wifiSend(0xA1, buf, lastIdx + 1);
}

