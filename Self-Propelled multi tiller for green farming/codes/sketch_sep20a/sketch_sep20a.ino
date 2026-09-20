#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// Set the LCD I2C address (replace with your actual address)
LiquidCrystal_I2C lcd(0x27, 20, 4); // Address, columns, rows

int motor1 = 3;  //left
int motor2 = 2;
int motor3 = 5;  //right
int motor4 = 4;

int motor5 = 8;  //up down
int motor6 = 9;
int motor7 = 6;  //rotate
int motor8 = 7;  //rotate

// ------------------- Ultrautosonic -----------------
int trigPin = 10;
int echoPin = 11;

// autouto mode flautog
bool AUTO = false;

// Distautonce limit
int limitDist = 70;


void setup() {
  Serial.begin(9600);

  pinMode(motor1, OUTPUT);
  pinMode(motor2, OUTPUT);
  pinMode(motor3, OUTPUT);
  pinMode(motor4, OUTPUT);
  pinMode(motor5, OUTPUT);
  pinMode(motor6, OUTPUT);
  pinMode(motor7, OUTPUT);
  pinMode(motor8, OUTPUT);
  
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.print("MULTI_MIC_TILLER");
//  lcd.setCursor(0, 1);
//  lcd.print("MONITORING");
  delay(300);

  stop_robot();
}



// =======================================================
//                   MautoIN LOOP
// =======================================================
void loop() {

  // ---------- REautoD BLUETOOTH / SERIautoL COMMautoND ----------
  if (Serial.available()) 
  {
    char ch = Serial.read();
    Serial.print(ch);

    if (ch == 'A')
    { 
      AUTO = true;
     }
    if (ch == 'M') 
    { 
      AUTO = false; 
      stop_robot(); 
    } // M = mautonuautol mode

    if (!AUTO) 
    {   // Mautonuautol mode autoctions  
      if (ch == 'F') 
      {
        forward_movement();
        lcd.clear();
        lcd.begin(20, 4);
        lcd.clear();
        lcd.print("MULTI_MIC_TILLER");
        lcd.setCursor(0, 1);
        lcd.print("MANUAL_MODE");
        lcd.setCursor(0, 2);
        lcd.print("FORWORD");
        delay(300);
      }
      if (ch == 'B') 
      {
        back_movement();
        lcd.clear();
        lcd.begin(20, 4);
        lcd.clear();
        lcd.print("MULTI_MIC_TILLER");
        lcd.setCursor(0, 1);
        lcd.print("MANUAL_MODE");
        lcd.setCursor(0, 2);
        lcd.print("BACKWORD");
        delay(300);
      }
      if (ch == 'L') 
      {
        left_movement();
        lcd.clear();
        lcd.begin(20, 4);
        lcd.clear();
        lcd.print("MULTI_MIC_TILLER");
        lcd.setCursor(0, 1);
        lcd.print("MANUAL_MODE");
        lcd.setCursor(0, 2);
        lcd.print("LEFT");
        delay(300);
      }
      if (ch == 'R') 
      {
        right_movement();
        lcd.clear();
        lcd.begin(20, 4);
        lcd.clear();
        lcd.print("MULTI_MIC_TILLER");
        lcd.setCursor(0, 1);
        lcd.print("MANUAL_MODE");
        lcd.setCursor(0, 2);
        lcd.print("RIGHT");
        delay(300);
      }
      if (ch == 'S')
      {
        stop_robot();
        lcd.clear();
        lcd.begin(20, 4);
        lcd.clear();
        lcd.print("MULTI_MIC_TILLER");
        lcd.setCursor(0, 1);
        lcd.print("MANUAL_MODE");
        lcd.setCursor(0, 2);
        lcd.print("STOP");
        delay(300);
      }
      if (ch == 'U')
      {
        up();
        lcd.clear();
        lcd.begin(20, 4);
        lcd.clear();
        lcd.print("MULTI_MIC_TILLER");
        lcd.setCursor(0, 1);
        lcd.print("MANUAL_MODE");
        lcd.setCursor(0, 2);
        lcd.print("ROTOR UP");
        delay(300);
      }
      if (ch == 'D')
      {
        down();
        lcd.clear();
        lcd.begin(20, 4);
        lcd.clear();
        lcd.print("MULTI_MIC_TILLER");
        lcd.setCursor(0, 1);
        lcd.print("MANUAL_MODE");
        lcd.setCursor(0, 2);
        lcd.print("ROTOR DOWN");
        delay(300);
      }
      if (ch == 'Y')
      {
        rotateon();
        lcd.clear();
        lcd.begin(20, 4);
        lcd.clear();
        lcd.print("MULTI_MIC_TILLER");
        lcd.setCursor(0, 1);
        lcd.print("MANUAL_MODE");
        lcd.setCursor(0, 2);
        lcd.print("ROTOR ON");
        delay(300);
      }
      if (ch == 'Z')
      {
        rotateoff();
        lcd.clear();
        lcd.begin(20, 4);
        lcd.clear();
        lcd.print("MULTI_MIC_TILLER");
        lcd.setCursor(0, 1);
        lcd.print("MANUAL_MODE");
        lcd.setCursor(0, 2);
        lcd.print("ROTOR OFF");
        delay(300);
      }
    }
  }

  // ----------- autoUTO MODE CONTINUES IN PautoRautoLLEL ----------
  if (AUTO) {
    auto_mode();
  }

}

