#include <WiFi.h>
#include <MQTT.h>

WiFiClient net;
MQTTClient client;

const char ssid[] = "Wokwi-GUEST";
const char pass[] = "";

const int PIN_MERAH = 25;
const int PIN_HIJAU = 26;
const int PIN_BIRU = 27;
const int PIN_SENSOR = 33;

int jumlahPengunjung = 0;

void connect(){
  rgb(1,0,0);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
  }
  rgb(0,1,0);

  while(!client.connect("perpustakaan-masuk", "kelasiot", "passwordkelasiot")){
    delay(500);
  }
  rgb(0,0,1);
  client.subscribe("kelasiot/pengunjung/perpustakaan", 1);
}

void setup() {
  pinMode(PIN_MERAH, OUTPUT);
  pinMode(PIN_HIJAU, OUTPUT);
  pinMode(PIN_BIRU, OUTPUT);
  pinMode(PIN_SENSOR, INPUT_PULLUP);

  WiFi.begin(ssid, pass);
  client.begin("kelasiot.cloud.shiftr.io", net);

  client.onMessage(subscribe);

  connect();
}

void loop() {
  client.loop();

  bool sensor = digitalRead(PIN_SENSOR);
  if(sensor == 0){ // 0 berarti ada pengunjung (sesuaikan dengan sensor)
    jumlahPengunjung++;
    client.publish("kelasiot/pengunjung/perpustakaan", String(jumlahPengunjung), true, 1);
    delay(500);
  }

  delay(10); // this speeds up the simulation
}

void subscribe(String &topic, String &data){
  jumlahPengunjung = data.toInt();
}

void rgb(bool red, bool green, bool blue){
  digitalWrite(PIN_MERAH, red);
  digitalWrite(PIN_HIJAU, green);
  digitalWrite(PIN_BIRU, blue);
}
