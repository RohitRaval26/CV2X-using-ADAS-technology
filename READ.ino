#define reduced_speed 80
#define speed_pin A1
#define distance_pin A2
#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>
#include <SPI.h>
#include <WiFi101.h>
char ssid[] = "Your_WiFi_SSID";
char pass[] = "Your_WiFi_Password";

struct CAM_Message {
    int speed = 100;
    int heading = 1;
    int monitorDistance = 1100;
    int distanceX = 1200;
    int distanceY = 1500;
    // Other relevant information
};

// Replace with your Adafruit IO credentials
#define AIO_USERNAME "Your_AIO_Username"
#define AIO_KEY "Your_AIO_Key"


#define MQTT_SERVER "io.adafruit.com"
#define MQTT_PORT 1883

WiFiClient wifiClient;
Adafruit_MQTT_Client mqttClient(&wifiClient, MQTT_SERVER, MQTT_PORT, AIO_USERNAME, AIO_KEY);

// Create an MQTT feed instance
Adafruit_MQTT_Publish feed1 = Adafruit_MQTT_Publish(&mqttClient, AIO_USERNAME "/feeds/my_data_feed/speed");
Adafruit_MQTT_Publish feed2 = Adafruit_MQTT_Publish(&mqttClient, AIO_USERNAME "/feeds/my_data_feed/heading");
Adafruit_MQTT_Publish feed3 = Adafruit_MQTT_Publish(&mqttClient, AIO_USERNAME "/feeds/my_data_feed/distanceX");
Adafruit_MQTT_Publish feed4 = Adafruit_MQTT_Publish(&mqttClient, AIO_USERNAME "/feeds/my_data_feed/distanceY");

CAM_Message recv_CAMMessage(){
  //Code for recieving data from OBD port in CAM format
  CAM_Message camMsg;
  camMsg.speed = 100;
  camMsg.heading = 50;
  camMsg.distanceX = 1200;
  camMsg.monitorDistance = 1500;
  camMsg.monitorDistance = 1100;
  return camMsg;
}

void send_CAMMessge(CAM_Message camMsg){
  //Code to send updated speed and configuration to OBD port with communicates it to ADAS
}

void send_data(CAM_Message camMsg){
  if(camMsg.speed>80 && camMsg.monitorDistance<1000){
    Serial.println("Speed above limit. Applying brake");
    camMsg.speed = camMsg.speed - 5;
    send_CAMMessge(camMsg);
  }
  if (feed1.publish((long)camMsg.speed)) {
    Serial.println("Data sent to Adafruit Cloud: " + String(camMsg.speed));
  } else {
    Serial.println("Failed to send data!");
  }
  if (feed2.publish((long)camMsg.heading)) {
    Serial.println("Data sent to Adafruit Cloud: " + String(camMsg.heading));
  } else {
    Serial.println("Failed to send data!");
  }
  if (feed3.publish((long)camMsg.distanceX)) {
    Serial.println("Data sent to Adafruit Cloud: " + String(camMsg.distanceX));
  } else {
    Serial.println("Failed to send data!");
  }
  if (feed4.publish((long)camMsg.distanceY)) {
    Serial.println("Data sent to Adafruit Cloud: " + String(camMsg.distanceY));
  } else {
    Serial.println("Failed to send data!");
  }
}

void setup() {
  while (!WiFi.begin(ssid, pass)) {
    Serial.println("Connecting to WiFi...");
    delay(1000);
  }
  while (!mqttClient.connected()) {
    Serial.println("Connecting to Adafruit MQTT...");
    if (mqttClient.connect()) {
      Serial.println("Connected to Adafruit MQTT!");
    } else {
      Serial.println("Connection failed. Retrying...");
      delay(1000);
    }
  }
  pinMode(speed_pin, INPUT);
  pinMode(distance_pin, INPUT);
  // put your setup code here, to run once:

}

void loop() {
  CAM_Message camMsg;
  camMsg = recv_CAMMessage();
  send_data(camMsg);
  // put your main code here, to run repeatedly:
  delay(1000);
}
