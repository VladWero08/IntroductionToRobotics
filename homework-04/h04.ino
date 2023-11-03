/*
  7-segment display controlled by a joystick

  Circuit that uses a joystick to control a 7-segment display. Starting from the DP
  LED, the user can move through each LED; the current LED where the joystick is pointing
  at will be blinking.

  If the joystick's switch is pressed, the LED will change its state: from ON to OFF,
  respectively from OFF to ON. To reset all LEDs long press the switch button (2 seconds).

  Circuit components:
  - INPUT:
    - A0: joystick VRx
    - A1: joystick VRy
    - DIGITAL 2: joystick SW
  - OUTPUT:
    - DIGITAL 4: joystick 5th pin (DP)
    - DIGITAL 5: joystick 10th pin (G)
    - DIGITAL 6: joystick 9th pin (F)
    - DIGITAL 7: joystick 1st pin (E)
    - DIGITAL 8: joystick 2nd pin (D)
    - DIGITAL 9: joystick 4th pin (C)
    - DIGITAL 10: joystick 6th pin (B)
    - DIGITAL 12: joystick 7th pin (A)

  Created 22-10-2023
  By Olaeriu Vlad Mihai
*/

struct segment_LED{
  int pin;
  int position;     // line position in the corresponding movement matrix 
  bool state;
};

// all the pins that will control the 7-segment display
segment_LED pin_A = {12, 0, LOW};
segment_LED pin_B = {10, 1, LOW};
segment_LED pin_C = {9, 2, LOW};
segment_LED pin_D = {8, 3, LOW};
segment_LED pin_E = {7, 4, LOW};
segment_LED pin_F = {6, 5, LOW};
segment_LED pin_G = {5, 6, LOW};
segment_LED pin_DP = {4, 7, LOW};

/* holds information about the active state of the LEDs,
depending if they have common anode/cathode */
const bool active_state = HIGH;
const int segment_size = 8;
/* array that contains all the pins values that
are connected to the 7 segment display */
segment_LED *segment_LEDs[segment_size] = {
  &pin_A, &pin_B, &pin_C, &pin_D, &pin_E, &pin_F, &pin_G, &pin_DP
};

// all the pins that will control the joystick
const int pin_SW = 2;   // digital pin connected to switch output
const int pin_X = A0;   // analogue pin connected to X axis output 
const int pin_Y = A1;   // analogue pin connected to Y axis output
int X_axis_value = 0, Y_axis_value = 0; // coordinates of the joystick

// controls the actual LED that the joystick is pointing to 
segment_LED *joystick_LED_pointer = &pin_DP;
/* holds the values of the LED that is controlled by the joystick
as a copy, to be able to override the actual state of the LED for blinking purposes*/
segment_LED joystick_LED = {pin_DP.pin, pin_DP.position, pin_DP.state};

/* holds the number of ms when the last blinking of
the LED controlled by the joystick occured*/
long int joystick_LED_last_blinking = 0;
// interval between blinkings of the LED
const int joystick_LED_blinking_interval = 500;

/* number of ms for which the button must be pressed 
to reset the segment*/
const int joystick_reset_interval = 2000;

const int joystick_directions = 4;
/* each direction of the joystick will have a
column in the matrix associated*/
const int joystick_up = 0;
const int joystick_right = 1;
const int joystick_down = 2;
const int joystick_left = 3;

int joystick_movement_matrix[segment_size][joystick_directions] = {
// up      right   down    left
  {&pin_A, &pin_B, &pin_G, &pin_F},       // pin_A
  {&pin_A, &pin_B, &pin_G, &pin_F},       // pin_B
  {&pin_G, &pin_DP, &pin_D, &pin_E},      // pin_C
  {&pin_G, &pin_C, &pin_D, &pin_E},       // pin_D
  {&pin_G, &pin_C, &pin_D, &pin_E},       // pin_E
  {&pin_A, &pin_B, &pin_G, &pin_F},       // pin_F
  {&pin_A, &pin_G, &pin_D, &pin_G},       // pin_G
  {&pin_DP, &pin_DP, &pin_DP, &pin_C}     // pin_DP
};

/* joystick bounds that will identify in which
direction is the user pointing at: up, down, left, right */
const int joystick_up_bound = 768;                    // 512 + 256
const int joystick_down_bound = 256;                  // 512 - 256
const int joystick_right_bound = 768;                 // 512 + 256
const int joystick_left_bound = 256;                  // 512 - 256
const int joystick_middle_interval[2] = {492, 532};   // 512 - 20, 512 + 20

long int joystick_last_direction_change = 0;
const int joystick_last_direction_interval = 500;

bool joystick_current_switch_state = LOW;
bool joystick_last_switch_state = LOW;
long int joystick_last_switch_debounce_time = 0;
const int joystick_switch_debounce_interval = 100;

void setup() {
  for (int i = 0; i < segment_size; i++) {
    // set all pins for the 7-segment as OUTPUT
    pinMode(segment_LEDs[i]->pin, OUTPUT);
    
    // set all LEDs to their inactive state
    segment_LEDs[i]->state = !active_state;
  }

  pinMode(pin_SW, INPUT_PULLUP);
  pinMode(pin_X, INPUT);
  pinMode(pin_Y, INPUT);

  Serial.begin(9600);
}

