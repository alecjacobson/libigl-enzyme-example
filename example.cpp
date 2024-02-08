// test.c
#include <cstdio>
extern double __enzyme_autodiff(void*, double);
double square(double x) {
    return x * x;
}
double dsquare(double x) {
    // This returns the derivative of square or 2 * x
    return __enzyme_autodiff((void*) square, x);
}
int main() {
    for(double i=1; i<5; i++)
    {
        std::printf("square(%f)=%f, dsquare(%f)=%f\n", i, square(i), i, dsquare(i));
    }
}
