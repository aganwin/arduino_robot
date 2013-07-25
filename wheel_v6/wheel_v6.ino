#include "Arduino.h"
//////////////////////////Modes///////////////////
#define move_forward 1
#define move_reverse 2
#define move_stop 3
#define move_rotateRight 4 
#define move_rotateLeft 5
/////////////////////////Sin/Cos lookup table/////////////

float COS_TABLE[] = { 1,
0.999848, 0.999391, 0.99863, 0.997564, 0.996195, 0.994522, 0.992546, 0.990268, 0.987688, 0.984808,
0.981627, 0.978148, 0.97437, 0.970296, 0.965926, 0.961262, 0.956305, 0.951057, 0.945519, 0.939693,
0.93358, 0.927184, 0.920505, 0.913545, 0.906308, 0.898794, 0.891007, 0.882948, 0.87462, 0.866025,
0.857167, 0.848048, 0.838671, 0.829038, 0.819152, 0.809017, 0.798636, 0.788011,0.777146, 0.766044,
0.75471, 0.743145, 0.731354, 0.71934, 0.707107, 0.694658, 0.681998, 0.669131, 0.656059, 0.642788,
0.62932, 0.615662, 0.601815, 0.587785, 0.573576, 0.559193, 0.544639, 0.529919, 0.515038, 0.5,
0.48481, 0.469472, 0.453991, 0.438371, 0.422618, 0.406737, 0.390731, 0.374607, 0.358368, 0.34202,
0.325568, 0.309017, 0.292372, 0.275637, 0.258819, 0.241922, 0.224951, 0.207912,0.190809, 0.173648,
0.156434, 0.139173, 0.121869, 0.104528, 0.0871558, 0.0697565, 0.052336, 0.0348995, 0.0174524, -4.37114e-008,
-0.0174524, -0.0348995, -0.0523359, -0.0697565, -0.0871558, -0.104529, -0.121869, -0.139173, -0.156434, -0.173648,
-0.190809, -0.207912, -0.224951, -0.241922, -0.258819, -0.275637, -0.292372, -0.309017, -0.325568, -0.34202,
-0.358368, -0.374607, -0.390731, -0.406737, -0.422618, -0.438371, -0.453991, -0.469472, -0.48481, -0.5,
-0.515038, -0.529919, -0.544639, -0.559193, -0.573576, -0.587785, -0.601815, -0.615661, -0.62932, -0.642788,
-0.656059, -0.669131, -0.681998, -0.694658, -0.707107, -0.71934, -0.731354, -0.743145, -0.75471, -0.766044,
-0.777146, -0.788011, -0.798635, -0.809017, -0.819152, -0.829038, -0.838671, -0.848048, -0.857167, -0.866025,
-0.87462, -0.882948, -0.891006, -0.898794, -0.906308, -0.913545, -0.920505, -0.927184, -0.93358, -0.939693,
-0.945519, -0.951056, -0.956305, -0.961262, -0.965926, -0.970296, -0.97437, -0.978148, -0.981627, -0.984808,
-0.987688, -0.990268, -0.992546, -0.994522, -0.996195, -0.997564, -0.99863, -0.999391, -0.999848, -1,
-0.999848, -0.999391, -0.99863, -0.997564, -0.996195, -0.994522, -0.992546, -0.990268, -0.987688, -0.984808,
-0.981627, -0.978148, -0.97437, -0.970296, -0.965926, -0.961262, -0.956305, -0.951057, -0.945519, -0.939693,
-0.93358, -0.927184, -0.920505, -0.913545, -0.906308, -0.898794, -0.891007, -0.882948, -0.87462, -0.866025,
-0.857167, -0.848048, -0.838671, -0.829038, -0.819152, -0.809017, -0.798635, -0.788011, -0.777146, -0.766044,
-0.75471, -0.743145, -0.731354, -0.71934, -0.707107, -0.694658, -0.681998, -0.669131, -0.656059, -0.642788,
-0.62932, -0.615662, -0.601815, -0.587785, -0.573576, -0.559193, -0.544639, -0.529919, -0.515038, -0.5,
-0.48481, -0.469472, -0.453991, -0.438371, -0.422618, -0.406737, -0.390731, -0.374607, -0.358368, -0.34202,
-0.325568, -0.309017, -0.292372, -0.275637, -0.258819, -0.241922, -0.224951, -0.207912, -0.190809, -0.173648,
-0.156435, -0.139173, -0.121869, -0.104528, -0.0871557, -0.0697565, -0.0523361,-0.0348993, -0.0174523, 1.19249e-008,
0.0174523, 0.0348993, 0.0523361, 0.0697566, 0.0871557, 0.104528, 0.121869, 0.139173, 0.156435, 0.173648,
0.190809, 0.207911, 0.224951, 0.241922, 0.258819, 0.275637, 0.292371, 0.309017,0.325568, 0.34202,
0.358368, 0.374607, 0.390731, 0.406737, 0.422618, 0.438371, 0.453991, 0.469472,0.48481, 0.5,
0.515038, 0.529919, 0.544639, 0.559193, 0.573576, 0.587785, 0.601815, 0.615662,0.62932, 0.642788,
0.656059, 0.669131, 0.681998, 0.694658, 0.707107, 0.71934, 0.731354, 0.743145, 0.75471, 0.766044,
0.777146, 0.788011, 0.798636, 0.809017, 0.819152, 0.829038, 0.838671, 0.848048,0.857167, 0.866025,
0.87462, 0.882948, 0.891007, 0.898794, 0.906308, 0.913546, 0.920505, 0.927184, 0.93358, 0.939693,
0.945519, 0.951057, 0.956305, 0.961262, 0.965926, 0.970296, 0.97437, 0.978148, 0.981627, 0.984808,
0.987688, 0.990268, 0.992546, 0.994522, 0.996195, 0.997564, 0.99863, 0.999391, 0.999848, 1
};