void loop() {
  joystick_blinking_LED_handler();
  joystick_movement_handler();
  joystick_switch_handler();

  write_to_segment_LEDs();
}

/*
  Writes a digital signal to each of the LEDs on the
  7-segment display, corresponding to the 
  values stored in segment_LED_values.
*/
void write_to_segment_LEDs(){
  for (int i = 0; i < segment_size; i++) {
    if (segment_LEDs[i] == joystick_LED_pointer) {
      continue;
    }
    digitalWrite(segment_LEDs[i]->pin, segment_LEDs[i]->state);
  }
}

/*
  Resets all the LEDs of the 7-segment display and
  the LED to which the joystick will point will be the DP one.
*/
void reset_segment_LEDs(){
  for (int i = 0; i < segment_size; i++) {
    segment_LEDs[i]->state = !active_state;
  }

  joystick_LED_pointer = &pin_DP;
  copy_joystick_segment_LED();
}

/*
  Verifies if the state of the LED controlled
  by the joystick shoudl be changed (checks if the
  interval between blinking events has passed). 
*/
void joystick_blinking_LED_handler(){
  if ((millis() - joystick_LED_last_blinking) > joystick_LED_blinking_interval) {
    joystick_LED.state = !joystick_LED.state;
    joystick_LED_last_blinking = millis();   
  }

  digitalWrite(joystick_LED.pin, joystick_LED.state);
}

/*
  Listens to the joystick movements and if a minimum interval
  of time passed since the last movement, tries to update the current
  state only if the joystick is pointing up, right, down or left.
*/
void joystick_movement_handler(){
  int joystick_direction = -1;  

  if ((millis() - joystick_last_direction_change) <= joystick_last_direction_interval) {
    /*if the minimum interval of time betweens joystick direction changes
    has not passed, do not evaluate the joystick's state*/
    return ;
  }

  joystick_last_direction_change = millis();

  X_axis_value = analogRead(pin_X);
  Y_axis_value = analogRead(pin_Y);

  // check in which direction is the joystick pointing at
  if (X_axis_value > joystick_up_bound && joystick_middle_interval[0] < Y_axis_value < joystick_middle_interval[1]) {
    joystick_direction = joystick_up;        // joystick is pointing up
  } else if (Y_axis_value > joystick_right_bound && joystick_middle_interval[0] < X_axis_value < joystick_middle_interval[1]) {
    joystick_direction = joystick_right;     // joystick is pointing right
  } else if (X_axis_value < joystick_down_bound && joystick_middle_interval[0] < Y_axis_value < joystick_middle_interval[1]) {
    joystick_direction = joystick_down;      // joystick is pointing down
  } else if (Y_axis_value < joystick_left_bound && joystick_middle_interval[0] < X_axis_value < joystick_middle_interval[1]) {
    joystick_direction = joystick_left;      // joystick is pointing left 
  }

  if (joystick_direction == -1) {
    /* if the joystick is pointing somewhere in the middle,
    ignore its movement*/
    return ;
  }

  if (joystick_LED_pointer != joystick_movement_matrix[joystick_LED.position][joystick_direction]) {
    /* if the joystick is pointing to a LED different to the current LED
    where it is pointing at, change its pointer*/
    joystick_LED_pointer = joystick_movement_matrix[joystick_LED.position][joystick_direction];
    copy_joystick_segment_LED();
  }
}

/*
  Listens to the switch button on the joystick, and
  changes its state using the debounce technique
*/
void joystick_switch_handler(){
  bool joystick_switch_state = !digitalRead(pin_SW);

  if (joystick_switch_state != joystick_last_switch_state) {
    // if the state has changed, update the last debounce time of the button
    joystick_last_switch_debounce_time = millis();
  } 

  if ((millis() - joystick_last_switch_debounce_time) > joystick_switch_debounce_interval) {
    /* because it passed a certain debounce time, it means that the reading state
    is not a noise, so the current state of the button must be changed*/
    
    if (joystick_current_switch_state != joystick_switch_state) {
      joystick_current_switch_state = joystick_switch_state;
    
      if (joystick_current_switch_state == HIGH) {
        /* if the button is pressed, update the LED's state
        to which the joystick is currently pointing at*/
        joystick_LED_pointer->state = !joystick_LED_pointer->state
      }
    }
  } 

  if ((millis() - joystick_last_switch_debounce_time) == joystick_reset_interval && joystick_switch_state == HIGH){
    /*if the button has been pressed for enough time,
    reset the 7-segment display*/
    reset_segment_LEDs();
  }

  joystick_last_switch_state = joystick_switch_state;
}

/*
  After the joystick pointer changed its focus to
  another LED, copy the new values into the joystick LED variable
*/
void copy_joystick_segment_LED(){
    joystick_LED.pin = joystick_LED_pointer->pin;
    joystick_LED.position = joystick_LED_pointer->position;
    joystick_LED.state = joystick_LED_pointer->state;
}