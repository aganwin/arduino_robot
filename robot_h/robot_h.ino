/*
 *
 * BLUE1:   GND
 * GREEN1:  GND
 * YELLOW1: PIN_ENABLE_LEFT_MOTOR
 * ORANGE1: PIN_ENABLE_RIGHT_MOTOR
 * RED1:    PIN_TTL_LEFT_0
 * BROWN1:  PIN_TTL_LEFT_1
 * BLACK:   PIN_TTL_RIGHT_0
 * WHITE:   PIN_TTL_RIGHT_1
 * GREY:    PIN_ENCODER_OUT_LEFT_0
 * PURPLE:  PIN_ENCODER_OUT_LEFT_1
 * BLUE2:   PIN_ENCODER_OUT_RIGHT_0
 * GREEN2:  PIN_ENCODER_OUT_RIGHT_1
 * YELLOW2: PIN_LIGHT_SENSOR_2
 * ORANGE2: PIN_LIGHT_SENSOR_1
 * RED2:    PIN_WEAPON
 * BROWN2:  GND
 *
 */

#include "C:\Users\Kevin\Dropbox\Porcessing folder\finalfinal_Fight\config_h\config_h.ino"
#include "C:\Users\Kevin\Dropbox\Porcessing folder\finalfinal_Fight\wheel_v6\wheel_v6.ino"


float short_range_ir_table[18][3] =
{
  {  3.25,  3.00,  -0.50  },
  {  3.00,  3.50,  -0.56  },
  {  2.72,  4.00,  -0.37  },
  {  2.35,  5.00,  -0.35  },
  {  2.00,  6.00,  -0.25  },
  {  1.75,  7.00,  -0.17  },
  {  1.58,  8.00,  -0.18  },
  {  1.40,  9.00,  -0.15  },
  {  1.25,  10.0,  -0.10  },
  {  1.05,  12.0,  -0.065 },
  {  0.92,  14.0,  -0.06  },
  {  0.80,  16.0,  -0.04  },
  {  0.72,  18.0,  -0.02  },
  {  0.68,  20.0,  -0.036 },
  {  0.50,  25.0,  -0.02  },
  {  0.40,  30.0,  -0.004 },
  {  0.38,  35.0,  -0.016 },
  {  0.30,  40.0,  0      }
};

float long_range_ir_table[15][3] =
{
  {  2.75,  15.00,  -0.048   },
  {  2.51,  20.00,  -0.054   },
  {  1.97,  30.00,  -0.044   },
  {  1.53,  40.00,  -0.028   },
  {  1.25,  50.00,  -0.020   },
  {  1.05,  60.00,  -0.015   },
  {  0.90,  70.00,  -0.010   },
  {  0.80,  80.00,  -0.008   },
  {  0.72,  90.00,  -0.007   },
  {  0.65,  100.00,  -0.007  },
  {  0.58,  110.00,  -0.004  },
  {  0.54,  120.00,  -0.004  },
  {  0.50,  130.00,  -0.003  },
  {  0.47,  140.00,  -0.002  },
  {  0.45,  150.00,  0       }
};

#define MOTOR_FORWARDS				0
#define MOTOR_BACKWARDS				1
#define MOTOR_TURN				  2


/*
-----------------------------------DISTANCE-------------------------------------
*  Distance,  CC   C    M    F    FF    FFF
*  cm,        5,   10,  20   50   100   150
*/
#define Distance_CC  5
#define Distance_C  10;
#define Distance_M  20;
#define Distance_F  50;
#define Distance_FF  100;
#define Distance_FFF  150;

/*
-------------------------------CLASS DECLARATION--------------------------------
*/

class Robot
{
  public:
    Robot();
    void  access_weapon(int control);
    void  access_turret(int deg);
    float access_close_range_IR_sensor(float voltage);
    float access_long_range_IR_sensor(float voltage);
    char* read_buffer();
    int   sweep_search();
    int   access_light_sensor();
    void  access_motor(int control, int value, int speed);
};

/*
-------------------------------CLASS DEFINITIONS--------------------------------
*/

