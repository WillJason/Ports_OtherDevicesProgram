/*
ȷ�� IIC ͨѶ ��ʼ�� �ɹ����������߼������� ��һ�� ʱ��
���õ� Ӳ�� IIC
�ȳ�ʼ��IIC  Ȼ��  ����    0x01;    LTC2943_WriteByte(0x01,0xFC);  
Ȼ��
double D_DC_V=0.;
D_DC_V   = GET_LTC2943_voltage();
�Ϳ��Զ�ȡ������

ÿ���ϵ磬���������������ޣ�����ʵ�� ����  ���ܡ���Ϊ�����оƬû���ڲ���flash��

*/
/************************************************
�������� ��GET_LTC2943_mAh
��    �� ������ �������
��    �� ��
            resistor  �� ��ӵ��� ��ֵ ��λŷķ
            prescalar �� M �� ��Ƶ ����ֵ
�� �� ֵ �� ������ ��ֵ mAh
��    �� ��
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
�������� �� LTC2943_code_to_voltage
��    �� �� ���� ��� ��ѹ
��    �� ��
�� �� ֵ �� ������ ��ֵ  V
��    �� ��
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
�������� ��GET_LTC2943_current
��    �� ������ ��ǰ ����ֵ
��    �� ��
           resistor  �� ��ӵ��� ��ֵ ��λŷķ
�� �� ֵ ��
��    �� ��
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
�������� ��GET_LTC2943kelvin_temperature
��    �� ������ ��ǰ�¶�ֵ
��    �� ��
�� �� ֵ �������� ��ֵ   K
��    �� ��
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
�������� ��GET_LTC2943_temperature
��    �� ������ ��ǰ�¶�ֵ
��    �� ��
�� �� ֵ �������� ��ֵ   ��
��    �� ��
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


//��״̬�Ĵ���
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



