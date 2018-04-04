#include <FastLED.h>

#include <LEDMatrix.h>

// Change the next 6 defines to match your matrix type and size

#define LED_PIN        7
#define COLOR_ORDER    GRB
#define CHIPSET        WS2812B

#define MATRIX_WIDTH   24  // Set this negative if physical led 0 is opposite to where you want logical 0
#define MATRIX_HEIGHT  20  // Set this negative if physical led 0 is opposite to where you want logical 0
#define NUM_LEDS MATRIX_WIDTH * MATRIX_HEIGHT
#define MATRIX_TYPE    HORIZONTAL_ZIGZAG_MATRIX  // See top of LEDMatrix.h for matrix wiring types

cLEDMatrix<MATRIX_WIDTH, MATRIX_HEIGHT, MATRIX_TYPE> leds;

uint8_t angle = 0;

void setup()
{
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds[0], leds.Size());
  FastLED.setBrightness(45);
 // set_max_power_in_volts_and_milliamps(5, 12000);       
  //Safety procotion, stating that i am using 5V 12A power supply (Max)
  Serial.begin(9600);
  Serial.println("The rogram has started");
  FastLED.clear(true);
  delay(500);
  FastLED.showColor(CRGB::Lime);
  delay(1000);
}


void loop(){
  fadeall();
  //SinCos();
  //DiagonalMirrors();
  //VerticalMirrors();
  //SinCosMod();
  //Boxes();
  Lines();
  RainbowMirrorShow();
}

void fadeall(){
  for(int i = 0; i <= NUM_LEDS; i++){
    leds(i).nscale8(230);
  }
}


void SinCos(){
//*******************************************************************************************
//A function that will split the matrix into two sections, Even Rows and Odd Rows.
//On the Even Rows the leds will be changing the hue value based off of the sin of the "angle" and will always be on
//On the  Odd rows the leds will be will be changing hues at the same rate of the Even rows, but, their Values will be changing 
//bassed off of the cos of the "angle".  
//This will give the effet that the odd rows of the matrix are fading in and out of the matrix.  Simulates a breathing effect.
//
//NOTE: Try using different color pallets on this animation to make it stand out from some of the other tuff i have been doing.   
//*******************************************************************************************
  uint8_t hue = sin8(angle);
  uint8_t value = cos8(angle);
  
  for (int16_t y=leds.Height()-1; y>=0; --y)
  {
    if( y % 2 == 0) { leds.DrawLine(leds.Width() - 1, y , leds.Width() - leds.Width(), y,  CHSV(hue, 255, 255)); hue +=16;}
    else            { leds.DrawLine(leds.Width() - leds.Width(), y, leds.Width() - 1, y,   CHSV(hue, 255, value));}
  }
  angle += 4;
  FastLED.show();
  delay(20);
}

void DiagonalMirrors(){
//*******************************************************************************************
//Playing around with the many mirror functions that are inclueded in the LEDMatrix Library.
//In this example i am drawing diagonal lines across the matrix and simply turning on different mirror functions.  
//A variable called count is used so that every second the count will increment by one. 
//When the count reaches a certain number, it will turn on that mirror. Finally when the count has exceeded a certain time, it will 
//reset itself back to 0 and the program will repeat.  
//*******************************************************************************************
  uint8_t hue = sin8(angle);
  uint8_t value = cos8(angle);
  static int count = 0;
    for(int x = 0; x <= leds.Width() + leds.Height(); x++)
    {
      leds.DrawLine(x - leds.Height(), leds.Height() - 1, x , 0, CHSV(hue, 255, 255));
      hue+=16;
    }
      if(count >= 0)  {EVERY_N_SECONDS(1) count++;}
      if(count >= 3)  {leds.VerticalMirror();}
      if(count >= 6)  {leds.HorizontalMirror();}
      if(count >= 9)  {leds.QuadrantMirror();}
      if(count >= 12) {leds.QuadrantRotateMirror();}
      if(count >= 15) {leds.TriangleTopMirror();}
      if(count >= 18) {leds.TriangleBottomMirror();}
      if(count >= 21) {leds.QuadrantTopTriangleMirror();}
      if(count >= 24) {leds.QuadrantBottomTriangleMirror();}
      if(count >= 27) {count = 0;}
    
    angle+=4;
    FastLED.delay(50);
   // Serial.println(count); 
}

