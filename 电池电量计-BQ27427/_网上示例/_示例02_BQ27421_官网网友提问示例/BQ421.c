#include "bq27421.h"
#include "fuel_guage.h"
#include "i2c.h"
static uint8_t txdata[16];
static uint8_t rxdata[16];
#define I2C_MASTER	    MXC_I2C0_BUS0

#define I2C_SLAVE	    MXC_I2C1_BUS0

#define I2C_SLAVE_ADDR	(0xAB)

#define  HAL_Delay(X)   local_task_sleep_ms(X)

#define HAL_StatusTypeDef int
int error = 0;
bool bq27421_i2c_command_write( uint8_t command, uint16_t data )
{
    HAL_StatusTypeDef ret;
    uint8_t i2c_data[3];

    i2c_data[0] = command;
    i2c_data[1] = ( uint8_t )( data & 0x00FF );
    i2c_data[2] = ( uint8_t )( ( data >> 8 ) & 0x00FF );

 	if((error = I2C_MasterWrite(I2C_MASTER, I2C_SLAVE_ADDR, i2c_data, 3, 0)) != 3) {
    		printf( "%s:%d Error writing: %d\n", __func__, __LINE__, error);
    }

    HAL_Delay( BQ27421_DELAY );

    return true;

}

bool bq27421_i2c_command_read( uint8_t command, uint16_t *data )
{
    HAL_StatusTypeDef ret;
    uint8_t i2c_data[2];

 	if((error = I2C_MasterWrite(I2C_MASTER, I2C_SLAVE_ADDR, command, 1, 0)) != 1) {
    		printf( "%s:%d Error writing: %d\n", __func__, __LINE__, error);
    }
    HAL_Delay( BQ27421_DELAY );

	if((error = I2C_MasterRead(I2C_MASTER, I2C_SLAVE_ADDR, i2c_data, 2, 0)) != 2) {
		printf("%s:%d Error writing: %d\n", __func__, __LINE__, error);
	}

    HAL_Delay( BQ27421_DELAY );
    //printf("%s:%d readVal: 0x%x%x \n", __func__, __LINE__,i2c_data[1], i2c_data[0]);
    *data = ( i2c_data[1] << 8 ) | i2c_data[0];

    return true;
}

bool bq27421_i2c_control_write( uint16_t subcommand )
{
    HAL_StatusTypeDef ret;
    uint8_t i2c_data[2];

    i2c_data[0] = BQ27421_CONTROL_LOW;
    i2c_data[1] = (uint8_t)( ( subcommand ) & 0x00FF );

 	if((error = I2C_MasterWrite(I2C_MASTER, I2C_SLAVE_ADDR, i2c_data, 2, 0)) != 2) {
    		printf( "%s:%d Error writing: %d\n", __func__, __LINE__, error);
    }
    //HAL_Delay( BQ27421_DELAY );

    i2c_data[0] = BQ27421_CONTROL_HIGH;
    i2c_data[1] = (uint8_t)( ( subcommand >> 8 ) & 0x00FF );

 	if((error = I2C_MasterWrite(I2C_MASTER, I2C_SLAVE_ADDR, i2c_data, 2, 0)) != 2) {
    		printf( "%s:%d Error writing: %d\n", __func__, __LINE__, error);
    }
    //HAL_Delay( BQ27421_DELAY );

    return true;
}

