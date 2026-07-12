#include "interface.h"
#include "system/HD44780.h"
#include "interface_parameters.h"
#include "interface_patches.h"
#include "interface_sample.h"
#include "interface_layout.h"
#include "synth.h"
#include <util/delay.h>
#include "main.h"
#include "parameter.h"

// Old stuff that still needs included?

uint8_t analogBusy = 0;
uint8_t updatePage = 0;
uint8_t page_update = 0;

// Global interface context
static InterfaceContext interface_ctx;

// Timers
#define DEBOUNCE_TIMER 0
#define KNOB_TIMER 1
#define MESSAGE_TIMER 2

#define DEBOUNCE_MAX 20
#define POT_THRESHOLD 256
#define POT_UPDATE_TIME 200
#define BUTTON_DEBOUNCE_TIME 250
#define NUM_OF_POTS 8
#define NUM_OF_BUTTONS 8
#define NUM_OF_SAMPLES 64

#define SAMPLE_PAGES 16
#define LAYOUT_PAGES 2

// Store knob values
static struct {
    uint16_t pot_samples[NUM_OF_POTS][NUM_OF_SAMPLES];
    uint64_t pot_time[NUM_OF_POTS];
    uint8_t increament_potentiometer;
    uint16_t pot_value[NUM_OF_POTS];
    uint16_t current_sample[NUM_OF_POTS];
    uint8_t potAdress[NUM_OF_POTS];

    int8_t integrator[NUM_OF_BUTTONS];
    uint8_t increament_button;
    uint32_t lastPressTime[NUM_OF_BUTTONS];
    uint64_t lastButtonPress;

} interface;

// Forward declarations for state handlers
static void handle_patch_browser_state(InterfaceEventData* event);
static void handle_parameter_edit_state(InterfaceEventData* event);
static void handle_sample_edit_state(InterfaceEventData* event);
static void handle_layout_edit_state(InterfaceEventData* event);
static void handle_name_edit_state(InterfaceEventData* event);
static void handle_saving_state(InterfaceEventData* event);
static void handle_initializing_state(InterfaceEventData* event);

// State handler function pointer array
typedef void (*StateHandler)(InterfaceEventData*);

static const StateHandler state_handlers[] = {
    handle_patch_browser_state,
    handle_parameter_edit_state,
    handle_sample_edit_state,
    handle_layout_edit_state,
    NULL, // Matrix edit - not implemented
    NULL, // Settings - not implemented
    handle_name_edit_state,
};


// Initialize the UI system
void interfaceInit(void) {

    GPIOA_PSOR = (1 << 17);

    // Initialize LCD
    lcdOpen();
    delay(200);
    
    // Setup custom characters
    const uint8_t custom_char[] = {
        0x06, 0x07, 0x05, 0x04, 0x1C, 0x1C, 0x1C, 0x00
    };
    lcdCustomChar(custom_char, 0);
    
    // Display welcome message
    const char *msg1 = "mphaines86/Roykeru";
    lcdSendCharArray(msg1);
    lcdChangePos(0, 1);
    delay(2);
    const char *msg2 = "SX8 Synthesizer";
    lcdSendCharArray(msg2);
    delay(2000);
    
    // Initialize knob and button tracking

    for(uint64_t &value: interface.pot_time){
        value=0;
    }

    for(uint16_t &value: interface.pot_value){
        value=0;
    }

    for(uint16_t &value: interface.current_sample){
        value=0;
    }

    interface.lastButtonPress = 0;
    
    // Initialize analog system
    analogReadRes(12);
    
    // Initialize context
    interface_ctx.current_state = STATE_PATCH;
    interface_ctx.current_page = 0;
    interface_ctx.name_edit_mode = false;
    interface_ctx.selected_parameter = 0;
    interface_ctx.last_event_time = 0;
    
    // Initialize patches subsystem
    interfacePatchesInitSystem();
    interface_ctx.current_patch = patchInfo.number;
    
    // Initial render
    interfaceTransition(STATE_PATCH);
}


