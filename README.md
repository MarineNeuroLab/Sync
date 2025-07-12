# Sync
Synchronise two cameras (here these are Basler cameras), an LED and a button. These four units are connected to one microcontroller that sends the signals it receives from each unit to a computer where they are saved in a .csv file. 

## How it works
Every time an event occurs (a frame is captured, the LED is turned on or the push button is pressed), the date, time, frame number for both cameras, the button press count, and LED status are continuously saved to a .csv file until a specified key on your keyboard is pressed.

## Requirements
You need to have the python packages [serial](https://pythonhosted.org/pyserial/) and [keyboard](https://github.com/boppreh/keyboard) installed in your environment.

## How to use
1. Set up your circuit as indicated in the **sync_sketch.png** file.
2. Configure the microcontroller according to the manufacturer's instructions if you have not already done so.
1. Configure both cameras in pylon Viewer:
   - Set the exposure, frame rate etc.
   - In "Digital I/O control", set *Line Selector* to *Line 2*, *Line Mode* to *Output*, and *Line Source* to *Exposure Active*. This results in a signal being sent from the Basler cameras every time a frame is acquired.
2. Update the **sync.ino** sketch to match your camera settings (especially the timeouts that are based on the exposure time for each camera) and upload it to the microcontroller that will receive the signals from all units.
3. Update the **serial_reader.py** file with the correct port (the one the microcontroller receiving signals is connected to the computer via) and folder location for where the output will be saved.
4. Run **serial_reader.py** in your terminal. The computer is now listening to signals coming in from the microcontroller through the specified port.
5. Start camera aquisition/LED stimuli/press the push button. The date, time, current frame number for both cameras, the button press count (if the button has been pressed), and LED status will be printed out to your terminal as well as saved to the .csv file.
6. To stop saving values, press the key specified in the **serial_reader.py** file (the default is the 's' key). Once the key press has been registered and the file saved, a message indicating this will be printed to the terminal (this might take a second or two).

## Note
- The LED is only detected when it turns on (i.e. the file will show a '1' to indicate it is on only when it turns on, not for the duration it is on).
- To reset camera frame counts and button press counts, re-upload the **sync.ino** file to the microcontroller.
- Only press the push button briefly (don't hold it in) if it is important to have an accurate count. If you hold it in it will count this as multiple button presses.
 
