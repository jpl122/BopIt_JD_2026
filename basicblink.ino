const int ledPin = 8;     // Physical pin 14
const int buttonPin = 9;  // Physical pin 15

unsigned long lastChange = 0;
bool ledOn = false;
bool wasPressed = false;

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  digitalWrite(ledPin, LOW);
}

void loop() {
  bool pressed = (digitalRead(buttonPin) == HIGH);
  unsigned long now = millis();

  if (!pressed) {
    // Turn off immediately when released.
    ledOn = false;
    digitalWrite(ledPin, LOW);
  } else {
    if (!wasPressed) {
      // Start with the LED on when first pressed.
      ledOn = true;
      lastChange = now;
    } else if (now - lastChange >= 1000) {
      ledOn = !ledOn;
      lastChange = now;
    }

    digitalWrite(ledPin, ledOn ? HIGH : LOW);
  }

  wasPressed = pressed;
}