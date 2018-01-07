#include <Servo.h>
const int W2_2 = 12, W2_1 = 13;
const int W1_1 = 10, W1_2 = 11; //wheel output
const int green = 8, red = 7; //debug light
const int servo = 9, minhz = 1000, maxhz = 2000; //servo
const int ir1 = 5, ir2 = 6; //IR sensor
const int eyetrig = 4, eyeecho = 3; //eye ca
long duration, distance; int correct = 0;
bool servoed = false;

int curstatus = 1;

Servo myservo;
void setup() {
  Serial.begin(9600);
  pinMode(W1_1, OUTPUT);
  pinMode(W1_2, OUTPUT);
  pinMode(W2_1, OUTPUT);
  pinMode(W2_2, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(red, OUTPUT);
  myservo.attach(servo, minhz, maxhz);
  pinMode(ir1, INPUT);
  pinMode(ir2, INPUT);
  pinMode(eyetrig, OUTPUT);
  pinMode(eyeecho, INPUT);
}

void loop() {
  motor(curstatus);
  curstatus = get_ir();
  int yo = get_eye();
  if(yo < 15 && yo > 1 && servoed == false){
    lets_servo();
    servoed = true;
  }
}

void motor(int xxx){
  switch (xxx){//no=0; only ir1 = 1; only ir2 = 2; both = 3
    case 0: //no, (no use?)
      digitalWrite(W1_1, LOW);
      digitalWrite(W1_2, LOW);
      digitalWrite(W2_1, LOW);
      digitalWrite(W2_2, LOW);
      digitalWrite(green, LOW);
      digitalWrite(red, LOW);
      break;

    case 1: // only 1, turn right
      digitalWrite(W1_1, LOW);
      digitalWrite(W1_2, HIGH);
      digitalWrite(W2_1, HIGH);
      digitalWrite(W2_2, LOW);
      digitalWrite(green, HIGH);
      digitalWrite(red, LOW);
      delay(100);
      break;

    case 2:   // 2, turn left
      digitalWrite(W1_1, HIGH);
      digitalWrite(W1_2, LOW);
      digitalWrite(W2_1, LOW);
      digitalWrite(W2_2, HIGH);
      digitalWrite(green, LOW);
      digitalWrite(red, HIGH);
      break;

    case 3:   // 1,2, keep moving!
      digitalWrite(W1_1, LOW);
      digitalWrite(W1_2, HIGH);
      digitalWrite(W2_1, LOW);
      digitalWrite(W2_2, HIGH);
      digitalWrite(green, HIGH);
      digitalWrite(red, HIGH);
      break;

    default: //end
      digitalWrite(W1_1, LOW);
      digitalWrite(W1_2, LOW);
      digitalWrite(W2_1, LOW);
      digitalWrite(W2_2, LOW);
      digitalWrite(green, LOW);
      digitalWrite(red, LOW);
      lets_reservo();
      break;
  }

}

void lets_servo(){
 // motor(0xC8763);
  digitalWrite(W1_1, LOW);
      digitalWrite(W1_2, LOW);
      digitalWrite(W2_1, LOW);
      digitalWrite(W2_2, LOW);
  digitalWrite(green, HIGH);
  digitalWrite(red, HIGH);
  for (int pos = 0; pos <= 25; pos += 1) {
    myservo.write(pos);
    delay(15);
  }
  digitalWrite(W1_1, LOW);
  digitalWrite(W1_2, HIGH);
  digitalWrite(W2_1, LOW);
  digitalWrite(W2_2, HIGH);
  delay(2017);
  digitalWrite(W1_1, LOW);
  digitalWrite(W1_2, LOW);
  digitalWrite(W2_1, LOW);
  digitalWrite(W2_2, LOW);
  for (int pos = 30; pos <= 110; pos += 1) {
    myservo.write(pos);
    delay(15);
  }
}

void lets_reservo(){
  digitalWrite(green, HIGH);
  digitalWrite(red, HIGH);
  for (int pos = 110; pos >= 90; pos -= 1) {
    myservo.write(pos);
    delay(15);
  }
   digitalWrite(W1_1, HIGH);
   digitalWrite(W1_2, LOW);
   digitalWrite(W2_1, HIGH);
   digitalWrite(W2_2, LOW);
   delay(2087);
   digitalWrite(W1_1, LOW);
      digitalWrite(W1_2, LOW);
      digitalWrite(W2_1, LOW);
      digitalWrite(W2_2, LOW);
      digitalWrite(green, LOW);
      digitalWrite(red, LOW);
}

int get_ir(){ //no=0; only ir1 = 1; only ir2 = 2; both = 3
  int ir1_state = digitalRead(ir1), ir2_state = digitalRead(ir2);
  if(ir1_state + ir2_state == 0){
    Serial.print("IR: 0 + 0 = 0  ");
    return 0;
  }
  else if(ir1_state + ir2_state == 2){
    Serial.print("IR: 1 + 1 = 3  ");
    return 3;
  }
  else{
    if(ir1_state == 0){
      Serial.print("IR: 1 + 0 = 1  ");
      return 1;
    }
    else{
      Serial.print("IR: 0 + 1 = 2   ");
      return 2;
    }
  }
}

int get_eye(){
  long last_distance = distance;
  digitalWrite(eyetrig, LOW);
  delayMicroseconds(5);
  digitalWrite(eyetrig, HIGH);
  delayMicroseconds(5);
  digitalWrite(eyetrig, LOW);
  duration = pulseIn(eyeecho, HIGH,30000);
                               //^OUT TIME

  if(duration != 0) {distance = duration / 29 / 2;}
  else{Serial.print("===Oh, no! No Response===\n"); return NULL;}

  //avoid randomness
  if(abs(distance - last_distance) <= 10){correct += 1 ;}
  else{correct = 0;}

  Serial.print("Gotcha! ");
  Serial.print(distance);
  Serial.print("cm     ");
  Serial.print(correct);
  Serial.print("combos!!");

  if(correct >= 5){
    Serial.print("   \\\\PERMISSION GRANTED!!//\n");
    return distance;
  }
  else{
    Serial.print("\n");
    return NULL;
  }

}
