const int data_pin = 12;    // pin for serial data input for the shift register
const int latch_pin = 11;   // pin for latching data into the shift register
const int clock_pin = 10;   // pin for the shift register clock

struct display_digit{
  int pin;         // pin to which the display digit is connected
  byte digit;      // digit displayed at the current moment [0-9]
  bool DP_active;  // HIGH if DP segment should be active, LOW otherwise
};

// 4 digit 7 segment display configuration
const int display_digits_size = 4; // number of digits in the display
display_digit display_digit_D1 = {4, 0, false};   // 1st digit is connected to the 4th pin   
display_digit display_digit_D2 = {5, 0, false};   // 2nd digit is connected to the 5th pin
display_digit display_digit_D3 = {6, 0, true};    // 3rd digit is connected to the 6th pin
display_digit display_digit_D4 = {7, 0, false};   // 4th digit is connected to the 7th pin

display_digit display_digits[display_digits_size] = {
  display_digit_D1, display_digit_D2, display_digit_D3, display_digit_D4
};

const int digits_encoding_size = 10; // number of digits in the encoding [0-9]
const int shift_register_size = 7;   // number of bits in the shift register

byte digits_encoding[digits_encoding_size][shift_register_size] = {
// A, B, C, D, E, F, G
  {1, 1, 1, 1, 1, 1, 0}, // 0
  {0, 1, 1, 0, 0, 0, 0}, // 1
  {1, 1, 0, 1, 1, 0, 1}, // 2
  {1, 1, 1, 1, 0, 0, 1}, // 3
  {0, 1, 1, 0, 0, 1, 1}, // 4
  {1, 0, 1, 1, 0, 1, 1}, // 5
  {1, 0, 1, 1, 1, 1, 1}, // 6
  {1, 1, 1, 0, 0, 0, 0}, // 7
  {1, 1, 1, 1, 1, 1, 1}, // 8
  {1, 1, 1, 1, 0, 1, 1}, // 9
};

struct stopwatch_control_button{
  int pin;                                // pin to which the button is connected
  bool last_button_state;        
  bool current_button_state;
  unsigned long int last_debounce_time;   // last time in ms when the physical state of button has changed
};
/* minimum ms that need to pass to consider a
button press valid for the control buttons*/
const int debounce_interval = 50;         

unsigned int stopwatch_timer = 0;         // will hold the current time in ms of the stopwatch

// button to control running/pause state of the stopwatch
stopwatch_control_button running_button = {13, LOW, LOW, 0};  
bool stopwatch_running_state = LOW;                           // LOW: pause mode, HIGH: running mode
unsigned long int stopwatch_last_pause = 0;                   // time in ms when the last pause occurred
unsigned long int stopwatch_total_pause_duration = 0;         // total duration of all pauses of the stopwatch

/* button that will reset the stopwatch timer if it is
in the PAUSE mode / reset the laps vector if the stopwatch is
in laps display mode */
stopwatch_control_button reset_button = {9, LOW, LOW, 0};        

// button that will save the laps of the stopwatch
stopwatch_control_button laps_button = {8, LOW, LOW, 0};      
bool stopwatch_display_laps_state = LOW;        // LOW: not in laps display mode, HIGH: in laps display mode
const unsigned int maximum_laps = 4;            // maximum number of laps that will be stored in the memory
unsigned int total_number_of_laps = 0;          // total number of laps that where saved
unsigned int current_number_of_laps = 0;        // number of laps completed in the vector of laps
unsigned int display_laps_counter = 0;          // counter for the vector of laps when displaying them
byte laps[maximum_laps][display_digits_size];   // vector of saved laps

const int control_buttons_size = 3;
stopwatch_control_button control_buttons[] = {
  running_button, reset_button, laps_button
};

void setup() {
  // initialize the shift register pins as OUTPUT pins
  pinMode(data_pin, OUTPUT);
  pinMode(latch_pin, OUTPUT);
  pinMode(clock_pin, OUTPUT);

  // initialize the digit control pins as OUTPUT and set them to OFF
  for (int i = 0; i < display_digits_size; i++) {
    pinMode(display_digits[i].pin, OUTPUT);
    digitalWrite(display_digits[i].pin, HIGH);
  }

  // initialize each control button as INPUT PULLUP
  for (int i = 0; i < control_buttons_size; i++) {
    pinMode(control_buttons[i].pin, INPUT_PULLUP);
  }

  Serial.begin(9600);
}

void loop() {
  stopwatch_timer_watcher();

  control_button_watcher(running_button, running_button_handler);
  control_button_watcher(reset_button, reset_button_handler);
  control_button_watcher(laps_button, laps_button_handler);

  write_digit_to_segments();
}

/*
  Updates each digit of the stopwatch only
  if the stopwatch is in running mode.
*/
void stopwatch_timer_watcher(){
  if (stopwatch_running_state == HIGH) {
    // the timer will be updated only if the 
    // stopwatch is in the RUNNING mode
    stopwatch_timer = (millis() - stopwatch_total_pause_duration) / 100;

    for (int i = 0; i < display_digits_size; i++){
      display_digits[i].digit = stopwatch_timer % 10;
      stopwatch_timer /= 10;
    }
  }
}

