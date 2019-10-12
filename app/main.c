/**
 * @file    main.c
 *
 * This file contains the source code for a sample application to test
 * logging.
 *
 * Copyright (c) 2019, Cooper Software Consulting, LLC.
 * All rights reserved.
 */

#include <stdbool.h>
#include <stdint.h>

#include "nrf_delay.h"
#include "boards.h"

#define NRF_LOG_LEVEL           4

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

#include "version.h"

#define TEST_NAME               "Log Test"

/**@brief Boolean used to indicate the state of the test. */
bool    m_passed = false;

/**@brief Enumeration defining the various test modes
 */
typedef enum
{
    MODE_Start = 0,                     /**< Start of enumeration */

    MODE_FLASH_ERASE = MODE_Start,      /**< Erase the entire flash. */
    MODE_FLASH_ERASE_VERIFY,            /**< Verify the erasure. */
    MODE_FLASH_WRITE,                   /**< Fill the entire flash with a
                                             pattern.  */
    MODE_FLASH_WRITE_VERIFY,            /**< Read the entire flash to verify
                                             the pattern.  */
    MODE_RESULTS,                       /**< Display the test results. */

    MODE_End,                           /**< End of enumeration */
} test_mode_t;


/**@brief Change to the next test mode, stop when we reach the last mode.
 *
 * @param   mode    The current mode
 *
 * @returns mode    The new mode.
 */
static test_mode_t next_mode(test_mode_t mode)
{
    if (++mode >= MODE_End)
    {
        mode = MODE_End;
    }
    return mode;
}

static void flash_erase(void)
{
    NRF_LOG_INFO("Erase Flash");
    nrf_delay_ms(1000);
    m_passed = true;
}

static void flash_verify_erase(void)
{
    NRF_LOG_INFO("Verify Erase");
    nrf_delay_ms(1000);
    m_passed = true;
}

static void flash_write(void)
{
    NRF_LOG_INFO("Write Flash");
    nrf_delay_ms(1000);
    m_passed = true;
}

static void flash_verify_write(void)
{
    NRF_LOG_INFO("Verify Write");
    nrf_delay_ms(1000);
    m_passed = true;
}

static void report_results(void)
{
    NRF_LOG_INFO("Result: %s", m_passed ? "Passed" : "Failed");
    if (m_passed)
    {
        bsp_board_led_on(BSP_BOARD_LED_0);
        bsp_board_led_off(BSP_BOARD_LED_1);
    }
    else
    {
        bsp_board_led_off(BSP_BOARD_LED_0);
        bsp_board_led_on(BSP_BOARD_LED_1);
    }
}

/**@brief Do any processing necessary for the current mode.
 *
 * @param   mode    The current mode
 *
 * @returns mode    The current mode.
 */
static test_mode_t handle_mode(test_mode_t mode)
{
    switch (mode)
    {
    case MODE_FLASH_ERASE:
        flash_erase();
        break;

    case MODE_FLASH_ERASE_VERIFY:
        flash_verify_erase();
        break;

    case MODE_FLASH_WRITE:
        flash_write();
        break;

    case MODE_FLASH_WRITE_VERIFY:
        flash_verify_write();
        break;

    case MODE_RESULTS:
        report_results();
        break;

    default:
        NRF_LOG_ERROR("Unexpected test mode: %d", mode);
        break;
    }

    return(mode);
}

/**@brief Function for initializing the nrf log module.
 */
static void log_init(void)
{
    ret_code_t err_code = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(err_code);

    NRF_LOG_DEFAULT_BACKENDS_INIT();
}

/**@brief Setup the board to the initial state for the test.
 */
static test_mode_t configure_board(void)
{
    // Configure board
    bsp_board_init(BSP_INIT_LEDS);
    NRF_LOG_INFO("LEDs initialized");

    bsp_board_led_off(BSP_BOARD_LED_0);
    bsp_board_led_off(BSP_BOARD_LED_1);
    NRF_LOG_INFO("LEDs off");

    return MODE_FLASH_ERASE;
}

static int process_logs(void)
{
    uint32_t i = 0;

    while(NRF_LOG_PROCESS())
    {
        i++;
    }
    NRF_LOG_INFO("Processed %d entries", i);
    NRF_LOG_PROCESS();

    NRF_LOG_FLUSH();
}

/**
 * @brief Function for application main entry.
 */
int main(void)
{
    // Initialize
    log_init();

    NRF_LOG_RAW_INFO("\n%s v%d.%d.%d\n", TEST_NAME, VERSION_MAJOR, VERSION_MINOR, VERSION_SUB);

    test_mode_t mode = configure_board();

    while (mode != MODE_End)
    {
        NRF_LOG_INFO("Mode %d", mode);

        process_logs();

        mode = next_mode(handle_mode(mode));

        // Toggle LEDs so we see something physical happening
        if (mode % 2)
        {
            bsp_board_led_on(BSP_BOARD_LED_0);
            bsp_board_led_off(BSP_BOARD_LED_1);
        }
        else
        {
            bsp_board_led_on(BSP_BOARD_LED_1);
            bsp_board_led_off(BSP_BOARD_LED_0);
        }
    }

    NRF_LOG_RAW_INFO("Done\n");

    process_logs();

    // Stop forever
    while (true)
    {
    }
}
