#include <M5Cardputer.h>
#include <HardwareSerial.h>

HardwareSerial SerialLoRa(2);
String rxBuffer = "";
bool lastOKFlag = false;
bool p2pReady = false;

void setup() {
  Serial.begin(115200);
  auto cfg = M5.config();
  M5Cardputer.begin(cfg);
  M5.Display.wakeup();
  M5.Display.setTextSize(1);
  M5.Display.setCursor(0, 0);
  M5.Display.printf("Mesh Public TX\n");
  delay(1000);
  Serial.println("=== RUI4.0.6 P2P for anrijs.lv Meshlog (EU868 868.1 SF12) ===");
  Serial.println("PTP=14 PWR, SF12 robust, full msg loop. Outdoor LOS.");

  SerialLoRa.begin(115200, SERIAL_8N1, 1, 2);
  delay(2000);
  flushBuffer();

  // ATR
  probeCmd("ATR");

  // AT
  probeCmd("AT");

  // BAND=4 EU868 ISM
  probeCmd("AT+BAND=4");

  // NWM P2P
  probeCmd("AT+NWM=?");
  probeCmd("AT+NWM=0");
  if (lastOK()) {
    delay(3000);
    probeCmd("AT+NWM=?");
    p2pReady = lastOK();
  }

  if (p2pReady) {
    // Public mesh params (868.1 ISM ch1, SF12 range 5km+)
    probeCmd("AT+PFREQ=868100000");  // Hz ch1 public
    probeCmd("AT+PSF=12");  // SF12 robust LOS
    probeCmd("AT+PBW=125");  // BW125kHz
    probeCmd("AT+PCR=1");  // CR4/5
    probeCmd("AT+PTP=14");  // PWR 14dBm EU max (range boost)
    M5.Display.printf("Public ready. Auto tx 10s\n");
    Serial.println("Setup OK - tx for meshlog filter 868.1 SF12 BW125");
  } else {
    M5.Display.printf("P2P fail\n");
  }
}

void loop() {
  M5.update();

  static unsigned long lastTx = 0;
  if ((millis() - lastTx > 10000) && p2pReady) {  // Auto tx 10s EU duty
    String hexMsg = "48656C6C6F206D6573636F72652074657374";  // "Hello mescore test" hex unique public
    probeCmd("AT+PSEND=" + hexMsg);
    if (lastOK()) {
      M5.Display.printf("Public TX OK! Mesh refresh\n");
      Serial.println("PSEND OK +EVT - public channel 868.1 SF12 14dBm");
    } else {
      M5.Display.printf("TX FAIL\n");
    }
    lastTx = millis();
  }

  // BtnA manual tx (override auto)
  if (M5.BtnA.wasReleased() && p2pReady) {
    lastTx = millis() - 10000;  // Trigger next auto
    M5.Display.printf("Manual tx now\n");
  }

  // Rx poll for 2nd node
  if (SerialLoRa.available()) {
    String line = SerialLoRa.readStringUntil('\n');
    Serial.println("Rx: " + line);
    M5.Display.printf("Rx: %s\n", line.c_str());
    if (line.indexOf("+EVT:") >= 0 || line.indexOf("+RECV") >= 0) {
      int pos = line.indexOf("DATA:") + 5;
      if (pos > 4) {
        String hexData = line.substring(pos);
        String msg = hexToStr(hexData);
        Serial.println("Public rx: " + msg);
        M5.Display.printf("Rx: %s\n", msg.c_str());
      }
    }
    flushBuffer();
  }

  delay(100);
}

// Probe
void probeCmd(String cmd) {
  flushBuffer();
  delay(500);
  sendAT(cmd);
  readResp(3000);
  bool ok = hasOK();
  Serial.println(cmd + " -> " + (ok ? "OK" : "FAIL: " + rxBuffer));
  M5.Display.printf("%s: %s\n", cmd.substring(0,10).c_str(), ok ? "OK" : "FAIL");
  lastOKFlag = ok;
}

bool lastOK() { return lastOKFlag; }

void sendAT(String cmd) {
  SerialLoRa.print(cmd + "\r\n");
  Serial.println("Sent: " + cmd);
  delay(200);
}

void readResp(int timeoutMs) {
  rxBuffer = "";
  unsigned long start = millis();
  while (millis() - start < timeoutMs) {
    if (SerialLoRa.available()) {
      String line = SerialLoRa.readStringUntil('\n');
      rxBuffer += line + "\n";
      Serial.println("Line: " + line);
      if (line.indexOf("OK") >= 0 || line.indexOf("ERROR") >= 0 || line.indexOf("NOT_FOUND") >= 0) return;
    }
    delay(10);
  }
}

bool hasOK() { return rxBuffer.indexOf("OK") >= 0; }

void flushBuffer() {
  while (SerialLoRa.available()) SerialLoRa.read();
  rxBuffer = "";
}

String hexToStr(String hex) {
  String str = "";
  for (size_t i = 0; i < hex.length(); i += 2) {
    if (i + 1 < hex.length()) {
      String byteStr = hex.substring(i, i + 2);
      char c = (char) strtol(byteStr.c_str(), NULL, 16);
      if (c >= 32 && c <= 126) str += c;
    }
  }
  return str;
}