float SIN_TABLE[] = {0,
0.0174524, 0.0348995, 0.052336, 0.0697565, 0.0871557, 0.104528, 0.121869, 0.139173, 0.156434, 0.173648,
0.190809, 0.207912, 0.224951, 0.241922, 0.258819, 0.275637, 0.292372, 0.309017,0.325568, 0.34202,
0.358368, 0.374607, 0.390731, 0.406737, 0.422618, 0.438371, 0.453991, 0.469472,0.48481, 0.5,
0.515038, 0.529919, 0.544639, 0.559193, 0.573576, 0.587785, 0.601815, 0.615662,0.62932, 0.642788,
0.656059, 0.669131, 0.681998, 0.694658, 0.707107, 0.71934, 0.731354, 0.743145, 0.75471, 0.766044,
0.777146, 0.788011, 0.798636, 0.809017, 0.819152, 0.829038, 0.838671, 0.848048,0.857167, 0.866025,
0.87462, 0.882948, 0.891007, 0.898794, 0.906308, 0.913545, 0.920505, 0.927184, 0.93358, 0.939693,
0.945519, 0.951057, 0.956305, 0.961262, 0.965926, 0.970296, 0.97437, 0.978148, 0.981627, 0.984808,
0.987688, 0.990268, 0.992546, 0.994522, 0.996195, 0.997564, 0.99863, 0.999391, 0.999848, 1,
0.999848, 0.999391, 0.99863, 0.997564, 0.996195, 0.994522, 0.992546, 0.990268, 0.987688, 0.984808,
0.981627, 0.978148, 0.97437, 0.970296, 0.965926, 0.961262, 0.956305, 0.951056, 0.945519, 0.939693,
0.93358, 0.927184, 0.920505, 0.913545, 0.906308, 0.898794, 0.891006, 0.882948, 0.87462, 0.866025,
0.857167, 0.848048, 0.838671, 0.829038, 0.819152, 0.809017, 0.798635, 0.788011,0.777146, 0.766044,
0.75471, 0.743145, 0.731354, 0.71934, 0.707107, 0.694658, 0.681998, 0.669131, 0.656059, 0.642788,
0.629321, 0.615661, 0.601815, 0.587785, 0.573576, 0.559193, 0.544639, 0.529919,0.515038, 0.5,
0.48481, 0.469472, 0.453991, 0.438371, 0.422618, 0.406737, 0.390731, 0.374606, 0.358368, 0.34202,
0.325568, 0.309017, 0.292372, 0.275637, 0.258819, 0.241922, 0.224951, 0.207912,0.190809, 0.173648,
0.156434, 0.139173, 0.121869, 0.104528, 0.0871556, 0.0697565, 0.052336, 0.0348995, 0.0174525, -8.74228e-008,
-0.0174524, -0.0348994, -0.052336, -0.0697564, -0.0871558, -0.104528, -0.121869, -0.139173, -0.156434, -0.173648,
-0.190809, -0.207912, -0.224951, -0.241922, -0.258819, -0.275637, -0.292372, -0.309017, -0.325568, -0.34202,
-0.358368, -0.374607, -0.390731, -0.406737, -0.422618, -0.438371, -0.453991, -0.469472, -0.48481, -0.5,
-0.515038, -0.529919, -0.544639, -0.559193, -0.573576, -0.587785, -0.601815, -0.615661, -0.62932, -0.642788,
-0.656059, -0.669131, -0.681998, -0.694658, -0.707107, -0.71934, -0.731354, -0.743145, -0.75471, -0.766045,
-0.777146, -0.788011, -0.798635, -0.809017, -0.819152, -0.829038, -0.838671, -0.848048, -0.857167, -0.866025,
-0.87462, -0.882948, -0.891006, -0.898794, -0.906308, -0.913545, -0.920505, -0.927184, -0.933581, -0.939693,
-0.945519, -0.951056, -0.956305, -0.961262, -0.965926, -0.970296, -0.97437, -0.978148, -0.981627, -0.984808,
-0.987688, -0.990268, -0.992546, -0.994522, -0.996195, -0.997564, -0.99863, -0.999391, -0.999848, -1,
-0.999848, -0.999391, -0.99863, -0.997564, -0.996195, -0.994522, -0.992546, -0.990268, -0.987688, -0.984808,
-0.981627, -0.978148, -0.97437, -0.970296, -0.965926, -0.961262, -0.956305, -0.951056, -0.945519, -0.939693,
-0.93358, -0.927184, -0.920505, -0.913545, -0.906308, -0.898794, -0.891006, -0.882948, -0.87462, -0.866025,
-0.857167, -0.848048, -0.83867, -0.829038, -0.819152, -0.809017, -0.798635, -0.788011, -0.777146, -0.766044,
-0.75471, -0.743145, -0.731354, -0.71934, -0.707107, -0.694658, -0.681998, -0.669131, -0.656059, -0.642788,
-0.62932, -0.615661, -0.601815, -0.587785, -0.573577, -0.559193, -0.544639, -0.529919, -0.515038, -0.5,
-0.484809, -0.469471, -0.453991, -0.438371, -0.422618, -0.406736, -0.390731, -0.374607, -0.358368, -0.34202,
-0.325568, -0.309017, -0.292372, -0.275638, -0.258819, -0.241922, -0.224951, -0.207912, -0.190809, -0.173648,
-0.156434, -0.139173, -0.121869, -0.104529, -0.0871556, -0.0697564, -0.052336, -0.0348996, -0.0174526, 1.74846e-007
};

