#include <Servo.h>//library to operate servo motor

#define segA 2//assign segment A to Digital pin 2
#define segB 3//assign segment B to Digital pin 3
#define segC 4//assign segment C to Digital pin 4
#define segD 5//assign segment D to Digital pin 5
#define segE 6//assign segment E to Digital pin 6
#define segF 7//assign segment F to Digital pin 7
#define segG 8//assign segment G to Digital pin 8
#define D1_left 13;//assign D1 or left display to Digital pin 13
#define D2_right 12;//assign D2 or right display to Digital pin 12


Servo SMe,SMx;//define variables to control servo SMe and SMx

int design [10][7]//array to store the numbers from 0 to 10
{
{1,1,1,1,1,1,0},//digit 0 pattern
{0,1,1,0,0,0,0},//digit 1 pattern
{1,1,0,1,1,0,1},//digit 2 pattern
{1,1,1,1,0,0,1},//digit 3 pattern
{0,1,1,0,0,1,1},//digit 4 pattern
{1,0,1,1,0,1,1},//digit 5 pattern
{1,0,1,1,1,1,1},//digit 6 pattern
{1,1,1,0,0,0,0},//digit 7 pattern
{1,1,1,1,1,1,1},//digit 8 pattern
{1,1,1,1,0,1,1}//digit 9 pattern
};

void display_digit(int d)//function to display the pattern on both the 7 segment display where d is the number to be displayed 
{
  for (int i=0;i<7;i++)//loop 7 times to go through all the index in array d
  {
        digitalWrite(i+2,design[d][i]);//light up segment according to the pattern of the array and plus 2 at the counter i as the segment starts at Digital pin 2 until Digital pin 8
  }
}

void determine_numbers(int one,int two,int ctr)//function to execute multiplexing between the two 7-Segment Display where one is the first digit, two is the second digit and ctr is the current value of the counter
{
    if (ctr==0)//if counter is 0
  {
        display_digit(0);//display the digit 00 on both the 7-Segment Displays
      digitalWrite(13,HIGH);//turn off the left display  as per required by the question
    digitalWrite(12,HIGH);//turn off the right display  as per required by the question
        delay(10);//delay for 10 milliseconds
    }
    else//if the counter is not 0
  {
        digitalWrite(13,LOW);//turn on the left display
        digitalWrite(12,HIGH);//turn off the right display
        display_digit(one);//display the first number of the counter
        delay(10);//delay for 10 milliseconds
        digitalWrite(13,HIGH);//turn off the left display 
        digitalWrite(12,LOW);//turn on the right display
        display_digit(two);//display the second number of the counter
        delay(10);//delay for 10 milliseconds
    }
}

int FSe=A0;//assign FSe to Analog pin 0 at the entrance
int FSx=A1;//assign FSx to Analog pin 1 at the exit
int loadSensorInPin0=0;//variable to store Analog pin 0 value
int loadSensorOutPin1=0;//variable to store Analog pin 1 value
int lowerLimit=748;//declare the lowerlimit to trigger the servo motor
int upperLimit=855;//declare the upperlimit to trigger the servo motor
int Cntr=20;//declare counter to count the number of vehicle entering into the parking lot starting from 20
int firstNum=0;//variable to store the counter's first number
int secondNum=0;//variable to store the counter's second number
String indexLength="";//variable to store the counter's value in string format
unsigned long previousMillsA0=0;//variable to store the old value of A0 for error handling to create a buffer when a vehicle is on the force sensor
unsigned long previousMillsA1=0;//variable to store the old value of A1 for error handling to create a buffer when a vehicle is on the force sensor
unsigned long previousMillsSMe=0;//variable to store the old value of SMe which will be used to count up to 15 second to close the survo
unsigned long previousMillsSMx=0;//variable to store the old value of SMx which will be used to count up to 15 second to close the survo
int SMeopenCtr=0;//variable to control the open and close condition of SMe
int SMxopenCtr=0;//variable to control the open and close condition of SMx

void setup()//function to setup the pins
{
  SMe.attach(11);//assign survo motor entrance to Digital pin 11
  SMx.attach(10);//assign survo motor exit to Digital pin 10
  SMe.write(0);//set the default angle of SMe to 0 degree
  SMx.write(0);//set the default angle of SMx to 0 degree
  for(int i=2;i<14;i++)//loop 12 times to activate all the Digital pins as OUTPUT from Digital pin 2 to pin 13
  { 
    if (i!=11||i!=10)//skip the activation of Digital pin 11 and 10 as it is the pin for the servo motors
  {   
      pinMode(i,OUTPUT);//activate the Digital pin at index i as OUTPUT
    } 
  }
}

