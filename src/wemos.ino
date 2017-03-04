#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include <functional>
#include "switch.h"
#include "UpnpBroadcastResponder.h"
#include "CallbackFunction.h"

// prototypes
boolean connectWifi();

//on/off callbacks
void switchOn();
void switchOff();
// void kitchenLightsOn();
// void kitchenLightsOff();

// Change this before you flash
const char* ssid = "Apt. #3";
const char* password = "Chowfornow3344";
const int outputPin = 2;//next to Gnd?
const int inputPin = 0;//further from G?
// const int inputPin = 0;//GPIO0
static int output_status = 0;
boolean wifiConnected = false;

UpnpBroadcastResponder upnpBroadcastResponder;

Switch *switchrelay = NULL;
// Switch *kitchen = NULL;
//
void setup()
{
  Serial.begin(9600);

  pinMode(outputPin, OUTPUT);
  pinMode(inputPin, INPUT);
  attachInterrupt(inputPin, toggle, FALLING);
  switchOff();
  // Initialise wifi connection
  wifiConnected = connectWifi();

  if(wifiConnected){
    upnpBroadcastResponder.beginUdpMulticast();

    // Define your switches here. Max 14
    // Format: Alexa invocation name, local port no, on callback, off callback
    switchrelay = new Switch("JPGBox001", 80, switchOn, switchOff);
    // kitchen = new Switch("kitchen lights", 81, kitchenLightsOn, kitchenLightsOff);

    Serial.println("Adding switches upnp broadcast responder");
    upnpBroadcastResponder.addDevice(*switchrelay);
    // upnpBroadcastResponder.addDevice(*kitchen);
  }
}

void loop()
{
	 if(wifiConnected){
      upnpBroadcastResponder.serverLoop();

      // kitchen->serverLoop();
      switchrelay->serverLoop();
	 }

  //  if(digitalRead(inputPin))
  //  {
  //    Serial.print("Button Press");
  //    output_status = !output_status;
  //    digitalWrite(outputPin,output_status);
  //  }
}

void switchOn() {
  output_status = HIGH;
  digitalWrite(outputPin, output_status);
  Serial.print("Switch: ");
  Serial.println(output_status);
}

void switchOff() {
  output_status = LOW;
  digitalWrite(outputPin, output_status);
  Serial.print("Switch: ");
  Serial.println(output_status);
}

void toggle(){
  output_status = !output_status;
  digitalWrite(outputPin, output_status);
  Serial.print("Toggle: ");
  Serial.println(output_status);
}
// void kitchenLightsOn() {
//     Serial.print("Switch 2 turn on ...");
// }
//
// void kitchenLightsOff() {
//   Serial.print("Switch 2 turn off ...");
// }

// connect to wifi – returns true if successful or false if not
boolean connectWifi(){
  boolean state = true;
  int i = 0;

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  Serial.println("Connecting to WiFi");

  // Wait for connection
  Serial.print("Connecting ...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (i > 10){
      state = false;
      break;
    }
    i++;
  }

  if (state){
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }
  else {
    Serial.println("");
    Serial.println("Connection failed.");
  }

  return state;
}