////////////////////////////Encoder//////////////////
int counterL = 0, counterR = 0;   //store the value from encoder, 304 per rotation
int angle =0;                     //possitive = turn right, negative = turn left
//int common_angle = 0;		      //clockwise +360, counterclockwise -360
//double x_axis = 1219, y_axis = 0;   //x, y postion 2438 mm max 
double x_axis_temp =0, y_axis_temp = 0; //temp values for x & y, the finalized temp values will be added to the overall x_axis, y_axis
float displacement = 0;				//turning displacement
float distanceL = 0, distanceR = 0; //distance of each wheel travelled in mm. Can be negative.
int light_sensor_FL = 0, light_sensor_FR = 0, light_sensor_BL = 0, light_sensor_BR = 0;
int int_light_sensor_FL = 0, int_light_sensor_FR = 0, int_light_sensor_BL = 0, int_light_sensor_BR = 0; //initial reading as reference points
int ls_flag = 0;//light sensor flag

int command;

double encoderL_x = 0, encoderL_y = 0;
double encoderR_x = 160, encoderR_y = 0;

int i =0 ;

void lightSensor();

/****************************************Convert my angle to GUI's angle*************************************************/

void convert(){
	common_angle = angle;
	
	if(common_angle >=0 ){
		common_angle = common_angle % 360;
	}else{
		common_angle = (- common_angle) % 360;
	}

  if (time > 0 && time < 150)
    TrAng  = 90.0 - time * 22.5 / 150.0; 
  else if (time > 150 && time < 325)
    TrAng  = 67.5 - (time - 150.0) * 45.0 / 175.0;
  else if (time > 325 && time <= 420)
    TrAng  = 22.5 - (time - 325.0) * 22.5 / 95.0;
  else if (time > 420 && time < 515)
    TrAng  = 360.0 - (time - 420.0) * 22.5 / 95.0;
  else if (time > 515 && time < 700)
    TrAng  = 337.5 - (time - 515.0) * 45.0 / 185.0;
  else if (time > 700 && time < 800)
    TrAng  = 292.5 - (time - 700.0) * 22.5 / 100.0;
  
    //x_axis = (encoderL_x + encoderR_x)/2;
    if (x_axis < 0 ) x_axis = 0;
    //y_axis = (encoderL_y + encoderR_y)/2;
    if (y_axis < 0) y_axis = 0;
        
    Enemy_y = y_axis + cos(((common_angle + TrAng) % 360) * PI /180) * dist_long;
    Enemy_x = x_axis + sin(((common_angle + TrAng) % 360) * PI /180) * dist_long;


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

		
		//lightSensor();
		//if( ls_flag == 1)
		//	break;
/*
	Serial.write("\nX: ");
	Serial.print(value1);
	Serial.write("  Y: ");
	Serial.print(value2);
	
	Serial.write("  X: ");
	Serial.print(x_axis);
	Serial.write("  Y: ");
	Serial.print(y_axis);
*/
        Serial.write(" \n X: ");
	Serial.print(angle);
	
        
	}
	while(distanceL > (distance + init_distanceL)){
		analogWrite(BIT_L_1, 0);
		analogWrite(BIT_L_2, 65);
		analogWrite(BIT_R_1, 0);
		analogWrite(BIT_R_2, 65);
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
			value = 55; 
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
                //lightSensor();
		//if( ls_flag == 1)
		//	break;

	}
	while(distanceL < (init_distanceL - distance)){
		analogWrite(BIT_L_1, 65);
		analogWrite(BIT_L_2, 0);
		analogWrite(BIT_R_1, 65);
		analogWrite(BIT_R_2, 0);
	}
		analogWrite(BIT_L_1, 0);//assign stop otherwise it will keep going forward
		analogWrite(BIT_L_2, 0);
		analogWrite(BIT_R_1, 0);
		analogWrite(BIT_R_2, 0);
}
/****************************************Manual Control*************************************************/
void motorManualControl(int command, int C_speed){
	switch(command){
	case 1:
		forward(10,C_speed);
		break;
	case 2:
		backward(10,C_speed);
		break;
	case 3:
			analogWrite(BIT_L_1, 0);
			analogWrite(BIT_L_2, 0);
			analogWrite(BIT_R_1, 0);
			analogWrite(BIT_R_2, 0);
		break;
	case 4:
			analogWrite(BIT_L_1, C_speed);//rotate left
			analogWrite(BIT_L_2, 0);
			analogWrite(BIT_R_1, 0);
			analogWrite(BIT_R_2, C_speed);
		break;
	case 5:
			analogWrite(BIT_L_1, 0);//rotate left
			analogWrite(BIT_L_2, C_speed);
			analogWrite(BIT_R_1, C_speed);
			analogWrite(BIT_R_2, 0);
		break;
	}

}