// Transition to a new state
void interfaceTransition(InterfaceState new_state) {
    // Handle any exit actions for the current state
    switch (interface_ctx.current_state) {
        case STATE_NAME:
            interface_ctx.name_edit_mode = false;
            break;
        default:
            break;
    }
    
    // Update state
    interface_ctx.current_state = new_state;
    Serial.print("Transition Number: ");
    Serial.println(interface_ctx.current_state);
    // Handle any entry actions for the new state
    switch (new_state) {
        case STATE_PATCH:
            interfacePatchesUpdatePage(interface_ctx.current_patch);
            break;
        case STATE_PARAMETER:
            interfaceParameterUpdatePage(interface_ctx.current_page);
            break;
        case STATE_SAMPLE_EDIT:
            interfaceSampleUpdatePage(interface_ctx.current_page);
            break;
        case STATE_LAYOUT:
            interfaceLayoutUpdatePage(interface_ctx.current_page);
            break;
        case STATE_NAME:
            {
            interface_ctx.name_edit_mode = true;
            // Clear display and show name edit interface
            lcdCmd(0x01);
            delay(2);
            lcdChangePos(0, 0);
            const char *row1 = "Patch Name:";
            lcdSendCharArray(row1);
            
            break;
            }
        default:
            break;
    }
}

// State handler implementations

static void handle_patch_browser_state(InterfaceEventData* event) {
            
    switch (event->type) {
        case EVENT_BUTTON_LEFT_PRESSED:
            interface_ctx.current_patch = (interface_ctx.current_patch == 0) ? 
                                   EEPROM_NUM_OF_PATCHES - 1 : interface_ctx.current_patch - 1;
            interfacePatchesUpdatePage(interface_ctx.current_patch);
            break;
            
        case EVENT_BUTTON_RIGHT_PRESSED:
            interface_ctx.current_patch = (interface_ctx.current_patch + 1) % EEPROM_NUM_OF_PATCHES;
            interfacePatchesUpdatePage(interface_ctx.current_patch);
            break;
            
        case EVENT_BUTTON_ONE_PRESSED:
            interface_ctx.current_page = 0;
            interfaceTransition(STATE_PARAMETER);
            
            break;
            
        case EVENT_BUTTON_TWO_PRESSED:
            interface_ctx.current_page = 0;
            interfaceTransition(STATE_SAMPLE_EDIT);
            break;
            
        case EVENT_BUTTON_THREE_PRESSED:
            interface_ctx.current_page = 0;
            interfaceTransition(STATE_LAYOUT);
            break;


        case EVENT_BUTTON_FIVE_PRESSED:
            interfaceTransition(STATE_NAME);
            break;

        case EVENT_KNOB_CHANGED:
            // Use knob to quickly select patches
            /*
            if (event->data.pot.pot_id == 0) {
                uint32_t temp_var = EEPROM_NUM_OF_PATCHES * ((event->data.pot.value >> 8)) / 255;
                interface_ctx.current_patch = temp_var;
                interfacePatchesUpdatePage(interface_ctx.current_patch);
            }
                */
            break;
            
        default:
            break;
    }
}

static void handle_parameter_edit_state(InterfaceEventData* event) {
    switch (event->type) {
        case EVENT_BUTTON_LEFT_PRESSED:
            interface_ctx.current_page = (interface_ctx.current_page == 0) ? 
                                 PARAMETER_PAGES - 1 : interface_ctx.current_page - 1;
            interfaceParameterUpdatePage(interface_ctx.current_page);
            break;
            
        case EVENT_BUTTON_RIGHT_PRESSED:
            interface_ctx.current_page = (interface_ctx.current_page + 1) % PARAMETER_PAGES;
            interfaceParameterUpdatePage(interface_ctx.current_page);
            break;
            
        case EVENT_BUTTON_ONE_PRESSED:
            interfaceTransition(STATE_PATCH);
            break;

        case EVENT_BUTTON_TWO_PRESSED:
            interfacePatchesInitPatch(interface_ctx.current_patch);
            synthParameterChange();
            interfacePatchesUpdatePage(interface_ctx.current_patch);
            break;

        case EVENT_BUTTON_FOUR_PRESSED:
            interfacePatchesSetWriteProtect(interface_ctx.current_patch);
            interfacePatchesUpdatePage(interface_ctx.current_patch);
            break;
        
        default:
            interfaceParameterHandleUserInput(event, interface_ctx.current_page);
            synthParameterChange();
            break;
    }
}

