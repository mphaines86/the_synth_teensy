import numpy as np
import wave

with open('tables_due.h', 'w') as filehandle:
    filehandle.write("/*Generated using python file \"Generate_Tables.py\"*/\n")
    filehandle.write("#ifndef TABLES_DUE_H\n#define TABLES_DUE_H\n#include <Arduino.h>\n")


    filehandle.write("\nstruct Voice {\n\
    uint32_t max_length;\n\
	uint32_t loop_point;\n\
	int8_t pitch_from_C5;\n\
    \n\
	const uint8_t *wave;\n\
    };\n")

    filehandle.write("#endif")

    filehandle.close()
    # ------------------------------------------------------------------------------------------------------------------
    # Files below here are used to import the wave files and structure them into a struct. Take note:
    # All waves imported must be mono unsigned 8-bit wave files.
    # Max_Length is the number of samples in your wave file bit shifted left (<<) by 9
    # Loop_point is how many samples you want to go back during a loop. This value is also bit shifted left (<<) by 9.
    # pitch_from_C5 = 0 is the distance in midi notes the wave file is from middle C (60)
with open('tables_due.c', 'w') as filehandle:

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
    
    '''f = wave.open("Piano_C3.wav", 'r')
    x = f.readframes(-1)
    given_wave = np.fromstring(x, 'uint8')
    given_wave = [int(i) for i in given_wave]
    filehandle.write('\nconst uint8_t piano_C3_wave[] = {' + ', '.join(repr(x) for x in given_wave) + '};\n')'''

    # The  creation of the sctruct for samples.
    filehandle.write('\nstruct Voice EuroBell1 {\n\
	.max_length = 262144,\n\
	.loop_point = 262144,\n\
	.pitch_from_C5 = 0,\n\
\n\
	.wave =EuroBell1_wave,\n\
};\n')

    filehandle.write('struct Voice EuroBell2 {\n\
	.max_length = 262144,\n\
	.loop_point = 262144,\n\
	.pitch_from_C5 = 0,\n\
\n\
	.wave =EuroBell2_wave,\n\
};\n')

    filehandle.write('struct Voice DarkHaven {\n\
	.max_length = 78101504,\n\
	.loop_point = 14003200,\n\
	.pitch_from_C5 = -12,\n\
\n\
	.wave =DarkHorse_wave,\n\
};\n')

    filehandle.write('struct Voice Swannee {\n\
	.max_length = 13107200,\n\
	.loop_point = 9382912,\n\
	.pitch_from_C5 = 3,\n\
\n\
	.wave =Swannee_wave,\n\
};\n')

    filehandle.write('struct Voice Bassgt {\n\
	.max_length = 6404608,\n\
	.loop_point = 0,\n\
	.pitch_from_C5 = 3,\n\
\n\
	.wave =bassgt_wave,\n\
};\n')


    filehandle.write('struct Voice guitar1 {\n\
	.max_length = 5767168,\n\
	.loop_point = 148992,\n\
	.pitch_from_C5 = 3,\n\
\n\
	.wave =guitar1_wave,\n\
};\n')
    
    filehandle.write('struct Voice bikodrum {\n\
	.max_length = 8388608,\n\
	.loop_point = 0,\n\
	.pitch_from_C5 = 3,\n\
\n\
	.wave =bikodrum_wave,\n\
};\n')

    '''filehandle.write('struct Voice piano_C3 {\n\
	.max_length = 44121600,\n\
	.loop_point = 3510784,\n\
	.pitch_from_C5 = -24,\n\
\n\
	.wave =piano_C3_wave,\n\
};\n')'''

    filehandle.write('#endif')
    filehandle.close()

