import numpy as np
import wave

'''with open('pitch_tables.h', 'w') as filehandle:
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
    const uint32_t end_length;\n\
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
    # pitch_from_C5 = 0 is the distance in midi notes the wave file is from middle C (60)'''
with open('tables_due.cpp', 'w') as filehandle:

    filehandle.write("#include \"tables_due.h\"\n#include <Arduino.h>\n")

    f = wave.open("Samples/Arr1.wav", 'r')
    x = f.readframes(-1)
    given_wave = np.fromstring(x, 'uint8')
    print(len(given_wave))
    given_wave = [int(i) for i in given_wave]
    filehandle.write('\nconst uint8_t Arr1_wave[] = {' + ', '.join(repr(x) for x in given_wave) + '};\n')

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
    print(f.getframerate())
    x = f.readframes(-1)
    given_wave = np.fromstring(x, 'uint8')
    given_wave = [int(i) for i in given_wave]
    print(len(given_wave))
    filehandle.write('\nconst uint8_t gtaraa_wave[] = {' + ', '.join(repr(x) for x in given_wave) + '};\n')

    f = wave.open("Samples/fhorn1.wav", 'r')
    print(f.getframerate())
    x = f.readframes(-1)
    given_wave = np.fromstring(x, 'uint8')
    given_wave = [int(i) for i in given_wave]
    print(len(given_wave))
    filehandle.write('\nconst uint8_t fhorn1_wave[] = {' + ', '.join(repr(x) for x in given_wave) + '};\n')

    filehandle.write('''\nconst uint8_t saw_wave[] = {0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10, 10, 11, 11, 12, 12, 13, 13, 14, 14, 15, 15, 16, 16, 17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 23, 24, 24, 25, 25, 26, 26, 27, 27, 28, 28, 29, 29, 30, 30, 31, 31, 32, 32, 33, 33, 34, 34, 35, 35, 36, 36, 37, 37, 38, 38, 39, 39, 40, 40, 41, 41, 42, 42, 43, 43, 44, 44, 45, 45, 46, 46, 47, 47, 48, 48, 49, 49, 50, 50, 51, 51, 52, 52, 53, 53, 54, 54, 55, 55, 56, 56, 57, 57, 58, 58, 59, 59, 60, 60, 61, 61, 62, 62, 63, 63, 64, 64, 65, 65, 66, 66, 67, 67, 68, 68, 69, 69, 70, 70, 71, 71, 72, 72, 73, 73, 74, 74, 75, 75, 76, 76, 77, 77, 78, 78, 79, 79, 80, 80, 81, 81, 82, 82, 83, 83, 84, 84, 85, 85, 86, 86, 87, 87, 88, 88, 89, 89, 90, 90, 91, 91, 92, 92, 93, 93, 94, 94, 95, 95, 96, 96, 97, 97, 98, 98, 99, 99, 100, 100, 101, 101, 102, 102, 103, 103, 104, 104, 105, 105, 106, 106, 107, 107, 108, 108, 109, 109, 110, 110, 111, 111, 112, 112, 113, 113, 114, 114, 115, 115, 116, 116, 117, 117, 118, 118, 119, 119, 120, 120, 121, 121, 122, 122, 123, 123, 124, 124, 125, 125, 126, 126, 127, 127, 128, 128, 129, 129, 130, 130, 131, 131, 132, 132, 133, 133, 134, 134, 135, 135, 136, 136, 137, 137, 138, 138, 139, 139, 140, 140, 141, 141, 142, 142, 143, 143, 144, 144, 145, 145, 146, 146, 147, 147, 148, 148, 149, 149, 150, 150, 151, 151, 152, 152, 153, 153, 154, 154, 155, 155, 156, 156, 157, 157, 158, 158, 159, 159, 160, 160, 161, 161, 162, 162, 163, 163, 164, 164, 165, 165, 166, 166, 167, 167, 168, 168, 169, 169, 170, 170, 171, 171, 172, 172, 173, 173, 174, 174, 175, 175, 176, 176, 177, 177, 178, 178, 179, 179, 180, 180, 181, 181, 182, 182, 183, 183, 184, 184, 185, 185, 186, 186, 187, 187, 188, 188, 189, 189, 190, 190, 191, 191, 192, 192, 193, 193, 194, 194, 195, 195, 196, 196, 197, 197, 198, 198, 199, 199, 200, 200, 201, 201, 202, 202, 203, 203, 204, 204, 205, 205, 206, 206, 207, 207, 208, 208, 209, 209, 210, 210, 211, 211, 212, 212, 213, 213, 214, 214, 215, 215, 216, 216, 217, 217, 218, 218, 219, 219, 220, 220, 221, 221, 222, 222, 223, 223, 224, 224, 225, 225, 226, 226, 227, 227, 228, 228, 229, 229, 230, 230, 231, 231, 232, 232, 233, 233, 234, 234, 235, 235, 236, 236, 237, 237, 238, 238, 239, 239, 240, 240, 241, 241, 242, 242, 243, 243, 244, 244, 245, 245, 246, 246, 247, 247, 248, 248, 249, 249, 250, 250, 251, 251, 252, 252, 253, 253, 254, 254, 255, 255};

const uint8_t square_wave_50[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255};

const uint8_t square_wave_25[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255};

const uint8_t triangle_wave[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255, 255, 254, 253, 252, 251, 250, 249, 248, 247, 246, 245, 244, 243, 242, 241, 240, 239, 238, 237, 236, 235, 234, 233, 232, 231, 230, 229, 228, 227, 226, 225, 224, 223, 222, 221, 220, 219, 218, 217, 216, 215, 214, 213, 212, 211, 210, 209, 208, 207, 206, 205, 204, 203, 202, 201, 200, 199, 198, 197, 196, 195, 194, 193, 192, 191, 190, 189, 188, 187, 186, 185, 184, 183, 182, 181, 180, 179, 178, 177, 176, 175, 174, 173, 172, 171, 170, 169, 168, 167, 166, 165, 164, 163, 162, 161, 160, 159, 158, 157, 156, 155, 154, 153, 152, 151, 150, 149, 148, 147, 146, 145, 144, 143, 142, 141, 140, 139, 138, 137, 136, 135, 134, 133, 132, 131, 130, 129, 128, 127, 126, 125, 124, 123, 122, 121, 120, 119, 118, 117, 116, 115, 114, 113, 112, 111, 110, 109, 108, 107, 106, 105, 104, 103, 102, 101, 100, 99, 98, 97, 96, 95, 94, 93, 92, 91, 90, 89, 88, 87, 86, 85, 84, 83, 82, 81, 80, 79, 78, 77, 76, 75, 74, 73, 72, 71, 70, 69, 68, 67, 66, 65, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0};''')
    # The  creation of the sctruct for samples.
    filehandle.write("""\n\nstruct Voice waveStruct[MAX_WAVE_COUNT] = {
                        {"Arr1    ", 18366976, 11068416, 3, Arr1_wave, //7298560
                        sizeof(Arr1_wave)/sizeof(*Arr1_wave)}, //0
                        {"Dark Hvn", 78101504, 64098304, -12, DarkHaven_wave, //14003200
                        sizeof(DarkHaven_wave)/sizeof(*DarkHaven_wave)}, //1
                        {"Swannee ", 13107200, 3724288, 3, Swannee_wave, //9382912
                        sizeof(Swannee_wave)/sizeof(*Swannee_wave)}, //2
                        {"Bassgt  ", 6404608, 6404608, 3, bassgt_wave, //0
                        sizeof(bassgt_wave)/sizeof(*bassgt_wave)}, //3
                        {"Guitar1 ", 5767168, 5618176, -3, guitar1_wave, //148992
                        sizeof(guitar1_wave)/sizeof(*guitar1_wave)}, //4
                        {"Bikodrum", 8388608, 8388608, 3, bikodrum_wave, // 0
                        sizeof(bikodrum_wave)/sizeof(*bikodrum_wave)}, //5
                        {"Saw     ", 262144, 0, 5, saw_wave, //262144
                        sizeof(saw_wave)/sizeof(*saw_wave)}, //6
                        {"Square50", 262144, 0, 5, square_wave_50, //262144
                        sizeof(square_wave_50)/sizeof(*square_wave_50)}, //7
                        {"Square25", 262144, 0, 5, square_wave_25,//262144
                        sizeof(square_wave_25)/sizeof(*square_wave_25)}, //8
                        {"Triangle", 262144, 0, 5, triangle_wave, //262144
                        sizeof(triangle_wave)/sizeof(*triangle_wave)}, //9
                        {"EurBell1", 262144, 0, 5, EuroBell1_wave, //262144
                        sizeof(EuroBell1_wave)/sizeof(*EuroBell1_wave)}, //10
                        {"EurBell2", 262144, 0, 5, EuroBell2_wave, //262144
                        sizeof(EuroBell2_wave)/sizeof(*EuroBell2_wave)}, //11
                        {"Gtaraa  ", 7995392, 799532, 3, gtaraa_wave, //262144
                        sizeof(gtaraa_wave)/sizeof(*gtaraa_wave)}, //12
                        {"fhorn1   ", 3211264, 4194304, 3, fhorn1_wave, // 0
                        sizeof(fhorn1_wave)/sizeof(*fhorn1_wave)}, //13
                        {"Cello3  ", 13703168, 6851584, 3, cello3_wave, //6851584
                        sizeof(cello3_wave)/sizeof(*cello3_wave)}, //14
                        {"StrChoir", 18320384, 10128384, 3, strchoir_wave, //8192000
                        sizeof(strchoir_wave)/sizeof(*strchoir_wave)}, //15
                        };""")

    filehandle.close()
