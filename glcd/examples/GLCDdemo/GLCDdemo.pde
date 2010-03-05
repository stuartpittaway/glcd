/*
 * GLCDexample
 *
 * Basic test code for the Arduino GLCD library.
 * This code exercises a range of graphic functions supported
 * by the library and provides examples of its use.
 * It also gives an indication of performance, showing the
 * number of frames drawn per second.  
 */

#include <glcd.h>

#include "fonts/SystemFont5x7.h"       // system font
#include "fonts/Arial14.h"             // proportional font
#include "bitmaps/ArduinoIcon64x64.h"  // 64x64 pixel bitmap 
#include "bitmaps/ArduinoIcon64x32.h"  // 32 pixel high bitmap  

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
  introScreen();
  GLCD.ClearScreen();    
  GLCD.SelectFont(System5x7, BLACK); // font for the default text area
}

void  loop()
{  
  iter=0; 
  startMillis = millis();
  while(iter++ < 10){   // do 10 iterations
    GLCD.DrawRect(0, 0, GLCD.CenterX, GLCD.Bottom); // rectangle in left side of screen
    GLCD.DrawRoundRect(GLCD.CenterX + 2, 0, GLCD.CenterX - 3, GLCD.Bottom, 5);  // rounded rectangle around text area   
    for(int i=0; i < GLCD.Bottom; i += 4)
      GLCD.DrawLine(1,1, GLCD.CenterX-1, i);  // draw lines from upper left down right side of rectangle  
    GLCD.DrawCircle(GLCD.CenterX/2, GLCD.CenterY-1, GLCD.CenterY-2);   // draw circle centered in the left side of screen  
    GLCD.FillRect( GLCD.CenterX + GLCD.CenterX/2-8 ,GLCD.CenterY + GLCD.CenterY/2 -8,16,16, WHITE); // clear previous spinner position  
    drawSpinner(loops++, GLCD.CenterX + GLCD.CenterX/2, GLCD.CenterY + GLCD.CenterY/2);       // draw new spinner position
    GLCD.GotoXY(GLCD.CenterX/2, GLCD.Bottom -15);          
    GLCD.print(iter);            // print current iteration at the current cursor position 
  } 
  // display iterations per second
  unsigned long duration = millis() - startMillis;
  int fps = 10000 / duration;
  int fps_fract = (10000 % duration) / 10;
  GLCD.ClearScreen();               // clear the screen  
  GLCD.CursorToXY(GLCD.CenterX + 16, 9);
  GLCD.print("GLCD ");
  GLCD.print(GLCD_VERSION, DEC);
  if(GLCD.Height <= 32)
    GLCD.CursorToXY(GLCD.CenterX + 4, 1);
  else  
    GLCD.CursorToXY(GLCD.CenterX + 4, 24);
  GLCD.print("FPS=");               // print a text string
  GLCD.print(fps);                  // print an integer value
  GLCD.print(".");
  GLCD.print(fps_fract);
}


void introScreen(){  
  if(GLCD.Height >= 64)   
    GLCD.DrawBitmap(ArduinoIcon, 32,0); //draw the bitmap at the given x,y position
  else
    GLCD.DrawBitmap(ArduinoIcon64x32, 32,0); // 32 pixel high bitmap for smaller displays
  countdown(3);
  GLCD.ClearScreen();
  GLCD.SelectFont(Arial_14); // you can also make your own fonts, see playground for details   
  GLCD.GotoXY(10, 2);
  GLCD.print("GLCD ver ");
  GLCD.print(GLCD_VERSION, DEC);
  GLCD.DrawRoundRect(8,0,GLCD.Width-9,18, 5);  // rounded rectangle around text area   
  countdown(3);  
  GLCD.ClearScreen(); 
  scribble(5000);  // run for 5 seconds
  moveBall(GLCD.Height/8 ,5000); // kick ball for 5 seconds
  GLCD.SelectFont(System5x7, BLACK);
  GLCD.ClearScreen();  
  showCharacters();
  countdown(3);
  textAreaDemo();
  scrollingDemo();
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
  showArea( textAreaTOPRIGHT,   "Top Rright");
  showArea( textAreaBOTTOMLEFT, "Bottom Left"); 
  showArea( textAreaBOTTOMRIGHT,"Bottom Right ");  
}

void showArea(predefinedArea area, char *description)
{
  GLCD.ClearScreen(); 
  if(GLCD.Height >= 64) 
  {  
    GLCD.DrawBitmap(ArduinoIcon, 0,0); 
    GLCD.DrawBitmap(ArduinoIcon, 64, 0); 
  }
  else
  {
    GLCD.DrawBitmap(ArduinoIcon64x32, 0,0); 
    GLCD.DrawBitmap(ArduinoIcon64x32, 64,0); 
  }
  textArea.DefineArea(area);
  textArea.SelectFont(System5x7);
  textArea.SetFontColor(WHITE); 
  textArea.ClearArea(); 
  textArea.print(description);
  textArea.print("  text area");
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
      // The newline needs to be sent just before the string to make
      // the scrolling look correct. It can be moved below
      // the delay but it won't look as good.
      // if the newline is done while printing the number,
      // you lose a line in the text area because of the scroll
      // and it no longer works with only a 2 line text area.

      textAreaArray[area].print("\nline ");
      textAreaArray[area].print(x);
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


void moveBall(int ballR, unsigned int duration) 
{

int ballX = GLCD.CenterX + 5;   // X position of the ball 
int ballY = GLCD.CenterY;       // Y position of the ball
int ballDirectionY = 1;        // X direction of the ball
int ballDirectionX = 1;        // Y direction of the ball

  GLCD.ClearScreen();
  for(unsigned long start = millis();  millis() - start < duration; )
  {
    // if ball goes off the screen top or bottom, reverse its Y direction
    if (ballY + ballR >= GLCD.Bottom || ballY - ballR <= 0 ) 
      ballDirectionY = -ballDirectionY;

     // if the ball goes off the screen left or right, reverse X direction
    if (ballX - ballR < 0 || ballX + ballR >= GLCD.Right )
      ballDirectionX = -ballDirectionX; 
	  
     // clear the ball's previous position:
    GLCD.DrawCircle(ballX, ballY,ballR, WHITE);
   
     // increment the ball's position in both directions:
    ballX = ballX + 4 * ballDirectionX;
    ballY = ballY + 4 * ballDirectionY;
	  
    GLCD.DrawCircle(ballX, ballY, ballR, BLACK);
    delay(50 );
  }
}



