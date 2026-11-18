# Arduino · Tvåsiffrig displaytimer

Tre knappar styr ett tal mellan **00 och 99** på en sjusegmentsdisplay. Ställ in tiden med plus och minus och starta nedräkningen. När den automatiska nedräkningen når **00** stannar timern och spelar en slutton i **1,5 sekunder**.

## Från lysdiod till timer

Projektet började med en knapp som tände och släckte en lysdiod. Därefter byggde vi en timer med två siffror, lade till knappar för att justera talet och kopplade in en högtalare. I den senaste versionen går det också att dubbelklicka på plus eller minus för att räkna automatiskt med dubbel hastighet.

## Så använder du knapparna

| Knapp | Ett klick | Dubbelklick | Håll inne |
| --- | --- | --- | --- |
| Start/paus · D1 | Starta, pausa eller fortsätt | Nollställ till 00 och tysta tonen | Ett start- eller pauskommando |
| Plus · D9 | Öka med 1, upp till 99 | Räkna upp automatiskt med två steg per sekund | Öka upprepade gånger till 99 |
| Minus · D10 | Minska med 1. Från 00 går du till 99 | Räkna ned automatiskt med två steg per sekund | Minska upprepade gånger, även förbi 00 till 99 |

Vid uppstart visar displayen **00**. Startar du därifrån börjar nedräkningen på **99**. Ett klick på start/paus under snabbräkning pausar den. Nästa klick fortsätter i samma riktning och hastighet.

## Prova projektet

1. Koppla display, knappar och ljudutgång enligt [kopplingstabellen i specifikationen](Specifikation.md#koppling).
2. Öppna den senaste timerskissen i Arduino IDE, välj kort och port och ladda upp. Om uppladdningen misslyckas på ett kort där D1 används för seriell kommunikation kan du lossa startknappens sladd från D1 under uppladdningen.
3. Ställ in **03** med tre separata tryck på plus. Vänta minst en halv sekund mellan trycken så att de registreras som enkelklick.
4. Tryck en gång på D1. Displayen går från **03** via **02** och **01** till **00**. Då hörs sluttonen och displayen står kvar på **00**.
5. Dubbelklicka på plus. Displayen räknar nu upp ett steg varje halvsekund. Tryck på D1 för att pausa.

[Öppna den tekniska specifikationen och testfallen](Specifikation.md)