void VerticalMirrors(){
//*******************************************************************************************
//Simular to the Diagonal mirrors function, this time i am doing the same thing with vertical columns.
//I have removed some of the mirrors that do not look good with this function.  
//*******************************************************************************************
  uint8_t hue = sin8(angle);
  uint8_t value = cos8(angle);
  static int count = 0;
    for(int x = 0; x <= leds.Width() - 1; x++)
    {
      leds.DrawLine(x, leds.Height() - 1, x , 0, CHSV(hue, 255, 255));
      hue+=16;
    }
      if(count >= 0)  {EVERY_N_SECONDS(1) count++;}

      if(count >= 2)  {leds.HorizontalMirror();}
      if(count >= 6) {leds.TriangleBottomMirror();}
      if(count >= 10) {leds.QuadrantTopTriangleMirror();}
      if(count >= 18) {count = 0;}
    
    angle+=4;
    FastLED.delay(50);
   // Serial.println(count);
}

void SinCosMod(){
//*******************************************************************************************
//Program that will mathematically draw a line on the bottom row and far right colun.
//The hue values will be controlled through a sin value so each led will light up a different color. 
//The bottom row will be shifted up and the far right column will be shifted to the left.  
//This will cause the two to crash into eachother along the diagonal center of the matrix. 
//*******************************************************************************************
  uint8_t hue = sin8(angle);
  uint8_t value = cos8(angle);
  static int count = 0;

  if(count >= 0)  {EVERY_N_SECONDS(1) count++;}
  
  if(count >= 1)                                  //Turns on bottom row
  {
    leds.ShiftDown();
    for(int x = leds.Width() - 1; x >= 0; x--)
    {
      leds.DrawLine(x, leds.Height() - 1, x , leds.Height() - 1, CHSV(hue, 255, 255));
      hue+=16;
    }
  }
  
  if(count >= 5)                                //Turns on Right Column
  {
    leds.ShiftLeft();
    for(int y = leds.Height() - 1; y >= 0; y--)
    {
      leds.DrawLine(leds.Width() - 1, y,  leds.Width() - 1 , y, CHSV(hue, 255, 255));
      hue+=16;
    }
  }

/*    //NOTE: This is removed because it will cancel out the 2 if statements above
  
  if(count >= 9)                                  //Turns on top row
  {
    leds.ShiftDown();
    for(int x = leds.Width() - 1; x >= 0; x--)
    {
      leds.DrawLine(x, leds.Height() - leds.Height(), x , leds.Height() - leds.Height(), CHSV(hue, 255, 255));
      hue+=16;
    }
  }
  if(count >= 13)                                //Turns on left Column
  {
    leds.ShiftRight();
    for(int y = leds.Height() - 1; y >= 0; y--)
    {
      leds.DrawLine(leds.Width() - leds.Width(), y,  leds.Width() - leds.Width() , y, CHSV(hue, 255, 255));
      hue+=16;
    }
  }
*/

/*
      if(count >= 0)  {EVERY_N_SECONDS(1) count++;}
      if(count >= 3)  {leds.VerticalMirror();}
      if(count >= 6)  {leds.HorizontalMirror();}
      if(count >= 9)  {leds.QuadrantMirror();}
      if(count >= 12) {leds.QuadrantRotateMirror();}
      if(count >= 15) {leds.TriangleTopMirror();}
      if(count >= 18) {leds.TriangleBottomMirror();}
      if(count >= 21) {leds.QuadrantTopTriangleMirror();}
      if(count >= 24) {leds.QuadrantBottomTriangleMirror();}
      if(count >= 27) {count = 0;}
    */
    angle+=4;
    FastLED.delay(50);
    Serial.println(count);
}











void Boxes(){
//*******************************************************************************************
//So far i have gotten this program to created filled boxes starting from opposite corners then come 
//closer together.  
//Really doesnt look all that good.  This code requires more work.  
//*******************************************************************************************
  uint8_t hue = 0;
  uint8_t hue1 = sin8(angle);
  uint8_t hue2 = cos8(angle);
  uint8_t value1 = sin8(angle);
  uint8_t value2 = cos8(angle);

  for(int xy = 0; xy <= (leds.Width() -1) / 2; xy++)
  {
    leds.DrawFilledRectangle(xy, xy, leds.Width()- 1 - xy, leds.Height()-1 - xy, CHSV(hue, 255, 255)); hue1 +=16;
    FastLED.delay(100);
    FastLED.clear();
  }
  /*
  for(int xy = (leds.Width() -1) / 2; xy >= (leds.Width() - leds.Width(); xy--){
  leds.DrawFilledRectangle(xy, xy, leds.Width()- 1 - xy, leds.Height()-1 - xy, CHSV(hue, 255, 255)); hue +=16;
*/
}

