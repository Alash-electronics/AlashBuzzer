// [ru] AlashBuzzer: готовые мелодии. Ода к радости, Звёздочка, Happy Birthday, Jingle Bells и сигналы
//      запуска, успеха, ошибки, тревоги. Мелодия играет сама, пока вы вызываете update() в loop().
// [kk] AlashBuzzer: дайын әуендер. «Қуаныш одасы», «Жұлдызша», Happy Birthday, Jingle Bells және іске қосу,
//      сәттілік, қате, дабыл сигналдары. Әуен loop() ішінде update() шақырып тұрсаңыз өзі ойнайды.
// [en] AlashBuzzer: ready-made tunes. Ode to Joy, Twinkle, Happy Birthday, Jingle Bells and the signals
//      for start, success, error, alarm. The tune plays by itself while you call update() in loop().
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
