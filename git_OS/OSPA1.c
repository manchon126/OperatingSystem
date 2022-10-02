#include <stdio.h>
#include <stdlib.h>

typedef enum {false, true} boolean;


typedef struct _processInfo{
    int PID;
    int arrivalT;
    int whichQ;
    int remainCycles;
    int* BTlist;
    int comefrom; //0:created 1:preempted 2:wakeup
} processInfo;

typedef processInfo* Data;
typedef struct _Qnode{
	Data ndata;
	struct _Qnode* next;
} Qnode;
typedef struct _queue{
	Qnode* front;
	Qnode* rear;
} Queue;




void initQueue(Queue* pqueue)
{
	pqueue->front=pqueue->rear=NULL;
}


boolean isEmptyQueue(Queue* pqueue)
{
	return (pqueue->front == NULL);
}


void Enqueue(Queue* pqueue, Data item)
{
	Qnode* newQnode = (Qnode *)malloc(sizeof(Qnode));
	newQnode->next = NULL;
	newQnode->ndata = item;

	if( isEmptyQueue(pqueue) ){
		pqueue -> front = pqueue->rear = newQnode;
	}
	else{
		pqueue->rear->next = newQnode;
		pqueue->rear = newQnode;
	}

}

Data Dequeue(Queue* pqueue)
{
   if(isEmptyQueue(pqueue)){
       printf("err");
       exit(1);
   }
	else{
		Qnode* rQnode = pqueue->front;
		Data rData = rQnode->ndata;
		pqueue->front = rQnode->next;
		
		free(rQnode);
		return rData;
	}
}

Data peekQueue(Queue* pqueue)
{
   if(isEmptyQueue(pqueue)){
       printf("err");
       exit(1);
   }
   return pqueue->front->ndata;
}


