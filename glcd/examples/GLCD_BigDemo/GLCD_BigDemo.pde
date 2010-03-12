/*
 * GLCD_BigDemo
 *
 * A rolling demo showing many capabilities of the GLCD library
 * It combines a number of sketches included in the download.
 * This sketch uses just over 24k of flash so will not run on 
 * an ATmega168
 *
 */

#include <Time.h>  // download from: http://www.arduino.cc/playground/Code/Time
#include <glcd.h>

#include "fonts/SystemFont5x7.h"       // system font
#include "fonts/Arial14.h"             // proportional font
#include "bitmaps/ArduinoIcon64x64.h"  // 64x64 pixel bitmap 
#include "bitmaps/ArduinoIcon64x32.h"  // 32 pixel high bitmap  

Image_t ArduinoIcon;

gText textArea;              // a text area to be defined later in the sketch
gText textAreaArray[3];      // an array of text areas  
gText countdownArea =  gText(GLCD.CenterX, GLCD.CenterY,1,1,Arial_14); // text area for countdown digits

unsigned long startMillis;
unsigned int  loops = 0;
unsigned int  iter = 0;
         int  theDelay = 20; 

void setup()
{

  GLCD.Init();   // initialise the library, non inverted writes pixels onto a clear screen
  if(GLCD.Height >= 64)   
    ArduinoIcon = ArduinoIcon64x64;  // the 64 pixel high icon
  else
    ArduinoIcon = ArduinoIcon64x32;  // the 32 pixel high icon
  introScreen();
  GLCD.ClearScreen();    
  GLCD.SelectFont(System5x7, BLACK); // font for the default text area
  clockBegin(); // start the clock
}

void  loop()
{  
  GLCD.ClearScreen();  
  scribble(5000);  // run for 5 seconds

  GLCD.ClearScreen(); 
  showCharacters();
  countdown(3);

  clock(8); // show the clock given number of seconds
  
 life(20000); // 20 seconds

  GLCD.ClearScreen();   
  textAreaDemo();
 
  rocket(30000);  // 30 seconds of rocket
  
  GLCD.SelectFont(System5x7, BLACK);
  GLCD.ClearScreen(); 
  scrollingDemo();

  GLCD.ClearScreen();   
  for(int i=0; i < 4; i++)  
     FPS(GLCD.Width, GLCD.Height);

  if(GLCD.Width >= 192)
  {         
    GLCD.ClearScreen();   
    for(int i=0; i < 4; i++)
       FPS(GLCD.Width/2, GLCD.Height);         
  }
}


void introScreen(){  
  GLCD.DrawBitmap(ArduinoIcon, 32,0); //draw the bitmap at the given x,y position
  countdown(3);
  GLCD.ClearScreen();
  GLCD.SelectFont(Arial_14); // you can also make your own fonts, see playground for details   
  GLCD.GotoXY(10, 2);
  GLCD.print("GLCD ver ");
  GLCD.print(GLCD_VERSION, DEC);
  GLCD.DrawRoundRect(8,0,GLCD.Width-9,18, 5);  // rounded rectangle around text area   
  countdown(3);  
  GLCD.ClearScreen(); 
}

void showCharacters(){
  // this displays the fixed width system font  
  GLCD.CursorTo(0,0);
  GLCD.print("5x7 font:");
  GLCD.DrawRoundRect(GLCD.CenterX + 2, 0, GLCD.CenterX -3, GLCD.Bottom, 5);  // rounded rectangle around text area 
  textArea.DefineArea( GLCD.CenterX + 5, 3, GLCD.Right-2, GLCD.Bottom-4, 1); 
  textArea.SelectFont(System5x7, BLACK);
  textArea.CursorTo(0,0);
  for(byte c = 32; c <=127; c++){
    textArea.print(c);  
    delay(20);
  }
}