bool bq27421_i2c_control_read( uint16_t subcommand, uint16_t *data )
{
    HAL_StatusTypeDef ret;
    uint8_t i2c_data[2];

    i2c_data[0] = BQ27421_CONTROL_LOW;
    i2c_data[1] = (uint8_t)( ( subcommand ) & 0x00FF );

 	if((error = I2C_MasterWrite(I2C_MASTER, I2C_SLAVE_ADDR, i2c_data, 2, 0)) != 2) {
    		printf( "%s:%d Error writing: %d\n", __func__, __LINE__, error);
    }

    HAL_Delay( BQ27421_DELAY );

    i2c_data[0] = BQ27421_CONTROL_HIGH;
    i2c_data[1] = (uint8_t)( ( subcommand >> 8 ) & 0x00FF );

 	if((error = I2C_MasterWrite(I2C_MASTER, I2C_SLAVE_ADDR, i2c_data, 2, 0)) != 2) {
    		printf( "%s:%d Error writing: %d\n", __func__, __LINE__, error);
    }

    HAL_Delay( BQ27421_DELAY );

	if((error = I2C_MasterRead(I2C_MASTER, I2C_SLAVE_ADDR, i2c_data, 2, 0)) != 2) {
		printf("%s:%d Error writing: %d\n", __func__, __LINE__, error);
	}


    HAL_Delay( BQ27421_DELAY );

    *data = ( i2c_data[1] << 8 ) | i2c_data[0];
    printf("%s:%d readVal: 0x%x%x \n", __func__, __LINE__,i2c_data[1], i2c_data[0]);
    return true;
}

bool bq27421_i2c_write_data_block( uint8_t offset, uint8_t *data, uint8_t bytes )
{
    HAL_StatusTypeDef ret;
    uint8_t i2c_data[2], i;

    for( i = 0; i < bytes; i++ )
    {
        i2c_data[0] = BQ27421_BLOCK_DATA_START + offset + i;
        i2c_data[1] = data[i];

     	if((error = I2C_MasterWrite(I2C_MASTER, I2C_SLAVE_ADDR, i2c_data, 2, 0)) != 2) {
        		printf( "%s:%d Error writing: %d\n", __func__, __LINE__, error);
        }
        HAL_Delay( BQ27421_DELAY );
    }

    return true;
}

bool bq27421_i2c_read_data_block( uint8_t offset, uint8_t *data, uint8_t bytes )
{
    HAL_StatusTypeDef ret;

    uint8_t i2c_data;

    i2c_data = BQ27421_BLOCK_DATA_START + offset;

 	if((error = I2C_MasterWrite(I2C_MASTER, I2C_SLAVE_ADDR, &i2c_data, 1, 0)) != 1) {
    		printf( "%s:%d Error writing: %d\n", __func__, __LINE__, error);
    }

    HAL_Delay( 5 );

	if((error = I2C_MasterRead(I2C_MASTER, I2C_SLAVE_ADDR, data, bytes, 0)) != bytes) {
		printf("%s:%d Error writing: %d\n", __func__, __LINE__, error);
	}

    HAL_Delay( BQ27421_DELAY );

    return true;
}


