// [ru] AlashBuzzer: самый простой пример. Один гудок, потом три коротких.
//
//      Подключение пассивной пищалки: плюс (+) к пину 9, минус (-) к GND. Резистор не нужен.
//      Для активной пищалки (со встроенным генератором) создайте так: AlashBuzzer buzzer(9, AlashBuzzer::ACTIVE);
// [kk] AlashBuzzer: ең қарапайым мысал. Бір дыбыс, сосын үш қысқа дыбыс.
//
//      Пассивті пищалканы қосу: плюс (+) 9 пинге, минус (-) GND-ға. Резистор керек емес.
//      Белсенді пищалка (кіріктірілген генераторлы) үшін былай жасаңыз: AlashBuzzer buzzer(9, AlashBuzzer::ACTIVE);
// [en] AlashBuzzer: the simplest example. One beep, then three short ones.
//
//      Wiring a passive buzzer: plus (+) to pin 9, minus (-) to GND. No resistor needed.
//      For an active buzzer (with a built-in oscillator) create it like this: AlashBuzzer buzzer(9, AlashBuzzer::ACTIVE);
#include <AlashBuzzer.h>

AlashBuzzer buzzer(9);

void setup() {
  buzzer.begin();
  buzzer.beep(2000, 300);                 // частота 2000 Гц, 300 мс
}

void loop() {
  buzzer.update();                        // вызывайте в loop(): выключает звук вовремя и ведёт мелодии
  static uint32_t last = 0;
  if (millis() - last > 3000) {           // каждые 3 секунды три коротких гудка (программа при этом не стоит)
    last = millis();
    buzzer.beepTimes(3, 1500, 80, 80);
  }
}
