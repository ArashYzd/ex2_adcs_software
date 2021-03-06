/*
 * Copyright (C) 2020  University of Alberta
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */
/**
 * @file adcs_io.c
 * @author Andrew Rooney
 * @date 2020-08-09
 */

#include "adcs_io.h"

#include <string.h>

#include "adcs_types.h"
#include "mock_uart_i2c.h"

ADCS_returnState send_uart_telecommand(uint8_t* command, uint32_t length) {
  uint8_t frame[length + 4];
  frame[0] = ADCS_ESC_CHAR;
  frame[1] = ADCS_SOM;
  memcpy(&frame[2], &command, length);
  frame[length + 2] = ADCS_ESC_CHAR;
  frame[length + 2] = ADCS_EOM;
  uart_send(frame, length + 4);
  uint8_t reply[6];
  uart_receive(reply, 6);
  ADCS_returnState TC_err_flag = reply[3];
  return TC_err_flag;
}

ADCS_returnState send_i2c_telecommand(uint8_t* command, uint32_t length) {
  return ADCS_OK;
}

ADCS_returnState request_uart_telemetry(uint8_t TM_ID, uint8_t* telemetry,
                                        uint32_t length) {
  uint8_t frame[5];
  frame[0] = ADCS_ESC_CHAR;
  frame[1] = ADCS_SOM;
  frame[2] = TM_ID;
  frame[3] = ADCS_ESC_CHAR;
  frame[4] = ADCS_EOM;
  uart_send(frame, 5);
  uint8_t reply[length + 5];
  uart_receive(reply, length + 5);
  for (int i = 0; i < length; i++) {
    *(telemetry + i) = reply[3 + i];
  }
  return ADCS_OK;
}

ADCS_returnState request_i2c_telemetry(uint8_t* command, uint32_t length) {
  return ADCS_OK;
}
