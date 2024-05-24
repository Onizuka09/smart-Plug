#ifndef STATE_MACHINE_H_
#define STATE_MACHINE_H_

typedef enum {
  init_state = 0,
  AP_sucess,
  submit_success,
  wifi_connect_success,
  wifi_save_success,
  connect_server_success,
  upload_success,
  error,
  wifi_connect_failed,
} state_machine;

#endif
