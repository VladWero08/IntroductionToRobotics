/*
  RGB LED controlled by potentiometers

  Small circuit that controls the colors of an RGB LED using individual potentiometers.

  Circuit components:
  - INPUT:
    - A0: potentiometer that controls the R channel of the LED
    - A1: potentiometer that controls the G channel of the LED
    - A2: potentiometer that controls the B channel of the LED
  - OUTPUT:
    - DIGITAL (PWM~) 9: writes mapped values from A0 to the R channel of the LED
    - DIGITAL (PWM~) 10: writes mapped values from A1 to the G channel of the LED
    - DIGITAL (PWM~) 11: writes mapped values from A2 to the B channel of the LED

  Created 22-10-2023
  By Olaeriu Vlad Mihai
*/

// pins used for writing analogue values to the RGB color channels
const int R_led_channel_pin = 9, G_led_channel_pin = 10, B_led_channel_pin = 11;
// pins used for reading analogue values from the potentiometer
const int R_channel_slider = A0, G_channel_slider = A1, B_channel_slider = A2;

const float RGB_lower_bound = 0.0, RGB_upper_bound = 255.0;
const float slider_lower_bound = 0.0, slider_upper_bound = 1023.0;

void setup() {
  Serial.begin(9600);

  // configure pins to behave as output for RGB color channels
  pinMode(R_led_channel_pin, OUTPUT);
  pinMode(G_led_channel_pin, OUTPUT);
  pinMode(B_led_channel_pin, OUTPUT);
}

void loop() {
  // for each RGB color channel, modify its intensity based
  // on the related potentiometer's value
  modify_RGB_channel_intensity(R_led_channel_pin, R_channel_slider);
  modify_RGB_channel_intensity(G_led_channel_pin, G_channel_slider);
  modify_RGB_channel_intensity(B_led_channel_pin, B_channel_slider);
}

/*
  Given a PIN corresponding to a color channel of an RGB LED and
  a PIN corresponding to a potentiometer that controls that RGB color channel,
  read the value from the potentiometer, map it to an RGB value, and write it to the interrelated PIN.
*/
void modify_RGB_channel_intensity(const int RGB_led_channel_pin, const int RGB_channel_slider){
  float RGB_channel_slider_value, RGB_channel_intensity;

  RGB_channel_slider_value = analogRead(RGB_channel_slider);
  // map previously read potentiometer value to RGB value
  RGB_channel_intensity = map(RGB_channel_slider_value, slider_lower_bound, slider_upper_bound, RGB_lower_bound, RGB_upper_bound);

  analogWrite(RGB_led_channel_pin, RGB_channel_intensity);
}