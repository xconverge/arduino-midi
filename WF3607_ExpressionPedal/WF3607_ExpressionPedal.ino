#include "MIDIUSB.h"

// Expression pedal pin
#define expPin A0

int lastVal = 0;
int tempAnalog = 0;

void setup() { Serial.begin(300); }

void controlChange(byte channel, byte control, byte value) {
  midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};
  MidiUSB.sendMIDI(event);
}

void loop() {
  // Read analog value
  tempAnalog = analogRead(expPin);

  // Convert 10 bit to 7 bit
  tempAnalog = map(tempAnalog, 0, 1023, 0, 127);
  tempAnalog = constrain(tempAnalog, 0, 127);

  // Send pedal status
  if (tempAnalog != lastVal) {
    // CC 11   (Our default value for expression pedal is using
    //          MIDI control change number 11
    //          Change this to change control the change number)
    controlChange(1, 11, tempAnalog);
    // !Important, Flush after send
    MidiUSB.flush();
  }

  // Store current value to be used laters
  lastVal = tempAnalog;
  delay(5);
}
