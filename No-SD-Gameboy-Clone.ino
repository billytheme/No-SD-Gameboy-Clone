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

// Buzzer
#define buzzer 12

TFT TFTscreen = TFT(CS, DC, RST);
LedControl led_matrix = LedControl(matrix_din, matrix_clk, matrix_cs, 3);

void test();

void setup() {
  TFTscreen.begin();
  Serial.begin(9600);
  led_matrix.shutdown(0, false);
  led_matrix.setIntensity(0, 5);
  led_matrix.shutdown(1, false);
  led_matrix.setIntensity(1, 5);  
  led_matrix.shutdown(2, false);
  led_matrix.setIntensity(2, 5);
  
  pinMode(menu, INPUT_PULLUP);
  pinMode(joystick_button, INPUT_PULLUP);
  pinMode(led_g, OUTPUT);
  pinMode(led_r, OUTPUT);

  TFTscreen.background(0, 0, 0);

  test();
}


void loop() {
  
}

void test() {
  
  for(int x = 0; x < 2; x++){
    digitalWrite(led_r, HIGH);
    delay(100);
    digitalWrite(led_r, LOW);
    delay(100);
  }
    for(int x = 0; x < 2; x++){
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

  Serial.println("on");

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

int count = 0;
while (count < 2000){
  if(count < 1000){
  
  digitalWrite(led_g, digitalRead(menu));
  digitalWrite(led_r, digitalRead(joystick_button));
  count++;
  }
  else{
  int x = analogRead(A0);
  int y = analogRead(A1);
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
  
}
