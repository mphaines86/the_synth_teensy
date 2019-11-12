//
// Created by michael on 11/9/19.
//

#include <stdint.h>
#include <tables_due.h>
#include <system/HD44780.h>
#include "interface_sample.h"

uint8_t interfaceSampleFindZeroPoint(uint8_t page, uint8_t paramPage){
    NVIC_DISABLE_IRQ(IRQ_FTM1);
    if (page == 3){
        cmd2LCD(0x01);
        delay(2);
        cposition(0, 0);
        const char *message = "Finding End Point  Zero Point...";
        putsLCD(message);
        uint32_t start_sample_loc = waveStruct[paramPage].end_length >> 9;
        int8_t sample = *(waveStruct[paramPage].wave + start_sample_loc) - 127;
        uint8_t sign_bit = 0;
        if (sample > 0)
            sign_bit = 1;
        uint8_t failure = 0;

        while (true){
            if (start_sample_loc <= 1){
                failure = 1;
                break;
            }
            //Serial.print(start_sample_loc);
            //Serial.print(", ");
            //Serial.println(sample);
            start_sample_loc--;
            sample = *(waveStruct[paramPage].wave + start_sample_loc) - 127;
            if (((sample > 0) != sign_bit) || sample == 0)
                break;
        }
        if(failure) {
            NVIC_ENABLE_IRQ(IRQ_FTM1);
            return 0;
        }

        waveStruct[paramPage].end_length = start_sample_loc << 9;

        if (waveStruct[paramPage].end_length < waveStruct[paramPage].loop_point){
            waveStruct[paramPage].loop_point = waveStruct[paramPage].end_length;
        }

        NVIC_ENABLE_IRQ(IRQ_FTM1);
        return 1;
    }

    else if (page == 4){
        cmd2LCD(0x01);
        delay(2);
        cposition(0, 0);
        const char *message = "Finding Loop Point  Zero Point...";
        putsLCD(message);
        uint32_t start_sample_loc = waveStruct[paramPage].loop_point >> 9;
        int8_t sample = *(waveStruct[paramPage].wave + start_sample_loc) - 127;
        uint8_t sign_bit = 0;
        if (sample > 0)
            sign_bit = 1;
        uint8_t failure = 0;

        while (true){
            if (start_sample_loc <= 1){
                failure = 1;
                break;
            }
            //Serial.print(start_sample_loc);
            //Serial.print(", ");
            //Serial.println(sample);
            start_sample_loc--;
            sample = *(waveStruct[paramPage].wave + start_sample_loc) - 127;
            if (((sample > 0) != sign_bit) || sample == 0)
                break;
        }

        if(failure){
            NVIC_ENABLE_IRQ(IRQ_FTM1);
            return 0;
        }

        waveStruct[paramPage].loop_point = start_sample_loc << 9;

        if (waveStruct[paramPage].end_length < waveStruct[paramPage].loop_point){
            waveStruct[paramPage].end_length = waveStruct[paramPage].loop_point;
        }

        NVIC_ENABLE_IRQ(IRQ_FTM1);
        return 1;
    }
}

void interfaceSampleUpdatePage(uint8_t paramPage) {
    cmd2LCD(0x01);
    delay(2);
    static char dv[9] = {0};
    cposition(0, 0);
    putsLCD(waveStruct[paramPage].name);

    cposition(0, 1);
    const char *row2_param = "End Point:  ";
    putsLCD(row2_param);
    sprintf(dv, "%8ld", waveStruct[paramPage].end_length>>9);
    putsLCD(dv);

    cposition(0, 2);
    const char *row3_param = "Loop Point: ";
    putsLCD(row3_param);
    sprintf(dv, "%8ld", waveStruct[paramPage].loop_point>>9);
    putsLCD(dv);

    cposition(0, 3);
    const char *row4_param = "Pitch from C5:  ";
    putsLCD(row4_param);
    sprintf(dv, "%4d", waveStruct[paramPage].pitch_from_C5);
    putsLCD(dv);
}

void interfaceSampleHandleUserInput(uint8_t input, uint8_t paramPage, uint16_t *pot_value) {
    input = -input - 1;
    static char dp[9] = {0};
    uint8_t x = pot_value[0] >> 8;
    uint8_t y = pot_value[1] >> 8;
    uint8_t z = pot_value[2] >> 8;
    if (x > 250) x = 250;
    if (y > 250) y = 250;
    if (z > 250) z = 250;
    uint32_t v = ((uint8_t) (x / 2.52)) * 10000 + ((uint8_t) (y / 2.52)) * 100 + ((uint8_t) (z / 2.52));

    if (v > waveStruct[paramPage].max_length-1)
        v = waveStruct[paramPage].max_length-1;
    waveStruct[paramPage].end_length = v << 9;
    cposition(12, 1);
    sprintf(dp, "%8ld", v);
    putsLCD(dp);


    x = pot_value[3] >> 8;
    y = pot_value[4] >> 8;
    z = pot_value[5] >> 8;
    if (x > 250) x = 250;
    if (y > 250) y = 250;
    if (z > 250) z = 250;
    v = ((uint8_t) (x / 2.52)) * 10000 + ((uint8_t) (y / 2.52)) * 100 + ((uint8_t) (z / 2.52));
    if (v > waveStruct[paramPage].end_length >> 9) v = waveStruct[paramPage].end_length >> 9;
    waveStruct[paramPage].loop_point= v << 9;
    cposition(12, 2);
    sprintf(dp, "%8ld", v);
    putsLCD(dp);

    auto u= (int8_t) ((pot_value[6] >> 9) - 63);
    waveStruct[paramPage].pitch_from_C5=u;
    cposition(16, 3);
    sprintf(dp, "%4d", u);
    putsLCD(dp);
    Serial.print(waveStruct[paramPage].end_length);
    Serial.print(", ");
    Serial.println(waveStruct[paramPage].loop_point);

    cposition(15, 0);
    if (input > 2){
        u = *(waveStruct[paramPage].wave + (waveStruct[paramPage].loop_point >> 9)) -127;
        //v = *(waveStruct[interface.param_page].wave + 51);
        //v = waveStruct[interface.param_page].wave[54];
        sprintf(dp, "%4d", u);
        putsLCD(dp);
    }
    if (input < 3){
        u = *(waveStruct[paramPage].wave + (waveStruct[paramPage].end_length >> 9)) -127;
        sprintf(dp, "%4d", u);
        putsLCD(dp);
    }
}

