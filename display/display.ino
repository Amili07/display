
#define LED_A 7
#define LED_B 8
#define LED_F 6
#define LED_G 5
#define LED_C 4
#define LED_D 3
#define LED_E 2
#define DISP_TENS 12
#define DISP_ONES 13

void setup() {
  pinMode(LED_A, OUTPUT);
  pinMode(LED_B, OUTPUT);
  pinMode(LED_F, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_C, OUTPUT);
  pinMode(LED_D, OUTPUT);
  pinMode(LED_E, OUTPUT);
  pinMode(DISP_ONES, OUTPUT);
  pinMode(DISP_TENS, OUTPUT);
  digitalWrite(DISP_ONES, LOW);
  digitalWrite(DISP_TENS, LOW);
}

void visaEttor(int num){
  digitalWrite(DISP_ONES, HIGH);
  digitalWrite(DISP_TENS, LOW);
  visaNummer(num);
  
}

void visaTior(int nums){
digitalWrite(DISP_TENS, HIGH);
  digitalWrite(DISP_ONES, LOW);
  visaNummer(nums);
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
#define PAUS 10

void loop() {
  for (int tal = 0; tal <= 99; tal++) {

    int tiotal = tal / 10;
    int ental = tal % 10;

    unsigned long start = millis();

    while (millis() - start < 1000) {
      visaTior(tiotal);
      delay(PAUS);

      visaEttor(ental);
      delay(PAUS);
    }
  }
}

