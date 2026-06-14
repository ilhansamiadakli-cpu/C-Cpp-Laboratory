#include "donusum.h"
#include "analiz.h"
#include <iostream>

// BURAYA TAKMAYIN HOCAM BU BENİM KODLARIM
#define YESIL      "\033[1;32m"
#define BEYAZ      "\033[0m"
#define KIRMIZI    "\033[1;31m"
#define SARI       "\033[1;33m"
#define MAVI       "\033[1;34m"
#define ACIK_MAVI  "\033[1;36m"
#define MOR        "\033[1;35m"
#define SIYAH      "\033[1;30m"
#define CYAN       "\033[1;36m"
#define MAGENTA    "\033[1;35m"
// ---------------------------------

using namespace std;

int main(){

    double hafta[] = {18.5, 22.0, 15.3, 25.8, 20.1, 17.6, 23.4};
    int n = 7;
    cout <<"\n\033[H\033[J" << endl; // bu da benim kodum :)
    cout << YESIL<<"----[SICAKLIK DEĞERLERİ]-----\n\n"<<BEYAZ;
    cout <<"En büyük sıcaklık: "<< YESIL << enYuksek(hafta, n) << "`C" <<BEYAZ << endl;
    cout <<"En düşük sıcaklık: "<< KIRMIZI << enDusuk(hafta, n)<< "`C"  <<BEYAZ << endl;
    cout <<"Ortalama sıcaklık: "<< SARI << gunlukOrtalama(hafta, n)<< "`C"  << BEYAZ << endl;
    double c = gunlukOrtalama(hafta, n);
    cout << "\n";
    cout <<"Ortalama sıcaklığın Fahrenheit değeri: "<< CYAN<<celsiusToFahrenheit(c)<< "`F"<<BEYAZ<< endl;
    cout <<"Ortalama sıcaklığın Kelvin değeri: "<< MOR<< celsiusToKelvin(c)<<"`K"<< BEYAZ<< "\n\n";
    return 0;
}