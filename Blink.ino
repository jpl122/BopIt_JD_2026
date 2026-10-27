#include <avr/io.h>
// jd assignment
void setup() {
  DDRB |= 0x01;   // Set PB0 (pin 14) as output
}

void loop() {
  PORTB |= 0x01;    // Set PB0 high
  delay(1000);
  PORTB &= ~0x01;   // Set PB0 low
  delay(1000);
}