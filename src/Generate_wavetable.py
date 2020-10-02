import numpy as np
import wave

with open('wave_table.cpp', 'w') as filehandle:

    filehandle.write("#include \"tables_due.h\"\n#include <Arduino.h>\n")

    num = 1
    for j in range(0, 31):
        if j in [3, 5, 6, 7, 9, 12, 13, 16, 17, 21, 22, 24, 25, 29, 30, 31]:
            continue

        f = wave.open("Samples/PPG_WA{:02d}.wav".format(j), 'r')
        x = f.readframes(-1)
        given_wave = np.fromstring(x, 'uint8')
        print(len(given_wave))
        given_wave = [int(i) for i in given_wave]

        filehandle.write('\nconst uint8_t wavetable{:d}[] = {{'.format(num) + ', '.join(repr(x) for x in given_wave) + '};\n')
        num += 1

    filehandle.write("""\n\nstruct Voice waveTableStruct[MAX_WAVE_COUNT] = {
                        {"wavetbl1", 0, 16777216, 0, 5, wavetable1, //7298560
                        sizeof(wavetable1)/sizeof(*wavetable1)}, //0
                        {"wavetbl2", 0, 16777216, 0, 5, wavetable2, //7298560
                        sizeof(wavetable2)/sizeof(*wavetable2)}, //0
                        {"wavetbl3", 0, 16777216, 0, 5, wavetable3, //7298560
                        sizeof(wavetable3)/sizeof(*wavetable3)}, //0
                        {"wavetbl4", 0, 16777216, 0, 5, wavetable4, //7298560
                        sizeof(wavetable4)/sizeof(*wavetable4)}, //0
                        {"wavetbl5", 0, 16777216, 0, 5, wavetable5, //7298560
                        sizeof(wavetable5)/sizeof(*wavetable5)}, //0
                        {"wavetbl6", 0, 16777216, 0, 5, wavetable6, //7298560
                        sizeof(wavetable6)/sizeof(*wavetable6)}, //0
                        {"wavetbl7", 0, 16777216, 0, 5, wavetable7, //7298560
                        sizeof(wavetable7)/sizeof(*wavetable7)}, //0
                        {"wavetbl8", 0, 16777216, 0, 5, wavetable8, //7298560
                        sizeof(wavetable8)/sizeof(*wavetable8)}, //0
                        {"wavetbl9", 0, 16777216, 0, 5, wavetable9, //7298560
                        sizeof(wavetable9)/sizeof(*wavetable9)}, //0
                        {"wavetbl10", 0, 16777216, 0, 5, wavetable10, //7298560
                        sizeof(wavetable10)/sizeof(*wavetable10)}, //0
                        {"wavetbl11", 0, 16777216, 0, 5, wavetable11, //7298560
                        sizeof(wavetable11)/sizeof(*wavetable11)}, //0
                        {"wavetbl12", 0, 16777216, 0, 5, wavetable12, //7298560
                        sizeof(wavetable12)/sizeof(*wavetable12)}, //0
                        {"wavetbl13", 0, 16777216, 0, 5, wavetable13, //7298560
                        sizeof(wavetable13)/sizeof(*wavetable13)}, //0
                        {"wavetbl14", 0, 16777216, 0, 5, wavetable14, //7298560
                        sizeof(wavetable14)/sizeof(*wavetable14)}, //0
                        {"wavetbl15", 0, 16777216, 0, 5, wavetable15, //7298560
                        sizeof(wavetable15)/sizeof(*wavetable15)}, //0
                        {"wavetbl16", 0, 16777216, 0, 5, wavetable16, //7298560
                        sizeof(wavetable16)/sizeof(*wavetable16)}, //0
                        };""")

    filehandle.close()
