// [ru] AlashBuzzer: свои мелодии в формате RTTTL (рингтоны для старых телефонов, их тысячи в интернете).
//      Формат: "имя:d=4,o=5,b=120:ноты". d это длительность по умолчанию, o октава, b темп (ударов в минуту).
//      Нота: [длительность][буква][#][.][октава], например 8c6 (восьмая до шестой октавы), 4e. (четвертная с точкой ми), p это пауза.
//      Строка в F("...") хранится во флеш-памяти и не занимает оперативную: на Uno это важно.
// [kk] AlashBuzzer: RTTTL пішіміндегі өз әуендеріңіз (ескі телефондардың рингтондары, интернетте мыңдаған).
//      Пішім: "аты:d=4,o=5,b=120:ноталар". d әдепкі ұзақтық, o октава, b қарқын (минутына соққы).
//      Нота: [ұзақтық][әріп][#][.][октава], мысалы 8c6 (алтыншы октаваның сегізден бір «до»-сы), 4e. (нүктелі төрттен бір «ми»), p үзіліс.
//      F("...") ішіндегі жол флеш жадта сақталады және оперативті жадты алмайды: Uno-да бұл маңызды.
// [en] AlashBuzzer: your own tunes in RTTTL format (ringtones for old phones, thousands of them on the internet).
//      Format: "name:d=4,o=5,b=120:notes". d is the default duration, o the octave, b the tempo (beats per minute).
//      A note: [duration][letter][#][.][octave], for example 8c6 (an eighth C of the sixth octave), 4e. (a dotted quarter E), p is a pause.
//      A string in F("...") is kept in flash memory and does not use RAM: this matters on Uno.
#include <AlashBuzzer.h>

AlashBuzzer buzzer(9);

void setup() {
  Serial.begin(115200);
  buzzer.begin();
  if (!buzzer.playRTTTL(F("scale:d=8,o=5,b=140:c,d,e,f,g,a,b,c6,p,4c6"))) {
    Serial.println(buzzer.errorText());   // объяснит, если мелодия записана неверно
  }
  buzzer.setTempo(1.0);                   // 2.0 вдвое быстрее, 0.5 вдвое медленнее
}

void loop() {
  buzzer.update();
}
