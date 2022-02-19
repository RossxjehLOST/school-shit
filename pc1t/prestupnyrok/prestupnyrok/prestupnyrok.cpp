// prestupnyrok.cpp : Defines the entry point for the application.
//

#include "prestupnyrok.h"

using namespace std;

int jePrestupny(int rok)
{
	int modulo4 = rok % 4;
	int modulo100 = rok % 100;
	int modulo400 = rok % 400;

	if((modulo400 == 400) ||(modulo4 == 0) && (modulo100 !=0))
	{
		
		return 1;

	}
	
	return 0;
}


int jeLichy(int rok)
{
	int modulo2 = rok % 2;
	

	if ((modulo2 == 0) )
	{
		
		return 1;

	}

	return 0;
}



int main()
{
	int rok;
	int prestupnost;
	int parnost;
	printf("Zadajte rok:");
	scanf("%d", &rok);


	prestupnost = jePrestupny(rok);
	parnost = jeLichy(rok);

	if (prestupnost == 1)
	{

		printf("Je prestupny");
	}
	else
	{
		printf("Je neprestupny");

	}


	if (parnost == 1)
	{
		printf("Je parny");

	}
	else
	{
		printf("Je neparny");

	}

	 


	

	
	
	return 0;
}
