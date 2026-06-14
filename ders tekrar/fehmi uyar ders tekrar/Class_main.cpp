#include <iostream>
#include <string>
using namespace std;
class Cars{
    private:
     // -------attributes--------
        bool isDriving; 
        int year;
        string name;
        string model;
        float sumKm = 150000.78;
    public:
    // -------metotds--------
        void getIsDriving(){ 
            if(isDriving){
                cout << "Araba şu an sürüş modunda"<< endl;
            }
            else{
                cout << "Araba şu an park halinde"<< endl;
            }
        }
        void Drive(float km){
            isDriving = true;
            sumKm += km;
        }
        void parking(){
            isDriving = false;
        }
        void getModel(){
            cout <<"Araba ismi: " <<name << endl;
            cout << "Model: " << model << endl;
            cout << "Yıl: " << year << endl;
        }
        void getKm(){
            cout << "Toplam kilometre: " << sumKm << endl;
        }
        // -------constructor--------
        Cars(string n, string m, int y){
            name = n;
            model = m;
            year = y;
        }
        // -------destructor--------
        ~Cars(){
            cout << "Araba bilgileri silindi." << endl;
        }

};
int main(){
    cout << "---------İlk araba bilgileri------" << endl;
    Cars car1("BMW", "X5", 2020);
    car1.getModel();
    car1.getKm();
    car1.Drive(500);
    car1.getIsDriving();
    car1.getKm();
    car1.parking();
    car1.getIsDriving();
    cout << endl;
    cout << "-----İkinci araba bilgileri------" << endl;
    Cars car2("Mercedes", "C200", 2018);
    car2.getModel();
    car2.getKm();
    car2.Drive(300);
    car2.getIsDriving();
    car2.getKm();


    return 0;
}