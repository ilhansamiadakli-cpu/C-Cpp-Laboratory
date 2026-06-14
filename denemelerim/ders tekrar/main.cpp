#include <iostream>
#include <string>
#include<fstream>
using namespace std;
/*
TODO: dosya işlemleri için kullanılan sınıflar:
* fstream: hem okuma hem yazma işlemi yapar
* ifstream: sadece okuma işlemi yapar
* ofstream: sadece yazma işlemi yapar

TODO: ios modları:
* ios::in = dosya okuma
* ios::out = dosya yazma
* ios::app = dosya sonuna ekleme
* ios::ate = dosya açıldıktan sonra imleci sona konumlandırır
* ios::trunc = dosya açıldığında içeriği siler, varsayılan olarak ofstream ile birlikte gelir
TODO: önemli notlar
* dosya okumada file.eof() kullanımı hatalıdır, çünkü EOF karakterine gelindiğinde true döner, yani son satır iki kere okunur. Bunun yerine, okuma işlemi sırasında döngüyü kontrol etmek daha doğrudur.
* EOF bayrağı, dosyanın sonuna gelindiğinde true döner, ancak bu durum genellikle okuma işlemi sırasında kontrol edilmelidir, çünkü EOF karakterine gelindiğinde true döner ve son satır iki kere okunabilir.
* 
*/

int main() {
ofstream yaz; yaz.open("kisa.txt");
yaz << "Ali 85" << endl;
yaz << "Ayse 90" << endl;
yaz.close();
ifstream f; f.open("kisa.txt");
string isim; int not_;

int tur = 0;
while (!f.eof()) { // YANLIS!, EOF karakterine gelindiğinde true döner, yani son satır iki kere okunur
f >> isim >> not_;
cout << ++tur << ": " << isim << " " << not_ << endl;
}
f.close();
return 0;
}