
bool LED_STATE1 = true;
bool LED_STATE2 = true;

void setup(){

pinMode(LED_BUILTIN, OUTPUT);     //set built-in led as output
pinMode(A0, INPUT);               //set A0 pin as input

cli();                            //stop interrupt 
TCCR1A = 0;                       //reset TCCR1A to 0
TCCR1B = 0;                       //reset TCCR1B to 0
TCCR1B |= B00000100;              //set prescalar as 256
TIMSK1 |= B00000010;              //set OCIE1A to 1 so we enable compare match A

/* system clock 16Mhz and prescaler 256;
   Timer 1 speed = 16Mhz/256 = 62.5Khz;
   pulse time = 1/62.5 khz = 16micro sec;
   500ms/16micro sec = 31250; */

OCR1A = 31250;                    

TCCR2A = 0;
TCCR2B = 0;
TCCR2B |= B00000100;
TIMSK2 |= B00000100;

/* system clock 16Mhz and prescaler 256;
   Timer 1 speed = 16Mhz/256 = 62.5Khz;
   pulse time = 1/62.5 khz = 16micro sec;
   250ms/16micro sec = 15625; */

OCR2B = 15625;
sei();                             //Enable the interrupts 
}

void loop(){

float sensor_value = analogRead(A0);  // read the value from the sensor
float temp_voltage = ((5/1023)*sensor_value)*1000; //convert the value to get respective voltage
float temp = temp_voltage/10;  // convert the voltage into celsius

if(temp < 30){             // if the temperature is below 30

 ISR(TIMER2_COMPB_vect){   // exicute the interrupt call

   TCNT1 = 0;                              
   LED_STATE1 = !LED_STATE1;
   digitalWrite(LED_BUILTIN, LED_STATE1);

}
}

else if(temp > 30){        // else if the temperature is above 30

ISR(TIMER1_COMPA_vect){    // exicute another interrupt call

  TCNT1 = 0;                               
  LED_STATE2 = !LED_STATE2;
  digitalWrite(LED_BUILTIN, LED_STATE2);
}
}
}


 

