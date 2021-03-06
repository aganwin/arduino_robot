/*
Library for controlling the motors. New: With 2 external new interrupts, and angle

Input: Instruction: move_forward, move_reverse, move_stop, move_rotateRight, move_rotateLeft
Output: Distance travelled in cm and angle turned 
Each circumference is 28.1 cm
XXProblem 1: How often does the motor recieve the signal? 
	If the signal is recieved too ofter 
	= constantly set the bit between 1 and 0 
	= consume resource
XXProblem 2: How much distance to cover per signal?
XXProblem 3: If the signal isnt being recieved too frequent, need set back

NOTE: digitalRead() returns HIGH/LOW not 1 and 0

NOTE: 19*64 = 1216 transition per wheel revolution, I am only using 1/4 which is 304
NEW Problem: If Transition happens before an ISR finishes = Encoder Stops and fucks up
NEW Problem: Encoder too close together, interference
NEW CHANGE: Implementation of Encoder. No longer be able to tell whether spining forward or backward. 
	Only useing 0 and 1 intead of the four transition. 
NEW CHANGE: Interrupt ISR is triggered by RISING or FALLING so only 1/4 of the transition is recoreded. 304 transitions. This is enough.
NEW CHANGE: PWM to change speed
NEW Problem: Encoders work when turning the wheel by hand. When connect to the power source, too much noise.

SOLUTION: Inconsistancy of 9 v from the h Bridge

Two ways to implement direction
	1. Use four interrupts
*/

#include "Arduino.h"

// HENRY: The pin configs are here...
#include "C:\Arduino\src\final\config_h\config_h.ino"

//////////////////////////Modes///////////////////
#define move_forward      1
#define move_reverse      2
#define move_stop         3
#define move_rotateRight  4
#define move_rotateLeft   5
#define move_turn         6
//////////////////////////Pins//////////////////////

/*
#define LIGHT_SENSOR1 10
#define LIGHT_SENSOR2 11
*/

////////////////////////////Encoder//////////////////
int counterL = 0, counterR = 0;   //store the value from encoder, 304 per rotation
int angle =0;                     //Possitive = turn left, negative = turn right
double x_axis = 1219, y_axis = 0;    //x, y postion 2438 mm max 
int rotation = 0; //number of robot rotation


float displacement = 0; //turning displacement
float distanceL = 0, distanceR = 0; //in mm
int command;
int light_sensor1 = 0, light_sensor2 = 0;
int ini_light_sensor1 = 0, ini_light_sensor2 = 0;

int i =0 ;

/*
void setup()
{
  pinMode(PIN_ENABLE_LEFT_MOTOR, OUTPUT);
  pinMode(PIN_ENABLE_RIGHT_MOTOR, OUTPUT);
  pinMode(PIN_TTL_LEFT_0, OUTPUT);
  pinMode(PIN_TTL_LEFT_1, OUTPUT);
  pinMode(PIN_TTL_RIGHT_0, OUTPUT);
  pinMode(PIN_TTL_RIGHT_1, OUTPUT);

  //Enable motors
  digitalWrite(PIN_ENABLE_LEFT_MOTOR, HIGH);
  digitalWrite(PIN_ENABLE_RIGHT_MOTOR, HIGH);

  ini_light_sensor1 = analogRead(LIGHT_SENSOR1);//set the initial value of LEft light sensor
  ini_light_sensor2 = analogRead(LIGHT_SENSOR2);//set the initial value of Right light sensor

  Serial.begin(4800);

  attachInterrupt(0, encoderStateL, CHANGE); //pin2
  attachInterrupt(2, encoderStateR, CHANGE); //pin21
}

void loop()
{	
	while(i<1){
	
	//turn(181, 60); //Usage: turn(angle, turn speed)   relative angle right turn: 0~180 left turn 181~360
	
	//forward(200, 60); //Usage: forward(distance in mm, speed)
	backward(200, 60); //Usage: backward(distance in mm, speed)
	i++;
	}	
	
	Serial.write("\n Angle ");
	Serial.print(angle);

}
*/
/****************************************turn, forward, backward function*************************************************/
void turn(int degree, int value){
	int init_angle = angle;  //initial position of angle 
	if (degree <= 180){
		while(angle  <= init_angle + degree){
			analogWrite(PIN_ENCODER_OUT_LEFT_0, value);//rotate right
			analogWrite(PIN_ENCODER_OUT_LEFT_1, 0);
			analogWrite(PIN_ENCODER_OUT_RIGHT_0, 0);
			analogWrite(PIN_ENCODER_OUT_RIGHT_1, value);
		}
	}else if (degree> 180){
		degree = 360 - degree;
		while(angle >= init_angle - degree){
			analogWrite(PIN_ENCODER_OUT_LEFT_0, 0);//rotate left
			analogWrite(PIN_ENCODER_OUT_LEFT_1, value);
			analogWrite(PIN_ENCODER_OUT_RIGHT_0, value);
			analogWrite(PIN_ENCODER_OUT_RIGHT_1, 0);
		}
	}
			analogWrite(PIN_ENCODER_OUT_LEFT_0, 0);//assign stop otherwise it will keep turing L/R
			analogWrite(PIN_ENCODER_OUT_LEFT_1, 0);
			analogWrite(PIN_ENCODER_OUT_RIGHT_0, 0);
			analogWrite(PIN_ENCODER_OUT_RIGHT_1, 0);
}
void forward(int distance, int value){
	int init_distanceL = distanceL;
	int init_distanceR = distanceR;
	int init_angle = angle;
	int value1, value2; 

	while(distanceL <= (distance + init_distanceL)){    //in this case distanceL and distanceR are supposed to be the same

		if((angle <= init_angle+2 && angle >= init_angle-2)){
			value1 = value;
			value2 = value;
		}else if(angle > init_angle+2){
			value1 = 0;
			value2 = value;
		}else if(angle < init_angle-2){
			value1 = value;
			value2 = 0;
		}
		analogWrite(PIN_ENCODER_OUT_LEFT_0, value1);
		analogWrite(PIN_ENCODER_OUT_LEFT_1, 0);
		analogWrite(PIN_ENCODER_OUT_RIGHT_0, value2);
		analogWrite(PIN_ENCODER_OUT_RIGHT_1, 0);
	}
		analogWrite(PIN_ENCODER_OUT_LEFT_0, 0);//assign stop otherwise it will keep going forward
		analogWrite(PIN_ENCODER_OUT_LEFT_1, 0);
		analogWrite(PIN_ENCODER_OUT_RIGHT_0, 0);
		analogWrite(PIN_ENCODER_OUT_RIGHT_1, 0);
}

