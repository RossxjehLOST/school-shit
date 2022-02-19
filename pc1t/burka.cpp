// Soucet.cpp : Defines the entry point for the application.
//

#include "Soucet.h"

using namespace std;

int main()
{
	double x;
	double y;
	printf("Zadajte 1. komplexne cislo: ");
	scanf_s("%lf + %lf ", &x,&y);
	
	printf("Zadajte 2. komplexne cislo: ");

	printf("Burka je daleko %lf metrov", x * y);
	return 0;
}
