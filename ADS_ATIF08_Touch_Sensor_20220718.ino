// ADS Touch Sensor Test Example Program (IC P/N:ATIF08 )
// Code: 
// Date: 2022.07.18  Ver.: 0.0.5
// H/W Target: ARDUINO UNO R3 / Leonardo, S/W: Arduino IDE  1.8.19
// Author: Park, Byoungbae (yni2yni@hanmail.net)
// Note: More information? Please send to e-mail.
// Uno R3, A4:SDA, A5: SCL, Leonardo 2:SDA,3:SCL

#include <Wire.h>

#define LF        0x0A //New Line
#define CR        0x0D //Carriage  return
#define SP        0x20 //Space

#define Ch_enable  0x01 //Touch Channel Enable/Disable
#define Clock_ctrl  0x34 //Clock Control Register (init_cal_opt, clk_sel, rb_sel)
#define Global_ctrl1 0x36 //Global Option Control Register1 
          //(response_off_ctrl, response_ctrl, bf_mode, Software Reet)
#define State_count 0x37 //Cal_pre_scale
#define Global_ctrl2 0x38 //Global Option Control Register2
              //(imp_sel,Single/Multi ,Cal_Hold_time,clock_off)
              
// Sensitivity level (threshold, Register Value X 0.025% = (1 Step=0.025%)              
#define Sensitivity1  0x39 //ch1,Default: 0x1C X 0.025% = 0.70% (threshold)
#define Sensitivity2  0x3A //ch2
#define Sensitivity3  0x3B //ch3
#define Sensitivity4  0x3C //ch4
#define Sensitivity5  0x3D //ch5
#define Sensitivity6  0x3E //ch6
#define Sensitivity7  0x3F //ch7
#define Sensitivity8  0x40 //ch8

#define Cal_speed1 0x41 //Calibration Speed Control at BF mode
#define Cal_speed2 0x42 //Calibration Speed Control at BS mode

#define PWM_ctrl1 0x43 //LED PWM control Register (D2,D1)
#define PWM_ctrl2 0x44 //LED PWM control Register (D4,D3)
#define PWM_ctrl3 0x45 //LED PWM control Register (D6,D5)
#define PWM_ctrl4 0x46 //LED PWM control Register (D8,D7)

#define Port_Mode 0x4F //Select the output port operation mode of each channel.
#define Lock_Control 0x48 // data 0x00: Unlock, 0x60 or 0x40 or 0x20: locking

#define Output  0x2A //Touch Output Data Register 1Byte (1~8Key)

#define ATIF08_ID  0x24 //0x48>>1 (7bit=0x24, 8bit=0x48, Chip_DI=VDD or Open)
// #define ATIF08_ID  0x7C //0xF8>>1 (7bit=0x7C, 8bit=0xF8, Chip_DI= GND)

void  Init_ATIF08(void); //Initialize ATIF08

#define RESET_PIN 7 //Reset pin
#define EN_PIN    6 //I2C Enable Pin

void Register_Dump()
{
   byte read_data[1] = {0};

   for (int i = 0; i < 256; i += 16)
   {
      for (int j = 0; j <= 15; j++)
      {
         Wire.beginTransmission(ATIF08_ID);   // sned ic slave address
         Wire.write((i + j));                 // sends register address
         Wire.endTransmission();              // stop transmitting
         Wire.requestFrom(ATIF08_ID, 1);      // key data read (1 byte)
         read_data[0] = Wire.read();          //Key 1~8
         Wire.endTransmission();
         print2hex(read_data, 1); //
      }
      Serial.write(LF);
      Serial.write(CR);
   }
   delay(1500);
}

void print2hex(byte *data, byte length) //Print Hex
{
       Serial.print("0x"); 
       for (int i=0; i<length; i++) { 
         if (data[i]<0x10) {Serial.print("0");} 
         Serial.print(data[i],HEX); 
         Serial.write(SP);          
       }
}

