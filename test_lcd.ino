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


static void lcdIfaceInit() {
 periph_module_enable(PERIPH_I2S0_MODULE);
/*
#define IO_PWCLK 19
#define IO_COLCLK 18
#define IO_COLSER 5
#define IO_COLLATCH 17
#define IO_ROWLATCH0 16
#define IO_ROWLATCH1 22
#define IO_ROWLATCH2 23
#define IO_HC585SEROUT 32
#define IO_LED1642_RST 35
*/

	//Init pins to i2s functions
	pinMatrixOutAttach(18, I2S0O_WS_OUT_IDX, false, false);

	pinMatrixOutAttach(23, I2S0O_DATA_OUT8_IDX, false, false); // bit 0
	pinMatrixOutAttach(19, I2S0O_DATA_OUT9_IDX, false, false); // bit 1
	pinMatrixOutAttach(5,  I2S0O_DATA_OUT10_IDX, false, false); // bit 2
	pinMatrixOutAttach(22, I2S0O_DATA_OUT11_IDX, false, false); // bit 3
	pinMatrixOutAttach(17, I2S0O_DATA_OUT12_IDX, false, false); // bit 4

	// enable apll
	rtc_clk_apll_enable(1, 0, 0, 6, 5); // 1, sdm0, sdm1, sdm2, odir

	//Reset I2S subsystem
	I2S0.conf.rx_reset=1; I2S0.conf.tx_reset=1;
	I2S0.conf.rx_reset=0; I2S0.conf.tx_reset=0;

	I2S0.conf2.val=0;
	I2S0.conf2.lcd_en=1;

	// Both I2S_LCD_TX_SDX2_EN bit and
	// I2S_LCD_TX_WRX2_EN bit are set to 1 in the data frame, form 2
	I2S0.conf2.lcd_tx_wrx2_en = 0;
	I2S0.conf2.lcd_tx_sdx2_en = 0;


	I2S0.sample_rate_conf.rx_bits_mod=16;
	I2S0.sample_rate_conf.tx_bits_mod=16;
	I2S0.sample_rate_conf.rx_bck_div_num=2; // min 2
	I2S0.sample_rate_conf.tx_bck_div_num=2; // min 2

	I2S0.clkm_conf.val=0;
	I2S0.clkm_conf.clka_en=1;
	I2S0.clkm_conf.clk_en=0;
	I2S0.clkm_conf.clkm_div_a=0;
	I2S0.clkm_conf.clkm_div_b=1;
	I2S0.clkm_conf.clkm_div_num=0;

	I2S0.fifo_conf.val=0;
	I2S0.fifo_conf.rx_fifo_mod_force_en=1;
	I2S0.fifo_conf.tx_fifo_mod_force_en=1;
	I2S0.fifo_conf.rx_fifo_mod=1;
	I2S0.fifo_conf.tx_fifo_mod=1;
	I2S0.fifo_conf.rx_data_num=32;
	I2S0.fifo_conf.tx_data_num=32;

	I2S0.conf1.val=0;
	I2S0.conf1.tx_stop_en=1;
	I2S0.conf1.tx_pcm_bypass=1;

	I2S0.conf_chan.val=0;
	I2S0.conf_chan.tx_chan_mod=1;
	I2S0.conf_chan.rx_chan_mod=1;

	//Invert WS to active-low
	I2S0.conf.tx_right_first=0;
	I2S0.conf.rx_right_first=0;
	
	I2S0.timing.val=0;

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

static uint16_t buf[4096];


static volatile lldesc_t dmaDesc[4];
#define MAX_DMA_ITEM_COUNT 1024

static void startDma() {

	digitalWrite(21, 1);


	//Reset I2S FIFO
	I2S0.conf.tx_reset=1;
	I2S0.conf.tx_fifo_reset=1;
	I2S0.conf.rx_fifo_reset=1; // I don't know again, rx fifo must also be reset
	I2S0.conf.tx_reset=0;
	I2S0.conf.tx_fifo_reset=0;
	I2S0.conf.rx_fifo_reset=0; 

	//Reset DMA
	I2S0.lc_conf.in_rst=1; // I don't know why but 'in link' must be reset as also as 'out link'
	I2S0.lc_conf.out_rst=1;
	I2S0.lc_conf.in_rst=0;
	I2S0.lc_conf.out_rst=0;

	//Fill DMA descriptor, each MAX_DMA_ITEM_COUNT entries
	volatile lldesc_t * pdma = dmaDesc;
	uint16_t *b = buf;
	int remain = sizeof(buf) / sizeof(buf[0]);
	while(remain > 0)
	{
		int one_len = MAX_DMA_ITEM_COUNT < remain ? MAX_DMA_ITEM_COUNT: remain;
		pdma->length=one_len*2;
		pdma->size=one_len*2;
		pdma->owner=1;
		pdma->sosf=0;
		pdma->buf=(uint8_t *)b;
		pdma->offset=0; //unused in hw
		pdma->empty= (int32_t)(pdma + 1);
		pdma->eof=0;

		remain -= one_len;
		++pdma;
		b += one_len;
	}

	pdma[-1].empty = (int32_t)(&dmaDesc[0]); // make loop

	//Set desc addr
	I2S0.out_link.addr=((uint32_t)(&(dmaDesc[0])))&I2S_OUTLINK_ADDR;


	//Enable and configure DMA
	I2S0.lc_conf.val= 	typeof(I2S0.lc_conf)  { {
            .in_rst =             0,
            .out_rst =            0,
            .ahbm_fifo_rst =      0,
            .ahbm_rst =           0,
            .out_loop_test =      0,
            .in_loop_test =       0,
            .out_auto_wrback =    1,
            .out_no_restart_clr = 0,
            .out_eof_mode =       1,
            .outdscr_burst_en =   1,
            .indscr_burst_en =    0,
            .out_data_burst_en =  1,
            .check_owner =        0,
            .mem_trans_en =       0,
	} }.val;


	//Clear int flags
	I2S0.int_clr.val=0xFFFFFFFF;

	I2S0.fifo_conf.dscr_en=1;

	//Start transmission
	I2S0.out_link.start=1;

	// make sure that DMA reads required descriptors and push its first data to FIFO
	while(I2S0.out_link_dscr == 0 &&
		I2S0.out_link_dscr_bf0  == 0 &&
		I2S0.out_link_dscr_bf1 == 0) /**/;

	I2S0.conf.tx_start=1;

}


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

128 closk             :    dummy

------------------    2048 clock boundary

1648 clock            :    dummy

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
			uint16_t t = 0;
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

static int build_set_led1642_reg(uint16_t *buf, int reg, uint16_t val)
{
	for(int i = 0; i < NUM_LED1642; ++i)
	{
		for(int bit = 15; bit >= 0; --bit)
		{
			uint16_t t = 0;
			// set bit
			if((val & (1<<bit))) t |= B_COLSER;

			// latch on
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


static int r = 0; // current row

void build_first_half()
{

	uint16_t *bufp = buf;

	for(int n = 0; n <= 14; ++n)
	{
		bufp += build_brightness(bufp, r, n);
	}

	for(int n = 0; n < 128; ++n)
	{
		// dummy clock
		*(bufp++)  = 0;
	}

	// word order shuffle
	for(int i = 0; i < 2048; i += 4)
	{
		uint16_t t0, t1, t2, t3;
		t0 = buf[i  ];
		t1 = buf[i+1];
		t2 = buf[i+2];
		t3 = buf[i+3];
		buf[i  ] = t1;
		buf[i+1] = t0;
		buf[i+2] = t3;
		buf[i+3] = t2;
	}

}

void build_second_half()
{

	uint16_t *bufp = buf + 2048;
	uint16_t *tmpp;

	for(int n = 0; n < 1648; ++n)
	{
		// dummy clock
		*(bufp++)  = 0;
	}



	// all LED off
	for(int i = 0; i < 8; ++ i)
	{
		*(bufp++) = B_COLSER;
	}

	bufp += build_set_led1642_reg(bufp, 2, 0x0000); // full LEDs off


	// row select
	tmpp = bufp;
	for(int i = 0; i < 8; ++ i)
	{
		uint16_t t = 0;
		if(i != r%8) t |= B_COLSER;
		*(bufp++) = t;
	}

	*(tmpp) |= (B_ROWLATCH0 | B_ROWLATCH1 | B_ROWLATCH2); // let HCT595 latch the buffer with '1' (clear all LED)

	bufp += build_brightness(bufp, r, 15); // global latch of brightness data


	tmpp = bufp;
	bufp += build_set_led1642_reg(bufp, 2, 0xffff); // full LEDs on

	switch(r>>3)
	{
	case 2:
		*(tmpp) |= B_ROWLATCH0;
		break;

	case 0:
		*(tmpp) |= B_ROWLATCH1;
		break;

	case 1:
		*(tmpp) |= B_ROWLATCH2;
		break;
	}


/*
	static uint16_t pat[16] = {-1, 1,2,4,8, 16};
	for(int i = 0; i < 4096; ++i) buf[i] = pat[i%16];
*/

	// word order shuffle
	for(int i = 2048; i < 4096; i += 4)
	{
		uint16_t t0, t1, t2, t3;
		t0 = buf[i  ];
		t1 = buf[i+1];
		t2 = buf[i+2];
		t3 = buf[i+3];
		buf[i  ] = t1;
		buf[i+1] = t0;
		buf[i+2] = t3;
		buf[i+3] = t2;
	}


}


void loop()
{


	while(dmaDesc[1].owner == 1) /* */; // wait dmaDesc[1] being finished by DMA 
	dmaDesc[1].owner = 1;
	build_first_half();

	while(dmaDesc[3].owner == 1) /* */; // wait dmaDesc[3] being finished by DMA 
	dmaDesc[3].owner = 1;
	build_second_half();

	++r;
	if(r >= 24) r = 0;

/*
	Serial.printf("==BEGIN %d==\r\n", bufp - buf);
	for(int i = 0; i < 4096; ++i)
	{
		Serial.printf("%4d:", i);
		for(int b = 5; b >= 0; --b)
		{
			Serial.print(((1<<b)&buf[i])  ? "1" : "0");
		}
		Serial.print("\r\n");
	}
	Serial.printf("==END==\r\n");
*/

/*
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
*/
}

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

	lcdIfaceInit();


	startDma();

}