void backward(int distance, int value){
	int init_distanceL = distanceL;
	int init_distanceR = distanceR;
	int init_angle = angle;
	int value1, value2; 

	
	while(distanceL <= (init_distanceL + distance)){    //in this case distanceL and distanceR are supposed to be the same

		if((angle <= init_angle+2 && angle >= init_angle-2)){
			value1 = value;
			value2 = value;
		}else if(angle > init_angle+2){
			value1 = value;
			value2 = 0;
		}else if(angle < init_angle-2){
			value1 = 0;
			value2 = value;
		}
		analogWrite(PIN_TTL_LEFT_0, 0);
		analogWrite(PIN_TTL_LEFT_1, value1);
		analogWrite(PIN_TTL_RIGHT_0, 0);
		analogWrite(PIN_TTL_RIGHT_1, value1);
	}
		analogWrite(PIN_TTL_LEFT_0, 0);//assign stop otherwise it will keep going forward
		analogWrite(PIN_TTL_LEFT_1, 0);
		analogWrite(PIN_TTL_RIGHT_0, 0);
		analogWrite(PIN_TTL_RIGHT_1, 0);

}
/****************************************ISR*************************************************/
void encoderStateL(){
	//counterL++; //test
	if(digitalRead(PIN_ENCODER_OUT_LEFT_0) == digitalRead(PIN_ENCODER_OUT_LEFT_1)){
		distanceL = distanceL + 0.462;
		if(angle > 0){
			x_axis = x_axis - 0.462 * cos((90 - angle )* 3.1415 / 180);
			y_axis = y_axis + 0.462 * sin((90 - angle )* 3.1415 / 180);
		}
		if(angle <= 0){
			x_axis = x_axis + 0.462 * cos((90 + angle )* 3.1415 / 180);
			y_axis = y_axis + 0.462 * sin((90 + angle )* 3.1415 / 180);
		}
	}
	else {
		distanceL = distanceL - 0.462;
		if(angle > 0){
			x_axis = x_axis - 0.462 * cos((90 - angle )* 3.1415 / 180);
			y_axis = y_axis - 0.462 * sin((90 - angle )* 3.1415 / 180);
		}
		if(angle <= 0){
			x_axis = x_axis + 0.462 * cos((90 + angle )* 3.1415 / 180);
			y_axis = y_axis - 0.462 * sin((90 + angle )* 3.1415 / 180);
		}
	}


	if(distanceL >= distanceR){ //singalling turn Right 
		displacement = distanceL - distanceR; //displacmente is difference between distanceL and distanceR, the curve on the circle
		angle = displacement / (2 * 538.6256) * 360; //circumference = 538.6256mm
	}


}
void encoderStateR(){

	if(digitalRead(PIN_ENCODER_OUT_RIGHT_1) == digitalRead(PIN_ENCODER_OUT_RIGHT_0))
		distanceR = distanceR + 0.462;
	
	else
		distanceR = distanceR - 0.462;
	
	if(distanceR >= distanceL){ //singalling turn Left 
		displacement = distanceR - distanceL;
		angle = -1 * displacement / (2 * 538.6256) * 360; //circumference = 538.6256mm
	}
}
