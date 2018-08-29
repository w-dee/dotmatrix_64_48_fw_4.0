// original retrieved from https://www.esp32.com/viewtopic.php?t=1743#p8156


#include "../.ssid.h"
#include <soc/io_mux_reg.h>
#include <soc/i2s_reg.h>
#include <soc/i2s_struct.h>
#include "freertos/queue.h"
#include "rom/lldesc.h"
#include <WiFi.h>

#include "driver/i2s.h"
#include "driver/rtc_io.h"
#include "soc/rtc.h"
#include "driver/rtc_io.h"

#include "i2s_struct_dump.h"


#define IO_PWCLK 19
#define IO_COLCLK 18
#define IO_COLSER 5
#define IO_COLLATCH 17
#define IO_ROWLATCH0 16
#define IO_ROWLATCH1 22
#define IO_ROWLATCH2 23
#define IO_HC585SEROUT 32
#define IO_LED1642_RST 35

/**
 * simple lfsr function
 */
static uint32_t led_lfsr(uint32_t lfsr)
{
	lfsr = (lfsr >> 1) ^ (-(lfsr & 1u) & 0xd0000001u);
	return lfsr;
}

static void led_print_0_1(int v)
{
	if(v) Serial.print(F("1")); else Serial.print(F("0"));
}

static void led_post()
{
	// prepare for sending bits
	constexpr int num_bits = 16*8 + 8;
	uint32_t lfsr = 0xabcd0123;

	Serial.println(F("LED matrix driver: Checking serial data path ..."));

	Serial.print(F("sent    :"));
	// shift in test pattern into the shift register.
	for(int i = 0; i < num_bits; ++i)
	{
		// unfortunately (fortunately?) there is no need to
		// insert wait between these bit-banging, since
		// ESP32/ESP8266's GPIO is very slow.
		digitalWrite(IO_COLSER, lfsr & 1);
		digitalWrite(IO_COLCLK, 1);
		delay(1);
		digitalWrite(IO_COLCLK, 0);
		delay(1);
		lfsr = led_lfsr(lfsr);
	}
	Serial.print(F("\r\n"));

	lfsr = 0xabcd0123;
	Serial.print(F("received:"));
	bool error = false;
	for(int i = 0; i < num_bits; ++i)
	{
		// sense the input pin;
		// we may need some delay here because
		// driving the input pin is very weak.
		delayMicroseconds(10);
		int r = digitalRead(IO_HC585SEROUT);
		led_print_0_1(r);
		if(r != (lfsr & 1))
		{
			// error found
			error = true;
		}
		
		digitalWrite(IO_COLCLK, 1);
		digitalWrite(IO_COLCLK, 0);
		lfsr = led_lfsr(lfsr);
	}

	Serial.print(F("\r\n"));

	if(error)
	{
		Serial.print(F("Error found\r\n"));
	}
	else
	{
		Serial.print(F("No error found\r\n"));
	}

}



#define NUM_LED1642  8 // number of LED1642 in serial
/**
 * set LED1642 register using bitbanging
 */
static void led_post_set_led1642_reg(int reg, uint16_t val)
{
	for(int i = 0; i < NUM_LED1642; ++i)
	{
		for(int bit = 15; bit >= 0; --bit)
		{
			// set bit
			digitalWrite(IO_COLSER, !!(val & (1<<bit)));
/*
		Serial.printf("%2d:%2d:%d:%d:%d:%d:%d:%d:%d:%d\r\n",
			i, bit, !!(val & (1<<bit)), digitalRead(IO_HC585SEROUT),
			digitalRead(25),
			digitalRead(26),
			digitalRead(27),
			digitalRead(14),
			digitalRead(12)
			);
*/
			// latch on
			if(i == (NUM_LED1642 - 1) &&
				bit == reg-1)
			{
				// latch (clock count while latch is active; if reg = 7, set CR function)
				digitalWrite(IO_COLLATCH, 1);
			}
			// clock
			digitalWrite(IO_COLCLK, 1);
			digitalWrite(IO_COLCLK, 0);
		}
		// latch off
		digitalWrite(IO_COLLATCH, 0);
	}
}

/**
 * Set LED1642 register per each chip
 */
static void led_post_set_led1642_reg_each(int reg, const uint16_t *val)
{
	for(int i = 0; i < NUM_LED1642; ++i)
	{
	}
}

/**
 * the frame buffer
 */
static uint8_t fb[48][64] = {

#include "matsu.inc"

};


