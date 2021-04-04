/*******
Body Centric Technologies - OCAD U - Winter 2021
Vibrating Knee Brace : By Patricia Mwenda
This project uses a circuit playground express with vibrating motors attached on a knee brace for use during 
physio therapy for people with knee pains. 
Created April 4th 2021

Based on:
Code for (Gaming Wearables) Glowing LED Team Badge for Pokemon Go by Richard Albritton - Adafruit
July 19, 2016 - (updated) Feb 18, 2021.
Aside from the color LED animation, pulsing the NeoPixels from light to dark and back, the code is looking 
for a button press and readings from the Accelerometer with added adition of vibrating motors.
*********/

/**Must include the Adafruit_CircuitPlayground.h**/
#include <Adafruit_CircuitPlayground.h>

// Variables will change:
int ledState = 0;            // the current state of the output pin
int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin
int ColorState;              // the previous Color set
int R = 0;
int G = 255;
int B = 0;
int Roffset;
int Goffset;
int Boffset;
int FadeDir = 1;
int FadeTurn =0;
int FadeRate = 50;

// the following variables are long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 50;    // the debounce time; increase if the output flickers
int Sensor;

int vibePin1 = 1; //Bottom Left Motor
int vibePin2 = 2; //Top Left Motor
int vibePin3 = 6; //Bottom Right Motor
int vibePin4 = 9; //Top Right Motor

void setup() {
  CircuitPlayground.begin();
  CircuitPlayground.setBrightness(155);

  //initialize digital pins
  pinMode(vibePin1,OUTPUT);
  pinMode(vibePin2,OUTPUT);
  pinMode(vibePin3,OUTPUT);
  pinMode(vibePin4,OUTPUT);

}

void loop() {

  // read the state of the button into a local variable:
  int reading = CircuitPlayground.rightButton();


  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH),  and you've waited
  // long enough since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;


      // only toggle the LED if the new button state is HIGH
      if (buttonState == HIGH) {
              ++ledState;
              FadeTurn = 0;
      }
    }
  }
  Sensor = abs(CircuitPlayground.motionX())+abs(CircuitPlayground.motionY())+abs(CircuitPlayground.motionZ());
  Serial.print("New Len: "); Serial.println(Sensor);
  if(Sensor>19)ledState=5;

  // set the LED:
  // Red, Amber, Greem (inspired by the traffic light functions)
  // do something different depending on the
  // range value:
  switch (ledState) {
    case 0:    // Red No Motor Vibrates
      R = 255;
      G = 0;
      B = 0;
      ColorState=0;
      digitalWrite(vibePin1, LOW);
      digitalWrite(vibePin2, LOW);
      digitalWrite(vibePin3, LOW);
      digitalWrite(vibePin4, LOW);
      break;
    case 1:    // Yellow Top Motors Vibrate
      R = 128;
      G = 128;
      B = 0;
      ColorState=1;
      digitalWrite(vibePin1, LOW);
      digitalWrite(vibePin2, HIGH);
      digitalWrite(vibePin3, LOW);
      digitalWrite(vibePin4, HIGH);
      break;
    case 2:    // Orange Bottom Motors Vibrate
      R = 255;
      G = 69;
      B = 0;
      ColorState=2;
      digitalWrite(vibePin1, HIGH);
      digitalWrite(vibePin2, LOW);
      digitalWrite(vibePin3, HIGH);
      digitalWrite(vibePin4, LOW);
      break;
    case 3:    // Green All Buttons Vibrate
      R = 0;
      G = 255;
      B = 0;
      ColorState=3;
      digitalWrite(vibePin1, HIGH);
      digitalWrite(vibePin2, HIGH);
      digitalWrite(vibePin3, HIGH);
      digitalWrite(vibePin4, HIGH);
      break;
    case 4:    // Return to Red No Motor Vibrates
      ledState = 0;
      digitalWrite(vibePin1, LOW);
      digitalWrite(vibePin2, LOW);
      digitalWrite(vibePin3, LOW);
      digitalWrite(vibePin4, LOW);
      break;
    case 5:    // Walking detected - All Motors Vibrate
      ColorFill(0, 128, 128);
      delay(300);
      ColorFill(R/20, G/20, B/20);
      delay(300);
      ledState=ColorState;
      digitalWrite(vibePin1, HIGH);
      digitalWrite(vibePin2, HIGH);
      digitalWrite(vibePin3, HIGH);
      digitalWrite(vibePin4, HIGH);
      break;
  }
  delay(1);        // delay in between reads for stability
  Roffset=R/FadeRate;
  Goffset=G/FadeRate;
  Boffset=B/FadeRate;
  if (FadeDir){
    FadeTurn++;  
    ColorFill(R-(Roffset*FadeTurn), G-(Goffset*FadeTurn), B-(Boffset*FadeTurn));
    if(FadeTurn>FadeRate){
       FadeDir = 0;
       //FadeTurn = 0;
    }
  } else{
    FadeTurn--;  
    ColorFill(R-(Roffset*FadeTurn), G-(Goffset*FadeTurn), B-(Boffset*FadeTurn));
    if(FadeTurn<1){
       FadeDir = 1;
       //FadeTurn = 0;
    }
  }

  // save the reading.  Next time through the loop,
  // it'll be the lastButtonState:
  lastButtonState = reading;

}

void ColorFill(uint16_t r,uint16_t g,uint16_t b){
  for(uint16_t i=0; i<10; i++) {
    CircuitPlayground.setPixelColor(i, r, g, b);
  }
}
