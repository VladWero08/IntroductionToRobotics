const int data_pin = 12;    // pin for serial data input for the shift register
const int latch_pin = 11;   // pin for latching data into the shift register
const int clock_pin = 10;   // pin for the shift register clock

struct display_digit{
  int pin;
  byte digit;
  bool DP_active;
};
// control pins for each individual digits of the display
display_digit display_digit_D1 = {4, 0, false};   
display_digit display_digit_D2 = {5, 0, false};   
display_digit display_digit_D3 = {6, 0, true};   
display_digit display_digit_D4 = {7, 0, false};   

const int display_digits_count = 4; // number of digits in the display

display_digit display_digits[display_digits_count] = {
  display_digit_D1, display_digit_D2, display_digit_D3, display_digit_D4
};

long unsigned int time = 0;

const int digits_encoding_size = 10;   
const int shift_register_size = 8;   // number of bits in the shift register

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

void setup() {
  // initialize the shift register pins as OUTPUT pins
  pinMode(data_pin, OUTPUT);
  pinMode(latch_pin, OUTPUT);
  pinMode(clock_pin, OUTPUT);

  // initialize the digit control pins as OUTPUT and set them to OFF
  for (int i = 0; i < display_digits_count; i++) {
    pinMode(display_digits[i].pin, OUTPUT);
    digitalWrite(display_digits[i].pin, HIGH);
  }

  Serial.begin(9600);
}

void loop() {
  write_digit_to_segments();
  time_watcher();
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

  for (int i = 0; i < shift_register_size - 1; i++) {
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
  for (int i = 0; i < display_digits_count; i++) {
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

/*
  Updates the digit of each 7-segment depending 
  on how much time has passed from the start of the timer.
*/
void time_watcher(){
    time = millis() / 100;

    display_digits[0].digit = (time / 1000) % 10;
    display_digits[1].digit = (time / 100) % 10;
    display_digits[2].digit = (time / 10) % 10;
    display_digits[3].digit = time % 10;
}