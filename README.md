Arduino – 7-segmentsdisplay (1–9)
Beskrivning
Projektet använder en Arduino för att visa siffrorna 1 till 9 på en 7-segmentsdisplay. Varje siffra visas i en sekund. Efter 9 börjar räkningen om från 1.

Koden nedan utgår från att displayens segment tänds med LOW (common anode), vilket var det som fungerade i ert test.
Kopplingar
Segment	Arduino-pin
A	7
B	8
C	4
D	3
E	2
F	6
G	5


Viktigt: Använd ett lämpligt strömbegränsningsmotstånd i serie med varje LED-segment. Displayens gemensamma ben måste kopplas enligt displayens datablad. Koppla inte displayens gemensamma ben på måfå.
Kod 1 – Testa ett segment (A)
Den här koden ska tända endast segment A. För er fungerande testkoppling tänds segmentet med LOW.
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

void loop() {
  digitalWrite(LED_A, LOW);   // Tänd A
  digitalWrite(LED_B, HIGH);  // Släck B
  digitalWrite(LED_F, HIGH);  // Släck F
  digitalWrite(LED_G, HIGH);  // Släck G
  digitalWrite(LED_C, HIGH);  // Släck C
  digitalWrite(LED_D, HIGH);  // Släck D
  digitalWrite(LED_E, HIGH);  // Släck E
}
Kod 2 – Testa ett segment i taget
Den här koden tänder A, B, C, D, E, F och G ett i taget. Kontrollera att varje fysisk del av displayen motsvarar rätt segment.
#define LED_A 7
#define LED_B 8
#define LED_F 6
#define LED_G 5
#define LED_C 4
#define LED_D 3
#define LED_E 2

int pinnar[] = {
  LED_A, LED_B, LED_C, LED_D,
  LED_E, LED_F, LED_G
};

void setup() {
  for (int i = 0; i < 7; i++) {
    pinMode(pinnar[i], OUTPUT);
    digitalWrite(pinnar[i], HIGH); // Släck alla
  }
}

void loop() {
  for (int i = 0; i < 7; i++) {
    for (int j = 0; j < 7; j++) {
      digitalWrite(pinnar[j], HIGH); // Släck alla segment
    }

    digitalWrite(pinnar[i], LOW); // Tänd ett segment
    delay(1000);
  }
}
Kod 3 – Visa siffrorna 1 till 9
Det här är huvudprogrammet. Displayen visar varje siffra i en sekund och börjar sedan om från 1.
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
  // Segmentordning: A, B, C, D, E, F, G
  // 1 = segmentet ska lysa, 0 = segmentet ska vara släckt.
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
    // Common anode: LOW tänder och HIGH släcker.
    digitalWrite(pinnar[i], siffror[nummer][i] ? LOW : HIGH);
  }
}

void loop() {
  for (int i = 1; i <= 9; i++) {
    visaNummer(i);
    delay(1000);
  }
}
Så fungerar huvudprogrammet
- setup() ställer in alla sju segment som utgångar.
- visaNummer() väljer vilka segment som ska lysa för den valda siffran.
- Segmentordningen i tabellen är A, B, C, D, E, F, G.
- Eftersom displayen tänds med LOW, skickar programmet LOW till segment som ska lysa och HIGH till segment som ska vara släckta.
- loop() visar siffrorna 1–9 med en sekunds paus och börjar sedan om.
Ladda upp koden
1. Öppna Arduino IDE.
2. Välj en av koderna ovan och kopiera hela kodblocket till en ny .ino-fil.
3. Anslut Arduino via USB.
4. Välj rätt kort under Tools → Board.
5. Välj rätt port under Tools → Port.
6. Klicka på Verify och sedan Upload.
Felsökning
- Om fel segment lyser: kontrollera att displayens ben verkligen är kopplade till A–G enligt er displaymodell.
- Om segmenten lyser omvänt: kontrollera displaytypen och om den ska styras med LOW eller HIGH.
- Om slumpmässiga segment lyser: kontrollera gemensam anslutning, kablar, kopplingsdäck och motstånd.
- Om ett segment inte lyser: kontrollera dess kabel, motstånd och displayens ben.
Krav
- Arduino-kompatibelt kort
- 7-segmentsdisplay
- Kopplingsdäck och jumperkablar
- Ett lämpligt strömbegränsningsmotstånd per LED-segment
- USB-kabel
