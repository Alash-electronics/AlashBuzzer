// AlashBuzzer: пищалка (активная и пассивная): гудки, мелодии, RTTTL-рингтоны без delay().
// Без зависимостей. MIT License. Alash Electronics, https://github.com/Alash-electronics/AlashBuzzer
#ifndef ALASH_BUZZER_H
#define ALASH_BUZZER_H

#include <Arduino.h>

class AlashBuzzer {
 public:
  // PASSIVE: пищалка без генератора, звук задаёт частота (мелодии, разные тона). ACTIVE: со встроенным генератором, один тон: включил и пищит.
  enum Type : uint8_t { PASSIVE = 0, ACTIVE = 1 };
  // Готовые мелодии (общественное достояние или наши короткие сигналы)
  enum Melody : uint8_t { MELODY_STARTUP = 0, MELODY_SUCCESS, MELODY_ERROR, MELODY_ALARM, MELODY_ODE_TO_JOY, MELODY_TWINKLE, MELODY_BIRTHDAY, MELODY_JINGLE_BELLS };
  enum Error : uint8_t { ERR_OK = 0, ERR_BAD_RTTTL };
  enum Language : uint8_t { LANG_RU = 0, LANG_KK = 1, LANG_EN = 2 };

  AlashBuzzer(uint8_t pin, Type type = PASSIVE, bool activeLow = false) : _pin(pin), _type(type), _low(activeLow) {}

  void begin();

  // ---- Простые звуки ---------------------------------------------------------------
  void beep(uint16_t frequency = 2000, uint16_t durationMs = 100);       // один гудок (частота нужна только пассивной)
  void beepTimes(uint8_t times, uint16_t frequency = 2000, uint16_t onMs = 100, uint16_t offMs = 100);   // серия гудков
  void on(uint16_t frequency = 2000);       // включить насовсем (до stop())
  void stop();                              // выключить звук и мелодию

  // ---- Мелодии ----------------------------------------------------------------------------
  void play(const uint16_t *frequencies, const uint16_t *durationsMs, uint8_t count);   // нота = частота (0 = пауза) и длительность
  bool playRTTTL(const char *rtttl);                                     // строка формата RTTTL "name:d=4,o=5,b=120:8c6,8e6,4g6"
  bool playRTTTL(const __FlashStringHelper *rtttl);                      // то же из флеш-памяти F("...")
  void playMelody(Melody m);
  void setTempo(float scale) { _tempo = scale > 0.1f ? scale : 0.1f; }   // 1.0 как записано, 2.0 вдвое быстрее
  bool isPlaying() { update(); return _mode != IDLE; }
  void update();                            // вызывайте в loop(): переключает ноты и выключает гудки

  static uint16_t noteFrequency(uint8_t semitone, uint8_t octave);     // семитон 0 = до ... 11 = си

  // ---- Диагностика -----------------------------------------------------------------
  Error error() const { return _err; }
  void setLanguage(Language lang) { _lang = lang; }
  const __FlashStringHelper *errorText();
  void diagnose(Print &out);

 private:
  enum RunMode : uint8_t { IDLE, SINGLE, SERIES, ARRAY, RTTTL } _mode = IDLE;
  uint8_t _pin;
  Type _type;
  bool _low, _started = false, _soundOn = false, _flash = false;
  Error _err = ERR_OK;
  Language _lang = LANG_RU;
  float _tempo = 1.0f;
  uint32_t _until = 0;                      // когда закончится текущий отрезок
  uint32_t _gapAt = 0; bool _gapDone = true;   // RTTTL: когда в конце ноты наступает тишина
  // серия гудков
  uint8_t _left = 0; uint16_t _freq = 2000, _onMs = 100, _offMs = 100; bool _inOn = false;
  // массив нот
  const uint16_t *_fa = nullptr, *_da = nullptr; uint8_t _n = 0, _i = 0;
  // RTTTL
  const char *_p = nullptr, *_notes = nullptr; uint8_t _defD = 4, _defO = 5; uint16_t _bpm = 63;

  void sound(uint16_t freq, uint16_t ms);
  void silence();
  void startTone(uint16_t freq);
  bool beginRtttl(const char *s, bool flash);
  bool nextRtttlNote();
  char rd(const char *p) const;
  void playPgm(const char *s);
};

#endif  // ALASH_BUZZER_H
