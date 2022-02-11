#include<stdio.h>
#include<math.h>
int main(void)
{
    float interval,sine,cosine;       //Declaring variables
    int i;
    for(i = 0; i <10; i++)    //Looping
    {
        interval = i/10.0;
        sine=(sin(interval));
        cosine=(cos(interval));
        printf("sine(%f)=%f\t cosine(%f)=%f\n",interval,sine,interval,cosine);     //Printing output
    }

    return 0;
}