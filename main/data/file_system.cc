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

void writeEEPROM(int deviceaddress, unsigned int eeaddress, byte data);
byte readEEPROM(int deviceaddress, unsigned int eeaddress );

/*
void setup() {
  
  GPIOPinTypeGPIOOutput(LED1Port, LED1);
  GPIOPinTypeGPIOOutput(LED2Port, LED2);
  GPIOPinTypeGPIOOutput(LED3Port, LED3);
  GPIOPinTypeGPIOOutput(LED4Port, LED4);
  
  Serial.begin(9600);
  Wire.begin();  
  Wire.setModule(0);
  
  clearFileSystem();
  initFileSystem();
  const String uploadHTML = "<html><head><title>Upload - Tiva</title></head><body><form action=\"upload\" method=\"post\" enctype=\"multipart/form-data\">Select image to upload:<input type=\"file\" name=\"fileToUpload\" id=\"fileToUpload\"><input type=\"submit\" value=\"Upload Image\" name=\"submit\"></form></body></html>";
  write_File("index.html",uploadHTML,uploadHTML.length());
  write_File("lol.txt","asdf",4);
}

void loop() {
  Serial.println(read_File("index.html"));
  Serial.println(read_File("lol.txt"));
  delay(2000);
}
*/

//init filesystem
void initFileSystem()
{
    Serial.println("helo");
    int file = 0;
    for(int addr = 0; addr < 32000; addr++)
    {
      char instant = readEEPROM(EEPROMADDR, addr);
      if(instant == (char)0) break;
   
      String nametemp;
      for(int i = addr; i < addr + 40; i++)
      {
        nametemp = nametemp + String(readEEPROM(EEPROMADDR, addr));
        addr++;
      }
      
      char lentemp[4];
      for(int i = addr; i < addr + 4; i++)
      {  
        readEEPROM(i,1);
        addr++;
      }
      int len;
      memcpy(&len, lentemp, sizeof(lentemp));

      FILE_SYSTEM[totalFiles].name = nametemp;
      FILE_SYSTEM[totalFiles].len = len;
      FILE_SYSTEM[totalFiles].addr = addr;
      FILE_SYSTEM[totalFiles].end_addr = addr + len;

      totalFiles++;
    }
}

void clearFileSystem()
{
  GPIOPinWrite(LED3Port, LED3, LED3);
  for(int i = 0; i < 32000; i++)
  {
    if((char)readEEPROM(EEPROMADDR, i) == (char)0) break;
    writeEEPROM(EEPROMADDR, i, (char)0);  
  }
  totalFiles = 0;
  GPIOPinWrite(LED3Port, LED3, LOW);
}

//returns -1 if file has the same name
int write_File(String filename, String contents, int contentLength)
{
  GPIOPinWrite(LED1Port, LED1, LED1);
  
  for(int i = 0; i < totalFiles; i++)
  {
    if(FILE_SYSTEM[i].name == filename)
    {
      GPIOPinWrite(LED1Port, LED1, LOW);
      return -1;
    }
  }
  
  //if no other files search for next null byte
  int addr;
  if(totalFiles == 0)
  {
    for(addr = 0; addr < 32000; addr++)
      if((char)readEEPROM(EEPROMADDR, addr) == (char)0) break;
  }
  else addr = FILE_SYSTEM[totalFiles - 1].end_addr + 1;

  //write the filename in the first 40 bytes
  for(int i = addr; i < 40 + addr; i++)
  {
    if((i-addr) < filename.length())
      writeEEPROM(EEPROMADDR,i,(byte)filename.charAt(i-addr));
    else
      writeEEPROM(EEPROMADDR,i,(byte)' ');
  }
  addr += 40;

  //write the length of the content to 4 bytes
  char bytes[4];
  bytes[0] = (contentLength >> 24) & 0xFF;
  bytes[1] = (contentLength >> 16) & 0xFF;
  bytes[2] = (contentLength >> 8) & 0xFF;
  bytes[3] = contentLength & 0xFF;
  for(int i = addr; i < 4 + addr; i++)
    writeEEPROM(EEPROMADDR,i,(byte)bytes[i-addr]);
  addr += 4;
  
  //write the contents of the file
  for(int i = addr; i < contentLength + addr; i++)
  {
    writeEEPROM(EEPROMADDR,i,(byte)contents.charAt(i-addr));
  }
  addr += contentLength;

  FILE_SYSTEM[totalFiles].name = filename;
  FILE_SYSTEM[totalFiles].len = contentLength;
  FILE_SYSTEM[totalFiles].addr = addr - contentLength;
  FILE_SYSTEM[totalFiles].end_addr = addr;
    
  totalFiles++;  
  GPIOPinWrite(LED1Port, LED1, LOW);
  return 0;
}

//returns file contents
String read_File(String filename)
{
  GPIOPinWrite(LED2Port, LED2, LED2);
  for(int i = 0; i < totalFiles; i++)
  {
    if(FILE_SYSTEM[i].name == filename)
    {
      String output;
      for(int j = FILE_SYSTEM[i].addr; j < FILE_SYSTEM[i].addr + FILE_SYSTEM[i].len; j++)
      {
        output = output + String((char)readEEPROM(EEPROMADDR, j));
      }
      GPIOPinWrite(LED2Port, LED2, LOW);
      return output;
    }
  }
  GPIOPinWrite(LED2Port, LED2, LOW);
}

void writeEEPROM(int deviceaddress, unsigned int eeaddress, byte data ) 
{
  Wire.beginTransmission(deviceaddress);
  Wire.write((int)(eeaddress >> 8));   // MSB
  Wire.write((int)(eeaddress & 0xFF)); // LSB
  Wire.write(data);
  Wire.endTransmission();
 
  delay(5);
}
 
byte readEEPROM(int deviceaddress, unsigned int eeaddress ) 
{
  byte rdata = 0xFF;
 
  Wire.beginTransmission(deviceaddress);
  Wire.write((int)(eeaddress >> 8));   // MSB
  Wire.write((int)(eeaddress & 0xFF)); // LSB
  Wire.endTransmission();
 
  Wire.requestFrom(deviceaddress,1);
 
  if (Wire.available()) rdata = Wire.read();
 
  return rdata;
}