Robot::Robot()
{
  pinMode(PIN_WEAPON, OUTPUT);
  pinMode(PIN_TURRET, OUTPUT);
  pinMode(10, OUTPUT);

	pinMode(PIN_ENABLE_LEFT_MOTOR, OUTPUT);
	pinMode(PIN_ENABLE_RIGHT_MOTOR, OUTPUT);
	pinMode(BIT_L_1, OUTPUT);  
	pinMode(BIT_L_2, OUTPUT);
	pinMode(BIT_R_1, OUTPUT);
	pinMode(BIT_R_2, OUTPUT);
  //pinMode(PIN_ENCODER_OUT_LEFT_0, INPUT); //Do not initialize these pin
  //pinMode(PIN_ENCODER_OUT_LEFT_1, INPUT); //Two of them are define as interrupt
  //pinMode(PIN_ENCODER_OUT_RIGHT_0, INPUT); //It might casue problem
  //pinMode(PIN_ENCODER_OUT_RIGHT_1, INPUT);
	
	pinMode(31, OUTPUT);//New, Eric said
	pinMode(32, OUTPUT);
	digitalWrite(31, HIGH);//New, Eric said
	digitalWrite(32, HIGH);

	//Enable motors
	digitalWrite(PIN_ENABLE_LEFT_MOTOR, HIGH);
	digitalWrite(PIN_ENABLE_RIGHT_MOTOR, HIGH);

	attachInterrupt(0, encoderStateL, CHANGE);  //pin2
	attachInterrupt(2, encoderStateR, CHANGE);  //pin21
        int_light_sensor_FL = analogRead(9);
	int_light_sensor_FR = analogRead(10);
	int_light_sensor_BL = analogRead(8);
	int_light_sensor_BR = analogRead(11);
}

float access_close_range_IR_sensor(float voltage)
{
  int i;
  float first, second, slope, value;
  
  voltage = (voltage * 5) / 1023;

  for (i = 0; i < 17; i++)
  {
    first = short_range_ir_table[i][0];
    second = short_range_ir_table[i+1][0];

    if (voltage <= first && voltage >= second)
    {
      slope = short_range_ir_table[i][2];
      value = (first - voltage) +
              (-1)*slope*short_range_ir_table[i][1];
      value /= (-1)*slope;

    }
  }
  
  
  return value;
  
}

float access_long_range_IR_sensor(float voltage)
{
  int i;
  float first, second, slope, value;
  
  voltage = (voltage * 5) / 1023;
    
  /*
  start_time = milliseconds;
  Serial.println("The start time is : ");
  Serial.print(start_time);
  Serial.println("\n");
  */
  
  for (i = 0; i < 17; i++)
  {
    first = long_range_ir_table[i][0];
    second = long_range_ir_table[i+1][0];

    if (voltage <= first && voltage >= second)
    {
      slope = long_range_ir_table[i][2];
      value = (first - voltage) +
              (-1)*slope*long_range_ir_table[i][1];
      value /= (-1)*slope;
    }
  }

  return value;
  /*
  end_time = milliseconds;
  Serial.println("The end time is : ");
  Serial.print(end_time);
  Serial.println("\n");
  */
    
}

void Robot::access_turret(int deg)
{
  OCR2B = 255 - deg;
}

int Robot::sweep_search()
{
  int i, sector;
  
  OCR2B = 255 - 1;
  
  delay(725);
  
  setup_timer5_enable();
  
  value_long = 0;
  value_short = 0;
  time = 0;
  
  OCR2B = 255 - 71;
  milliseconds = 0;

  // delay(725);

  delay(900);
  
  setup_timer5_disable();
  

  /*
  if (time > 0 && time < 150)
    sector = 1;
  else if (time > 150 && time < 275)
    sector = 2;
  else if (time > 275 && time < 415)
    sector = 3;
  else if (time > 415 && time < 580)
    sector = 4;
  else if (time > 580)
    sector = 5;
  */

  if (time > 0 && time < 150)
    sector = 1;
  else if (time > 150 && time < 325)
    sector = 2;
  else if (time > 325 && time < 515)
    sector = 3;
  else if (time > 515 && time < 700)
    sector = 4;
  else if (time > 700)
    sector = 5;
  
  OCR2B = 255 - 42;
  
  return sector;        
}

int Robot::access_light_sensor()
{
  int input;

  for (int i = 0; i < 4; i++)
  {
    input = analogRead(PIN_LIGHT_SENSOR_1 + i);
/*
    if (input > 400)
    {
      Serial.print("Light Sensor ");
      Serial.print(i+1);
      Serial.print(" sees black:");
      Serial.println(input);
      // return PIN_LIGHT_SENSOR_1 + i;
    }
    else
    {
      Serial.print("Light Sensor ");
      Serial.print(i+1);
      Serial.print(" sees white");
      Serial.println(input);
      // return 0;
    }
    */
  }
}

void Robot::access_motor(int control, int value, int speed)
{
  //Determine which instruction
    if(control == MOTOR_FORWARDS)
    {
      forward(value, speed);
    }
    else if(control == MOTOR_BACKWARDS)
    {
      backward(value, speed);
    }
    else if(control == MOTOR_TURN)
    {
      turn(value, speed);
    }
}

void Robot::access_weapon(int control)
{
  digitalWrite(PIN_WEAPON, control);    
}



/****************************************Wireless Comm************************************************/

