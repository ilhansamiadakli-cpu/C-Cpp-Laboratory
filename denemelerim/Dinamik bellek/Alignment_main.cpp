#include <iostream>

struct BadPlayerData {
    bool isAlive;      // 1 byte
    double x_pos;      // 8 bytes
    int score;         // 4 bytes
    bool hasKey;       // 1 byte
    short level;       // 2 bytes
}; // olması gereken 16 ama 24 bayt kaplamış
struct GoodPlayerData{
    double x_pos;
    int score;
    short level;
    bool isAlive;
    bool hasKey;
};


int main(){
    BadPlayerData badData;
    GoodPlayerData goodData;
    std::cout << "kötü structın kapladığı bayt: "<< sizeof(badData)<< std::endl;
    std::cout << "iyi structın kapladığı bayt: "<< sizeof(goodData)<< std::endl;
    return 0;
}
/* oluşan çıktı:
kötü structın kapladığı bayt: 24
iyi structın kapladığı bayt: 16
*/