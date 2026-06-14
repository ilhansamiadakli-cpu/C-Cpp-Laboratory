#include <iostream>
using namespace std;

bool isAsal(int number) {
    // 1 asal değildir, negatifler de öyle.
    if (number <= 1) return false;
    
    // En küçük asal sayılar
    if (number == 2 || number == 3) return true;
    
    // Çift sayıları direkt ele
    if (number % 2 == 0) return false;

    // Kareköküne kadar git (Kritik: <= kullanmalısın)
    for (int i = 3; i * i <= number; i += 2) {
        if (number % i == 0) {
            return false;
        }
    }
    return true;
}

int main() {
    int number;
    int asalAdet = 0;
    
    cout << "Bir sayi giriniz: ";
    if (!(cin >> number)) return 1; 

    for (int i = 1; i <= number; i++) {
        if (isAsal(i)) {
            asalAdet++;
        }
    }
    
    cout << "1 ile " << number << " arasinda " << asalAdet << " adet asal sayi var." << endl;
    return 0;
}