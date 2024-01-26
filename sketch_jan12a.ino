#include <ArduinoMqttClient.h>
#include <WiFiNINA.h>

char ssid[] = "";      
char pass[] = "";   

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

const char broker[] = "";
int        port     = 1883;
const char moisture[]  = "alwinvonaesch/moisture";
const char pump[]  = "alwinvonaesch/pump";

//set interval for sending messages (milliseconds)
const long interval = 1800000;
unsigned long previousMillis = 0;

int count = 0;

int pumpPort = A2;
int sensorPort = A0;

int moisturevalue = 0;
int pumpvalue = 0;


void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // attempt to connect to Wifi network:
  Serial.print("Attempting to connect to WPA SSID: ");
  Serial.println(ssid);
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    // failed, retry
    Serial.print(".");
    delay(5000);
  }

  Serial.println("You're connected to the network");
  Serial.println();

  Serial.print("Attempting to connect to the MQTT broker: ");
  Serial.println(broker);

  if (!mqttClient.connect(broker, port)) {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());

    while (1);
  }

  Serial.println("You're connected to the MQTT broker!");
  Serial.println();
}

void loop() {
  // call poll() regularly to allow the library to send MQTT keep alive which
  // avoids being disconnected by the broker
  mqttClient.poll();

  if(pumpvalue == 20)
  {
    pumpvalue = 6;
  }
  if (currentMillis - previousMillis >= interval) {
    // save the last time a message was sent
    previousMillis = currentMillis;

    int moisturevalue = analogRead(sensorPort);

    Serial.print("Sending message to topic: ");
    Serial.println(moisture);
    Serial.println(moisturevalue);

    mqttClient.beginMessage(moisture);
    mqttClient.print(moisturevalue);
    mqttClient.endMessage();

    if(moisturevalue < 600)
    {
      analogWrite(pumpPort, 255);
      delay(3000);
      analogWrite(pumpPort, 0);
      pumpvalue--;

      Serial.print("Sending message to topic: ");
      Serial.println(pump);
      Serial.println(pumpvalue);

      mqttClient.beginMessage(pump);
      mqttClient.print(pumpvalue);
      mqttClient.endMessage();

      Serial.println();
    }
  }
  

    
  }