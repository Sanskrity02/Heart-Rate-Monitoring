// #include <DallasTemperature.h>
#include <Wire.h>
#include "MAX30105.h"
#include "heartRate.h"
#include "spo2_algorithm.h"
#include <WiFi.h>
#include <WiFiClient.h>
#include <ThingSpeak.h>

#define alert D5
#define ONE_WIRE_BUS D6

#define InPin5 32
//just change the following
#define WIFI_SSID "Galaxy A21"     //your wifi name
#define WIFI_PASSWORD "abcd1234"      //wifi password

char auth[] = "yxghE2L4SsCdLQou_0YnjyrLrUWVZkS_";
unsigned long myChannelNumber =;    //channel id from thingspeak
const char * myWriteAPIKey = "";    //read API from thingspeak

uint32_t irBuffer[100]; //infrared LED sensor data
uint32_t redBuffer[100];  //red LED sensor data

MAX30105 particleSensor;



const byte RATE_SIZE = 4; //Increase this for more averaging. 4 is good.
byte rates[RATE_SIZE]; //Array of heart rates
byte rateSpot = 0;
long lastBeat = 0; //Time at which the last beat occurred
long sensorValue;
long gsravg;
float beatsPerMinute;
int beatAvg;
int32_t bufferLength; //data length
int32_t spo2; //SPO2 value
int8_t validSPO2; //indicator to show if the SPO2 calculation is valid
int32_t heartRate; //heart rate value
int8_t validHeartRate;
const int LM35_PIN = A0;



void setup()
{
 
  Serial.begin(115200);
  Serial.println("Initializing...");


  // Connect to Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    // Serial.println("Connecting to WiFi...");
  }

  // Initialize sensor
  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) //Use default I2C port, 400kHz speed
  {
    Serial.println("MAX30105 was not found. Please check wiring/power. ");
    while (1);
  }

  particleSensor.setup(); //Configure sensor with default settings
  particleSensor.setPulseAmplitudeRed(0x0A); //Turn Red LED to low to indicate sensor is running
  particleSensor.setPulseAmplitudeGreen(0); //Turn off Green LED
  
}

void loop()
{

  // int sensorValue = analogRead(LM35_PIN);
  long irValue = particleSensor.getIR();

    bufferLength = 100;
    for (byte i = 0 ; i < bufferLength ; i++)
  {
    while (particleSensor.available() == false) //do we have new data?
    particleSensor.check(); //Check the sensor for new data

    redBuffer[i] = particleSensor.getRed();
    irBuffer[i] = particleSensor.getIR();
    particleSensor.nextSample(); //We're finished with this sample so move to next sampl
  }
    maxim_heart_rate_and_oxygen_saturation(irBuffer, bufferLength, redBuffer, &spo2, &validSPO2, &heartRate, &validHeartRate);
    Serial.println(heartRate);
    ThingSpeak.writeField(myChannelNumber,1,heartRate,myWriteAPIKey);
    ThingSpeak.writeField(myChannelNumber,2,spo2,myWriteAPIKey); 
}

  