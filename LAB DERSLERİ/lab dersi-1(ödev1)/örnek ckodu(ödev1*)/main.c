#include<stdio.h>
int main(){
    printf("lütfen 2 adet sayının değerini girin\n");
   int a,b;
    scanf("%d %d",&a,&b);
    printf("girdiğiniz sayıların toplamı: %d\n",a+b);
    printf("girdiğiniz sayıların farkı: %d\n",a-b);
    printf("girdiğiniz sayıların çarpımı: %d\n",a*b);
    printf("girdiğiniz sayıların bölümünün tam kısmı: %d\n",a/b);
    if(b!=0){
        printf("girdiğiniz sayıların bölümü: %f\n",(float)a/b);
    }
    printf("girdiğiniz sayıların mod alma işlemi: %d\n",a%b);
    printf("girdiğiniz sayıların ondalık bölümü: %f\n",(double)a/b);
    (a>b) ? printf("a sayısı b sayısından büyüktür\n") : printf("b sayısı a sayısından büyüktür\n");
    (a>0 && b>0) ? printf("girdiğiniz sayıların ikisi de pozitiftir\n") : printf("girdiğiniz sayıların en az biri negatiftir\n");
    return 0;
}