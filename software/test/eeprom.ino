#include <EEPROM.h>

String content;
boolean a = HIGH;
void writeString(char add,String content);
String read_String(char add);
 
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  EEPROM.begin(512);
  while(a)  {
    if(Serial.available()>0){
      content = Serial.readString();
      a = !a;
    }
  }
  Serial.println();
  Serial.print("Writing Data:");
  Serial.println(content);
 
  writeString(10, content);  //Address 10 and String type data
  delay(10);
}
 
void loop() {
  // put your main code here, to run repeatedly:
  String recivedData;
  recivedData = read_String(10);
  Serial.print("Read Data:");
  Serial.println(recivedData);
  delay(10000);
}
 
void writeString(char add,String content)
{
  int _size = content.length();
  int i;
  for(i=0;i<_size;i++)
  {
    EEPROM.write(add+i,content[i]);
  }
  EEPROM.write(add+_size,'\0');   //Adicionar caractere nulo de terminação para dados da string
  EEPROM.commit();
}
 
 
String read_String(char add)
{
  int i;
  char content[100]; //Max 100 Bytes
  int len=0;
  unsigned char k;
  k=EEPROM.read(add);
  while(k != '\0' && len<500)   //Ler até caractere nulo
  {    
    k=EEPROM.read(add+len);
    content[len]=k;
    len++;
  }
  content[len]='\0';
  return String(content);
}
