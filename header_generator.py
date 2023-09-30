import datetime
import time
import os
import csv

# input_file, output_file, array_name
files = (('landfill_dates.csv',  os.path.join('bin_day', 'landfill_unix.hpp'),  'landfill_unix' ),
         ('recycling_dates.csv', os.path.join('bin_day', 'recycling_unix.hpp'), 'recycling_unix'))

def main():
    for file in files:
        create_header(file[0], file[1], file[2])

def date_to_unix(day, month, year):
    return int(time.mktime(datetime.datetime(year, month, day, 0, 0, 0).timetuple()))

def create_header(csv_file, hpp_file, array_name):
    # Read the data
    data = []
    with open(csv_file) as csvfile:
        reader = csv.reader(csvfile, delimiter=',')
        for row in reader:
            data.append(row)
    # And write some data
    with open(hpp_file, 'w') as header_file:
        header_file.write("#include <stdint>\n\n")
        header_file.write("uint64_t " + array_name + "[] = {\n")
        for date in data:
            unix = date_to_unix(int(date[0]), int(date[1]), int(date[2]))
            header_file.write("\t" + str(unix) + ",\n")
        header_file.write("};\n")



if __name__ == '__main__': main()
