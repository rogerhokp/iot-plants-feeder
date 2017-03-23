#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266HTTPClient.h>
#include <WifiConnect.h>

#define ONE_WIRE_BUS D7
#define MH_SENSOR A0
#define NOTI_LED D0

#define ssid "hihi"
#define password "Sss99889988"

OneWire oneWire(ONE_WIRE_BUS);

DallasTemperature temperatureSensor(&oneWire);

String ip;
void setup(void)
{
  Serial.begin(115200);
  temperatureSensor.begin();

  ip = WifiConnect(ssid, password);
  Serial.print("Connected to wifi with IP");
  Serial.println(ip);

  pinMode(MH_SENSOR, INPUT);
  pinMode(NOTI_LED, OUTPUT);
}


void postResult(float t, float mh) {

  HTTPClient http;
  http.begin("http://iot-rogerhokp.rhcloud.com/dirt");
  http.addHeader("Content-Type", "application/json; charset=utf-8");
  http.POST("{\"temperature\":" + String(t) + ",\"mh\":" + String(mh) + "}");
  Serial.println("POST REQUEST {\"temperature\":" + String(t) + ",\"mh\":" + String(mh) + "}");
  Serial.println("POST Result " + http.getString());
}


void loop(void)
{

  digitalWrite(NOTI_LED, LOW);
  Serial.print("Requesting temperatures...");
  temperatureSensor.requestTemperatures(); // Send the command to get temperatures


  // Serial.print("Temperature for the device 1 (index 0) is: ");
  // Serial.println(temperatureSensor.getTempCByIndex(0));
  //
  // Serial.println("MN Sensor");
  // Serial.println(analogRead(MH_SENSOR));

  postResult(temperatureSensor.getTempCByIndex(0), analogRead(MH_SENSOR));
  digitalWrite(NOTI_LED, HIGH);
  delay(60000);
}
