#include <Servo.h>
Servo myservo;
const int PIN_SERVO = 3;
const int PIN_LIGHT = 0;
const unsigned long BAUD_RATE = 9600;
const unsigned long PERIOD_1SEC = 1000;
const unsigned long PERIOD_1_5SEC = 1500;
const unsigned long PERIOD_3SEC = 3000;
const unsigned long PERIOD_5SEC = 5000;
const unsigned long PERIOD_7SEC = 7000;
const unsigned long PERIOD_10SEC = 10000;
const unsigned long PERIOD_15SEC = 15000;

unsigned long currentMilli = 0;
unsigned long previousMilli1 = 0;
unsigned long previousMilli1_5 = 0;
unsigned long previousMilli3 = 0;
unsigned long previousMilli5 = 0;
unsigned long previousMilli7 = 0;
unsigned long previousMilli10 = 0;
unsigned long previousMilli15 = 0;

int led_blink = 0;
int led_status = 0;
int light_calibration = 1;
int calibrarion_time_counter = 10;
int current_motor_degree = 0;
int previous_motor_degree = 0;
unsigned long light_value = 0;
unsigned long light_val_max = 0;
unsigned long light_val_min = 0;

void setup() {
  Serial.begin(BAUD_RATE);
  myservo.attach(PIN_SERVO);
  pinMode(LED_BUILTIN, OUTPUT);
  if(light_calibration == 1) {
    Serial.print("Sart light calibration for 10 sec...\n");
    led_blink = 1;
    light_value = analogRead(PIN_LIGHT);
    light_val_min = light_value;
  }
}

void loop() {
  currentMilli = millis();
  light_value = analogRead(PIN_LIGHT);

  // Light sensor calibration
  if(currentMilli - previousMilli10 > PERIOD_10SEC) {
    previousMilli10 = currentMilli;
    if(light_calibration == 1) {
      light_calibration = 0;
      led_blink = 0;
      Serial.print("Stop light calibration\n");
      Serial.print("Maximum_light value: ");
      Serial.print(light_val_max);
      Serial.print("\n");
      Serial.print("Minimun light value : ");
      Serial.print(light_val_min);
      Serial.print("\n\n");
    }
  }
  
  // blinking or rotate servo motor
  if(currentMilli - previousMilli1 > PERIOD_1SEC) {
    previousMilli1 = currentMilli;

    if(light_calibration == 1) {
      if(light_val_max < light_value) {
        light_val_max = light_value;
      }
      if(light_val_min > light_value) {
        light_val_min = light_value;
      }
    }
    
    if(led_blink == 1) {
      Serial.print(--calibrarion_time_counter);
      
      // for debug:
      // Serial.print(" ");
      // Serial.print(light_value);
      // Serial.print(" ");
      // Serial.print(light_val_max);
      // Serial.print(" ");
      // Serial.print(light_val_min);

      Serial.print(" sec...\n");
      if(led_status == 0) {
        digitalWrite(LED_BUILTIN, HIGH);
        led_status = 1;
      }else{
        digitalWrite(LED_BUILTIN, LOW);
        led_status = 0;
      }
    }

    if(light_calibration == 0) {
      current_motor_degree = map(light_value, light_val_min, light_val_max, 0, 179);
      
      if(previous_motor_degree != current_motor_degree) {
        previous_motor_degree = current_motor_degree;
        Serial.print("current motor degree: ");
        Serial.print(current_motor_degree);
        Serial.print("\n");
        myservo.write(current_motor_degree);
      }else{
        Serial.print("current motor degree: ");
        Serial.print(current_motor_degree);
        Serial.print("\n");
      }

    }
  }

  // milli() will overflow (go back to zero), 
  // after approximately 50 days.
  if((currentMilli - previousMilli1 < 0) 
    || (currentMilli - previousMilli3 < 0) 
    || (currentMilli - previousMilli7 < 0) 
    || (currentMilli - previousMilli15 < 0))
  {
    Serial.print("MILLI COUNTER RESET!");
    previousMilli1 = 0;
    previousMilli3 = 0;
    previousMilli7 = 0;
    previousMilli15 = 0;
  }
  
  if(currentMilli - previousMilli3 >= PERIOD_3SEC) {
    previousMilli3 = currentMilli;
  //  Serial.print("It's 3 seconds");
  //  Serial.print(" (current milli: ");
  //  Serial.print(previousMilli3);
  //  Serial.print(")");
  //  Serial.print("\n");
  }
  
  if(currentMilli - previousMilli7 >= PERIOD_7SEC) {
    previousMilli7 = currentMilli;
  //  Serial.print("It's 7 seconds");
  //  Serial.print(" (current milli: ");
  //  Serial.print(previousMilli7);
  //  Serial.print(")");
  //  Serial.print("\n");
  }
  
  if(currentMilli - previousMilli15 >= PERIOD_15SEC) {
    previousMilli15 = currentMilli;
  //  Serial.print("It's 15 seconds");
  //  Serial.print(" (current milli: ");
  //  Serial.print(previousMilli15);
  //  Serial.print(")");
  //  Serial.print("\n");
  }
}