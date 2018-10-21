#include <stdio.h>
#include "rgb_lcd.h"
#include <Wire.h>
#include <Adafruit_CAP1188.h>

/* General parameters */
const int BAUD_RATE = 9600;
const int PERIOD_0_5SEC = 500;
unsigned long currentMilli = 0;
unsigned long previousMilli = 0;

/* LCD settings */
rgb_lcd lcd;
// const int colorR = 255;
// const int colorG = 0;
// const int colorB = 0;

/* Reset Pin setting for I2C/SPI */
#define CAP1188_RESET 9

/* Touch sensor initialization: */
/* For I2C (Arduino UNO - SDA: A4, SCL: A5) */
/* No reset pin! */
Adafruit_CAP1188 cap = Adafruit_CAP1188();
/* With reset pin */
// Adafruit_CAP1188 cap = Adafruit_CAP1188(CAP1188_RESET);

void setup()
{
    Serial.begin(BAUD_RATE);
    Serial.println("CAP1188 test!");

    /* set up the LCD's number of columns and rows: */
    lcd.begin(16, 2);
    // lcd.setRGB(colorR, colorG, colorB);

    /* Initialize the sensor, no need to provide address.
    However, if using I2C, you can pass in the I2C address. */
    if (!cap.begin(0x28)) {
        Serial.println("CAP1188 not found");
        lcd.print("CAP1188 not found");
        while (1);
    }
    Serial.println("CAP1188 found!");
    lcd.print("CAP1188 found!");
    delay(1500);
}

void loop()
{
    int touchCount = 0;
    uint8_t touched = cap.touched();

    currentMilli = millis();
    if (currentMilli - previousMilli > PERIOD_0_5SEC) {
        previousMilli = currentMilli;

        if (touched != 0) {
            for (uint8_t i = 0; i < 8; i++) {
                if (touched & (1 << i)) {
                    Serial.print("Touched: C");
                    Serial.print(i + 1);
                    Serial.println();
                    touchCount++;
                }
            }
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("touched: ");
            lcd.print(touchCount);
        } else {
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("touched: 0");
        }
    }
}
