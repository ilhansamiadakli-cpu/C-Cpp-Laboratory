#include <cstdint>
#include <iostream>
using namespace std;
/*
* reinterpret_cast<dönüş tipi*> kullanılıcak 
Todo: reinterpret_cast nedir, neden kullanılır, nasıl kullanılır?
* reinterpret_cast, bir türü başka bir türe dönüştürmek için kullanılan bir C++ operatörüdür. 
* Genellikle bellek adreslerini farklı türlerdeki işaretçilere dönüştürmek için kullanılır.
* reinterpret_cast, türler arasında herhangi bir dönüşüm yapmaz, sadece bit düzeyinde bir yeniden yorumlama yapar. 
* Bu nedenle, kullanırken dikkatli olunmalıdır, çünkü yanlış kullanması durumunda bellek hatalarına veya beklenmeyen davranışlara yol açabilir.
* reinterpret_cast, genellikle düşük seviyeli programlama veya performans kritik kodlarda kullanılır, ancak yüksek seviyeli kodlarda genellikle önerilmez. 
* reinterpret_cast kullanırken, türler arasında uyumsuzluk olup olmadığını kontrol etmek önemlidir, çünkü yanlış kullanımı bellek hatalarına neden olabilir.
? reinterpret_cast kullanımı şu şekildedir:
* *yeni_değişken = reinterpret_cast<yeni_tür*>(eski_tür_değişkeni);
* *yeni değişken = girilecek değer
* cout << *reinterpret_cast<yeni_tür*>(eski_tür_değişkeni) << endl; ile eski tür değişkeninden okunabilir.
*/
int main(){

    uint8_t buffer[16];
    uint32_t* intID = reinterpret_cast<uint32_t*>(buffer);
    *intID = 1024;
    float* floatID = reinterpret_cast<float*>(buffer + sizeof(uint32_t));
    *floatID = 100.5f;
    double* doubleID = reinterpret_cast<double*>(buffer +(sizeof(uint32_t) + sizeof(float)));
    *doubleID = 1540.123456;
    cout << "------[adres kontrolü]------\n";
    cout << "bufer adresi: " << &buffer << endl;
    cout << "int değişkeni adresi: " << intID << endl;
    cout << "float değişkeni adresi: " << floatID << endl;
    cout << "double değişkeni adresi: " << doubleID << endl;
    cout << "------[değerler]------\n";
    cout << "initID: "<< *reinterpret_cast<uint32_t*>(buffer) << endl;
    cout << "floatID: "<< *reinterpret_cast<float*>(buffer + sizeof(uint32_t))<< endl;
    cout << "doubleID: "<< *reinterpret_cast<double*>(buffer +(sizeof(uint32_t) + sizeof(float))) << endl;

    return 0;
}