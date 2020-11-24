#include <stdbool.h>
#include <stdint.h>
#include "nrf.h"

static uint32_t cnt;
bool gWaitForFlag = false;

/**
 * @brief Handler for timer events.
 */
void T0_Handler(void* p_context)
{
    cnt++;
    gWaitForFlag = true;
}

/**
 * @brief Function for main application entry.
 */
int main(void)
{
    uint32_t time_ms = 509; //Time(in miliseconds) between consecutive compare events.

    TIMER0_Start((time_ms*1000), T0_Handler);

    while (1)
    {
      if(gWaitForFlag)
      {
          gWaitForFlag = false;
          // Enable NRF_LOG_ENABLED for Logs
#if NRF_LOG_ENABLED
          // Print the Counter
          NRFX_LOG_DEBUG("Counter Value: %d.", cnt);
#endif
      }
      // Wait for Interrupt
      __WFI();
    }
}

/** @} */
