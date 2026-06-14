#include <iostream>
#include <fstream>
#include <string>
#include "colors.h"
using namespace std;
/*
? ofstream:
Dosya oluşturmak ve yazmak için kullanılır.
? ifstream:
Var olan bir dosyayı okumak için kullanılır.
? fstream:
Hem okuma hem de yazma işlemleri için kullanılır.
*/
string Rengicoz(string renk);

namespace Voide {
    
    
    enum AnlikDurum {
        BOSTA,
        OKUYOR,
        BASKI,
    };

    struct OkuyucuCantasi {
        int satirSayisi = 0;
        string metin = "";
        AnlikDurum durum = BOSTA;
        bool hataVarMi = false;   
        string renk =""; 
    };
}
int main() {
    ifstream dosya("deneme.txt");
    if (!dosya.is_open()) {
         cerr << "Dosya açılamadı!" << endl;
         exit(1);
    }
    Voide ::OkuyucuCantasi canta;
    string satır;
    cout<<"\n\033[H\033[J" << endl;
    while (getline(dosya,satır)) {
        canta.satirSayisi++;
        for (int i=0; i<satır.length(); i++) {
            char harf = satır[i];

            if(harf == '/' && i+1<satır.length() && satır[i+1] == '/'){
                break;
            }
            switch (canta.durum) {
                case Voide::BOSTA:
                    if(harf == '{'){
                        canta.durum = Voide::OKUYOR;
                    }else if(harf == '['){
                        canta.durum = Voide::BASKI;
                    }
                    else if(harf == ' ' || harf == '\t' || harf == '\r'){
                        continue;
                    }
                    else{
                        canta.hataVarMi = true;
                    }
                    break;
                case Voide::OKUYOR:
                    if(harf=='}'){
                       canta.durum = Voide::BOSTA;
                    }
                    else{
                        canta.metin += harf;
                    }
                    break;
                case Voide::BASKI:
                    if(harf == ']'){
                        canta.durum = Voide::BOSTA;
                    }
                    else{
                        canta.renk += harf;
                    }
                    
                    break;

            }
            if(canta.hataVarMi){
                cerr<<"[Warning]:"<<canta.satirSayisi<<
                ". satırda sözdizimi hatası lütfen yorum yada metin belirtmek için sözdizi kurallarını belirtin"
                << endl;
                break;
            }
        }
        if(canta.hataVarMi) break;
        
        else if(canta.durum == Voide::OKUYOR){
            canta.metin += "\n";
        }
    }
    dosya.close();
    if (!canta.hataVarMi){
        cout<< Rengicoz(canta.renk);
        cout<< canta.metin;
        cout<< RESET;
    }
    return 0;
}
string Rengicoz(string renk) {
    if      (renk == "yesil" || renk == "YESIL") return YESIL;
    else if (renk == "beyaz" || renk == "BEYAZ") return BEYAZ;
    else if (renk == "kirmizi" || renk == "KIRMIZI") return KIRMIZI;
    else if (renk == "sari" || renk == "SARI") return SARI;
    else if (renk == "mavi" || renk == "MAVI") return MAVI;
    else if (renk == "acik_mavi" || renk == "ACIK_MAVI") return ACIK_MAVI;
    else if (renk == "mor" || renk == "MOR") return MOR;
    else if (renk == "siyah" || renk == "SIYAH") return SIYAH;
    else if (renk == "cyan" || renk == "CYAN") return CYAN;
    else if (renk == "magenta" || renk == "MAGENTA") return MAGENTA;
    else return BEYAZ; // Varsayılan renk
}