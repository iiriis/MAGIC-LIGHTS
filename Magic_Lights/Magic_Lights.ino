/*
 * calibrate the th value according to your mic needs. Debug the code by uncommenting the Serial.print part of the code.
 * A code by TEAM OMEGA.
 */



#define th 4

int c = -1, y = 0, ch = 0;
long ls = 0, tim = 0, tim2 = 0,b=0;
boolean f = true, state=true;

byte pat[]={0b00101010, 0b00010101};  //the dancing pattern of the LEDs;

void setup() {
  pinMode (A0, INPUT); //make A0 as ADC input;
  
  Serial.begin (38400);// Start the serial communication for debugging. (remember without debugging and calibrating, this code wont work for you. );
DDRB=0x3f;

  for(int i=0;i<1000;i++) 
  b = b + analogRead(A0); // accumulate 1000 no of samples for averaging the base value of the mic input;

  b/=1000; //average the samples;


  for (;;) //avoid void loop for timing stability. Use an infinite for loop instead;
  { 
    y = analogRead (A0); //read the samples;

    if (abs(y-b) > th) //trigger the threshold;
      {
        if(millis()-ls>30)  //debounce the values;
        {
        if (f) //restart the clearing timer for the clap sequence;
        {
          tim = millis();
          f = false;  //clear the timer only once;
        }

        ls=millis(); //clear the debounce timer;
        c++;
        }
      }


    if (millis() - tim > 2000)  //the clearing timer that reads the sequences;
    {
      if (c == 0)
        ch = 1;
      else if (c == 1)
        ch = 2;
      else if (c == 2)
        ch = 3;

      c = -1;

      tim = millis(); //reset the clearing timer;
      f = true;
    }
    
    Serial.print (y);
    Serial.print (" ");
    Serial.print (ch);
    Serial.print (" ");
    Serial.println (c);
    
  
    if(ch==1) 
    PORTB=0x3f;   //use port register access for faster output;

    else if(ch==2)
    PORTB=0x00;   //use port register access for faster output;

    else if(ch==3)
    {
      if(millis()-tim2>1000)  //create a delay between the dancing pattern 1s;
      {
      state=!state;   //create the dancing pattern by inverting the outputs sequentially;
      tim2=millis();
      } 
     PORTB=pat[state];  //use port register access for faster output;
    }
  }
  }

void loop() {
  
}
