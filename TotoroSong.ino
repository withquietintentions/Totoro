/* Sweep
  A0 (a.k.a D12) - This is a special pin that can do true analog output so it's great for playing audio clips. In can be digital I/O, or analog I/O, but if you do that it will interfere with the built-in speaker. This is the one pin that cannot be used for capacitive touch.
  A1 / D6 - This pin can be digital I/O, or analog Input. This pin has PWM output and can be capacitive touch sensor
  A2 / D9 - This pin can be digital I/O, or analog Input. This pin has PWM output and can be capacitive touch sensor
  A3 / D10 - This pin can be digital I/O, or analog Input. This pin has PWM output and can be capacitive touch sensor
  A4 / D3 - This pin can be digital I/O, or analog Input. This pin is also the I2C SCL pin, and can be capacitive touch sensor
  A5 / D2 - This pin can be digital I/O, or analog Input. This pin is also the I2C SDA pin, and can be capacitive touch sensor
  A6 / D0 - This pin can be digital I/O, or analog Input. This pin has PWM output, Serial Receive, and can be capacitive touch sensor
  A7 / D1 - This pin can be digital I/O, or analog Input. This pin has PWM output, Serial Transmit, and can be capacitive touch sensor
*/
#include <Adafruit_CircuitPlayground.h>

// A few music note frequencies as defined in this tone example:
//   https://www.arduino.cc/en/Tutorial/toneMelody

#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978

// Define note durations.  You only need to adjust the whole note
// time and other notes will be subdivided from it directly.
#define WHOLE         1800 //2200      // Length of time in milliseconds of a whole note (i.e. a full bar).
#define HALF          WHOLE/2
#define QUARTER       HALF/2
#define EIGHTH        QUARTER/2
#define EIGHTH_TRIPLE QUARTER/3
#define SIXTEENTH     EIGHTH/2



#include <Servo.h>
Servo myservo;  // create servo object to control a servo
int pos = 1;    // variable to store the servo position

//attached
const int totoroMotor = 9;//A2
const int treesMotor = 6;//A1
const int lights = 0; //A6 lights outside
const int lightsInside = 10;//A3
//buttons
const int lightInsideButton = 1;//A7 inside light
const int treesButton = 2;//A5
const int lightButton= 12; //A0 outside
const int totoroButton = 3;//A4



bool lightsOn = false;
int brightness = 0;
bool totoroOn = false;
// Variables will change:
int treesPushCounter = 0;   // counter for the number of button presses
int treesButtonState = 0;         // current state of the button
int treeslastButtonState = 0;     // previous state of the button
bool moveTrees = false;
bool insideLightsOn = false;

void setup() {
  Serial.begin(9600);
  myservo.attach(treesMotor);  // attaches the tree servos to pin A1
  pinMode(totoroMotor, OUTPUT);//totoro motor
  pinMode(lightButton, INPUT_PULLUP);//magnet2
  pinMode(treesButton, INPUT_PULLUP);// magnet 1
  pinMode(totoroButton, INPUT_PULLUP);//soft toggle 
  pinMode(lightInsideButton, INPUT_PULLUP);//in roof magnet
  pinMode(lightsInside, OUTPUT);
  pinMode(lights, OUTPUT); //

  //sets all to off
  lightsOn = false;
  moveTrees = false;
  totoroOn = false;
  insideLightsOn = false;
  

}

void loop() {
  int totoroVal = digitalRead(totoroButton);
  int lightVal = digitalRead(lightButton);
  int treesVal = digitalRead(treesButton);
  int insideLightVal = digitalRead(lightInsideButton);
  Serial.print("  treesVal: ");
  Serial.print(treesVal);
   Serial.print("  outside light: ");
  Serial.println(lightVal);
   Serial.print("  insideLightVal: ");
  Serial.println(insideLightVal); 
  

//inside light
  
  if (insideLightVal == LOW && insideLightsOn == false) {
    for (int fadeValue = 0 ; fadeValue <= 255; fadeValue += 5) {
      // sets the value (range from 0 to 255):
      analogWrite(lightsInside, fadeValue);
      // wait for 30 milliseconds to see the dimming effect
      delay(30);
    }
    insideLightsOn = true;
  }

  else if (insideLightVal == HIGH && insideLightsOn == true) {
    // fade out from max to min in increments of 5 points:
    for (int fadeValue = 255 ; fadeValue >= 0; fadeValue -= 5) {
      // sets the value (range from 0 to 255):
      analogWrite(lightsInside, fadeValue);
      // wait for 30 milliseconds to see the dimming effect
      delay(30);
    }
    insideLightsOn = false;
  }

  
  ///totoro spinning
  if (totoroVal == LOW) {
    analogWrite(totoroMotor, 100);//totoro motor
    torotoSong();
  }
  else {
    analogWrite(totoroMotor, 0);//totoro motor
  }



  ///Trees moving
  if (treesVal == LOW) {
    digitalWrite(0, HIGH);
    //lightsOutside();
    for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
      // in steps of 1 degree
      myservo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(15);
      treesVal = digitalRead(treesButton);// waits 15 ms for the servo to reach the position
      if (treesVal == HIGH) {     // bail out on sensor detect
        pos = pos;
        break;
      }
    }
    for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
      myservo.write(pos);  
     // tell servo to go to position in variable 'pos'
      delay(15);
      treesVal = digitalRead(treesButton);// waits 15 ms for the servo to reach the position
      if (treesVal == HIGH) {     // bail out on sensor detect
        pos = pos;
        break;
      }
    }

  }
  else {

digitalWrite(0, LOW);
   
  }
  //debounce
  delay(10);
}

void torotoSong() {
  delay(EIGHTH);
  CircuitPlayground.playTone(NOTE_G4, QUARTER);
  CircuitPlayground.playTone(NOTE_E4, EIGHTH);
  CircuitPlayground.playTone(NOTE_C4, QUARTER);
  CircuitPlayground.playTone(NOTE_G4, QUARTER);
  CircuitPlayground.playTone(NOTE_F4, QUARTER);
  CircuitPlayground.playTone(NOTE_D4, QUARTER);
  CircuitPlayground.playTone(NOTE_G4, HALF);
  delay(EIGHTH);
  CircuitPlayground.playTone(NOTE_F5, QUARTER);
  CircuitPlayground.playTone(NOTE_D5, EIGHTH);
  CircuitPlayground.playTone(NOTE_B4, QUARTER);
  CircuitPlayground.playTone(NOTE_F5, QUARTER);
  CircuitPlayground.playTone(NOTE_E5, QUARTER);
  CircuitPlayground.playTone(NOTE_C5, QUARTER);
}

void lightsOutside(){
 for (int fadeValue = 0 ; fadeValue <= 255; fadeValue += 5) {
    // sets the value (range from 0 to 255):
    analogWrite(lights, fadeValue);
     // wait for 30 milliseconds to see the dimming effect
     delay(30);
   }
   
}
