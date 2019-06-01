/*
 * HD44780.h
 *
 * Created: 28/08/2012 9:52:43 PM
 *  Author: Michael
 */


#ifndef HD44780_H
#define HD44780_H

#include <Arduino.h>


void cmd2LCD(char cmd);   // Send command to the screen
void openLCD(void);      // send commands to setup the screen 4 bit, etc
void putcLCD(char cx);    // send char to the lcd
void putsLCD(const char *ptr);  // send full string to lcd
void clearscreen(void);   // send command to clear the screen of characters
void clearline(int line);   // send command to clear a specific  line.
void cposition(int x, int y);  // set position of curser (x,y) positions starting at 0,1
void CChar(int *character, int ccram);

#endif /* HD44780_H_ */
