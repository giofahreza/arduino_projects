//You can modifiy the next line if your board has the LED connected to another Pin
#define LED 2 // ESP-01 -> Pin 1 or ESP-01S -> Pin 2

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(LED, HIGH); // turn the LED on (HIGH is the voltage level)
  delay(1000);
  digitalWrite(LED, LOW); // turn the LED off by making the voltage LOW
  delay(1000); // wait for a second
}