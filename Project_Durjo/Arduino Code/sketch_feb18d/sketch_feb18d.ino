#include<Wire.h>
#include<Servo.h>
Servo Myservo1;
Servo Myservo2;

const int MPU=0x68; 
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;

int ActrMotor1_pin1 = 11;
int ActrMotor1_pin2 = 12;
int ActrMotor1_Key = 1;

int LED1 = 13;
int flame_sensor = 6;
int flame_detected;
int FlameKey = 1;

int PumpMotor = 10;
int PumpKey=1;

int gas_sensor = 7;
int gas_detected;
int GasKey=1;

int servoPin1 = 5;
int ServoKey1= 1;
int servoPin2 = 9;
int SerPos2;

const int SolDrLck = 3;
const int LED2 = 13;
int GyroKey = 1;
int IRSensor = 4; // connect ir sensor to arduino pin 2

void setup(){
  pinMode(ActrMotor1_pin1, OUTPUT);
  pinMode(ActrMotor1_pin2, OUTPUT);
  pinMode (SolDrLck, OUTPUT);
  pinMode (IRSensor, INPUT); // sensor pin INPUT
  pinMode(LED1, OUTPUT);
  pinMode(flame_sensor, INPUT);
  pinMode(gas_sensor, INPUT);
  pinMode(PumpMotor, OUTPUT);
  
  Myservo1.attach(servoPin1);
  Myservo2.attach(servoPin2);
  
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B); 
  Wire.write(0);    
  Wire.endTransmission(true);
  Serial.begin(9600); 
}

