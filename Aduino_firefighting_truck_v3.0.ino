/*------ Arduino Fire Fighting Robot Code----- */
  
int sensorValueRight_S = 0;// variable to store the value coming from the sensor 
int sensorValueLeft_S = 0;
int sensorValueForward_1_S = 0;   
boolean fire = false;
 
/*-------defining Inputs------*/
// select the input pin for the LDR 
int Left_S = A0;
int Forward_1_S = A4;
int Right_S = A5;
 
#include<SoftwareSerial.h> 
//defining ports for the pump
#define enA 13 //enable A
#define in1 12 
#define in2 11  

int rotDirection = 0; //seting the rotation direction value to 0

//Pin numbers definition for the motors 
const int motorEnableLeft = 10;
const int motorForwardLeft = 9;
const int motorBackLeft = 8;
const int motorEnableRight = 5;
const int motorForwardRight = 6;
const int motorBackRight = 7;
const int trigPinFront = A1;
const int echoPinFront = 2;
const int trigPinLeft = A2;
const int echoPinLeft = 3;
const int trigPinRight = A3;
const int echoPinRight = 4;

//Sart variables for the Motors
const int leftMotorSpeed = 255; //best at 160
const int rightMotorSpeed = 255;//best at 255
const int delayTime = 150;

//Variables for Ultrasonic Sensors
long durationFront;
int distanceFront;
long durationLeft;
int distanceLeft;
long durationRight;
int distanceRight;
const int minFrontDistance = 30;
const int minSideDistance = 20;
const int stuckDistance = 10; //what distance the code defines as stuck 

/*-------preset actions------*/

//-preset for stoping the car
void stopCar () {
  digitalWrite(motorForwardLeft, LOW);
  digitalWrite(motorBackLeft, LOW);
  digitalWrite(motorForwardRight, LOW);
  digitalWrite(motorBackRight, LOW);
  analogWrite(motorEnableLeft, 0);
  analogWrite(motorEnableRight, 0);
}
//-preset for the car to go full forward
void goForwardFull () {
  digitalWrite(motorForwardLeft, HIGH);
  digitalWrite(motorBackLeft, LOW);
  digitalWrite(motorForwardRight, HIGH);
  digitalWrite(motorBackRight, LOW);
  analogWrite(motorEnableLeft, leftMotorSpeed);
  analogWrite(motorEnableRight, rightMotorSpeed);
}
//-preset for the car to turn to the left
void goLeft () {
  digitalWrite(motorForwardLeft, LOW);
  digitalWrite(motorBackLeft, LOW);
  digitalWrite(motorForwardRight, HIGH);
  digitalWrite(motorBackRight, LOW);
  analogWrite(motorEnableLeft, 0);
  analogWrite(motorEnableRight, rightMotorSpeed);
}
//-preset for the car to turn to the right
void goRight () {
  digitalWrite(motorForwardLeft, HIGH);
  digitalWrite(motorBackLeft, LOW);
  digitalWrite(motorForwardRight, LOW);
  digitalWrite(motorBackRight, LOW);
  analogWrite(motorEnableLeft, leftMotorSpeed);
  analogWrite(motorEnableRight, 0);
}
//-preset for the car to go backwards
void goBack () {
  digitalWrite(motorForwardLeft, LOW);
  digitalWrite(motorBackLeft, HIGH);
  digitalWrite(motorForwardRight, LOW);
  digitalWrite(motorBackRight, HIGH);
  analogWrite(motorEnableLeft, leftMotorSpeed);
  analogWrite(motorEnableRight, rightMotorSpeed);
}

//-preset for the distancesensors to start reading
void sensorRead () {
  //Read front sensor value
  digitalWrite(trigPinFront, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPinFront, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinFront, LOW);
  durationFront = pulseIn(echoPinFront, HIGH);
  distanceFront = durationFront * 0.034 / 2;
  //Read left sensor value
  digitalWrite(trigPinLeft, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPinLeft, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinLeft, LOW);
  durationLeft = pulseIn(echoPinLeft, HIGH);
  distanceLeft = durationLeft * 0.034 / 2;
  //Read right sensor value
  digitalWrite(trigPinRight, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPinRight, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinRight, LOW);
  durationRight = pulseIn(echoPinRight, HIGH);
  distanceRight = durationRight * 0.034 / 2;
}


