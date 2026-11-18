#include <Arduino.h>
#include <DFRobotDFPlayerMini.h>

#define AUDIO_RX 10
#define AUDIO_TX 11

HardwareSerial audioSerial(1);
DFRobotDFPlayerMini player;

void setup() {
  Serial.begin(115200);

  audioSerial.begin(
    9600, SERIAL_8N1,
    AUDIO_RX, AUDIO_TX
  );

  delay(1500);

  Serial.println("Initializing DFPlayer...");

  if (!player.begin(audioSerial)) {
    Serial.println("DFPlayer initialization failed.");

    while (true) {
      delay(1000);
    }
  }

  Serial.println("DFPlayer connected!");
  player.volume(10);

  Serial.println("Playing 0001.mp3");
  player.playMp3Folder(1);
}

void loop() {
  // Enter p in Serial Monitor to replay.
  if (Serial.available()) {
    char input = Serial.read();

    if (input == 'p') {
      player.playMp3Folder(1);
      Serial.println("Playing sound!");
    }
  }
}
