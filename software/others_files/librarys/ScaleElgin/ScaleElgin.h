/*
    Autor: Matheus Gois Vieira
    Março de 2020
    Obs: Utilize um conversor RS232-TTL, conecte:

*/
#ifndef SCALEELGIN_H_INCLUDED  
#define SCALEELGIN_H_INCLUDED

#include <Arduino.h>
 
class ScaleElgin
{
    public:
        ScaleElgin();
        String readScale();
    
    private:
        String weight;
};

#endif