void setup(){
  delay(100); //wait for 100[msec]
  
  Wire.begin();        // join i2c bus (address optional for master)
  Wire.setClock(2000000); //I2C Speed 200Khz
  Serial.begin(115200);  // start serial for output (Speed)
  // put your setup code here, to run once:
 
  pinMode(RESET_PIN, OUTPUT); //Reset Pin
  pinMode(EN_PIN, OUTPUT); // Enable Pin
  
  // ----------------------------------------------------------------------------------
  delay(400); //wait 400[msec] , This value is the time required to operate the ATIF08.
  Init_ATIF08(); //Initialize ATIF08
  delay(10); //wait for 10[msec]
    
}
void loop() {

  byte read_data[2]={0};
 
  Wire.beginTransmission(ATIF08_ID); // sned ic slave address
  Wire.write(byte(Output)); // sends register address
  Wire.endTransmission(); // stop transmitting
  Wire.requestFrom(ATIF08_ID,1); // key data read (1 byte)
  read_data[0]=Wire.read(); //Key 1~8
  Wire.endTransmission(); // 
   
  Serial.write(10); 
  Serial.print(" Touch Sensor Output Data (Hex) ---- > ");// Test Code
  
  print2hex(read_data,1);
  //Serial.print(read_data,HEX); 
  //Serial.write(SP); 
  //Serial.write(LF);   
  //Serial.write(CR);
  //Register_Dump();
  delay(50);
}

