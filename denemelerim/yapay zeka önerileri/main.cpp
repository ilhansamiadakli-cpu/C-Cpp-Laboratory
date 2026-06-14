#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

std::mutex kilit;
std::condition_variable sinyal;

// İletişim bayrağımız
bool sira_birinci_iscide = true; 

void isci_bir() {
    while(true) {
        // Kilit mekanizmasını hazırlıyoruz (unique_lock Condition Variable için şarttır)
        std::unique_lock<std::mutex> kapi(kilit);

        // 1. İşçi çalışıyor...
        std::cout << "1. Isci: Fizik hesaplandi!\n";

        // Sırayı ikinci işçiye ver ve bayrağı değiştir
        sira_birinci_iscide = false;

        // İkinci işçiye "Uyan ve başla!" sinyali gönder
        sinyal.notify_one();

        // Kendi sıram (sira_birinci_iscide == true) gelene kadar kilitlen ve uyu!
        // OS Scheduler ne yaparsa yapsın, bu bayrak true olmadan 1. işçi asla uyanamaz.
        sinyal.wait(kapi, []{ return sira_birinci_iscide; });
    }
}

void isci_iki() {
    while(true) {
        std::unique_lock<std::mutex> kapi(kilit);

        // İkinci işçi başlar başlamaz beklemeye geçer. 
        // Ancak 1. işçinin sırası bittiğinde (bayrak false olduğunda) uyanır.
        sinyal.wait(kapi, []{ return !sira_birinci_iscide; });

        // 2. İşçi çalışıyor...
        std::cout << "\t\t2. Isci: Ekrana cizildi!\n";

        // Sırayı tekrar 1. işçiye ver
        sira_birinci_iscide = true;

        // 1. İşçiyi uyandır
        sinyal.notify_one();
    }
}

int main() {
    //std::thread t1(isci_bir);
    //std::thread t2(isci_iki);
    unsigned int n = std::thread::hardware_concurrency();
    std::cout <<"fiziksel çekirdek sayısı:" <<n<< std::endl; 
    //t1.join();
    //t2.join();
    return 0;
}