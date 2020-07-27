#include <Arduino.h>
#include <SPI.h>
#include <WiFi101.h>
//#include "secrets.h"

#define sensorPin 0
//#define pushButton 7

char ssid[] = "YOUR_SSID";                            // change it!
char pass[] = "YOUR_PASSWORD";                        // change it!
//const String yourDevice = YOUR_DEVICE_DEVELOPER_ID; // change it!
String apikey = "YOUR_APIKEY";

int status = WL_IDLE_STATUS;
char serverAddr[] = "apiv2.favoriot.com";
WiFiClient client;

int sensorValue = 0;
float celcius = 0.0;
float voltage = 0.0;

void dataStream(float celcius);

void setup()
{
  Serial.begin(115200);
  analogReadResolution(10);
  // attempt to connect to WiFi network:
  while (status != WL_CONNECTED)
  {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);

    // wait 5 seconds for connection:
    delay(5000);
  }

  // you're connected now, so print out the data:
  Serial.print("You're connected to the network");
}

void loop()
{

  //  if (digitalRead(pushButton) == LOW) //disable this line to retrieve data automatically without pressing the button
  //  {
  //execute when the push button is pressed only
  sensorValue = analogRead(sensorPin); //read the temperature and convert to celcius
  voltage = sensorValue * (3300 / 1024);
  celcius = (voltage - 500) / 10;
  dataStream(celcius); //send data to FAVORIOT platform
                       //  }
  delay(5000);         //enable this line if you disable the 'if' condition
}

void dataStream(float celcius)
{
  // Json Data to send to Platform
  String json = "{\"device_developer_id\":\"deviceDefault@stiotchallenge\",\"data\":{\"Temperature\":\"" + String(celcius) + "\"}}";
  // display temperature value
  Serial.println("\n        TEMPERATURE : " + String(celcius) + " Celcius");

  if (client.connect(serverAddr, 80))
  {
    // Make a HTTP request:
    Serial.println("        STATUS : Sending data..."); //Display sending status
    client.println("POST /v2/streams HTTP/1.1");
    client.println("Host: apiv2.favoriot.com");
    client.print(String("apikey: "));
    client.println(apikey);
    client.println("Content-Type: application/json");
    client.println("cache-control: no-cache");
    client.print("Content-Length: ");
    int thisLength = json.length();
    client.println(thisLength);
    client.println("Connection: close");

    client.println();
    client.println(json);
    Serial.println("        STATUS : Data sent!"); //display sent status
  }
}