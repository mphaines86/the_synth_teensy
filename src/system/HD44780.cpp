/*
 * HD44780.c
 *
 * Created: 28/08/2012 9:54:54 PM
 *  Author: Michael
 *  This library works with all HD44780 LCD MODULES
 */
#include "HD44780.h"         /* standard input and output library */
#include "utilities.h"
#include "util/delay.h"
#include "main.h" //TODO: Remove and improve debug code

///////////////////////////////////////////////////////////////////////////
/*			LCD		DUE		SAM3X8E
			D4		4		C26 now C15
			D5		5		C25 now C16
			D6		6		C24 now C17
			D7		7		C23 now C18
			E		  8		C21 now C17
			RS		9		C22 now C18
*/
///////////////////////////////////////////////////////////////////////////
#define  LCD_DAT  REG_PIOC_SODR   /* Port C drives LCD data pins, E, and RS */
///////////////////////////////////////////////////////////////////////////


/*   Confirm the pin out of your LCD Shield or LCD Project board
    This LCD Shield Uses The last 4 bits of portD for parallel processing
 */


// LCD MODULES void lcdCmd(char cmd)
void lcdCmd(char cmd)
{
	int SendCmd=0;

	SendCmd = cmd;
	SendCmd &=0xF0;          /* zero out the lower 4 bits */
	SendCmd >>= 4;

	GPIOB_PSOR = static_cast<uint32_t>(SendCmd);
	GPIOB_PCOR = 1 << 4; // pull rs LOW
	GPIOB_PSOR = 1 << 18;  /* pull E signal to high */
	_delay_us(8);

	GPIOB_PCOR = 1 << 18; /*pull e signal low*/
	GPIOB_PCOR = static_cast<uint32_t>(SendCmd);

	SendCmd = cmd & 0x0F;    /* extract the lower four bits */
	//SendCmd <<= 11;
	GPIOB_PSOR = static_cast<uint32_t>(SendCmd);
	GPIOB_PSOR = 1 << 18;  /* pull E signal to high */

	_delay_us(8);
	GPIOB_PCOR = 1 << 18; // pull e low
	GPIOB_PCOR = static_cast<uint32_t>(SendCmd);
	GPIOB_PCOR = 1 << 4; // pull rs low

	//LCD_E_RS_DAT &= (0 << LCD_E)| (1<< LCD_BL);  /* pull E clock to low */
	_delay_us(30);       /* wait until the command is complete */
}



void lcdOpen(void)
{
	//LCD_DIR |=  (1 <<PORTD4) |(1 <<PORTD5) |(1 <<PORTD6) |(1 <<PORTD7);       /* configure LCD_DAT port for output */
	//LCD_E_RS_DIR =  (1 << LCD_E) | (1 << LCD_RS) | (1 << LCD_BL);
	//_delay_ms(100);
	//lcdCmd(0x20);        /* set 4-bit data, 2-line display, 5x7 font */
	_delay_ms(100);        /* wait until "clear display" command is complete */
	lcdCmd(0x28);        /* set 4-bit data, 2-line display, 5x7 font */
	_delay_ms(100);        /* wait until "clear display" command is complete */
	//lcdCmd(0x08);        /* set 4-bit data, 2-line display, 5x7 font */
	_delay_ms(100);        /* wait until "clear display" command is complete */
	lcdCmd(0x0F);        /* turn on display, cursor, blinking */
	_delay_ms(100);        /* wait until "clear display" command is complete */
	//lcdCmd(0x06);        /* Set the cursor to move right*/
	//_delay_ms(100);        /* wait until "clear display" command is complete */
	lcdCmd(0x01);        /* clear screen, move cursor to home */
	_delay_ms(100);        /* wait until "clear display" command is complete */
#ifdef DEBUG
    Serial.println("Finished LCD init");
#endif
}


void lcdSendChar(char cx)
{

	int SendCmd=0;
	SendCmd = cx & 0xF0;    /* zero out the lower 4 bits */
	SendCmd >>= 4;

    GPIOB_PSOR = static_cast<uint32_t>(SendCmd);
    GPIOB_PSOR = 1 << 4; // pull rs HIGH
    GPIOB_PSOR = 1 << 18;  /* pull E signal to high */
    _delay_us(8);

    GPIOB_PCOR = 1 << 18; /*pull e signal low*/
    GPIOB_PCOR = static_cast<uint32_t>(SendCmd);

    SendCmd = cx & 0x0F;    /* extract the lower four bits */
    //SendCmd <<= 11;
    GPIOB_PSOR = static_cast<uint32_t>(SendCmd);
    GPIOB_PSOR = 1 << 18;  /* pull E signal to high */

    _delay_us(8);
    GPIOB_PCOR = 1 << 18; // pull e low
    GPIOB_PCOR = static_cast<uint32_t>(SendCmd);
    GPIOB_PCOR = 1 << 4; // pull rs low

	//LCD_E_RS_DAT &= (0 << LCD_E)| (1<< LCD_BL);  /* pull E clock to low */
	_delay_us(30);       /* wait until the command is complete */
}

void lcdSendCharArray(const char *ptr)  /* breaks the string down to characters ie a pointer to an address that holds the string */
{
	while (*ptr) {     /* while char pointer is not null*/
		lcdSendChar(*ptr);  /* put the character on the screen on letter at a time */
 		ptr++;   /* increment the memory the pointer points to */
	}
}


void lcdClearScreen(void)
{
	lcdCmd(0x01);  // clears screen  0x08 blanks the display without clearing
	lcdChangePos(0, 0);   // returns cursor home
}

void lcdClearline(int line)
{
	lcdChangePos(0, line);
	for (int i=0; i < 20; i++) { lcdSendCharArray(" "); }
	lcdChangePos(0, line);
}

void lcdChangePos(int x, int y)
{
	int temp = 0x00 + x;

	switch(y)
	{
		case 0:
		temp += (0x80);      /// return home
		break;
		case 1:
		temp += (0xC0);      /// Line 2
		break;
		case 2:
		temp += (0x94);      /// line 3
		break;
		case 3:
		temp += (0xD4);      /// line 4
		break;
	}
	lcdCmd(temp);
}


/*  ************************************************************
Custom character function
	you will need to pass an array of 8 integers see my example code to accomplish this.
	characters are defined by 8 sets bytes. Each Byte represents one Row(ie 1st byte is 1st row) the first 5 bytes represent the on and off
	states of the pixels.
	Please note that each time a character is defined the cursor returns home.
*/
void lcdCustomChar(const uint8_t *character, uint8_t ccram)
{
	int x= 0; /// temp var for counting.
	char command= (0x40+(ccram*8));  // adds the cc ram address 0x40 + the input for the cc (note: each cc takes 8 bytes)
	lcdCmd(command);  /// selects the first location of CG ram

	while (x < 8)
	{
		lcdSendChar(character[x]);
		x++;
	}
	lcdCmd(0x80);   /// send curser back home
}
