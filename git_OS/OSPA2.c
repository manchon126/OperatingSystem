#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    FILE* fp;
    
    int nOfPs, nOfRTypes;
    int* rUnits;
    int** allocMtr;
    int** reqMtr;

    int* psStat;
    int nOfBlockedPs;
    int old_nOfBPs;
    int* freeRes;


    fp = fopen( "input.txt", "r" );
    
    fscanf(fp, "%d %d", &nOfPs, &nOfRTypes);

    nOfBlockedPs = nOfPs;


    rUnits = (int *)malloc(sizeof(int) * nOfRTypes);
    allocMtr = (int **)malloc(sizeof(int *) * nOfPs);
    reqMtr = (int **)malloc(sizeof(int *) * nOfPs);

    psStat = (int *)malloc(sizeof(int) * nOfPs);
    freeRes = (int *)malloc(sizeof(int) * nOfRTypes);


    for( int i=0; i<nOfRTypes; i++ ){
        fscanf(fp, "%d", rUnits+i);
    }

    for( int i=0; i<nOfPs; i++ ){
        allocMtr[i] = (int *)malloc(sizeof(int) * nOfRTypes);
        for( int j=0; j<nOfRTypes; j++ ){
            fscanf(fp, "%d", &allocMtr[i][j]);
        }
    }

    for( int i=0; i<nOfPs; i++ ){
        reqMtr[i] = (int *)malloc(sizeof(int) * nOfRTypes);
        for( int j=0; j<nOfRTypes; j++ ){
            fscanf(fp, "%d", &reqMtr[i][j]);
        }
    }

    for( int i=0; i<nOfPs; i++ ){
        psStat[i] = 1;
    }


    for( int i=0; i<nOfRTypes; i++ ){
        int sumOfAllocatedR = 0;
        for( int j=0; j<nOfPs; j++ ){
            sumOfAllocatedR += allocMtr[j][i];
        }
        freeRes[i] = rUnits[i] - sumOfAllocatedR;
    }


    old_nOfBPs = nOfBlockedPs+1;

     while( old_nOfBPs != nOfBlockedPs && nOfBlockedPs > 0 ){
        old_nOfBPs = nOfBlockedPs;
        for( int i=0; i<nOfPs; i++ ){
            int isUnblocked = 1;
            if( !psStat[i] ){ //already unblocked;
                continue;
            }
            for( int j=0; j<nOfRTypes; j++ ){
                if( reqMtr[i][j] > freeRes[j] ){
                    isUnblocked = 0;
                    break;
                }
            }
            if( isUnblocked ){
                for( int j=0; j<nOfRTypes; j++ ){
                    freeRes[j] += allocMtr[i][j];
                }
                psStat[i] = 0;
                nOfBlockedPs--;
                if( !nOfBlockedPs )
                    break;
            }
        }
    }

    if( nOfBlockedPs ){
        printf("Deadlock detected, blocked processes:\n");        
        for(int i=0; i<nOfPs; i++ ){
            if( psStat[i] ){
                printf("process %d\t", i);
            }
        }
    }
    else{
        printf("Deadlock undetected\n");
    }

    
    
    for( int i=0; i<nOfPs; i++ ){
        free(allocMtr[i]);
        free(reqMtr[i]);
    }

    free(allocMtr);
    free(reqMtr);

    free(psStat);
    free(freeRes);

    fclose( fp );

    return 0;
}