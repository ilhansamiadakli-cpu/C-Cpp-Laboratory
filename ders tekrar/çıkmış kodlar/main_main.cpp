#include <iostream>
#include <vector>

using namespace std;


int main()
{
    // Ucu açık, bomboş akıllı bir dinamik dizi doğuruyoruz
    vector<int> myVector;

    // 1. OYUN: push_back ile arkaya eleman dolduruyoruz
    myVector.push_back(10);
    myVector.push_back(20);
    myVector.push_back(30);

    // Sinsi sorgu testi
    cout << "Size (Eleman Sayisi): " << myVector.size() << "\n";       // Çıktı: 3
    cout << "Capacity (Arsa Buyuklugu): " << myVector.capacity() << "\n"; // Çıktı: 4 (Katlama kuralı!)

    // 2. OYUN: Element erişim muhafızları
    cout << "Ilk Eleman: " << myVector.front() << "\n";  // Çıktı: 10
    cout << "Son Eleman: " << myVector.back() << "\n";   // Çıktı: 30
    cout << "Guvenli Erisim (indeks 1): " << myVector.at(1) << "\n"; // Çıktı: 20

    // 3. OYUN: pop_back ile en arkadaki 30'u vuruyoruz
    myVector.pop_back();
    cout << "pop_back sonrasi yeni son eleman: " << myVector.back() << "\n"; // Çıktı: 20 oldu!

    // 4. OYUN: Modern for döngüsüyle şantiyeyi tarıyoruz
    cout << "--- Vektorun Kalan Elemanlari --- \n";
    for(int x : myVector)
    {
        cout << x << " "; // Ekrana: 10 20 basar
    }
    cout << "\n";

    return 0; // "delete" yazmadık! Çünkü vektör biterken RAM'i otomatik süpürdü!
}