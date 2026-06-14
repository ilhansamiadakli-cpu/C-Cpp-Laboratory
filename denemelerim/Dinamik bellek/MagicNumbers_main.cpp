#include <cstdint>
#include <iostream>

void checkMemory(uint8_t* mem){
    uint32_t* mevcutNumara = reinterpret_cast<uint32_t*>(mem + (sizeof(uint32_t)*2));
    if(*mevcutNumara == 0xDEADBEEF){
        std::cout <<"[OK] mevcut bellek korunmuş" << std::endl;
    }
    else{
        std::cerr << "[CRITICAL] Memory Corruption Detected! Kanarya öldü!" << std::endl;
    }
}

int main(){
    uint8_t buffer[12];
    uint32_t* Can = reinterpret_cast<uint32_t*>(buffer);
    *Can = 100;
    uint32_t* Zırh = reinterpret_cast<uint32_t*>(buffer + sizeof(uint32_t));
    *Zırh = 50;
    uint32_t* Kanarya = reinterpret_cast<uint32_t*>(buffer + (sizeof(uint32_t)*2));
    *Kanarya = 0xDEADBEEF;
    checkMemory(buffer);
    
    uint32_t* Katil = reinterpret_cast<uint32_t*>(buffer + (sizeof(uint32_t)*2));
    *Katil = 99;
    checkMemory(buffer);
    return 0;
}
/* oluşan çıktı:
[OK] mevcut bellek korunmuş
[CRITICAL] Memory Corruption Detected! Kanarya öldü!
*/