/* variables used for reading Strings 
from the Serial */
char read_incoming_byte = 0;
String read_input_string = "";

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
// 
bool LED_toggle_mode_state = LOW;

// PIN value for the photocell sensor
const int photocell_pin = 0;   // connected to A0
int photocell_value;

// PIN values for HC-SR04 ultrasonic sensor
const int trig_pin = 5;   // trig pin 
const int echo_pin = 6;   // echo pin 

/* variables in which the user input
will be read into */
int user_input_int = 0;
String user_input_str;
// current menu that is being displayed
unsigned int current_menu = 0;

/* variables related to every menu of 
the program: main, sensor, logger, system and RGB LED menus*/

String main_menu_error_input = "Invalid input! Number in interval [1-4] is expected.";
const unsigned int main_menu_options_size = 5;
String main_menu_options[] = {
  "Press the labeled number to enter in the desired menu:",
  "1. Sensor Settings",
  "2. Reset Logger Data",
  "3. System Status",
  "4. RGB LED Control"
};

String sensor_menu_error_input = "Invalid input! Number in interval [1-4] is expected.";
const unsigned int sensor_menu_options_size= 5;
String sensor_menu_options[] = {
  ">>> Sensor Settings:",
  ">>> 1. Sensor Sampling Interval",
  ">>> 2. Ultrasonic Alert Threshold",
  ">>> 3. LDR Alert Threshold",
  ">>> 4. Back"
};

String logger_menu_error_input = "Invalid input! `y` or `n` is expected.";
const unsigned int logger_menu_options_size = 4; 
String logger_menu_options[] = {
  ">>> Reset Logger Data:",
  ">>> Are you sure you want to reset the logs?",
  ">>> 1. Yes. (y)",
  ">>> 2. No. (n)"
};

String system_menu_error_input = "Invalid input! Number in interval [1-4] is expected.";
const unsigned int system_menu_options_size = 5; 
String system_menu_options[] = {
  ">>> System Status:",
  ">>> 1. Current Sensor Readings",
  ">>> 2. Current Sensor Settings",
  ">>> 3. Display Logged Data",
  ">>> 4. Back"
};

String RGB_LED_menu_error_input = "Invalid input! Number in interval [1-3] is expected.";
const unsigned int RGB_LED_menu_options_size = 4; 
String RGB_LED_menu_options[] = {
  ">>> RGB LED Control:",
  ">>> 1. Manual Color Control",
  ">>> 2. LED: Toggle Automatic ON/OFF",
  ">>> 3. Back"
};

const unsigned int RGB_LED_manual_control_message_size = 2;
String RGB_LED_manual_control_message [] {
  "Enter values for the RGB LED between [0-255] as follows:",
  "`255 255 255`, 1st: R, 2nd: G, 3rd B: "
};

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

  write_to_RGB_LED();

  // digitalWrite(trig_pin, LOW);
  // delayMicroseconds(2);

  // digitalWrite(trig_pin, HIGH);
  // delayMicroseconds(10);
  // digitalWrite(trig_pin, LOW);

  // long duration = pulseIn(echo_pin, HIGH);
  // int distance = duration * 0.034 / 2;

  // Serial.print("Distance: ");
  // Serial.println(distance);
  // delay(100); 
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
    case 2:
      handle_user_input_on_logger_menu();
      break;
    case 3:
      handle_user_input_on_system_menu();
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
    Serial.println(main_menu_error_input);
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
    Serial.println(sensor_menu_error_input);
    return;
  }

  switch (user_input_int) {
    case 1:
      break;
    case 2:
      break;
    case 3:
      break;
    case 4:
      // switch back to the main menu
      current_menu = 0;
      break;
    default:
      break;
  }
}

void handle_user_input_on_logger_menu(){
  user_input_str = Serial.readString();

  if (user_input_str != "y" || user_input_str != "n") {
    /* user input is not valid, print an error
    message and exit the function */
    Serial.println(logger_menu_error_input);
    return;
  }

  if (user_input_str == "y") {
    return;
  } else {
    return;
  }
}

void handle_user_input_on_system_menu(){
  user_input_int = Serial.parseInt();

  if (user_input_int < 1 || user_input_int > 4) {
        /* user input is not valid, print an error
    message and exit the function */
    Serial.println(system_menu_error_input);
    return;
  } 

  switch (user_input_int) {
    case 1:
      break;
    case 2:
      break;
    case 3:
      break;
    case 4:
      // switch back to the main menu
      current_menu = 0;
      break;
    default:
      break;
  }
}

void handle_user_input_on_RGB_LED_menu(){
  user_input_int = Serial.parseInt();

  if (user_input_int < 1 || user_input_int > 3) {
    /* user input is not valid, print an error
    message and exit the function */
    Serial.println(RGB_LED_menu_error_input);
    return;
  }

  switch (user_input_int) {
    case 1:
      // switch to manual color control settting
      current_menu = 41;
      break;
    case 2:
      // switch to toggle automatic setting
      current_menu = 42;
      break;
    case 3:
      // switch back to the main menu
      current_menu = 0;
      break;
    default:
      break;
  }
}

void read_RGB_LED_input(){
  LED_red_user_input = Serial.parseFloat();
  Serial.read();
  LED_green_user_input = Serial.parseFloat();
  Serial.read();
  LED_blue_user_input = Serial.parseFloat();

  if (LED_red_user_input < 0 || LED_red_user_input > 255) {
    Serial.println("Wrong RGB values! Please follow the instructions.");
    return;
  }
  if (LED_green_user_input < 0 || LED_green_user_input > 255) {
    Serial.println("Wrong RGB values! Please follow the instructions.");
    return;
  }
  if (LED_blue_user_input < 0 || LED_blue_user_input > 255) {
    Serial.println("Wrong RGB values! Please follow the instructions.");
    return;
  }

  current_menu = 4;
  LED_red_pin_value = LED_red_user_input;
  LED_green_pin_value = LED_green_user_input;
  LED_blue_pin_value = LED_blue_user_input;
}

void write_to_RGB_LED(){
  if (LED_toggle_mode_state == LOW) {
      analogWrite(LED_red_pin, LED_red_pin_value);
      analogWrite(LED_green_pin, LED_green_pin_value);
      analogWrite(LED_blue_pin, LED_blue_pin_value);
  } else {
    /* check if the threshold for the 
    sensors is respected*/
  }
}

/*
  Depending on the value of the 'current_menu', display
  a menu with options for the user.
*/
void choose_menu_to_display(){
  switch (current_menu) {
    case 0:
      display_menu(main_menu_options, main_menu_options_size);
      break;
    case 1:
      display_menu(sensor_menu_options, sensor_menu_options_size);
      break;
    case 2:
      display_menu(logger_menu_options, logger_menu_options_size);
      break;
    case 3:
      display_menu(system_menu_options, system_menu_options_size);
      break;
    case 4:
      display_menu(RGB_LED_menu_options, RGB_LED_menu_options_size);
      break;
    case 41:
      display_menu(RGB_LED_manual_control_message, RGB_LED_manual_control_message_size);
      break;
    default:
      break;
  }
}

/*
  Given a menu and its size, display all the
  options provided by that menu.
*/
void display_menu(String menu[], int menu_size){
  Serial.println(menu_size);
  for (int i = 0; i < menu_size; i++) {
    Serial.println(menu[i]);
  }
  Serial.println();
}