void  Init_ATIF08(void)
{

   //-------------------- Register write Lock control ------------------------
   Wire.beginTransmission(ATIF08_ID); //
   Wire.write(byte(Lock_Control));    // address
   Wire.write(byte(0x00));            // data 0x00: Unlock, 0x60 or 0x40 or 0x20: locking
   Wire.endTransmission();            //

   //------------------ Software Reset Enable (Set)----------------------
   Wire.beginTransmission(ATIF08_ID); // 
   Wire.write(byte(Global_ctrl1)); // 
   Wire.write(byte(0x4D)); // 
   Wire.endTransmission(); // 
   
// --------------- Hidden Register Start ---------------------------------
// user does not change the register. if you want change value, please contact to us.
// -----------------------------------------------------------------------
   Wire.beginTransmission(ATIF08_ID);// 
   Wire.write(byte(0x05)); // address
   Wire.write(byte(0x80)); // data
   Wire.endTransmission(); //   

   Wire.beginTransmission(ATIF08_ID);// 
   Wire.write(byte(0x08)); // address
   Wire.write(byte(0x11)); //0x08h
   Wire.write(byte(0x11)); //0x09h
   Wire.write(byte(0x11)); //0x0Ah
   Wire.write(byte(0x11)); //0x0Bh
   Wire.write(byte(0x11)); //0x0Ch
   Wire.write(byte(0x11)); //0x0Dh
   Wire.write(byte(0x01)); //0x0Eh
   Wire.write(byte(0x10)); //0x0Fh   
   Wire.endTransmission(); //   

   Wire.beginTransmission(ATIF08_ID);// 
   Wire.write(byte(0x10)); // address
   Wire.write(byte(0xCE)); //0x10h Default: 0xF5
   Wire.write(byte(0x20)); //0x11h
   Wire.write(byte(0xFF)); //0x12h
//Wire.write(byte(0x88));  //0x12h
//Change the value of the register 0x12h when a problem occurs due to voltage drop
   Wire.write(byte(0x92)); //0x13h
   Wire.write(byte(0x88)); //0x14h //RF Noise Mode Control 8ch 
   Wire.write(byte(0x73)); //0x15h
   Wire.write(byte(0x64)); //0x16h
   Wire.write(byte(0xFF)); //0x17h 
   Wire.write(byte(0x2B)); //0x18h  
   Wire.write(byte(0x11)); //0x19h 
   Wire.write(byte(0x03)); //0x1Ah  Default: 0x01
   Wire.write(byte(0xFF)); //0x1Bh  
   Wire.write(byte(0x11)); //0x1Ch  
   Wire.endTransmission(); //  
/* // do not control
   Wire.beginTransmission(ATIF08_ID);// 
   Wire.write(byte(0x20)); // address 
   Wire.write(byte(0x00)); // 0x20h data Default: 0x00
   Wire.write(byte(0x04)); //0x21h
   Wire.write(byte(0x04)); //0x22h 
   Wire.write(byte(0x04)); //0x23h
   Wire.write(byte(0x04)); //0x24h  
   Wire.write(byte(0x04)); //0x25h  
   Wire.write(byte(0x04)); //0x26h  
   Wire.write(byte(0x04)); //0x27h  
   Wire.write(byte(0x04)); //0x28h  
   Wire.write(byte(0x04)); //0x29h  
   Wire.endTransmission(); // 
*/
   Wire.beginTransmission(ATIF08_ID);// 
   Wire.write(byte(0x35)); // address
   Wire.write(byte(0xC0)); // Default Value
   // Wire.write(byte(0x40));// Change to High Band Sensing Frequncy 
   Wire.endTransmission(); //   

/* Change if arrected by low frequency noise error. 
   Wire.beginTransmission(ATIF08_ID);// 
   Wire.write(byte(0x35)); // address
   Wire.write(byte(0x40)); // data
   Wire.endTransmission(); //   
*/
   Wire.beginTransmission(ATIF08_ID);// 
   Wire.write(byte(0x47)); // address
   Wire.write(byte(0x0D)); // data
   Wire.endTransmission(); //   
   
   Wire.beginTransmission(ATIF08_ID);// 
   Wire.write(byte(0x49)); // address
   Wire.write(byte(0x80)); // data
   Wire.endTransmission(); //    

   Wire.beginTransmission(ATIF08_ID);// 
   Wire.write(byte(0x4A)); // address
   Wire.write(byte(0x09)); // data
   Wire.endTransmission(); //    
   
   Wire.beginTransmission(ATIF08_ID);// 
   Wire.write(byte(0x59)); // address
   Wire.write(byte(0x00)); // data
   Wire.endTransmission(); //    
   
   Wire.beginTransmission(ATIF08_ID);// 
   Wire.write(byte(0x5B)); // address
   Wire.write(byte(0x00)); // data
   Wire.endTransmission(); //    
   
   Wire.beginTransmission(ATIF08_ID);// 
   Wire.write(byte(0x5C)); // address
   Wire.write(byte(0x00)); // data
   Wire.endTransmission(); //    

   Wire.beginTransmission(ATIF08_ID);// 
   Wire.write(byte(0x62)); // address
   Wire.write(byte(0x00)); // data
   Wire.endTransmission(); //    

   Wire.beginTransmission(ATIF08_ID);// 
   Wire.write(byte(0x63)); // address
   Wire.write(byte(0x00)); // data
   Wire.endTransmission(); // 
// --------------- Hidden Register End-------------------------------

// ---------------- User Control Resgiter ---------------------//   
   Wire.beginTransmission(ATIF08_ID);// 
   Wire.write(byte(Ch_enable)); // 0x01h
   Wire.write(0xFF); // 0:Touch Key CHx Disable, 1: Touch Key CHx Enable
   Wire.endTransmission(); // 

   Wire.beginTransmission(ATIF08_ID);// 
   Wire.write(byte(Clock_ctrl)); //  0x34h
   Wire.write(0x65); // RB Normal -> Fast
   Wire.endTransmission(); // 

   Wire.beginTransmission(ATIF08_ID);// 
   Wire.write(byte(Global_ctrl1)); // 0x36h 
   Wire.write(0x4D); // 
   Wire.endTransmission(); // 

   Wire.beginTransmission(ATIF08_ID);// 
   Wire.write(byte(State_count)); // 0x37h
   Wire.write(0xE6); // Calibration speed prescaler: fast
   Wire.endTransmission(); //   
   
   Wire.beginTransmission(ATIF08_ID);// 
   Wire.write(byte(Global_ctrl2)); // 0x38h
   Wire.write(0xBC); // 
   Wire.endTransmission(); //  
   
//------------ Sensitivity control  -----------------------------------
// touch output threshold level:
   Wire.beginTransmission(ATIF08_ID);// 
   Wire.write(byte(Sensitivity1)); // 0x39h
   Wire.write(0x24); // HEX x 0.025% = 0.90%
   Wire.endTransmission(); //      

   Wire.beginTransmission(ATIF08_ID);// 
   Wire.write(byte(Sensitivity2)); // 0x3Ah
   Wire.write(0x24); // HEX x 0.025% = 0.90%
   Wire.endTransmission(); //   
   
   Wire.beginTransmission(ATIF08_ID);// 
   Wire.write(byte(Sensitivity3)); // 0x3Bh
   Wire.write(0x24); //  HEX x 0.025% = 0.90%
   Wire.endTransmission(); //   
   
   Wire.beginTransmission(ATIF08_ID);// 
   Wire.write(byte(Sensitivity4)); // 0x3Ch
   Wire.write(0x24); //  HEX x 0.025% = 0.90%
   Wire.endTransmission(); //   
   
   Wire.beginTransmission(ATIF08_ID);// 
   Wire.write(byte(Sensitivity5)); // 0x3Dh
   Wire.write(0x24); //  HEX x 0.025% = 0.90%
   Wire.endTransmission(); //   
   
   Wire.beginTransmission(ATIF08_ID);// 
   Wire.write(byte(Sensitivity6)); // 0x3Eh
   Wire.write(0x24); //  HEX x 0.025% = 0.90%
   Wire.endTransmission(); //   
   
   Wire.beginTransmission(ATIF08_ID);// 
   Wire.write(byte(Sensitivity7)); // 0x3Fh
   Wire.write(0x24); //  HEX x 0.025% = 0.90%
   Wire.endTransmission(); //   
   
   Wire.beginTransmission(ATIF08_ID);// 
   Wire.write(byte(Sensitivity8)); // 0x40h
   Wire.write(0x24); // HEX x 0.025% = 0.90%
   Wire.endTransmission(); //  
 
// ------------ Calibration Speed Control ------------------------ 
   Wire.beginTransmission(ATIF08_ID);// 
   Wire.write(byte(Cal_speed1)); // 0x41h
   Wire.write(0x66); // Calibration Speed
   Wire.endTransmission(); //   
   
   Wire.beginTransmission(ATIF08_ID);// 
   Wire.write(byte(Cal_speed2)); // 0x42h
   Wire.write(0x66); // Calibration Speed
   Wire.endTransmission(); //   
// -------------------- LED PWM Control -----------------------------
   Wire.beginTransmission(ATIF08_ID);// 
   Wire.write(byte(PWM_ctrl1)); // 0x43h
   Wire.write(0x00); // D2,D1
   Wire.endTransmission(); //

   Wire.beginTransmission(ATIF08_ID);// 
   Wire.write(byte(PWM_ctrl2)); // 0x44h
   Wire.write(0x00); // D4,D3
   Wire.endTransmission(); //

   Wire.beginTransmission(ATIF08_ID);// 
   Wire.write(byte(PWM_ctrl3)); // 0x45h
   Wire.write(0x00); // D6,D5
   Wire.endTransmission(); //

   Wire.beginTransmission(ATIF08_ID);// 
   Wire.write(byte(PWM_ctrl4)); // 0x46h
   Wire.write(0x00); // D8,D7
   Wire.endTransmission(); //
// ---------------- Port Mode Control Register --------------------------
   Wire.beginTransmission(ATIF08_ID);// 
   Wire.write(byte(Port_Mode)); // 0x4Fh
   Wire.write(0x00); // 0: Parallel output mode, 1: LED Drive mode
   Wire.endTransmission(); //

//------------------ Software Reset Disable (Clear) ---------------------
   Wire.beginTransmission(ATIF08_ID); // 
   Wire.write(byte(Global_ctrl1)); // Soft Reset Disable
   Wire.write(byte(0x4C)); // 
   Wire.endTransmission(); // 

//-------------------- Register write Lock control ------------------------
   Wire.beginTransmission(ATIF08_ID);// 
   Wire.write(byte(Lock_Control)); // address
   Wire.write(byte(0x20)); // data 0x00: Unlock, 0x60 or 0x40 or 0x20: locking 
   Wire.endTransmission(); //   
   
   }
// End
