import numpy as np

with open("lfo_tables.h", "w") as filehandle:
    filehandle.write("/*Generated using python file \"lfo_tables.py\"*/\n")
    filehandle.write("#ifndef LFO_TABLES_H\n#define LFO_TABLES_H\n#include <Arduino.h>\n")

    wave_length = 2**9

    top_octave = []
    for i in np.arange(120.0,132.0,(1.0/20.0)):
        top_octave.append((440 * np.power(2.0, (i-69)/12.0)))

    one_octave_midi_table = []
    for i in top_octave:
        one_octave_midi_table.append(int(np.round(i / (8000.0 / (2 ** 17)))))

    filehandle.write("\nconst uint32_t One_Octave_Pitches[] = {" + ', '.join(repr(x) for x in one_octave_midi_table) + '};\n')

    square = []
    for i in range(wave_length/2):
        square.append(0)

    for i in range(wave_length/2):
        square.append(255)

    filehandle.write('\nconst uint8_t square_lfo[] = {' + ', '.join(repr(x) for x in square) + '};\n')

    triangle = []
    for i in range(wave_length/2):
        triangle.append(int(2*i * 256 / wave_length))

    for i in range(wave_length/2):
        triangle.append(int(255.0 - 2 * i * 256.0 / wave_length))

    filehandle.write('\nconst uint8_t triangle_lfo[] = {' + ', '.join(repr(x) for x in triangle) + '};\n')

    saw = []
    for i in range(wave_length):
        saw.append(int(i * 256.0 / wave_length))

    filehandle.write('\nconst uint8_t saw_lfo[] = {' + ', '.join(repr(x) for x in saw) + '};\n')

    sine = []
    for i in range(wave_length):
        sine.append(int(np.round(127 * np.sin(2*np.pi * (i/256.0 * (256.0 / wave_length))) + 127)))

    filehandle.write('\nconst uint8_t sine_lfo[] = {' + ', '.join(repr(x) for x in sine) + '};\n')

    filehandle.write('#endif')
    filehandle.close()
