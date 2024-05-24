#ifndef DEVICE_H_
#define DEVICE_H_
#include <Arduino.h>
typedef struct {
  int min;
  int hour;
  int day;
} Timer;
typedef struct {
  float power;
  float cureent;
} Dev_data;
typedef enum {
  Off = 0,
  On,
  timer,
  Ping,
  data_transmit,
} function_e;

typedef struct {
  function_e st;
  union {
    Timer timer;
    Dev_data data;
    bool ping_st;
  };
  // void (* function) ( function_e st); // the function assiociated to each
  // function e
} function;

typedef struct {
  char mac[18];
  char uuid[7];
  function func;

} Device;

typedef enum {
  init_st = 0,
  function_mode,
  idle_st,
  error_st,
} program_states;

//

#endif
