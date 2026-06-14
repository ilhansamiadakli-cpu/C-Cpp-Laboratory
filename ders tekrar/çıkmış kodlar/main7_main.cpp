#include <iostream>
#include <vector>
#include <stdexcept> // out_of_range istisnası için

using namespace std;

int main() {
    // --- BAŞLANGIÇ: Vektör Tanımlama ---
    vector<int> numbers = {10, 20, 30, 40, 50};
    vector<int> other_numbers = {100, 200};

    cout << "=== 1. ELEMANA ERİŞİM FONKSİYONLARI ===" << endl;
    cout << "numbers.at(0): " << numbers.at(0) << endl; //* at() metodu, belirtilen konumdaki elemanı döndürür ve sınırları kontrol eder. Eğer geçersiz bir konum belirtilirse, out_of_range istisnası fırlatır.
    cout << "numbers[1]: " << numbers[1] << endl; //* [] operatörü, belirtilen konumdaki elemanı döndürür ancak sınırları kontrol etmez. Geçersiz bir konum belirtilirse, tanımsız davranışa (undefined behavior) yol açabilir.
    cout << "numbers.front(): " << numbers.front() << endl; //* front() metodu, vektörün ilk elemanına referans döndürür. Vektör boşsa, tanımsız davranışa yol açabilir.
    cout << "numbers.back(): " << numbers.back() << endl; //* back() metodu, vektörün son elemanına referans döndürür. Vektör boşsa, tanımsız davranışa yol açabilir.
    cout << "*numbers.data(): " << *numbers.data() << endl; //* data() metodu, vektörün arkasındaki ham diziye (raw array) ait bellek adresini (pointer) döndürür. Düşük seviyeli C-Style API'lerle çalışırken kullanışlıdır.


    cout << "\n=== 2. KAPASİTE VE BOYUT FONKSİYONLARI ===" << endl;
    cout << "numbers.empty(): " << numbers.empty() << endl; //* empty() metodu, vektörün boş olup olmadığını kontrol eder. Boşsa true (1), doluysa false (0) döndürür.
    cout << "numbers.size(): " << numbers.size() << endl; //* size() metodu, vektörün içindeki mevcut eleman sayısını döndürür.
    cout << "numbers.max_size(): " << numbers.max_size() << endl; //* max_size() metodu, sistemin ve bilgisayar mimarisinin izin verdiği, bir vektörün tutabileceği maksimum teorik eleman sayısını döndürür.
    cout << "numbers.capacity(): " << numbers.capacity() << endl; //* capacity() metodu, vektörün yeni bir bellek alanı allocate etmeden (yeniden boyutlanmadan) tutabileceği maksimum eleman sayısını döndürür.
    
    numbers.reserve(20); //* reserve() metodu, vektörün kapasitesini (capacity) en az belirtilen sayıya yükseltir. Gereksiz re-allocation (belleği boşaltıp yeniden taşıma) işlemlerini engellemek için performans optimizasyonunda kritik rol oynar. Boyutu (size) değiştirmez.
    cout << "reserve(20) sonrasi capacity: " << numbers.capacity() << endl;

    numbers.shrink_to_fit(); //* shrink_to_fit() metodu, vektörün kapasitesini (capacity), mevcut eleman sayısına (size) eşitleyecek şekilde küçültür ve kullanılmayan belleği sisteme geri verir. Bağlayıcı bir kural değildir, derleyiciye bir "istektir".
    cout << "shrink_to_fit() sonrasi capacity: " << numbers.capacity() << endl;


    cout << "\n=== 3. DEĞİŞTİRİCİLER (MODIFIERS) ===" << endl;
    numbers.push_back(60); //* push_back() metodu, parametre olarak verilen elemanın bir kopyasını (veya rvalue ise taşıyarak) vektörün sonuna ekler.
    
    numbers.pop_back(); //* pop_back() metodu, vektörün sonundaki elemanı siler. Vektör boşsa tanımsız davranışa yol açar. Silinen elemanı döndürmez (void).

    numbers.insert(numbers.begin() + 1, 15); //* insert() metodu, belirtilen iteratör konumuna yeni eleman ekler. Bu örnekte 1. indekse 15 ekler ve sonraki elemanları sağa kaydırır. Doğrusal zamanda ($O(n)$) çalıştığı için maliyetlidir.

    numbers.erase(numbers.begin() + 1); //* erase() metodu, belirtilen iteratör konumundaki elemanı (veya iki iteratör arasındaki aralığı) siler. Silinen elemandan sonrakiler sola kaydırılır. $O(n)$ maliyetlidir.

    numbers.emplace(numbers.begin() + 1, 25); //* emplace() metodu, insert() gibi çalışır ancak nesneyi doğrudan vektörün belleğinde construct eder (oluşturur). Geçici nesne kopyalama maliyetini önleyerek performans avantajı sağlar.

    numbers.emplace_back(70); //* emplace_back() metodu, push_back() gibi çalışır ancak nesneyi vektörün sonunda doğrudan belleğin içinde oluşturur. Özellikle büyük struct ve sınıflarda kopyalama/taşıma maliyetini sıfıra indirir.

    numbers.resize(7, 0); //* resize() metodu, vektörün boyutunu (size) değiştirir. Yeni boyut mevcut boyuttan büyükse, aradaki farkı verilen varsayılan değerle (burada 0) doldurur. Küçükse, sığmayan elemanları yok eder.

    numbers.swap(other_numbers); //* swap() metodu, iki vektörün içeriğini (aslında arka plandaki pointer'larını, size ve capacity değerlerini) takas eder. Çok hızlı ($O(1)$) çalışır çünkü elemanlar tek tek kopyalanmaz.
    numbers.swap(other_numbers); // Eski haline geri getirelim.

    numbers.assign(3, 100); //* assign() metodu, vektörün içindeki tüm eski elemanları siler ve yerine belirtilen sayıda (burada 3 tane) ve değerde (100) yeni eleman atar. Vektörü sıfırlayıp yeniden doldurmak için optimize edilmiştir.

    numbers.clear(); //* clear() metodu, vektörün içindeki tüm elemanları yok eder (destructor'larını çağırır). İşlem sonrasında vektörün boyutu (size) 0 olur, ancak kapasitesi (capacity) genellikle değişmeden kalır (bellek iade edilmez).


    cout << "\n=== 4. İTERATÖRLER (ITERATORS) ===" << endl;
    // Yeniden dolduralım
    numbers = {10, 20, 30};

    cout << "*numbers.begin(): " << *numbers.begin() << endl; //* begin() metodu, vektörün ilk elemanını gösteren bir iteratör (pointer benzeri yapı) döndürür.
    cout << "*(numbers.end() - 1): " << *(numbers.end() - 1) << endl; //* end() metodu, vektörün son elemanından *bir sonraki* hayali konumu gösteren bir iteratör döndürür. Döngü sınırlarını belirlemek için kullanılır. Direkt dereference edilirse tanımsız davranış olur.
    
    cout << "*numbers.rbegin(): " << *numbers.rbegin() << endl; //* rbegin() metodu, vektörün tersine ilk elemanını (yani aslında gerçek son elemanı) gösteren bir reverse_iterator döndürür. Vektörü sondan başa taramak için kullanılır.
    cout << "*(numbers.rend() - 1): " << *(numbers.rend() - 1) << endl; //* rend() metodu, vektörün tersine son elemanından (yani gerçek ilk elemandan) bir önceki hayali konumu gösteren bir reverse_iterator döndürür.

    cout << "*numbers.cbegin(): " << *numbers.cbegin() << endl; //* cbegin() metodu, vektörün ilk elemanını gösteren bir const_iterator döndürür. Bu iteratör üzerinden vektörün elemanları değiştirilemez (salt okunur - read only).
    cout << "*numbers.cend(): " << *(numbers.cend() - 1) << endl; //* cend() metodu, vektörün son elemanından bir sonrasını gösteren salt okunur const_iterator döndürür.
    cout << "*numbers.crbegin(): " << *numbers.crbegin() << endl; //* crbegin() metodu, vektörün son elemanını gösteren salt okunur ters yönlü const_reverse_iterator döndürür.
    cout << "*numbers.crend(): " << *(numbers.crend() - 1) << endl; //* crend() metodu, vektörün ilk elemanından bir öncesini gösteren salt okunur ters yönlü const_reverse_iterator döndürür.

    return 0;
}