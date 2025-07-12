/* 
This script listens to four inputs: two cameras, one button, and a signal indicating that e.g. an led is on
Whenever one of these inputs changes as indicated below, a value for each input is printed out 
(these are: current frame number for each camera, button press number for the button and a 0/1 for the LED)

Code modified from original written by Horst Obenhaus
https://github.com/kavli-ntnu/wheel_tracker/blob/master/only_rotary_teensy_prtc/only_rotary_teensy_prtc.ino
*/ 

// Define input pins
const int sync_pin_cam1 = 3; // Input pin for camera 1
const int sync_pin_cam2 = 6; // Input pin for camera 2
const int sync_pin_button = 5; // Input pin for button
const int sync_pin_led = 2; // Input pin for led

// Specify a timeout period in ms (how much time needs to pass before a new interrupt is counted?)
const int cam1_timeout = 31; //!! THIS NEEDS TO BE 1 ms LONGER THAN THE EXPOSURE TIME !!
const int cam2_timeout = 11; //!! THIS NEEDS TO BE 1 ms LONGER THAN THE EXPOSURE TIME !!
const int button_timeout = 500;
const int led_timeout = 10; 


// Define status for the LED input
volatile int status_sync_pin_led = 0;

// Save the current millisecond time for checking when the last interrupt occurred
unsigned long lastInterrupt_cam1 = millis();
unsigned long lastInterrupt_cam2 = millis();
unsigned long lastInterrupt_button = millis();
unsigned long lastInterrupt_led = millis();

// Set the counters and flags
volatile unsigned int frame_count_cam1 = 0; //count frame numbers for camera 1
volatile unsigned int frame_count_cam2 = 0; //count frame numbers for camera 2
volatile unsigned int count_button_press = 0; //count button presses
volatile int previous_button_press = -1; //set a value to compare button presses to

volatile bool flag_cam1 = 0;
volatile bool flag_cam2 = 0;
volatile bool flag_button = 0;
volatile bool flag_led = 0;


void setup() {
 // Define input and interrupt routines
 pinMode(sync_pin_cam1, INPUT_PULLUP);
 pinMode(sync_pin_cam2, INPUT_PULLUP);
 pinMode(sync_pin_button, INPUT_PULLUP);
 pinMode(sync_pin_led, INPUT);

 attachInterrupt(digitalPinToInterrupt(sync_pin_cam1), sync_detected_cam1, FALLING); // Listen to the input pin for camera 1 and execute sync_detected_cam1 if there has been a falling change
 attachInterrupt(digitalPinToInterrupt(sync_pin_cam2), sync_detected_cam2, FALLING); // As above but for camera 2
 attachInterrupt(digitalPinToInterrupt(sync_pin_button), sync_detected_button, FALLING); // As above but for the button
 attachInterrupt(digitalPinToInterrupt(sync_pin_led), sync_detected_led, RISING); // As above but for the LED

 Serial.begin(115200);
}

void loop() {
    if ((flag_cam1) || (flag_cam2) || (flag_button) || (flag_led)) //Check if one of the flags have changed (i.e. is true)
    {
      //Get the status of the LED input
      status_sync_pin_led=digitalRead(status_sync_pin_led); 
      
      //Print the frame number/button press number/status of each input 
      Serial.flush();      
      Serial.print(frame_count_cam1); 
      Serial.print(",");
      Serial.print(frame_count_cam2);
      Serial.print(",");
      
      if (count_button_press != previous_button_press) { //Only print the button press value if it has been updated
        previous_button_press++; //Update the counter for the 'previous button press' to reflect the button status has changed
        Serial.print(count_button_press);
        Serial.print(","); 
        }
        else {
          Serial.print("0"); //If the button press value has not been updated, print a zero
          Serial.print(",");
        }
      
      Serial.println(flag_led);

      //Reset the flags
      flag_cam1 = 0;
      flag_cam2 = 0;
      flag_button = 0;
      flag_led = 0;
    }
}


//Interrupt rutines
void sync_detected_cam1() //Runs when something is detected on the input pin for camera 1 
  {
    if(millis() - lastInterrupt_cam1 > cam1_timeout) //Check if the time since the last interrupt is larger than the timeout specified
    {
      frame_count_cam1++; //Increase the frame number by one
      flag_cam1=1;
      lastInterrupt_cam1 = millis(); //Update the time indicating the last interrupt
    }
  }


void sync_detected_cam2() //Runs when something is detected on the input pin for camera 2
  {
    if(millis() - lastInterrupt_cam2 > cam2_timeout) //Check if the time since the last interrupt is larger than the timeout specified
    {
      frame_count_cam2++; //Increase the frame number by one
      flag_cam2=1;
      lastInterrupt_cam2 = millis(); //Update the time indicating the last interrupt
    }
  }

void sync_detected_button() //Runs when something is detected on the input pin for the button
  {
    if(millis() - lastInterrupt_button > button_timeout) //Check if the time since the last interrupt is larger than the timeout specified
    { 
      count_button_press++; //Increase the counter by one
      flag_button=1;
      lastInterrupt_button = millis(); //Update the time of the last interrupt
    }
  }


  void sync_detected_led() //Runs when something is detected on the input pin for the LED
  {
    if(millis() - lastInterrupt_led > led_timeout) //Check if the time since the last interrupt is larger than the timeout specified
    {
      flag_led=1;
      lastInterrupt_led = millis(); //Update the time indicating the last interrupt
    }
  }