void setup() {
  Serial.begin(9600); 
  
  // Set initial rotation direction for the pump
  digitalWrite(in1, LOW); 
  digitalWrite(in2, HIGH);
  /*-------defining Inputs and outputs------*/
  //__pump__
  pinMode(enA, OUTPUT); //enable for the pump
  pinMode(in1, OUTPUT); //pump
  pinMode(in2, OUTPUT);  //pump

  //__fire sensors__
  pinMode(Left_S, INPUT); //Left fire sensor
  pinMode(Right_S, INPUT); //Right fire sensor
  pinMode(Forward_1_S, INPUT); //Fire sensor in the front
  
  //__Motors__
  pinMode(motorEnableLeft, OUTPUT); 
  pinMode(motorForwardLeft, OUTPUT);
  pinMode(motorBackLeft, OUTPUT);
  pinMode(motorEnableRight, OUTPUT);
  pinMode(motorForwardRight, OUTPUT);
  pinMode(motorBackRight, OUTPUT);

  //__ultrasonic sensors__
  pinMode(trigPinFront, OUTPUT);
  pinMode(echoPinFront, INPUT);
  pinMode(trigPinLeft, OUTPUT);
  pinMode(echoPinLeft, INPUT);
  pinMode(trigPinRight, OUTPUT);
  pinMode(echoPinRight, INPUT);
}

//preset for the pump to fire water
void put_off_fire(){
  delay (500);
  int potValue = analogRead(A0); // Read potentiometer value 
  int pwmOutput = map(potValue, 0, 1023, 0 , 255); // Map the potentiometer value from 0 to 255 
  int pwmOutput = 255; 
  analogWrite(enA, pwmOutput); // Send PWM signal to L298N Enable pin    
  delay(500); //a delay is added so the pump fires water in bursts    
  digitalWrite(enA, LOW);
  
}

void loop() {
  //**--defining the variables for the flamesensors--**
  //Left flamesensor
  sensorValueLeft_S = analogRead(Left_S);
  Serial.println(sensorValueLeft_S);
  //Right flamesensor
  sensorValueRight_S = analogRead(Right_S);
  Serial.println(sensorValueRight_S);
  //Flamesensor in the front
  sensorValueForward_1_S = analogRead(Forward_1_S);
  Serial.println(sensorValueForward_1_S);  
  
  sensorRead(); //starts the function for activating the distance sensors

  //**--loop for selfdriving car--**
  
  if(1==1){ //**if this line is active the loop will always run
 //** if the line under is active the loop will run when there is no flames
 //if (sensorValueLeft_S > 100 && sensorValueRight_S > 100 && sensorValueForward_1_S > 100) //If Fire not detected all sensors are above 100 {
    if ((distanceFront <= minFrontDistance) || (distanceLeft <= minSideDistance) || (distanceRight <= minSideDistance)) {
      if ((distanceLeft < stuckDistance) || (distanceRight < stuckDistance) || (distanceFront < stuckDistance)) {
        goBack();
        delay(1.5*delayTime);
      }
      else if ((distanceFront <= minFrontDistance) && (distanceLeft <= minSideDistance) && (distanceRight <= minSideDistance)) {
        goBack();
        delay(1.5*delayTime);
      }
      else if (distanceLeft > distanceRight ) {
        goLeft();
        delay(delayTime);
      }
      else if (distanceLeft <= distanceRight) {
        goRight();
        delay(delayTime);
      }
      else
        goForwardFull();
    }
    else
      goForwardFull();
  }    
    
//the following loops will run when the fire sensors are reading a value less than 100. Values less than a 100 = fire.  
if (sensorValueForward_1_S < 100) //If Fire is straight ahead
      {
      Serial.print("Forward_1_S");
      Serial.print(Forward_1_S);
      //Move the robot forward
      goForwardFull();
      put_off_fire();
      }
      else if (sensorValueLeft_S < 100) //If Fire is to the left
      {
      Serial.print("Left_S");
      Serial.print(Left_S); 
      //Move the robot left
      goLeft();
      delay(500); //a delay is added so the robot turns and then fire and not fire while turning
      put_off_fire();
      }
      else if (sensorValueRight_S < 100) //If Fire is to the right
      {
      Serial.print("Right_S");
      Serial.print(Right_S);
      //Move the robot right
      goRight();
      delay(500); //a delay is added so the robot turns and then fire and not fire while turning
      put_off_fire();
      }
}