void loop(){
  if (ServoKey1==1){
      Myservo1.write(120); //Servo motor lock position fixed
  }
  else{
      Myservo1.write(0); //Servo motor lock position fixed
  }
  if(GyroKey == 1){
    Wire.beginTransmission(MPU);
    Wire.write(0x3B);  
    Wire.endTransmission(false);
    Wire.requestFrom(MPU,12,true);  
    AcX=Wire.read()<<8|Wire.read();    
    AcY=Wire.read()<<8|Wire.read();  
    AcZ=Wire.read()<<8|Wire.read();  
    GyX=Wire.read()<<8|Wire.read();  
    GyY=Wire.read()<<8|Wire.read();  
    GyZ=Wire.read()<<8|Wire.read();  
    
    Serial.print("Accelerometer: ");
    Serial.print("X = "); Serial.print(AcX);
    Serial.print(" | Y = "); Serial.print(AcY);
    Serial.print(" | Z = "); Serial.println(AcZ); 
    
    Serial.print("Gyroscope: ");
    Serial.print("X = "); Serial.print(GyX);
    Serial.print(" | Y = "); Serial.print(GyY);
    Serial.print(" | Z = "); Serial.println(GyZ);
    Serial.println(" ");
    delay(333);
    
    if (((1200<GyY)||(GyY<-1200))&&((2704<GyX)||(GyX<-2704))&&((1055<GyZ)||(GyZ<-1055))&&((748<AcY)||(AcY<-748))&&((2455<AcX)||(AcX<-2455))&&((17101<AcZ)||(AcZ<-17101))){
        digitalWrite (SolDrLck, LOW);
        
        if (ActrMotor1_Key== 1){
            digitalWrite(ActrMotor1_pin1, HIGH);
            digitalWrite(ActrMotor1_pin2, LOW);
        }
        delay(2000);
        GyroKey =0;
    }
    digitalWrite (SolDrLck, HIGH);
    delay(100);
  }
  
  flame_detected = digitalRead(flame_sensor);
  Serial.println(flame_detected);
  
  if (FlameKey==1){
    
    if (flame_detected == 0){
      Serial.println("Flame detected...! take action immediately.");
      digitalWrite (SolDrLck, LOW);
      digitalWrite (PumpMotor, HIGH);
      delay(200);

      PumpKey=0;
      
      Myservo2.write(0);
      delay(300);
      Myservo2.write(90);
      delay(300);
      Myservo2.write(180);
      delay(300);
      
      if (ActrMotor1_Key== 1){
        digitalWrite(ActrMotor1_pin1, HIGH);
        digitalWrite(ActrMotor1_pin2, LOW);
        
        digitalWrite(LED1, HIGH);
        delay(100);
        digitalWrite(LED1, LOW);
        delay(100);
      }
      delay(2000);
      digitalWrite (PumpMotor, LOW);
      delay(100);
      FlameKey =0;
    }
    
    else{
        Serial.println("No flame detected. stay cool");
        digitalWrite(LED1, HIGH);
        digitalWrite (SolDrLck, HIGH);
        delay(100);
    }
  }   
  gas_detected = digitalRead(gas_sensor);
  Serial.println(gas_detected);
  
  if (GasKey==1){
      
      if (gas_detected == 0){
          Serial.println("gas detected...! take action immediately.");
          digitalWrite (SolDrLck, LOW);
          
          digitalWrite (PumpMotor, HIGH);
          delay(200);
          
          PumpKey=0;
          
          Myservo2.write(0);
          delay(300);
          Myservo2.write(90);
          delay(300);
          Myservo2.write(180);
          delay(300);
          
          if (ActrMotor1_Key== 1){
              digitalWrite(ActrMotor1_pin1, HIGH);
              digitalWrite(ActrMotor1_pin2, LOW);
      
              digitalWrite(LED1, HIGH);
              delay(100);
              digitalWrite(LED1, LOW);
              delay(100);
          }
          delay(2000);
          digitalWrite (PumpMotor, LOW);
          delay(200);
          GasKey =0;
      }
      else{
          Serial.println("No gas detected. stay cool");
          digitalWrite(LED1, HIGH);
          digitalWrite (SolDrLck, HIGH);
          delay(100);
      }
  
  }
   
  int statusSensorIR = digitalRead (IRSensor);
  Serial.print("IR: ");Serial.print(statusSensorIR);// 1==far,0==near
  
  if ((GyroKey==0)||(FlameKey==0)||(GasKey==0)){
    
    if (statusSensorIR == 1){
      digitalWrite(LED2, HIGH);
      if (ActrMotor1_Key== 1){
        digitalWrite(ActrMotor1_pin1, HIGH);
        digitalWrite(ActrMotor1_pin2, LOW);
        if (PumpKey==0){
            digitalWrite (PumpMotor, HIGH);
            
            Myservo2.write(0);
            delay(300);
            Myservo2.write(90);
            delay(300);
            Myservo2.write(180);
            delay(300);
        }
        delay(1000);
        
       
        digitalWrite (PumpMotor, LOW);

        digitalWrite(LED1, HIGH);
        delay(100);
        digitalWrite(LED1, LOW);
        delay(100);
        
      }
      else{
        
        digitalWrite(ActrMotor1_pin1, LOW);
        digitalWrite(ActrMotor1_pin2, LOW);
        
        if (PumpKey==0){
            digitalWrite (PumpMotor, HIGH);
              
            Myservo2.write(0);
            delay(300);
            Myservo2.write(90);
            delay(300);
            Myservo2.write(180);
            delay(300);
        }

        delay(1000);
        ServoKey1=0;
        
        digitalWrite (PumpMotor, LOW);
        
        digitalWrite(LED1, HIGH);
        delay(100);
        digitalWrite(LED1, LOW);
        delay(100);
      }
    }
    else
    {
      digitalWrite(LED2, LOW);
      ActrMotor1_Key = 0;
      ServoKey1=0;
      
      digitalWrite(ActrMotor1_pin1, LOW);
      digitalWrite(ActrMotor1_pin2, LOW);
      digitalWrite(LED1, HIGH);

      if (PumpKey==0){
        digitalWrite (PumpMotor, HIGH);
 
        Myservo2.write(0);
        delay(300);
        Myservo2.write(90);
        delay(300);
        Myservo2.write(180);
        delay(300);
      }
      delay(1000);
      
      digitalWrite (PumpMotor, LOW);
      delay(200);
    }
  
  }

}
