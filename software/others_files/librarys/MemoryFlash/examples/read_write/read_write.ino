#include <MemoryFlash.h>

String content;
boolean a = HIGH;

MemoryFlash mflash(1);

void setup(){
     Serial.begin(9600);
     while(a)  {
        if(Serial.available()>0){
          content = Serial.readString();
          a = !a;
        }
    }
    Serial.println();
    Serial.print("Writing Data:");
    Serial.println(content);
 
   mflash.writeString(10, content);  //Address 10 and String type data
    delay(10);
}
void loop() {
  // put your main code here, to run repeatedly:
  String recivedData;
  recivedData = mflash.read_String(10);
  Serial.print("Read Data:");
  Serial.println(recivedData);
  delay(10000);
}