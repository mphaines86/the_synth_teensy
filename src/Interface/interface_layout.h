//
// Created by michael on 11/9/19.
//

#ifndef THE_SYNTH_TEENSY_INTERFACE_LAYOUT_H
#define THE_SYNTH_TEENSY_INTERFACE_LAYOUT_H

int8_t interfaceLayoutInit(uint8_t patch);
int8_t interfaceLayoutLoad(uint8_t patch);
int8_t interfaceLayoutSave(uint8_t patch);
void interfaceLayoutUpdatePage(uint8_t paramPage);
void interfaceLayoutHandleUserInput();

#endif //THE_SYNTH_TEENSY_INTERFACE_LAYOUT_H
