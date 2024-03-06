#include <ezButton.h>

ezButton button(8);
const int RELAY_PIN1 = 6;
const int RELAY_PIN2 = 5;
const int LED_PIN = 13;
int current_state = LOW;
int next_state = LOW;
int important_state = 0;

unsigned long previousMillis = 0;
int seconds = 0;
int minutes = 0;
int hours = 0;
int days = 0;
int interval_on = 10800; // in seconds (180 minutes)
int interval_off = 300; // in seconds (5 minutes)
// int interval_on = 10; // in seconds
// int interval_off = 3; // in seconds
int next_off = interval_on + interval_off;


void setup() {
  pinMode(RELAY_PIN1, OUTPUT);
  pinMode(RELAY_PIN2, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  Serial.begin(9600);
  button.setDebounceTime(50);

  // Set first state as HIGH (Because every app start, the button however will pressed)
  changeState(HIGH);
}

void loopingInSecond() {
  // Check if at least 1 second has passed since the previous check
  if (millis() - previousMillis >= 1000) {
    seconds++;
    if(important_state != 2){
      // Continue countdown if user not turn on machine
      next_off--;
    }else{
      // Reset if user turn on machine
      next_off = interval_on + interval_off;
    }
      
    if(seconds>=60){
      seconds=0;
      minutes++;
      if(minutes>=60){
        minutes=0;
        hours++;
        if(hours>=24){
          hours=0;
          days++;
        }
      }
    }

    Serial.println("Loop ("+String(millis())+") "+String(days)+" / "+String(hours)+":"+String(minutes)+":"+String(seconds)+")  ("+String(next_off)+")");
    Serial.println("CS:"+String(current_state)+" NS:"+String(next_state)+" IS:"+String(important_state));

    // Update the previous time for the next check
    previousMillis = millis();

    // Make decision to turn on/off relay
    stateDecision();
  }
}

void stateDecision() {

  if(important_state != 0){
    if(important_state == 1){
      next_state = LOW;
    }else{
      next_state = HIGH;
    }
  }else{
      // turn on switch
    if(next_off==interval_off){
      // important_state = 2;
      next_state = HIGH;
    }

    // turn off switch & reset next_off
    if(next_off==0){
      // important_state = 1;
      next_state = LOW;
      next_off = interval_on + interval_off;
    }
  }

  // Change state just when current_state different with next_state
  if (current_state != next_state) {
    changeState(next_state);
  }
}

void changeState(int newState) {
  if (newState == LOW){
    Serial.println("Clicked: OFF");
    current_state = LOW;
    // important_state = 1;

    // Remove important state if user turned off machine
    important_state = 0;

    digitalWrite(RELAY_PIN1, HIGH);
    digitalWrite(RELAY_PIN2, LOW);
  }else{
    Serial.println("Clicked: ON");
    current_state = HIGH;
    // important_state = 2;

    digitalWrite(RELAY_PIN1, LOW);
    digitalWrite(RELAY_PIN2, HIGH);
  }
}

void sendToServer(){}

// the loop function runs over and over again forever
void loop() {
  // Check if Millis reset
  if (previousMillis>=5000 & millis()<previousMillis) {
    previousMillis = 0;
  }

  button.loop();
  if(button.isPressed()){
    Serial.println("Button pressed");
    if (current_state == HIGH){
      important_state = 1;
      // changeState(HIGH);
    }else{
      important_state = 2;
      // changeState(LOW);
    }
  }

  // Count loop interval
  loopingInSecond();

  // Send api or mqtt or anything to store datetime on/off relay at server
  sendToServer();
}