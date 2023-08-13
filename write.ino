#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>
#include <WiFi101.h>

// Replace these with your WiFi credentials
const char* WIFI_SSID = "your_wifi_ssid";
const char* WIFI_PASS = "your_wifi_password";

// Replace these with your Adafruit Cloud credentials
#define ADAFRUIT_IO_USERNAME "your_username"
#define ADAFRUIT_IO_KEY "your_key"

// Set the name of your data feeds on Adafruit Cloud
#define DATA_FEED_1 "data_feed_1"
#define DATA_FEED_2 "data_feed_2"

// Create an instance of the WiFi client
WiFiClient client;

// Setup the Adafruit IO feeds
Adafruit_MQTT_Client mqtt(&client, ADAFRUIT_IO_USERNAME, ADAFRUIT_IO_KEY);
Adafruit_MQTT_Subscribe dataFeed1 = Adafruit_MQTT_Subscribe(&mqtt, ADAFRUIT_IO_USERNAME "/feeds/my_data_feed/speed");
Adafruit_MQTT_Subscribe dataFeed2 = Adafruit_MQTT_Subscribe(&mqtt, ADAFRUIT_IO_USERNAME "/feeds/my_data_feed/distance");

void setup() {
  Serial.begin(9600);
  delay(10);

  // Connect to WiFi
  Serial.println();
  Serial.print("Connecting to WiFi...");
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.print("Connected to WiFi. IP Address: ");
  Serial.println(WiFi.localIP());

  // Connect to Adafruit IO MQTT server
  mqtt.subscribe(&dataFeed1);
  mqtt.subscribe(&dataFeed2);
}

void loop() {
  // Ensure the MQTT client is connected
  if (!mqtt.connected()) {
    reconnect();
  }

  // Listen for incoming MQTT messages
  mqtt.processPackets(10000);
  mqtt.ping();

  // Print data when new messages arrive
  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(1000))) {
    if (subscription == &dataFeed1) {
      Serial.print("Speed Value: ");
      Serial.println((char *)dataFeed1.lastread);
    } else if (subscription == &dataFeed2) {
      Serial.print("Distance Value: ");
      Serial.println((char *)dataFeed2.lastread);
    }
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!mqtt.connected()) {
    Serial.print("Connecting to MQTT... ");
    if (mqtt.connect()) {
      Serial.println("connected.");
    } else {
      Serial.print("failed, rc=");
      Serial.println(" Retrying in 5 seconds...");
      delay(5000);
    }
  }
}
