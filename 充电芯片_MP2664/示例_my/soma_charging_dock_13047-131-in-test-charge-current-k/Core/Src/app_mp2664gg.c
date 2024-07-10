
#include "app_mp2664gg.h"


void selbattchannel(uint8_t isel)
{
   switch (isel)
   {
      case 0:
         LL_GPIO_ResetOutputPin( SEL0_GPIO_Port, SEL0_Pin);    
         LL_GPIO_ResetOutputPin( SEL1_GPIO_Port, SEL1_Pin);    
         LL_GPIO_ResetOutputPin( SEL2_GPIO_Port, SEL2_Pin);           
         break;

      case 1:
         LL_GPIO_SetOutputPin( SEL0_GPIO_Port, SEL0_Pin);    
         LL_GPIO_ResetOutputPin( SEL1_GPIO_Port, SEL1_Pin);    
         LL_GPIO_ResetOutputPin( SEL2_GPIO_Port, SEL2_Pin);           
         break;

      case 2:
         LL_GPIO_ResetOutputPin( SEL0_GPIO_Port, SEL0_Pin);    
         LL_GPIO_SetOutputPin( SEL1_GPIO_Port, SEL1_Pin);    
         LL_GPIO_ResetOutputPin( SEL2_GPIO_Port, SEL2_Pin);           
         break;

      case 3:
         LL_GPIO_SetOutputPin( SEL0_GPIO_Port, SEL0_Pin);    
         LL_GPIO_SetOutputPin( SEL1_GPIO_Port, SEL1_Pin);    
         LL_GPIO_ResetOutputPin( SEL2_GPIO_Port, SEL2_Pin);           
         break;

      case 4:
         LL_GPIO_ResetOutputPin( SEL0_GPIO_Port, SEL0_Pin);    
         LL_GPIO_ResetOutputPin( SEL1_GPIO_Port, SEL1_Pin);    
         LL_GPIO_SetOutputPin( SEL2_GPIO_Port, SEL2_Pin);           
         break;

      case 5:
         LL_GPIO_SetOutputPin( SEL0_GPIO_Port, SEL0_Pin);    
         LL_GPIO_ResetOutputPin( SEL1_GPIO_Port, SEL1_Pin);    
         LL_GPIO_SetOutputPin( SEL2_GPIO_Port, SEL2_Pin);           
         break;

      
      default:
         break;
   }
   
}   

void set_hiz(uint8_t isel, uint8_t ienable)
{  
   uint8_t iregval=0; 
   selbattchannel(isel); 
   
   iregval=i2c_readreg( ICADDR_MP2664GG, MP2664GG_INPUTSOURCE_CONTROL_REG);    
   iregval &= ~(1<<7);
   iregval |= (ienable & (1<<7) );
   
   i2c_writereg( ICADDR_MP2664GG, MP2664GG_INPUTSOURCE_CONTROL_REG,iregval);
   LL_mDelay(2);
} 

void set_input_min_voltage(uint8_t isel, uint8_t ival)
{  
   uint8_t iregval=0; 
   selbattchannel(isel); 
   
   iregval=i2c_readreg( ICADDR_MP2664GG, MP2664GG_INPUTSOURCE_CONTROL_REG);    
   iregval &= ~(1<<6|1<<5|1<<4|1<<3);
   iregval |= (ival & (1<<6|1<<5|1<<4|1<<3) );
   
   i2c_writereg( ICADDR_MP2664GG, MP2664GG_INPUTSOURCE_CONTROL_REG,iregval);
   LL_mDelay(2);
} 

void set_input_current_limit(uint8_t isel, uint8_t ival)
{  
   uint8_t iregval=0; 
   selbattchannel(isel); 
   
   iregval=i2c_readreg( ICADDR_MP2664GG, MP2664GG_INPUTSOURCE_CONTROL_REG);    
   iregval &= ~(1<<2|1<<1|1<<0);
   iregval |= (ival & (1<<2|1<<1|1<<0) );
   
   i2c_writereg( ICADDR_MP2664GG, MP2664GG_INPUTSOURCE_CONTROL_REG,iregval);
   LL_mDelay(2);
} 


