// Stranger Jaunts
#include <FastLED.h>

#define COLOR_ORDER RGB

#define DATA1 12

#define NUM_LEDS 150
#define START_CEILING 100
#define LETTER_SPACING 3
#define OFFSET 10
#define BRIGHTNESS 200

CRGB leds[NUM_LEDS];

void allWhite(CRGB color = CRGB(255, 255, 255)){
  FastLED.clear();
  for (int i = 0; i < NUM_LEDS; i++){
    leds[i] = color;
  }
}

void fillFromPalette(CRGBPalette16 palette, int start=0, int endLED=NUM_LEDS){
  static int offset = 0;
  offset++;
  for (int i = start; i < endLED; i++){
    leds[i%NUM_LEDS] = ColorFromPalette(palette, i+offset + 3, BRIGHTNESS, LINEARBLEND);
  }
}

void allOnThenFade(double timeToFade, CRGBPalette16 palette){ // WARNING takes 1.569 
  //double startT = millis();                                 // Seconds to run 
  FastLED.setBrightness(255);
  fillFromPalette(palette);
  FastLED.show();

  int brightness = BRIGHTNESS;
  //double delayamt = (timeToFade - (millis() - startT)) / 255;
  while (brightness != 0){
    brightness--;
    FastLED.setBrightness(brightness);
    FastLED.show();
    //delayMicroseconds(delayamt * 100);
  }
}

void fadeOutNoEdit(double timeToFade){
  int brightness = BRIGHTNESS;
  float timeToWait = timeToFade / 255;
  while (brightness != 0){
    brightness = brightness + 1;
    FastLED.setBrightness(brightness);
    delay(timeToWait);
  }
}

void fadeIn(double timeToFade, CRGBPalette16 palette){
//  double startT = millis();
  
  FastLED.setBrightness(0);
  fillFromPalette(palette);
  FastLED.show();
  
  int brightness = 0;
//  double delayamt = timeToFade / BRIGHTNESS;
  while (brightness != BRIGHTNESS){
    FastLED.setBrightness(brightness);
    FastLED.show();
    brightness = brightness + 1;
//    delay(delayamt);
  }
}

void glowingLine(int startLED, int len=10, CRGBPalette16 palette=CloudColors_p, boolean reverse=false){
  // send row of lights down the line
  startLED = startLED % NUM_LEDS;
  if (!reverse){
    fillFromPalette(palette, startLED, startLED+10);
  } else {
    fillFromPalette(palette, startLED-len, startLED); 
  }
}

void ceilingGlow( CRGBPalette16 palette=CloudColors_p ){
    fillFromPalette(palette, START_CEILING, NUM_LEDS);
}

void setUp(){
  // Test method to make setting up easier
  while(1){
    FastLED.clear();
    FastLED.show();
    delay(500);
    
    for (int i = 0; i < 26; i++){
      leds[(i*LETTER_SPACING) + OFFSET] = CRGB(255, 0, 0);
    }
    for (int i = START_CEILING; i < NUM_LEDS; i++){
     leds[i] = CRGB(0, 0, 255);
    }
    FastLED.show();
    delay(500);
  }
  
}