byte intTObyte_100 (int k)
{
  return (byte) (k/100); 
}

byte intTObyte_1 (int k)
{
  return (byte) (k%100);
}

int byteTOint ( byte x, byte y )
{
   return ( (int)x *100 + (int)y );
}
void sendMessage()
{
     
  //how is angle defined?

      byte temp1 = 0xFF;
      byte temp2 = 0xFF;
      Serial.write(temp1);//1
      
      temp1 = 0xF0;
      temp2 = temp1^temp2;
      Serial.write(temp1);//2
      
        temp1 = intTObyte_100((int)x_axis);
      temp2 = temp1 ^ temp2;
      Serial.write(temp1);//3
      
      temp1 = intTObyte_1((int)x_axis);
      temp2 = temp1 ^ temp2;
      Serial.write(temp1);//4
      
      temp1 = intTObyte_100((int)y_axis);
      temp2 = temp1 ^ temp2;
      Serial.write(temp1);//5
      
      temp1 = intTObyte_1((int)y_axis);
      temp2 = temp1 ^ temp2;
      Serial.write(temp1);//6
      
      temp1 = intTObyte_100(Enemy_x);
      temp2 = temp1 ^ temp2;
      Serial.write(temp1);//7
      
      temp1 = intTObyte_1(Enemy_x);
      temp2 = temp1 ^ temp2;
      Serial.write(temp1);//8

      temp1 = intTObyte_100(Enemy_y);
      temp2 = temp1 ^ temp2;
      Serial.write(temp1);//9
      
      temp1 = intTObyte_1(Enemy_y);
      temp2 = temp1 ^ temp2;
      Serial.write(temp1);//10
      
      temp1 = intTObyte_100(dist_long);
      temp2 = temp1 ^ temp2;
      Serial.write(temp1);//11
      
      temp1 = intTObyte_1(dist_long);
      temp2 = temp1 ^ temp2;
      Serial.write(temp1);//12
      
      temp1 = intTObyte_100(TrAng);
      temp2 = temp1 ^ temp2;
      Serial.write(temp1);//13
      
      temp1 = intTObyte_1(TrAng);
      temp2 = temp1 ^ temp2;
      Serial.write(temp1);//14
      
      temp1 = intTObyte_100(common_angle);
      temp2 = temp1 ^ temp2;
      Serial.write(temp1);//1IR_L5
      
      temp1 = intTObyte_1(common_angle);
      temp2 = temp1 ^ temp2;
      Serial.write(temp1);//16
      
      temp1 = intTObyte_100(AttCount);
      temp2 = temp1 ^ temp2;
      Serial.write(temp1);//17
      
      temp1 = intTObyte_1(AttCount);
      temp2 = temp1 ^ temp2;
      Serial.write(temp1);//18
      
      Serial.write(temp2); //19
      //Serial.println();
      //Serial.println("here2");
}
void receiveMessage()
{
  //Robot XXX;
 	int i;
	int serial_input_buffer[3];
    //int serial_output_buffer[3];
    //int input;
    int temp = -1;
 
  if(Serial.available())
  {
    for (i = 0; i < 4;  )	
    {
	temp = Serial.read();
	if (temp >= 0)	
	{
            serial_input_buffer[i] = temp;
	    i++;
	}
    }
//    Serial.println(" ");
    if (serial_input_buffer[3] == (serial_input_buffer[0] ^ serial_input_buffer[1] ^ serial_input_buffer[2]))
    {
	for (i = 0; i < 4; i++ )
	{
	  if (serial_input_buffer[i] == 0xf0) 
	  {
	    if (i > 0 )  orderReceived = serial_input_buffer[i-1];
	    else orderReceived = serial_input_buffer[3];
            
	  }
	}
    //Serial.println(orderReceived);
    
    }
    else
    {
      orderReceived = 0;
    } 
  }
}

void LoadVariables()
{
  //Serial.println(orderReceived);
 	if(orderReceived & 0x80 )	command = move_forward;
	else if (orderReceived & 0x40)	command = move_reverse;
	else if (orderReceived & 0x20)		command = move_rotateRight;
	else if (orderReceived & 0x10)		command = move_rotateLeft;
	else	command = move_stop;
        //if (command >=0) Serial.println("Direction: " + command);

        if (orderReceived & 0x8)    turnTr = 1; // 0 = not move, 1 = left, 3 = right
        else if (orderReceived & 0x4)    turnTr = 3;
        else turnTr = 0;

        //if (command >=0)        Serial.println("Other1: " + command);
        
        Attact = (orderReceived & 0x2) >> 1; //0=not attact, 1 = attact
        Manual = orderReceived & 0x1; //0 = manual, 1 = auto 
        //if (command >=0)        Serial.println("Other2: " + command);  
}