void setup() {

	for(int y = 0; y < 48; ++y)
		for(int x = 0; x < 64; ++x)
			fb[y][x] = 0;

	for(int y = 0; y < 48; ++y)
			fb[y][y+16] = 255;


	Serial.begin(115200);


	pinMode(IO_PWCLK, OUTPUT);
	pinMode(IO_COLCLK, OUTPUT);
	pinMode(IO_COLSER, OUTPUT);
	pinMode(IO_COLLATCH, OUTPUT);
	pinMode(IO_ROWLATCH0, OUTPUT);
	pinMode(IO_ROWLATCH1, OUTPUT);
	pinMode(IO_ROWLATCH2, OUTPUT);
	pinMode(IO_HC585SEROUT, INPUT);
	pinMode(IO_LED1642_RST, INPUT);

	pinMode(33, INPUT);
	pinMode(25, INPUT);

	digitalWrite(IO_PWCLK, LOW);
	digitalWrite(IO_COLCLK, LOW);
	digitalWrite(IO_COLSER, LOW);
	digitalWrite(IO_COLLATCH, LOW);
	digitalWrite(IO_ROWLATCH0, LOW);
	digitalWrite(IO_ROWLATCH1, LOW);
	digitalWrite(IO_ROWLATCH2, LOW);


	delay(1000);

	// Here we should repeat setting configuration register several times
	// at least twice of number of LED1642.
	// We should enable SDO delay because the timing is too tight to
	// transfer data on the serial chain if no SDO delay is applied.
	// Because SDO delay would be set properly only if the previous
	// LED1642 on the chain has delay on the data,
	// so we must set SDO delay one by one from the first LED1642 on the chain
	// to make all LED1642s have proper configuration.
	for(int i = 0; i <NUM_LED1642 * 4; ++i)
	{
		constexpr uint16_t led_config = (1<<13) | // enable SDO delay
			(1<<11) | (1<<12) |(1<<15); // Output turn-on/off time: on:180ns, off:150ns
		led_post_set_led1642_reg(7, led_config); // set control register
	}

	led_post();

	led_post_set_led1642_reg(1, 0xffff); // full LEDs on
/*
	for(int i = 0; i < 16; ++i)
	{
		led_post_set_led1642_reg(i == 15 ? 5 :3, i*4096/16); // test pattern
	}
*/

}

/**
 * Gamma curve function
 */
static constexpr uint32_t gamma_255_to_4095(int in)
{
  return /*byte_reverse*/(
  	/*bit_interleave*/(
  	(uint32_t) (pow((double)(in+20) / (255.0+20), 3.5) * 3800)));  
}

#define G4(N) gamma_255_to_4095((N)), gamma_255_to_4095((N)+1), \
      gamma_255_to_4095((N)+2), gamma_255_to_4095((N)+3), 

#define G16(N) G4(N) G4((N)+4) G4((N)+8) G4((N)+12) 
#define G64(N) G16(N) G16((N)+16) G16((N)+32) G16((N)+48) 

/**
 * Gamma curve table
 */
static constexpr uint32_t gamma_table[256] = {
	G64(0) G64(64) G64(128) G64(192)
	}; // this table must be accessible from interrupt routine;
	// do not place in FLASH !!




#define B_COLSER (1<<0)
#define B_COLLATCH (1<<1)
#define B_ROWLATCH0 (1<<2)
#define B_ROWLATCH1 (1<<3)
#define B_ROWLATCH2 (1<<4)



/*
	To simplify things,
	the LED1642's PWM clock receives exactly the same signal as serial data clock.
	This means the data clock must be synchronized to PWM clock time frame:
	4096 clocks or its integral multiples.
	To reduce PWM clock interfering with WiFi, PWM clock frequency should be as low
	as possible; Here we use the most basic 4096 clocks per one line.

time frame:

16*8 = 128 clocks     :    Pixel brightness data(0) for next line 
16*8 = 128 clocks     :    Pixel brightness data(1) for next line 
16*8 = 128 clocks     :    Pixel brightness data(2) for next line 
              : 
16*8 = 128 clocks     :    Pixel brightness data(14) for next line 


1776 clock            :    dummy

8  clocks             :    All LEDs off
16*8 = 128 clocks     :    All LEDs off
8  clocks             :    Next row data for one HC(T)595
16*8 = 128 clocks     :    HC(T)595 latch + 
                           Pixel brightness data(15) + global latch for next line 
16*8 = 128 clocks     :    All LED on

	The last 256 clocks in a time frame are dead clocks; all leds are off at this period.
*/

static int build_brightness(uint16_t *buf, int row, int n)
{
	// build framebuffer content
	for(int i = 0; i < NUM_LED1642; ++i)
	{
		int x = i * 8 + (n >> 1);
		int y = (row >> 3) * 16 + (row & 7) + ((n&1)?8:0);

		uint16_t br = /*x==y ? 0xaaa: 0;// */gamma_table[fb[y][x]];

		int reg = ( i == NUM_LED1642 - 1 && n == 15) ? 6 : 4; // issue global latch at last transfer

		for(int bit = 15; bit >= 0; --bit)
		{
			uint16_t t;
			// set bit
			if((br & (1<<bit))) t |= B_COLSER;

			// set COLLATCH
			if(i == (NUM_LED1642 - 1) &&
				bit <= reg-1)
			{
				// latch (clock count while latch is active; if reg = 7, set CR function)
				t |= B_COLLATCH;
			}

			// store
			*(buf++) = t;
		}
	}

	return NUM_LED1642 * 16;
} 


