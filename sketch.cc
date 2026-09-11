#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "Wokwi-GUEST";
const char* password = "";
const char* mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;

const char* topic_gas_val = "esp32/gas/nilai";
const char* topic_gas_status = "esp32/gas/status";
const char* topic_gas_presentase = "esp32/gas/persen";

#define GAS_PIN 34
#define BUZZER_PIN 14
#define LED_HEARTBEAT 2

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;

void setup_wifi(){
  delay(10);
  Serial.print("Koneksi ke wifi...");
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWifi berhasil terkoneksi!");
}

void reconnect(){
  while(!client.connected()){
    Serial.print("Koneksi ke HiveMQ...");
    String clientId = "ESP32GasNode-";
    clientId += String(random(0xffff), HEX);
    if(client.connect(clientId.c_str())){
      Serial.println("HiveMQ terkoneksi!");
    } else {
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_HEARTBEAT, OUTPUT);

  noTone(BUZZER_PIN);
  digitalWrite(LED_HEARTBEAT, LOW);

  analogReadResolution(12);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
}

void loop() {
  if(!client.connected()){
    reconnect();
  }
  client.loop();
  unsigned long now = millis();

  if (now - lastMsg > 3000) {
    lastMsg = now;
    int gasVal = analogRead(GAS_PIN);
    String statusGas = "";
    if (gasVal < 1000) {
      statusGas = "NORMAL";
      noTone(BUZZER_PIN);
    } else if (gasVal <= 2500) {
      statusGas = "WASPADA";
      tone(BUZZER_PIN, 1000);
      delay(100);
      noTone(BUZZER_PIN);
    } else {
      for(i = 0; i < 3; i++ ){
        statusGas = "BAHAYA GAS";
        tone(BUZZER_PIN, 2000);
        delay(80);
        noTone(BUZZER_PIN);
      }
    }
    
    float persentase = ((float)gasVal / 4095.0) * 100.0;
    char gasStr[8];
    char persenStr[10]
    
    itoa(gasVal, gasStr, 10);
    dtostrtf(persentase, 4,2, persenStr);
    
    client.publish(topic_gas_val, gasStr);
    client.publish(topic_gas_status, statusGas.c_str());
    client.publish(topic_gas_presentase, persenStr);
    
    digitalWrite(LED_HEARTBEAT, HIGH);
    delay(100);
    digitalWrite(LED_HEARTBEAT, LOW);

    Serial.printf("Sensor Gas: %s/4095 | Status: [%s] | Presentase: [%s]\n", gasStr, statusGas.c_str(), persenStr);
  }
}