CRGB Wheel(byte WheelPos) {
  if(WheelPos < 85) {
    return CRGB(WheelPos * 3, 255 - WheelPos * 3, 0);
  } 
  else if(WheelPos < 170) {
    WheelPos -= 85;
    return CRGB(255 - WheelPos * 3, 0, WheelPos * 3);
  } 
  else {
    WheelPos -= 170;
    return CRGB(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

CRGB randomColor(){
  return Wheel(random(256)); 
}

void glowingStripes(int numStripes, int offset, CRGBPalette16 palette){
  int numPatches = numStripes*2;
  int lenOfPatches = (int) NUM_LEDS / numPatches;

  for (int i=0; i < numPatches; i++){
    if (i % 2 == 0){
      for (int led = 0; led < lenOfPatches; led++){
        int ledIndex = led + (i * lenOfPatches) + offset;
        leds[ledIndex % NUM_LEDS] = ColorFromPalette(palette, i, BRIGHTNESS, LINEARBLEND);
      }
    }
  }
}

CRGB shadeRGBColor(int R, int G, int B, double percent) {
  double t=percent<0?0:255;
  double p=percent<0?percent*-1:percent;

  if (R < 5 && G < 5 && B < 5){
    return CRGB(0,0,0);
  }
  return CRGB((round((t-R)*p)+R), (round((t-G)*p)+G), (round((t-B)*p)+B));
}

void fadeOut(double lenTilOff){
  double timeToWait = lenTilOff / BRIGHTNESS;
  
  int brightness = BRIGHTNESS;
  while (brightness != 0){
    FastLED.setBrightness(brightness);
    FastLED.show();
    brightness = brightness - 1;
    //delay(timeToWait);
  }
}

void setup(){
  Serial.begin(9600);
  FastLED.addLeds<WS2811, DATA1, COLOR_ORDER>(leds, NUM_LEDS);
  pinMode(11, OUTPUT);
}

void loop(){
  //test();
  setupRun();
}

void setupRun(){
  digitalWrite(11, LOW);
  int i = 0;                        // Pulse 4 times to sync music
  while(i < 4) {
    leds[i] = CRGB(255, 255, 0);
    FastLED.show();
    i++;
    delay(500);         
  }
  
  allWhite(CRGB(0,0,0));
  FastLED.setBrightness(255);
  
  run();
}

CRGBPalette16 getRedPalette(){
  CHSV currentPalette[16];
  for (int i = 0; i < 16; i++){
    currentPalette[i] = CHSV(HUE_RED, 17 + (15*i), 255);
  }
  return CRGBPalette16(  currentPalette[14], currentPalette[1], currentPalette[2], currentPalette[12],
                         currentPalette[9], currentPalette[6], currentPalette[10], currentPalette[11],
                         currentPalette[4], currentPalette[5], currentPalette[8], currentPalette[7],
                         currentPalette[13], currentPalette[0], currentPalette[15], currentPalette[3] );
}

CRGBPalette16 getPurpleAndGreenPalette()
{
    CRGB purple = CHSV( HUE_PURPLE, 255, 255);
    CRGB green  = CHSV( HUE_GREEN, 255, 255);
    CRGB black  = CRGB::Black;
    
    return CRGBPalette16(
                                   green,  green,  black,  black,
                                   purple, purple, black,  black,
                                   green,  green,  black,  black,
                                   purple, purple, black,  black );
}

void test(){                        // Not implimented in official version, for testing specific patterns
 double startT = millis();
 allOnThenFade(1, CloudColors_p);
 double runTime = millis() - startT;

 byte * b = (byte *) &runTime;
 Serial.print(runTime);
}

void snakeOn(CRGBPalette16 palette){
  FastLED.clear();
  for (int i = 0; i < NUM_LEDS; i++){
    leds[i] = ColorFromPalette(palette, i, BRIGHTNESS, LINEARBLEND);
    FastLED.show();
  }
}

void glowingStripes_intro(int stripeLen, int offset, CRGBPalette16 palette){
  glowingStripes(stripeLen, offset, palette);
  for (int i; i < START_CEILING; i++){
    leds[i] = CRGB(0, 0, 0);
  }
  int letter = random(26);
  leds[letter*LETTER_SPACING + OFFSET] = ColorFromPalette(palette, letter, 255, LINEARBLEND);
}

void twinkle(int numLights, CRGBPalette16 palette){
  for (int i = 0; i < numLights; i++){
    leds[random(NUM_LEDS)] = ColorFromPalette(palette, i, 255, LINEARBLEND);
  }
}

void shitFlash(int delayTime=100){
  fillFromPalette(PartyColors_p);
  FastLED.show();
  delay(delayTime);
  FastLED.clear();
  FastLED.show();
}
void run(){

  CRGBPalette16 red_p = getRedPalette();
  CRGBPalette16 pg_p  = getPurpleAndGreenPalette();
  double startMethodTime;
  digitalWrite(11, HIGH);
  FastLED.clear();
  FastLED.show();
  
  int start = millis();
  Serial.write("1");
  delay(21658 - (millis() - start));
  snakeOn(LavaColors_p);
  
  delay(22965 - (millis() - start));     // Doesn't start until here
  int i = 0;
  while(i < 12) {   // Rhythm for part 1 84BPM == 714ms

    startMethodTime = millis();
    glowingStripes_intro(15, 0, LavaColors_p);
    FastLED.show();
    delay(715 - (millis() - startMethodTime));

    startMethodTime = millis();
    glowingStripes_intro(15, 1, LavaColors_p);
    delay(715 - (millis() - startMethodTime));
    i++;
  }
  
  delay(40826 - (millis() - start));
  i = 0;
  while(i < 31){  // Rhythm for part 2.a 56BPM == 1072ms
    FastLED.clear();        
    startMethodTime = millis();
    glowingStripes(15, i, CloudColors_p);
    ceilingGlow(CloudColors_p);
    FastLED.show();
    delay(1072 - (millis() - startMethodTime));
    i++;
  }

  delay((60000 + 14806) - (millis() - start));
  i = 0;
  while (i < ((((16 * 4) - 2)) - 30)){
    FastLED.clear();        
    startMethodTime = millis();
    glowingStripes(15, i, red_p);
    ceilingGlow(PartyColors_p);
    FastLED.show();
    delay((984/2) - (millis() - startMethodTime));
    i++;
  }

  i = 0;
  while (i < 14){ // Originally 16
    FastLED.clear();        
    startMethodTime = millis();
    glowingStripes(15, i, CloudColors_p);
    ceilingGlow(RainbowColors_p);
    FastLED.show();
    delay((1000) - (millis() - startMethodTime));
    i++;
  }

  fadeOut(1);
  
  delay((60000 + 45799) - (millis() - start));
  i = 0;
  FastLED.clear();
  FastLED.setBrightness(BRIGHTNESS);
  while(i < 3){  // Rhythm for 2.b 60BPM == 1000ms (could maybe add one more?)
    fadeIn(500, red_p);
    fadeOut(500);
    delay(1000);
    i++;
  }
    fadeIn(500, red_p);
    delay(250);
    fadeOut(500);

    delay(500);
    fadeIn(500, red_p);
    fadeOut(500);
  // Fades here at these times
  FastLED.clear();
  // flash on
  
  //start -= 50;  // account for timer drift
  delay((60000*2 + 4158) - (millis() - start));   // Get better timing here
  FastLED.setBrightness(255);
  shitFlash();
  delay((60000*2 + 4469) - (millis() - start));
  shitFlash();
  delay((60000*2 + 4718) - (millis() - start));
  shitFlash();
  delay((60000*2 + 4967) - (millis() - start));
  shitFlash();
  delay((60000*2 + 5278) - (millis() - start));
  shitFlash();
  delay((60000*2 + 5589) - (millis() - start));
  shitFlash();

  delay((60000*2 + 6212) - (millis() - start));
  i = 0;
  while (i < (8 * 8) + 4 + 8){ // Wild fast song
    startMethodTime = millis();
    FastLED.clear();
    FastLED.setBrightness(255);
    glowingStripes(15, i, red_p);
    FastLED.show();
    delay(250 - (millis() - startMethodTime));
    i++;
  }
  
  // Last place the timer is accurate. Should be negative, so this is subtracted from every timecode to compensate
  double conversion = (60000*2 + 25212 + start) - millis();
  
  FastLED.setBrightness(BRIGHTNESS);
  i = 0;

  delay(((60000*2 + 26251)-conversion) - (millis() - start));
  while(i < 32*3){  // 32 triplets at 51 BPM
    FastLED.clear();
    startMethodTime = millis();
    if (i > 16*3){
      glowingStripes(15, i, OceanColors_p);
    }
    ceilingGlow(OceanColors_p);
    FastLED.show();
    delay(392 - (millis() - startMethodTime));
    i++;
  }

  // Kids part 2.a 98 bpm

  delay(((60000*3 + 4214)-conversion + 50) - (millis() - start));
  i = 0;
  while (i < 16*3 - 1){
    startMethodTime = millis();
    FastLED.clear();
    twinkle(15, CloudColors_p);
    if (i >= 16){
      twinkle(15, OceanColors_p);
    } if (i >= 32) {
      twinkle(15, pg_p);
    }
    FastLED.show();
    delay(610 - (millis() - startMethodTime));
    i++;
  }
  fadeOut(1); 

  //conversion -= 100;
  delay(100);
  FastLED.setBrightness(BRIGHTNESS);
  delay(((60000*3 + 35705) - conversion) - (millis() - start));
  fillFromPalette(pg_p);
  FastLED.show();
  fadeOut(1000);
  
  FastLED.setBrightness(BRIGHTNESS);
  delay(((60000*3 + 37946) - conversion) - (millis() - start));
  fillFromPalette(ForestColors_p);
  FastLED.show();
  fadeOut(1000);

  FastLED.setBrightness(BRIGHTNESS);
  delay(((60000*3 + 40062) - conversion) - (millis() - start));
  fillFromPalette(OceanColors_p);
  FastLED.show();
  fadeOut(1000);

  FastLED.setBrightness(BRIGHTNESS);
  delay(((60000*3 + 42427) - conversion) - (millis() - start));
  fillFromPalette(LavaColors_p);
  FastLED.show();
  fadeOut(1000);

  FastLED.setBrightness(BRIGHTNESS);
  delay(((60000*3 + 44667) - conversion) - (millis() - start));
  fillFromPalette(CloudColors_p);
  FastLED.show();
  fadeOut(1000);

  //conversion -= 50;
  FastLED.setBrightness(BRIGHTNESS);
  delay(((60000*3 + 46783) - conversion) - (millis() - start));
  fillFromPalette(ForestColors_p);
  FastLED.show();
  fadeOut(1000);

  FastLED.setBrightness(BRIGHTNESS);
  delay(((60000*3 + 48899) - conversion) - (millis() - start));
  fillFromPalette(OceanColors_p);
  FastLED.show();
  fadeOut(1000);

  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();
  delay(((60000*3 + 51139) - conversion) - (millis() - start));
  fillFromPalette(CloudColors_p);
  FastLED.show();

  conversion -= 150;
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();
  delay(((60000*3 + 52260) - conversion) - (millis() - start));
  twinkle(30, ForestColors_p);
  FastLED.show();

  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();
  delay(((60000*3 + 53504) - conversion) - (millis() - start));
  twinkle(30, CloudColors_p);
  FastLED.show();

  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();
  delay(((60000*3 + 54749) - conversion) - (millis() - start));
  twinkle(30, ForestColors_p);
  FastLED.show();

  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();
  delay(((60000*3 + 55994) - conversion) - (millis() - start));
  twinkle(30, CloudColors_p);
  FastLED.show();

  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();
  delay(((60000*3 + 57238) - conversion) - (millis() - start));
  twinkle(30, ForestColors_p);
  FastLED.show();

  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();
  delay(((60000*3 + 58608) - conversion) - (millis() - start));
  twinkle(30, CloudColors_p);
  FastLED.show();

  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();
  delay(((60000*4 + 101) - conversion) - (millis() - start));
  startMethodTime = millis();
  twinkle(30, ForestColors_p);
  FastLED.show();
  fadeOut(1);


  //conversion -= 100;
  // Should I Stay or Should I Go
  delay(((60000*4 + 2217) - (60000*4 +101)) - (millis() - startMethodTime));
  startMethodTime = millis();
  FastLED.setBrightness(BRIGHTNESS);
  shitFlash(100);

  delay(((60000*4 + 2715) - (60000*4 + 2217)) - (millis() - startMethodTime));
  startMethodTime = millis();
  shitFlash(100);

  delay(((60000*4 + 3088) - (60000*4 + 2715)) - (millis() - startMethodTime));
  shitFlash(100);
  
  delay(((60000*4 + 3213) - conversion) - (millis() - start));
  snakeOn(PartyColors_p);
  
  delay(((60000*4 + 5827) - conversion) - (millis() - start));
  i = 0;
  while(i < (14 * 4*2) - 12) { // 14 measures, runs every 8th note
                        // 116 BPM == 517ms (259 for 8th note)
    FastLED.clear();
    startMethodTime = millis(); 

    glowingLine(i, 10, PartyColors_p);
    twinkle(30, red_p);
    twinkle(30, RainbowColors_p);    

    FastLED.show();
    delay(259 - (millis() - startMethodTime));
    i++;
  }
}