void Lines(){
//*******************************************************************************************
//Working on some line animations in combination with the Mirror function.  
//What would like to accomplish is to have a single line(starting from the left) make its way to the center 
//Using the mirror function will make it apear as though the lines are coming together and meating in the middle.   
//*******************************************************************************************
  uint8_t hue = 192;
  static int count = 0;    //The value of count will be incremented every second 
  int delayTime = 50;     //Quick variable to control the delay time in each of the loops. 
  
  if(count >= 0)      {EVERY_N_SECONDS(1) count++;}
  //Every second we will add one to the current count value
  if(count >= 1 && count <=10)                                                          
  //Begin by Sliding vertical Columns towards and away from the center
  {
    for(int x = 0; x <= (leds.Width() / 2) -1; x++){
      leds.HorizontalMirror();
      leds.DrawLine(x, leds.Height() - leds.Height(), x, leds.Height() - 1, CHSV(hue, 255, 255));
     // leds.DrawLine(x-2, leds.Height() - leds.Height(), x-2, leds.Height() - 1, CRGB::Black);
      fadeall();
      hue+=10;
      FastLED.delay(delayTime);
    }
    for(int x = (leds.Width() / 2) - 1; x >= 0; x--){
      leds.HorizontalMirror();
      leds.DrawLine(x, leds.Height() - leds.Height(), x, leds.Height() - 1, CHSV(hue,255, 255));
      fadeall();
      hue+=10;
      FastLED.delay(delayTime);
    }
  }
  if(count >= 10 && count <= 19)                                                        
  //Once our count has reached ten, we will start sliding the horizontal lines towards and away from the center. 
  {
    for(int y = 0; y <= (leds.Height() / 2) - 1; y++){
      leds.VerticalMirror();
      leds.DrawLine(leds.Width() - leds.Width(), y, leds.Width() - 1, y, CHSV(hue, 255, 255));
      fadeall();
      hue+=12;
      FastLED.delay(delayTime);
    }
    for(int y = (leds.Height() / 2) -1; y >= 0; y--){
      leds.VerticalMirror();
      leds.DrawLine(leds.Width() - leds.Width(), y, leds.Width() - 1, y, CHSV(hue, 255, 255));
      fadeall();
      hue+=12;
      FastLED.delay(delayTime);
    }
  }
  if(count >=19 && count <= 28){
  //Transition to the Rows and columns moving away from the Center 
    for(int x = (leds.Width() / 2) - 1; x >= 0; x--){
      leds.HorizontalMirror();
      leds.DrawLine(x, leds.Height() - leds.Height(), x, leds.Height() - 1, CHSV(hue,255, 255));
      fadeall();
      hue+=10;
      FastLED.delay(delayTime);
    } 
    for(int y = (leds.Height() / 2) -1; y >= 0; y--){
      leds.VerticalMirror();
      leds.DrawLine(leds.Width() - leds.Width(), y, leds.Width() - 1, y, CHSV(hue, 255, 255));
      fadeall();
      hue+=10;
      FastLED.delay(delayTime);
      }
  }
  if(count >= 28 && count <= 37)
  //Transition to the columns sliding away from the center and the Rows moving towards the center.                                       
  {
    for(int x = (leds.Width() / 2) - 1; x >= 0; x--){
      leds.HorizontalMirror();
      leds.DrawLine(x, leds.Height() - leds.Height(), x, leds.Height() - 1, CHSV(hue,255, 255));
      fadeall();
      hue+=14;
      FastLED.delay(delayTime);
    } 
    for(int y = 0; y <= (leds.Height() / 2) - 1; y++){
      leds.VerticalMirror();
      leds.DrawLine(leds.Width() - leds.Width(), y, leds.Width() - 1, y, CHSV(hue, 255, 255));
      fadeall();
      hue+=14;
      FastLED.delay(delayTime);
    }
  }
  if(count >= 37)   {count = 0;}
  //Finish the function by reseting the count value back to 0 so that the function can begin again. 
  Serial.println(count);
  //Used to measure count
}

void RainbowMirrorShow(){
  int hue = sin8(angle);
  static int count = 0;
  for(int x = leds.Width() - 1; x >= 0; x--){
    leds.DrawLine(x , leds.Height() - 1, x, leds.Height() - leds.Height(), CHSV(hue, 255, 255));
    hue+=12;
  }
  if(count >= 0)  {EVERY_N_SECONDS(1) count++;}
  if(count >= 6)  {leds.HorizontalMirror();}
  if(count >= 12)  {leds.QuadrantMirror();}
  if(count >= 18) {leds.TriangleTopMirror();}
  if(count >= 24) {leds.TriangleBottomMirror();}
  if(count >= 30) {count = 0;} 
  angle+=4;
  FastLED.show();
}

