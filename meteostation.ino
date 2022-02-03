/******************************************************************************
   INCLUDES
 ******************************************************************************/
#include "arduino_secret.h"
#include <LiveObjects.h>
#include <DHT.h>
/******************************************************************************
   USER VARIABLES
 ******************************************************************************/

uint32_t messageRate = 60000;   // stores the current data message rate in Milliseconds
unsigned long lastMessageTime = 0;   // stores the time when last data message was sent   1->1000 3600->3600000
float h,t ; 

#define DHTPIN 15    // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT 11
DHT dht(DHTPIN, DHTTYPE);// Initialize DHT sensor.

/******************************************************************************
   USER PROGRAM
 ******************************************************************************/
void setup() {
  Serial.begin(115200);
  Serial.print("\n*** Live Objects for Arduino MKR boards, revision ");
  dht.begin();
  Serial.print(SW_REVISION);
  Serial.println("***");
  lo.setSecurity(TLS);
  lo.begin(MQTT, TEXT, true);
  lo.connect(); // connects to the network + Live Objects
}

void loop() {
  
   if (millis() - lastMessageTime > messageRate) {
    // collect data periodically("Sampling data");
           h = dht.readHumidity();
           // Read temperature as Celsius (the default)
           t = dht.readTemperature(); 
      while((isnan(h) || isnan(t))){
           h = dht.readHumidity();
           // Read temperature as Celsius (the default)
           t = dht.readTemperature(); 
      }
   lo.addToPayload("Temperature",t );  // adding 'Temperature' value to the current payload
   lo.addToPayload("Humidity", h);// adding 'Humidity' value to the current payload
   Serial.println("Sending data to Live Objects");
   lo.sendData();  // send the data to Live Objects
   lastMessageTime = millis();
   }
   lo.loop();   // don't forget to keep this in your main loop
   
}