//in: 200mAh, 3000mV, 80mA
bool bq27421_init( uint16_t designCapacity_mAh, uint16_t terminateVoltage_mV, uint16_t taperCurrent_mA )
{

    uint16_t designEnergy_mWh, taperRate, flags, checksumOld, checksumRead;
    uint8_t checksumNew;
	 int ret = 0;
 	 uint8_t block[32];   
   

    designEnergy_mWh = 3.7 * designCapacity_mAh;  //easy
    taperRate = designCapacity_mAh / ( 0.1 * taperCurrent_mA );
    HAL_Delay( 1000 );
    // Unseal gauge
    bq27421_i2c_control_write( BQ27421_CONTROL_UNSEAL );  //reg01-00 = 0x8000
    bq27421_i2c_control_write( BQ27421_CONTROL_UNSEAL );  //reg01-00 = 0x8000
    HAL_Delay( 50 );
    // Send CFG_UPDATE
    bq27421_i2c_control_write( BQ27421_CONTROL_SET_CFGUPDATE );    //reg01-reg00 =0x0013  

    printf("%s:%d Checking  config flag\n", __func__, __LINE__);
    // Poll flags
    int counter = 0;
    do
    {	flags = 0;
        bq27421_i2c_command_read( BQ27421_FLAGS_LOW, &flags );    //flags= reg07-reg06
        if( !(flags & 0x0010) )
        {
            HAL_Delay( 50 );
        }
        counter ++;
        if(counter >= 50){
        	break;
        }
    }
    while( !(flags & 0x0010) );
    printf("%s:%d Flags: 0x%x \n", __func__, __LINE__, flags);
    // Enable Block Data Memory Control
    bq27421_i2c_command_write( BQ27421_BLOCK_DATA_CONTROL, 0x0000 );  //reg61-62=0000

    HAL_Delay( BQ27421_DELAY );
    printf("%s:%d \n", __func__, __LINE__);
    // Access State subclass
    bq27421_i2c_command_write( BQ27421_DATA_CLASS, 0x0052 );   //reg3f-3e =0052
    printf("%s:%d \n", __func__, __LINE__);
    // Write the block offset
    bq27421_i2c_command_write( BQ27421_DATA_BLOCK, 0x0000 );   //reg40-3f =0000  //or repeat to remove?

    // Read block checksum
    bq27421_i2c_command_read( BQ27421_BLOCK_DATA_CHECKSUM, &checksumOld );   //checksumOld = reg60-61
    printf("%s:%d CheckSumOld:0x%x\n", __func__, __LINE__, checksumOld);
    // Read 32-byte block of data

    for(uint8_t i = 0; i < 32; i++ )
    {
        block[i] = 0x00;
    }
    printf("%s:%d \n", __func__, __LINE__);
    bq27421_i2c_read_data_block( 0x00, block, 32 );

    // Calculate checksum
    uint8_t checksumCalc = 0x00;

    for(uint8_t i = 0; i < 32; i++ )
    {
        checksumCalc += block[i];
        printf("Block[%d]:%d %x\n", i, block[i], block[i]);
    }
    printf("\n");
    printf("%s:%d Calculated CheckSum: 0x%x \n", __func__, __LINE__, checksumCalc);
    checksumCalc = 0xFF - checksumCalc;
    printf("%s:%d Calculated CheckSum: 0x%x \n", __func__, __LINE__, checksumCalc);
    // Update design capacity
    block[10] = (uint8_t)( designCapacity_mAh >> 8 );
    block[11] = (uint8_t)( designCapacity_mAh & 0x00FF );
    // Update design energy
    block[12] = (uint8_t)( designEnergy_mWh >> 8 );
    block[13] = (uint8_t)( designEnergy_mWh & 0x00FF );
    // Update terminate voltage
//    block[16] = (uint8_t)( terminateVoltage_mV >> 8 );
//    block[17] = (uint8_t)( terminateVoltage_mV & 0x00FF );
    // Update taper rate
//    block[27] = (uint8_t)( taperRate >> 8 );
//    block[28] = (uint8_t)( taperRate & 0x00FF );

    // Calculate new checksum
    checksumNew = 0x00;
    for(int i = 0; i < 32; i++ )
    {
        checksumNew += block[i];
    }
    printf("%s:%d New CheckSum: 0x%x \n", __func__, __LINE__, checksumNew);
    checksumNew = 0xFF - checksumNew;
    printf("%s:%d New CheckSum: 0x%x \n", __func__, __LINE__, checksumNew);
    // Enable Block Data Memory Control
    bq27421_i2c_command_write( BQ27421_BLOCK_DATA_CONTROL, 0x0000 );

    HAL_Delay( BQ27421_DELAY );

     // Write 32-byte block of updated data
    bq27421_i2c_write_data_block( 0x00, block, 32 );

    bq27421_i2c_command_write( BQ27421_BLOCK_DATA_CHECKSUM, checksumNew );




    // Read block checksum
    bq27421_i2c_command_read( BQ27421_BLOCK_DATA_CHECKSUM, &checksumRead );

    printf("%s:%d checksumRead:0x%x \n", __func__, __LINE__, checksumRead);
    // Verify
    if( checksumRead != (uint8_t)checksumNew )
    {
    	printf("%s:%d CheckSum not matched: checksumRead:0x%x,  wrote:0x%x \n", __func__, __LINE__, checksumRead, checksumNew);
    }

    // Configure BAT_DET
    bq27421_i2c_control_write( BQ27421_CONTROL_BAT_INSERT );

    // Send Soft Reset
    bq27421_i2c_control_write( BQ27421_CONTROL_SOFT_RESET );
printf("FG Soft Reset \n");
    // Poll flags
    do
    {
        bq27421_i2c_command_read( BQ27421_FLAGS_LOW, &flags );
        if( !(flags & 0x0010) )
        {
            HAL_Delay( 50 );
        }
    }
    while( (flags & 0x0010) );
    printf("FG Seal Gauge: flag:%x \n", flags);
    // Seal gauge
    bq27421_i2c_control_write( BQ27421_CONTROL_SEALED );

    HAL_Delay( 1000 );
    bq27421_i2c_control_write( BQ27421_CONTROL_SOFT_RESET );
    HAL_Delay( 1000 );

    
	 bq27421_readDeviceFWver(&ret);
  	 printf("DEVICE FIRMWARE: %x \n", ret);
	 bq27421_readDeviceType(&ret);
	 printf("DEVICE TYPE: %x \n", ret);
	 checksumOld = 0;
    bq27421_i2c_command_read( BQ27421_BLOCK_DATA_CHECKSUM, &checksumOld );
    printf("%s:%d CheckSumOld:0x%x\n", __func__, __LINE__, checksumOld);

    for(uint8_t i = 0; i < 32; i++ )
    {
        block[i] = 0x00;
    }
    bq27421_i2c_read_data_block( 0x00, block, 32 );
    for(uint8_t i = 0; i < 32; i++ )
    {
        printf("Block[%d]:%d - %x\n", i, block[i], block[i]);
    }
    printf("\n");
   // printf("Design Capacity: %x , %d \n", ret, ret);

    return true;
}