void drawSpinner(byte pos, byte x, byte y) {   
  // this draws an object that appears to spin
  switch(pos % 8) {
    case 0 : GLCD.DrawLine( x, y-8, x, y+8);        break;
    case 1 : GLCD.DrawLine( x+3, y-7, x-3, y+7);    break;
    case 2 : GLCD.DrawLine( x+6, y-6, x-6, y+6);    break;
    case 3 : GLCD.DrawLine( x+7, y-3, x-7, y+3);    break;
    case 4 : GLCD.DrawLine( x+8, y, x-8, y);        break;
    case 5 : GLCD.DrawLine( x+7, y+3, x-7, y-3);    break;
    case 6 : GLCD.DrawLine( x+6, y+6, x-6, y-6);    break; 
    case 7 : GLCD.DrawLine( x+3, y+7, x-3, y-7);    break;
  } 
}

void  textAreaDemo()
{ 
  showArea( textAreaFULL,       "Full"); 
  showArea( textAreaTOP,        "Top");
  showArea( textAreaBOTTOM,     "Bottom"); 
  showArea( textAreaRIGHT,      "Right");
  showArea( textAreaLEFT,       "Left"); 
  showArea( textAreaTOPLEFT,    "Top Left");
  showArea( textAreaTOPRIGHT,   "Top Right");
  showArea( textAreaBOTTOMLEFT, "Bot Left"); 
  showArea( textAreaBOTTOMRIGHT,"Bot Right");  
}

void  textAreaDemox()
{ 
  showArea( textAreaFULL,       "F"); 
  showArea( textAreaTOP,        "T");
  showArea( textAreaBOTTOM,     "B"); 
  showArea( textAreaRIGHT,      "R");
  showArea( textAreaLEFT,       "L"); 
  showArea( textAreaTOPLEFT,    "TL");
  showArea( textAreaTOPRIGHT,   "TR");
  showArea( textAreaBOTTOMLEFT, "BL"); 
  showArea( textAreaBOTTOMRIGHT,"BR");  
}

void showArea(predefinedArea area, char *description)
{
  GLCD.ClearScreen(); 
  GLCD.DrawBitmap(ArduinoIcon, 0,  0); 
  GLCD.DrawBitmap(ArduinoIcon, 64, 0); 
  textArea.DefineArea(area);
  textArea.SelectFont(System5x7);
  textArea.SetFontColor(WHITE); 
  textArea.ClearArea(); 
  textArea.println(description);
  textArea.print("text area");
  delay(1000);
  textArea.SetFontColor(WHITE); 
  textArea.ClearArea();
  showLines(10);
  delay(1000);   
  textArea.ClearArea();
  textArea.SetFontColor(BLACK);   
  showLines(10);
  delay(1000); 
}

void showAscii()
{
  for( char ch = 64; ch < 127; ch++)
  {
    GLCD.print(ch);
    delay(theDelay);
  }   
}

void showLines(int lines)
{
  for(int i = 1; i <= lines; i++)
  { 
    textArea << " Line  " << i << endl;  
    delay(theDelay);  // brief pause between lines
  }
}


void countdown(int count){
  while(count--){  // do countdown  
    countdownArea.ClearArea(); 
    countdownArea.print(count);
    delay(1000);  
  }  
}

void scrollingDemo()
{
  GLCD.ClearScreen();  
  textAreaArray[0].DefineArea( textAreaTOPLEFT);  
  textAreaArray[0].SelectFont(System5x7, WHITE);
  textAreaArray[0].CursorTo(0,0);
  textAreaArray[1].DefineArea( textAreaTOPRIGHT, SCROLL_DOWN);  // reverse scroll
  textAreaArray[1].SelectFont(System5x7, BLACK);
  textAreaArray[1].CursorTo(0,0);
  textAreaArray[2].DefineArea(textAreaBOTTOM); 

  textAreaArray[2].SelectFont(Arial_14, BLACK);
  textAreaArray[2].CursorTo(0,0);

  for(byte area = 0; area < 3; area++)
  {
    for( char c = 64; c < 127; c++)
      textAreaArray[area].print(c);
    delay(theDelay);    
  }
  for(char c = 32; c < 127; c++)
  {
    for(byte area = 0; area < 3; area++)
      textAreaArray[area].print(c);
    delay(theDelay);
  }  

  for(byte area = 0; area< 3; area++)
  {
    textAreaArray[area].ClearArea();
  }
  for(int x = 0; x < 15; x++)
  {
    for(byte area = 0; area < 3; area++)
    {
      textAreaArray[area].print("line ");
      textAreaArray[area].println(x);
      delay(50);
    }
  } 
  delay(1000);
}

