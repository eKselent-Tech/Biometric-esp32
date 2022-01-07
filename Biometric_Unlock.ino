#include<Servo.h>
Servo myservo;  
int pos= 0, pos_set = 100;
#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 3);
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
unsigned char i = 0;
String MSG = "", Receive_MSG = "", in_str = "",fids="";
char fid;
#define en_sw    4
#define ver_sw   5
#define buz  12
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
bool ch_status=false;
int e,v,flag1=0,flag2=0;
uint8_t id=0;
int getFingerprintIDez();
int p = -1,fp_scan=0;
//////////////////////////////////////////////////
//http://3.17.74.57/MRCET/FP_Auth/exams.php?Student_id=002&Attend=PRESENT

//////////////////////////////////////////////////

void setup()  
{
  myservo.attach(6);  // attaches the servo on pin 9 to the servo object
  myservo.write(pos);

  pinMode(en_sw,INPUT_PULLUP);
  pinMode(ver_sw,INPUT_PULLUP);
  pinMode(buz,OUTPUT);
  Serial.begin(115200);
  lcd.begin();
  lcd.backlight();
  lcd.setCursor(4, 0);
  lcd.print(F("WELCOME"));
  lcd.setCursor(0, 1);
  lcd.print(F(" .............. "));
  delay(1000);
  lcd.clear();
  
  while (!Serial);  // For Yun/Leo/Micro/Zero/...
  delay(100);
  Serial.println(F("\n\nAdafruit Fingerprint sensor enrollment & Verify"));
  finger.begin(57600);
  
  if (finger.verifyPassword()) {
    Serial.println(F("Found fingerprint sensor!"));
      lcd.setCursor(0, 0);
      lcd.print(F("FOUND FP SENSOR "));delay(1500);

  } else {
    Serial.println(F("Did not find fingerprint sensor :("));
    lcd.setCursor(0, 0);
      lcd.print(F("FP NOT FOUND  "));
    while (1) { delay(1); }
  }
  if (Serial.available()) {
    fids = Serial.readString();
    //Serial.print(fid);
  if(fids="WiFi connected"){
    Serial.println("WIFI Connected");
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(F("WiFi Connected"));delay(1000);
  }
  }
 lcd.clear();
 delay(1000);
 lcd.setCursor(0,0);
 lcd.print(F("BIOMETRIC LOCK &"));
 lcd.setCursor(0,1);
 lcd.print(F("UNLOCK SYSTEM   "));
  
lcd_msg();
}

void lcd_msg(){
 flag2=0;
 delay(2000);
 lcd.clear();
 lcd.setCursor(0,0);
 lcd.print(F("PLS SCAN UR FACE"));
 lcd.setCursor(0,1);
 lcd.print(F("& FINGERPRINT   "));
}

uint8_t readnumber(void) {
  uint8_t num = 0;
  
  while (num == 0) {
    while (! Serial.available());
    num = Serial.parseInt();
  }
  return num;
}

void open_close(){
   for (pos = 0; pos <= 100; pos += 1) { 
      myservo.write(pos);        
      delay(15);}                
      delay(2000);
   lcd.setCursor(0,1);                                
   lcd.print(F("GATE: OPEN        "));            
      delay(500);
   for (pos = 100; pos >= 0; pos -= 1) { 
       myservo.write(pos);              
       delay(15);}                      
       delay(2000);              
   lcd.setCursor(0,1);                                
   lcd.print(F("GATE: CLOSE       "));            
                flag1=0;fid='z';
                lcd_msg();flag2=0;                            
}

void loop() {
  e=digitalRead(en_sw);
  v=digitalRead(ver_sw);
 // ch_status=false;
if(e==0){
  if(ch_status==false){
  id++;
  
      lcd.setCursor(0,0);
      lcd.print(F("Enrolling......."));
     // delay(500);
     Serial.print(F("Enrolling ID #"));
     Serial.println(id);
  while(p==-1)
  {
  p=getFingerprintEnroll();
  }
  ch_status=true;
  p=-1;
  }
else{
  ch_status=false;
  p=-1;
  }
}
//-------------------------------//
  fid;
  if (Serial.available()) {
    fid = Serial.read();
    //Serial.print(fid);
  }
  if(fid=='0' || fid=='1' || fid=='2' || fid=='3' || fid=='4' || fid=='5'){
            Serial.print("Face Identified");Serial.println(fid);
            lcd.setCursor(0,0);
            lcd.print(F("Face Identified "));
            lcd.setCursor(0,1);
            lcd.print(F("Scan UR Finger  "));
    flag1=1;
  }
  else if(fid=='9'){
    Serial.print("Unknown Face Identified");Serial.println(fid);
            lcd.setCursor(0,0);
            lcd.print(F("Unknown Face    "));
            lcd.setCursor(0,1);
            lcd.print(F("Identified      "));delay(1000);
  }
//-------------------------------//
if(flag1==1 && v==0){
  if(flag2<3){
            lcd.setCursor(0,0);
            lcd.print(F("Ver.Finger To Acc"));
            lcd.setCursor(0,1);
            lcd.print(F("                   "));
            delay(2000);
            finger.getTemplateCount();
            Serial.print(F("Sensor contains ")); Serial.print(finger.templateCount); 
            Serial.println(F(" templates"));
            Serial.println(F("Waiting for valid finger...")); 
            int ID;
            ID=getFingerprintIDez();//returns  Finger Id
            delay(1500);            //don't ned to run this at full speed.
          
       if(ID==1){
            Serial.println(F("Match Found With ID 1"));
            digitalWrite(buz,0);
            lcd.setCursor(0,1);                                
            lcd.print(F("FP MATCHED USER1  "));            
            open_close();
                }   
          else if(ID==2){
            Serial.println(F("Match Found With ID 2"));
            digitalWrite(buz,0);
            lcd.setCursor(0,1);                                
            lcd.print(F("FP MATCHED USER2  ")); 
            open_close();
                    } 
          else if(ID==3){
            Serial.println(F("Match Found With ID 3"));
            digitalWrite(buz,0);            
            lcd.setCursor(0,1);                                
            lcd.print(F("FP MATCHED USER3   ")); 
            open_close();
                   }
          else if(ID==4){
            Serial.println(F("Match Found With ID 4"));            
            digitalWrite(buz,0);            
            lcd.setCursor(0,1);                                
            lcd.print(F("FP MATCHED USER4   ")); 
            open_close();
                    }
          else if(ID==5){
            Serial.println(F("Match Found With ID 5"));            
            digitalWrite(buz,0);            
            lcd.setCursor(0,1);                                
            lcd.print(F("FP MATCHED USER5   ")); 
            open_close();
                      }
          else if(ID==6){
            Serial.println(F("Match Found With ID 6"));            
            digitalWrite(buz,0);            
            lcd.setCursor(0,1);                                
            lcd.print(F("FP MATCHED USER6   ")); 
            open_close();
                      }
          else{
            flag2++;
            Serial.println(F("Match Not Found"));
            digitalWrite(buz,1);//delay(1000);

                lcd.setCursor(0,0);
                lcd.print(F("MATCH NOT FOUND  "));
                lcd.setCursor(0,1);                                
                lcd.print(F("PLS. TRY AGAIN   "));
                delay(1000);
                digitalWrite(buz,0);
                }
             
             if(flag2==3){
             Serial.println("Exceed Limit   ");fid='z';
             lcd.setCursor(0,0);
             lcd.print(F("Exceed FPrint   "));
             lcd.setCursor(0,1);                                
             lcd.print(F("Scan Limit      "));lcd_msg(); 
            }
         }                   
      }
      // */ 
} // Loop End
/////////////////////////////////////////////////////////////////////////                          

