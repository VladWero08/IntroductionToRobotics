/*
  Elevator wannabe

  Small circuit that is simulating a 3-floor elevator control system using
  LEDs and buttons.

  Circuit components:
  - INPUT:
    - DIGITAL (PWM~) 9:  button that calls the elevator on floor 0 
    - DIGITAL (PWM~) 10: button that calls the elevator on floor 1
    - DIGITAL (PWM~) 11: button that calls the elevator on floor 2
  - OUTPUT
    - DIGITAL 2:        led that is ON when the elevator is at floor 0
    - DIGITAL (PWM~) 3: led that is ON when the elevator is at floor 0
    - DIGITAL 4:         led that is ON when the elevator is at floor 0

    Created 28-10-2023
    By Olaeriu Vlad Mihai
*/

// each floor of the elevator will have an LED connected to a PIN,
// this array holds the PINS used for this elevator as:
// floor 0 has PIN 2: elevator_floor_LED_PINS[0] = 2 
const int elevator_floor_LED_PINS[] = {2, 3, 4};
byte elevator_floor_LED_states[] = {HIGH, LOW, LOW};

// the PIN of the button that will blink during the elevator movement
const int elevator_moving_LED_PIN = 5;
byte elevator_moving_LED_state = LOW;
// variable that will decide if the elevator moving LED is blinking
byte elevator_moving_LED_blinking_state = LOW;
// the time when the last blinking of the moving elevator's LED occured
unsigned long elevator_moving_LED_last_blinking = 0;
// the minimum interval when blinking can occur 
const int elevator_moving_LED_blinking_interval = 100;

// data structure that will hold information 
// about the button on each floor of the elevator
struct elevator_button{
  const int button_PIN;                 // the PIN to which the button is connected
  int floor;                            // the elevator floor assigned to the button 
  byte current_button_state;            // the current state of the button's PIN
  byte last_button_state;               // the previous state of the button's PIN
  unsigned long last_debounce_time;     // the last time the button state has changed
};
// initialize buttons for every floor of the elevator
elevator_button elevator_floor_0_button = {9,  0, LOW, LOW, 0};
elevator_button elevator_floor_1_button = {10, 1, LOW, LOW, 0};
elevator_button elevator_floor_2_button = {11, 2, LOW, LOW, 0};
// the minimum time that a reading state must persist
// to consider it an actual action, not a noise
unsigned long debounce_delay = 50;

// variable that will tell if the elevator is moving
byte elevator_is_moving = LOW;
// which floor is the elevator currently at
int elevator_current_floor = 0;
int elevator_destination_floor = 0;
// depending on the destination floor, the variable 
// will be 1/-1, meaning: moving up or moving down
int elevator_direction = 1;
// the last time the elevator has moved
unsigned long elevator_last_movement_time = 0;

// LOW if the doors are closed, HIGH if they are opened
byte elevator_doors = LOW;
// the time since the doors state has changed
unsigned long elevator_doors_state_change_time = 0;
// the time it takes for the elevator to 
// move to the next floor or to
// close/open its doors
const int elevator_actions_interval = 2000;

void setup() {
  // put your setup code here, to run once:
  pinMode(elevator_floor_LED_PINS[0], OUTPUT);
  pinMode(elevator_floor_LED_PINS[1], OUTPUT);
  pinMode(elevator_floor_LED_PINS[2], OUTPUT);
  pinMode(elevator_moving_LED_PIN, OUTPUT);

  pinMode(elevator_floor_0_button.button_PIN, INPUT_PULLUP);
  pinMode(elevator_floor_1_button.button_PIN, INPUT_PULLUP);
  pinMode(elevator_floor_2_button.button_PIN, INPUT_PULLUP);

  Serial.begin(9600);
}

void loop() {
  // handle each button of the elevator
  elevator_button_handler(elevator_floor_0_button);
  elevator_button_handler(elevator_floor_1_button);
  elevator_button_handler(elevator_floor_2_button);

  elevator_tries_closing_doors();
  elevator_tries_moving();

  // write digital signals to each LED
  digitalWrite(elevator_floor_LED_PINS[0], elevator_floor_LED_states[0]);
  digitalWrite(elevator_floor_LED_PINS[1], elevator_floor_LED_states[1]);
  digitalWrite(elevator_floor_LED_PINS[2], elevator_floor_LED_states[2]);

  elevator_moving_LED_handler();
}

