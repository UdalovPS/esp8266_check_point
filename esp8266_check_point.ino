#include <ESP8266WiFi.h>

#define BLUE_LEG D2
#define YELLOW_LEG D0
#define BLUE_TUMB D3
#define YELLOW_TUMB D5

const char* ssid ="RT-GPON-1138";
const char* password="HMK63RVX";

const char* host = "192.168.0.6";
uint16_t port = 14141;


byte neitral_point = 10;
byte blue_point = 11;
byte yellow_point = 12;
byte *mark;

unsigned long tmp_time = 0;

void setup() {
  pinMode(BLUE_LEG, OUTPUT);
  pinMode(YELLOW_LEG, OUTPUT);
  pinMode(BLUE_TUMB, INPUT_PULLUP);
  pinMode(YELLOW_TUMB, INPUT_PULLUP);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  delay(10);
  wifi_connect();
}

void loop() {
  boolean yellow = !digitalRead(YELLOW_TUMB);
  boolean blue = !digitalRead(BLUE_TUMB);

  WiFiClient client;

  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    wifi_connect();
  }

//  if (!client.connect(host, port)) {
//    Serial.println("Connect False");
//    delay(2000);
//  }

  if (blue == 1) {
    digitalWrite(BLUE_LEG, HIGH);
    mark = &blue_point;
  }
  else if (yellow == 1) {
    digitalWrite(YELLOW_LEG, HIGH);
    mark = &yellow_point;
    }
  else {
    digitalWrite(BLUE_LEG, LOW);
    digitalWrite(YELLOW_LEG, LOW);
    mark = &neitral_point;
  }
  if (millis() - tmp_time > 1000) {
    if (!client.connect(host, port)) {
      Serial.println("Connect False");
      delay(2000);
    }
     Serial.println(*mark);
     if (client.connected()) {
      client.print(*mark);
      client.stop();
     }
     tmp_time = millis();
  }
}

void wifi_connect() {
  Serial.println("");
  Serial.println("Start_to connect");
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    digitalWrite(BLUE_LEG, HIGH);
    digitalWrite(YELLOW_LEG, HIGH);
    Serial.println("Connection Failed! Rebooting...");
//    delay(5000);
    ESP.restart();
  }
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}