bool bq27421_update( bq27421_info *battery )
{
    uint16_t temp;

    if( !bq27421_readVoltage_mV( &(battery->voltage_mV) ) )
    {
        return false;
    }
    if( !bq27421_readAvgCurrent_mA( &(battery->current_mA) ) )
    {
        return false;
    }
    if( !bq27421_readTemp_degK( &temp ) )
    {
        return false;
    }
    battery->temp_degC = ( (double)temp / 10 ) - 273.15;

    if( !bq27421_readStateofCharge_percent( &(battery->soc_percent) ) )
    {
        return false;
    }
    if( !bq27421_readStateofHealth_percent( &(battery->soh_percent) ) )
    {
        return false;
    }
    if( !bq27421_readDesignCapacity_mAh( &(battery->designCapacity_mAh) ) )
    {
        return false;
    }
    if( !bq27421_readRemainingCapacity_mAh( &(battery->remainingCapacity_mAh) ) )
    {
        return false;
    }
    if( !bq27421_readFullChargeCapacity_mAh( &(battery->fullChargeCapacity_mAh) ) )
    {
        return false;
    }
    if( !bq27421_readFlagsReg( &temp ) )
    {
        return false;
    }
    battery->isCritical = temp & 0x0002;
    battery->isLow = temp & 0x0004;
    battery->isFull = temp & 0x0200;
    if( battery->current_mA <= 0 )
    {
        battery->isDischarging = 1;
        battery->isCharging = 0;
    }
    else
    {
        battery->isDischarging = 0;
        battery->isCharging = 1;
    }

    return true;
}

bool bq27421_readDeviceType( uint16_t *deviceType )
{
    if( !bq27421_i2c_control_write( BQ27421_CONTROL_DEVICE_TYPE ) )
    {
        return false;
    }
    if( !bq27421_i2c_command_read( BQ27421_CONTROL_LOW, deviceType ) )
    {
        return false;
    }

    return true;
}

bool bq27421_readDeviceFWver( uint16_t *deviceFWver )
{
    if( !bq27421_i2c_control_write( BQ27421_CONTROL_FW_VERSION ) )
    {
        return false;
    }
    if( !bq27421_i2c_command_read( BQ27421_CONTROL_LOW, deviceFWver ) )
    {
        return false;
    }

    return true;
}

