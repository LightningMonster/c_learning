/*The precision of a floating point value indicates how many digits 
the value can have after the decimal point. The precision of float 
is six or seven decimal digits, while double variables have a precision
of about 15 digits. Therefore, it is often safer to use double for most 
calculations - but note that it takes up twice as much memory as float (8 bytes vs. 4 bytes). */

#include <stdio.h>

int main() {
    float f1 = 35e3;
    double d1 = 12E4;

    printf("f1 = %f\n", f1);
    printf("%lf \n", d1);

    float myFloatNum = 3.5;

    printf("%f\n", myFloatNum);   // Default will show 6 digits after the decimal point
    printf("%.1f\n", myFloatNum); // Only show 1 digit
    printf("%.2f\n", myFloatNum); // Only show 2 digits
    printf("%.4f", myFloatNum);   // Only show 4 digits
}