/*
  Function that checks if the blicking state of the elevator moving LED
  has changed. If the state is TRUE, blink the LED once in 50 ms.
  [ write digital signal every 50 ms ]
*/
void elevator_moving_LED_handler(){
  if(elevator_moving_LED_blinking_state == HIGH){
    if((millis() - elevator_moving_LED_last_blinking) > elevator_moving_LED_blinking_interval){
      // if the LED should be blinking, and at least 'elevator_moving_LED_flickering_interval' ms
      // have passed since the last blink, change the LED's state
      elevator_moving_LED_state = !elevator_moving_LED_state;
      // also, update the time when the last blinking happened
      elevator_moving_LED_last_blinking = millis();
    }
  } else{
    elevator_moving_LED_state = LOW;
  }

  // update the elevator moving LED'S PIN value 
  digitalWrite(elevator_moving_LED_PIN, elevator_moving_LED_state);
}

/*
  Function that, using the debounce technique, reads the digital signal from 
  the elevator button given as parameter and updates its state.

  If the button is active, it tries to move the elevator to the desired floor.
*/
void elevator_button_handler(elevator_button &elevator_button_){
  // read the state of the button from its digital PIN
  byte elevator_button_state = !digitalRead(elevator_button_.button_PIN);

  // if the state has changed, update the last debounce time of the button
  if(elevator_button_state != elevator_button_.last_button_state){
    elevator_button_.last_debounce_time = millis();
  }

  if((millis() - elevator_button_.last_debounce_time) > debounce_delay){
    // because it passed a certain debounce time, it means that the reading state
    // is not a noise, so the current state of the elevator button must be changed
    if(elevator_button_state != elevator_button_.current_button_state){
      elevator_button_.current_button_state = elevator_button_state;

      if(elevator_button_.current_button_state == HIGH){
        // if the button state is high, try to call the elevator
        // to the desired floor
        elevator_floor_change_handler(elevator_button_.floor);
      }
    } 
  }

  // save the current state to be the last button state for the next iteration
  elevator_button_.last_button_state = elevator_button_state;
}

/*
  Function that will check if the elevator is moving in the current moment.
  
  If it is not, the destination floor of the elevator will be `destination_floor`
  parameter received in the function.
*/
void elevator_floor_change_handler(int destination_floor){
  // if the elevator is moving, ignore the desired change 
  if(elevator_is_moving == HIGH){
    return;
  }

  // if the elevator is already on the desired floor
  if(destination_floor == elevator_current_floor){
    return;
  }  

  if(destination_floor < 0 || destination_floor > 2){
    Serial.print("The elevator has only 3 floors! [0, 1, 2]. floor ");
    Serial.print(destination_floor);
    Serial.println(" has been required.");

    return;
  }

  elevator_doors = HIGH;
  // update the time when the elevator doors changed
  elevator_doors_state_change_time = millis();
  // update the elevator destination
  elevator_destination_floor = destination_floor;

  // update the direction of the elevator
  if(elevator_destination_floor > elevator_current_floor)
    elevator_direction = 1;
  else
    elevator_direction = -1;
}

/*
  Function that checks if the doors need to be closing. 

  If they are closing, a certain amount of time needs to pass
  before the elevator will start moving.
*/
void elevator_tries_closing_doors(){
  if(elevator_doors == LOW)
    // if elevator doors are not closing, exit
    return;


  if((millis() - elevator_doors_state_change_time) > elevator_actions_interval){
    // if the amount of time has passed, elevator can start
    // moving, and the the elevator moving LED will start blinking
    elevator_doors = LOW;
    elevator_is_moving = HIGH;
    elevator_last_movement_time = millis();
    elevator_moving_LED_blinking_state = HIGH;
  }
}

/*
  Function that checks if the elevator is moving. 

  If it is moving, a certain amount of time needs to pass
  before it reaches the next floor.
*/
void elevator_tries_moving(){
    if(elevator_is_moving == HIGH){  
      if((millis() - elevator_last_movement_time) > elevator_actions_interval){
        // if the elevator "has been moving" for more than `elevator_moving_interval` ms
        // it means it has reached the next floor
        elevator_last_movement_time = millis();

        // the passed floor LED will be turned off
        elevator_floor_LED_states[elevator_current_floor] = LOW;
        elevator_current_floor += elevator_direction;
        // the newly reached floor LED will be turned on 
        elevator_floor_LED_states[elevator_current_floor] = HIGH;
        
        if(elevator_current_floor == elevator_destination_floor){
          // if the elevator recahed the destination,
          // stops moving, opens the doors, and the LED stops blinking
          elevator_is_moving = LOW;
          elevator_moving_LED_blinking_state = LOW;
        }
      }
    }
}