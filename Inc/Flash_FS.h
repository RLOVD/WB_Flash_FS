#ifndef FLASH_FS_H
#define FLASH_FS_H

#include "stm32f0xx.h"
#include "assert.h"


/*Размер страницы в б*/
#define START_PAGE_FLASH_ADR   0x08000000
#define END_PAGE_FLASH_ADR     0x08007000


#define SIZE_PAGE  1024

#define NUM_PAGE    64





typedef struct 
	{
 	 uint16_t ver_file;
	 uint16_t id;
  }Servise_info_Tpf;
	


void Flash_Init(void);
uint32_t Find_File(uint16_t id_file, uint16_t *ver_file);
	
void File_Write(uint16_t id_file, uint32_t *w_file, uint16_t size); 
void File_Read(uint16_t id_file, uint32_t *r_file, uint16_t size);
#endif // FLASH_FS_H
