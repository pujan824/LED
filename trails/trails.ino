#include "FastLED.h"

#if FASTLED_VERSION < 3001000
#error "Requires FastLED 3.1 or later; check github for latest code."
#endif

#define LED_DT 12                                             // Data pin to connect to the strip.
#define COLOR_ORDER BGR                                       
#define LED_TYPE WS2811                                       
#define NUM_LEDS 50                                          

uint8_t max_bright = 128;                                      
struct CRGB leds[NUM_LEDS];                                   
CRGBPalette16 currentPalette;                                
CRGBPalette16 targetPalette;                                  
TBlendType    currentBlending;                              

uint8_t    numdots =   4;                                    
uint8_t   thisfade =   2;                                    
uint8_t   thisdiff =  16;                                     
uint8_t    thishue =   0;                                 
uint8_t     curhue =   0;                                 
uint8_t    thissat = 255;                                     
uint8_t thisbright = 255;                                     
uint8_t   thisbeat =   5;                                   

void setup() {
  
  Serial.begin(9600);                                        
  delay(1000);                                           

  LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(leds, NUM_LEDS);    
  currentPalette  = CRGBPalette16(CRGB::Black);
  targetPalette   = RainbowColors_p;
  currentBlending = LINEARBLEND;  
  FastLED.setBrightness(max_bright);

  //power management set at 5V, 500mA.
  set_max_power_in_volts_and_milliamps(5, 1000);              
  
}  
void loop() {
  EVERY_N_MILLISECONDS(100) {                                
    uint8_t maxChanges = 24; 
    nblendPaletteTowardPalette(currentPalette, targetPalette, maxChanges);   
  }
 
  Sequence();
  juggle();
  FastLED.show();                         
  
} 
void juggle() {                                        
  
  curhue = thishue;                                           
  fadeToBlackBy(leds, NUM_LEDS, thisfade);
  
  for( int i = 0; i < numdots; i++) {
    leds[beatsin16(thisbeat+i+numdots,0,NUM_LEDS)] += ColorFromPalette(currentPalette, curhue , thisbright, currentBlending);  
    curhue += thisdiff;
  }
  
} 
void Sequence() {                                             
  
  uint8_t secondHand = (millis() / 1000) % 30;                
  static uint8_t lastSecond = 99;                             
  
  if (lastSecond != secondHand) {                             
    lastSecond = secondHand;
    switch(secondHand) {
      case  0: numdots = 1; thisbeat = 20; thisdiff = 16; thisfade = 2; thishue = 0; break;                  
      case 10: numdots = 4; thisbeat = 10; thisdiff = 16; thisfade = 8; thishue = 128; break;
      case 20: numdots = 8; thisbeat =  3; thisdiff =  0; thisfade = 8; thishue=random8(); break; 
      case 30: break;
    }
  }
  
}
