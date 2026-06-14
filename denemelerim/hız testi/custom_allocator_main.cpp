#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <cstdint>
#include <cstring>

class MemoryArena {
private:
    uint8_t* hambellek_adres;
    size_t bellek_boyutu;
    size_t ofset;
public:
    MemoryArena(size_t boyut){
        bellek_boyutu = boyut;
        hambellek_adres = (uint8_t*)malloc(boyut);
        // olası sorunların önüne geçmek için belleği temizle
        std::memset(hambellek_adres, 0, boyut);
        ofset = 0;
    }
    void* arena_alloc(size_t istenenBoyut){
        if(istenenBoyut + ofset > bellek_boyutu){
            std::cerr << "[EROR]: bellek yetersiz" << std::endl;
            return nullptr;
        }
        void* verilecekAdres = &hambellek_adres[ofset];
        ofset = ofset + istenenBoyut;
        return verilecekAdres;
    }
    void arena_reset(){
        // burda çoklu free desteği verilmemiş ama ilerde onu eklemeyi planlıcam
        ofset = 0;
    }
    ~MemoryArena(){
        free(hambellek_adres);
    }
};
template <typename T>
class myVector {
private:
    T* veri;
    size_t boyut;
    size_t kapasite;
    MemoryArena& arena;
public:
    myVector(MemoryArena& harici_arena) : arena(harici_arena), boyut(0), kapasite(2) { // bu kısım karmaşık geldi ama geminiye güvenicem
        // İlk başta 2 elemanlık yeri arenadan ışık hızında koparıyoruz
        veri = (T*)arena.arena_alloc(kapasite * sizeof(T));
    }

    void push_back(const T& değer){
        if(kapasite == boyut){
            std::cout << "[DİKKAT]: kapasite yetersizdir ";
            size_t yeni_kapasite = kapasite*2;
            T* yeni_veri = (T*)arena.arena_alloc(yeni_kapasite*sizeof(T));
            if(yeni_veri == nullptr){
                std:: cerr << " veri yazılamadı" << std::endl;
            }
            else{
            for (int i = 0; i < boyut; i++) {
                yeni_veri[i] = veri[i];
            }
            std::cout << yeni_kapasite - kapasite << " kadar alan açıldı" << std::endl;
            veri = yeni_veri;
            kapasite = yeni_kapasite;
            }
            
        }
        veri[boyut] = değer;
        boyut++;
    }
    size_t size(){
        return boyut;
    }

    size_t capacity(){
        return kapasite;
    }
    T at(int konum){
        return veri[konum];
    }
    void clear(){
        boyut = 0;
        kapasite = 2;
    }
    void pop_back(){
        if(boyut > 0)boyut--;
    }

};
int main() {
    std::cout << "=== SISTEM STRES TESTI BASLIYOR ===\n\n";

    // 1. ADIM: İşletim sisteminden 1024 baytlık (1 KB) ham arsayı alıyoruz.
    MemoryArena myarena(1024);

    // 2. ADIM: Vektörümüzü bu arsanın üzerine inşa ediyoruz.
    myVector<int> v(myarena);

    std::cout << "[ASAMA 1]: Kapasite Buyume (Re-allocation) Testi\n";
    std::cout << "--------------------------------------------------\n";
    // Sadece 15 eleman ekliyoruz. Senin yazdığın o "X kadar alan açıldı" 
    // mesajlarının arka arkaya nasıl patladığını izle!
    for (int i = 0; i < 15; i++) {
        v.push_back(i * 10);
    }
    
    std::cout << "\nSu anki Boyut (Size): " << v.size() << "\n";
    std::cout << "Su anki Kapasite (Capacity): " << v.capacity() << "\n";


    std::cout << "\n[ASAMA 2]: Hafiza Okuma Testi\n";
    std::cout << "--------------------------------------------------\n";
    // 0, 10, 20, 30, 40, 50... diye gittiği için 5. indeks 50 olmalı.
    std::cout << "5. indisteki veri (Beklenen 50): " << v.at(5) << "\n";


    std::cout << "\n[ASAMA 3]: Gaddar Optimizasyon Testi (pop_back & clear)\n";
    std::cout << "--------------------------------------------------\n";
    v.pop_back(); // Son elemanı (140) uçuruyoruz. (Sadece sınırı daraltıyor)
    std::cout << "pop_back calisti. Yeni Boyut: " << v.size() << "\n";
    
    v.clear();
    std::cout << "clear calisti. Yeni Boyut: " << v.size() << "\n";
    std::cout << "(Not: clear calisti ama Arena hala hayatta, diger vektorler guvende!)\n";


    std::cout << "\n[ASAMA 4]: LIMITLERI ZORLAMA (Mavi Ekrana Gidis)\n";
    std::cout << "--------------------------------------------------\n";
    // Arsamız 1024 bayttı. Ancak her kapasite artışında eski veriyi bırakıp 
    // yeni yere geçtiğimiz için arsamız hızlıca tükenecek.
    // Bakalım kaçıncı elemanda sistem "bellek yetersiz" diye bağıracak?
    
    int eklenen_eleman = 0;
    for (int i = 0; i < 500; i++) {
        // Eğer push_back işlemi içindeki arena_alloc 'nullptr' döndürürse
        // program çöker (segmentation fault). 
        // Senin yazdığın error mesajını görmek için sınırları zorluyoruz.
        v.push_back(i);
        eklenen_eleman++;
        
        // Kapasite 256, 512 gibi devasa rakamlara ulaştığında arena yetmeyecek.
        if (v.capacity() * sizeof(int) > 1024) {
            std::cout << "\n>> LIMIT ASILDI! Vektor 1024 Baytlik arsayi yuttu.\n";
            std::cout << ">> Toplam eklenebilen eleman sayisi: " << eklenen_eleman << "\n";
            break; // Döngüyü kırıp çöküşü engelliyoruz
        }
    }

    std::cout << "\n=== SISTEM TESTI BASARIYLA TAMAMLANDI ===\n";
    return 0;
}