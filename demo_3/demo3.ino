#define BAUD_RATE 9600
#define RED_PIN 3
#define YELLOW_PIN 4
#define GREEN_PIN 6
#define RED_STATE 0
#define RED_YELLOW_STATE 1
#define YELLOW_STATE 2
#define GREEN_STATE 3
#define RED_MILLIS 10000
#define YLW_MILLIS 2000
#define GRN_MILLIS 5000
#define MS500 500
#define MS1500 1500
#define MS250 250
#define BUZZER_PIN 7
#define TOUCH_PIN 2

int tl_state;           // Traffic light state.
unsigned long tl_timer; // Traffic light timer.
unsigned long currentMilli = 0;
unsigned long tl_timer_buzz = 0;
bool pressed = false;
int isTouched = 0;
bool buzz_on = true;

void setup()
{
  Serial.begin(BAUD_RATE);

  // Configure LED pins as outputs.
  pinMode(RED_PIN, OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);

  // Buzzer
  pinMode(BUZZER_PIN, OUTPUT);
  // Touch
  pinMode(TOUCH_PIN, INPUT);

  // Initial state for states and timers..
  tl_state = RED_STATE;
  tl_timer = millis() + RED_MILLIS;
}

void loop()
{
  // Buzzer state machine.
  switch (tl_state)
  {
  case RED_STATE:
    if (buzz_on && currentMilli > tl_timer_buzz)
    {
      digitalWrite(BUZZER_PIN, LOW);
      tl_timer_buzz = millis() + MS250;
      buzz_on = false;
    }
    if (!buzz_on && currentMilli > tl_timer_buzz)
    {
      digitalWrite(BUZZER_PIN, HIGH);
      tl_timer_buzz = millis() + MS250;
      buzz_on = true;
    }
    break;
  case RED_YELLOW_STATE:
    digitalWrite(BUZZER_PIN, LOW);
    break;
  case YELLOW_STATE:
    digitalWrite(BUZZER_PIN, LOW);
    break;
  case GREEN_STATE:
    if (buzz_on && currentMilli > tl_timer_buzz)
    {
      digitalWrite(BUZZER_PIN, LOW);
      tl_timer_buzz = millis() + MS1500;
      buzz_on = false;
    }
    if (!buzz_on && currentMilli > tl_timer_buzz)
    {
      digitalWrite(BUZZER_PIN, HIGH);
      tl_timer_buzz = millis() + MS500;
      buzz_on = true;
    }
    break;
  }
  // Detect touch-button pressed.
  isTouched = digitalRead(TOUCH_PIN);

  // main loop
  currentMilli = millis();

  // TL state machine.
  switch (tl_state)
  {
  case RED_STATE:
    // Turn red light on.
    digitalWrite(RED_PIN, HIGH);

    if (currentMilli > tl_timer)
    { // Turn red light off.
      //Set timer for red-yellow state.
      tl_state = RED_YELLOW_STATE;
      tl_timer = millis() + YLW_MILLIS;
    }
    break;

  case RED_YELLOW_STATE:
    // Code for red-yellow state.
    digitalWrite(YELLOW_PIN, HIGH);
    if (currentMilli > tl_timer)
    { // Turn red light off.
      //Set timer for red-yellow state.
      tl_state = GREEN_STATE;
      tl_timer = millis() + GRN_MILLIS;
      tl_timer_buzz = millis() + MS500;
      buzz_on = true;
      digitalWrite(RED_PIN, LOW);
      digitalWrite(YELLOW_PIN, LOW);
    }
    break;

  case GREEN_STATE:
    Serial.print("touch:");
    Serial.print(isTouched);
    Serial.print("\n");
    // Turn green light on.
    digitalWrite(GREEN_PIN, HIGH);

    if (!pressed && isTouched)
    {
      Serial.print("touched!");
      tl_timer = millis() + GRN_MILLIS;
      pressed = true;
    }
    if (pressed && currentMilli > tl_timer)
    {
      tl_state = YELLOW_STATE;
      tl_timer = millis() + YLW_MILLIS;
      pressed = false;
      digitalWrite(GREEN_PIN, LOW);
    }
    break;

  case YELLOW_STATE:
    // Code for yellow state.
    digitalWrite(YELLOW_PIN, HIGH);

    if (currentMilli > tl_timer)
    { // Turn red light off.
      //Set timer for red-yellow state.
      tl_state = RED_STATE;
      tl_timer = millis() + RED_MILLIS;
      tl_timer_buzz = millis() + MS250;
      buzz_on = true;
      digitalWrite(YELLOW_PIN, LOW);
    }
    break;
  }
}