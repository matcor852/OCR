#include <stdio.h>
#include <stdlib.h>


int solver(int array[9][9]) {

    //global variables
    size_t isOnRow[9][9];
    size_t isOnCol[9][9];
    size_t isOnBloc[9][9];

    size_t x[81];
    size_t y[81];
    int v[81];

    size_t nbEmptyCell = 0;

    //initialization boolean arrays and build empty cells
    int k;
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            for (int k=1; k < 10; k++)
            isOnRow[i][k-1] = isOnCol[j][k-1] = isOnBloc[3*(i/3)+(j/3)][k-1] = 0;

    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            if ( (k = array[i][j]) != 0)
                isOnRow[i][k-1] = isOnCol[j][k-1] = isOnBloc[3*(i/3)+(j/3)][k-1] = 1;
            else {
                x[nbEmptyCell] = i;
                y[nbEmptyCell] = j;
                v[nbEmptyCell] = -1;
                nbEmptyCell ++;
            }
                
    size_t indexCell = 0;
    size_t vCurrent = 0;
    size_t nCount = 0;

    while (indexCell != nbEmptyCell) {
        nCount++;

        size_t vx = x[indexCell];
        size_t vy = y[indexCell];
        int vv = v[indexCell];

        if(vv != -1)
            isOnRow[vx][vv] = isOnCol[vy][vv] = isOnBloc[3*(vx/3)+(vy/3)][vv] = 0;

        if (vCurrent > 8) {
            if (indexCell == 0)
                    return 0;
            indexCell--;
            vCurrent = v[indexCell]+1;
        }    
        else {

            vv = vCurrent;
            vCurrent++;
            if (!isOnRow[vx][vv] && !isOnCol[vy][vv] && !isOnBloc[3*(vx/3)+(vy/3)][vv]) {
                isOnRow[vx][vv] = isOnCol[vy][vv] = isOnBloc[3*(vx/3)+(vy/3)][vv] = 1;
                v[indexCell] = vv;
                indexCell++;
                v[indexCell] = -1;
                vCurrent = 0;
            }
        }
    }

    for(size_t i = 0; i < nbEmptyCell; i++)
        array[x[i]][y[i]] = v[i]+1;

    return nCount;
}

int main(void) {
	int array[9][9] =
    {
        {9,0,0,1,0,0,0,0,5},
        {0,0,5,0,9,0,2,0,1},
        {8,0,0,0,4,0,0,0,0},
        {0,0,0,0,8,0,0,0,0},
        {0,0,0,7,0,0,0,0,0},
        {0,0,0,0,2,6,0,0,9},
        {2,0,0,3,0,0,0,0,6},
        {0,0,0,2,0,0,9,0,0},
        {0,0,1,9,0,4,5,7,0}
    };
    int nCount = solver(array);
    if (nCount) {
        printf("Solved in %d moves\n",nCount);
        for(size_t i = 0; i < 9; i++) {
            for(size_t j = 0; j < 9; j++)
                printf("%d ", array[i][j]);
            printf("\n");
        }
    }
    else printf("Impossible to solve");
	return 0;
} 