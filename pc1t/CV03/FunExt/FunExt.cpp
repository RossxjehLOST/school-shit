// FunExt.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <float.h>
#include <limits.h>


int _tmain(int argc, _TCHAR* argv[])
{
    double minY = DBL_MAX;
    int minX = INT_MAX;
    for(int x = 1; x <= 10; ++x)
    {
        double y = 5-3*x+2*(x-5)*(x-5)-(x-10)*(x-10)*(x-10);
        
        if (y<minY)
        {
            minY = y;
            minX = x;
        }        
    }
    printf("Minimum value: %lf at x=%d\n", minY, minX);
    getchar();
    return 0;
}