void loop() //main function
{
  unsigned long currentMillisA0 = millis();//assign the current active time of arduino to currentMillisA0 which will be used as a buffer counter to validate the weight of the vehicle
  unsigned long currentMillisA1 = millis();//assign the current active time of arduino to currentMillisA1 which will be used as a buffer counter to validate the weight of the vehicle
  unsigned long currentMillisSMe = millis();//assign the current active time of arduino to currentMillisSMe which will be used as a buffer counter to open SMe for 15 seconds
  unsigned long currentMillisSMx = millis();//assign the current active time of arduino to currentMillisSMx which will be used as a buffer counter to open SMx for 15 seconds
  loadSensorInPin0=analogRead(FSe);//assign and read the analog value from pin 0 to load loadSensorInPin0
  loadSensorOutPin1=analogRead(FSx);//assign and read the analog value from pin 1 to load loadSensorOutPin1
  indexLength=String(Cntr);//pass the current counter value to the string 
  if (indexLength.length()==2)//if the length of the string is equals to 2
  {
    firstNum=indexLength[0]-'0';//grab the first index of the string and convert it to integer
    secondNum=indexLength[1]-'0';//grab the second index of the string and convert it to integer
  }
  else if (indexLength.length()==1)//if the length of the string is equals to 1
  {         
    firstNum=0;//set the default first value to 0
    secondNum=indexLength[0]-'0';//grab the only value in the string as the second value by converting it to integer
  }
  determine_numbers(firstNum,secondNum,Cntr);//pass the first number, second number and the current counter to the function to be displayed at the 7-Segment Display
  if (currentMillisA0-previousMillsA0>=2000)//if the subtraction of the current runtime with the initial runtime is more than 2 seconds to finalize the weight of the vehicle
  {
    int comparePin0=analogRead(FSe);//assign and read the current value of Analog pin 0
    if (loadSensorInPin0>=lowerLimit && loadSensorInPin0<=upperLimit && Cntr!=0 && comparePin0==loadSensorInPin0)//if A0 force sensor value is between the upper and lower limit and the counter is not 0 and the current value of the pin is the same as the previous value 
    {   
      if (SMeopenCtr==0){//if SMeopenCtr is 0 which is used to indicate that the entrance to the parking lot has no car to avoid traffic congestion
        previousMillsSMe=currentMillisSMe;//assign previousMillsSMe as currentMillisSMe to start counting 15 seconds which is the interval to allow a vehicle to enter the parking
        SMeopenCtr=SMeopenCtr+1;//increase the counter by 1 to indicate that a vehicle is currently entering the parking lot
        SMe.write(90);//rotate the SMe at 90 degree to open the gate to the parking to allow a vehicle to enter
      }  
    }
    previousMillsA0=currentMillisA0;//assign the previous runtime of the initial counter of Analog pin A0 to match with the updated runtime of the arduino
  }
  if(currentMillisSMe-previousMillsSMe>=15000 && SMeopenCtr==1){//if the subtraction of the currentMillisSMe and previousMillsSMe is more or equal to 15 second and if the SMeopenCtr is 1
  SMe.write(0);//SMe will close at 0 degree to close the gate
    Cntr=Cntr-1;//decrease the counter by 1 to indicate that a parking space has been occupied
    SMeopenCtr=0;//reset SMeopenCtr to 0 to simulate that a vehicle has now enter the parking lot
  }
  if (currentMillisA1-previousMillsA1>=2000)//if the subtraction of the current runtime with the initial runtime is more than 2 seconds to finalize the weight of the vehicle
  {
    int comparePin1=analogRead(FSx);//assign and read the current value of Analog pin 1
    if (loadSensorOutPin1>=lowerLimit && loadSensorOutPin1<=upperLimit && Cntr!=20 && comparePin1==loadSensorOutPin1)//if A1 force sensor value is between the upper and lower limit and the counter is not 20 and the current value of the pin is the same as the previous value 
    {   
      if (SMxopenCtr==0){//if SMxopenCtr is 0 which is used to indicate that the exit to the parking lot has no car to avoid traffic congestion
        previousMillsSMx=currentMillisSMx;//assign previousMillsSMx as currentMillisSMx to start counting 15 seconds which is the interval to allow a vehicle to exit the parking
        SMxopenCtr=SMxopenCtr+1;//increase the counter by 1 to indicate that a vehicle is currently exiting the parking lot
        SMx.write(90);//rotate the SMe at 90 degree to open the gate of the exit to allow a vehicle to exit the parking 
      }  
    }
    previousMillsA1=currentMillisA1;//assign the previous runtime of the initial counter of Analog pin A1 to match with the updated runtime of the arduino
  }
  if(currentMillisSMx-previousMillsSMx>=15000 && SMxopenCtr==1){//if the subtraction of the currentMillisSMx and previousMillsSMx is more or equal to 15 second and if the SMxopenCtr is 1
  SMx.write(0);//SMx will close at 0 degree to close the gate
    Cntr=Cntr+1;//increase the counter by 1 to indicate that an empty space is available after a vehicle has exit the parking lot
    SMxopenCtr=0;//reset SMxopenCtr to 0 to simulate that a vehicle has can now exit the parking lot
  }
  determine_numbers(firstNum,secondNum,Cntr);//pass the first number, second number and the current counter to the function to be displayed at the 7-Segment Display
}
