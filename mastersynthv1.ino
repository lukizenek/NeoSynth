#include "MIDIUSB.h"

#include <Keypad.h>


//Libraries and intigers used in the code

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
const int NB_BUTTONS = ROWS * COLS;
int anal = A5;
int ledPin = 13;
int potPin = A0;
float brightness = 0;    // how bright the LED is
float fadeAmount = 5;
int potmap;
int potVal = analogRead(potPin);//read data from potentiometer
const int PRESET = 8;
const int NB_NoteNumber = 12;
String preset_name[PRESET] = {"1", "2", "3", "4", "5", "6", "7", "8"};
int notes[PRESET][NB_NoteNumber] = {
  {12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23},
  {24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35},
  {36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47}, //Preset 1
  {48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59}, // Preset 2
  {60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71}, // Preset 3
  {72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83},  //Preset A
  {84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95},  //Preset 4
  {96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107},  //Preset 5
};

int velocity[PRESET][NB_NoteNumber] = {
  {70, 70, 70, 70, 100, 100, 100, 100, 100, 100, 100, 100}, //Preset 1
  {70, 70, 70, 70, 70, 70, 70, 70, 100, 70, 70, 70}, //Preset 2
  {100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100}, //Preset 3
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, //Preset A
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, //Preset 4
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, //Preset 5
};

const int velocity_button = 15; // The D key is used to change velocity if velocity changer is true
int VELOCITY_CHANGE; //Velocity when D is pressed (should probably be changeable)
bool velocity_changer[PRESET] = {true, true, true, false, false, false};
bool doneFlag = false;


#include <Keypad.h>
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

//Keypad pins
byte rowPins[ROWS] = {6, 5, 4, 7}; //connect to the row pinouts of the kpd
byte colPins[COLS] = {8, 9, 10, 11}; //connect to the column pinouts of the kpd
Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

//Keypad state : 1 = DOWN 3 = UP (there are other state which are unused here)
unsigned int buttons_state[NB_BUTTONS] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
bool preset_selector = false;


//Oled Display
#include <Wire.h> //I2C
#include <U8g2lib.h>
U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
String oled_line1;
String oled_line2;

//Preset
int selected_preset = 0;
void setup() {
  Serial.begin(115200);
  u8g2.begin();
  change_preset(selected_preset);
  pinMode(ledPin, OUTPUT);


}

//Change preset and display it on screen
//You need to press "*" again to validate the preset
void change_preset(int preset) {
  if (preset < PRESET) {
    selected_preset = preset;
    oled_line1 = "Octave";
    oled_line2 = preset_name[preset];
    printScreen();
    Serial.println(oled_line1);
    Serial.println(oled_line2);
  }
}
void change_preset(int preset) {
  if (preset < PRESET) {
    selected_preset = preset;
    oled_line1 = "Octave";
    oled_line2 = preset_name[preset];
    printScreen();
    Serial.println(oled_line1);
    Serial.println(oled_line2);
  }
}

  
}

void printScreen() {
  u8g2.setFont(u8g2_font_7x14_mr);
  u8g2.firstPage();
  do {
    u8g2.setCursor(0, 14);
    u8g2.println(oled_line1);  // write something to the internal memory
    u8g2.setCursor(0, 30);
    u8g2.println(oled_line2);  // write something to the internal memory
  }
  while ( u8g2.nextPage() );
}

void loop() {
  int potVal = analogRead(potPin);//read data from potentiometer
  potmap = map(potVal, 0, 1023, 0, 127);
  int current_velocity = potmap;
  //digitalWrite(ledPin, HIGH);
  if (doneFlag == false) {
    oled_line1 = "Welcome to";
    oled_line2 = "          SYNTH32";
    printScreen();
    delay(2000);
    doneFlag = true;  
    
  }
  if ((buttons_state[velocity_button] == 1) || (buttons_state[velocity_button] == 2)) {
              oled_line1 = "Volume: ";
              oled_line2 = potmap;
              printScreen();
    }
 
  if (kpd.getKeys()) {
    for (int i = 0; i < LIST_MAX; i++) // Scan the whole key list.
    {
      if ( kpd.key[i].stateChanged )   // Only find keys that have changed state.
      {
        buttons_state[kpd.key[i].kcode] = kpd.key[i].kstate;

        if (kpd.key[i].kstate == 1) { //DOWN
          Serial.print(kpd.key[i].kcode);
          Serial.print(":");
          Serial.println("1");
          

          //If key "#" is pressed
          if (kpd.key[i].kcode == 12) {
            //If preset selection is not active, we go to selection mode
            if (!preset_selector) {
              Serial.println("Selector");
              preset_selector = true;
              oled_line1 = "Selector";
              oled_line2 = "--------";
              printScreen();
            } else {
              //If preset selection is active, we return to midi mode
              Serial.println("Deselect");
              oled_line1 = preset_name[selected_preset];
              oled_line2 = "";
              printScreen();
              preset_selector = false;
            }
          } else {
            //If in selection mode we change preset
            if (preset_selector) {
              Serial.println("Change preset");
              change_preset(kpd.key[i].kcode);
            } else {
              //If in midi mode we play midi note
              key_down(kpd.key[i].kcode);
               digitalWrite(ledPin, HIGH);
               btntime = millis();
              Serial.println(kpd.key[i].kcode);
            }
          }
        }
        if (kpd.key[i].kstate == 3) { //UP
          Serial.print(kpd.key[i].kcode);
          Serial.print(":");
          Serial.println("0");
          // If key is up we send a midi note off
          key_up(kpd.key[i].kcode);
           digitalWrite(ledPin, LOW);
        }
      }
    }
  }
}

//Key down (pressed)
void key_down(int key) {
  int current_velocity = potmap;


  if (!velocity_changer[selected_preset]) {
    //Play note if velocity changer not set
    noteOn(0, notes[selected_preset][key], current_velocity);
  } else {
    //Else do not play note on velocity changer button (D)
    if (key != velocity_button) {
      noteOn(0, notes[selected_preset][key], current_velocity);
    }
  }

  //Display note and velocity on screen
  oled_line1 = preset_name[selected_preset];
  oled_line2 = "Note:" + String(notes[selected_preset][key]) + " " "V:" + String(current_velocity);
  printScreen();
  MidiUSB.flush();
}

void key_up(int key) {
  //digitalWrite(ledPin, LOW);
  //If velocity_changer
  if (!velocity_changer[selected_preset]) {
    noteOff(0, notes[selected_preset][key], 127);
  } else {
    if (key != velocity_button) {
      noteOff(0, notes[selected_preset][key], 127);
    }
  }
  MidiUSB.flush();
}

void noteOn(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOn);
}

void noteOff(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOff);
}

void controlChange(byte channel, byte control, byte value) {
  midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};
  MidiUSB.sendMIDI(event);
}
