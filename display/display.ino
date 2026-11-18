#define LED_A 7
#define LED_B 8
#define LED_C 4
#define LED_D 3
#define LED_E 2
#define LED_F 6
#define LED_G 5

#define DISP_TENS 12
#define DISP_ONES 13

#define KNAPP_MINUS_1   1
#define KNAPP_MINUS_10  0
#define KNAPP_PLUS_1    9
#define KNAPP_PLUS_10  10
#define KNAPP_START    11

const byte pinnar[7] = {
  LED_A, LED_B, LED_C, LED_D, LED_E, LED_F, LED_G
};

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

const unsigned long debounceTid = 30;
const unsigned long dubbelklickTid = 350;

struct Knapp {
  byte pinne;
  int senasteLasning;
  int stabilStatus;
  unsigned long senasteAndring;

  bool nyttTryck(unsigned long nu) {
    int lasning = digitalRead(pinne);

    if (lasning != senasteLasning) {
      senasteLasning = lasning;
      senasteAndring = nu;
    }

    if (lasning != stabilStatus && nu - senasteAndring >= debounceTid) {
      stabilStatus = lasning;
      return stabilStatus == LOW;
    }

    return false;
  }
};

Knapp minusEtt   = {KNAPP_MINUS_1,  HIGH, HIGH, 0};
Knapp minusTio   = {KNAPP_MINUS_10, HIGH, HIGH, 0};
Knapp plusEtt    = {KNAPP_PLUS_1,   HIGH, HIGH, 0};
Knapp plusTio    = {KNAPP_PLUS_10,  HIGH, HIGH, 0};
Knapp startknapp = {KNAPP_START,    HIGH, HIGH, 0};

int tal = 0;
bool timerIgang = false;
bool visaTiotal = true;
bool vantarPaAndraKlick = false;

unsigned long senasteLoop = 0;
unsigned long sparadTid = 0;
unsigned long senasteDisplay = 0;
unsigned long forstaKlick = 0;

void setup() {
  for (byte i = 0; i < 7; i++) {
    pinMode(pinnar[i], OUTPUT);
    digitalWrite(pinnar[i], HIGH);
  }

  pinMode(DISP_TENS, OUTPUT);
  pinMode(DISP_ONES, OUTPUT);
  digitalWrite(DISP_TENS, LOW);
  digitalWrite(DISP_ONES, LOW);

  pinMode(KNAPP_MINUS_1, INPUT_PULLUP);
  pinMode(KNAPP_MINUS_10, INPUT_PULLUP);
  pinMode(KNAPP_PLUS_1, INPUT_PULLUP);
  pinMode(KNAPP_PLUS_10, INPUT_PULLUP);
  pinMode(KNAPP_START, INPUT_PULLUP);

  senasteLoop = millis();
}

void andraTal(int steg) {
  // Displayen går runt mellan 00 och 99.
  tal = (tal + steg + 100) % 100;
}

void visaNummer(int nummer) {
  for (byte i = 0; i < 7; i++) {
    digitalWrite(pinnar[i], siffror[nummer][i] ? LOW : HIGH);
  }
}

void loop() {
  unsigned long nu = millis();
  unsigned long tidSedanSist = nu - senasteLoop;
  senasteLoop = nu;

  if (timerIgang) {
    sparadTid += tidSedanSist;

    while (sparadTid >= 1000) {
      sparadTid -= 1000;
      andraTal(1);
    }
  }

  // Ett steg per tryck, även när timern är pausad.
  if (minusEtt.nyttTryck(nu)) andraTal(-1);  // D1
  if (minusTio.nyttTryck(nu)) andraTal(-10); // D0
  if (plusEtt.nyttTryck(nu))  andraTal(1);   // D9
  if (plusTio.nyttTryck(nu))  andraTal(10);  // D10

  // Ett enkelklick på D11 startar eller pausar.
  if (vantarPaAndraKlick && nu - forstaKlick > dubbelklickTid) {
    timerIgang = !timerIgang;
    vantarPaAndraKlick = false;
  }

  if (startknapp.nyttTryck(nu)) {
    if (vantarPaAndraKlick) {
      // Dubbelklick nollställer och pausar.
      tal = 0;
      sparadTid = 0;
      timerIgang = false;
      vantarPaAndraKlick = false;
    } else {
      forstaKlick = nu;
      vantarPaAndraKlick = true;
    }
  }

  if (nu - senasteDisplay >= 5) {
    senasteDisplay = nu;

    digitalWrite(DISP_TENS, LOW);
    digitalWrite(DISP_ONES, LOW);

    if (visaTiotal) {
      visaNummer(tal / 10);
      digitalWrite(DISP_TENS, HIGH);
    } else {
      visaNummer(tal % 10);
      digitalWrite(DISP_ONES, HIGH);
    }

    visaTiotal = !visaTiotal;
  }
}