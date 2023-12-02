#include <EEPROM.h>

// PIN values for the RGB LED
const int LED_red_pin = 9;
const int LED_green_pin = 10;
const int LED_blue_pin = 11;
// last value for each color channel
float LED_red_pin_value;
float LED_green_pin_value;
float LED_blue_pin_value;
// user input values for the RGB led
float LED_red_user_input;
float LED_green_user_input; 
float LED_blue_user_input; 

const int LED_channel_active = 255;   // channel color intensity when active
const int LED_channel_inactive = 0;   // channel color intensity when inactive
bool LED_toggle_mode_state = LOW;

// default value for sensor settings (threshold, sampling)
const int sensor_value_default = -1;
// PIN value for the photocell sensor
const int photocell_pin = 0;                    // connected to A0
int photocell_value;
int photocell_threshold = sensor_value_default; // maximum value allowed for the photocell
bool is_photocell_threshold_respected;          // HIGH if the environment respects sensor's threshold

// PIN values for HC-SR04 ultrasonic sensor
const int trig_pin = 5;                          // trig pin 
const int echo_pin = 6;                          // echo pin 
long ultrasonic_duration;                        // duration of the signal travel
int ultrasonic_distance;                         // actual distance registered by the ultrasonic
int ultrasonic_threshold = sensor_value_default; // maximum distance allowed for the ultrasonic 
const float ultrasonic_speed_factor = 0.034 / 2;
bool is_ultrasonic_threshold_respected; // HIGH if the environment respects sensor's threshold
// samping rate per seconds
int sampling_rate = sensor_value_default;

// number of sesnors for the system
const int number_of_sensors = 2;
// maximum number of logs for each sensor
int maximum_logs = 10;
unsigned long int last_long_displayed = 0;
// where data is stored from EEPROM reads
int photocell_counter_value, ultrasonic_counter_value;
/* because the logs of the photocell will contain
integers, which are stored on 2 bytes, 
from byte 0-19, photocell logs */
const int eeprom_photocell_memory_space = sizeof(int) * maximum_logs;
int eeprom_photocell_address_counter = 0;
int eeprom_photocell_address;           // actual address of the photocell logs
// from byte 20-39, ultrasonic logs
int eeprom_ultrasonic_address_counter = 0;
int eeprom_ultrasonic_address;          // actual address of the ultrasonic logs

// variable for user input reads
int user_input_int;

// current menu that is being displayed
unsigned int current_menu = 0;

void setup() {
  pinMode(LED_red_pin, OUTPUT);
  pinMode(LED_green_pin, OUTPUT);
  pinMode(LED_blue_pin, OUTPUT);

  pinMode(trig_pin, OUTPUT);
  pinMode(echo_pin, INPUT);

  Serial.begin(9600);
  // display the initial menu to the user
  choose_menu_to_display();
}

void loop() {
  if (Serial.available() > 0){
    handle_user_input();  
    choose_menu_to_display();
  }

  display_current_sensor_readings();
  read_ultrasonic_sensor();
  read_photocell_sensor();
  write_to_RGB_LED();
}

/*
  Depending on the input given by the user
  and the current menu where the input was written,
  forward the task of handling the input.
*/
void handle_user_input(){
  switch (current_menu) {
    case 0:
      handle_user_input_on_main_menu();
      break;
    case 1:
      handle_user_input_on_sensor_menu();
      break;
    case 11:
      read_sampling_interval();
      break;
    case 12:
      read_ultrasonic_threshold();
      break;
    case 13:
      read_photocell_threshold();
      break;
    case 2:
      handle_user_input_on_logger_menu();
      break;
    case 3:
      handle_user_input_on_system_menu();
      break;
    case 31:
      handle_user_on_input_sensor_readings();
      break;
    case 4:
      handle_user_input_on_RGB_LED_menu();
      break;
    case 41:
      read_RGB_LED_input();
      break;
    default:
      break;
  }
}

void handle_user_input_on_main_menu(){
  user_input_int = Serial.parseInt();

  if (user_input_int < 1 || user_input_int > 4) {
    /* user input is not valid, print an error
    message and exit the function */
    Serial.println("Invalid input! Number in interval [1-4] is expected.");
    return;
  }

  switch (user_input_int) {
    /* switch the current menu to the menu
    chosen by the user */
    case 1:
      current_menu = 1;
      break;
    case 2:
      current_menu = 2;
      break;
    case 3:
      current_menu = 3;
      break;
    case 4:
      current_menu = 4;
      break;
    default:
      break;
  }
}

