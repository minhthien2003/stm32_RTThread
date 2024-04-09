/**
 * Author Nguyen Minh Thien 2023
 * Logic Process Flash operation
*/
#ifndef __APPDATAFLASHOPERATION_H__
#define __APPDATAFLASHOPERATION_H__
#endif

#define ADD_PAGE_63_SECTOR_15_START ((uint32_t)0x0800FC00)
#define ADD_PAGE_63_SECTOR_15_END   ((uint32_t)0x0800FFFF)
#define SIZE_PAGE 1024


void WriteBackupData(uint32_t address, uint32_t *u32Data);
void ReadBackupData(uint32_t address, uint32_t *u32Data);
