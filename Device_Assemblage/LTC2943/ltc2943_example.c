/*
确定 IIC 通讯 初始化 成功，可以用逻辑分析仪 看一下 时序。
我用的 硬件 IIC
先初始化IIC  然后  设置    0x01;    LTC2943_WriteByte(0x01,0xFC);  
然后
double D_DC_V=0.;
D_DC_V   = GET_LTC2943_voltage();
就可以读取数据了

每次上电，必须先设置上下限，才能实现 警报  功能。因为，这个芯片没有内部的flash。

*/
/************************************************
函数名称 ：GET_LTC2943_mAh
功    能 ：返回 电池容量
参    数 ：
            resistor  ： 外接电阻 阻值 单位欧姆
            prescalar ： M 的 分频 设置值
返 回 值 ： 浮点数 数值 mAh
作    者 ：
*************************************************/
float GET_LTC2943_mAh(float resistor, uint16_t prescalar)
// The function converts the 16-bit RAW adc_code to mAh
{
  uint8_t adc_code_M;
  uint8_t adc_code_L;
  uint16_t adc_code;
 
  float mAh_charge;
  LTC2943_ReadByte(LTC2943_ACCUM_CHARGE_MSB_REG, &adc_code_M);
  LTC2943_ReadByte(LTC2943_ACCUM_CHARGE_LSB_REG, &adc_code_L);
  adc_code  = (((adc_code_M << 8) & 0xFF00)) + ((adc_code_L)&(0X00FF));
 
  mAh_charge = 1000*(float)(adc_code*LTC2943_CHARGE_lsb*prescalar*50E-3)/(resistor*4096);
  return(mAh_charge);
}
/************************************************
函数名称 ： LTC2943_code_to_voltage
功    能 ： 返回 电池 电压
参    数 ：
返 回 值 ： 浮点数 数值  V
作    者 ：
*************************************************/
double GET_LTC2943_voltage(void)
// The function converts the 16-bit RAW adc_code to Volts
{
  uint8_t adc_code_M;
  uint8_t adc_code_L;
  uint16_t adc_code;
  double voltage;
 
  LTC2943_ReadByte(LTC2943_VOLTAGE_MSB_REG , &adc_code_M);
  LTC2943_ReadByte(LTC2943_VOLTAGE_LSB_REG , &adc_code_L);
  adc_code  = (((adc_code_M << 8) & 0xFF00)) + ((adc_code_L)&(0X00FF));
 
  voltage = ((double)adc_code/(65535))*LTC2943_FULLSCALE_VOLTAGE;
 
  return(voltage);
}
 
/************************************************
函数名称 ：GET_LTC2943_current
功    能 ：返回 当前 电流值
参    数 ：
           resistor  ： 外接电阻 阻值 单位欧姆
返 回 值 ：
作    者 ：
*************************************************/
float GET_LTC2943_current(float resistor)
// The function converts the 16-bit RAW adc_code to Amperes
{
  uint8_t adc_code_M;
  uint8_t adc_code_L;
  uint16_t adc_code;
  float current;
 
  LTC2943_ReadByte(LTC2943_CURRENT_MSB_REG , &adc_code_M);
  LTC2943_ReadByte(LTC2943_CURRENT_LSB_REG , &adc_code_L);
  adc_code  = (((adc_code_M << 8) & 0xFF00)) + ((adc_code_L)&(0X00FF));
 
  current = (((float)adc_code-32767)/(32767))*((float)(LTC2943_FULLSCALE_CURRENT)/resistor);
  return(current);
}
 
/************************************************
函数名称 ：GET_LTC2943kelvin_temperature
功    能 ：返回 当前温度值
参    数 ：
返 回 值 ：浮点数 数值   K
作    者 ：
*************************************************/
float GET_LTC2943kelvin_temperature(void)
// The function converts the 16-bit RAW adc_code to Kelvin
{
  uint8_t adc_code_M;
  uint8_t adc_code_L;
  uint16_t adc_code;
  float temperature;
 
  LTC2943_ReadByte(LTC2943_TEMPERATURE_MSB_REG   , &adc_code_M);
  LTC2943_ReadByte(LTC2943_TEMPERATURE_LSB_REG   , &adc_code_L);
  adc_code  = (((adc_code_M << 8) & 0xFF00)) + ((adc_code_L)&(0X00FF));
 
 
  temperature = adc_code*((float)(LTC2943_FULLSCALE_TEMPERATURE)/65535);
  return(temperature);
}
/************************************************
函数名称 ：GET_LTC2943_temperature
功    能 ：返回 当前温度值
参    数 ：
返 回 值 ：浮点数 数值   ℃
作    者 ：
*************************************************/
float GET_LTC2943_temperature(void)
// The function converts the 16-bit RAW adc_code to Celcius
{
  uint8_t adc_code_M;
  uint8_t adc_code_L;
  uint16_t adc_code;
  float temperature;
 
  LTC2943_ReadByte(LTC2943_TEMPERATURE_MSB_REG   , &adc_code_M);
  LTC2943_ReadByte(LTC2943_TEMPERATURE_LSB_REG   , &adc_code_L);
  adc_code  = (((adc_code_M << 8) & 0xFF00)) + ((adc_code_L)&(0X00FF));
 
  temperature = (adc_code*((float)(LTC2943_FULLSCALE_TEMPERATURE/65535))) - 273.15;
 
  return(temperature);
}


//读状态寄存器
void LinearGauge_ReadSTA(void)   
{     
  u8 LinearGaugeVolt_LSB = 0;                            
  IIC_Start();  
  IIC_Send_Byte(0xc8);
  IIC_Wait_Ack();
  IIC_Send_Byte(0x00);
  IIC_Wait_Ack();  
        IIC_Start();         
  IIC_Send_Byte(0xc9);                             
  IIC_Wait_Ack();              
  LinearGaugeVolt_LSB=IIC_Read_Byte(0);       
  IIC_Stop();   
}



