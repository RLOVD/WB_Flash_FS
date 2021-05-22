#include "Flash_FS.h"

Servise_info_Tpf clr_file; // ��������� ��� �������� �����
static uint32_t head_adr=START_PAGE_FLASH_ADR;  // ���������� ������ ��������� �� ������� ��������

void Erase_Page(unsigned int pageAddress);
void Flash_Write(uint32_t *data, uint32_t address, uint16_t count);
uint32_t Find_Clear_Page();

void Flash_Init(void)
{
	/*������� �������������. ���������� ������� �� ������������� ��*/

}


uint8_t File_Write(uint16_t id_file, uint32_t *w_file, uint16_t size) //size 1024-4
{
	assert(size<SIZE_PAGE-4); //�������� ���������� ������� �����
	Servise_info_Tpf service_info;
	
  uint32_t clr_page_adr;
	/*��������� ����� ������ �������� � ������ �� �������� ���������� 0*/
  clr_page_adr=Find_Clear_Page();
	if (clr_page_adr==0) return 0;
	
	/*��������� ����� ������� �����*/
	uint16_t ver_f=0;
	Find_File(id_file,&ver_f); 
	/*����������� �������� ������*/
	ver_f++;
	
	service_info.ver_file=ver_f;
	service_info.id=id_file;
	
/* ������ �����*/
	Flash_Write(w_file, clr_page_adr, size);
	
	
	/* ������ �������������� � ������ ����� � ����� �������� */
	uint32_t end_adr=clr_page_adr + SIZE_PAGE-4 ;
	Flash_Write((uint32_t*)&service_info,end_adr, size);
	
	/*����������� �������� ������� �������� ��� ���������� �����  */
	if (head_adr<END_PAGE_FLASH_ADR) head_adr+=SIZE_PAGE;
	return 1;
}

void File_Read(uint16_t id_file, uint32_t *r_file, uint16_t size)
{
 

}

void File_Delete(uint16_t id_file)
{
	/*�������� ����� �� �����������, ���� �������� ������ ������ 0 ������ ���� ������������ - ������� */
	uint16_t ver_file=0;
 if (Find_File(id_file,&ver_file))
 {
	 /*��������� ������ � ������� �� ��������*/
	 clr_file.id=id_file;
	 clr_file.ver_file=ver_file;
	 
 }
  
}

/*������� ������ ��������� ��� ������ �������� , ���� ������� ����������� ������� 0*/
uint32_t Find_Clear_Page()
{
	/*�������� �� ��������� �� ������� ����� �� ����� �������� ������� */
	if (head_adr>=END_PAGE_FLASH_ADR) head_adr=START_PAGE_FLASH_ADR;

	uint32_t read_word=0;
		/*��������� ��������� 4 ����� �� ������ �������� ��������������� */
  for (uint32_t adr=head_adr;adr<END_PAGE_FLASH_ADR; adr+= SIZE_PAGE)
	{ 
		read_word=*(uint32_t*)(adr+SIZE_PAGE-4);
		
		if (read_word==0xFFFFFFFF)
		{
				 return adr; //���������� �������� ���������� ������
		}
	}

return 0;
}


/* ������ ������ �����, ���� �� ������������ ���������� ����� �������� � ������������ ������������ ���������� ������ */
uint32_t Find_File(uint16_t id_file, uint16_t *ver_file)
{
  uint16_t read_ver=0;
	uint16_t read_id=0;
	uint32_t read_word=0;
	
 /*��������� ��������� 4 ����� �� ������ �������� � ���� id */
  for (uint32_t adr=START_PAGE_FLASH_ADR;adr<END_PAGE_FLASH_ADR; adr+= SIZE_PAGE)
	{ 
		read_word=*(uint32_t*)(adr+SIZE_PAGE-4);
		
		if (read_word!=0xFF)
		{
		  read_id	= (uint16_t)read_word;	
		  if (read_id==id_file)
	    	{
			   *ver_file=(uint16_t)((read_word>16)&0xFF);
				 return adr; //���������� �������� ������
		    }
		}

	}
	
/*���� �� ������, ������� 0*/
return  0; 
	
}

/*������� ������ � �������� ������ ������ ����� */
void Clear_file_process(Servise_info_Tpf *clr_file)
{
  if (clr_file->id==0 && clr_file->ver_file==0) return; //���� ��� ����� �� �������� �������
	else
	{
		uint32_t page_adr;
		
    page_adr=Find_File(clr_file->id,0);
		/*�������� �������� � ������ � ������ ���� �� ������  */
		if (page_adr!=0)
		{
	  	Erase_Page(page_adr);
		}
	}
}

/*������� �������� �������� flash*/
void Erase_Page(unsigned int pageAddress) 
	{
		/*�� ������ ������ �� ����������� , ���������� � ����������� �� ��
		���� ������������ ��������� ���
	while (FLASH->SR & FLASH_SR_BSY);
	if (FLASH->SR & FLASH_SR_EOP) {
		FLASH->SR = FLASH_SR_EOP;
	}

	FLASH->CR |= FLASH_CR_PER;
	FLASH->AR = pageAddress;
	FLASH->CR |= FLASH_CR_STRT;
	while (!(FLASH->SR & FLASH_SR_EOP));
	FLASH->SR = FLASH_SR_EOP;
	FLASH->CR &= ~FLASH_CR_PER;*/
}
	


/*������� ������  flash*/

//data - ��������� �� ������������ ������
//address - ����� �� flash
//count - ���������� ������������ ����, ������ ���� ������ 2

void Flash_Write(uint32_t *data, uint32_t address, uint16_t count) 
	{
		/*�� ������ ������ �� ����������� , ���������� � ����������� �� ��
		���� ������������ ��������� ���
	unsigned int i;

	while (FLASH->SR & FLASH_SR_BSY);
	if (FLASH->SR & FLASH_SR_EOP) {
		FLASH->SR = FLASH_SR_EOP;
	}

	FLASH->CR |= FLASH_CR_PG;

	for (i = 0; i < count; i += 2) {
		*(volatile unsigned short*)(address + i) = (((unsigned short)data[i + 1]) << 8) + data[i];
		while (!(FLASH->SR & FLASH_SR_EOP));
		FLASH->SR = FLASH_SR_EOP;
	}

	FLASH->CR &= ~(FLASH_CR_PG);
		*/
}