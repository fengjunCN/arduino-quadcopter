/*
  Created by Basel Al-Rudainy, 6 april 2013.
  
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.
  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.
*/

#include "MPULib.h"
#include "Arduino.h"
#include "Wire.h"

MPULib::MPULib()
{

}

void MPULib::init(){
//init - ADXL345
writeCmd(ADXL_addr,REG_DATA_FORMAT,0x0B); //+-16g
writeCmd(ADXL_addr,REG_PWR_CTL,0x08);  //measurement mode
writeCmd(ADXL_addr,REG_BW_RATE,0x09);  //REG_BW_RATE rate=50hz, bw=20hz
//1G = 265
//-----end init ADXL345

//init - L3G4200D
writeCmd(L3G4_addr,CTRL_REG1,L3G4_BW_ENAX);
writeCmd(L3G4_addr,CTRL_REG2,L3G4_LPF);
writeCmd(L3G4_addr,CTRL_REG4,MODE_2000);
writeCmd(L3G4_addr,CTRL_REG5,L3G4_HPF);
//-----end init L3G4200D

//init - HMC5883
writeCmd(HMC_addr,HMC_mode_reg,HMC_contm_val);
//-----end init HMC5883
}

void MPULib::getAxlData(int16_t *axl_x, int16_t *axl_y, int16_t *axl_z){
byte buffer[6];
readCmd(ADXL_addr,DATAX0,6,buffer);
*axl_y=(buffer[1]<<8) | buffer[0];
*axl_x=(buffer[3]<<8) | buffer[2];
*axl_z=(buffer[5]<<8) | buffer[4];

}

void MPULib::getGyroData(float *gyro_x, float *gyro_y, float *gyro_z){
byte buffer[6];
readCmd(L3G4_addr,READALLSIX,6,buffer);
*gyro_x = (float)((int)(buffer[1]<<8) | buffer[0])*SCALE_2000;
*gyro_y = (float)-1*((int)(buffer[3]<<8) | buffer[2])*SCALE_2000;
*gyro_z = (float)((int)(buffer[5]<<8) | buffer[4])*SCALE_2000;
}

void MPULib::getMagData(int buff[]){
byte buffer[6];
readCmd(HMC_addr,HMC_X_MSB,6,buffer);
buff[0]=(buffer[0]<<8) | buffer[1];
buff[2]=(buffer[2]<<8) | buffer[3];
buff[1]=(buffer[4]<<8) | buffer[5];
}

void MPULib::readCmd(byte addr,byte reg,byte num,byte buff[]){
  Wire.beginTransmission(addr);
  Wire.write(reg);
  Wire.endTransmission();
  Wire.requestFrom(addr, num);
  while(Wire.available()<num);
  for(byte i = 0;i<num;i++){
  buff[i] = Wire.read();
  }
}

void MPULib::writeCmd(byte addr, byte reg, byte val){
  Wire.beginTransmission(addr);
  Wire.write(reg);
  Wire.write(val);
  Wire.endTransmission();
}
