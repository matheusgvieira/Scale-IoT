/*
    Autor: Matheus Gois Vieira
    Março de 2020

*/
#ifndef MEMORYFLASH_H_INCLUDED
#define MEMORYFLASH_H_INCLUDED

#include <Arduino.h>

class MemoryFlash
{
    public:
        MemoryFlash(int a);
        void writeString(char add, String content);
        String read_String(char add);
    private:
        int x;
};

#endif