int main(void)
{
	FILE* fp;
    Queue q0, q1, q2, q3;

    int simulTime = 0;
    int nOfProcesses = 0;
    int remainProcesses = 0;
    int remainCPUtime = 0;

    processInfo* processListOrigin = NULL;
    processInfo* processList = NULL;
    processInfo* processing = NULL;

    
    int printFlag;
    int* TTlist;
    int* WTlist;
    int TTavrg=0, WTavrg=0;


    initQueue( &q0 ); initQueue( &q1 ); initQueue( &q2 ); initQueue( &q3 );

    
	fp = fopen( "input.txt", "r" );

    fscanf(fp, "%d", &nOfProcesses);

    remainProcesses = nOfProcesses;

    processListOrigin = (processInfo *)malloc(sizeof(processInfo) * nOfProcesses );
    processList = (processInfo *)malloc(sizeof(processInfo) * nOfProcesses );

    TTlist = (int *)malloc(sizeof(int) * nOfProcesses );
    WTlist = (int *)malloc(sizeof(int) * nOfProcesses );

    for(int i=0; i<nOfProcesses; i++){
        fscanf(fp, "%d %d %d %d", &(processListOrigin[i].PID), &(processListOrigin[i].arrivalT), &(processListOrigin[i].whichQ), &(processListOrigin[i].remainCycles) );
        processList[i].PID = processListOrigin[i].PID; processList[i].arrivalT = processListOrigin[i].arrivalT; processList[i].whichQ = processListOrigin[i].whichQ; processList[i].remainCycles = processListOrigin[i].remainCycles; processList[i].comefrom = processListOrigin[i].comefrom = 0;
        processListOrigin[i].BTlist = (int *)malloc(sizeof(int) * (2*processListOrigin[i].remainCycles + 1) );
        processList[i].BTlist = (int *)malloc(sizeof(int) * (2*processList[i].remainCycles + 1) );
        
        for( int j=0; j<2*(processListOrigin[i].remainCycles)-1; j++ ){
            fscanf(fp, "%d", &(processListOrigin[i].BTlist[j]) );
            processList[i].BTlist[j] = processListOrigin[i].BTlist[j];
        }
    }

    for( int i=0; i<nOfProcesses; i++ ){
        WTlist[i] = 0;
        for( int j=0; j<processListOrigin[i].remainCycles; j++ ){
                WTlist[i] += processListOrigin[i].BTlist[2*j];           
        }
    }


    while( remainProcesses ){
        printFlag = false;
        if( remainCPUtime ){
            remainCPUtime--;
            (processing->BTlist[0])--;
            if( processing->BTlist[0] == 0){ // terminated or asleep
                processing->remainCycles--;
                
                printFlag = true;
                printf("time %d: \t", simulTime);

                if( processing->remainCycles == 0){
                    free(processing->BTlist);
                    remainProcesses--;
                    printf("process #%d terminated", processing->PID);
                    TTlist[processing->PID-1] = simulTime - processListOrigin[processing->PID-1].arrivalT;
                    WTlist[processing->PID-1] = TTlist[processing->PID-1] - WTlist[processing->PID-1];
                }
                else{
                    int IOBT = processing->BTlist[1];
                    int* tBTlist = (int *)malloc(sizeof(int) * (2*(processing->remainCycles)-1) );
                    for( int i=0; i<2*(processing->remainCycles)-1; i++ ){
                        tBTlist[i] = processing->BTlist[i+2];
                    }
                    free( processing->BTlist );
                    processing->BTlist = tBTlist;
                    switch( processing->whichQ ){
                        case 0:
                            processing->arrivalT = simulTime+IOBT;
                            processing->whichQ = 0;
                            processing->comefrom = 2;
                            break;
                        case 1:
                            processing->arrivalT = simulTime+IOBT;
                            processing->whichQ = 0;
                            processing->comefrom = 2;
                            break;
                        case 2:
                            processing->arrivalT = simulTime+IOBT;
                            processing->whichQ = 1;
                            processing->comefrom = 2;
                            break;
                        case 3:
                            processing->arrivalT = simulTime+IOBT;
                            processing->whichQ = 3;
                            processing->comefrom = 2;
                            break;
                    }

                    printf("process #%d sleep(I/O)\t", processing->PID);
                }
            }
            else if( remainCPUtime == 0 ){ //timerrunout
                if( !printFlag ){
                    printFlag = true;
                    printf("time %d: \t", simulTime);
                }
                printf("process #%d timerrunout\t", processing->PID);
                switch(processing->whichQ){
                    case 0:
                        processing->arrivalT = simulTime;
                        processing->whichQ = 1;
                        processing->comefrom = 1;
                        break;
                    case 1:
                        processing->arrivalT = simulTime;
                        processing->whichQ = 2;
                        processing->comefrom = 1;
                        break;
                    case 2:
                        processing->arrivalT = simulTime;
                        processing->whichQ = 3;
                        processing->comefrom = 1;
                        break;
                }
            }
        }

        for(int i=0; i<nOfProcesses; i++){
            if( processList[i].arrivalT == simulTime ){
                if( !printFlag ){
                    printFlag = true;
                    printf("time %d: \t", simulTime);
                }
                printf("enqueue process #%d into RQ #%d", processList[i].PID, processList[i].whichQ);
                switch(processList[i].comefrom ){
                    case 0:
                        printf("(created)");
                        break;
                    case 1:
                        printf("(preempted)");
                        break;
                    case 2:
                        printf("(wakeup)");
                        break;
                }
                printf("\t");
                switch( processList[i].whichQ ){
                    case 0:
                        Enqueue( &q0, &(processList[i]) );
                        break;
                    case 1:
                        Enqueue( &q1, &(processList[i]) );
                        break;
                    case 2:
                        Enqueue( &q2, &(processList[i]) );
                        break;
                    case 3:
                        Enqueue( &q3, &(processList[i]) );
                        break;
                }
            }
        }

        if( remainCPUtime == 0 ){
            if( !isEmptyQueue(&q0) ){
                processing = Dequeue(&q0);

                if( !printFlag ){
                    printFlag = true;
                    printf("time %d: \t", simulTime);
                }
                printf("dispatch&run process #%d from RQ #0", processing->PID );
                if( processing->BTlist[0] < 2 ){
                    remainCPUtime = processing->BTlist[0];
                }
                else{
                    remainCPUtime = 2;
                }
            }
            else if( !isEmptyQueue(&q1) ){
                processing = Dequeue(&q1);

                if( !printFlag ){
                    printFlag = true;
                    printf("time %d: \t", simulTime);
                }
                printf("dispatch&run process #%d from RQ #1", processing->PID );
                if( processing->BTlist[0] < 4 ){
                    remainCPUtime = processing->BTlist[0];
                }
                else{
                    remainCPUtime = 4;
                }

            }
            else if( !isEmptyQueue(&q2) ){
                processing = Dequeue(&q2);

                if( !printFlag ){
                    printFlag = true;
                    printf("time %d: \t", simulTime);
                }
                printf("dispatch&run process #%d from RQ #2", processing->PID );
                if( processing->BTlist[0] < 8 ){ 
                    remainCPUtime = processing->BTlist[0];
                }
                else{
                    remainCPUtime = 8;
                }
            }
            else if( !isEmptyQueue(&q3) ){
                processing = Dequeue(&q3);

                if( !printFlag ){
                    printFlag = true;
                    printf("time %d: \t", simulTime);
                }
                printf("dispatch&run process #%d from RQ #3", processing->PID );
                remainCPUtime = processing->BTlist[0];
            }
        }



        simulTime++;
        if( printFlag )
            printf("\n");
    }
    printf("\n");

    for( int i=0; i<nOfProcesses; i++ ){
        printf("Turnaround Time, Waiting Time of process #%d : %d, %d\n", processListOrigin[i].PID, TTlist[i], WTlist[i] );
    }
    printf("\n");

    for( int i=0; i<nOfProcesses; i++ ){
        TTavrg += TTlist[i]; WTavrg += WTlist[i];
    }
    TTavrg /= nOfProcesses; WTavrg /= nOfProcesses;
    printf("Average of Turnaround Time, Waiting Time: %d, %d\n", TTavrg, WTavrg);

    
    free( TTlist );
    free( WTlist );

    free( processListOrigin );
    free( processList );

	fclose(fp);


	return 0;
}