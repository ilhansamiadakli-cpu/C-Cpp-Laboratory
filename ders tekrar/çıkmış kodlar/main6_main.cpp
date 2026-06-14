#include<iostream>
#include <fstream>
using namespace std;
struct metin{
    string str;
    int tekrarsayısı;
};
int main(){
    string satırlar[1000];
    string satır;
    ifstream dosya("metin.txt");
    int satırAdet = 0;
    while(getline(dosya, satır)){
        satırlar[satırAdet] = satır;
        satırAdet++;
    }
    metin metinler[1000];
    for (int i = 0; i< satırAdet; i++) {
        metinler[i].str = satırlar[i];
        metinler[i].tekrarsayısı = 1; // Her satır başlangıçta 1 kez göründüğü varsayılarak başlatılır
        for (int j = 0; j < i; j++) {
            if (satırlar[i] == satırlar[j]) {
                metinler[i].tekrarsayısı++;
            }
        }
    }
    // en çok tekrar eden metni bulma
    int maxTekrar = 0;
    string enCokTekrarEdenMetin;
    for (int i = 0; i < satırAdet; i++) {
        if (metinler[i].tekrarsayısı > maxTekrar) {
            maxTekrar = metinler[i].tekrarsayısı;
            enCokTekrarEdenMetin = metinler[i].str;
        }
    }
    cout << "En çok tekrar eden metin: " << enCokTekrarEdenMetin << " (Tekrar sayısı: " << maxTekrar << ")" << endl;
    return 0;
}