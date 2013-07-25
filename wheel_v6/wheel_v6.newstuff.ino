/*
Library for controlling the motors. 
NEW April 2:Correction works. 
			Line sensor, if we go full speed at the line, there is no way we will make it go back
New: With 2 external new interrupts, and angle


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

void lightSensor();

#include "Arduino.h"
//////////////////////////Modes///////////////////
	#define move_forward		1
	#define move_reverse		2
	#define move_stop			3
	#define move_rotateRight    4
	#define move_rotateLeft     5

//////////////////////////Wheel Globals/////////////////////////
	int counterL = 0, counterR = 0;   //store the value from encoder, 304 per rotation
	int angle =0;                     //possitive = turn right, negative = turn left
	int common_angle = 0;		      //clockwise +360, counterclockwise -360
	double x_axis = 1219, y_axis = 0;   //x, y postion 2438 mm max 
	double x_axis_temp =1219, y_axis_temp = 0; //temp values for x & y, the finalized temp values will be added to the overall x_axis, y_axis
	float displacement = 0;				//turning displacement
	float distanceL = 0, distanceR = 0; //distance of each wheel travelled in mm. Can be negative.

	int i =0; //Remove this when you comment out my setup and loop functions

	int light_sensor_FL = 0, light_sensor_FR = 0, light_sensor_BL = 0, light_sensor_BR = 0;
	int int_light_sensor_FL = 0, int_light_sensor_FR = 0, int_light_sensor_BL = 0, int_light_sensor_BR = 0; //initial reading as reference points
	int ls_flag = 0;//light sensor flag

/****************************************Convert my angle to GUI's angle*************************************************/

void convert(){
	common_angle = angle;
	
	if(common_angle >=0 ){
		while(common_angle > 360)
		common_angle = common_angle - 360;
	}else{
		while(common_angle < 0)
		common_angle = common_angle + 360;
	}
}
/****************************************turn, forward, backward function*************************************************/

void turn(int degree, int value){
	/*
	Dont give over 130, 130 is fast enough
	When reaching 60% of the angle, speed goes down to speed 55
	When given degree is <60 degree turn, it turns at speed 80
	*/
	int init_angle = angle;  //initial position of angle 
	int value1;
	value1 = value;
	if(degree < 60 || degree > 300)
		value1 = 80;

	if (degree <= 180){
		while(angle  <= init_angle + degree){    //get the relative "final angle "

			if(angle > (0.6*degree+init_angle) && degree >= 60) //Slow down when reaching 60% of the distance to prevent over shooting
				value1 = 55; 

			analogWrite(BIT_L_1, value1);//rotate right
			analogWrite(BIT_L_2, 0);
			analogWrite(BIT_R_1, 0);
			analogWrite(BIT_R_2, value1);
		}
		while(angle > init_angle + degree){
			analogWrite(BIT_L_1, 0);//rotate left
			analogWrite(BIT_L_2, 50);
			analogWrite(BIT_R_1, 50);
			analogWrite(BIT_R_2, 0);
		}
	}else if (degree> 180){
		degree = 360 - degree;
		while(angle >= init_angle - degree){

			if(angle < (init_angle - 0.6*degree) && degree <= 300 ) //Slow down when reaching 70% of the distance to prevent over shooting
				value1 = 55; 

			analogWrite(BIT_L_1, 0);//rotate left
			analogWrite(BIT_L_2, value1-20);
			analogWrite(BIT_R_1, value1-20);
			analogWrite(BIT_R_2, 0);
		}
		while(angle < init_angle - degree){
			analogWrite(BIT_L_1, 65);//rotate right
			analogWrite(BIT_L_2, 0);
			analogWrite(BIT_R_1, 0);
			analogWrite(BIT_R_2, 65);
		}
	}

			analogWrite(BIT_L_1, 0);//assign stop otherwise it will keep turing L/R
			analogWrite(BIT_L_2, 0);
			analogWrite(BIT_R_1, 0);
			analogWrite(BIT_R_2, 0);
}

void forward(int distance, int value){
	//Max Value 100
	int init_distanceL = distanceL;//get the instant value
	int init_distanceR = distanceR;//get the instant value (not used)
	int init_angle = angle;//get the instant value
	int init_x_axis = x_axis, init_y_axis = y_axis;
	int value1, value2; 
	
	while(distanceL <= (distance + init_distanceL)){    //in this case distanceL and distanceR are supposed to be the same, so only use one value
		
		if(distanceL > (0.6*distance+init_distanceL)){  //Slow down when reaching 60% of the distance to prevent over shooting
			value = 55; 
		}

		if((angle <= init_angle+2 && angle >= init_angle-2)){
			value1 = value;
			value2 = value;
		}else if(angle > init_angle+2){ 
			value1 = value/2;
			value2 = value;
		}else if(angle < init_angle-2){               
			value1 = value;
			value2 = value/2;
		}
		analogWrite(BIT_L_1, value1);
		analogWrite(BIT_L_2, 0);
		analogWrite(BIT_R_1, value2);
		analogWrite(BIT_R_2, 0);

		x_axis_temp = (distanceL-init_distanceL) * sin(angle * 3.14159 / 180 );   //temp value will continue to update 
		y_axis_temp = (distanceL-init_distanceL) * cos(angle * 3.14159 / 180 ); 
		x_axis = init_x_axis + x_axis_temp;                                       //final value will be the init value + temp value
		y_axis = init_x_axis + y_axis_temp;

//		lightSensor();
//		if( ls_flag == 1)
//			break;
//	Serial.write("\nX: ");
//	Serial.print(value1);
//	Serial.write("  Y: ");
//	Serial.print(value2);
//
//	Serial.write("  X: ");
//	Serial.print(x_axis);
//	Serial.write("  Y: ");
	// Serial.print(y_axis);
	}
	while(distanceL > (distance + init_distanceL)){
		analogWrite(BIT_L_1, 0);
		analogWrite(BIT_L_2, 55);
		analogWrite(BIT_R_1, 0);
		analogWrite(BIT_R_2, 55);
	}
		analogWrite(BIT_L_1, 0);//assign stop otherwise it will keep going forward
		analogWrite(BIT_L_2, 0);
		analogWrite(BIT_R_1, 0);
		analogWrite(BIT_R_2, 0);
}