static void do_clock()
{

	digitalWrite(IO_PWCLK, 1);
	digitalWrite(IO_COLCLK, 1);


	digitalWrite(IO_PWCLK, 0);
	digitalWrite(IO_COLCLK, 0);

}


static void transfer_brightness(int row, int n)
{
	// transfer framebuffer content
	for(int i = 0; i < NUM_LED1642; ++i)
	{
		int x = i * 8 + (n >> 1);
		int y = (row >> 3) * 16 + (row & 7) + ((n&1)?8:0);

		uint16_t br = /*x==y ? 0xaaa: 0;// */gamma_table[fb[y][x]];

		int reg = ( i == NUM_LED1642 - 1 && n == 15) ? 6 : 4; // issue global latch at last transfer

		for(int bit = 15; bit >= 0; --bit)
		{
			// set bit
			digitalWrite(IO_COLSER, !!(br & (1<<bit)));

			// latch on
			if(i == (NUM_LED1642 - 1) &&
				bit == reg-1)
			{
				// latch (clock count while latch is active; if reg = 7, set CR function)
				digitalWrite(IO_COLLATCH, 1);
			}
			// clock
			do_clock();
		}
		// latch off
		digitalWrite(IO_COLLATCH, 0);

	}

}

static void led_set_led1642_reg(int reg, uint16_t val)
{
	for(int i = 0; i < NUM_LED1642; ++i)
	{
		for(int bit = 15; bit >= 0; --bit)
		{
			// set bit
			digitalWrite(IO_COLSER, !!(val & (1<<bit)));

			// latch on
			if(i == (NUM_LED1642 - 1) &&
				bit == reg-1)
			{
				// latch (clock count while latch is active; if reg = 7, set CR function)
				digitalWrite(IO_COLLATCH, 1);
			}
			// clock
			do_clock();
		}
		// latch off
		digitalWrite(IO_COLLATCH, 0);
	}
}

static void 

void loop()
{
	static int r = 0;

	++r;
	if(r >= 24) r = 0;

	for(int n = 0; n <= 14; ++n)
		transfer_brightness(r, n);


	for(int n = 0; n < 1776; ++n)
	{
		// dummy clock
		do_clock();
	}



	for(int i = 0; i < 8; ++ i)
	{
		digitalWrite(IO_COLSER, 1);
		do_clock();
	}

	led_set_led1642_reg(2, 0x0000); // full LEDs off

	digitalWrite(IO_ROWLATCH0, HIGH); // let HCT595 latch the buffer with '1' (clear all LED)
	digitalWrite(IO_ROWLATCH1, HIGH); // let HCT595 latch the buffer with '1' (clear all LED)
	digitalWrite(IO_ROWLATCH2, HIGH); // let HCT595 latch the buffer with '1' (clear all LED)
	digitalWrite(IO_ROWLATCH0, LOW);
	digitalWrite(IO_ROWLATCH1, LOW);
	digitalWrite(IO_ROWLATCH2, LOW);

	for(int i = 0; i < 8; ++ i)
	{
		digitalWrite(IO_COLSER, i != r%8);
		do_clock();
	}

	transfer_brightness(r, 15); // global latch of brightness data

	switch(r>>3)
	{
	case 2:
		digitalWrite(IO_ROWLATCH0, HIGH); // let HCT595 latch the buffer
		digitalWrite(IO_ROWLATCH0, LOW);
		break;

	case 0:
		digitalWrite(IO_ROWLATCH1, HIGH); // let HCT595 latch the buffer
		digitalWrite(IO_ROWLATCH1, LOW);
		break;

	case 1:
		digitalWrite(IO_ROWLATCH2, HIGH); // let HCT595 latch the buffer
		digitalWrite(IO_ROWLATCH2, LOW);
		break;

	}

/*
	for(int i = 0; i < 24; ++ i)
	{
		do_clock();

		delayMicroseconds(10);
		int readout = digitalRead(IO_HC585SEROUT);
		Serial.printf("%d: %d (%d)\r\n", i, readout, i != r);
	}
*/
//	delay(10);

	led_set_led1642_reg(2, 0xffff); // full LEDs on



	{
		static uint32_t next = millis() + 1000;
		static int n = 0;
		if((int32_t)millis() - (int32_t)next >= 0)
		{
			next =millis() + 1000;

			for(int y = 0; y < 48; ++y)
				for(int x = 0; x < 64; ++x)
					fb[y][x] = 0;


			for(int i = 0; i < 48; ++i) fb[i][n] = 255;

			++n;
			if(n >= 64) n = 0;
		}
	}

}


