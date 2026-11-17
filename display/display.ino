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

const byte pinnar[7] = {
  LED_A, LED_B, LED_C, LED_D,
  LED_E, LED_F, LED_G
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

bool timerIgang = false;
int tal = 0;

unsigned long senasteLoop = 0;
unsigned long sparadTid = 0;
unsigned long senasteDisplay = 0;
bool visaTiotal = true;

// Knappens tillstånd
int senasteLasning = HIGH;
int knappStatus = HIGH;
unsigned long senasteAndring = 0;
const unsigned long debounceTid = 30;

// Dubbelklick
bool vantarPaAndraKlick = false;
unsigned long forstaKlick = 0;
const unsigned long dubbelklickTid = 350;

void setup() {
  for (int i = 0; i < 7; i++) {
    pinMode(pinnar[i], OUTPUT);
    digitalWrite(pinnar[i], HIGH);
  }

  pinMode(DISP_ONES, OUTPUT);
  pinMode(DISP_TENS, OUTPUT);

  digitalWrite(DISP_ONES, LOW);
  digitalWrite(DISP_TENS, LOW);

  pinMode(KNAPP, INPUT_PULLUP);

  senasteLoop = millis();
}

void visaNummer(int nummer) {
  for (int i = 0; i < 7; i++) {
    digitalWrite(
      pinnar[i],
      siffror[nummer][i] ? LOW : HIGH
    );
  }
}

void loop() {
  unsigned long nu = millis();
  unsigned long tidSedanSist = nu - senasteLoop;
  senasteLoop = nu;

  // Räkna endast när timern är igång.
  if (timerIgang) {
    sparadTid += tidSedanSist;

    while (sparadTid >= 1000) {
      sparadTid -= 1000;
      tal = (tal + 1) % 100;
    }
  }

  // Enkelklick: starta eller pausa.
  if (vantarPaAndraKlick &&
      nu - forstaKlick > dubbelklickTid) {
    timerIgang = !timerIgang;
    vantarPaAndraKlick = false;
  }

  // Läs knappen.
  int lasning = digitalRead(KNAPP);

  if (lasning != senasteLasning) {
    senasteAndring = nu;
  }

  // Filtrera kontaktstudsar.
  if (nu - senasteAndring >= debounceTid) {
    if (lasning != knappStatus) {
      knappStatus = lasning;

      // Reagera bara när knappen trycks ned.
      if (knappStatus == LOW) {
        if (vantarPaAndraKlick &&
            nu - forstaKlick <= dubbelklickTid) {

          // Dubbelklick: nollställ och pausa.
          tal = 0;
          sparadTid = 0;
          timerIgang = false;
          vantarPaAndraKlick = false;

        } else {
          forstaKlick = nu;
          vantarPaAndraKlick = true;
        }
      }
    }
  }

  senasteLasning = lasning;

  // Håll displayen tänd även när timern är pausad.
  if (nu - senasteDisplay >= 5) {
    senasteDisplay = nu;

    // Släck båda innan segmenten ändras.
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
