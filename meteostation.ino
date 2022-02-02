/******************************************************************************
   INCLUDES
 ******************************************************************************/
#include "arduino_secret.h"
#include <LiveObjects.h>
#include <DHT.h>
/******************************************************************************
   USER VARIABLES
 ******************************************************************************/

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
   Serial.println("Sampling data");
   lo.addToPayload("Temperature",dht.readTemperature() );  // adding 'Temperature' value to the current payload
   lo.addToPayload("Humidity", dht.readHumidity());// adding 'Humidity' value to the current payload
   Serial.println("Sending data to Live Objects");
   lo.sendData();                                   // send the data to Live Objects
   lo.loop();                             // don't forget to keep this in your main loop
}