static void handle_sample_edit_state(InterfaceEventData* event) {
    switch (event->type) {
        case EVENT_BUTTON_LEFT_PRESSED:
            interface_ctx.current_page = (interface_ctx.current_page == 0) ? 
                                 SAMPLE_PAGES - 1 : interface_ctx.current_page - 1;
            interfaceSampleUpdatePage(interface_ctx.current_page);
            break;
            
        case EVENT_BUTTON_RIGHT_PRESSED:
            interface_ctx.current_page = (interface_ctx.current_page + 1) % SAMPLE_PAGES;
            interfaceSampleUpdatePage(interface_ctx.current_page);
            break;
            
        case EVENT_BUTTON_ONE_PRESSED:
            interfaceTransition(STATE_PATCH);
            break;
            
        case EVENT_BUTTON_TWO_PRESSED:
            NVIC_DISABLE_IRQ(IRQ_FTM1);
            if (interfaceSampleFindZeroPoint(event->data.button.button_id, interface_ctx.current_page)) {
                synthParameterChange();
                interfaceSampleUpdatePage(interface_ctx.current_page);
            }
            NVIC_ENABLE_IRQ(IRQ_FTM1);
            break;
            
        case EVENT_KNOB_CHANGED:
            NVIC_DISABLE_IRQ(IRQ_FTM1);
            // TODO: check if interface.pot_value is correct
            
            interfaceSampleHandleUserInput(interface.pot_value[0] >> 14, 
                                          event->data.pot.pot_id,
                                          interface_ctx.current_page, 
                                          interface.pot_value);
            synthParameterChange();
            
            NVIC_ENABLE_IRQ(IRQ_FTM1);
            break;
            
        default:
            break;
    }
}

static void handle_layout_edit_state(InterfaceEventData* event) {
    switch (event->type) {
        case EVENT_BUTTON_LEFT_PRESSED:
            interface_ctx.current_page = (interface_ctx.current_page == 0) ? 
                                 LAYOUT_PAGES - 1 : interface_ctx.current_page - 1;
            interfaceLayoutUpdatePage(interface_ctx.current_page);
            break;
            
        case EVENT_BUTTON_RIGHT_PRESSED:
            interface_ctx.current_page = (interface_ctx.current_page + 1) % LAYOUT_PAGES;
            interfaceLayoutUpdatePage(interface_ctx.current_page);
            break;
            
        case EVENT_BUTTON_ONE_PRESSED:
            interfaceTransition(STATE_PATCH);
            break;
            
        case EVENT_KNOB_CHANGED:
            // Implementation depends on layout editing logic
            interfaceLayoutHandleUserInput();
            break;
            
        default:
            break;
    }
}

static void handle_name_edit_state(InterfaceEventData* event) {
    static unsigned char buffer[EEPROM_PATCH_NAME_LENGTH] = {0};
    
    switch (event->type) {
        case EVENT_BUTTON_ONE_PRESSED:
        {
            // Save the name
            static unsigned char buffer[EEPROM_PATCH_NAME_LENGTH] = {0};
            for(uint8_t i=0; i < EEPROM_PATCH_NAME_LENGTH; i++){
                buffer[i] = static_cast<unsigned char>(96 * (interface.pot_value[i] >> 8) / 255 + 32);
            }
            memcpy(&patchInfo.name[0], (char *) &buffer[0], EEPROM_PATCH_NAME_LENGTH*sizeof(char));
            //patch_name_edit_marker=0;
            interfaceTransition(STATE_PARAMETER);
            break;
        }
        case EVENT_BUTTON_TWO_PRESSED:
            interfaceTransition(STATE_PARAMETER);
            break;
        case EVENT_KNOB_CHANGED:
        {
            unsigned char character = static_cast<unsigned char>(96 * (event->data.pot.value >> 8) / 255 + 32);
            //patchInfo.name[input] = reinterpret_cast<char *>(character);
        
            lcdChangePos(event->data.pot.value, 1);
            lcdSendChar(character);
            break;
        }
        default:
            break;
    }
}

static int uint16Compare(const void * a,const void * b)
{
	if (*(uint16_t*)a==*(uint16_t*)b)
		return 0;
	else if (*(uint16_t*)a < *(uint16_t*)b)
		return -1;
	else
		return 1;
}

uint16_t interfaceGetPotValues(uint8_t input) {
    return interface.pot_value[input];
}

void interfaceUpdateNoteList(int8_t note, uint8_t state){
    if (interface_ctx.current_state == STATE_PATCH){
        lcdChangePos(note, 3);
        if(state){
            lcdSendChar(static_cast<char>(0x00));
        }
        else{
            lcdSendChar(static_cast<char>(0b10100000));
        }

    }
}