bool bq27421_readDesignCapacity_mAh( uint16_t *capacity_mAh )
{
    if( !bq27421_i2c_command_read( BQ27421_DESIGN_CAP_LOW, capacity_mAh ) )
    {
        return false;
    }

    return true;
}

bool bq27421_readVoltage_mV( uint16_t *voltage_mV )
{
    if( !bq27421_i2c_command_read( BQ27421_VOLTAGE_LOW, voltage_mV ) )
    {
        return false;
    }

    return true;
}

bool bq27421_readTemp_degK( uint16_t *temp_degKbyTen )
{
    if( !bq27421_i2c_command_read( BQ27421_TEMP_LOW, temp_degKbyTen ) )
    {
        return false;
    }

    return true;
}

bool bq27421_readAvgCurrent_mA( int16_t *avgCurrent_mA )
{
    if( !bq27421_i2c_command_read( BQ27421_AVG_CURRENT_LOW, (uint16_t *)avgCurrent_mA ) )
    {
        return false;
    }

    return true;
}

bool bq27421_readStateofCharge_percent( uint16_t *soc_percent )
{
    if( !bq27421_i2c_command_read( BQ27421_STATE_OF_CHARGE_LOW, soc_percent ) )
    {
        return false;
    }

    return true;
}

bool bq27421_readControlReg( uint16_t *control )
{
    if( !bq27421_i2c_control_write( BQ27421_CONTROL_STATUS ) )
    {
        return false;
    }
    if( !bq27421_i2c_command_read( BQ27421_CONTROL_LOW, control ) )
    {
        return false;
    }

    return true;
}

bool bq27421_readFlagsReg( uint16_t *flags )
{
    if( !bq27421_i2c_command_read( BQ27421_FLAGS_LOW, flags ) )
    {
        return false;
    }

    return true;
}

bool bq27421_readopConfig( uint16_t *opConfig )
{
    if( !bq27421_i2c_command_read( BQ27421_OPCONFIG_LOW, opConfig ) )
    {
        return false;
    }

    return true;
}

bool bq27421_readRemainingCapacity_mAh( uint16_t *capacity_mAh )
{
    if( !bq27421_i2c_command_read( BQ27421_REMAINING_CAP_LOW, capacity_mAh ) )
    {
        return false;
    }

    return true;
}

bool bq27421_readFullChargeCapacity_mAh( uint16_t *capacity_mAh )
{
    if( !bq27421_i2c_command_read( BQ27421_FULL_CHARGE_CAP_LOW, capacity_mAh ) )
    {
        return false;
    }

    return true;
}

bool bq27421_readStateofHealth_percent( uint16_t *soh_percent )
{
    if( !bq27421_i2c_command_read( BQ27421_STATE_OF_HEALTH_LOW, soh_percent ) )
    {
        return false;
    }

    *soh_percent = *soh_percent & 0x00FF;

    return true;
}

static int16_t fg_currentmA(){

	txdata[0] = COMMAND_CURRENT_MA; //cmd
	if((error = I2C_MasterWrite(I2C_MASTER, I2C_SLAVE_ADDR, txdata, 1, 0)) != 1) {
		printf_log(LOG_ERROR, "%s:%d Error writing: %d\n", __func__, __LINE__, error);
	}

	memset(rxdata, 0, sizeof(rxdata));
	if((error = I2C_MasterRead(I2C_MASTER, I2C_SLAVE_ADDR, rxdata, 2, 0)) != 2) {
		printf_log(LOG_ERROR, "%s:%d Error writing: %d\n", __func__, __LINE__, error);
	}
	return ((rxdata[0] & 0xFF) | (rxdata[1] << 8));
}

