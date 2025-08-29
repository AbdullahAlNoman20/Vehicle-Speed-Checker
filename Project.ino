#include <Wire.h> // Include the Wire library for I2C communication
#include <LiquidCrystal_I2C.h> // Include the library for I2C LCD

// Adjust the address and LCD dimensions to your specific hardware setup
LiquidCrystal_I2C lcd(0x27, 16, 2); // Address for I2C LCD (commonly 0x27 or 0x3F), and dimensions

int timer1;
int timer2;

float Time;

int flag1 = 0;
int flag2 = 0;

float distance = 10.0; // Distance between sensors in centimeters
float speed;

int ir_s1 = A0; // First IR sensor pin
int ir_s2 = A1; // Second IR sensor pin

int buzzer = 13; // Buzzer pin

// ----------- Login Credentials -----------
String correctUsername = "admin";   // Set your username
String correctPassword = "1234";    // Set your password
bool accessGranted = false;         // Flag for login access
// -----------------------------------------

void setup() {
  pinMode(ir_s1, INPUT); 
  pinMode(ir_s2, INPUT);
  pinMode(buzzer, OUTPUT); 
  
  lcd.init(); 
  lcd.backlight(); 
  lcd.clear(); 

  Serial.begin(9600); // Initialize Serial Monitor
  delay(500);

  // ----------- LOGIN SYSTEM -----------
  while (!accessGranted) {
    Serial.println("\n===== Login Required =====");
    Serial.print("Enter Username: ");

    while (Serial.available() == 0);  
    String username = Serial.readStringUntil('\n');
    username.trim();

    Serial.print("Enter Password: ");
    while (Serial.available() == 0);  
    String password = Serial.readStringUntil('\n');
    password.trim();

    if (username == correctUsername && password == correctPassword) {
      Serial.println("Access Granted! System Starting...\n");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Access Granted");
      delay(2000);
      lcd.clear();
      accessGranted = true;
    } else {
      Serial.println("Access Denied! Try again.\n");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Access Denied");
      delay(2000);
      lcd.clear();
    }
  }
  // -------------------------------------

  lcd.setCursor(0, 0); 
  lcd.print("WELCOME To "); 
  lcd.setCursor(0, 1); 
  lcd.print("Our Project");
  delay(2000); 
  lcd.clear(); 
}

void loop() {
  if (digitalRead(ir_s1) == LOW && flag1 == 0) { 
    timer1 = millis(); 
    flag1 = 1; 
  }

  if (digitalRead(ir_s2) == LOW && flag2 == 0) { 
    timer2 = millis(); 
    flag2 = 1; 
  }

  if (flag1 == 1 && flag2 == 1) { 
    Time = abs(timer1 - timer2) / 1000.0; 
    speed = (distance / Time) * 3600 / 1000; 

    lcd.clear(); 
    lcd.setCursor(0, 0); 
    lcd.print("Speed: ");
    lcd.print(speed, 1); 
    lcd.print(" Km/Hr");
    lcd.setCursor(0, 1); 

    if (speed > 50) { 
      lcd.print("Over Speeding"); 
      digitalWrite(buzzer, HIGH); 
      Serial.print("ALERT: Vehicle Speed = "); 
      Serial.print(speed, 1);
      Serial.println(" Km/Hr - OVER SPEEDING!"); 
    } else {
      lcd.print("Normal Speed"); 
      Serial.print("Vehicle Speed = ");
      Serial.print(speed, 1);
      Serial.println(" Km/Hr - Normal");
    }

    delay(3000); 
    digitalWrite(buzzer, LOW); 
    speed = 0; 
    flag1 = 0; 
    flag2 = 0; 
  } else {
    lcd.setCursor(0, 1); 
    if (flag1 == 0 && flag2 == 0) { 
      lcd.print("No car detected"); 
    } else {
      lcd.print("Searching...   "); 
    }
  }
}
