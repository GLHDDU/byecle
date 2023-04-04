#ifndef __ADC_H
#define __ADC_H	
#include "sys.h"
						  
  void adc_gpio_init(void);
 void adc_init(void);
	uint16_t read_adc_value(uint8_t ch);
#endif 


