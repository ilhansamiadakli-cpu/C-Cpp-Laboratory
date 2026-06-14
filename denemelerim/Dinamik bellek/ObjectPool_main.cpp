#include <iostream>

struct Bullet{
    float x,y;
    bool isActive = false;
};
class BulletPool{
    private:

        Bullet my_pool[10];
    public:

        Bullet* acquire(){
            for (int i=0;i<10;i++) {
                if(my_pool[i].isActive == false){
                    my_pool[i].isActive = true;
                    return my_pool + i; // isteğe bağlı olarak &my_pool[i] de yazılabilir, ikisi de aynı sonucu verir.
                }
            }
            return  nullptr;
        }
        void release(Bullet* bullet){
            bullet->isActive = false;
        }

};


int main(){
    BulletPool pool;
    Bullet* b1 = pool.acquire();
    std::cout << " mermi adresi: " << b1 << std::endl;
    if (b1) {
        b1->x = 10.0f;
        b1->y = 20.0f;
        std::cout << "mermi konumları (" << b1->x << ", " << b1->y << ")\n";
    } else {
        std::cout << "mermi oluşmadı\n";
    }

    pool.release(b1);
    std::cout << "mermi resetlendi.\n";
    Bullet* b2 = pool.acquire();
    std ::cout << "2. oluşturulan mermi adresi: " << b2 <<std::endl;
return 0;
}
/* oluşan çıktı:
mermi adresi: 0x16f48e964
mermi konumları (10, 20)
mermi resetlendi.
2. oluşturulan mermi adresi: 0x16f48e964
*/