void backward(int distance, int value){
	//Max Value 100
	int init_distanceL = distanceL;//get the instant value
	int init_distanceR = distanceR;//get the instant value (not used)
	int init_angle = angle;//get the instant value
	int init_x_axis = x_axis, init_y_axis = y_axis;
	int value1, value2; 

	while(distanceL >= (init_distanceL - distance)){    //in this case distanceL and distanceR are supposed to be the same

		if(distanceL < (init_distanceL - 0.7*distance)){  //Slow down when reaching 70% of the distance to prevent over shooting
			value = 50; 
		}

		if((angle <= init_angle+2 && angle >= init_angle-2)){
			value1 = value;
			value2 = value;
		}else if(angle > init_angle+2){
			value1 = value;
			value2 = value/2;
		}else if(angle < init_angle-2){
			value1 = value/2;
			value2 = value;
		}
		analogWrite(BIT_L_1, 0);
		analogWrite(BIT_L_2, value1);
		analogWrite(BIT_R_1, 0);
		analogWrite(BIT_R_2, value2);

		x_axis_temp = (distanceL-init_distanceL) * sin(angle * 3.14159 / 180 );   //temp value will continue to update 
		y_axis_temp = (distanceL-init_distanceL) * cos(angle * 3.14159 / 180 ); 
		x_axis = init_x_axis - x_axis_temp; //backward so minus
		y_axis = init_x_axis - y_axis_temp;
	}
	while(distanceL < (init_distanceL - distance)){
		analogWrite(BIT_L_1, 55);
		analogWrite(BIT_L_2, 0);
		analogWrite(BIT_R_1, 55);
		analogWrite(BIT_R_2, 0);
	}
		analogWrite(BIT_L_1, 0);//assign stop otherwise it will keep going forward
		analogWrite(BIT_L_2, 0);
		analogWrite(BIT_R_1, 0);
		analogWrite(BIT_R_2, 0);
}
/****************************************Manual Control*************************************************/
void motorManualControl(int command){
	switch(command){
	case 1:
		forward(10,55);
		break;
	case 2:
		backward(10,55);
		break;
	case 3:
			analogWrite(BIT_L_1, 0);
			analogWrite(BIT_L_2, 0);
			analogWrite(BIT_R_1, 0);
			analogWrite(BIT_R_2, 0);
		break;
	case 4:
			analogWrite(BIT_L_1, 55);//rotate left
			analogWrite(BIT_L_2, 0);
			analogWrite(BIT_R_1, 0);
			analogWrite(BIT_R_2, 55);
		break;
	case 5:
			analogWrite(BIT_L_1, 0);//rotate left
			analogWrite(BIT_L_2, 55);
			analogWrite(BIT_R_1, 55);
			analogWrite(BIT_R_2, 0);
		break;
	}

}



/****************************************Encoder ISR*************************************************/
void encoderStateL(){
	counterL++; //test
	if(digitalRead(2) == digitalRead(3))
		distanceL = distanceL + 0.472;

	else 
		distanceL = distanceL - 0.472;

	if(distanceL >= distanceR){ //singalling turn Right 
		displacement = distanceL - distanceR; //displacmente is difference between distanceL and distanceR, the curve on the circle
		angle = displacement / (2 * 538.6256) * 360; //circumference = 538.6256mm
	}
}
void encoderStateR(){
	counterR++; //test
	if(digitalRead(20) == digitalRead(21))
		distanceR = distanceR + 0.472;
	
	else
		distanceR = distanceR - 0.472;
	
	if(distanceR >= distanceL){ //singalling turn Left 
		displacement = distanceR - distanceL;
		angle = -1 * displacement / (2 * 538.6256) * 360; //circumference = 538.6256mm
	}
}

/****************************************LIGHT SENSOR*************************************************/
void lightSensor(){
	ls_flag = 0;
		//int flag = 0;
	 light_sensor_FL = analogRead(9);
	 light_sensor_FR = analogRead(8);
	 light_sensor_BL = analogRead(11);
	 light_sensor_BR = analogRead(10);
	//Front Sensors
	if(light_sensor_FL > int_light_sensor_FL + 130 && light_sensor_FR > int_light_sensor_FR + 130 ){
		backward(150, 70);
		turn(180, 90);
		ls_flag = 1;
	}
	else if(light_sensor_FL > int_light_sensor_FL + 120){
		backward(150, 70);
		turn(115,90);
		ls_flag = 1;
	}
	else if(light_sensor_FR > int_light_sensor_FR + 120){
		backward(150, 70);
		turn(245,90);
		ls_flag = 1;
	}

	//Back Sensors
	if(light_sensor_BL > int_light_sensor_BL + 120 && light_sensor_BR > int_light_sensor_BR + 120 ){
		forward(150, 70);
		ls_flag = 1;
	}
	else if(light_sensor_BL > int_light_sensor_BL + 120){
		turn(45,90);
		forward(150, 70);
		ls_flag = 1;
	}
	else if(light_sensor_BR > int_light_sensor_BR + 120){
		turn(315,90);
		forward(150, 70);
		ls_flag = 1;
	}

}
