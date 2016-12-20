#ifndef file_system_h
#define file_system_h

#include <Wire.h>
#include "OrbitBoosterPackDefs.h"
#include <stdlib.h>


int totalFiles = 0;

typedef struct {
    String name;
    int len;
    int addr;
    int end_addr;
} file;

file FILE_SYSTEM[128];

/*
void initFileSystem();
void clearFileSystem();
int writeEEPROM(int deviceaddress, unsigned int eeaddress, byte data);
byte readEEPROM(int deviceaddress, unsigned int eeaddress );
*/
#endif 
