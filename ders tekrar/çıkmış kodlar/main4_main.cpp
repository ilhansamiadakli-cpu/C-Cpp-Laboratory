#include<iostream>
using namespace std;
int hesapla(int n){
    cout << "çağırıldı: " << n << endl;
    if(n<=0){
        cout << "taban:" << n << endl;
        return 0;
    }
    if (n%2 == 0){
        int sonuc = hesapla(n - 2) + n;
        return sonuc;
    }
    else{
        int sonuc = hesapla(n-1) + n;
        return sonuc;
    }
}
int main(){
    int değer = hesapla(5) + hesapla(3);
    cout << "SONUC= " << değer << endl; // 5 için: 5 + 4 + 2 + 0 = 11, 3 için: 3 + 2 + 0 = 5, Toplam: 16

    return 0;
}