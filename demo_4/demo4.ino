#include "SparkFunLSM6DS3.h"
#include "Wire.h"
#include "SPI.h"
#include "math.h"
#include <RF24.h>
#include <BTLE.h>

RF24 radio(9, 10);
BTLE btle(&radio);
LSM6DS3 myIMU; //Default constructor is I2C, addr 0x6B

float x_base;
float y_base;
unsigned long previous_millis = 0;
unsigned long previous_millis10 = 0;
unsigned long previous_millis00 = 0;
unsigned long current_millis = 0;
double rms = 0;
double rmsMax = 0;
bool calibrate = true;
float accX;
float accY;
bool moving = false;
int count = 0;

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(1000); //relax...
  Serial.println("Processor came out of reset.\n");

  //Call .begin() to configure the IMU
  myIMU.begin();
  x_base = myIMU.readFloatAccelX();
  y_base = myIMU.readFloatAccelY();
  Serial.println("Program start!");

  // Bluetooth SSID
  btle.begin("Jack");
}

void loop()
{
  // put your main code here, to run repeatedly:
  btle.advertise(0, 0);
  current_millis = millis();

  if (current_millis - previous_millis > 20)
  {
    previous_millis = current_millis;
    accX = myIMU.readFloatAccelX();
    accY = myIMU.readFloatAccelY();
  }

  if (calibrate)
  {
    rms = sqrt(pow(double(accX) - double(x_base), 2) + pow(double(accY) - double(y_base), 2));

    if (rmsMax < rms)
    {
      rmsMax = rms;
    }

    if (current_millis - previous_millis10 > 5000)
    {
      previous_millis10 = current_millis;
      Serial.println("done calibrating");
      calibrate = false;
    }
  }
  else
  {
    rms = sqrt(pow(double(accX) - double(x_base), 2) + pow(double(accY) - double(y_base), 2));

    // if(!moving && rms > rmsMax)
    if (rms > rmsMax - 0.05)
    {
      count++;
      Serial.println(count);
    }

    nrf_service_data buf;
    buf.service_uuid = NRF_TEMPERATURE_SERVICE_UUID;
    buf.value = count;
    btle.advertise(0X16, &buf, sizeof(buf));
    btle.hopChannel();
  }
  delay(150);
}