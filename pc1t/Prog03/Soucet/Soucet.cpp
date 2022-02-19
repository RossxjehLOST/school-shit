// Soucet.cpp : Defines the entry point for the application.
//

#include "Soucet.h"
#include <complex>


using namespace std;

int main()
{
	double x;
	double y;
	double z;
	double f;
	printf("Zadajte 1. komplexne cislo:");
	scanf_s("%lf  %lfi", &x,&y);
	printf("Zadajte 1. komplexne cislo:");
	scanf_s("%lf + %lfi", &z, &f);

	printf("Sucet je: %.1lf + %.1fi",x+z,y+f);
	return 0;
}
