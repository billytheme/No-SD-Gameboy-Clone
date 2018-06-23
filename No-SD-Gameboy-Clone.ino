#include <LedControl.h>
#include <TFT.h>
#include <SPI.h>

// LED Matrix
#define matrix_din 4
#define matrix_clk 7
#define matrix_cs 8

// LCD display
#define CS   10
#define DC   9
#define RST  6
//#define CLK 13
//#define DIN  11

// Menu
#define menu 2

// Joystick
#define joystick_x A0
#define joystick_y A1
#define joystick_button 1

// LED
#define led_r 3
#define led_g 5


//initalize output classes for LED and TFT displays
TFT TFTscreen = TFT(CS, DC, RST);
LedControl led_matrix = LedControl(matrix_din, matrix_clk, matrix_cs, 3);

void setup() {
  TFTscreen.begin();
  Serial.begin(9600);
  led_matrix.shutdown(0, false);
  led_matrix.setIntensity(0, 5);
  led_matrix.shutdown(1, false);
  led_matrix.setIntensity(1, 5);  
  led_matrix.shutdown(2, false);
  led_matrix.setIntensity(2, 5);

  //initialize output pins and button input pins
  pinMode(menu, INPUT_PULLUP);
  pinMode(joystick_button, INPUT_PULLUP);
  pinMode(led_g, OUTPUT);
  pinMode(led_r, OUTPUT);

  //clear the TFT screen
  TFTscreen.background(0, 0, 0);
}

//array to store current screen state
byte LEDDisplay[24][8] = {{0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0},
                          {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0},
                          {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0},
                          {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0},
                          {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0},
                          {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0},
                          {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0},
                          {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0},
                          {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0},
                          {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0},
                          {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0},
                          {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}};

//data determining current play piece information
byte piece_x = 4;
byte piece_y = 4;
int piece_rotation = 2;
byte shapeID = 4;

//Storage array for different possible shapes. 
//1st dimension stores all different possible shapes
//2nd dimension stores points for individual shapes
//3rd dimension stores individual points as a relative (x, y) coordinate from an origin point
int pieces[][4][2] = {{{0, 0}, {0, 1}, {0, -1}, {0, -2} },  //line
                      {{0, 0}, {1, 0}, {0, 1},  {1, 1}  },  //square
                      {{0, 0}, {0, 1}, {1, 0},  {1, -1} },  //squiggle to right
                      {{0, 0}, {0, 1}, {-1, 0}, {-1, -1}},  //sqiggle to left
                      {{0, 0}, {0, 1}, {0, -1}, {1, -1} },  //right hook
                      {{0, 0}, {0, 1}, {0, -1}, {-1, -1}},  //left hook
                      {{0, 0}, {0, 1}, {1, 0},  {-1, 0} }}; //nearly-cross

byte rotateHoldCount = 0;
byte fallCounter = 1;
byte leftHoldCount = 0;
byte rightHoldCount = 0;

void loop() {
  eraseShape();
  
  int x = analogRead(joystick_x);
  int y = analogRead(joystick_y);

  //rotation
  if(y > 700){
    if(rotateHoldCount == 0 || (rotateHoldCount > 4 && rotateHoldCount % 2 == 0)){
      piece_rotation = (piece_rotation + 1) % 4;
    }
    if(rotateHoldCount == 254){
      rotateHoldCount = 5;
    }
    else{
      rotateHoldCount++;
    }
  }
  else{
    rotateHoldCount = 0;
  }

  //falling
  if(y < 300){
    piece_y++;
    fallCounter = 0;
  }
  else{
    fallCounter = (fallCounter + 1) % 20;
    if(fallCounter == 0){
      piece_y++;
    }
  }

  //move left
  if(x > 700){
    if(leftHoldCount == 0 || (leftHoldCount > 3 && leftHoldCount % 2 == 0)){
      piece_x++;
    }
    if(leftHoldCount == 254){
      leftHoldCount = 4;
    }
    else{
      leftHoldCount++;
    }
  }
  else{
    leftHoldCount = 0;
  }

  //move right
  if(x > 700){
    if(rightHoldCount == 0 || (rightHoldCount > 3 && rightHoldCount % 2 == 0)){
      piece_x--;
    }
    if(rightHoldCount == 254){
      rightHoldCount = 4;
    }
    else{
      rightHoldCount++;
    }
  }
  else{
    rightHoldCount = 0;
  }
  
  drawShape();
  delay(50);
  updateLED();
}

//iterates through all points for the relevant shape and applies some logic to rotate them, then displays to a screen
//shapeID is the shape to displat
//piece_rotation is represented as either 0, 1, 2, or 3, with 0 being unrotated, 1 being rotated 90 to the right, 2 being 180, ect.
void drawShape(){
  for(int z = 0; z < 4; z++){
    int x = pieces[shapeID][z][0];
    int y = pieces[shapeID][z][1];

    for(int a = 0; a < piece_rotation; a++){
      int hold = x * -1;
      x = y;
      y = hold;
    }
    LEDDisplay[piece_x + x][piece_y + y] = 1;
  }
}

//exactly the same as above, except erasing instead of writing. intended to be done before any manipulation and displaying of the board
void eraseShape(){
  for(int z = 0; z < 4; z++){
    int x = pieces[shapeID][z][0];
    int y = pieces[shapeID][z][1];

    for(int a = 0; a < piece_rotation; a++){
      int hold = x * -1;
      x = y;
      y = hold;
    }
    LEDDisplay[piece_x + x][piece_y + y] = 0;
  }
}