void handle_user_input_on_sensor_menu(){
  user_input_int = Serial.parseInt();

  if (user_input_int < 1 || user_input_int > 4) {
    /* user input is not valid, print an error
    message and exit the function */
    Serial.println("Invalid input! Number in interval [1-4] is expected.");
    return;
  }

  switch (user_input_int) {
    case 1:
      // switch to sampling rate setting
      current_menu = 11;
      break;
    case 2:
      // switch to ultra sound threshold setting
      current_menu = 12;
      break;
    case 3:
      // switch to photocell threshold setting
      current_menu = 13;
      break;
    case 4:
      // switch back to the main menu
      current_menu = 0;
      break;
    default:
      break;
  }
}

void read_sampling_interval(){
  user_input_int = Serial.parseInt();

  // check if the value introduced by the
  // use is a valid sampling interval of the sensors' values
  if (user_input_int < 1 || user_input_int > 10) {
    Serial.println("Invalid samping interval! Please follow the instructions.");
    return;
  }

  sampling_rate = user_input_int;
  current_menu = 1;
}

void read_photocell_threshold(){
  user_input_int = Serial.parseInt();
  
  // check if the value introduced by the
  // use is a valid threshold for the photocell
  if (user_input_int < 0) {
    Serial.println("Invalid LDR threshold! Please follow the instructions.");
    return;
  }

  photocell_threshold = user_input_int; 
  current_menu = 1;
}

void read_ultrasonic_threshold(){
  user_input_int = Serial.parseInt();
  
  // check if the value introduced by the
  // use is a valid threshold for the ultrasonic sensor
  if (user_input_int < 0 || user_input_int > 400) {
    Serial.println("Invalid Ultrasonic threshold! Please follow the instructions.");
    return;
  }

  ultrasonic_threshold = user_input_int;
  current_menu = 1;
}

void handle_user_input_on_logger_menu(){
  user_input_int = Serial.parseInt();

  if (user_input_int != 0 && user_input_int != 1) {
    /* user input is not valid, print an error
    message and exit the function */
    Serial.println("Invalid input! 0 or 1 are expected.");
    return;
  }

  if (user_input_int == 0) {
    /* reset the EEPROM memory; because the value of the sensors
    will be stored as integers, the jumps in memory will be of the size
    of an integer (2 bytes)*/
    for (int i = 0; i < sizeof(int) * number_of_sensors * maximum_logs; i = i + sizeof(int)){
      EEPROM.put(i, 0);
    }
  } 

  current_menu = 1;
}

void handle_user_input_on_system_menu(){
  user_input_int = Serial.parseInt();

  if (user_input_int < 1 || user_input_int > 4) {
    /* user input is not valid, print an error
    message and exit the function */
    Serial.println("Invalid input! Number in interval [1-4] is expected.");
    return;
  } 

  switch (user_input_int) {
    case 1:
      // switch to displaying current data from sensors
      current_menu = 31;
      break;
    case 2:
      display_current_sensor_settings();
      break;
    case 3:
      display_logged_data();
      break;
    case 4:
      // switch back to the main menu
      current_menu = 0;
      break;
    default:
      break;
  }
}

/* 
  Reads the user input, and if the exit code
  was entered, switch the menu back to the `System Status` menu.
*/
void handle_user_on_input_sensor_readings(){
  user_input_int = Serial.parseInt();

  if (user_input_int == 1) {
    // exit if the user chose to exit 
    current_menu = 3;
    return;
  }
}

/* 
  Displays sensor values 
  if the current menu corresponds.
*/
void display_current_sensor_readings(){
  if (current_menu != 31){
    // exit if the chosen menu is not for sensor reading
    return; 
  }

  /* sampling interval it's in seconds, so
  it must be transformed in milliseconds */
  if ((millis() - last_long_displayed) > sampling_rate * 1000){
    Serial.println("PRESS 1 TO EXIT!");
    Serial.print("Photocell: ");
    Serial.println(photocell_value);
    Serial.print("Ultrasonic: ");
    Serial.println(ultrasonic_distance);
    Serial.println();

    last_long_displayed = millis();
  }
}

void display_current_sensor_settings(){
  Serial.print("Sampling rate: ");
  if (sampling_rate != sensor_value_default) {
    Serial.print("1 log/");
    Serial.print(sampling_rate);
    Serial.println(" sec");
  } else {
    Serial.println("unset");
  }

  Serial.print("Ultrasonic threshold: ");
  if (ultrasonic_threshold != sensor_value_default) {
    Serial.println(ultrasonic_threshold);
  } else {
    Serial.println("unset");
  }
  
  Serial.print("LDR sensor threshold: ");
  if (photocell_threshold != sensor_value_default) {
    Serial.println(photocell_threshold);
  } else {
    Serial.println("unset");
  }

  Serial.println();
}

