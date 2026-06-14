#include<iostream>
using namespace std;
int main()
{
  int a,b;
  cout<<"Lütfen 2 adet sayının değerini girin: \n";
  cout<<"a: ";
  cin>>a;
  cout<<"b: ";
  cin>>b;
  cout<<"Girdiğiniz sayıların toplamı: "<<a+b<<endl;
  cout<<"Girdiğiniz sayıların farkı: "<<a-b<<endl;
  cout<<"Girdiğiniz sayıların çarpımı: "<<a*b<<endl;
  cout<<"Girdiğiniz sayıların bölümünün tam kısmı: "<<a/b<<endl;
  if(b!=0){
    cout<<"Girdiğiniz sayıların bölümü: "<<(float)a/b<<endl;
}
    cout<<"Girdiğiniz sayıların mod alma işlemi: "<<a%b<<endl;
    cout<<"Girdiğiniz sayıların ondalık bölümü: "<<(double)a/b<<endl;
    (a>b) ? cout<<"a sayısı b sayısından büyüktür"<<endl : cout<<"b sayısı a sayısından büyüktür"<<endl;
    (a>0 && b>0) ? cout<<"Girdiğiniz sayıların ikisi de pozitiftir"<<endl : cout<<"Girdiğiniz sayıların en az biri negatiftir"<<endl;
    return 0;
}