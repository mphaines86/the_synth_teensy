import numpy as np
import wave

with open('pitch_tables.h', 'w') as filehandle:
    filehandle.write("/*Generated using python file \"Generate_Tables.py\"*/\n")
    filehandle.write("#ifndef PITCH_TABLES_H\n#define PITCH_TABLES_H\n#include <Arduino.h>\n")

    top_octave = []
    for i in np.arange(120.0, 132.0, (1.0 / 20.0)):
        top_octave.append((440 * np.power(2.0, (i - 69) / 12.0)))

    one_octave_midi_table = []
    for i in top_octave:
        one_octave_midi_table.append(int(np.round(i / (32000.0 / (2 ** 17)))))

    filehandle.write(
        "\nconst uint32_t One_Octave_Pitches[] = {" + ', '.join(repr(x) for x in one_octave_midi_table) + '};\n')
    filehandle.write("#endif")

with open('tables_due.h', 'w') as filehandle:
    filehandle.write("/*Generated using python file \"Generate_Tables.py\"*/\n")
    filehandle.write("#ifndef TABLES_DUE_H\n#define TABLES_DUE_H\n#include <Arduino.h>\n\n#define MAX_WAVE_COUNT 16")

    filehandle.write("\nstruct Voice {\n\
    const uint32_t max_length;\n\
	const uint32_t loop_point;\n\
	const int8_t pitch_from_C5;\n\
    \n\
	const uint8_t *wave;\n\
    };\n\
    \n\
    extern struct Voice waveStruct[MAX_WAVE_COUNT];")

    filehandle.write("#endif")

    filehandle.close()
    # ------------------------------------------------------------------------------------------------------------------
    # Files below here are used to import the wave files and structure them into a struct. Take note:
    # All waves imported must be mono unsigned 8-bit wave files.
    # Max_Length is the number of samples in your wave file bit shifted left (<<) by 9
    # Loop_point is how many samples you want to go back during a loop. This value is also bit shifted left (<<) by 9.
    # pitch_from_C5 = 0 is the distance in midi notes the wave file is from middle C (60)
with open('tables_due.cpp', 'w') as filehandle:

    filehandle.write("#include \"tables_due.h\"\n")

    f = wave.open("Samples/EuroBell - wave1.wav", 'r')
    x = f.readframes(-1)
    given_wave = np.fromstring(x, 'uint8')
    given_wave = [int(i) for i in given_wave]
    filehandle.write('\nconst uint8_t EuroBell1_wave[] = {' + ', '.join(repr(x) for x in given_wave) + '};\n')

    f = wave.open("Samples/EuroBell - wave2.wav", 'r')
    x = f.readframes(-1)
    given_wave = np.fromstring(x, 'uint8')
    given_wave = [int(i) for i in given_wave]
    filehandle.write('\nconst uint8_t EuroBell2_wave[] = {' + ', '.join(repr(x) for x in given_wave) + '};\n')

    f = wave.open("Samples/Dark Haven.wav", 'r')
    x = f.readframes(152544)
    given_wave = np.fromstring(x, 'uint8')
    given_wave = [int(i) for i in given_wave]
    filehandle.write('\nconst uint8_t DarkHaven_wave[] = {' + ', '.join(repr(x) for x in given_wave) + '};\n')

    f = wave.open("Samples/swannee.wav", 'r')
    x = f.readframes(-1)
    given_wave = np.fromstring(x, 'uint8')
    given_wave = [int(i) for i in given_wave]
    print(len(given_wave))
    filehandle.write('\nconst uint8_t Swannee_wave[] = {' + ', '.join(repr(x) for x in given_wave) + '};\n')

    f = wave.open("Samples/bassgt.wav", 'r')
    x = f.readframes(-1)
    given_wave = np.fromstring(x, 'uint8')
    given_wave = [int(i) for i in given_wave]
    filehandle.write('\nconst uint8_t bassgt_wave[] = {' + ', '.join(repr(x) for x in given_wave) + '};\n')

    f = wave.open("Samples/guitar1.wav", 'r')
    x = f.readframes(-1)
    given_wave = np.fromstring(x, 'uint8')
    given_wave = [int(i) for i in given_wave]
    filehandle.write('\nconst uint8_t guitar1_wave[] = {' + ', '.join(repr(x) for x in given_wave) + '};\n')

    f = wave.open("Samples/bikodrum.wav", 'r')
    x = f.readframes(-1)
    given_wave = np.fromstring(x, 'uint8')
    given_wave = [int(i) for i in given_wave]
    filehandle.write('\nconst uint8_t bikodrum_wave[] = {' + ', '.join(repr(x) for x in given_wave) + '};\n')

    f = wave.open("Samples/cello3.wav", 'r')
    x = f.readframes(-1)
    given_wave = np.fromstring(x, 'uint8')
    given_wave = [int(i) for i in given_wave]
    filehandle.write('\nconst uint8_t cello3_wave[] = {' + ', '.join(repr(x) for x in given_wave) + '};\n')

    f = wave.open("Samples/strchoir.wav", 'r')
    x = f.readframes(-1)
    given_wave = np.fromstring(x, 'uint8')
    given_wave = [int(i) for i in given_wave]
    print(len(given_wave))
    filehandle.write('\nconst uint8_t strchoir_wave[] = {' + ', '.join(repr(x) for x in given_wave) + '};\n')

    f = wave.open("Samples/gtaraa.wav", 'r')
    x = f.readframes(-1)
    given_wave = np.fromstring(x, 'uint8')
    given_wave = [int(i) for i in given_wave]
    print(len(given_wave))
    filehandle.write('\nconst uint8_t gtaraa_wave[] = {' + ', '.join(repr(x) for x in given_wave) + '};\n')

    f = wave.open("Samples/strchoir.wav", 'r')
    x = f.readframes(-1)
    given_wave = np.fromstring(x, 'uint8')
    given_wave = [int(i) for i in given_wave]
    print(len(given_wave))
    filehandle.write('\nconst uint8_t gtrac_wave[] = {' + ', '.join(repr(x) for x in given_wave) + '};\n')

    # The  creation of the sctruct for samples.
    filehandle.write("""struct Voice waveStruct[MAX_WAVE_COUNT] = {
                      {18366976, 7298560, 3, Arr1_wave}, //0\n
                      {78101504, 14003200, -12, DarkHaven_wave}, //1\n
                      {13107200, 9382912, 3, Swannee_wave}, //2\n
                      {6404608, 0, 3, bassgt_wave}, //3\n
                      {5767168, 148992, -3, guitar1_wave}, //4\n
                      {8388608, 0, 3, bikodrum_wave}, //5\n
                      {262144, 262144, 5, saw_wave}, //6\n
                      {262144, 262144, 5, square_wave_50}, //7\n
                      {262144, 262144, 5, square_wave_25}, //8\n
                      {262144, 262144, 5, triangle_wave}, //9\n
                      {262144, 262144, 5, EuroBell1_wave}, //10\n
                      {262144, 262144, 5, EuroBell2_wave}, //11\n
                      {7995392, 262144, -3, gtaraa_wave},\n
                      {8388608, 0, -3, gtrac_wave},\n
                      {13703168, 6851584, 3, cello3_wave},\n
                      {18320384, 8192000, 3, strchoir_wave},\n
                      };\n""")

    filehandle.write('#endif')
    filehandle.close()
