// NeoPixelTest
// This example will cycle between showing four pixels as Red, Green, Blue, White
// and then showing those pixels as Black.
//
// Included but commented out are examples of configuring a NeoPixelBus for
// different color order including an extra white channel, different data speeds, and
// for Esp8266 different methods to send the data.
// NOTE: You will need to make sure to pick the one for your platform 
//
//
// There is serial output of the current state so you can confirm and follow along
//

#include <NeoPixelBus.h>

const uint16_t PixelCount = 60; // this example assumes 4 pixels, making it smaller will cause a failure
const uint8_t PixelPin = 2;  // make sure to set this to the correct pin, ignored for Esp8266

#define colorSaturation 128

NeoPixelBus<NeoBrgFeature, Neo800KbpsMethod> strip(PixelCount, PixelPin);





namespace colorFade {
  uint8_t prev_red = 0;
  uint8_t prev_green = 0;
  uint8_t prev_blue = 0;
  uint8_t next_red = 0;
  uint8_t next_green = 0;
  uint8_t next_blue = 0;
  uint8_t r = 0;
  uint8_t g = 0;
  uint8_t b = 0;
  
  unsigned long starttime = 0;
  unsigned long endtime = 0;
  double progress = 1.0;

  void calcProgress() {
    unsigned long currenttime = millis();
    unsigned long remaining = 0;
    unsigned long delta = currenttime - starttime;
    
    if(endtime > currenttime) {
      remaining = endtime - currenttime;
      progress = (delta * 1.0) / ((remaining + delta) * 1.0);
    } else {
      progress = 1.0;
    }


    if(progress < 0) {
      progress = 0;
    }

    if (progress >= 1.0) {
      progress = 1.0;
    }
    
  }

  

  void setColor () {
    RgbColor color(r, g, b);
     for(uint8_t i=0; i < PixelCount; i++) {
      strip.SetPixelColor(i, color);
     }
    strip.Show();
  }

  uint8_t protectChannel(uint8_t& input) {
    uint8_t lower = 0;
    uint8_t upper = 255;
    if(input > upper) {
      input = upper;
    } 
    if(input < lower ) {
      input = lower;
    }
    return input;
  }

  void protectAll() {
    protectChannel(r);
    protectChannel(g);
    protectChannel(b);
    protectChannel(next_red);
    protectChannel(next_green);
    protectChannel(next_blue);
  }

  void updateColor () {
    calcProgress();
    int delta_red = next_red - (prev_red);
    int delta_green = next_green - (prev_green);
    int delta_blue = next_blue - (prev_blue);
    r = prev_red + progress * delta_red;
    g = prev_green + progress * delta_green;
    b = prev_blue + progress * delta_blue;      
    protectAll();
    setColor();
  }

  void fadeColor(uint8_t red, uint8_t green, uint8_t blue, unsigned long duration) {
    prev_red = r;
    prev_green = g;
    prev_blue = b;
    next_red = red;
    next_green = green;
    next_blue = blue;
    starttime = millis();
    endtime = starttime + duration;
  }
}

namespace parser {  
  void colorFade(String redstr, String greenstr, String bluestr, String timestr) {
    uint8_t selector = 0;
    uint8_t red=0, green=0, blue=0;
    unsigned long timeul=0;

    char redbuff[redstr.length()+1];
    redstr.toCharArray(redbuff, redstr.length()+1);
    red = atoi(redbuff);

    char greenbuff[greenstr.length()+1];
    greenstr.toCharArray(greenbuff, greenstr.length()+1);
    green = atoi(greenbuff);

    char bluebuff[bluestr.length()+1];
    bluestr.toCharArray(bluebuff, bluestr.length()+1);
    blue = atoi(bluebuff);

    char timebuff[timestr.length()+1];
    timestr.toCharArray(timebuff, timestr.length()+1);
    timeul = atol(timebuff);
    
/*
    Serial.println("The data: ");
    Serial.println(red);
    Serial.println(green);
    Serial.println(blue);
    Serial.println(timebuff);
    Serial.println(timestr);
    Serial.println(timeul);
*/    
    if(timeul >= 0) {
      colorFade::fadeColor(red, green, blue, timeul);
    } else {
      
    }
  }
}

namespace readline {
  String line = "";
  bool newLine = false;

  String red = "";
  String green = "";
  String blue = "";
  String timestr = "";

  bool isNum(char c) {
    bool theBool = false;
    switch(c) {
      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
      theBool = true;
      break;
    }
    return theBool;
  }

  String readNum() {
    char c = Serial.read();
    String retstr = "";
    while(isNum(c)) {
      retstr += c;
      c = Serial.read();
    }
    return retstr;
  }
  
  void parseChar() {
    if(Serial.available() > 0) {
      char c = Serial.read();
      switch(c) {
        case 'R':
          red = readNum();
          break;
        case 'G':
          green = readNum();
          break;
        case 'B':
          blue = readNum();
          break;
        case 'T':
          timestr = readNum();
          break;
        case 'E':
          parser::colorFade(red, green, blue, timestr);
      }
      Serial.flush();
    }
  }

  
  String readLine() {
    String lastLine = line;
    line = "";
    newLine = false;
    return lastLine;
  }
}

void setup()
{
    Serial.begin(9600);

    Serial.flush();

    colorFade::prev_red = 0;
    colorFade::prev_green = 0;
    colorFade::prev_blue = 0;
    colorFade::next_red = 0;
    colorFade::next_green = 0;
    colorFade::next_blue = 0;

    // this resets all the neopixels to an off state
    strip.Begin();
    strip.Show();

    Serial.println();
    Serial.println("Ready to go!");
    
}

char receivedChar;
boolean newData = false;

void loop()
{
    
    
    colorFade::updateColor();
    readline::parseChar();

    delay(30);

}


