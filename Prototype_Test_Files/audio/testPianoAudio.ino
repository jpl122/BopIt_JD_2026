
#include <Arduino.h>
#include <Wire.h>
#include <DFRobotDFPlayerMini.h>

// DJ BOP-IT
// 12-key capacitive piano audio prototype
// ESP32-S3 development board

#define SDA_PIN 8
#define SCL_PIN 9

#define AUDIO_RX 10
#define AUDIO_TX 11

#define MPR_ADDR 0x5A
#define NUM_PADS 12

const int TOUCH_THRESHOLD = 10;
const int RELEASE_THRESHOLD = 5;

bool padState[NUM_PADS] = {};
uint8_t padCount[NUM_PADS] = {};

HardwareSerial audioSerial(1);
DFRobotDFPlayerMini player;

const char* noteNames[NUM_PADS] = {
  "F4", "F#4", "G4", "G#4",
  "A4", "A#4", "B4", "C5",
  "C#5", "D5", "D#5", "E5"
};

// Assumes E0 corresponds to physical key 1.
// Adjust after verifying the keyboard connector.
const uint8_t trackMap[NUM_PADS] = {
  1, 2, 3, 4, 5, 6,
  7, 8, 9, 10, 11, 12
};

// ---------- MPR121 REGISTER FUNCTIONS ----------

bool writeReg(uint8_t reg, uint8_t data) {
  Wire.beginTransmission(MPR_ADDR);
  Wire.write(reg);
  Wire.write(data);

  return Wire.endTransmission() == 0;
}

bool readReg8(uint8_t reg, uint8_t &data) {
  Wire.beginTransmission(MPR_ADDR);
  Wire.write(reg);

  if (Wire.endTransmission(false) != 0)
    return false;

  if (Wire.requestFrom(MPR_ADDR, 1) != 1)
    return false;

  data = Wire.read();
  return true;
}

bool readReg16(uint8_t reg, uint16_t &data) {
  Wire.beginTransmission(MPR_ADDR);
  Wire.write(reg);

  if (Wire.endTransmission(false) != 0)
    return false;

  if (Wire.requestFrom(MPR_ADDR, 2) != 2)
    return false;

  uint8_t low = Wire.read();
  uint8_t high = Wire.read();

  data = low | ((uint16_t)high << 8);

  return true;
}

// ---------- INITIALIZE MPR121 ----------

bool initializeMPR121() {

  if (!writeReg(0x80, 0x63)) return false;
  delay(10);

  if (!writeReg(0x5E, 0x00)) return false;

  // Baseline filters
  if (!writeReg(0x2B, 0x01)) return false;
  if (!writeReg(0x2C, 0x01)) return false;
  if (!writeReg(0x2D, 0x0E)) return false;
  if (!writeReg(0x2E, 0x00)) return false;

  if (!writeReg(0x2F, 0x01)) return false;
  if (!writeReg(0x30, 0x05)) return false;
  if (!writeReg(0x31, 0x01)) return false;
  if (!writeReg(0x32, 0x00)) return false;

  if (!writeReg(0x33, 0x00)) return false;
  if (!writeReg(0x34, 0x00)) return false;
  if (!writeReg(0x35, 0x00)) return false;

  // Configure all 12 electrodes
  for (int i = 0; i < NUM_PADS; i++) {
    if (!writeReg(0x41 + 2*i, 12))
      return false;

    if (!writeReg(0x42 + 2*i, 6))
      return false;
  }

  if (!writeReg(0x5B, 0x00)) return false;
  if (!writeReg(0x5C, 0x10)) return false;
  if (!writeReg(0x5D, 0x20)) return false;

  // Enable E0 through E11
  if (!writeReg(0x5E, 0x8C)) return false;

  delay(400);

  return true;
}

// ---------- READ TOUCH DELTA ----------

bool readPadDelta(int pad, int &delta) {

  uint16_t rawFiltered;
  uint8_t rawBaseline;

  if (!readReg16(0x04 + 2*pad,
                 rawFiltered))
    return false;

  if (!readReg8(0x1E + pad,
                rawBaseline))
    return false;

  int filtered = rawFiltered & 0x03FF;
  int baseline = ((int)rawBaseline) << 2;

  delta = baseline - filtered;

  return true;
}

// ---------- AUDIO PLAYBACK ----------

void playPadSound(int pad) {

  int track = trackMap[pad];

  Serial.print("Electrode E");
  Serial.print(pad);

  Serial.print(" | Note: ");
  Serial.print(noteNames[track - 1]);

  Serial.print(" | Track: ");
  Serial.println(track);

  player.playMp3Folder(track);
}

// ---------- TOUCH DETECTION ----------

void readPads() {

  for (int i = 0; i < NUM_PADS; i++) {

    int delta;

    if (!readPadDelta(i, delta)) {
      padCount[i] = 0;
      continue;
    }

    if (!padState[i]) {

      if (delta >= TOUCH_THRESHOLD) {

        if (++padCount[i] >= 2) {
          padState[i] = true;
          padCount[i] = 0;

          playPadSound(i);
        }

      } else {
        padCount[i] = 0;
      }

    } else {

      if (delta <= RELEASE_THRESHOLD) {

        if (++padCount[i] >= 2) {
          padState[i] = false;
          padCount[i] = 0;

          Serial.print("E");
          Serial.print(i);
          Serial.println(" RELEASED");
        }

      } else {
        padCount[i] = 0;
      }
    }
  }
}

// ---------- SETUP ----------

void setup() {

  Serial.begin(115200);
  delay(1000);

  Wire.begin(SDA_PIN, SCL_PIN);
  Wire.setClock(100000);
  Wire.setTimeOut(50);

  Serial.println("DJ BOP-IT PIANO TEST");
  Serial.println("Keep all keys untouched.");

  if (!initializeMPR121()) {
    Serial.println("MPR121 initialization failed.");

    while (true) {
      delay(1000);
    }
  }

  Serial.println("MPR121 connected!");
  Serial.println("12 electrodes enabled.");

  audioSerial.begin(
    9600, SERIAL_8N1,
    AUDIO_RX, AUDIO_TX
  );

  delay(1500);

  if (!player.begin(audioSerial)) {
    Serial.println("DFPlayer initialization failed.");

    while (true) {
      delay(1000);
    }
  }

  player.volume(10);

  Serial.println("DFPlayer connected!");
  Serial.println("PIANO READY!");
  Serial.println("Touch any key.");
}

// ---------- MAIN LOOP ----------

void loop() {

  readPads();
  delay(10);
}
