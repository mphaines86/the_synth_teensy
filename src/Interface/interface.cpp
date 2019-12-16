#include "interface.h"
#include "system/HD44780.h"
#include <synth.h>
#include <util/delay.h>
#include <system/utilities.h>
#include "tables_due.h"
#include "interface_sample.h"
#include "interface_parameters.h"
#include "interface_patches.h"

#define TIME_BUFFER 100
#define DEBOUNCE_MAX 20
#define SAMPLE_PAGES 16
#define LAYOUT_PAGES 2
#define POT_TIME 250
#define POT_THRESHOLD 1000
#define NUM_OF_BUTTONS 8
#define BUTTON_TIME 250
#define NUM_OF_SAMPLES 20

uint16_t cpParameterList[cpCount];
uint16_t spParameterList[spCount];
uint8_t analogBusy = 0;
uint8_t updatePage = 0;
uint8_t page_update = 0;

uint8_t spParameterBits[spCount] = {
        4, // wave A
        4, // wave B
        1, // sync
        1, // ring
        1, // fltr
        1, //
        1, // Amp
        1, //
        1, // Aux
        1, //
        2, // LFO A wave
        1, // LFO A
        1, // LFO A
        1, // LFO A
        2, // LFO B
        1, // LFO B
        1, // LFO B
        1, // LFO B

};

static struct {
    uint16_t pot_samples[NUM_OF_POTS][NUM_OF_SAMPLES];
    uint64_t pot_time[NUM_OF_POTS];
    uint8_t increament_potentiometer;
    uint16_t pot_value[NUM_OF_POTS];
    uint16_t current_sample[NUM_OF_POTS];
    uint8_t potAdress[NUM_OF_POTS];

    int8_t integrator[NUM_OF_BUTTONS];
    uint8_t increament_button;
    int16_t param_page;
    uint8_t page;
    uint32_t lastPressTime[NUM_OF_BUTTONS];
    uint64_t lastButtonPress;

    uint8_t temp_parameter;

} interface;


enum interfacePage_e {
    iLeft=0, iRight, iParam, iSample, iLayout, iMatrix, iSettings, iPatch
};



static int uint16Compare(const void * a,const void * b)
{
	if (*(uint16_t*)a==*(uint16_t*)b)
		return 0;
	else if (*(uint16_t*)a < *(uint16_t*)b)
		return -1;
	else
		return 1;
}

static uint32_t debounce(uint8_t port, uint8_t button){

    uint32_t output = 0;
    //test_variable = (GPIOC_PDIR&(1<<port));

    output = (GPIOC_PDIR&(1<<port))>>10;
    /*if (!(GPIOC_PDIR&(1<<port))){
        if (interface.integrator[button] > 0)
            interface.integrator[button]--;
    }
    else if (interface.integrator[button] < DEBOUNCE_MAX)
        interface.integrator[button]++;

    if (interface.integrator[button] == 0)
        output = 0;
    else if (interface.integrator[button] >= DEBOUNCE_MAX){
        output = 1;
        interface.integrator[button] = DEBOUNCE_MAX;
    }*/

    return output;
}

static void handleUserInput(int8_t input){

    // Set register output to 0 or LCD becomes corrupted
    GPIOA_PSOR = (1 << 17);
    GPIOD_PCOR = (interface.increament_potentiometer << 11);

    if (interface.page<iPatch && input > -1){
        // Serial.print(input);
        // Serial.print(interface.param_page);
        if (input == iParam){
            cmd2LCD(0x01);
            delay(2);
            //interfacePatchesLoadPatch(patchInfo.number);

            interface.param_page = patchInfo.number;
            static char dv[4] = {0};
            cposition(0, 0);
            sprintf(dv, "%3d:", patchInfo.number);
            putsLCD(dv);
            putsLCD((char *) patchInfo.name);
            return;
        }
        else if (input < 2){
            interfaceUpdatePage();
            return;
        }

        switch (interface.page) {
            case iSample: {
                if (!interfaceSampleFindZeroPoint(input, interface.param_page)) {
                    return;
                }

                delay(2);
                parameterChange();
                interfaceUpdatePage();
                break;
            }
            case iLayout:{
                break;
            }

            default:
                break;
        }
    }

    if (interface.page==iPatch && input > -1){
        switch (input){
            case iLeft:
            case iRight:
                interfaceUpdatePage();
                break;

                //interfaceUpdatePage();
                //break;

            case iParam:
            case iSample:
            case iLayout:
                interface.page = input;
                interface.param_page = 0;
                interfaceUpdatePage();
                break;

                /*interface.param_page = 0;
                interfaceUpdatePage();
                break;
                interface.param_page = 0;
                interfaceUpdatePage();
                break;*/
            case iMatrix:
            case iSettings:
                break;
            case iPatch: {
                interfaceUpdatePage();
                break;
            }
            default:
                break;
        }

    }



    if(input<iLeft){
        switch (interface.page) {
            case iParam: {
                input = -input - 1;

                interfaceParameterHandleUserInput(input, interface.pot_value[input], interface.param_page);

                parameterChange();
                break;
            }

            case iSample: {
                NVIC_DISABLE_IRQ(IRQ_FTM1);
                interfaceSampleHandleUserInput(input, interface.param_page, interface.pot_value);

                parameterChange();
                NVIC_ENABLE_IRQ(IRQ_FTM1);

                break;
            }
            case iLayout:
                break;
            default:
                break;
        }
    }
  }

