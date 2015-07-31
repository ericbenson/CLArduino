
/*
  Writing to Serial Port
  Writes a digital input on pin 2 into the serial

  This example code is in the public domain
  */

//digital pin 2 has a pushbutton attached to it
int pushButton = 2;

//the setup routine runs once you press reset:
void setup(){
  //initialize serial communcation at 9600 bits per second
  Serial.begin(9600);
  //make pushbutton's pin as input:
  pinMode(pushButton, INPUT_PULLUP);
}

//the loop routine runs over and over again forever:
void loop(){
  //read the input pin:
  int buttonState = digitalRead(pushButton);
  //print out the state of teh button into the serial port:
  if(buttonState == HIGH){
    Serial.write(1);
  } else {
    Serial.write(0);
  }

  //delay in between reads for stability
  delay(1000);
}
