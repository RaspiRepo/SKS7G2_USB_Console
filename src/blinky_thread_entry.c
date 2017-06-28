/***********************************************************************************************************************
* File Name    : blinky_thread_entry.c
* Description  : This is a very simple example application that blinks all the LEDs on a board.
***********************************************************************************************************************/

#include "blinky_thread.h"

extern TX_QUEUE g_cdc_queue;

/*******************************************************************************************************************//**
 * @brief  Blinky example application
 *
 * Blinks all leds at a rate of 1 second using the the threadx sleep function.
 * Only references two other modules including the BSP, IOPORT.
 *
 **********************************************************************************************************************/
void blinky_thread_entry(void)
{
    /* Define the units to be used with the threadx sleep function */
	const uint32_t threadx_tick_rate_Hz = 100;
    /* Set the blink frequency (must be <= threadx_tick_rate_Hz */
    const uint32_t freq_in_hz = 2;
    /* Calculate the delay in terms of the threadx tick rate */
    const uint32_t delay = threadx_tick_rate_Hz/freq_in_hz;
    /* LED type structure */
    bsp_leds_t leds;
    /* LED state variable */
    ioport_level_t level = IOPORT_LEVEL_HIGH;

    /* Get LED information for this board */
    R_BSP_LedsGet(&leds);

    /* If this board has no leds then trap here */
    if (0 == leds.led_count)
    {
        while(1);   // There are no leds on this board
    }

    char send_str[80];  //to store debug strings
    sprintf(send_str, "Welcome to SKS7G2");
    tx_queue_send(&g_cdc_queue, send_str, TX_WAIT_FOREVER);

   uint32_t counter = 0;

    while (1) {
        /* Determine the next state of the LEDs */
        if(IOPORT_LEVEL_LOW == level) {
            level = IOPORT_LEVEL_HIGH;
            sprintf(send_str, "LED LEVEL HIGH %d\r",counter );
        }   else  {
            level = IOPORT_LEVEL_LOW;
            sprintf(send_str, "LED LEVEL LOW %d\r", counter);
        }
        ++counter ;

        /* Update all board LEDs */
        for(uint32_t i = 0; i < leds.led_count; i++)   {
            g_ioport.p_api->pinWrite(leds.p_leds[i], level);
        }

       //TX_WAIT_FOREVER : This will block Blinky Thread until PC receive message.
        tx_queue_send(&g_cdc_queue, send_str, delay);  // TX_WAIT_FOREVER);

        /* Delay */
        tx_thread_sleep (delay);
    }
}
