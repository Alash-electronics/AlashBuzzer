// Тесты AlashBuzzer без железа: журнал tone / noTone со временем, шаг времени по 1 мс.
//   g++ -std=c++17 -Wall -I mock -I ../../src test_buzzer.cpp ../../src/AlashBuzzer.cpp && ./a.out
#include <stdio.h>
#include "Arduino.h"
#include "AlashBuzzer.h"

static int fails = 0;
#define CHECK(cond) do { if (!(cond)) { printf("FAIL line %d: %s\n", __LINE__, #cond); fails++; } } while (0)
static bool near(float a, float b, float e) { return fabsf(a - b) <= e; }
static void reset() { g_ev.clear(); g_ms = 1000; memset(g_pinLevel, 0, sizeof g_pinLevel); }
static void run(AlashBuzzer &b, uint32_t ms) { for (uint32_t i = 0; i < ms; i++) { g_ms++; b.update(); } }
// Только тоны (без тишины), в порядке следования
static std::vector<Ev> tones() { std::vector<Ev> r; for (auto &e : g_ev) if (e.freq > 0) r.push_back(e); return r; }

int main() {
  // 1) частоты нот: ля 4-й октавы 440, до 5-й 523, до-диез 6-й 1109
  CHECK(AlashBuzzer::noteFrequency(9, 4) == 440 && AlashBuzzer::noteFrequency(0, 5) == 523 && AlashBuzzer::noteFrequency(1, 6) == 1109);
  CHECK(AlashBuzzer::noteFrequency(0, 4) == 262);

  // 2) одиночный гудок: включился на 2000 Гц и выключился через 100 мс
  reset(); AlashBuzzer b(9);
  b.beep(2000, 100); CHECK(b.isPlaying());
  run(b, 99); CHECK(b.isPlaying());
  run(b, 2);  CHECK(!b.isPlaying());
  CHECK(tones().size() == 1 && tones()[0].freq == 2000 && g_ev.back().freq == 0 && g_ev.back().t >= 1100 && g_ev.back().t <= 1102);

  // 3) серия гудков: три по 50 мс через 100 мс
  reset(); AlashBuzzer s(9);
  s.beepTimes(3, 1500, 50, 100);
  run(s, 600);
  CHECK(tones().size() == 3);
  auto tt = tones();
  CHECK(near(tt[1].t - tt[0].t, 150, 2) && near(tt[2].t - tt[1].t, 150, 2));
  CHECK(!s.isPlaying());

  // 4) массив нот: частоты и длительности, 0 это пауза
  reset(); AlashBuzzer a(9);
  const uint16_t f[] = {440, 0, 880}; const uint16_t d[] = {100, 50, 200};
  a.play(f, d, 3);
  run(a, 400);
  tt = tones();
  CHECK(tt.size() == 2 && tt[0].freq == 440 && tt[1].freq == 880 && near(tt[1].t - tt[0].t, 150, 2));
  CHECK(!a.isPlaying());
  reset(); AlashBuzzer a2(9); a2.setTempo(2.0f); a2.play(f, d, 3); run(a2, 400);
  tt = tones(); CHECK(near(tt[1].t - tt[0].t, 75, 2));                   // вдвое быстрее

  // 5) RTTTL: темп 120, 8c6 это 250 мс (до 6-й октавы 1047), 4e 500 мс (ми 5-й 659), 2g#. 1500 мс (соль-диез 5-й 831)
  reset(); AlashBuzzer r(9);
  CHECK(r.playRTTTL("test:d=4,o=5,b=120:8c6,p,4e,2g#."));
  run(r, 4000);
  tt = tones();
  CHECK(tt.size() == 3 && tt[0].freq == 1047 && tt[1].freq == 659 && tt[2].freq == 831);
  CHECK(near(tt[1].t - tt[0].t, 250 + 500 /*пауза p по умолчанию d=4 это 500*/, 3));
  CHECK(near(tt[2].t - tt[1].t, 500, 3));
  CHECK(!r.isPlaying());
  // конец последней ноты: 1500 мс, звучит 90 %
  CHECK(g_ev.back().freq == 0 && near(g_ev.back().t - tt[2].t, 1500 * 0.9, 5) || near(g_ev.back().t - tt[2].t, 1500, 5));

  // 6) значения по умолчанию RTTTL: d=4, o=5, b=63 и точка после октавы
  reset(); AlashBuzzer q(9);
  CHECK(q.playRTTTL("x::c,e5."));                                         // без шапки: все по умолчанию
  run(q, 3000);
  tt = tones();
  CHECK(tt.size() == 2 && tt[0].freq == 523 && tt[1].freq == 659);
  CHECK(near(tt[1].t - tt[0].t, 4.0f * 60000 / 63 / 4, 4));              // четвертная при 63 уд/мин

  // 7) неверная мелодия: false и ERR_BAD_RTTTL, звука нет
  reset(); AlashBuzzer e(9);
  CHECK(!e.playRTTTL("no header here") && e.error() == AlashBuzzer::ERR_BAD_RTTTL);
  CHECK(!e.playRTTTL("n:d=4,o=5,b=0:c") && e.error() == AlashBuzzer::ERR_BAD_RTTTL);
  CHECK(e.playRTTTL("n:d=4,o=5,b=120:c,@") ); run(e, 2000); CHECK(e.error() == AlashBuzzer::ERR_BAD_RTTTL && !e.isPlaying());
  CHECK(tones().size() >= 1);

  // 8) мелодии из флеш-памяти и встроенные: каждая проигрывается до конца и разбирается без ошибок
  for (int m = 0; m < 8; m++) {
    reset(); AlashBuzzer p(9); p.playMelody((AlashBuzzer::Melody)m);
    run(p, 60000);
    if (p.isPlaying() || p.error() != AlashBuzzer::ERR_OK || tones().empty()) printf("  мелодия %d: playing=%d err=%d tones=%zu\n", m, p.isPlaying(), p.error(), tones().size());
    CHECK(!p.isPlaying() && p.error() == AlashBuzzer::ERR_OK && !tones().empty());
  }
  reset(); AlashBuzzer ode(9); ode.playMelody(AlashBuzzer::MELODY_ODE_TO_JOY); run(ode, 60000);
  CHECK(tones().size() == 30);                                            // в "Оде к радости" здесь 30 нот
  CHECK(tones()[0].freq == 659 && tones()[2].freq == 698);                // ми, ми, фа

  // 9) стоп посреди мелодии
  reset(); AlashBuzzer st(9); st.playMelody(AlashBuzzer::MELODY_TWINKLE); run(st, 1000); st.stop();
  size_t n = tones().size(); run(st, 5000);
  CHECK(!st.isPlaying() && tones().size() == n && g_ev.back().freq == 0);

  // 10) активная пищалка: включается и выключается уровнем, с активным LOW наоборот
  reset(); AlashBuzzer act(5, AlashBuzzer::ACTIVE);
  act.begin(); CHECK(g_pinLevel[5] == LOW);
  act.beep(0, 100); CHECK(g_pinLevel[5] == HIGH); run(act, 120); CHECK(g_pinLevel[5] == LOW && tones().empty());
  reset(); AlashBuzzer al(6, AlashBuzzer::ACTIVE, true);
  al.begin(); CHECK(g_pinLevel[6] == HIGH);
  al.on(); CHECK(g_pinLevel[6] == LOW); al.stop(); CHECK(g_pinLevel[6] == HIGH);

  // 11) диагностика
  reset(); AlashBuzzer dg(9); dg.setLanguage(AlashBuzzer::LANG_KK); const char *kk = (const char *)dg.errorText();
  dg.setLanguage(AlashBuzzer::LANG_EN); dg.diagnose(*(new Print));
  CHECK(kk != nullptr && tones().size() == 3);

  if (fails) { printf("FAILED: %d\n", fails); return 1; }
  printf("ALL TESTS PASSED\n");
  return 0;
}
