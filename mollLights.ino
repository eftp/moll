/*
 * Moll Dyer room lights and door lock
*/

// digital pin 2 has a pushbutton attached to it. Give it a name:
int lightSwitch = 1;  //turns the lights on and off
int doorLock = 2;     //turns the door lock on and off
int doorButton = 7;  //the button inside the doorjam that gets pushed (set to high) when the door is closed
int indicator = 13;  //indicator led on the board.  If lit, door is closed
int bookshelfDoorButton = 8;
int resetButton = 9;
int DoorClosedPiOutput = 3;

int doorButtonState = 0;  //store the state of the door button
int previousButtonState = 0;
int bookshelfDoorButtonState = 0;
int numDoorButtonPushes = 0;
int resetButtonState = 0;
int lightsOn = 0;
int previousButtonStateLow = 0;
int previousButtonStateHigh = 0;



// the setup routine runs once when you press reset:
void setup() {
  // make the pushbutton's pin an input:
  pinMode(lightSwitch, OUTPUT);
  pinMode(doorLock, OUTPUT);
  pinMode(doorButton, INPUT);
  pinMode(indicator, OUTPUT);
  pinMode(bookshelfDoorButton, INPUT);
  pinMode(resetButton, INPUT);
  pinMode(DoorClosedPiOutput, OUTPUT);

  numDoorButtonPushes = 1; //start at 1 for % operator to work
  doorButtonState = 0;  //store the state of the door button
  bookshelfDoorButtonState = 0;
  previousButtonState = 0;
  resetButtonState = 0;
  lightsOn = 0;
  previousButtonStateLow = 0;
  previousButtonStateHigh = 0;
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input pin:
  doorButtonState = digitalRead(doorButton);
//  bookshelfDoorButtonState = digitalRead(bookshelfDoorButton); //not hooked up yet
//  resetButtonState = digitalRead(resetButton);  //not hooked up yet

  //if the switch is depressed
  if(doorButtonState == HIGH)
  {
    digitalWrite(indicator, HIGH);        

    //if it was not pressed on the last loop
    if(previousButtonState == LOW)
      ++previousButtonStateLow;
      
    if(previousButtonStateLow > 2 && previousButtonState == LOW)  //button must be pushed for 3 iterations of the loop before a change, if state is high
    {
      previousButtonState = HIGH;
      previousButtonStateLow = 0; //reset counter for next iteration
      previousButtonStateHigh = 0;
      
      //if the light is off, turn it on and lock the door, folks have entered the room
      if(lightsOn == 0) //if lights are off, turn them on and lock the door
      {
        //turn on lights when door is closed
        digitalWrite(lightSwitch, HIGH);    
        
        //lock door magnetic lock when it is closed
        digitalWrite(doorLock, HIGH);

        //set lightsOn to 1 for next time that button is depressed
        lightsOn = 1;

        //tell the PI the door is closed and locked
        digitalWrite(DoorClosedPiOutput, HIGH);        

      }
      //if the light is on, turn it off and unlock the door for room reset
      else if(lightsOn == 1)
      {
        //turn on lights when door is closed
        digitalWrite(lightSwitch, LOW);    
        
        //lock door magnetic lock when it is closed
        digitalWrite(doorLock, LOW);
        
        //set lightsOn to 0 for next time that button is depressed
        lightsOn = 0;

        //tell the PI the door is unlocked/open
        digitalWrite(DoorClosedPiOutput, LOW);        
      }
    }  //end previous state is low
  }//end if button depressed

  //if the switch is not pressed, turn off the light and unlock the door
  if(doorButtonState == LOW /*|| bookshelfDoorButtonState == LOW*/)
  {
    if(previousButtonState == HIGH)
      ++previousButtonStateHigh;

    if(previousButtonStateHigh > 2 && previousButtonState == HIGH)
    { 
      //lights off when door is open
      digitalWrite(lightSwitch, LOW);    
      //ensure door magnetic lock is off when door is open
      digitalWrite(doorLock, LOW);
      digitalWrite(indicator, LOW);  //turn off the led indicator on the arduino     
      previousButtonState = LOW;   
      previousButtonStateLow = 0; //reset counter for next iteration
      previousButtonStateHigh = 0;
      //tell the PI the door is unlocked/open
      digitalWrite(DoorClosedPiOutput, LOW);        
    }
  }

  delay(1);        // delay in between reads for stability
}

/*
  //if front door is closed and bookshelf door is closed, lock the front door
  if(doorButtonState == HIGH  && bookshelfDoorButtonState == HIGH)
  {
    if(previousButtonState == LOW)
    {
      digitalWrite(indicator, HIGH);    
      ++numDoorButtonPushes;

  
      if(lightsOn == 0) //if lights are off, turn them on and lock the door
      {    
        //turn on lights when door is closed
        digitalWrite(lightSwitch, HIGH);    
        //lock door magnetic lock when it is closed
        digitalWrite(doorLock, HIGH);
        lightsOn = 1;
      }
      else if(lightsOn == 1)
      {
        //turn on lights when door is closed
        digitalWrite(lightSwitch, HIGH);    
        //lock door magnetic lock when it is closed
        digitalWrite(doorLock, HIGH);
        lightsOn = 0;
      }
    }
  }
  //if either door is open, unlock the door, turn off the lights
  else if(doorButtonState == LOW || bookshelfDoorButtonState == LOW)
  {
    //lights off when door is open
    digitalWrite(lightSwitch, LOW);    
    //ensure door magnetic lock is off when door is open
    digitalWrite(doorLock, LOW);

    digitalWrite(indicator, LOW);        
  }
  
  previousButtonState = doorButtonState;

  if(resetButtonState == HIGH)
    setup();


  delay(1);        // delay in between reads for stability
}
*/
