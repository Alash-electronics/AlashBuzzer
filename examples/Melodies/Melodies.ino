// AlashBuzzer: готовые мелодии. Ода к радости, Звёздочка, Happy Birthday, Jingle Bells и сигналы
// запуска, успеха, ошибки, тревоги. Мелодия играет сама, пока вы вызываете update() в loop().
#include <AlashBuzzer.h>

AlashBuzzer buzzer(9);
uint8_t next = 0;
uint32_t last = 0;

void setup() {
  Serial.begin(115200);
  buzzer.begin();
  buzzer.playMelody(AlashBuzzer::MELODY_STARTUP);
}

void loop() {
  buzzer.update();
  if (!buzzer.isPlaying() && millis() - last > 1500) {     // после паузы следующая мелодия
    last = millis();
    Serial.print(F("Мелодия "));
    Serial.println(next);
    buzzer.playMelody((AlashBuzzer::Melody)next);
    next = (next + 1) % 8;
  }
}
