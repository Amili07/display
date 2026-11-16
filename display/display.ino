
#define LED_A 7
#define LED_B 8
#define LED_F 6
#define LED_G 5
#define LED_C 4
#define LED_D 3
#define LED_E 2

void setup() {
  pinMode(LED_A, OUTPUT);
  pinMode(LED_B, OUTPUT);
  pinMode(LED_F, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_C, OUTPUT);
  pinMode(LED_D, OUTPUT);
  pinMode(LED_E, OUTPUT);
}

void visaNummer(int nummer) {
  const byte siffror[10][7] = {
    {1,1,1,1,1,1,0}, // 0
    {0,1,1,0,0,0,0}, // 1
    {1,1,0,1,1,0,1}, // 2
    {1,1,1,1,0,0,1}, // 3
    {0,1,1,0,0,1,1}, // 4
    {1,0,1,1,0,1,1}, // 5
    {1,0,1,1,1,1,1}, // 6
    {1,1,1,0,0,0,0}, // 7
    {1,1,1,1,1,1,1}, // 8
    {1,1,1,1,0,1,1}  // 9
  };

  int pinnar[7] = {
    LED_A, LED_B, LED_C, LED_D,
    LED_E, LED_F, LED_G
  };

  for (int i = 0; i < 7; i++) {
    digitalWrite(pinnar[i], siffror[nummer][i] ? LOW : HIGH);
  }
}

void loop() {
  for (int i = 1; i <= 9; i++) {
    visaNummer(i);
    delay(1000);
  }
}

