
#include "i2c.h"
#include "eeprom24xx.h"

void EEPROM24XX_Save(uint16_t Address,void *data,size_t size_of_data)
{

	uint8_t pages = size_of_data >> 3;
	uint8_t rem = size_of_data & 0x07;
	uint8_t i = 0;
	while (pages--){
		HAL_I2C_Mem_Write(&hi2c1,0xa0,Address + (i<<3),I2C_MEMADD_SIZE_8BIT,(uint8_t*)(data + (i<<3)), 8, 1000);
		i++;
		HAL_Delay(7);
	}
	
	HAL_I2C_Mem_Write(&hi2c1,0xa0,Address + (i<<3),I2C_MEMADD_SIZE_8BIT,(uint8_t*)(data + (i<<3)), rem, 1000);
	HAL_Delay(7);

}
//##########################################################################
void EEPROM24XX_Load(uint16_t Address,void *data,size_t size_of_data)
{

	HAL_I2C_Mem_Read(&hi2c1,0xa0,Address,I2C_MEMADD_SIZE_8BIT,(uint8_t*)data,size_of_data,1000);

}