/****************************************Encoder ISR*************************************************/
void encoderStateL(){
	counterL++; //test
        int AngleIn = (angle + 720) % 360;
	if(digitalRead(2) == digitalRead(3)){  //forward
                
		distanceL = distanceL + 0.472;
		//encoderL_x = encoderL_x + 0.472 * sin(angle * 3.14159 / 180 );
		//encoderL_y = encoderL_y + 0.472 * cos(angle * 3.14159 / 180 );
                 encoderL_x = encoderL_x + 0.472 * SIN_TABLE[ AngleIn];
                encoderL_y = encoderL_y + 0.472 * COS_TABLE[ AngleIn ];
       
	}else{                      //backward
		distanceL = distanceL - 0.472;
               encoderL_x = encoderL_x - 0.472 * SIN_TABLE[ AngleIn ];
		encoderL_y = encoderL_y - 0.472 * COS_TABLE[ AngleIn ];
   
        //	encoderL_x = encoderL_x - 0.472 * sin(angle * 3.14159 / 180 );
	//	encoderL_y = encoderL_y - 0.472 * cos(angle * 3.14159 / 180 );

	}

	if(distanceL >= distanceR){ //singalling turn Right 
		displacement = distanceL - distanceR; //displacmente is difference between distanceL and distanceR, the curve on the circle
		angle = displacement / (2 * 538.6256) * 360; //circumference = 538.6256mm
	}


}
void encoderStateR(){
	counterR++; //test
        int AngleIn = (angle + 720) % 360;
	if(digitalRead(20) == digitalRead(21)){
		distanceR = distanceR + 0.472;
//		encoderR_x = encoderR_x + 0.472 * sin(angle * 3.14159 / 180 );
//		encoderR_y = encoderR_y + 0.472 * cos(angle * 3.14159 / 180 );
                encoderL_x = encoderL_x - 0.472 * SIN_TABLE[ AngleIn ];
		encoderL_y = encoderL_y - 0.472 * COS_TABLE[ AngleIn ];
	}
	else{
		distanceR = distanceR - 0.472;
//		encoderR_x = encoderR_x - 0.472 * sin(angle * 3.14159 / 180 );
//		encoderR_y = encoderR_y - 0.472 * cos(angle * 3.14159 / 180 );
                encoderL_x = encoderL_x - 0.472 * SIN_TABLE[ AngleIn ];
		encoderL_y = encoderL_y - 0.472 * COS_TABLE[ AngleIn ];
	}
	if(distanceR >= distanceL){ //singalling turn Left 
		displacement = distanceR - distanceL;
		angle = -1 * displacement / (2 * 538.6256) * 360; //circumference = 538.6256mm
	}
}


