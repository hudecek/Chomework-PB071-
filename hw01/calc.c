/*
* Program represents a simple calculator. Its functions - addition, subtraction,
* iteger division, real division, modulo, multiplication, N-th power,
* factorial, sum of numbers, average of nubers, binomial coefficient and primality
* test - are called by arguements in front of parameters.
*/

#include <stdio.h>

int add(int a, int b) {
    return a+b;
}

int sub(int a, int b) {
    return a-b;
}

float div(float a, float b) {
    return a/b;
}

int iDiv(int a, int b) {
    return a/b;
}

int mod(int a, int b) {
    return a%b;
}

int mul(int a, int b) {
    return a*b;
}

long long int nthPower(int a, int b) {
    if(b==0) return 1;
    int res=a;
    for(int i=1;i<b;i++) {
        res*=a;
    }
    return res;
}

long long int fact(int f) {
    if(f<0) {
        return -1;
    } else if(f==0||f==1) {
        return 1;
    } else {
        return f*fact(f-1);
    }
}

int sum(int size) {
    int sum=0;
    int b=0;
    for(int i=0;i<size;i++) {
        scanf("%d", &b);
        sum += b;
    }
    return sum;
}

float avg(int size) {
    if(size !=0) {
        return (float)sum(size)/size;
    } else {
        return 0;
    }
}

long long int binom(int a, int b) {
    if(a<b || a<0 || b<0) {
        return -1;
    } else {
        return fact(a)/(fact(a-b)*fact(b));
    }
}

int prime(int a) {
    if(a==1||a==0) return 0;
    for(int i=2;i*i<=a;i++) {
        if(a%i==0)
            return 0;
    }
    return 1;
}

int main(void)
{

    char choice='x';
    int a,b, exit=0;
    float fa,fb;

    do {
        printf("> ");
        fflush(stdin);
        scanf("%1s", &choice);
        switch(choice) {
            case '+':
                scanf("%d %d", &a, &b);
                printf("# %d\n",add(a,b));
                break;

            case '-':
                scanf("%d %d", &a, &b);
                printf("# %d\n",sub(a,b));
                break;

            case '/':
                scanf("%f %f", &fa, &fb);
                printf("# %.2f\n",div(fa, fb));
                break;

            case 'd':
                scanf("%d %d", &a, &b);
                printf("# %d\n",iDiv(a,b));
                break;

            case 'm':
                scanf("%d %d", &a, &b);
                printf("# %d\n",mod(a,b));
                break;

            case '*':
                scanf("%d %d", &a, &b);
                printf("# %d\n",mul(a,b));
                break;

            case '^':
                scanf("%d %d", &a,&b);
                printf("# %lld\n",nthPower(a, b));
                break;

            case '!':
                scanf("%d", &a);
                printf("# %lld\n",fact(a));
                break;

            case 's':
                scanf("%d", &a);
                printf("# %d\n",sum(a));
                break;

            case 'a':
                scanf("%d", &a);
                printf("# %.2f\n",avg(a));
                break;

            case 'c':
                scanf("%d %d", &a, &b);
                printf("# %lld\n",binom(a,b));
                break;

            case 'p':
                scanf("%d", &a);
                if(prime(a)) printf("# y\n");
                    else printf("# n\n");
                break;

            case 'q':
                exit = 1;
                break;

            default:printf("Unknown arguement: %c\n", choice);
        }
    } while(!exit);

    return 0;

}
