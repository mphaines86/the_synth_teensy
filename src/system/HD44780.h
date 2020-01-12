/*
 * HD44780.h
 *
 * Created: 28/08/2012 9:52:43 PM
 *  Author: Michael
 */


#ifndef HD44780_H
#define HD44780_H

#include <Arduino.h>


void lcdCmd(char cmd);   // Send command to the screen
void lcdOpen(void);      // send commands to setup the screen 4 bit, etc
void lcdSendChar(char cx);    // send char to the lcd
void lcdSendCharArray(const char *ptr);  // send full string to lcd
void lcdClearScreen(void);   // send command to clear the screen of characters
void lcdClearline(int line);   // send command to clear a specific  line.
void lcdChangePos(int x, int y);  // set position of curser (x,y) positions starting at 0,1
void lcdCustomChar(const uint8_t *character, uint8_t ccram);

#endif /* HD44780_H_ */
