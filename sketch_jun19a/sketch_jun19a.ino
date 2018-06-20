byte Marks[13][4];

void setup() {
  Serial.begin(9600);

  Get10(Marks);
  for (int i = 0; i < 13; i++) {
    for (int j = 0; j < 4; j++) {
      Serial.print(Marks[i][j]);
      Serial.print(" ");
    }
    Serial.println();
  }
}

void loop() {
}

void Get10(byte parameter[13][4]) {
  for (int i = 0; i < 13; i++) {
    for (int j = 0; j < 4; j++) {
      parameter[i][j] = 10;
    }
  }
}
