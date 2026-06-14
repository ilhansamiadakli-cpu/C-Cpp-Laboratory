#include "analiz.h"

double gunlukOrtalama(const double* sicakliklar, int n){
    double toplam=0.0;
    for (int i=0; i<n; i++) {
        toplam +=sicakliklar[i];
    }
    
    return toplam/n;
}
double enYuksek(const double* sicakliklar, int n){
    double enbuyuk = sicakliklar[0];
    for (int i=0; i<n; i++) {
        if(sicakliklar[i] > enbuyuk){
            enbuyuk = sicakliklar[i];
        }
    }
    return enbuyuk;
}
double enDusuk(const double* sicakliklar, int n){
    double endusuk = sicakliklar[0];
    for (int i=0; i<n; i++) {
        if(endusuk > sicakliklar[i]){
            endusuk = sicakliklar[i];
        }
    }
    return endusuk;
}