/*
  Using the debounce technique, reads the digital signal from 
  the control button given as parameter and updates its state.

  If the button is active, calls the handler parameter function.
*/
void control_button_watcher(stopwatch_control_button &control_button, void (*handle_control_button_change)()){
  // read the state of the control button
  bool control_button_state = !digitalRead(control_button.pin);

  // if the state has changed, update the last debouce time of the button
  if (control_button_state != control_button.last_button_state){
    control_button.last_debounce_time = millis();
  }

  if ((millis() - control_button.last_debounce_time) > debounce_interval) {
    // because it passed a certain debounce time, it means that the reading state
    // is not a noise, so the current state of the control button must be changed
    
    if (control_button_state != control_button.current_button_state) {
      control_button.current_button_state = control_button_state;

      if (control_button.current_button_state == HIGH){
        // if the button state is HIGH, call the function
        // that will handle this change for the given button
        handle_control_button_change();
      }
    }
  }

  // update the last_button_state to correspond to 
  // current physical tate of the button
  control_button.last_button_state = control_button_state;
}

/*
  Handles the stopwatch running state change when 
  the running/pause button has been pressed.
*/
void running_button_handler(){
  stopwatch_running_state = !stopwatch_running_state;

  if (stopwatch_running_state == HIGH) {
    /* the stopwatch will enter the running MODE, so 
    the pause duration must be updated by adding the time 
    that passed since the last pause has made */ 
    stopwatch_total_pause_duration += (millis() - stopwatch_last_pause);

    /* if the stopwatch started running, it means the lap
    display functionality needs to be turned OFF */
    stopwatch_display_laps_state = LOW;
  } else {
    /* the stopwatch will enter the PAUSE MODE, so
    the time when the last pause occurred must be updated */    
    stopwatch_last_pause = millis();
  }
}

/*
  It will reset the stopwatch timer if the stopwatch is
  in PAUSE mode. It will reset the saved laps if the stopwatch is
  in laps display mode.
*/
void reset_button_handler(){
  if (stopwatch_running_state == HIGH) {
    // if the stopwatch is running, exit
    return;
  }

  if (stopwatch_display_laps_state == HIGH) {
    // in display laps mode
    total_number_of_laps = 0;     // reset the total number of laps
    current_number_of_laps = 0;   // reset the number of laps completed in the laps vector

    // reset the vector of saved laps
    for (int i = 0; i < maximum_laps; i++) {
      for (int j = 0; j < display_digits_size; j++){
        laps[i][j] = 0;
      }
    }
  }

  for (int i = 0; i < display_digits_size; i++){
    // reset each digit of the stopwatch to display "0"
    display_digits[i].digit = 0;
  }

  /* the reset will be available only if the stopwatch is in pause mode,
  so, to reset the stopwatch's total pause duration, it needs to be set
  to the time when the pause preceding the reset occurred */
  stopwatch_total_pause_duration = stopwatch_last_pause;
}

/*
  Handles the lap viewing mode.

  If the stopwatch is running, pressing the corresponding
  button will save the current time as a lap (max. 4). Saving
  more than 4 laps will running overriding the existing laps.

  If the stopwatch is in PAUSE mode, pressing the button will cycle
  through the saved laps until this moment.
*/
void laps_button_handler(){
  if (stopwatch_running_state == HIGH) {
    // stopwatch is in RUNNING mode
    for (int i = 0; i < display_digits_size; i++) {
      laps[total_number_of_laps][i] = display_digits[i].digit;
    }

    /* update the number of laps after adding the current lap using modulo,
    in order to be able to cycle the vector of laps and override the laps
    from the past, if the total number of laps is bigged than the maximum allowed*/
    total_number_of_laps = (total_number_of_laps + 1) % maximum_laps;

    /* update the display lap counter to be 0 in case the user 
    will try to display the laps */
    display_laps_counter = 0;
    
    /* update the current number of laps only if maximum number
    of laps has not been reached yet */
    if (current_number_of_laps < maximum_laps) {
      current_number_of_laps += 1;
    }
  } else {
    // stopwatch is in PAUSE mode
    stopwatch_display_laps_state = HIGH;

    if (current_number_of_laps >= 1) {
        /* update the 4 digit 7-segment with the lap 
        from the current position only if there was at least one
        lap saved*/
        for (int i = 0; i < display_digits_size; i++) {
          display_digits[i].digit = laps[display_laps_counter][i];
        }

        /* increment the lap display counter using modulo
        of the current number of laps to cycle if the
        counter will be too big */
        display_laps_counter = (display_laps_counter + 1) % current_number_of_laps;
        }
  }
} 

/*
  Loads a digit (as bytes) into the
  shift register and latches data into it.

  DP byte will depend on the parameter given
  in the function. 
*/
void write_shift_register(byte encoding[], bool DP_active) {
  // prepare to send data to the register
  digitalWrite(latch_pin, LOW); 

  for (int i = 0; i < shift_register_size; i++) {
    digitalWrite(clock_pin, LOW);           // set the clock pin LOW before sending data
    digitalWrite(data_pin, encoding[i]);    // send the data bit
    digitalWrite(clock_pin, HIGH);          // clock the data bit into the shift register
  }

  digitalWrite(clock_pin, LOW);           
  digitalWrite(data_pin, DP_active);
  digitalWrite(clock_pin, HIGH);  

  // latch the data into the shift register to update the digit
  digitalWrite(latch_pin, HIGH);
}

/*
  For every 7-segment display, load the corresponding
  digit into the shift register and latches it.

  After latching the shift register, the 7-segment display
  will be active for a small period of time. Doing this for
  every segment, it will create the optical illusion that
  they are showing different digits.
*/
void write_digit_to_segments(){
  for (int i = 0; i < display_digits_size; i++) {
    // load and latch the shift register with the digit
    // found in the current 7-segment
    byte current_digit = display_digits[i].digit;
    write_shift_register(digits_encoding[current_digit], display_digits[i].DP_active);

    // turn ON the 7-segment for a short period of time
    digitalWrite(display_digits[i].pin, LOW);
    delay(1);
    digitalWrite(display_digits[i].pin, HIGH);
  }
}