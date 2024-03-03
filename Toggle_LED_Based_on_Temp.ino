/*

*/

volatile bool toggle = false;     // to toggle led state
float temp =0;

void setup(){

pinMode(LED_BUILTIN, OUTPUT);     //set built-in led as output
pinMode(A0, INPUT);               //set A0 pin as input

cli();                            //stop interrupt 
TCCR1A = 0;                       //reset TCCR1A to 0
TCCR1B = 0;                       //reset TCCR1B to 0
TCNT1 = 0;                        //reset timer 1 counter value
TCCR1B |= (1 << WGM12);           //turn on CTC mode
TCCR1B |= B00000100;              //set prescalar as 256
TIMSK1 |= B00000010;              //set OCIE1A to 1 so we enable compare match A

/* system clock 16Mhz and prescaler 256;
   Timer 1 speed = 16Mhz/256 = 62.5Khz;
   pulse time = 1/62.5 khz = 16micro sec;
   500ms/16micro sec = 31250; */

OCR1A = 31250;                    
sei();                             //Enable the interrupts 
}

void loop(){

float sensor_value = analogRead(A0);               // read the value from the sensor
float temp_voltage = ((5/1023)*sensor_value)*1000; //convert the value to get respective voltage
temp = temp_voltage/10;                            // convert the voltage into celsius

}

ISR(TIMER1_COMPA_vect) {             // Timer1 interrupt service routine
  
  unsigned long timerInterval;       // Variable to store the timer interval
  
  if (temp > 30) {                   // Check if temp is greater than 30
    timerInterval = 500;             // Set timer interval to 500 milliseconds
  } else {
    timerInterval = 250;             // Set timer interval to 250 milliseconds
  }
  
  OCR1A = (timerInterval / 16) - 1;  // update OCR1A
  
  toggle = !toggle;                  // Toggle the state
  digitalWrite(LED_BUILTIN, toggle); // Apply the new state
}
