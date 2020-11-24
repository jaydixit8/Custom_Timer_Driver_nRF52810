#include <nrfx_timer.h>
typedef void(* timer_handler)(void* context);

timer_handler gTIMER0_handler = NULL;

#define CUSTOM_DRIVER
#define FRQ 16000ULL

/** TIMTER0 Start Driver. Call this driver to start TIMER0
 */
void TIMER0_Start(uint32_t time_us, timer_handler timer0_handler)
{
  NRF_TIMER0->MODE = TIMER_MODE_MODE_Timer;                   // Select Timer Mode
  //NRF_TIMER0->TASKS_CLEAR = 1;
  NRF_TIMER0->EVENTS_COMPARE[0] = 0;                          // Clear the Compare Event register of channel 0
  NRF_TIMER0->PRESCALER = 6;                                  // Set Prescaler.
  NRF_TIMER0->BITMODE = TIMER_BITMODE_BITMODE_32Bit;	      // Set counter to 32 bit resolution
  
  NRF_TIMER0->CC[0] = ((time_us * FRQ) >> (NRF_TIMER0->PRESCALER));    // Set value for TIMER0 compare register 0
                                                                       // fTIMER = 16 MHz / (2PRESCALER)

  // Saving the handler
  gTIMER0_handler = timer0_handler;

  // Enable interrupt on Timer 0 for CC[0] compare match events
  NRF_TIMER0->INTENSET = (TIMER_INTENSET_COMPARE0_Enabled << TIMER_INTENSET_COMPARE0_Pos);

  // Setting the TIMER0_IRQ as 6(lower one)
  NVIC_SetPriority(TIMER0_IRQn, 6);

  NVIC_EnableIRQ(TIMER0_IRQn);

  NRF_TIMER0->TASKS_START = 1;               // Start TIMER0
}


/** TIMTER0 Stop Driver. Call this driver to stop TIMER0
 */
void TIMER0_Stop(void)
{		
  // Disable on Timer 0, both for CC[0] compare match events
  NRF_TIMER0->INTENCLR = (TIMER_INTENSET_COMPARE0_Disabled);
  NVIC_DisableIRQ(TIMER2_IRQn);
		
  NRF_TIMER0->TASKS_STOP = 1;               // Stop TIMER0
}
		
/** TIMTER0 peripheral interrupt handler. This interrupt handler is called whenever there it a TIMER0 interrupt
 */
void TIMER0_IRQHandler(void)
{
  if ((NRF_TIMER0->EVENTS_COMPARE[0] != 0) && ((NRF_TIMER0->INTENSET & TIMER_INTENSET_COMPARE0_Msk) != 0))
  {
      NRF_TIMER0->EVENTS_COMPARE[0] = 0;           // Clear compare register 0 event
      NRF_TIMER0->TASKS_CLEAR = 0;                 // reset timer

      gTIMER0_handler(NULL);
  }
}