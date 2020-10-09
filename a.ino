#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include "AD9833.h"
LiquidCrystal_I2C lcd(0x3f, 16, 2);

AD9833 GenerateSignal(10, 25000000);

int btn[5]={3,4,5,6,7};//Pin allocation
int Statebtn[5]={0,0,0,0,0};//for checking the button states (Whether it is pressed or not)
int ChooseDisplay=0;
bool OkPressed=false;
int s;//variable to save potentiometer reading
int Freque=1;
int WaveType=0;
unsigned long FreqDigits[5]={0,0,0,0,1};
int CountForStart=0;

int AmpRead;
int AmpMag=1;

int AmpMag1,AmpMag2;
int AmpDigits[3]={0,0,0};
 
void setup()
{
  Serial.begin(9600);//Serial for check the accuracy of the inputs
  pinMode(btn[0], INPUT);//Declaration for the buttons as input buttons
  pinMode(btn[1], INPUT);
  pinMode(btn[2], INPUT);
  pinMode(btn[3], INPUT);
  pinMode(btn[4], INPUT);
  AmpRead=analogRead(A1);
  AmpMag1 = (AmpRead*30)/1023;
  AmpMag2 = (AmpRead*20)/1023;
  AmpMag=AmpMag1+AmpMag2;
 
  
  
}

void GenWave(){//Call this function after every function
  GenerateSignal.reset(1);
  GenerateSignal.setFreq(Freque);
  GenerateSignal.setPhase(0);
  GenerateSignal.setFPRegister(1);
  GenerateSignal.setFreq(Freque);
  GenerateSignal.setPhase(0);
  GenerateSignal.setFPRegister(0);
  GenerateSignal.mode(WaveType);
  GenerateSignal.reset(0);
  
  while(CountForStart%2==1){
    Statebtn[3] = digitalRead(btn[3]);
    lcd.begin();
    lcd.backlight();
    lcd.setCursor(0,0);
    lcd.print("Running...");
    if(Statebtn[3]==HIGH){
    delay(500);
    CountForStart++;
    loop();
  }
    
  }
  
  
  
  
}

void FreAmp(){

  AmpRead=analogRead(A1);
  Serial.print("Reading is : ");
  Serial.println(AmpRead);
  int lrselection=0;
  lcd.begin();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print(">");
  lcd.print("FREQ : ");
  for(int i=0;i<5;i++){
    lcd.print(FreqDigits[i]);
  }
  lcd.print("Hz");
  lcd.setCursor(0,1);
  lcd.print(" AMPL : ");
  int temAmp=AmpMag;
   if(WaveType==0 || WaveType==1){
    /////////////////////////////////////////////////////
    AmpDigits[2]=temAmp%10;
    temAmp/=10;
    AmpDigits[1]=temAmp%10;
    temAmp/=10;
    AmpDigits[0]=temAmp%10;
    
    
  }
  else{
    temAmp*=2;
    AmpDigits[2]=temAmp%10;
    temAmp/=10;
    AmpDigits[1]=temAmp%10;
    temAmp/=10;
    AmpDigits[0]=temAmp%10;
  }
  lcd.print(AmpDigits[0]);
  lcd.print(AmpDigits[1]);
  lcd.print(".");
  lcd.print(AmpDigits[2]);
  lcd.print("V");
   while(OkPressed==true){
   
    
    delay(500);
    int FAOAK=digitalRead(btn[3]);
 
  
      
      lcd.begin();
      lcd.backlight();
      lcd.setCursor(0,0);
      lcd.print(">");
      lcd.print("FREQ : ");
      for(int i=0;i<5;i++){
        lcd.print(FreqDigits[i]);
      }
      lcd.print("Hz");
      lcd.setCursor(0,1);
      lcd.print(" AMPL : ");
      lcd.print(AmpDigits[0]);
      lcd.print(AmpDigits[1]);
      lcd.print(".");
      lcd.print(AmpDigits[2]);
      lcd.print("V");
      s=analogRead(A0);//Potentiometer reading
      
      int FAleft=digitalRead(btn[4]);
      int FAright=digitalRead(btn[1]);
      if(FAleft==HIGH){
        lrselection--;
        Serial.println("LR left pressed");
      }
      if(FAright==HIGH){
        lrselection++;
        Serial.println("LR Right pressed");
      }
      Serial.print("LR selection value : ");
      Serial.println(lrselection);
      switch (abs(lrselection)%5){
        case 0:
        if(FreqDigits[3]==0&&FreqDigits[2]==0&&FreqDigits[1]==0&&FreqDigits[0]==0){
        FreqDigits[4]=(s*8/1023)+1;
        }
        else{
          FreqDigits[4]=(s*9/1023);
        }
        
        break;
        case 1:
        FreqDigits[3]=(s*9/1023);
        break;
        case 2:
        FreqDigits[2]=(s*9/1023);
        break;
        case 3:
        FreqDigits[1]=(s*9/1023);
        break;
        case 4:
        FreqDigits[0]=(s*1/1023);
        break;
        default:
        Serial.print("Error in lrselection\n");
        break;
      }
      Freque=0;
      for(int i=5;i>=0;i--){
        Freque+=FreqDigits[i]*pow(10,i);
      }
      //Serial.println(Freque);
      /*Serial.print(FreqDigits[0]);
      Serial.print(FreqDigits[1]);
      Serial.print(FreqDigits[2]);
      Serial.print(FreqDigits[3]);
      Serial.println(FreqDigits[4]);*/
      Freque=FreqDigits[0]*10000+FreqDigits[1]*1000+FreqDigits[2]*100+FreqDigits[3]*10+FreqDigits[4];
      Serial.print("Answer is : ");
      Serial.println(Freque);

     
    
 if(FAOAK==HIGH){
  Serial.print("FAOK is HIGH ");
    OkPressed=false;
    FreAmp();
    
  }
    
  }
}

