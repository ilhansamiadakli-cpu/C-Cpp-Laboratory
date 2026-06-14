#include <cstddef>
#include <iostream>
#include <string>
#include <vector>
#include <cctype>
using namespace std;
class HesapMakinesi{
    private:
    enum tokenType {SAYI, OPERATOR};
    struct token{
        tokenType type;
        double number;
        char Op;
    };
    vector<token> myToken;

    void adNumber(double number){
        token t;
        t.type = SAYI;
        t.number = number;
        t.Op = '\0';
        myToken.push_back(t);
    }
    void adOpeartor(char Operator){
        token t;
        t.type = OPERATOR;
        t.Op = Operator;
        // sayı yazmassak olur herhalde
        myToken.push_back(t);
    }
    void ayristir(std::string satir) {
        std::string sayi_deposu = "";
        for (char c : satir) {
            if (isdigit(c)) {
                sayi_deposu += c; // Sayıyı biriktir
            } else if (c == '+' || c == '-' || c == '*' || c == '/') {
                if (!sayi_deposu.empty()) {
                    adNumber(stod(sayi_deposu));
                    sayi_deposu = "";
                }
                adOpeartor(c);
            }
        }
        // Satır bittiğinde son sayıyı yazdır
        if (!sayi_deposu.empty()) {
            adNumber(stod(sayi_deposu)) ;
        }
    }
    double parseEt() {
        // ADIM 1: Çarpma ve Bölme İşlemleri
        vector<token> geciciListe;
        // ön kontrol
        for (int i = 0; i <myToken.size(); i++) {
            if(myToken[i].type == OPERATOR){
                if( i+1<myToken.size() && myToken[i+1].type == OPERATOR){
                    cerr << "hata iki matematiksel işlem yanyana olmuş" << endl;
                    return 0;
                }
            }
        }
        
        for (int i = 0; i < myToken.size(); i++) {
            // Eğer o anki eleman bir * veya / operatörü ise
            if (myToken[i].type == OPERATOR && (myToken[i].Op == '*' || myToken[i].Op == '/')) {
                
                // 1. Gecici listenin en sonundaki sayıyı al ve listeden sil
                double solSayi = geciciListe.back().number;
                geciciListe.pop_back(); 
                
                // 2. Asıl listedeki bir sonraki sayıyı al
                double sagSayi = myToken[i + 1].number;
                
                // 3. İşlemi yap
                double sonuc = 0;
                if (myToken[i].Op == '*') sonuc = solSayi * sagSayi;
                if (myToken[i].Op == '/') {
                    if(sagSayi == 0){
                        cerr << "hata payda 0 olamaz"<< endl;
                        return 0;// şimdilik 0 döndürebilir
                    }
                    else {
                    sonuc = solSayi / sagSayi;
                    }
                }
                
                // 4. Sonucu yeni bir 'SAYI' token'ı olarak geçici listeye ekle
                token t;
                t.type = SAYI;
                t.number = sonuc;
                geciciListe.push_back(t);
                
                // 5. Sağdaki sayıyı kullandığımız için döngüyü 1 adım fazladan atlat
                i++; 
            } 
            else {
                // Çarpma veya bölme değilse, elemanı geçici listeye olduğu gibi aktar
                geciciListe.push_back(myToken[i]);
            }
        }

        // ADIM 2: Toplama ve Çıkarma İşlemleri (Artık elimizde geciciListe var)
        // İlk sayıyı başlangıç sonucu olarak alıyoruz
        double finalSonuc = geciciListe[0].number; 
        
        // Operatörler hep 1, 3, 5 gibi tek sayılardaki indekslerde olur
        for (int i = 1; i < geciciListe.size(); i += 2) {
            char op = geciciListe[i].Op;
            double sagSayi = geciciListe[i + 1].number; // Operatörün hemen sağındaki sayı
            
            if (op == '+') finalSonuc += sagSayi;
            if (op == '-') finalSonuc -= sagSayi;
        }

        return finalSonuc;
    }

    public:
    HesapMakinesi(string satir){
        ayristir(satir);
        cout << "işlem sonucu: "<<parseEt()<< endl;
    }

};


int main(){
    cout << "lütfen bir matematik işlemi giriniz: " << endl;
    string örnek;
    getline(cin,örnek);
    HesapMakinesi hesap(örnek);

    return 0;
}