#include "donusum.h"

double celsiusToFahrenheit(double c){
    return c* (9.0/5.0)+ 32;
}
double fahrenheitToCelsius(double f){
    return (f - 32) * (9.0 / 5.0);
}
double celsiusToKelvin(double c){
    return c + 273.15;
}