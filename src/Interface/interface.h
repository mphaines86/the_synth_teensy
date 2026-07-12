#ifndef INTERFACE_H
#define INTERFACE_H

#include <stdint.h>


#define NUM_OF_POTS 8

typedef enum {
    STATE_PATCH,
    STATE_PARAMETER,
    STATE_SAMPLE_EDIT,
    STATE_LAYOUT,
    STATE_MATRIX,
    STATE_SETTINGS,
    STATE_NAME,
    STATE_SAVING,
    STATE_INITIALIZING
} InterfaceState;

typedef enum {
    EVENT_BUTTON_LEFT_PRESSED,
    EVENT_BUTTON_RIGHT_PRESSED,
    EVENT_BUTTON_ONE_PRESSED,
    EVENT_BUTTON_TWO_PRESSED,
    EVENT_BUTTON_THREE_PRESSED,
    EVENT_BUTTON_FOUR_PRESSED,
    EVENT_BUTTON_FIVE_PRESSED,
    EVENT_BUTTON_SIX_PRESSED,
    EVENT_KNOB_CHANGED,
    EVENT_TIMER_EXPIRED,
    EVENT_PAGE_CHANGED,
    EVENT_NONE
} InterfaceEvent;

typedef struct {
    InterfaceEvent type;
    union {
        struct {
            uint8_t pot_id;
            uint16_t value;
        } pot;
        struct {
            uint8_t button_id;
        } button;
        struct {
            uint8_t page_id;
        } page;
    } data;
} InterfaceEventData;

// Define UI context structure
typedef struct {
    InterfaceState current_state;
    uint8_t current_page;
    int16_t current_patch;
    bool name_edit_mode;
    uint8_t selected_parameter;
    uint8_t timer_id;
    uint64_t last_event_time;
    // Retain other necessary context from original interface struct
} InterfaceContext;

// Function declarations
void interfaceInit(void);
void interfaceProcessEvent(InterfaceEventData* event);
void interfaceUpdate(void);
void interfaceTransition(InterfaceState new_state);
//void interfaceRenderer(void);
uint16_t interfaceGetPotValues(uint8_t input);
void interfaceUpdateNoteList(int8_t note, uint8_t state);

#endif