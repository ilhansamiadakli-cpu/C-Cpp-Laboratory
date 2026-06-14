#include <iostream>
using namespace std;

int main() {
    int a = 5; // 0101
    int b = 3; // 0011

    cout << "a & b: " << (a & b) << endl; // 0001 -> 1
    cout << "a | b: " << (a | b) << endl; // 0111 -> 7
    cout << "a ^ b: " << (a ^ b) << endl; // 0110 -> 6
    cout << "~a: " << (~a) << endl;        // 1010 -> -6 (2's complement)
    cout << "b << 1: " << (b << 1) << endl; // 0110 -> 6
    cout << "b >> 1: " << (b >> 1) << endl; // 0001 -> 1

    return 0;
}