//////////////  Enroll   ///////////////////

uint8_t getFingerprintEnroll() {

  
  Serial.print(F("Waiting for valid finger to enroll as #")); Serial.println(id);
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println(F("Image taken"));
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println(F("."));
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println(F("Communication error"));
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println(F("Imaging error"));
      break;
    default:
      Serial.println(F("Unknown error"));
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println(F("Image converted"));
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println(F("Image too messy"));
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println(F("Communication error"));
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println(F("Could not find fingerprint features"));
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println(F("Could not find fingerprint features"));
      return p;
    default:
      Serial.println(F("Unknown error"));
      return p;
  }
  
  Serial.println(F("Remove finger"));
  lcd.setCursor(0,0);
  lcd.print(F("Remove Finger   "));
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  Serial.print(F("ID ")); Serial.println(id);
  p = -1;
  Serial.println(F("Place same finger again"));
 // lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(F("Put Finger Again"));
  delay(3000);
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println(F("Image taken"));
      break;
    case FINGERPRINT_NOFINGER:
      Serial.print(F("."));
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println(F("Communication error"));
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println(F("Imaging error"));
      break;
    default:
      Serial.println(F("Unknown error"));
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println(F("Image converted"));
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println(F("Image too messy"));
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println(F("Communication error"));
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println(F("Could not find fingerprint features"));
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println(F("Could not find fingerprint features"));
      return p;
    default:
      Serial.println(F("Unknown error"));
      return p;
  }
  
  // OK converted!
  Serial.print(F("Creating model for #"));  Serial.println(id);
  
  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println(F("Prints matched!"));
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println(F("Communication error"));
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println(F("Fingerprints did not matches"));
    delay(1000);
    lcd.setCursor(0,0);
    lcd.print(F("FAIL TO ENROLL  "));id--;delay(1500);
    return p;
    lcd_msg();
  } else {
    Serial.println(F("Unknown error"));
    return p;
  }   
  
  Serial.print(F("ID ")); Serial.println(id);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    Serial.println(F("Stored!"));
 //   lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(F("Enroll Success "));lcd.print(id);delay(500);lcd_msg();
  return p;
  
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println(F("Communication error"));
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println(F("Could not store in that location"));
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println(F("Error writing to flash"));
    return p;
  } else {
    Serial.println(F("Unknown error"));
    return p;
  }   
}
//////////////  Enroll End  ///////////////////

//////////////   Verify    ///////////////////

uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println(F("Image taken"));
      lcd.setCursor(0,1);                                
      lcd.print(F("Image Taken"));
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println(F("No finger detected"));
      lcd.setCursor(0,1);                                
      lcd.print(F("NO FINGER      "));
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println(F("Communication error"));
      return p;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println(F("Imaging error"));
      return p;
    default:
      Serial.println(F("Unknown error"));
      return p;
  }

  // OK success!

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println(F("Image converted"));
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println(F("Image too messy"));
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println(F("Communication error"));
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println(F("Could not find fingerprint features"));
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println(F("Could not find fingerprint features"));
      return p;
    default:
      Serial.println(F("Unknown error"));
      return p;
  }
  
  // OK converted!
  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println(F("Found a print match!"));
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println(F("Communication error"));
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println(F("Did not find a match"));
    return p;
  } else {
    Serial.println(F("Unknown error"));
    return p;
  }   
  
  // found a match!
  Serial.print(F("Found ID #")); Serial.print(finger.fingerID); 
  Serial.print(F(" with confidence of ")); Serial.println(finger.confidence); 

  return finger.fingerID;
}

// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;
  
  // found a match!
  Serial.print(F("Found ID #")); Serial.print(finger.fingerID); 
  Serial.print(F(" with confidence of ")); Serial.println(finger.confidence);
  return finger.fingerID; 
}
////////////// Verify End ////////////////////
