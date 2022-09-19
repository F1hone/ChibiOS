/*
    ChibiOS - Copyright (C) 2006..2018 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#include "ch.h"
#include "hal.h"
#include "rt_test_root.h"
#include "oslib_test_root.h"

/*
 * Green LED blinker thread, times are in milliseconds.
 */
static THD_WORKING_AREA(waThread1, 128);
static THD_FUNCTION(Thread1, arg) {

  (void)arg;
  chRegSetThreadName("blinker");
  while (true) {
    palSetLine(LINE_LED_UP_RED);
    chThdSleepMilliseconds(250);
    palClearLine(LINE_LED_UP_RED);
    palSetLine(LINE_LED_RIGHT_GREEN);
    chThdSleepMilliseconds(250);
    palClearLine(LINE_LED_RIGHT_GREEN);
    palSetLine(LINE_LED_DOWN_BLUE);
    chThdSleepMilliseconds(250);
    palClearLine(LINE_LED_DOWN_BLUE);
    palSetLine(LINE_LED_LEFT_ORANGE);
    chThdSleepMilliseconds(250);
    palClearLine(LINE_LED_LEFT_ORANGE);
  }
}

/*
 * Application entry point.
 */
int main(void) {

  /*
   * System initializations.
   * - HAL initialization, this also initializes the configured device drivers
   *   and performs the board-specific initializations.
   * - Kernel initialization, the main() function becomes a thread and the
   *   RTOS is active.
   */
  halInit();
  chSysInit();

  /*
   * Activates the Serial or SIO driver using the default configuration.
   */
//  sdStart(&LPSD3, NULL);
  sioStart(&SIOD3, NULL);
  sioStartOperation(&SIOD3, NULL);

  /*
   * Creates the blinker thread.
   */
  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);

  /*
   * Normal main() thread activity, in this demo it does nothing except
   * sleeping in a loop and check the button state.
   */
  while (true) {
   if (palReadLine(LINE_JOY_SELECT) == PAL_LOW) {
      test_execute((BaseSequentialStream *)&SIOD3, &rt_test_suite);
      test_execute((BaseSequentialStream *)&SIOD3, &oslib_test_suite);
    }
    chThdSleepMilliseconds(500);
  }
}

