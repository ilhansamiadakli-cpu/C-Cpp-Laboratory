#include <iostream>
#include <vector>
using namespace std;
int main()
{
    //* Vektörler, dinamik olarak boyutlandırılabilen dizilerdir. Eleman ekleyebilir, kaldırabilir ve erişebilirsiniz.
    vector<int> numbers;
    numbers = {1,2,3,4,5,6,7,8,9,10};
    cout <<"numbers.at(): "<<numbers.at(0)<< endl;//* at() metodu, belirtilen konumdaki elemanı döndürür ve sınırları kontrol eder. Eğer geçersiz bir konum belirtilirse, out_of_range istisnası fırlatır.
    cout <<"numbers[1]: "<<numbers[1]<< endl;//* [] operatörü, belirtilen konumdaki elemanı döndürür ancak sınırları kontrol etmez. Geçersiz bir konum belirtilirse, tanımsız davranışa yol açabilir.
    cout <<"numbers.front(): "<<numbers.front()<< endl;//* front() metodu, vektörün ilk elemanını döndürür. Vektör boşsa, tanımsız davranışa yol açabilir.
    cout <<"numbers.back(): "<<numbers.back()<< endl;//* back() metodu, vektörün son elemanını döndürür. Vektör boşsa, tanımsız davranışa yol açabilir.
    cout <<"numbers.size(): "<<numbers.size() << endl;//* size() metodu, vektördeki eleman sayısını döndürür.
    cout <<"numbers.capacity(): "<<numbers.capacity() << endl;//* capacity() metodu, vektörün şu anda depolayabileceği maksimum eleman sayısını döndürür. Vektör büyüdükçe kapasite genellikle artar.
    numbers.push_back(11);//* push_back() metodu, vektörün sonuna yeni bir eleman ekler. Eğer mevcut kapasite yeterli değilse, vektörün kapasitesi artırılır.
    cout <<"numbers.size() after push_back: "<<numbers.size() << endl;
    cout <<"numbers.capacity() after push_back: "<<numbers.capacity() << endl;//* burada kapasite 20 oldu çünkü vektörler genellikle kapasiteyi iki katına çıkarır.
    numbers.pop_back();//* pop_back() metodu, vektörün son elemanını kaldırır. Bu işlem vektörün boyutunu azaltır ancak kapasiteyi değiştirmez.
    cout <<"numbers.size() after pop_back: "<<numbers.size() << endl;//* pop_back() metodu, vektörün son elemanını kaldırır, bu yüzden boyut 10'a düşer.
    cout <<"numbers.capacity() after pop_back: "<<numbers.capacity() << endl;//* pop_back sadece son elemanı kaldırır, kapasiteyi değiştirmez.
    cout <<"numbers.empty(): "<<numbers.empty() << endl; //* vektör boş mu kontrol eder.
    numbers.clear(); //* vektörün tüm elemanlarını kaldırır, boyutu sıfırlar ancak kapasiteyi değiştirmez.
    cout <<"numbers.size() after clear: "<<numbers.size() << endl;//* clear() metodu, vektörün tüm elemanlarını kaldırır, bu yüzden boyut 0 olur.
    cout <<"numbers.capacity() after clear: "<<numbers.capacity() << endl;//* clear() metodu, vektörün tüm elemanlarını kaldırır ancak kapasiteyi değiştirmez, bu yüzden kapasite hala 20'dir.
    return 0;
}
