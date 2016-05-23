#include <generated/csr.h>
#ifdef CSR_GENERATOR_BASE
#include "bist.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include "ci.h"


#define test_size 64*1024*1024

unsigned int ticks;
unsigned long speed;

void bist(void) {
	// write
	printf("writing %d Mbytes...", test_size/(1024*1024));
	generator_reset_write(1);
	generator_base_write(0);
	generator_length_write((test_size*8)/128);

	timer0_en_write(0);
	timer0_load_write(0xffffffff);
	timer0_en_write(1);

	generator_shoot_write(1);
	while(generator_done_read() == 0);

	timer0_update_value_write(1);
	ticks = timer0_value_read();
	ticks = 0xffffffff - ticks;
	speed = SYSTEM_CLOCK_FREQUENCY/ticks;
	speed = test_size*speed/1000000;
	speed = 8*speed;
	printf("/ %u Mbps\n", speed);

	// read
	printf("reading %d Mbytes...", test_size/(1024*1024));
	checker_reset_write(1);
	checker_base_write(0);
	checker_length_write((test_size*8)/128);

	timer0_en_write(0);
	timer0_load_write(0xffffffff);
	timer0_en_write(1);

	checker_shoot_write(1);
	while(checker_done_read() == 0);

	timer0_update_value_write(1);
	ticks = timer0_value_read();
	ticks = 0xffffffff - ticks;
	speed = SYSTEM_CLOCK_FREQUENCY/ticks;
	speed = test_size*speed/1000000;
	speed = 8*speed;
	printf("/ %u Mbps\n", speed);

	// errors
	printf("errors: %d\n", checker_error_count_read());
}

#endif