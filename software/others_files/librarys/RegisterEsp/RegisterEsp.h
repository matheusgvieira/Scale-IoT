#ifndef   REGISTERESP_H
#define   REGISTERESP_H

#include <Arduino.h>

class RegisterEsp
{
private:
    String _url;
public:
    RegisterEsp(char mac[], String url);
    void begin();    
    int dev();
    int emp();
    String dev_mac();
    String dev_desc();
    String dev_cfg();
    String dev_hrultima();
    String nowDate();
    String nowTimeJson();
    int nowTimeSegundos();
    long int timestamp();
    String token();
};

#endif;