//Takes the LEDDisplay matrix, converts each line to binary, then passes it to the display function
void updateLED() {
  for(byte x = 0; x < 24; x++){
    byte total = 0;
    for(byte y = 0; y < 8; y++){
      total = total + (LEDDisplay[x][7-y] * ceil(pow(2, y)));
    }
    led_matrix.setRow(((x - (x % 8)) / 8), x % 8, total);
  } 
}

//takes eliminates full lines from the LEDDisplay array, and collapse other lines, adds empty lines on top
void eliminateLine(){
  byte num = 0;
  byte output[24][8];
  
  bool assign;
  for(byte x = 0; x < 24; x++){
    assign = false;
    for(byte help = 0; help < 8; help++){
      if(LEDDisplay[x][help] == 0){
        assign = true;
        break;
      }
    }
    if(assign){
      for(byte y = 0; y < 8; y++){
        output[num][y] = LEDDisplay[x][y];
      }
      num++;
    }
  }
  
  for(byte x; num < 24; num++){
    for(byte y = 0; y < 8; y++){
      output[num][y] = 0;
    }
  }
  
  for(byte x = 0; x < 24; x++){
    for (byte y = 0; y < 8; y++){
      LEDDisplay[x][y] = output[x][y];
    }
  }
}
/*
void test() {
  
  for(byte x = 0; x < 2; x++){
    digitalWrite(led_r, HIGH);
    delay(100);
    digitalWrite(led_r, LOW);
    delay(100);
  }
    for(byte x = 0; x < 2; x++){
    digitalWrite(led_g, HIGH);
    delay(100);
    digitalWrite(led_g, LOW);
    delay(100);
  }

  delay(100);

  led_matrix.setRow(0, 0, B11111111);
  led_matrix.setRow(0, 1, B11111111);
  led_matrix.setRow(0, 2, B11111111);
  led_matrix.setRow(0, 3, B11111111);
  led_matrix.setRow(0, 4, B11111111);
  led_matrix.setRow(0, 5, B11111111);
  led_matrix.setRow(0, 6, B11111111);
  led_matrix.setRow(0, 7, B11111111);

  led_matrix.setRow(1, 0, B11111111);
  led_matrix.setRow(1, 1, B11111111);
  led_matrix.setRow(1, 2, B11111111);
  led_matrix.setRow(1, 3, B11111111);
  led_matrix.setRow(1, 4, B11111111);
  led_matrix.setRow(1, 5, B11111111);
  led_matrix.setRow(1, 6, B11111111);
  led_matrix.setRow(1, 7, B11111111);

  led_matrix.setRow(2, 0, B11111111);
  led_matrix.setRow(2, 1, B11111111);
  led_matrix.setRow(2, 2, B11111111);
  led_matrix.setRow(2, 3, B11111111);
  led_matrix.setRow(2, 4, B11111111);
  led_matrix.setRow(2, 5, B11111111);
  led_matrix.setRow(2, 6, B11111111);
  led_matrix.setRow(2, 7, B11111111);

  delay(1000);

  led_matrix.setRow(0, 0, B00000000);
  led_matrix.setRow(0, 1, B00000000);
  led_matrix.setRow(0, 2, B00000000);
  led_matrix.setRow(0, 3, B00000000);
  led_matrix.setRow(0, 4, B00000000);
  led_matrix.setRow(0, 5, B00000000);
  led_matrix.setRow(0, 6, B00000000);
  led_matrix.setRow(0, 7, B00000000);

  led_matrix.setRow(1, 0, B00000000);
  led_matrix.setRow(1, 1, B00000000);
  led_matrix.setRow(1, 2, B00000000);
  led_matrix.setRow(1, 3, B00000000);
  led_matrix.setRow(1, 4, B00000000);
  led_matrix.setRow(1, 5, B00000000);
  led_matrix.setRow(1, 6, B00000000);
  led_matrix.setRow(1, 7, B00000000);

  led_matrix.setRow(2, 0, B00000000);
  led_matrix.setRow(2, 1, B00000000);
  led_matrix.setRow(2, 2, B00000000);
  led_matrix.setRow(2, 3, B00000000);
  led_matrix.setRow(2, 4, B00000000);
  led_matrix.setRow(2, 5, B00000000);
  led_matrix.setRow(2, 6, B00000000);
  led_matrix.setRow(2, 7, B00000000);

  TFTscreen.stroke(255, 255, 255);
  // set the font size
  TFTscreen.setTextSize(2);
  // write the text to the top left corner of the screen
  TFTscreen.text("go WU\n", 4, 2);
  TFTscreen.text("kill\n", 4, 22);
  TFTscreen.text("your\n", 4, 42);
  TFTscreen.text("self\n", 4, 62);
  TFTscreen.text("coulter\n", 4, 82);

byte count = 0;
while (count < 2000){
  if(count < 1000){
  
  digitalWrite(led_g, digitalRead(menu));
  digitalWrite(led_r, digitalRead(joystick_button));
  count++;
  }
  else{
  byte x = analogRead(A0);
  byte y = analogRead(A1);
  if(x < 200){
      digitalWrite(led_g, LOW);
  }
  if(x > 800){
      digitalWrite(led_g, HIGH);
  }
  if(y < 200){
      digitalWrite(led_r, LOW);
  }
  if(y > 800){
      digitalWrite(led_r, HIGH);
  }

  count++;
}

  delay(10);
}

  TFTscreen.background(0, 0, 0);
  digitalWrite(led_r, LOW);
  digitalWrite(led_g, LOW);
  
}*/