/****************************************LIGHT SENSOR*************************************************/
void lightSensor(){
	
		//int flag = 0;
	light_sensor_FL = analogRead(9);
	light_sensor_FR = analogRead(10);
	light_sensor_BL = analogRead(8);
	light_sensor_BR = analogRead(11);
	//Front Sensors
	if(light_sensor_FL > (int_light_sensor_FL+ 150)  && (int_light_sensor_FR + 100) ){
		backward(250, 130);
		turn(180, 90);
		ls_flag = 1;
	}
	else if(light_sensor_FL > (int_light_sensor_FL + 150) ){
		backward(250, 130);
		turn(115,90);
		ls_flag = 1;
	}
	else if(light_sensor_FR > (int_light_sensor_FR + 150)){
		backward(250, 130);
		turn(245,90);
		ls_flag = 1;
	}else
         ls_flag = 0;
	/*
	//Back Sensors
	if(light_sensor_BL >( int_light_sensor_BL + 150)  && light_sensor_BR > (int_light_sensor_BL + 150) ){
		forward(150, 70);
		ls_flag = 1;
	}
	else if(light_sensor_BL >( int_light_sensor_BL + 150) ){
		turn(45,90);
		forward(150, 70);
		ls_flag = 1;
	}
	else if(light_sensor_BR > (int_light_sensor_BL + 150)){
		turn(315,90);
		forward(150, 70);
		ls_flag = 1;
	}else
                ls_flag = 0;*/

}
