/*
    Autor: Matheus Gois Vieira
    Março de 2020

*/
#include "MemoryFlash.h"
#include "Arduino.h"
#include "EEPROM.h"

MemoryFlash::MemoryFlash(int a){
    x = a;
    EEPROM.begin(x); 
}

void MemoryFlash::writeString(char add, String content)
{
  int _size = content.length();
  int i;
  for (i = 0; i < _size; i++)
  {
    EEPROM.write(add + i, content[i]);
  }
  EEPROM.write(add + _size, '\0'); //Adicionar caractere nulo de terminação para dados da string
  EEPROM.commit();
}


String MemoryFlash::read_String(char add)
{
  int i;
  char content[100]; //Max 100 Bytes
  int len = 0;
  unsigned char k;
  k = EEPROM.read(add);
  while (k != '\0' && len < 500) //Ler até caractere nulo
  {
    k = EEPROM.read(add + len);
    content[len] = k;
    len++;
  }
  content[len] = '\0';
  return String(content);
}
