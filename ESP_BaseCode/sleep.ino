void gotoSleep(){
  watchdogOn();  //Turn the 8 second watchdog timer on
  //store ADC settings
  byte old_ADCSRA = ADCSRA; 
  // disable ADC
  ADCSRA = 0;
  byte old_PRR = PRR;
  PRR=0xFF;
  set_sleep_mode (SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  noInterrupts();
  // turn off brown-out enable in software
  // BODS must be set to one and BODSE must be set to zero within four clock cycles
  MCUCR = bit (BODS) | bit (BODSE);
  // The BODS bit is automatically cleared after three clock cycles
  MCUCR = bit (BODS);
  // We are guaranteed that the sleep_cpu call will be done
  // as the processor executes the next instruction after
  // interrupts are turned on.
  interrupts();
  sleep_cpu (); // one cycle
  //********************Ultra Lower Power MODE now ON*********************
  //*********CPU will wake when HERE the interrupt pin goes LOW***********
  sleep_disable(); // Disable sleep mode after waking.
  //Restore ADC settings to ON
  ADCSRA = old_ADCSRA;
  PRR = old_PRR;
}

void watchdogOn() {
  // Clear the reset flag, the WDRF bit (bit 3) of MCUSR.
  MCUSR &= B11110111;
  // Set the WDCE bit (bit 4) and the WDE bit (bit 3)
  // of WDTCSR. The WDCE bit must be set in order to
  // change WDE or the watchdog prescalers. Setting the
  // WDCE bit will allow updtaes to the prescalers and
  // WDE for 4 clock cycles then it will be reset by
  // hardware.
  WDTCSR |= B00011000;
  // Set the watchdog timeout prescaler value to 1024 K
  // which will yeild a time-out interval of about 8.0 s.
  WDTCSR = B00100001;
  //B00(1)00(001) == Change Braketed to:
  //0000 0001 0010 0011 0100 0101 0110 0111 1000 1001
  // 16   32   64  125  250  500  1000 2000 4000 8000 (mS)
  // Enable the watchdog timer interupt.
  WDTCSR |= B01000000;
  MCUSR &= B11110111;
}

ISR(WDT_vect)
{
  sleep_count ++; // keep track of how many sleep cycles
  // have been completed.
}


/*  PRR Bit's
    Bit 7 - PRTWI: Power Reduction TWI
    Bit 6 - PRTIM2: Power Reduction Timer/Counter2
    Bit 5 - PRTIM0: Power Reduction Timer/Counter0
    Bit 4 - Res: Reserved bit
    Bit 3 - PRTIM1: Power Reduction Timer/Counter1
    Bit 2 - PRSPI: Power Reduction Serial Peripheral Interface
    Bit 1 - PRUSART0: Power Reduction USART0
    Bit 0 - PRADC: Power Reduction ADC
*/
