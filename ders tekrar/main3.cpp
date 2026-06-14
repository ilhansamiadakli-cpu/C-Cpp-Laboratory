#include <iostream>
using namespace std;

// Pointer'ın kopyasını alır (Pass by Value)
void adresiDegistirDene(int *p, int *yeniAdres) {
    p = yeniAdres; // Sadece fonksiyon içindeki kopya değişir
}

// Pointer'ın kendisine referans alır (Pass by Reference)
void adresiKalıcıDegistir(int *&p, int *yeniAdres) {
    p = yeniAdres; // Dışarıdaki orijinal pointer da artık yeni adresi gösterir
}

int main() {
    int x = 10, y = 20;
    int *ptr = &x;

    cout << "Baslangic: ptr -> x'i gosteriyor (" << *ptr << ")" << endl;

    adresiDegistirDene(ptr, &y);
    cout << "Dene sonrasi: ptr -> " << (*ptr == 10 ? "Hala x" : "y") << endl;

    adresiKalıcıDegistir(ptr, &y);
    cout << "Kalıcı degisim sonrasi: ptr -> " << (*ptr == 20 ? "Artik y" : "x") << endl;

    return 0;
}