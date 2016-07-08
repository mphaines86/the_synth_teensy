/*Generated using python file "Generate_Tables.py"*/
#ifndef TABLES_DUE_H
#define TABLES_DUE_H
#include <Arduino.h>

struct Voice {
    uint32_t max_length;
	uint32_t loop_point;
	int8_t pitch_from_C5;

	const uint8_t *wave;
    };

  extern const uint8_t EuroBell1_wave[];
  extern const uint8_t EuroBell2_wave[];
  extern const uint8_t DarkHaven_wave[];
  extern const uint8_t Swannee_wave[];
  extern const uint8_t bassgt_wave[];
  extern const uint8_t guitar1_wave[];
  extern const uint8_t bikodrum_wave[];
  extern const uint8_t saw_wave[];

  extern struct Voice EuroBell1;
  extern struct Voice EuroBell2;
  extern struct Voice DarkHaven;
  extern struct Voice Swannee;
  extern struct Voice Bassgt;
  extern struct Voice guitar1;
  extern struct Voice bikodrum;

  extern struct Voice saw;

#endif
