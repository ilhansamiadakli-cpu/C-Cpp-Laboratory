#include <iostream>
#include <string>

using namespace std;

void hasarAl(int *hp, int hasar);

void seviyeAtla(int &hp, int &attack, int &armor, double katsayi);

void swapEkipman(int &attack1, int &armor1, int &attack2, int &armor2);

int* enDusukHP(int *hp1, int *hp2, int *hp3);

void karakterYazdir(string isim, int hp, int attack, int armor);


int main() {
    
    string isim1 = "Savasci", isim2 = "Buyucu", isim3 = "Koruyucu";
    int hp1 = 150, atk1 = 80, arm1 = 60;
    int hp2 = 90, atk2 = 120, arm2 = 30;
    int hp3 = 200, atk3 = 40, arm3 = 90;

    cout << "--- Karakterler Hazirlandi ---" << endl;
    karakterYazdir(isim1, hp1, atk1, arm1);
    karakterYazdir(isim2, hp2, atk2, arm2);
    karakterYazdir(isim3, hp3, atk3, arm3);

    
    cout << "\n[Savas gerceklesti!]" << endl;
    hasarAl(&hp1, 50);   
    hasarAl(&hp2, 100);  

    
    cout << "\n[Koruyucu seviye atladi!]" << endl;
    seviyeAtla(hp3, atk3, arm3, 1.5); 

    
    cout << "\n[Ekipman takasi yapildi!]" << endl;
    swapEkipman(atk1, arm1, atk2, arm2); 

    
    cout << "\n[Kritik Iyilestirme Uygulaniyor... ]" << endl;
    int* zayifHP_adresi = enDusukHP(&hp1, &hp2, &hp3); 
    *zayifHP_adresi += 50; 

    
    cout << "\n=== OYUN SONU ISTATISTIKLERI ===" << endl;
    
    karakterYazdir(isim1, hp1, atk1, arm1);
    karakterYazdir(isim2, hp2, atk2, arm2);
    karakterYazdir(isim3, hp3, atk3, arm3);

    return 0;
}




void hasarAl(int *hp, int hasar) {
    *hp -= hasar;
    if (*hp < 0) {
        *hp = 0; 
    }
}

void seviyeAtla(int &hp, int &attack, int &armor, double katsayi) {
    hp = hp * katsayi;
    attack = attack * katsayi;
    armor = armor * katsayi;
}

void swapEkipman(int &attack1, int &armor1, int &attack2, int &armor2) {
    int tempAtk = attack1;
    attack1 = attack2;
    attack2 = tempAtk;

    int tempArm = armor1;
    armor1 = armor2;
    armor2 = tempArm;
}

int* enDusukHP(int *hp1, int *hp2, int *hp3) {
    int* minHP = hp1; 
    
    if (*hp2 < *minHP) {
        minHP = hp2;
    }
    if (*hp3 < *minHP) {
        minHP = hp3;
    }
    
    return minHP;
}

void karakterYazdir(string isim, int hp, int attack, int armor) {
    cout << isim << "\t-> HP: " << hp << "\t| ATK: " << attack << "\t| ARM: " << armor << endl;
}