void interfaceUpdatePage(){

    switch (interface.page) {
        case iParam: {
            if (interface.param_page == -1)
                interface.param_page = PARAMETER_PAGES -1;

            interface.param_page %= PARAMETER_PAGES;
            interfaceParameterUpdatePage(interface.param_page);
            break;
        }
        case iSample: {
            if (interface.param_page == -1)
                interface.param_page = SAMPLE_PAGES-1;
            interface.param_page %= SAMPLE_PAGES;
            interfaceSampleUpdatePage(interface.param_page);
            break;
        }
        case iLayout:
            cmd2LCD(0x01);
            delay(2);
            static char dv[9] = {0};
            cposition(0, 0);

            if(interface.param_page < 2){
                const char *row1 = "Sample: ";
                putsLCD(row1);
                putsLCD(waveStruct[interface.param_page].name);
            }
        case iPatch:{
            if (interface.param_page == -1)
                interface.param_page = EEPROM_NUM_OF_PATCHES-1;

            interface.param_page %= EEPROM_NUM_OF_PATCHES;
            interfacePatchesUpdatePage(interface.param_page);
            break;
        }
        default:
            break;
    }
}

static void readButton(){

    uint32_t output = debounce(10, interface.increament_button);
    //uint8_t output = (uint8_t)(GPIOC_PDIR&(1<<10))>>10;
    if(output == 1){
        //test_variable = tik - interface.lastButtonPress;
        //Serial.println(test_variable);
        if (tik - interface.lastButtonPress >= BUTTON_TIME){
            //Serial.print(interface.increament_button);
            //Serial.print(":");
            //Serial.println(output);
            updatePage = 1;
            GPIOA_PSOR = (1 << 17);

            if (interface.increament_button < 2) {
                if (!interface.increament_button) {
                    interface.param_page--;
                } else {
                    interface.param_page++;
                }
                //Serial.println(interface.increament_button);
                handleUserInput(interface.increament_button);

            } else if (interface.increament_button > 1) {
                //Serial.println(interface.page);
                if (interface.page == iPatch) {
                    //Serial.println("Patch");
                    handleUserInput(interface.increament_button);
                } else if (interface.increament_button == 2){
                    interface.page = iPatch;
                    handleUserInput(interface.page);
                }else{
                    handleUserInput(interface.increament_button);
                }
            }
            updatePage = 1;
        }
        interface.lastButtonPress = tik;
    }

    interface.increament_button = (interface.increament_button+1)%NUM_OF_BUTTONS;
}




static void readPotentiometers(){
    uint32_t new_value;
  	uint16_t tmp[NUM_OF_SAMPLES];
    page_update = 0;

    if (!analogBusy){
        GPIOD_PSOR = (interface.increament_potentiometer << 11);
        GPIOA_PCOR = (1 << 17);
        analogBusy = 1;
        ADC0_SC1A = 23;
        // Read buttons because button mux is synced to pot mux
        _delay_us(25);
        interface.increament_button = interface.increament_potentiometer;
        readButton();

    }

    if ((ADC0_SC1A & ADC_SC1_COCO)){
        if (updatePage){
            analogBusy = 0;
            updatePage=0;
            return;
        }
        interface.pot_samples[interface.increament_potentiometer]
        [interface.current_sample[interface.increament_potentiometer]]= ADC0_RA;
        GPIOA_PSOR = (1 << 17);
        GPIOD_PCOR = (interface.increament_potentiometer << 11);
        analogBusy = 0;

        memcpy(&tmp[0], &interface.pot_samples[interface.increament_potentiometer][0],
               NUM_OF_SAMPLES*sizeof(uint16_t));
        qsort(tmp, NUM_OF_SAMPLES, sizeof(uint16_t), uint16Compare);

        new_value = tmp[NUM_OF_SAMPLES/2];

        if (abs(new_value-interface.pot_value[interface.increament_potentiometer]) >= POT_THRESHOLD){
            if(tik - interface.pot_time[interface.increament_potentiometer] >= POT_TIME) {
                interface.pot_value[interface.increament_potentiometer] = new_value;
                handleUserInput(-interface.increament_potentiometer-1);
            }
            else if (abs(new_value-interface.pot_value[interface.increament_potentiometer]) >= 2*POT_THRESHOLD){
                interface.pot_value[interface.increament_potentiometer] = new_value;
                handleUserInput(-interface.increament_potentiometer-1);
            }
            interface.pot_time[interface.increament_potentiometer] = tik;
        }

        interface.increament_potentiometer=
                (interface.increament_potentiometer+1)%NUM_OF_POTS;

        interface.current_sample[interface.increament_potentiometer] =
                (interface.current_sample[interface.increament_potentiometer] + 1) %
                NUM_OF_SAMPLES;
    }


}


void interfaceInit() {
    //memcpy(&interface.potAdress[0], &tmp[0], sizeof(uint8_t));
    GPIOA_PSOR = (1 << 17);

    for(unsigned long long &value: interface.pot_time){
        value=0;
    }

    for(unsigned short &value: interface.pot_value){
        value=0;
    }

    for(unsigned short &value: interface.current_sample){
        value=0;
    }

    /*for(unsigned long long &value: interface.lastButtonPress){
        value=0;
    }*/
    interface.lastButtonPress = 0;

    openLCD();
    delay(200);
    const char *msg1="mphaines88/Roykeru";
    putsLCD(msg1);
    cposition(0,2);
    delay(2);
    const char *msg2="SX8 Synthesizer";
    putsLCD(msg2);
    delay(2000);
    //cmd2LCD(0x01);
    analogReadRes(16);

    interface.temp_parameter = 255;
    interface.page=iPatch;

    Serial.println("Patch Codes: ");
    Serial.println(interfacePatchesInitSystem());
    interface.param_page = patchInfo.number;
    handleUserInput(interface.page);
    //interfaceUpdatePage();
    interfaceUpdate();

}

void interfaceUpdate(){
    if (updatePage & !analogBusy){
        //interfaceUpdatePage();
        //updatePage = 0;
    }
    readPotentiometers();

}
