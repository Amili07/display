#define KNAPP_START  1
#define KNAPP_PLUS   9
#define KNAPP_MINUS 10
#define HOGTALARE   11

#define DISP_TENS 12
#define DISP_ONES 13

// Segmenten i ordningen A, B, C, D, E, F, G.
const byte pinnar[7] = {7, 8, 4, 3, 2, 6, 5};

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

const unsigned int slutTonHz = 1760;
const unsigned long slutTonLangd = 1500;
const unsigned long debounceTid = 30;
const unsigned long dubbelklickTid = 350;
const unsigned long hallTid = 500;
const unsigned long upprepaTid = 120;

enum Handelse { INGEN, KLICK, DUBBELKLICK, HALLSTEG };

struct Knapp {
  byte pinne;
  bool senasteLasning = HIGH;
  bool stabil = HIGH;
  bool vantar = false;
  bool halls = false;
  unsigned long andrad = 0;
  unsigned long nedtryckt = 0;
  unsigned long senasteSteg = 0;

  Knapp(byte nummer) : pinne(nummer) {}

  Handelse las(unsigned long nu, bool tillatHall) {
    bool signal = digitalRead(pinne);

    if (signal != senasteLasning) {
      senasteLasning = signal;
      andrad = nu;
    }

    if (signal != stabil && nu - andrad >= debounceTid) {
      stabil = signal;

      if (stabil == LOW) {
        if (vantar && nu - nedtryckt <= dubbelklickTid) {
          vantar = false;
          return DUBBELKLICK;
        }
        nedtryckt = nu;
        vantar = true;
        halls = false;
      } else {
        halls = false;
      }
    }

    // Ett kort klick registreras när tiden för dubbelklick har gått.
    if (vantar && nu - nedtryckt > dubbelklickTid) {
      if (!tillatHall || stabil == HIGH) {
        vantar = false;
        return KLICK;
      }

      if (nu - nedtryckt >= hallTid) {
        vantar = false;
        halls = true;
        senasteSteg = nu;
        return HALLSTEG;
      }
    }

    if (halls && stabil == LOW && nu - senasteSteg >= upprepaTid) {
      senasteSteg = nu;
      return HALLSTEG;
    }

    return INGEN;
  }
};

Knapp startknapp(KNAPP_START);
Knapp plusknapp(KNAPP_PLUS);
Knapp minusknapp(KNAPP_MINUS);

int tal = 0;
int riktning = -1;
bool raknar = false;
bool visaTiotal = true;
unsigned long stegTid = 1000;
unsigned long sparadTid = 0;
unsigned long senasteLoop = 0;
unsigned long senasteDisplay = 0;

void stoppaRakning() {
  raknar = false;
  sparadTid = 0;
  riktning = -1;
  stegTid = 1000;
}

void startaSnabbRakning(int nyRiktning) {
  noTone(HOGTALARE);
  stoppaRakning();

  if (nyRiktning > 0 && tal == 99) return;
  if (nyRiktning < 0 && tal == 0) tal = 99;

  riktning = nyRiktning;
  stegTid = 500; // Ett steg varje halvsekund.
  raknar = true;
}

void hanteraJustering(Handelse handelse, int steg) {
  if (handelse == DUBBELKLICK) {
    startaSnabbRakning(steg);
    return;
  }

  if (handelse != KLICK && handelse != HALLSTEG) return;

  if (steg < 0 && tal == 0) {
    tal = 99;
  } else {
    tal = constrain(tal + steg, 0, 99);
  }

  if (tal == 0 || (tal == 99 && riktning > 0)) {
    stoppaRakning();
  }
}

void visaNummer(int nummer) {
  for (byte i = 0; i < 7; i++) {
    digitalWrite(pinnar[i], siffror[nummer][i] ? LOW : HIGH);
  }
}

void setup() {
  for (byte i = 0; i < 7; i++) {
    pinMode(pinnar[i], OUTPUT);
    digitalWrite(pinnar[i], HIGH);
  }

  pinMode(DISP_TENS, OUTPUT);
  pinMode(DISP_ONES, OUTPUT);
  digitalWrite(DISP_TENS, LOW);
  digitalWrite(DISP_ONES, LOW);

  pinMode(KNAPP_START, INPUT_PULLUP);
  pinMode(KNAPP_PLUS, INPUT_PULLUP);
  pinMode(KNAPP_MINUS, INPUT_PULLUP);
  pinMode(HOGTALARE, OUTPUT);
  digitalWrite(HOGTALARE, LOW);

  senasteLoop = millis();
}

void loop() {
  unsigned long nu = millis();
  unsigned long tidSedanSist = nu - senasteLoop;
  senasteLoop = nu;

  if (raknar) {
    sparadTid += tidSedanSist;

    while (raknar && sparadTid >= stegTid) {
      sparadTid -= stegTid;
      tal += riktning;

      if (tal == 0) {
        stoppaRakning();
        tone(HOGTALARE, slutTonHz, slutTonLangd);
      } else if (tal == 99 && riktning > 0) {
        stoppaRakning();
      }
    }
  }

  hanteraJustering(plusknapp.las(nu, true), 1);
  hanteraJustering(minusknapp.las(nu, true), -1);

  Handelse start = startknapp.las(nu, false);

  if (start == DUBBELKLICK) {
    stoppaRakning();
    noTone(HOGTALARE);
    tal = 0;
  } else if (start == KLICK) {
    noTone(HOGTALARE);
    if (tal == 0) tal = 99;
    raknar = !raknar;
  }

  // Visa tiotal och ental växelvis.
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