void display_logged_data(){
  for(int i = 0; i < maximum_logs; i++){
    // calculate the index address for each logs for the photocell
    int photocell_index_address = i * sizeof(int);
    /* add the maximum number of logs for a photocell to
    get the address for the ultrasonic sensor */
    int ultrasonic_index_address = photocell_index_address + sizeof(int) * maximum_logs;

    EEPROM.get(photocell_index_address, photocell_counter_value);
    EEPROM.get(ultrasonic_index_address, ultrasonic_counter_value);

    Serial.print(i + 1);
    Serial.println("th iteration:");
    Serial.print("Photocell: ");
    Serial.println(photocell_counter_value);
    Serial.print("Ultrasonic: ");
    Serial.println(ultrasonic_counter_value);
    Serial.println();
  }
}

void handle_user_input_on_RGB_LED_menu(){
  user_input_int = Serial.parseInt();

  if (user_input_int < 1 || user_input_int > 3) {
    /* user input is not valid, print an error
    message and exit the function */
    Serial.println("Invalid input! Number in interval [1-3] is expected.");
    return;
  }

  switch (user_input_int) {
    case 1:
      // switch to manual color control settting
      current_menu = 41;
      break;
    case 2:
      // toggle the automatic mode
      toggle_automatic();
      break;
    case 3:
      // switch back to the main menu
      current_menu = 0;
      break;
    default:
      break;
  }
}

/*
  Reads RGB input from user that is expected
  to be of the format <int><sep1><int><sep2><int> where:
  - <num>: a integer in the interval [0, 255]
  - <sep1>, <sep2>: any separator, any symbol

  Updates the values of the RGB only if all
  values read are valid. 
  Switches back to the RGB LED Control menu on success. 
*/
void read_RGB_LED_input(){
  LED_red_user_input = Serial.parseFloat();
  Serial.read();
  LED_green_user_input = Serial.parseFloat();
  Serial.read();
  LED_blue_user_input = Serial.parseFloat();

  if (LED_red_user_input < 0 || LED_red_user_input > 255) {
    Serial.println("Invalid RGB values! Please follow the instructions.");
    return;
  }
  if (LED_green_user_input < 0 || LED_green_user_input > 255) {
    Serial.println("Invalid RGB values! Please follow the instructions.");
    return;
  }
  if (LED_blue_user_input < 0 || LED_blue_user_input > 255) {
    Serial.println("Invalid RGB values! Please follow the instructions.");
    return;
  }

  // switch back to RGB LED Control
  current_menu = 4;
  // update RGB values
  LED_red_pin_value = LED_red_user_input;
  LED_green_pin_value = LED_green_user_input;
  LED_blue_pin_value = LED_blue_user_input;
}

/*
  Changes the toggle mode from LOW -> HIGH or
  from HIGH -> LOW and displays a message to inform
  the user about the new configuration.
*/
void toggle_automatic(){
  LED_toggle_mode_state = !LED_toggle_mode_state;

  if (LED_toggle_mode_state == HIGH){
    Serial.println("LED Toggle Mode set to ON!");
  } else {
    Serial.println("LED Toggle Mode set to OFF!");
  }
}

/*
  Automatic mode ON - check if the sensors
  thresholds are respected:
  If they are, make the RGB LED green;
  Otherwise, make it red.

  Automatic mode OFF - write the last 
  configuration of the RGB to the corresponding pins.
*/
void write_to_RGB_LED(){
  if (LED_toggle_mode_state == LOW) {
      analogWrite(LED_red_pin, LED_red_pin_value);
      analogWrite(LED_green_pin, LED_green_pin_value);
      analogWrite(LED_blue_pin, LED_blue_pin_value);
  } else {
    /* check if the threshold for the 
    sensors is respected*/
    if (is_photocell_threshold_respected == false || is_ultrasonic_threshold_respected == false){
      /* make the button red if at least one sensor
      is not respecting the threshold */
      analogWrite(LED_red_pin, LED_channel_active);
      analogWrite(LED_green_pin, LED_channel_inactive);
    } else {
      // otherwise, make it green
      analogWrite(LED_red_pin, LED_channel_inactive);
      analogWrite(LED_green_pin, LED_channel_active);
    }

    // blue channel will alwasy be inactive in this configuration
    analogWrite(LED_blue_pin, LED_channel_inactive);
  }
}