static int16_t getVoltage(){
	txdata[0] = COMMAND_VOLTAGE; //cmd
	if((error = I2C_MasterWrite(I2C_MASTER, I2C_SLAVE_ADDR, txdata, 1, 0)) != 1) {
		printf_log(LOG_ERROR, "%s:%d Error writing: %d\n", __func__, __LINE__, error);
	}

	memset(rxdata, 0, sizeof(rxdata));
	if((error = I2C_MasterRead(I2C_MASTER, I2C_SLAVE_ADDR, rxdata, 2, 0)) != 2) {
		printf_log(LOG_ERROR, "%s:%d Error writing: %d\n", __func__, __LINE__, error);
	}
	return ((rxdata[0] & 0xFF) | (rxdata[1] << 8));
}

static uint16_t fg_remainingChargingCapacity()
{
	txdata[0] = COMMAND_REMAINING_CHARGING_CAPACITY; //cmd
	if((error = I2C_MasterWrite(I2C_MASTER, I2C_SLAVE_ADDR, txdata, 1, 0)) != 1) {
		printf_log(LOG_ERROR, "%s:%d Error writing: %d\n", __func__, __LINE__, error);
	}

	memset(rxdata, 0, sizeof(rxdata));
	if((error = I2C_MasterRead(I2C_MASTER, I2C_SLAVE_ADDR, rxdata, 2, 0)) != 2) {
		printf_log(LOG_ERROR, "%s:%d Error writing: %d\n", __func__, __LINE__, error);
	}
	//printf("REMAINING CAPCITY mA : %d %d \n", rxdata[0], rxdata[1]);
	return ((rxdata[0] & 0xFF) | (rxdata[1] << 8));
}
static uint16_t fg_fullChargeCapacity(){

	txdata[0] = COMMAND_FULL_CHARGE_CAPACITY; //cmd
	if((error = I2C_MasterWrite(I2C_MASTER, I2C_SLAVE_ADDR, txdata, 1, 0)) != 1) {
		printf_log(LOG_ERROR, "%s:%d Error writing: %d\n", __func__, __LINE__, error);
	}

	memset(rxdata, 0, sizeof(rxdata));
	if((error = I2C_MasterRead(I2C_MASTER, I2C_SLAVE_ADDR, rxdata, 2, 0)) != 2) {
		printf_log(LOG_ERROR, "%s:%d Error writing: %d\n", __func__, __LINE__, error);
	}
	printf_log(LOG_DEBUG, "FULL CHARGE CAPCITY mA: %d %d \n", rxdata[0], rxdata[1]);
	return ((rxdata[0] & 0xFF) | (rxdata[1] << 8));
}

static uint16_t fg_soc(){

	txdata[0] = 0x1C; //cmd
	if((error = I2C_MasterWrite(I2C_MASTER, I2C_SLAVE_ADDR, txdata, 1, 0)) != 1) {
		printf_log(LOG_ERROR, "%s:%d Error writing: %d\n", __func__, __LINE__, error);
	}

	memset(rxdata, 0, sizeof(rxdata));
	if((error = I2C_MasterRead(I2C_MASTER, I2C_SLAVE_ADDR, rxdata, 2, 0)) != 2) {
		printf_log(LOG_ERROR, "%s:%d Error writing: %d\n", __func__, __LINE__, error);
	}
	printf_log(LOG_DEBUG, "FULL CHARGE CAPCITY mA: %d %d \n", rxdata[0], rxdata[1]);
	return ((rxdata[0] & 0xFF) | (rxdata[1] << 8));
}

void fg_print_data(){
	printf("\nCapacity->FULL:%d\tRemaining:%d\n",fg_fullChargeCapacity(), fg_remainingChargingCapacity());
	printf("Voltage:%d\tCurrent:%d\tSOC:%d\n", getVoltage(), fg_currentmA(), fg_soc());
	printf("Percentage:%d \n", getBatteryPercentage());
}


void new_fg_init()
{
	printf("SLAVE Address : %x \n", BQ27421_I2C_ADDRESS);
	bq27421_init(200, 3000, 80);
	printf("fg_init done \n");
} 