#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <time.h>
#include "TZ.h"
#include "FS.h"
#include "LittleFS.h"
#include <CertStoreBearSSL.h>

#include "SECRETS.h"
#include "Pins.h"

BearSSL::CertStore certStore;

String name="FLLTFT";

int LED = 16;

#include "Deserializer.h"
Payload _signal;
Deserializer _deserializer = Deserializer();

#include <TFT_eSPI.h>
#include <SPI.h>
#include "Free_Fonts.h"

TFT_eSPI _tft = TFT_eSPI();
#define TFT_GREY 0x5AEB;
int rotatn = 1;
int xpos = 0;
int ypos = 40;

/*
TFT Display uses a minimum of 3 and upto 6 GPIO pins
on ESP8266 nodemcu. Here, removed previous display
elements - OLED and LEDs, which are anyway redundant.
*/

unsigned long tic = millis();

WiFiClientSecure HTTPClient;
PubSubClient MQTTClient(HTTPClient);

void setDateTime() {
  // You can use your own timezone, but the exact time is not used at all.
  // Only the date is needed for validating the certificates.
  configTime(TZ_America_Chicago, "pool.ntp.org", "time.nist.gov");

  Serial.print("Waiting for NTP time sync: ");
  time_t now = time(nullptr);
  while (now < 8 * 3600 * 2) {
    delay(100);
    Serial.print(".");
    now = time(nullptr);
  }
  Serial.println();

  struct tm timeinfo;
  gmtime_r(&now, &timeinfo);
  Serial.printf("%s %s", tzname[0], asctime(&timeinfo));
}

void onmessage(char* topic, byte* payload, unsigned int length) {
  /*
  Handle a new message published to the subscribed topic on the 
  MQTT broker and show in the OLED display.
  This is the heart of the subscriber, making it so the nodemcu
  can act upon information, say, to operate a solenoid valve when
  the mositure sensor indicates dry soil.
  */
  Serial.print("Got a message in topic ");
  Serial.println(topic);
  Serial.print("Received data: ");
  char message2display[length];
  for (unsigned int i = 0; i < length; i++) {
    Serial.print(char(payload[i]));
    message2display[i] = payload[i];
  }
  Serial.println();
  _signal = _deserializer.deserialize(message2display, length);
  if (_signal.Hi == 1) {
      _tft.fillScreen(TFT_RED);
      _tft.setCursor(xpos, ypos);
      _tft.setFreeFont(FSBI12);
      _tft.println();
      _tft.println("Flood @ I-55");
      _tft.println();
      _tft.setFreeFont(FSSB24);
      _tft.println("DO NOT");
      _tft.println("PASS!");
  } else if (_signal.Me == 1) {
      _tft.fillScreen(TFT_ORANGE);
      _tft.setCursor(xpos, ypos);
      _tft.setFreeFont(FSBI12);
      _tft.println();
      _tft.println("Flood @ I-55");
      _tft.println();
      _tft.setFreeFont(FSSB24);
      _tft.println("DANGER!");
  } else if (_signal.Lo == 1) {
      _tft.fillScreen(TFT_GOLD);
      _tft.setCursor(xpos, ypos);
      _tft.setFreeFont(FSBI12);
      _tft.println();
      _tft.println("Flood @ I-55");
      _tft.println();
      _tft.setFreeFont(FSSB24);
      _tft.println("CAUTION!");
  } else {
      _tft.fillScreen(TFT_DARKGREEN);
      _tft.setCursor(xpos, ypos);
      _tft.setFreeFont(FSBI12);
      _tft.println();
      _tft.println("Flood @ I-55");
      _tft.println();
      _tft.setFreeFont(FSSB24);
      _tft.println("ALL OKAY!");
  }
}

void reconnect() {
  // Loop until we’re reconnected
  while (!MQTTClient.connected()) {
    Serial.print("Attempting MQTT connection .. ");
    String clientID = "FLLTFT";
    // Attempt to connect
    // Insert your password
    if (MQTTClient.connect(clientID.c_str(), HIVE_USERID, HIVE_PASSWD)) {
      Serial.println("connected");
      // Once connected, publish an announcement…
      MQTTClient.publish("FLL/Test", "Namaste FLL!");
      // … and resubscribe
      MQTTClient.subscribe("FLL");
    } else {
      Serial.print("failed, rc = ");
      Serial.print(MQTTClient.state());
      Serial.println(" try again in 5 seconds.");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while(!Serial) {
    // Stabilize the serial bus
  }
  delay(600);

  // Connect to WiFi:
  WiFi.mode(WIFI_OFF);
  delay(1500);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(600);
    Serial.print(".");
  }
  Serial.println(".");
  Serial.print("Connected: ");
  Serial.println(WIFI_SSID);

  // MQTT:
  HTTPClient.setInsecure();
  MQTTClient.setServer(HiveMQX, 8883);  // MQTT_IP, 1883 or HiveNQ, 1883
  MQTTClient.setCallback(onmessage);    

  // TFT:
  _tft.init();
  _tft.setRotation(rotatn);
  _tft.fillScreen(TFT_DARKGREEN);
  _tft.fillScreen(TFT_OLIVE);
  _tft.setCursor(xpos, ypos);
  _tft.setFreeFont(FSBI12);
  _tft.println();
  _tft.println("TFT READY!");
}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned long toc = millis();
  /*
    Lights! Camera! Action!
    Here is where the action is for publisher and subscriber.
    Note the use of millis to scheduling the publication of
    sensor readings to the MQTT broker in a non-blocking way. 
    The use of 'delay()' would block the listener, 
    causing events to be missed.  
  */
  digitalWrite(LED, LOW);  
  if ((toc - tic) > 3000) {
     tic = toc;
    if (!MQTTClient.connected()) {
      Serial.println("Made no MQTT connection.");
      reconnect();
    } else {
      digitalWrite(LED, HIGH);
      // publish_message(); // Publisher action
    }
  }
  MQTTClient.loop(); // Callbacks handled in event loop
}