void TypeFreq(){
 
  lcd.begin();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print(">");
  lcd.print("TYPE : ");
  switch(WaveType){
      case 0:
      lcd.print("SINUSOID");
      break;
      case 1:
      lcd.print("TRIANGU");
      break;
      case 2:
      lcd.print("SQUARE");
      break;
      default:
      break;
    }
  lcd.setCursor(0,1);
   lcd.print(" FREQ : ");
  for(int i=0;i<5;i++){
    lcd.print(FreqDigits[i]);
  }
  lcd.print("Hz");


  while(OkPressed==true){
   

    delay(500);
    int TSOK=digitalRead(btn[3]);
 
    
    s=analogRead(A0);
    lcd.begin();
    lcd.backlight();
    lcd.setCursor(0,0);
    lcd.print(">");
    lcd.print("TYPE : ");
    WaveType=s*2/1023;
    switch(WaveType){
      case 0:
      lcd.print("SINUSOID");
      break;
      case 1:
      lcd.print("TRIANGU");
      break;
      case 2:
      lcd.print("SQUARE");
      break;
      default:
      break;
    }


    
    lcd.setCursor(0,1);
     lcd.print(" FREQ : ");
    for(int i=0;i<5;i++){
      lcd.print(FreqDigits[i]);
   } 
    lcd.print("Hz");



    if(TSOK==HIGH){
  Serial.print("TSOK is HIGH ");
    OkPressed=false;
    TypeFreq();
    
  }
   }
}

void AmpStart(){
 
  lcd.begin();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print(">");
  lcd.print("AMPL : ");

  //////////////////////////
  AmpRead=analogRead(A1);
  //Serial.print("Reading is : ");
  //Serial.println(AmpRead);
  //For Square reading 57->5V
  // 353->10V 
  //For Sinusoid
  //For Triangular 0-5V
  //AmpMag=(AmpRead*50)/1023;

  int AmpMag1 = (AmpRead*30)/1023;
  int AmpMag2 = (AmpRead*20)/1023;
  AmpMag=AmpMag1+AmpMag2;
  int temAmp=AmpMag;

  if(WaveType==0 || WaveType==1){
    /////////////////////////////////////////////////////
    AmpDigits[2]=temAmp%10;
    temAmp/=10;
    AmpDigits[1]=temAmp%10;
    temAmp/=10;
    AmpDigits[0]=temAmp%10;
    
    
  }
  else{
    temAmp*=2;
    AmpDigits[2]=temAmp%10;
    temAmp/=10;
    AmpDigits[1]=temAmp%10;
    temAmp/=10;
    AmpDigits[0]=temAmp%10;
  }
 
  Serial.print("Reading 1 is  : ");
  Serial.println(AmpRead);
  Serial.print("Reading is : ");
  Serial.println(AmpMag);
  lcd.print(AmpDigits[0]);
  lcd.print(AmpDigits[1]);
  lcd.print(".");
  lcd.print(AmpDigits[2]);
  lcd.print("V");

  lcd.setCursor(0,1);
 lcd.print(" START  ");
}

void StartType(){
  Statebtn[3] = digitalRead(btn[3]);
  if(Statebtn[3]==HIGH){
    delay(500);
    CountForStart++;
  }
  if(CountForStart%2==1){
    GenWave();
    
  }
  
  lcd.begin();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print(">");
  lcd.print("START  ");

 lcd.setCursor(0,1);
  lcd.print(" TYPE : ");
  switch(WaveType){
      case 0:
      lcd.print("SINUSOID");
      break;
      case 1:
      lcd.print("TRIANGU");
      break;
      case 2:
      lcd.print("SQUARE");
      break;
      default:
      break;
    }

  
}
void Check(int ch){
  if(ch%4==0){TypeFreq();}
  else if(ch%4==1){FreAmp();}
  else if(ch%4==2){AmpStart();}
  else{StartType();}
  
}

void loop()
{
 
  Statebtn[0] = digitalRead(btn[0]);
  Statebtn[1] = digitalRead(btn[1]);
  Statebtn[2] = digitalRead(btn[2]);
  Statebtn[3] = digitalRead(btn[3]);
  Statebtn[4] = digitalRead(btn[4]);
  s=analogRead(A0);//Potentiometer reading
  if(Statebtn[0]==HIGH){
    Serial.println("UP");
    //lcd.print("UP");
    ChooseDisplay++;
  }
  if(Statebtn[1]==HIGH){
    Serial.println("RIGHT");
    //lcd.print("RIGHT");
  }
  if(Statebtn[2]==HIGH){
    Serial.println("DOWN");
    //lcd.print("DOWN");
    ChooseDisplay--;
  }
  if(Statebtn[3]==HIGH){
    Serial.println("OK");
    lcd.print("OK");
    OkPressed=true;
  }
  if(Statebtn[4]==HIGH){
    Serial.println("LEFT");
    //lcd.print("LEFT");
  }

  //Serial.println(s);
  //lcd.print(s);
  Check(abs(ChooseDisplay));
  
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   
  
  
}
