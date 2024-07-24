#include <DHTesp.h>
#include <WiFi.h>
#include <PubSubClient.h>

#define pinDHT 15
#define WiFiAP "Wokwi-GUEST"
#define WiFiPassword ""
#define mqttServer "test.mosquitto.org"

DHTesp dhtSensor;
WiFiClient espClient;
PubSubClient client(espClient);

unsigned long lastMsg = 0;

void initWiFi() {
  Serial.println("Menghubungkan ke WiFi...");
  WiFi.begin(WiFiAP, WiFiPassword);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nTerhubung ke WiFi!");
  Serial.println("IP address: " + WiFi.localIP().toString());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void reconnect() {
  while (!client.connected()) {
    Serial.println("Menghubungkan ke server...");
    String clientId = "ESP32Client";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())) {
      Serial.println("Terhubung ke server!");
    } else {
      Serial.println("Gagal terhubung ke server!");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  dhtSensor.setup(pinDHT, DHTesp::DHT22);
  initWiFi();
  client.setServer(mqttServer, 1883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  unsigned long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    TempAndHumidity data = dhtSensor.getTempAndHumidity();
    float temp = data.temperature;
    float hum = data.humidity;

    Serial.println("Temperature: " + String(temp) + "°C");
    Serial.println("Humidity: " + String(hum) + "%");
    Serial.println("-----");

    String tempStr = String(temp) + "°C";
    String humStr = String(hum) + "%";

    client.publish("Suhu", tempStr.c_str());
    client.publish("Kelembapan", humStr.c_str());
  }
}
