#include<iostream>
using namespace std;
template<typename T>

void solaKaydır(T* arr, int size, int kaydirmaMiktari) {
    T* temp = new T[size]; // Geçici dizi oluştur

    for (int i = 0; i < size; i++) {
        temp[i] = arr[(i + kaydirmaMiktari) % size]; // Elemanları kaydır
    }

    for (int i = 0; i < size; i++) {
        arr[i] = temp[i]; // Kaydırılmış elemanları orijinal diziye kopyala
    }

    delete[] temp; // Geçici diziyi temizle
}
int main(){
    int arr[] = {4,5,6,7,8};
    solaKaydır<int>(arr, 5, 2);
    cout << "Kaydırılmış Dizi: ";
    for (int i = 0; i < 5; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
    return 0;
}