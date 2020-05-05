#include <iostream>
#include <string>
using namespace std;
#include <bits/stdc++.h>



string czero(string json)
{
  int n = json.length() + 1;
  char aux[n];
  strcpy(aux, json.c_str());

  if (aux[1] == '0' && aux[0] == ' ')
  {
    for (int i = 1; i < n; i++)
    {
      aux[i] = aux[i + 1];
    }
  }
  return aux;
}
float kg(string p)
{

  int len = p.length() + 1;
  char peso[len];
  strcpy(peso, p.c_str());

  string s1;
  string s2;
  string s3;
  string s4;
  string s5;
  s1 += peso[0];
  s2 += peso[1];
  s3 += peso[2];
  s4 += peso[3];
  s5 += peso[4];

  string whole = s1 + s2;  
  string decimal = s3 + s4 + s5;
      

  float num1 = strtof(whole.c_str(),0);
  float num2 = strtof(decimal.c_str(),0);

  float num = num1 + num2/1000;

  return num;
}

char handlingError(string vString){
  int len = vString.length() + 1;
  char c[len];
  strcpy(c, vString.c_str());

  int i = 0;
  char a = 'a';

  while (a != 'k')
  {
    a = c[i];
    i++;
  }
  return c[i-1];

} 

int main()
{

  string weight = "10195";
  string num = "130kï 0130";
  cout << handlingError(num);

  return 0;
}