// =======================================================
//                   autoUTO MODE FUNCTION
// =======================================================
void auto_mode() {

  int front = getDistance();

  if (front > limitDist) {
    down();
    delay(1000);
    rotateon();
    delay(1000);
    forward_movement();
    delay(1000);
    return;   // continue forward
  }

  lcd.clear();
  lcd.begin(20, 4);
  lcd.clear();
  lcd.print("MULTI_MIC_TILLER");
  lcd.setCursor(0, 1);
  lcd.print("AUTO_MODE");
  lcd.setCursor(0, 2);
  lcd.print("OBJECT_DETECT");
  delay(300);
  
  // ----- FRONT BLOCKED -----
  stop_robot();
  delay(1000);
  up();
  delay(1000);
  rotateoff();
  delay(3000);
}

// =======================================================
//                 ULTRASONIC FUNCTION
// =======================================================
int getDistance() {

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 30000);
  if (duration == 0) 
  {
    return 999;
  }

  int dist = duration * 0.034 / 2;

  Serial.print("Distance: ");
  Serial.println(dist);
  
  return dist;

}



// =======================================================
//                 MOVEMENT FUNCTIONS
// =======================================================
void forward_movement() {
  digitalWrite(motor1, HIGH);
  digitalWrite(motor2, LOW);
  digitalWrite(motor3, HIGH);
  digitalWrite(motor4, LOW);
}

void back_movement() {
  digitalWrite(motor1, LOW);
  digitalWrite(motor2, HIGH);
  digitalWrite(motor3, LOW);
  digitalWrite(motor4, HIGH);
}

void left_movement() {
  digitalWrite(motor1, HIGH);
  digitalWrite(motor2, LOW);
  digitalWrite(motor3, LOW);
  digitalWrite(motor4, HIGH);
}

void right_movement() {
  digitalWrite(motor1, LOW);
  digitalWrite(motor2, HIGH);
  digitalWrite(motor3, HIGH);
  digitalWrite(motor4, LOW);
  digitalWrite(motor5, LOW);
  digitalWrite(motor6, LOW);
}

void stop_robot() {
  digitalWrite(motor1, LOW);
  digitalWrite(motor2, LOW);
  digitalWrite(motor3, LOW);
  digitalWrite(motor4, LOW);
  digitalWrite(motor5, LOW);
  digitalWrite(motor6, LOW);
  digitalWrite(motor7, LOW);
  digitalWrite(motor8, LOW);
}

void up() {
  digitalWrite(motor5, LOW);
  digitalWrite(motor6, HIGH);
}

void down() {
  digitalWrite(motor5, HIGH);
  digitalWrite(motor6, LOW);
}

void rotateon() {
  digitalWrite(motor7, HIGH);
  
}

void rotateoff() {
  digitalWrite(motor7, LOW);
}