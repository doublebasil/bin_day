# bin_day
Code to make my Arduino Nano 33 IoT tell me when it's bin day.

Each csv file contains the dates (dd/mm/yyyy) of bin days; one csv for recycling/glass and one for landfill. The header_generator.py file converts the dates in each csv into unix time (time since the beginning of 1970), and puts those unix times into c arrays in a hpp and cpp file. 

In terms of hardware, the board is connected to a button, a red LED and a green LED. Every so many hours, the board connects to WiFi and downloads the time. It uses the millis() function to calculate the current time based on the time since downloading unix time. Each LED represents a type of bin day. A LED should blink slowly if bin day is tomorrow, and blink quickly if bin day is this morning. The button can be pressed to stop the LED blinking, if you've put the bins out.

This is dumb and badly coded, it's mostly just for fun.
