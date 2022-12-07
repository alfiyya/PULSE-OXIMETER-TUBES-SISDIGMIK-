//FOR  Pulse OXIMETER SENSOR 
#include <LiquidCrystal.h>
#include <Wire.h>
#include "MAX30100_PulseOximeter.h"


// FOR PUSH BUTTON WITH BUZZER 
#define REPORTING_PERIOD_MS     1000
#include "pitches.h"  //add note library


// to give song buzzer
//notes in the melody
int melody[]={NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4};

//digital pin 4 has a button attached to it. Give it an name
int buttonPin= 4;

//note durations. 4=quarter note / 8=eighth note for buzzer 
int noteDurations[]={4, 8, 8, 4, 4, 4, 4, 4};

//initiate pulse oximeter sensor
LiquidCrystal lcd(13, 12, 11, 10, 9, 8);
int ledPin = 2;
PulseOximeter pox;

uint32_t tsLastReport = 0;

void onBeatDetected()
{
  Serial.println("Beat!");
}

void setup()
{ 
    // Set Up buzzer with push button
    //make the button's pin input
    pinMode(buttonPin, INPUT);
    
    // Set up Pulse oximeter 
    Serial.begin(115200);
    Serial.print("Initializing pulse oximeter..");
    lcd.begin(16,2);
    lcd.print("Initializing...");
    delay(3000);
    lcd.clear();
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, HIGH);
    // Failure karena wiring atau kurang power supply
    if (!pox.begin()) {
        Serial.println("FAILED");
        digitalWrite(ledPin, LOW);
        for(;;);
    } else {
        Serial.println("SUCCESS");
    }
    pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);
}

void loop()
{ 
    // Void loop for pulse oximeter 
   // Make sure to call update as fast as possible
    pox.update();

    // Asynchronously dump heart rate and oxidation levels to the serial
    // For both, a value of 0 means "invalid"
    if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
        Serial.print("Heart rate:");
        Serial.print(pox.getHeartRate());
        Serial.print("bpm / SpO2:");
        Serial.print(pox.getSpO2());
        Serial.println("%");
        
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("BPM :");
        lcd.print(pox.getHeartRate());

        lcd.setCursor(0,1);
        lcd.print("Sp02: ");
        lcd.print(pox.getSpO2());
        lcd.print("%");

        tsLastReport = millis();
  }


  //void loop for buzzer 
  //read the input pin
  int buttonState = digitalRead(buttonPin);

  //if the button is pressed
  if (buttonState == 1){

    //iterate over the notes of the melody
    for (int thisNote=0; thisNote <8; thisNote++){

      //to calculate the note duration, take one second. Divided by the note type
      int noteDuration = 1000 / noteDurations [thisNote];
      tone(7, melody [thisNote], noteDuration);

      //to distinguish the notes, set a minimum time between them
      //the note's duration +30% seems to work well
      int pauseBetweenNotes = noteDuration * 1.30;
      delay(pauseBetweenNotes);

      //stop the tone playing
      noTone(7);
    }
  }
}
