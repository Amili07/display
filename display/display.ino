#define LED_A 7
#define LED_B 8
#define LED_F 6
#define LED_G 5
#define LED_C 4
#define LED_D 3
#define LED_E 2

#define DISP_TENS 12
#define DISP_ONES 13

#define KNAPP 11
#define KNAPP_PLUS 9
#define KNAPP_MINUS 10

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

// Varje knapp håller reda på sitt eget tillstånd.
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

    if (nu - senasteAndring >= debounceTid && lasning != stabilStatus) {
      stabilStatus = lasning;
      return stabilStatus == LOW;
    }

    return false;
  }
};

Knapp startknapp = {KNAPP, HIGH, HIGH, 0};
Knapp plusknapp  = {KNAPP_PLUS, HIGH, HIGH, 0};
Knapp minusknapp = {KNAPP_MINUS, HIGH, HIGH, 0};

bool timerIgang = false;
bool vantarPaAndraKlick = false;
bool visaTiotal = true;

int tal = 0;
unsigned long forstaKlick = 0;
unsigned long senasteLoop = 0;
unsigned long sparadTid = 0;
unsigned long senasteDisplay = 0;

void setup() {
  for (byte i = 0; i < 7; i++) {
    pinMode(pinnar[i], OUTPUT);
    digitalWrite(pinnar[i], HIGH);
  }

  pinMode(DISP_TENS, OUTPUT);
  pinMode(DISP_ONES, OUTPUT);
  digitalWrite(DISP_TENS, LOW);
  digitalWrite(DISP_ONES, LOW);

  pinMode(KNAPP, INPUT_PULLUP);
  pinMode(KNAPP_PLUS, INPUT_PULLUP);
  pinMode(KNAPP_MINUS, INPUT_PULLUP);

  senasteLoop = millis();
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
      tal = (tal + 1) % 100;
    }
  }

  // D9 och D10 ändrar talet ett steg per tryck.
  if (plusknapp.nyttTryck(nu)) {
    tal = (tal + 1) % 100;
  }

  if (minusknapp.nyttTryck(nu)) {
    tal = (tal + 99) % 100; // Ett steg bakåt, även från 00 till 99.
  }

  // Ett ensamt klick på D11 startar eller pausar timern.
  if (vantarPaAndraKlick && nu - forstaKlick > dubbelklickTid) {
    timerIgang = !timerIgang;
    vantarPaAndraKlick = false;
  }

  if (startknapp.nyttTryck(nu)) {
    if (vantarPaAndraKlick) {
      // Andra klicket kom i tid. Nollställ och pausa.
      tal = 0;
      sparadTid = 0;
      timerIgang = false;
      vantarPaAndraKlick = false;
    } else {
      forstaKlick = nu;
      vantarPaAndraKlick = true;
    }
  }

  // Växla mellan tiotal och ental var femte millisekund.
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