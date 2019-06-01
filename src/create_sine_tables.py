import math

sine_lengths = [2048, 1024, 512, 256, 128]
with open("sine_tables.c", "w") as filehandle:
    filehandle.write("/*Generated using python file \"sine_tables.py\"*/\n")
    filehandle.write("#include \"sine_tables.h\"\n\n")

    for i in sine_lengths:
         sine_table = []
         for j in range(i):
             sine_table.append(int(65535.0 * math.sin((2.0 / i) * j * math.pi)))
	 filehandle.write('const int16_t sine_table_' + str(i) + '_wave[] = {' + ', '.join(repr(x) for x in sine_table) + '}\n')
    filehandle.close()

with open("sine_tables.h", "w") as filehandle:
    filehandle.write("/*Generated using python file \"sine_tables.py\"*/\n")
    filehandle.write("#ifndef SINE_TABLES_H\n#define SINE_TABLES_H\n#include <Arduino.h>\n\n")
    for i in sine_lengths:
        filehandle.write("extern const int16_t sine_table_" + str(i) + "_wave[];\n")
    filehandle.close()
    
