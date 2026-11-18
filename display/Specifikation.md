# Teknisk specifikation · Arduino-displaytimer

Den här specifikationen beskriver skissen med tre knappar, tvåsiffrig display och ljudutgång. Alla tider nedan kommer från skissens inställningar.

## Koppling

| Funktion | Arduino | Signal |
| --- | --- | --- |
| Segment A, B, C, D, E, F, G | D7, D8, D4, D3, D2, D6, D5 i denna ordning | LOW tänder segmentet |
| Tiotal | D12 | HIGH väljer siffran |
| Ental | D13 | HIGH väljer siffran |
| Start/paus | D1 | Knapp mellan D1 och GND |
| Plus | D9 | Knapp mellan D9 och GND |
| Minus | D10 | Knapp mellan D10 och GND |
| Ljudutgång | D11 | Fyrkantsvåg för sluttonen |

Knapparna använder `INPUT_PULLUP`. En släppt knapp läses som HIGH och en nedtryckt som LOW. Använd ett strömbegränsande motstånd per segment och följ displayens benmärkning. D11 kan ge signal till en passiv piezo. En lågohmig högtalare på 4–8 Ω behöver ett förstärkarsteg.

## Räkning och gränsvärden

| Läge | Intervall | Beteende vid gränsen |
| --- | --- | --- |
| Vanlig nedräkning | 1 000 ms per steg | Stannar på 00 och spelar sluttonen |
| Snabb nedräkning | 500 ms per steg | Stannar på 00 och spelar sluttonen |
| Snabb uppräkning | 500 ms per steg | Stannar på 99 utan ton |
| Manuell justering | Ett steg per klick eller upprepning | Plus stannar på 99. Minus kan gå från 00 till 99 |

Ett dubbelklick på plus eller minus väljer riktning och startar en ny halvsekundsperiod. Snabb nedräkning från 00 börjar på 99. Snabb uppräkning från 99 lämnar timern stoppad. När en automatisk räkning når sitt slut återgår nästa vanliga start till nedräkning med ett steg per sekund.

Paus bevarar riktning, hastighet och den del av stegtiden som redan gått. Dubbelklick på D1 återställer talet och den sparade tiden, stoppar räkningen och tystar en pågående ton. Att manuellt minska talet till 00 stoppar också räkningen, utan slutton.

## Tidtagning, display och ljud

| Funktion | Inställning | Betydelse |
| --- | --- | --- |
| Kontaktstuds | 30 ms | Knappsignalen måste vara stabil innan en ändring godtas |
| Dubbelklick | 350 ms | Andra nedtryckningen måste registreras inom detta intervall från den första |
| Hålltryck på plus/minus | 500 ms, sedan 120 ms | Första steget efter en halv sekund, därefter ett steg var 120:e ms |
| Displayväxling | 5 ms | Båda siffrorna släcks innan segmentmönstret byts och nästa siffra väljs |
| Slutton | 1 760 Hz i 1 500 ms | Spelas när automatisk nedräkning når 00 |

Skissen mäter förfluten tid med `millis()`. Under räkning läggs tiden i `sparadTid`. När ett helt steg har gått uppdateras talet. Display och knappar fortsätter att uppdateras även under paus och medan `tone()` spelar.

## Testfall

Följande är förväntade resultat enligt skissen. Börja varje fristående test med att dubbelklicka på D1 för att återställa timern.

| Test | Gör så här | Förväntat resultat |
| --- | --- | --- |
| Normal nedräkning | Ställ in 03 och klicka på D1 | Ett steg per sekund till 00, sedan ton i 1,5 s |
| Snabb upp och paus | Dubbelklicka på plus och klicka sedan på D1 | Två steg per sekund tills räkningen pausas. Nästa klick fortsätter uppåt |
| Övre gräns | Håll inne plus tills 99 visas | Talet stannar på 99 |
| Snabb ned | Ställ in 04 och dubbelklicka på minus | 00 nås efter cirka två sekunder och sluttonen spelas |
| Manuell passage av noll | Ställ in 02 och håll inne minus | Displayen visar 01, 00, 99 och 98 i följd utan slutton |
| Återställning | Starta från 99 och dubbelklicka på D1 under räkningen | Displayen står kvar på 00 tills användaren ger ett nytt kommando |

[Tillbaka till projektöversikten](README.md)