/*
 * scribble drawing routine adapted from TellyMate scribble Video sketch
 * http://www.batsocks.co.uk/downloads/tms_scribble_001.pde
 */
void scribble( const unsigned int duration )
{
  const  float tick = 1/128.0;
  float g_head_pos = 0.0;
  
  for(unsigned long start = millis();  millis() - start < duration; )
  {
    g_head_pos += tick ;

    float head = g_head_pos ;
    float tail = head - (256 * tick) ;

    // set the pixels at the 'head' of the line...
    byte x = fn_x( head ) ;
    byte y = fn_y( head ) ;
    GLCD.SetDot( x , y , BLACK) ;

    // clear the pixel at the 'tail' of the line...
    x = fn_x( tail ) ;
    y = fn_y( tail ) ;  
     GLCD.SetDot( x , y , WHITE) ;
  }
}

byte fn_x( float tick )
{
  return (byte)(GLCD.Width/2 + (GLCD.Width/2-1) * sin( tick * 1.8 ) * cos( tick * 3.2 )) ;
}

byte fn_y( float tick )
{
  return (byte)(GLCD.Height/2 + (GLCD.Height/2 -1) * cos( tick * 1.2 ) * sin( tick * 3.1 )) ;
}

void FPS( const byte width, const byte height)
{
  
  const byte CenterX = width/2;
  const byte CenterY = height/2;
  const byte Right = width-1;
  const byte Bottom = height-1;
 
  iter=0; 
  startMillis = millis();
  while(iter++ < 10){   // do 10 iterations
    GLCD.DrawRect(0, 0, CenterX, Bottom); // rectangle in left side of screen
    GLCD.DrawRoundRect(CenterX + 2, 0, CenterX - 3, Bottom, 5);  // rounded rectangle around text area   
    for(int i=0; i < Bottom; i += 4)
      GLCD.DrawLine(1,1, CenterX-1, i);  // draw lines from upper left down right side of rectangle  
    GLCD.DrawCircle(CenterX/2, CenterY-1, CenterY-2);   // draw circle centered in the left side of screen  
    GLCD.FillRect( CenterX + CenterX/2-8 , CenterY + CenterY/2 -8,16,16, WHITE); // clear previous spinner position  
    drawSpinner(loops++, CenterX + CenterX/2, CenterY + CenterY/2);       // draw new spinner position
    GLCD.GotoXY(CenterX/2, Bottom -15);          
    GLCD.print(iter);            // print current iteration at the current cursor position 
  } 
  // display iterations per second
  unsigned long duration = millis() - startMillis;
  int fps = 10000 / duration;
  int fps_fract = (10000 % duration) / 10;
  GLCD.ClearScreen();               // clear the screen  
  GLCD.CursorToXY(CenterX + 16, 9);
  GLCD.print("GLCD ");
  GLCD.print(GLCD_VERSION, DEC);
  if(GLCD.Height <= 32)
    GLCD.CursorToXY(CenterX + 4, 1);
  else  
    GLCD.CursorToXY(CenterX + 4, 24);
  GLCD.print("FPS=");               // print a text string
  GLCD.print(fps);                  // print an integer value
  GLCD.print(".");
  GLCD.print(fps_fract);  
}


