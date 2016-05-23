import numpy as np
import wave

with open('tables_due.h', 'w') as filehandle:
    filehandle.write("/*Generated using python file \"Generate_Tables.py\"*/\n")
    filehandle.write("#ifndef TABLES_DUE_H\n#define TABLES_DUE_H\n#include <Arduino.h>\n")
    # Note Frequancies
    frequancies = [8.1757989156, 8.6619572180, 9.1770239974, 9.7227182413, 10.3008611535, 10.9133822323, 11.5623257097,
                   12.2498573744, 12.9782717994, 13.7500000000, 14.5676175474, 15.4338531643, 16.3515978313,
                   17.3239144361,
                   18.3540479948, 19.4454364826, 20.6017223071, 21.8267644646, 23.1246514195, 24.4997147489,
                   25.9565435987,
                   27.5000000000, 29.1352350949, 30.8677063285, 32.7031956626, 34.6478288721, 36.7080959897,
                   38.8908729653,
                   41.2034446141, 43.6535289291, 46.2493028390, 48.9994294977, 51.9130871975, 55.0000000000,
                   58.2704701898,
                   61.7354126570, 65.4063913251, 69.2956577442, 73.4161919794, 77.7817459305, 82.4068892282,
                   87.3070578583,
                   92.4986056779, 97.9988589954, 103.826174395, 110.0000000000, 116.540940379, 123.4708253140,
                   130.8127826503, 138.591315488, 146.8323839587, 155.563491861, 164.8137784564, 174.6141157165,
                   184.997211355, 195.9977179909, 207.652348790, 220.0000000000, 233.081880759, 246.9416506281,
                   261.6255653006, 277.182630976, 293.6647679174, 311.126983722, 329.6275569129, 349.2282314330,
                   369.994422711, 391.9954359817, 415.304697579, 440.0000000000, 466.163761518, 493.8833012561,
                   523.2511306012, 554.365261953, 587.3295358348, 622.253967444, 659.2551138257, 698.4564628660,
                   739.988845423, 783.9908719635, 830.609395159, 880.0000000000, 932.327523036, 987.7666025122,
                   1046.5022612024, 1108.7305239, 1174.6590716696, 1244.5079348, 1318.5102276515, 1396.9129257320,
                   1479.9776908465, 1567.9817439270, 1661.2187903198, 1760.0000000000, 1864.6550460724, 1975.5332050245,
                   2093.0045224048, 2217.4610478150, 2349.3181433393, 2489.0158697766, 2637.0204553030, 2793.8258514640,
                   2959.9553816931, 3135.9634878540, 3322.4375806396, 3520.0000000000, 3729.3100921447, 3951.0664100490,
                   4186.0090448096, 4434.9220956300, 4698.6362866785, 4978.0317395533, 5274.0409106059, 5587.6517029281,
                   5919.9107633862, 6271.92697571, 6644.8751612791, 7040.0000000000, 7458.6201842894, 7902.1328200980,
                   8372.0180896192, 8869.8441912599, 9397.2725733570, 9956.0634791066, 10548.081821211, 11175.303405856,
                   11839.821526772, 12543.853951416, ]

    # This is for later implementation. Instead of a table that stretches over the entire range of the synth this table
    # just represents the top octave at 1/20th semitone intervals. Each lower octave is obtain by dividing the previous
    # octave by two.
    top_octave = []
    for i in np.arange(120.0,132.0,(1.0/20.0)):
        top_octave.append((440 * np.power(2.0, (i-69)/12.0)) )

    one_octave_midi_table = []
    for i in top_octave:
        one_octave_midi_table.append(np.round(i / (34500.0 / (2 ** 17))))

    filehandle.write("\nuint16_t One_Octave_Pitches[] = {" + ', '.join(repr(x) for x in one_octave_midi_table) + '};\n')

    # This writes out the pitches spanning the range given by the midi protocal.
    midi_table = []
    for i in frequancies:
        midi_table.append(np.round(i / (34500.0 / (2 ** 17))))

    filehandle.write("\nuint16_t PITCHS[] = {" + ', '.join(repr(x) for x in midi_table) + '};\n')

    # This has no importance to the synth and exists due to legacy reasons
    filehandle.write('\nuint16_t EFTWS[]={\
0x0371,0x0340,0x0311,0x02E5,0x02BB,0x0294,0x026F,0x024C,0x022B,0x020C,0x01EE,0x01D3,0x01B8,0x01A0,0x0188,0x0172,\n\
0x015D,0x014A,0x0137,0x0126,0x0115,0x0106,0x00F7,0x00E9,0x00DC,0x00D0,0x00C4,0x00B9,0x00AE,0x00A5,0x009B,0x0093,\n\
0x008A,0x0083,0x007B,0x0074,0x006E,0x0068,0x0062,0x005C,0x0057,0x0052,0x004D,0x0049,0x0045,0x0041,0x003D,0x003A,\n\
0x0037,0x0034,0x0031,0x002E,0x002B,0x0029,0x0026,0x0024,0x0022,0x0020,0x001E,0x001D,0x001B,0x001A,0x0018,0x0017,\n\
0x0015,0x0014,0x0013,0x0012,0x0011,0x0010,0x000F,0x000E,0x000D,0x000D,0x000C,0x000B,0x000A,0x000A,0x0009,0x0009,\n\
0x0008,0x0008,0x0007,0x0007,0x0006,0x0006,0x0006,0x0005,0x0005,0x0005,0x0004,0x0004,0x0004,0x0004,0x0003,0x0003,\n\
0x0003,0x0003,0x0003,0x0002,0x0002,0x0002,0x0002,0x0002,0x0002,0x0002,0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,\n\
0x0001,0x0001,0x0001,0x0001,0x0001,0x0001,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,\n\
};\n')

    filehandle.write("\nstruct Voice {\n\
    uint32_t max_length;\n\
	uint32_t loop_point;\n\
	int8_t pitch_from_C5;\n\
    \n\
	const uint8_t *wave;\n\
    };\n")

    # ------------------------------------------------------------------------------------------------------------------
    # Files below here are used to import the wave files and structure them into a struct. Take note:
    # All waves imported must be mono unsigned 8-bit wave files.
    # Max_Length is the number of samples in your wave file bit shifted left (<<) by 9
    # Loop_point is how many samples you want to go back during a loop. This value is also bit shifted left (<<) by 9.
    # pitch_from_C5 = 0 is the distance in midi notes the wave file is from middle C (60)

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

    f = wave.open("Samples/Arr1.wav", 'r')
    x = f.readframes(-1)
    given_wave = np.fromstring(x, 'uint8')
    given_wave = [int(i) for i in given_wave]
    filehandle.write('\nconst uint8_t Arr1_wave[] = {' + ', '.join(repr(x) for x in given_wave) + '};\n')
   
    f = wave.open("Samples/swannee.wav", 'r')
    x = f.readframes(-1)
    given_wave = np.fromstring(x, 'uint8')
    given_wave = [int(i) for i in given_wave]
    print(len(given_wave))
    filehandle.write('\nconst uint8_t Swannee_wave[] = {' + ', '.join(repr(x) for x in given_wave) + '};\n')
    
    f = wave.open("Samples/strchoir.wav", 'r')
    x = f.readframes(-1)
    given_wave = np.fromstring(x, 'uint8')
    given_wave = [int(i) for i in given_wave]
    print(len(given_wave))
    filehandle.write('\nconst uint8_t strchoir_wave[] = {' + ', '.join(repr(x) for x in given_wave) + '};\n')
    
    f = wave.open("Samples/bassgt.wav", 'r')
    x = f.readframes(-1)
    given_wave = np.fromstring(x, 'uint8')
    given_wave = [int(i) for i in given_wave]
    filehandle.write('\nconst uint8_t bassgt_wave[] = {' + ', '.join(repr(x) for x in given_wave) + '};\n')
    
    f = wave.open("Samples/cello3.wav", 'r')
    x = f.readframes(-1)
    given_wave = np.fromstring(x, 'uint8')
    given_wave = [int(i) for i in given_wave]
    filehandle.write('\nconst uint8_t cello3_wave[] = {' + ', '.join(repr(x) for x in given_wave) + '};\n')

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

    # The  creation of the sctruct for 3 samples.
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

    filehandle.write('struct Voice Arr1 {\n\
	.max_length = 18366976,\n\
	.loop_point = 7298560,\n\
	.pitch_from_C5 = 3,\n\
\n\
	.wave =Arr1_wave,\n\
};\n')

    filehandle.write('struct Voice Swannee {\n\
	.max_length = 13107200,\n\
	.loop_point = 9382912,\n\
	.pitch_from_C5 = 3,\n\
\n\
	.wave =Swannee_wave,\n\
};\n')

    filehandle.write('struct Voice Strchoir {\n\
	.max_length = 18320384,\n\
	.loop_point = 8192000,\n\
	.pitch_from_C5 = 3,\n\
\n\
	.wave =strchoir_wave,\n\
};\n')

    filehandle.write('struct Voice Bassgt {\n\
	.max_length = 6404608,\n\
	.loop_point = 0,\n\
	.pitch_from_C5 = 3,\n\
\n\
	.wave =bassgt_wave,\n\
};\n')

    filehandle.write('struct Voice cello3 {\n\
	.max_length = 13703168,\n\
	.loop_point = 6851584,\n\
	.pitch_from_C5 = 3,\n\
\n\
	.wave =cello3_wave,\n\
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

# Creating the LFO Tables
with open('lfo_tables.h', 'w') as filehandle:

    filehandle.write("/*Generated using python file \"Generate_Tables.py\"*/\n")
    filehandle.write("#ifndef LFO_TABLES_H\n#define LFO_TABLES_H\n#include <Arduino.h>\n")

    square = []
    for i in range(128):
        square.append(0)

    for i in range(128):
        square.append(255)

    filehandle.write('\nconst uint8_t square_lfo[] = {' + ', '.join(repr(x) for x in square) + '};\n')

    triangle = []
    for i in range(128):
        triangle.append(2*i)

    for i in range(128):
        triangle.append(255 - 2*i)

    filehandle.write('\nconst uint8_t triangle_lfo[] = {' + ', '.join(repr(x) for x in triangle) + '};\n')

    saw = []
    for i in range(256):
        saw.append(i)

    filehandle.write('\nconst uint8_t saw_lfo[] = {' + ', '.join(repr(x) for x in saw) + '};\n')

    sine = []
    for i in range(256):
        sine.append(np.round(127 * np.sin(2*np.pi * (i/256.0)) + 127))

    filehandle.write('\nconst uint8_t sine_lfo[] = {' + ', '.join(repr(x) for x in sine) + '};\n')

    filehandle.write('#endif')
    filehandle.close()