void set_register_reset(uint8_t isel, uint8_t ival)
{  
   uint8_t iregval=0; 
   selbattchannel(isel); 
   
   iregval=i2c_readreg( ICADDR_MP2664GG, MP2664GG_POWERON_CONFIG_REG);    
   iregval &= ~(1<<7);
   iregval |= (ival & (1<<7) );
   
   i2c_writereg( ICADDR_MP2664GG, MP2664GG_POWERON_CONFIG_REG,iregval);
   LL_mDelay(2);
} 


void opencharge(uint8_t isel)
{  
   uint8_t iregval=0; 
   selbattchannel(isel); 
   
   iregval=i2c_readreg( ICADDR_MP2664GG, MP2664GG_POWERON_CONFIG_REG);    
   iregval &= ~(1<<3);  
   
   i2c_writereg( ICADDR_MP2664GG, MP2664GG_POWERON_CONFIG_REG,iregval);
   LL_mDelay(2);     
      
} 

void closecharge(uint8_t isel)
{  
   uint8_t iregval=0; 
   selbattchannel(isel); 
   
   iregval=i2c_readreg( ICADDR_MP2664GG, MP2664GG_POWERON_CONFIG_REG);    
   iregval |= (1<<3);
   
   i2c_writereg( ICADDR_MP2664GG, MP2664GG_POWERON_CONFIG_REG,iregval);
   LL_mDelay(2);     
      
}  

void set_batt_UVLO(uint8_t isel, uint8_t ilevel)
{     
   uint8_t iregval=0; 
   selbattchannel(isel); 
   
   iregval = i2c_readreg( ICADDR_MP2664GG, MP2664GG_POWERON_CONFIG_REG);    
   iregval &= ~(0x07);
   iregval |= (ilevel & 0x07);
   
   i2c_writereg( ICADDR_MP2664GG, MP2664GG_POWERON_CONFIG_REG, iregval);
   LL_mDelay(2);  
}  

void set_fastcharge_current(uint8_t isel, uint8_t ilevel)
{     
   uint8_t iregval=0; 
   selbattchannel(isel); 
   
   iregval = i2c_readreg( ICADDR_MP2664GG, MP2664GG_CHARGECURRENT_CONTROL_REG);    
   iregval &= ~(0x1f);
   iregval |= (ilevel & 0x1f);
   
   i2c_writereg( ICADDR_MP2664GG, MP2664GG_CHARGECURRENT_CONTROL_REG, iregval);
   LL_mDelay(2);  
}  

void set_precharge_current(uint8_t isel, uint8_t ilevel)
{     
   uint8_t iregval=0; 
   selbattchannel(isel); 
   
   iregval = i2c_readreg( ICADDR_MP2664GG, MP2664GG_PRECHARGECURRENT_REG);    
   iregval &= ~(0x03);
   iregval |= (ilevel & 0x03);
   
   i2c_writereg(ICADDR_MP2664GG, MP2664GG_PRECHARGECURRENT_REG, iregval);
   LL_mDelay(2);
}  

void set_batt_regulation_voltage(uint8_t isel, uint8_t ilevel)
{     
   uint8_t iregval=0;
   selbattchannel(isel); 
   
   iregval = i2c_readreg(ICADDR_MP2664GG, MP2664GG_CHARGEVOLTAGE_CONTROL_REG);    
   iregval &= ~(0xfc);
   iregval |= (ilevel & 0xfc);
  
   i2c_writereg( ICADDR_MP2664GG, MP2664GG_CHARGEVOLTAGE_CONTROL_REG, iregval);
   LL_mDelay(2);
}  

void set_fastcharge_threshold(uint8_t isel, uint8_t ilevel)
{     
   uint8_t iregval=0;
   selbattchannel(isel); 
   
   iregval = i2c_readreg( ICADDR_MP2664GG, MP2664GG_CHARGEVOLTAGE_CONTROL_REG);    
   iregval &= ~(0x02);
   iregval |= (ilevel & 0x02);
  
   i2c_writereg( ICADDR_MP2664GG, MP2664GG_CHARGEVOLTAGE_CONTROL_REG, iregval);
   LL_mDelay(2);
}  

