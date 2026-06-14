#include <cstddef>
#include <cstring>
#include <iostream>
#include <cstdlib>
//#include <new>
using namespace std;
/*
todo: bir sınıf yaratmalıyım
* bu sınıfın içinde createArena, myMalloc ve Reset fonksiyonları olacak
* ClassName() ile createArena() fonksiyonu için max belleği alma seçeneği olucak, değer girilmesede 1024 baytlık alan tahsil edilicek 
* bu sınıfın içinde bir struct olacak bu struct entitiy oluşturucak.
* createEntitiy() fonksiyonu new ile myMalloc ile verilen adrese verileri yazacak
* ~ClassName ile delet komutlarını çağırıp bellek sızmasını önlemiş olucam
* 
* 
*/
class Arena {
private:
    void* buffer;
    int ofset;
    int capacity;

public:
    // 1. KURUCU (Constructor) - Doğum Anı
    //! listendeki "max belleği alma" kısmı burada. 'createArena'ya gerek kalmadı!
    Arena(int kapasite = 1024) {
        capacity = kapasite;
        ofset = 0;
        buffer = malloc(capacity);
        
        if (!buffer) {
            cerr << "KRİTİK HATA: İşletim sistemi Arena için RAM vermedi!" << endl;
            exit(1); // Programı durdur
        }
        cout << capacity << " baytlik Arena basariyla kuruldu." << endl;
    }

    // 2. YIKICI (Destructor) - Ölüm Anı (Todo listendeki sızıntı önleyici!)
    // Sen main'in sonunda 'free' yazmayı unutsan bile, bu Arena silinirken otomatik çalışır.
    ~Arena() {
        if (buffer) {
            free(buffer);
            cout << "Arena silindi, RAM isletim sistemine geri verildi." << endl;
        }
    }

    // 3. TAHSİS EDİCİ
    void* myMalloc(int bayt, size_t aligin) {
        size_t yeniOfset = (ofset + aligin - 1) & ~(aligin - 1);
        
        if (bayt + yeniOfset > capacity) {
            cerr << "DİKKAT: Arena kapasitesi aşıldı! " << bayt << " bayt yer yok." << endl;
            return nullptr;
        }
        
        void* safeBuffer = (char*)buffer + yeniOfset;
        ofset = yeniOfset + bayt;
        return safeBuffer;
    }

    // 4. SIFIRLAYICI
    // Artık dışarıdan parametre (voideArena* arena) almasına gerek yok, kendi ofsetini biliyor.
    void Reset() {
        ofset = 0;
        cout << "Arena sifirlandi (0. ofset)." << endl;
    }
};
struct dusman{
    int can;// + 4 bayt
    int saldırı; // + 4 bayt
    int ID = 33; // + 4 bayt olması gereken
    char harfi; // güüüm burası 1 bayt ama sizeof(dusman) değeri 13 değil 16 oldu çünkü alignment var yani bellekteki hizalama kuralları gereği 4 baytlık bir alan kaplıyor.
    dusman(int c,int s){
        can = c;
        saldırı = s;
    }
};

int main() {
    // Sadece bunu yazarak hem kapasiteyi belirledin, hem malloc yaptın! (RAII Kuralı)
    Arena levelArenasi(2048); 
    
    // Arena sınıfının içindeki fonksiyona nokta (.) ile erişiyoruz
    void* hamAlan = levelArenasi.myMalloc(16, 4); 

    levelArenasi.Reset();

    return 0; 
}
