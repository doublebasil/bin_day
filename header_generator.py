import datetime
import time
import os
import csv

# Output file name with NO extension
OUTPUT_FILENAME = os.path.join('bin_day_arduino', 'auto_generated', 'data')

# input file, led pin number
SETTINGS = (('landfill.csv', 10),
            ('recycling.csv', 9))

def main():
    cpp_filename = OUTPUT_FILENAME+".cpp"
    hpp_filename = OUTPUT_FILENAME+".hpp"
    # Create a blank hpp and cpp file and fill it with necessary stuff
    init_files(cpp_filename, hpp_filename, len(SETTINGS))
    # Read data from each of the csvs listed in SETTINGS
    for data_set in SETTINGS:
        unix_times = []
        with open(data_set[0]) as csvfile:
            reader = csv.reader(csvfile, delimiter=',')
            for date in reader:
                unix_timestamp = date_to_unix(int(date[0]), int(date[1]), int(date[2]))
                unix_times.append(unix_timestamp)
        write_array_to_cpp(cpp_filename, data_set[0], unix_times, data_set[1])
    # Definition of array of structs
    write_struct_array(cpp_filename, [i[0] for i in SETTINGS])

def init_files(cpp_filename, hpp_filename, number_of_datasets):
    with open(cpp_filename, 'w') as cpp_file: # cpp file
        cpp_file.write("#include <stdint.h>\n")
        cpp_file.write("#include \"" + os.path.split(hpp_filename)[-1] + "\"\n\n")
    with open(hpp_filename, 'w') as hpp_file: # hpp file
        hpp_file.write("#include <stdint.h>\n\n")
        hpp_file.write("#define NUMBER_OF_DATASETS ( " + str(number_of_datasets) + " )\n\n")
        hpp_file.write("typedef struct\n")
        hpp_file.write("{\n")
        hpp_file.write("\tconst uint64_t *arrayStart;\n")
        hpp_file.write("\tconst uint16_t arrayLength;\n")
        hpp_file.write("\tconst uint8_t ledPin;\n")
        hpp_file.write("} t_binDay;\n\n")
        hpp_file.write("const extern t_binDay setArray[NUMBER_OF_DATASETS];\n")

def write_array_to_cpp(cpp_filename, csv_filename, unix_timestamps, led_pin):
    # Open the cpp file ready to append
    with open(cpp_filename, 'a') as cpp_file:
        cpp_file.write("// --- Data from " + os.path.split(csv_filename)[-1] + "\n\n")
        cpp_file.write("const uint64_t " + generate_array_name(csv_filename) + "[] = {\n")
        for item in unix_timestamps:
            cpp_file.write("\t" + str(item) + ",\n")
        cpp_file.write("};\n")
        cpp_file.write("const uint16_t " + generate_array_name(csv_filename) + "Length = " + str(len(unix_timestamps)) + ";\n")
        cpp_file.write("const uint8_t " + generate_array_name(csv_filename) + "LedPin = " + str(led_pin) + ";\n\n")

def write_struct_array(cpp_filename, csv_filenames):
    with open(cpp_filename, 'a') as cpp_file:
        cpp_file.write("// Array of structs definition\n\n")
        cpp_file.write("const t_binDay setArray[NUMBER_OF_DATASETS] = {\n")
        for csv_filename in csv_filenames:
            string = "\t{"
            string += ".arrayStart = " + generate_array_name(csv_filename)
            string += ", "
            string += ".arrayLength = " + generate_array_name(csv_filename) + "Length"
            string += ", "
            string += ".ledPin = " + generate_array_name(csv_filename) + "LedPin"
            string += "},\n"
            cpp_file.write(string)
        cpp_file.write("};\n")

def date_to_unix(day, month, year, hour=0, minute=0, second=0) -> int:
    return int(time.mktime(datetime.datetime(year, month, day, hour, minute, second).timetuple()))

def generate_array_name(csv_filename) -> str:
    return os.path.splitext(os.path.split(csv_filename)[-1])[0] + "UnixArray"

if __name__ == '__main__': main()
