#define IS_MODE_VIBRATION false
#define IS_MODE_DRAGGER true

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define SERVOMIN  150 // This is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  600 // This is the 'maximum' pulse length count (out of 4096)
#define SERVOCENTER 375
#define USMIN  400 // This is the rounded 'minimum' microsecond length based on the minimum pulse of 150
#define USMAX  2400 // This is the rounded 'maximum' microsecond length based on the maximum pulse of 600
#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates

const int SONG_INDEX = 0  ;

// A note contains 3 parameters in the following order
// 1) PIN ID
// 2) Duration (milliesconds) that the PIN is turned on for
// 3) Duration (milliesconds) that the PIN is turned off for until the next note is started
// Procedure is: PIN turned on for note[1] -> PIN turned off for note[2] -> next note

/*
C = Pin 2
D = Pin 4
E = Pin 6
F = Pin 8
G = Pin 1

Pins für A    => 4, 6, 8, 4, 6, 1,2, 6, 4, 2
Melodie A (0) => D, E, F, D, E, G, C, E, D, C -> 2 Viertel am Ende

Pins für B    => 2, 4, 6, 2, 1,8, 4, 8, 1,2 -> 1 Viertel am Ende
Melodie B (1) => C, D, E, C, G, F, D, F, G, C

Pins für C    => 6, 8, 6, 10,2, 6, 4, 2, 4, 2 -> 2 Viertel am Ende
Melodie C (2) => E, F, E, G, C, E, D, C, D, C

 */

// Durations for how long the note should be played
int quarter = 1065; // at 70bpm this is the duration for a quarter note
int eighth = 431; // at 70bpm this is the duration for a eighth note
int durationAfter = 50;
int durationBeforeRepitition = 20000;
 
int songs[3][10][3] = { // Melodie A
                        {{4, eighth, durationAfter}, {6, eighth, durationAfter}, {8, eighth, durationAfter}, {4, eighth, durationAfter}, {6, eighth, durationAfter}, {10, eighth, durationAfter}, {2, eighth, durationAfter}, {6, eighth, durationAfter}, {4, quarter, durationAfter}, {2, quarter, durationBeforeRepitition}},
                        // Melodie B
                        {{2, eighth, durationAfter}, {4, eighth, durationAfter}, {6, eighth, durationAfter}, {2, eighth, durationAfter}, {10, eighth, durationAfter}, {8, eighth, durationAfter}, {4, eighth, durationAfter}, {8, eighth, durationAfter}, {10, eighth, durationAfter}, {2, quarter, durationBeforeRepitition}},
                        // Melodie C
                        {{6, eighth, durationAfter}, {8, eighth, durationAfter}, {6, eighth, durationAfter}, {10, eighth, durationAfter}, {2, eighth, durationAfter}, {6, eighth, durationAfter}, {4, eighth, durationAfter}, {2, eighth, durationAfter}, {4, quarter, durationAfter}, {2, quarter, durationBeforeRepitition}},
                     };

                     
void setup() {
  // Initialize Serial connection
  Serial.begin(9600);

  // Initialization of output HW for output
  if(IS_MODE_VIBRATION){
      // For Vibration Motors (VIBRATION)
      for(int i = 0; i < 1; i++){
        Serial.println("Initializing Pin " + songs[SONG_INDEX][i][0]);
        pinMode(songs[SONG_INDEX][i][0], OUTPUT);
      }
  }else{
      // For Servo Motors (TAPPER) && DRAGGER since they both use servo motors
      // Configure PWM on start
      pwm.begin();
      pwm.setOscillatorFrequency(27000000);
      pwm.setPWMFreq(SERVO_FREQ);  // Analog servos run at ~50 Hz updates
      delay(1);
  }
}

void loop() {
  if(IS_MODE_VIBRATION){
    // Iterate through each note of the song
    for(int i = 0; i < 10; i++){
      // Store values in properly readable variables
      int pin = songs[SONG_INDEX][i][0];
      int duration_on_ms = songs[SONG_INDEX][i][1];
      int duration_off_ms = songs[SONG_INDEX][i][2];
  
      Serial.print("Turning on pin ");
      Serial.print(pin);
      Serial.print(" duration_on = ");
      Serial.println(duration_on_ms);
  
      // Set the current pin to HIGH
      digitalWrite(pin, HIGH);
      // Wait for the specified (on) duration
      delay(duration_on_ms);
      // Set the current pin to LOW
      digitalWrite(pin, LOW);
      // Wait for the specified (off) duration
      delay(duration_off_ms);
    }  
  }else if(IS_MODE_DRAGGER){
    // Implement playing of the note for DRAGGER!
    // Same functionality since they both use servo motors, only 
    // difference seems to be the hardware setup at the moment
    for(int i = 0; i < 10; i++){
      int servonum = songs[SONG_INDEX][i][0];
      int duration_on_ms = songs[SONG_INDEX][i][1];
      int duration_off_ms = songs[SONG_INDEX][i][2]; 

      // Debug Output
      Serial.print("Turning on servo number ");
      Serial.print(servonum);
      Serial.print(", duration_on = ");
      Serial.println(duration_on_ms);

      // Move dragger to center into startposition (= on skin)
      for (uint16_t pulselen = SERVOMAX; pulselen > 250; pulselen--) {
        pwm.setPWM(servonum, 0, pulselen);
      }
      // Keep tapper in on position for specified duration
      delay(duration_on_ms);

      // Move tapper into end position (= on skin)
      for (uint16_t pulselen = 250; pulselen < SERVOMAX; pulselen++) {
        pwm.setPWM(servonum, 0, pulselen);
      }


      // Keep tapper in off position for specified duration
      delay(duration_off_ms);
    }
  }else{
    // Implement playing of the note for TAPPER & DRAGGER!
    // Same functionality since they both use servo motors, only 
    // difference seems to be the hardware setup at the moment
    for(int i = 0; i < 10; i++){
      int servonum = songs[SONG_INDEX][i][0];
      int duration_on_ms = songs[SONG_INDEX][i][1];
      int duration_off_ms = songs[SONG_INDEX][i][2]; 

      // Debug Output
      Serial.print("Turning on servo number ");
      Serial.print(servonum);
      Serial.print(", duration_on = ");
      Serial.println(duration_on_ms);

      // Move tapper into startposition (= off skin)
      for (uint16_t pulselen = SERVOMAX; pulselen > SERVOMIN; pulselen--) {
        pwm.setPWM(servonum, 0, pulselen);
      }
      // Keep tapper in on position for specified duration
      delay(duration_on_ms);

      // Move tapper into end position (= on skin)
      for (uint16_t pulselen = SERVOMIN; pulselen < SERVOMAX; pulselen++) {
        pwm.setPWM(servonum, 0, pulselen);
      }


      // Keep tapper in off position for specified duration
      delay(duration_off_ms);
    }
 }
}