// Process a UI event
void interfaceProcessEvent(InterfaceEventData* event) {
    // Potentiometer mux on same line as LCD so disable POT mux 
    GPIOA_PSOR = (1 << 17);
    GPIOB_PCOR = (interface.increament_potentiometer);
    // Process event based on current state
#ifdef DEBUG
    Serial.print("Current State Number: ");
    Serial.println(interface_ctx.current_state);
#endif    
    if (state_handlers[interface_ctx.current_state]) {
        state_handlers[interface_ctx.current_state](event);
    }
    
    // Update the display if needed
    //update_display();
}


// Update function to be called from main loop
void interfaceUpdate(void) {
    
    uint32_t new_value;
  	uint16_t tmp[NUM_OF_SAMPLES];
    page_update = 0;

    if (!analogBusy){
        GPIOB_PSOR = interface.increament_potentiometer;
        GPIOA_PCOR = (1 << 17);
        analogBusy = 1;
        ADC0_SC1A = 23;
        
        // Read buttons because button mux is synced to pot mux
        _delay_us(25);
        interface.increament_button = interface.increament_potentiometer;
        
        uint32_t output = 0;
        //test_variable = (GPIOC_PDIR&(1<<port));
        
        output = (GPIOA_PDIR&(1<<5))>>5;

        //uint32_t output = debounce(10, interface.increament_button);
        //uint8_t output = (uint8_t)(GPIOC_PDIR&(1<<10))>>10;
        if(output == 1){
            //test_variable = tik - interface.lastButtonPress;
            //Serial.println(test_variable);
            if (tik - interface.lastButtonPress >= BUTTON_DEBOUNCE_TIME){
                
#ifdef DEBUG
                Serial.print("Button Number: ");
                Serial.print(interface.increament_button);
                Serial.print(", Value:");
                Serial.println(output);
                updatePage = 1;
                GPIOA_PSOR = (1 << 17);
#endif
                
                InterfaceEventData event;
                event.type = (InterfaceEvent) interface.increament_button;
                event.data.button.button_id = interface.increament_button;
                interfaceProcessEvent(&event);
                
            }
            interface.lastButtonPress = tik;
        }

        interface.increament_button = (interface.increament_button+1)%NUM_OF_BUTTONS;

    }

    if ((ADC0_SC1A & ADC_SC1_COCO)){
        if (updatePage){
            analogBusy=0;
            updatePage=0;
            return;
        }
        
        interface.pot_samples[interface.increament_potentiometer]
        [interface.current_sample[interface.increament_potentiometer]]= ADC0_RA;
        GPIOA_PSOR = (1 << 17);
        GPIOB_PCOR = interface.increament_potentiometer;
        analogBusy = 0;

        memcpy(&tmp[0], &interface.pot_samples[interface.increament_potentiometer][0],
               NUM_OF_SAMPLES*sizeof(uint16_t));
        qsort(tmp, NUM_OF_SAMPLES, sizeof(uint16_t), uint16Compare);

        new_value = tmp[NUM_OF_SAMPLES/2] * 16;

        if ((new_value >> 8) <= 4){
            new_value=0;
        }

        if (abs((int32_t)(new_value-interface.pot_value[interface.increament_potentiometer])) >= POT_THRESHOLD){
            
            InterfaceEventData event;
            event.type = EVENT_KNOB_CHANGED;
            
            
            if(tik - interface.pot_time[interface.increament_potentiometer] <= POT_UPDATE_TIME) {
#ifdef DEBUG
                Serial.print("Potentiometer Number: ");
                Serial.print(interface.increament_potentiometer);
                Serial.print(", value: ");
                Serial.println(new_value >> 8);
#endif
                interface.pot_value[interface.increament_potentiometer] = new_value;
                event.data.pot.value = new_value;
                event.data.pot.pot_id = interface.increament_potentiometer;
                interfaceProcessEvent(&event);
                interface.pot_time[interface.increament_potentiometer] = tik;
                
            }
            else if (abs((int32_t)(new_value-interface.pot_value[interface.increament_potentiometer])) >= 2*POT_THRESHOLD){
                interface.pot_value[interface.increament_potentiometer] = new_value;
                event.data.pot.value = new_value;
                event.data.pot.pot_id = interface.increament_potentiometer;
                interfaceProcessEvent(&event);
                interface.pot_time[interface.increament_potentiometer] = tik;
            }

            
        }

        interface.increament_potentiometer=
                (interface.increament_potentiometer+1)%NUM_OF_POTS;

        interface.current_sample[interface.increament_potentiometer] =
                (interface.current_sample[interface.increament_potentiometer] + 1) %
                NUM_OF_SAMPLES;
    }
    
}


