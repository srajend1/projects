#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"
#include "neopixel.h"

#include "Bus71A0.h"
#include "Bus71A5.h"
#include "Bus71A10.h"
#include "Bus71A15.h"

#include "Bus71C0.h"
#include "Bus71C5.h"
#include "Bus71C10.h"
#include "Bus71C15.h"

//#include "Min3.h"
#include "PhotonWaveOut.h"
//OLED CONNECTIONS
#define OLED_DC     D3 //green
#define OLED_CS     D4 //blue
#define OLED_RESET  D5 //orange
#define OLED_D0     A3 //SCK //yellow
#define OLED_D1     A5 //MOSI // white
#define RESET       D6
Adafruit_SSD1306 display(OLED_DC, OLED_RESET, OLED_CS);

//NeoPixel CONNECTIONS
#define PIXEL_PIN   D2
#define PIXEL_COUNT 16
#define PIXEL_TYPE  WS2812B

#define NO_OF_BUSES 3
#define BUS_71A 0
#define BUS_71C 1
#define BUS_82 2

#define DEBUG false
#define SPEAKER_PIN D0
#define BUTTON_PIN  D6

const int pwm_p = D0;
const int pwm_n = D1;

int buttonState = HIGH;
int buttonStatePrev = HIGH;
int etas[3] = {-1, -1, -1};
WaveOut *wave;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, PIXEL_TYPE);

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
int Wheel(byte WheelPos) {
  if (WheelPos < 85) {
    return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
    WheelPos -= 170;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}




void rainbow(int numberOfBuses) {
  int i, j, k;
  for (k=0; k<2; k++) {
    for (j=0; j<250; j++) {
      for (i=0; i<strip.numPixels(); i++) {
        strip.setPixelColor(i, Wheel((i+j) & 255));
      }
      strip.show();
      delay(30/numberOfBuses);
    }
  }
}

void minutesToLEDs(int mins, int numberOfBuses) {
  int noOfLedsToGlow , id, green, red;
  if ( mins == 0)
    {
      //Bus is Approaching
      rainbow(numberOfBuses);
    }
  else
      {
        if ( mins >= strip.numPixels())
        {
          //Bus is too far
        noOfLedsToGlow = 0;
        }

        else {
          noOfLedsToGlow = strip.numPixels() - mins;
          }

          for (id = 0; id <strip.numPixels(); id ++)
          {
            if (id < noOfLedsToGlow)
          {
            //red = (16 * (16 - id )) - 1;
            //green = 17 * id;
            //--------------------
              if(id < 5){
                red = 255;
                green = 0;
              }else if(id >=5 && id < 10){
                red = 150;
                green = 80;
              }else {
                green = 255;
                red = 0;
              }
            //--------------------
            strip.setPixelColor(id, strip.Color(red, green , 0));
          }
          else
          {
            strip.setPixelColor(id, strip.Color(0, 0 ,0));
          }
        }
    strip.show();
    delay(15000/numberOfBuses);
  }
}



void playAudio(String bus, int eta)
{
  if (bus.compareTo("71A") == 0 && eta != -1){
    Serial.println("Playing audio" + String(eta));
    switch(eta){
      case 0:
      case 1:
          wave->play(Bus71A0_data, sizeof(Bus71A0_data), false);
          break;
      case 2:
      case 3:
      case 4:
      case 5:
          wave->play(Bus71A5_data, sizeof(Bus71A5_data), false);
          break;
      case 6:
      case 7:
      case 8:
      case 9:
      case 10:
          wave->play(Bus71A10_data, sizeof(Bus71A10_data), false);
          break;

      /*case 11:
      case 12:
      case 13:
      case 14:
      case 15:
          wave->play(Bus71A15_data, sizeof(Bus71A15_data), false);
          break;*/
      default:
        break;
    }
  }
}

void storeETAs(String bus, int eta){
    if(bus.compareTo("71A") == 0 ){
      etas[BUS_71A] = eta;
      Serial.println("Storing 71A data " +  String(eta));
    }else if(bus.compareTo("71C") == 0){
      etas[BUS_71C] = eta;
    }else if(bus.compareTo("82") == 0){
      etas[BUS_82] = eta;
    }
}

int monitorButton(){

    buttonState = digitalRead(BUTTON_PIN);
    //if (DEBUG) Serial.println("Button State: " + String(buttonState));
    //if (DEBUG) Serial.println("Button State Previous State: " + String(buttonStatePrev));
    if(buttonState == LOW){
          Serial.println("Button Was Pressed");
          playAudio("71A", etas[BUS_71A]);
    }
    //delay(50); // prevent bounce
    buttonStatePrev = buttonState;
    //delay(50);
}


void setup()   {
  display.begin(SSD1306_SWITCHCAPVCC); // initializing OLED function
  display.clearDisplay();   // clears the screen and buffer
  display.display();   //actually display

  Particle.subscribe("PATdisplay", myHandler);

  strip.begin(); // initialize neopixel
  strip.show(); // Initialize all pixels to 'off'

  wave = new WaveOut(pwm_p, pwm_n);

  if (DEBUG) Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(SPEAKER_PIN, OUTPUT);

  attachInterrupt(BUTTON_PIN, monitorButton, FALLING);
  Serial.begin(9600);
}

void loop() {
  monitorButton();
}

void myHandler(const char *event, const char *data)
{
  int startPos = 0;
  int posOfNL = 0;
  int posOfDash = 0;
  int numberOfBuses = 0;
  String bus;
  String eta;

  String patData = data;
  //Serial.println("Input: " + patData);   // Print to Serial Monitor

  //Find NumberOfBuses at this stop -> Useful for determining rotation time
  while(startPos < patData.length())
  {
    posOfNL = patData.indexOf('\n', startPos);
    numberOfBuses++;
    startPos = posOfNL + 1 ;
  }

  Serial.println("NumberOfBuses:" + String(numberOfBuses));   // Print to Serial Monitor
  startPos = 0;
  while(startPos < patData.length())
  {
    posOfNL = patData.indexOf('\n', startPos);
    String sub = patData.substring(startPos, posOfNL);
    //Serial.println("posOfNL:" + String(posOfNL) + " substring:" + sub);

    posOfDash = sub.indexOf('-');
    bus = sub.substring(0,posOfDash);

    Serial.println("bus = " + bus);
    eta = sub.substring(posOfDash + 1);
    //Serial.println("eta = " + eta);
    storeETAs(bus, eta.toInt());
    //playAudio(bus,eta.toInt());

    //Serial.println("Displaying");
    display.clearDisplay();       // clear screen
    display.setTextSize(6);       // set text size
    display.setTextColor(WHITE);  // set text colour
    display.setCursor(20,5);       // set cursor 'start' position on screen
    display.println(bus);        // prints data received by myhandler i.e. from the PATdisplay event

    display.setTextSize(1);       // set text size
    display.setTextColor(WHITE);  // set text colour
    display.setCursor(20,50);       // set cursor 'start' position on screen
    //display.println(eta);

    display.display();            // to actually display it on the screen, call the display() function
    minutesToLEDs(eta.toInt(), numberOfBuses);

    startPos = posOfNL + 1 ;
  }

}
