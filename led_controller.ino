#include <FastLED.h>

#define NUM_LEDS 100

#define DATA_PIN D7
#define CLOCK_PIN 13
CRGB leds[NUM_LEDS];

#define LINE_BUF_SIZE 128   //Maximum input string length
char line[LINE_BUF_SIZE];

void setup() { 
    // Uncomment/edit one of the following lines for your leds arrangement.
    // ## Clockless types ##
    FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);  // GRB ordering is assumed
    FastLED.setBrightness(128);
 
    Serial.begin(115200);
    Serial.println("UART to WS2812 LED Driver.");
}
 
void loop() {
  read_line();
  switch(line[0]){
    case 0x00:
      break;
    case 'B':
      unsigned long int brightness;
      brightness = getArgument();
      Serial.print("Setting brightness: ");
      Serial.println(brightness);
      FastLED.setBrightness(brightness);
      break;
    case 'C':
      unsigned long int ledAndColor;
      ledAndColor = getArgument();
      int led = (ledAndColor & 0xFF000000) >> 24;
      leds[led].red = (ledAndColor & 0x00FF0000) >> 16;
      leds[led].green = (ledAndColor & 0x0000FF00) >> 8;
      leds[led].blue = (ledAndColor & 0x000000FF);
      break;
  }
  
  FastLED.show();
  delay(1);
//  // Now turn the LED off, then pause
//  for(int i=0;i<NUM_LEDS;i++)
//    leds[i] = CRGB::Black;
//  FastLED.show();
//  delay(500);
}

void read_line(){
    String line_string;
    memset(line, 0, sizeof(line));
    long timeStart = millis();
    while(!Serial.available()){
      delay(1);
      if(millis() - timeStart > 10){
        return;
      }
    }
 
    if(Serial.available()){
        line_string = Serial.readStringUntil('\n');
        if(line_string.length() < LINE_BUF_SIZE){
          line_string.toCharArray(line, LINE_BUF_SIZE);
          Serial.println(line_string);
        }
        else{
          Serial.println("Input string too long.");
        }
    }    
}

unsigned long int getArgument(){
    unsigned long int number = 0x00;
    for(int i = 0; i<8;i++){
      number = number << 4;      
      number += (line[3+i] < '9') ? line[3+i] - '0' : line[3+i] - '7';
    }
    
    return number;
}
