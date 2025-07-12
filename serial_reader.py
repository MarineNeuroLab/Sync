"""
Read signals coming in through a serial port:
- the current frame number from two cameras 
- the current button press number of a button
- the status of an LED (is it on or off)

Inspired by https://makersportal.com/blog/2018/2/25/python-datalogger-reading-the-serial-output-from-arduino-to-analyze-data-using-pyserial
and
https://github.com/kavli-ntnu/wheel_tracker/blob/master/save_tracking.py
"""

import serial
import keyboard
from datetime import datetime

ser = serial.Serial('COM5') #The port to read from (i.e. the USB port the Arduino is connected to)

ser.flushInput() #This clears the serial buffer so everything is ready to go

# Specify which folder to save the data in
root_folder = 'C:/DATA/Sync_data' 

# Get the current time
now = datetime.now().strftime('%Y-%m-%d_%H-%M-%S')

# Create a cvs file with a name based on the current time in the root folder specified 
output_file='{}/{}_sync.csv'.format(root_folder, now)

# While this variable is True, values will be recorded
record = True

#If the key under keyboard.add_hotkey is pressed, this function is invoked and record is set to False
def stop_recording(): 
    global record #Make this variable global
    record=False
keyboard.add_hotkey('s', stop_recording) #Specify which key to press to stop recording values (default = s)

# Main code:
f = open(output_file,"a") #Open the csv file
f.write('Date, Time, Frame camera 1, Frame camera 2, Button count, LED\n') #Add headers to the file

while record: #While record = True, run the following code to record and save the values in output_file
    
    ser_bytes = ser.readline() #Read one line from the port

    decoded_bytes = ser_bytes.decode('utf-8') #Convert the read data so it's legible
    decoded_bytes_split = decoded_bytes.strip().split(',') #Strip away the prefix and suffix characters, and split the values using the comma as the separator
    
    # Extract the different values and convert them to integers
    frame_count_cam1 =  int(decoded_bytes_split[0])
    frame_count_cam2 =  int(decoded_bytes_split[1])
    count_button_press =int(decoded_bytes_split[2])
    sync_pin_led =      int(decoded_bytes_split[3])


    # Get the current date and time
    now = datetime.now().strftime('%Y-%m-%d_%H:%M:%S.%f')                                       
    
    # Print out the values in the terminal
    print('{} | Frame camera 1: {} | Frame camera 2: {} | Button count: {} | LED: {}'.format(
                                                now[0:-7],  
                                                frame_count_cam1,
                                                frame_count_cam2,
                                                count_button_press,
                                                sync_pin_led))

    # Save the values in the csv file
    f.write('{},{},{},{},{},{}\n'.format(now[0:-16],
                                        now[11:],
                                        frame_count_cam1,
                                        frame_count_cam2,
                                        count_button_press,
                                        sync_pin_led))

f.close() #Close the file when 'record' is no longer True
print('Finished saving values')