/*
  Reads the value of the photocell sensor
  and store it as a log in the EEPROM. 

  Previously written logs will be overridden if
  the maximum number of logs is passed.
*/
void read_photocell_sensor(){
  photocell_value = analogRead(photocell_pin);

  if (photocell_value < photocell_threshold) {
    is_photocell_threshold_respected = true;
  } else {
    is_photocell_threshold_respected = false;
  }

  /* write the value as a log at % 2 * maximum_logs,
  because if the number of logs will exceed the maximum logs,
  previous logs will be overwritten */ 
  eeprom_photocell_address = eeprom_photocell_address_counter % (2 * maximum_logs);
  EEPROM.put(eeprom_photocell_address, photocell_value);
  // go to the next address space for an integer
  eeprom_photocell_address_counter += 2;
}

/*
  Reads the value of the ultrasonic sensor 
  and store it as a log in the EEPROM. 
  
  Previously written logs will be overridden if
  the maximum number of logs is passed.
*/
void read_ultrasonic_sensor(){
  // clear the trig pin
  digitalWrite(trig_pin, LOW);
  delayMicroseconds(2);

  // set the trig pin active for 10 microseconds
  digitalWrite(trig_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig_pin, LOW);

  // get the sound wave travel time in microseconds
  ultrasonic_duration = pulseIn(echo_pin, HIGH);
  /* because the sound wave reflects back from the obstacle,
  distance between the sensor and the obstacle is half of
  the traveled distances */
  ultrasonic_distance = ultrasonic_duration * ultrasonic_speed_factor;

  if (ultrasonic_distance < ultrasonic_threshold) {
    is_ultrasonic_threshold_respected = true;
  } else {
    is_ultrasonic_threshold_respected = false;
  }

  /* write the value as a log at % 2 * maximum_logs,
  because if the number of logs will exceed the maximum logs,
  previous logs will be overwritten; also, + (2 * maximum_logs) is 
  necessary to go after the addresses allocated for the photocell */ 
  eeprom_ultrasonic_address = eeprom_ultrasonic_address_counter % (sizeof(int) * maximum_logs) + eeprom_photocell_memory_space; 
  EEPROM.put(eeprom_ultrasonic_address, ultrasonic_distance);

  // go to the next address space for an integer
  eeprom_ultrasonic_address_counter += 2;
}

/*
  Depending on the value of the 'current_menu', display
  a menu with options for the user.
*/
void choose_menu_to_display(){
  switch (current_menu) {
    case 0:
      Serial.println("MAIN:");
      Serial.println("1. Sensor Settings");
      Serial.println("2. Reset Logger Data");
      Serial.println("3. System Status");
      Serial.println("4. RGB LED Control");
      Serial.println();
      break;
    case 1:
      Serial.println("> Sensor Settings:");
      Serial.println("> 1. Sensor Sampling Interval");
      Serial.println("> 2. Ultrasonic Alert Threshold");
      Serial.println("> 3. LDR Alert Threshold");
      Serial.println("> 4. Back");
      Serial.println();
      break;
    case 11:
      Serial.println("> Sensor Settings > Sensor Sampling Interval");
      Serial.println("Enter interval [1-10 integer]: ");
      Serial.println();
      break;
    case 12:
      Serial.println("> Sensor Settings > Ultrasonic Sampling Threshold");
      Serial.println("Enter Ultrasonic sensor MAX threshold [0, 400 integer]: ");
      Serial.println();
      break;
    case 13:
      Serial.println("> Sensor Settings > LDR Alert Threshold");
      Serial.println("Enter LDR sensor MAX threshold [0, ... integer]: ");
      Serial.println();
      break;
    case 2:
      Serial.println("> Reset Logger Data:");
      Serial.println("> Are you sure you want to reset the logs?");
      Serial.println("> 1. Yes. (0)");
      Serial.println("> 2. No. (1)");
      Serial.println();
      break;
    case 3:
      Serial.println("> System Status:");
      Serial.println("> 1. Current Sensor Readings");
      Serial.println("> 2. Current Sensor Settings");
      Serial.println("> 3. Display Logged Data");
      Serial.println("> 4. Back");
      Serial.println();
      break;
    case 4:
      Serial.println("> RGB LED Control:");
      Serial.println("> 1. Manual Color Control");
      Serial.println("> 2. LED: Toggle Automatic ON/OFF");
      Serial.println("> 3. Back");
      Serial.println();
      break;
    case 41:
      Serial.println("> RGB LED Control > Manual Color Control");
      Serial.println("Enter values for the RGB LED between [0-255] as follows:");
      Serial.println("`255 255 255`, 1st: R, 2nd: G, 3rd B: ");
      Serial.println();
      break;
    default:
      break;
  }
}