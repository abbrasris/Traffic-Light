#define DO_RLed 14 // output D5
#define DO_YLed 12 // output D6
#define DO_GLed 13 // output D7
#define DI_CarSensor 15 // input D8
#define DI_AnalogPin 2 // input D4

typedef enum TrafficLightStates {
  StateRed,
  StateRedYellow,
  StateGreen,
  StateYellow
};

TrafficLightStates TLState;
bool CarSense = false;

unsigned long delayStart = 0;
bool delayRunning = false;

/**
 * Turns three LED lamps on or off.
 * 
 * @param red Red LED lamp.
 * @param yellow Yellow LED lamp.
 * @param green Green LED lamp.
 */
void changeLight(bool red, bool yellow, bool green) {
  digitalWrite(DO_RLed, red ? HIGH : LOW);
  digitalWrite(DO_YLed, yellow ? HIGH : LOW);
  digitalWrite(DO_GLed, green ? HIGH : LOW);
}

/**
 * Starts the timer.
 */
void startTimer() {
  if (delayRunning) {
     return;
  }

  delayStart = millis();
  delayRunning = true;
}

/**
 * Stops the timer.
 */
void stopTimer() {
  delayRunning = false;
}

/**
 * Checks if the timer is complete.
 * 
 * @return whether or not the timer is complete.
 */
bool checkTimer() {
  return delayRunning && ((millis() - delayStart) >= 10000);
}

void setup() {
  pinMode(DO_RLed, OUTPUT);
  pinMode(DO_YLed, OUTPUT);
  pinMode(DO_GLed, OUTPUT);
  pinMode(DI_CarSensor, INPUT);

  TLState = StateRed;
}

void loop() {
  // Read from button input
  CarSense = digitalRead(DI_CarSensor);
  
  switch (TLState) {
    case StateRed:
      changeLight(1, 0, 0);
      startTimer();

      // Check if timer is finished or button is pressed
      if (checkTimer() || CarSense) {
        TLState = StateRedYellow;
        stopTimer();
      }
      break;
      
    case StateRedYellow:
      changeLight(1, 1, 0);
      delay(1000);
  
      TLState = StateGreen;
      break;
      
    case StateGreen:
      changeLight(0, 0, 1);
      startTimer();

      // Check if timer is finished or button is pressed
      if (checkTimer() || CarSense) {
        TLState = StateYellow;
        stopTimer();
      }
      break;
      
    case StateYellow:
      changeLight(0, 1, 0);
      delay(1000);

      TLState = StateRed;
      break;
  }
}