void set_recharge_threshold(uint8_t isel, uint8_t ilevel)
{     
   uint8_t iregval=0;
   selbattchannel(isel); 
   
   iregval = i2c_readreg( ICADDR_MP2664GG, MP2664GG_CHARGEVOLTAGE_CONTROL_REG);    
   iregval &= ~(0x01);
   iregval |= (ilevel & 0x01);
  
   i2c_writereg( ICADDR_MP2664GG, MP2664GG_CHARGEVOLTAGE_CONTROL_REG, iregval);
   LL_mDelay(2);
}  

void set_terminate(uint8_t isel, uint8_t ienable)
{     
   uint8_t iregval=0;
   selbattchannel(isel); 
   
   iregval = i2c_readreg( ICADDR_MP2664GG, MP2664GG_CHARGETIME_CONTROL_REG);    
   iregval &= ~(1<<6);
   iregval |= (ienable & (1<<6));
  
   i2c_writereg( ICADDR_MP2664GG, MP2664GG_CHARGETIME_CONTROL_REG, iregval);
   LL_mDelay(2);
}  

void set_safety_timer(uint8_t isel, uint8_t ienable)
{     
   uint8_t iregval=0;
   selbattchannel(isel); 
   
   iregval = i2c_readreg( ICADDR_MP2664GG, MP2664GG_CHARGETIME_CONTROL_REG);    
   iregval &= ~(1<<3);
   iregval |= (ienable & (1<<3));
  
   i2c_writereg( ICADDR_MP2664GG, MP2664GG_CHARGETIME_CONTROL_REG, iregval);
   LL_mDelay(2);
}  

void set_fastcharge_timer(uint8_t isel, uint8_t ilevel)
{     
   uint8_t iregval=0;
   selbattchannel(isel); 
   
   iregval = i2c_readreg( ICADDR_MP2664GG, MP2664GG_CHARGETIME_CONTROL_REG);    
   iregval &= ~(1<<2|1<<1);
   iregval |= (ilevel & (1<<2|1<<1));
  
   i2c_writereg( ICADDR_MP2664GG, MP2664GG_CHARGETIME_CONTROL_REG, iregval);
   LL_mDelay(2);
}  

void set_terminate_timer(uint8_t isel, uint8_t ienable)
{     
   uint8_t iregval=0;
   selbattchannel(isel); 
   
   iregval = i2c_readreg( ICADDR_MP2664GG, MP2664GG_CHARGETIME_CONTROL_REG);    
   iregval &= ~(1<<0);
   iregval |= (ienable & (1<<0));
  
   i2c_writereg( ICADDR_MP2664GG, MP2664GG_CHARGETIME_CONTROL_REG, iregval);
   LL_mDelay(2);
}  

void set_ntc(uint8_t isel, uint8_t ienable)
{     
   uint8_t iregval=0;
   selbattchannel(isel); 
   
   iregval = i2c_readreg( ICADDR_MP2664GG, MP2664GG_MISCOPERATION_CONTROL_REG);    
   iregval &= ~(1<<3);
   iregval |= (ienable & (1<<3));
  
   i2c_writereg( ICADDR_MP2664GG, MP2664GG_MISCOPERATION_CONTROL_REG, iregval);
   LL_mDelay(2);
}  

void set_thermal_regulation_threshold(uint8_t isel, uint8_t ilevel)
{     
   uint8_t iregval=0;
   selbattchannel(isel); 
   
   iregval = i2c_readreg( ICADDR_MP2664GG, MP2664GG_MISCOPERATION_CONTROL_REG);    
   iregval &= ~(1<<0|1<<1);
   iregval |= (ilevel & (1<<0|1<<1));
  
   i2c_writereg( ICADDR_MP2664GG, MP2664GG_MISCOPERATION_CONTROL_REG, iregval);
   LL_mDelay(2);
}  




uint8_t get_system_status(uint8_t isel)
{  
   uint8_t iregval=0; 
   selbattchannel(isel); 
   
   iregval=i2c_readreg( ICADDR_MP2664GG, MP2664GG_SYSTEMSTATUS_REG);    
   
   return iregval;   
} 

uint8_t get_fault(uint8_t isel)
{  
   uint8_t iregval=0; 
   selbattchannel(isel); 
   
   iregval=i2c_readreg( ICADDR_MP2664GG, MP2664GG_FAULT_REG);    
   
   return iregval;   
} 





