#include <stdio.h>
#include <math.h>
int main(void) {
    double SINE,X;           
    printf("Enter value of X : must be between 0 and 1 \n");
    scanf("%lf",&X);
    SINE= sin(X);
    printf("\n The sine value of X is %lf=%lf",X,SINE);
    return 0;
}