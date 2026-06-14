#include <_time.h>
#include <iostream>
#include <ctime>
using namespace std;
int main() {
    clock_t başlangıç= clock();
    int k=0;
    for (int i=0; i<1000000000; i++) {
        k +=i;
    }
    clock_t duration= clock() - başlangıç;
    cout <<"Geçen zaman: "<< (float)duration/CLOCKS_PER_SEC<<" saniye" <<endl;
    return 0;
}