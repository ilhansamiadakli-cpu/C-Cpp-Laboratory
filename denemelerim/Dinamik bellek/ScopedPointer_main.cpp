#include<iostream>

/*
TODO: akıllı işaretçi


*/
template <typename T>
class Voidptr{
    private:
    T* m_ptr;
    public:
    Voidptr(T* ptr){
        m_ptr = ptr;
        std::cout << "bellek oluşturuldu" << std:: endl;
    }
    ~Voidptr(){
        delete m_ptr;
        std::cout << "bellek başarıyla temizlendi" << std::endl;
    }

    T& operator*() {
        return *m_ptr; 
    }

    T* operator->() {
        return m_ptr;
    }
    Voidptr(const Voidptr&) = delete;
    Voidptr& operator=(const Voidptr&) = delete;
};
int main() {
    // Test:
    {
        Voidptr<int> canPuanı(new int(100)); // Heap'te 100 değeri oluşturup sarmalıyoruz
        std::cout << "Can: " << *canPuanı << std::endl; // operator* çalışacak
        
        *canPuanı = 80; // Değeri değiştiriyoruz
        std::cout << "Yeni Can: " << *canPuanı << std::endl;
        
    } // Kapsam (scope) burada bitiyor. Yıkıcı (Destructor) otomatik çalışmalı!

    return 0;
}