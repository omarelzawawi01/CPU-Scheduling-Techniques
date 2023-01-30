#include <iostream>
#include <queue>
#include <stack>
#include <cstdlib>
#include<algorithm>
#include <bits/stdc++.h>
#include <math.h>
#include <string>
//#include <boost/lexical_cast.hpp>
using namespace std;

typedef struct
{
    char name;
    int arrival_time;
    int service_time;
    int remaining_time;
    int turnaround_time;
    int waiting_time;
    int enqueued;
    int finish_time;
    int rq;
    int initial_priority;
    int priority;
    float nor_turnaround_time;
    char* timeline;
} process;


process* process_init(char name, int arrival_time, int service_time, int totaltime, int priority)
{
    process* p = (process*)malloc(sizeof(process));
    p->name=name;
    p->arrival_time=arrival_time;
    p->service_time=service_time;
    p->remaining_time=service_time;
    p->turnaround_time=0;
    p->waiting_time=0;
    p->finish_time=0;
    p->rq=0;
    p->enqueued=0;
    p->initial_priority=priority;
    p->priority=priority;
    p->timeline=(char*)malloc(sizeof(char)*totaltime);
    for(int i=0; i< totaltime; i++)
    {
        (p->timeline)[i]=' ';
    }
    return p;
}

void FCFS(process* plist[],int number,int totaltime)
{
    queue<process*> q;
    process* current_process;
    process* current_p;
    char runningp='0';
//    printf("number of processes: %d \n",number);
    for(int i=0; i<totaltime; i++)
    {
        for(int j=0; j<number; j++)
        {
            if(plist[j]->arrival_time==i)
            {
                q.push(plist[j]);
            }
        }
        if (runningp == '0')
        {
            if (q.size()==0)
            {
                continue;
            }
            else
            {
                current_process=(process*)q.front();
                q.pop();
                runningp=current_process->name;
            }
            //get all processes in queue, increment waiting time, continue
        }
//        else
//        {
            for(int k=0; k < q.size();k++)
            {
                current_p=(process*)q.front();
                q.pop();
                current_p->waiting_time++;
                (current_p->timeline)[i]='.';
                q.push(current_p);
            }
//        }
        current_process->remaining_time--;
        (current_process->timeline)[i]='*';
        if (current_process->remaining_time == 0)
        {
            current_process->finish_time=i+1;
            current_process->turnaround_time=current_process->finish_time - current_process->arrival_time;
            current_process->nor_turnaround_time=current_process->turnaround_time*1.0/current_process->service_time;
            runningp='0';
        }
    }

}

void RR(process* plist[],int number, int quantum,int totaltime)
{
    queue<process*> q;
    process* current_process;
    process* current_p;
    char runningp='0';
    int current_quantum=quantum;
//    printf("number of processes: %d \n",number);
    for(int i=0; i<totaltime; i++)
    {
        for(int j=0; j<number; j++)
        {
            if(plist[j]->arrival_time==i && !plist[j]->enqueued)
            {
                q.push(plist[j]);
                plist[j]->enqueued=1;
            }
        }
        if (runningp == '0')
        {
            if (q.size()==0)
            {
                continue;
            }
            else
            {
                current_process=(process*)q.front();
                q.pop();
                runningp=current_process->name;
                for(int k=0; k < q.size();k++)
                {
                    current_p=(process*)q.front();
                    q.pop();
                    current_p->waiting_time++;
                    (current_p->timeline)[i]='.';
                    q.push(current_p);
                }
            }
            //get all processes in queue, increment waiting time, continue
        }
        else
        {
            for(int k=0; k < q.size();k++)
            {
                current_p=(process*)q.front();
                q.pop();
                current_p->waiting_time++;
                (current_p->timeline)[i]='.';
                q.push(current_p);
            }
        }
        current_process->remaining_time--;
        (current_process->timeline)[i]='*';
        current_quantum--;
        if (!current_process->remaining_time || !current_quantum)
        {
            if(!current_process->remaining_time)
            {
                current_process->finish_time=i+1;
                current_process->turnaround_time=current_process->finish_time - current_process->arrival_time;
                current_process->nor_turnaround_time=current_process->turnaround_time*1.0/current_process->service_time;
            }
            else
            {
                for(int j=0; j<number; j++)
                {
                    if(plist[j]->arrival_time==i+1 && !plist[j]->enqueued)
                    {
                        q.push(plist[j]);
                        plist[j]->enqueued=1;
                    }
                }
                q.push(current_process);
            }
            current_quantum=quantum;
            runningp='0';
        }
    }

}

void Aging(process* plist[],int number, int quantum,int totaltime)
{
    queue<process*> q;
    process* current_process=0;
    process* current_p;
    char runningp='0';
    int current_quantum=quantum;
//    printf("number of processes: %d \n",number);
    for(int i=0; i<totaltime; i++)
    {
        for(int j=0; j<number; j++)
        {
            if(plist[j]->arrival_time==i && !plist[j]->enqueued)
            {
                q.push(plist[j]);
                plist[j]->enqueued=1;
            }
        }
        if (runningp == '0')
        {
            if (q.size()==0 && current_process==0)
            {
                continue;
            }
            else
            {
                int qsize=q.size();
                int highestpr=-100;
                int pr=0;
                for(int k=0; k < q.size();k++)
                {
                    current_p=(process*)q.front();
                    q.pop();
                    current_p->priority++;
                    q.push(current_p);
                }

                if (current_process != 0)
                {
                    q.push(current_process);
                    qsize++;
                }
                process* maxprocess=q.front();
                for (int a=0;a<qsize;a++)
                {
                    pr=q.front()->priority;
                    current_p=q.front();
                    if (pr > highestpr)
                    {
                        highestpr=pr;
                        maxprocess=current_p;
                    }
                    q.pop();
                    q.push(current_p);
                }

                for (int a=0;a<qsize;a++)
                {

                    if (q.front() == maxprocess)
                    {
                        current_process=q.front();
                        q.pop();
                        runningp='1';
                    }
                    else
                    {
                        current_p=q.front();
                        q.pop();
                        (current_p->timeline)[i]='.';
                        current_p->waiting_time++;
                        q.push(current_p);
                    }
                }

                runningp=current_process->name;
            }
            //get all processes in queue, increment waiting time, continue
        }
        else
        {
            for(int k=0; k < q.size();k++)
            {
                current_p=(process*)q.front();
                q.pop();
                current_p->waiting_time++;
                (current_p->timeline)[i]='.';
                current_p->priority++;
                q.push(current_p);
            }
        }
        (current_process->timeline)[i]='*';
        current_quantum--;
        current_process->priority=current_process->initial_priority;
        if (!current_quantum)
        {
//            for(int j=0; j<number; j++)
//            {
//                if(plist[j]->arrival_time==i+1 && !plist[j]->enqueued)
//                {
//                    q.push(plist[j]);
//                    plist[j]->enqueued=1;
//                }
//            }
            current_quantum=quantum;
            runningp='0';
        }
    }
}



void SPN(process* plist[],int number,int totaltime)
{
    queue<process*> q;
    process* current_process;
    process* current_p;
    char runningp='0';
//    printf("number of processes: %d \n",number);
    for(int i=0; i<totaltime; i++)
    {
        for(int j=0; j<number; j++)
        {
            if(plist[j]->arrival_time==i)
            {
                q.push(plist[j]);
            }
        }
        if (runningp == '0')
        {
            if (q.size()==0)
            {
                continue;
            }
            else
            {
                int qsize=q.size();
                int minrt=999999.9999;
                int rt=0;
                process* minprocess=q.front();
                for (int a=0;a<qsize;a++)
                {
                    rt=q.front()->service_time;
                    current_p=q.front();
                    if (rt < minrt)
                    {
                        minrt=rt;
                        minprocess=current_p;
                    }
                    q.pop();
                    q.push(current_p);
                }
                for (int a=0;a<qsize;a++)
                {
                    if (q.front() == minprocess)
                    {
                        current_process=q.front();
                        q.pop();
                        runningp='1';
                    }
                    else
                    {
                        current_p=q.front();
                        q.pop();
                        (current_p->timeline)[i]='.';
                        current_p->waiting_time++;
                        q.push(current_p);
                    }
                }
                //get all processes in queue, increment waiting time, continue
            }
        }
        else
        {
            for(int k=0; k < q.size();k++)
            {

                current_p=q.front();
                q.pop();
                current_p->waiting_time++;
                (current_p->timeline)[i]='.';
                q.push(current_p);
            }
        }
        current_process->remaining_time--;
        (current_process->timeline)[i]='*';
        if (current_process->remaining_time == 0)
        {
            current_process->finish_time=i+1;
            current_process->turnaround_time=current_process->finish_time - current_process->arrival_time;
            current_process->nor_turnaround_time=current_process->turnaround_time*1.0/current_process->service_time;
            runningp='0';
        }
    }

}

void SRT_(process* plist[],int number,int totaltime)
{
    queue< process* > q;
    process* current_process;
    process* current_p;
    char runningp='0';
//    printf("number of processes: %d \n",number);
    for(int i=0; i<totaltime; i++)
    {
        for(int j=0; j<number; j++)
        {
            if(plist[j]->arrival_time==i && !plist[j]->enqueued)
            {
                q.push(plist[j]);
                plist[j]->enqueued=1;
            }
        }
        if (q.size()==0)
        {
            continue;
        }
        else
        {
            int qsize=q.size();
            int minrt=999999.9999;
            int rt=0;
            process* minprocess=q.front();
            for (int a=0;a<qsize;a++)
            {
                rt=q.front()->remaining_time;
                current_p=q.front();
                if (rt < minrt)
                {
                    minrt=rt;
                    minprocess=current_p;
                }
                q.pop();
                q.push(current_p);
            }
            for (int a=0;a<qsize;a++)
            {
                if (q.front() == minprocess)
                {
                    current_process=q.front();
                    q.pop();
                }
                else
                {
                    current_p=q.front();
                    q.pop();
                    (current_p->timeline)[i]='.';
                    current_p->waiting_time++;
                    q.push(current_p);
                }
            }
//            runningp=(current_process.second)->name;
//            printf("i= %d, running process:%c \n",i,runningp);
        }
        current_process->remaining_time--;
        (current_process->timeline)[i]='*';
        if (current_process->remaining_time== 0)
        {
            current_process->finish_time=i+1;
            current_process->turnaround_time=current_process->finish_time - current_process->arrival_time;
            current_process->nor_turnaround_time=current_process->turnaround_time*1.0/current_process->service_time;
        }
        else
        {
            for(int j=0; j<number; j++)
            {
                if(plist[j]->arrival_time==i+1 && !plist[j]->enqueued)
                {
                    q.push(plist[j]);
                    plist[j]->enqueued=1;
                }
            }
//            printf("Remaining time: %d\n",current_process.first);
            q.push(current_process);
        }
    }
}



void HRRN(process* plist[],int number,int totaltime)
{
    queue< process*> q;
    process* current_process;
    queue< process*> temp;
    process* current_p;
    char runningp='0';
    float respr;
//    printf("number of processes: %d \n",number);
    for(int i=0; i<totaltime; i++)
    {
        for(int j=0; j<number; j++)
        {
            if(plist[j]->arrival_time==i)
            {
                q.push(plist[j]);
            }
        }
        if (runningp == '0')
        {
            if (q.size()==0)
            {
                continue;
            }
            else
            {
                int qsize=q.size();
                float maxrespr=-999999.9999;
                process* maxprocess=q.front();
                for (int a=0;a<qsize;a++)
                {
                    respr=((q.front()->service_time)*1.0+q.front()->waiting_time)/q.front()->service_time;
                    current_p=q.front();
                    if (respr > maxrespr)
                    {
                        maxrespr=respr;
                        maxprocess=current_p;
                    }
                    q.pop();
                    q.push(current_p);
                }
                for (int a=0;a<qsize;a++)
                {
                    if (q.front() == maxprocess)
                    {
                        current_process=q.front();
                        q.pop();
                    }
                    else
                    {
                        current_p=q.front();
                        q.pop();
                        (current_p->timeline)[i]='.';
                        current_p->waiting_time++;
                        q.push(current_p);
                    }
                }
                runningp=current_process->name;
//                printf("i= %d, running process:%c \n",i,runningp);
            }
            //get all processes in queue, increment waiting time, continue
        }
        else
        {
//            printf("i= %d, priority queue size:%d, running process: %c\n",i,q.size(),runningp);
            int qsize=q.size();
            for(int k=0; k < qsize;k++) //A loop to increment the waiting time of processes and copy them into a queue
            {
                current_p=q.front();
                q.pop();
                current_p->waiting_time++;
                (current_p->timeline)[i]='.';
//                printf("Process in priority queue: %c,process waiting time: %d\n",(current_p.second)->name,(current_p.second)->waiting_time);
                q.push(current_p);
            }
//            int qsize=temp.size();
//            for(int m=0; m < qsize;m++)   //A loop to copy the processes back into the priority queue
//            {
//                current_p=temp.front();
//                temp.pop();
//                current_p.first=(((current_p.second)->service_time)*1.0+(current_p.second)->waiting_time)/(current_p.second)->service_time;
//                q.push(current_p);
//            }
        }
        current_process->remaining_time--;
        (current_process->timeline)[i]='*';
        if (current_process->remaining_time == 0)
        {
            current_process->finish_time=i+1;
            current_process->turnaround_time=current_process->finish_time - current_process->arrival_time;
            current_process->nor_turnaround_time=current_process->turnaround_time*1.0/current_process->service_time;
            runningp='0';
        }
    }
}


bool My_func( process* a, process* b)
{
	return a->service_time > b->service_time;
}
void FB_1_(process* plist[],int number,int totaltime)
{
    queue< process* > q;
    process* current_process;
    process* current_p;
    char runningp='0';
    int quantum=1;
//    printf("number of processes: %d \n",number);
    for(int i=0; i<totaltime; i++)
    {
        for(int j=0; j<number; j++)
        {
            if(plist[j]->arrival_time==i && !plist[j]->enqueued)
            {
                q.push(plist[j]);
                plist[j]->enqueued=1;
//                printf("process %c dakhalet el queue at %d\n",rq0.front()->name,i);
            }
        }

        if (runningp=='0')
        {
            if (!q.size())
            {
                continue;
            }
            else
            {
                int qsize=q.size();
                int minrq=100;
                int rq=0;
                process* minprocess=0;

                for (int a=0;a<qsize;a++)
                {
                    rq=q.front()->rq;
                    current_p=q.front();
                    if (rq < minrq)
                    {
                        minrq=rq;
                        minprocess=current_p;
                    }
                    q.pop();
                    q.push(current_p);
                }
                for (int a=0;a<qsize;a++)
                {
                    if (q.front() == minprocess)
                    {
                        current_process=q.front();
                        q.pop();
                    }
                    else
                    {
                        current_p=q.front();
                        q.pop();
                        (current_p->timeline)[i]='.';
                        current_p->waiting_time++;
                        q.push(current_p);
                    }
                }
                runningp=current_process->name;
            }
        }
        else
        {
            int qsize=q.size();
            for (int a=0;a<qsize;a++)
            {
                current_p=q.front();
                q.pop();
                (current_p->timeline)[i]='.';
                current_p->waiting_time++;
                q.push(current_p);
            }
        }
        current_process->remaining_time--;
        (current_process->timeline)[i]='*';
        quantum--;
        if (current_process->remaining_time == 0 || quantum ==0)
        {
            if (current_process->remaining_time == 0)
            {
                current_process->finish_time=i+1;
                current_process->turnaround_time=current_process->finish_time - current_process->arrival_time;
                current_process->nor_turnaround_time=current_process->turnaround_time*1.0/current_process->service_time;
                runningp='0';
                quantum=1;
            }
//            printf("process %c khalaset",current_process->name);
            else
            {
                current_process->rq++;
                quantum=1;
                for(int j=0; j<number; j++)
                {
                    if(plist[j]->arrival_time==i+1 && !plist[j]->enqueued)
                    {
                        q.push(plist[j]);
                        plist[j]->enqueued=1;
        //                printf("process %c dakhalet el queue at %d\n",rq0.front()->name,i);
                    }
                }
                if (q.size())
                {
                    q.push(current_process);
                    runningp='0';
                }
                else
                {
                    current_process->rq--;
                }
            }
        }
    }
}
void FB_2i_(process* plist[],int number,int totaltime)
{
    queue< process* > q;
    process* current_process;
    process* current_p;
    char runningp='0';
    int quantum=1;
//    printf("number of processes: %d \n",number);
    for(int i=0; i<totaltime; i++)
    {
        for(int j=0; j<number; j++)
        {
            if(plist[j]->arrival_time==i && !plist[j]->enqueued)
            {
                q.push(plist[j]);
                plist[j]->enqueued=1;
//                printf("process %c dakhalet el queue at %d\n",rq0.front()->name,i);
            }
        }

        if (runningp=='0')
        {
            if (!q.size())
            {
                continue;
            }
            else
            {
                int qsize=q.size();
                int minrq=100;
                int rq=0;
                process* minprocess=0;

                for (int a=0;a<qsize;a++)
                {
                    rq=q.front()->rq;
                    current_p=q.front();
                    if (rq < minrq)
                    {
                        minrq=rq;
                        minprocess=current_p;
                    }
                    q.pop();
                    q.push(current_p);
                }
                for (int a=0;a<qsize;a++)
                {
                    if (q.front() == minprocess)
                    {
                        current_process=q.front();
                        q.pop();
                    }
                    else
                    {
                        current_p=q.front();
                        q.pop();
                        (current_p->timeline)[i]='.';
                        current_p->waiting_time++;
                        q.push(current_p);
                    }
                }
                runningp=current_process->name;
                quantum=pow(2,current_process->rq);
            }
        }
        else
        {
            int qsize=q.size();
            for (int a=0;a<qsize;a++)
            {
                current_p=q.front();
                q.pop();
                (current_p->timeline)[i]='.';
                current_p->waiting_time++;
                q.push(current_p);
            }
        }
        current_process->remaining_time--;
        (current_process->timeline)[i]='*';
        quantum--;
        if (current_process->remaining_time == 0 || quantum ==0)
        {
            if (current_process->remaining_time == 0)
            {
                current_process->finish_time=i+1;
                current_process->turnaround_time=current_process->finish_time - current_process->arrival_time;
                current_process->nor_turnaround_time=current_process->turnaround_time*1.0/current_process->service_time;
                runningp='0';
                quantum=1;
            }
//            printf("process %c khalaset",current_process->name);
            else
            {
                current_process->rq++;
                quantum=1;
                for(int j=0; j<number; j++)
                {
                    if(plist[j]->arrival_time==i+1 && !plist[j]->enqueued)
                    {
                        q.push(plist[j]);
                        plist[j]->enqueued=1;
        //                printf("process %c dakhalet el queue at %d\n",rq0.front()->name,i);
                    }
                }
                if (q.size())
                {
                    q.push(current_process);
                    runningp='0';
                }
                else
                {
                    current_process->rq--;
                }
            }
        }
    }
}

void print_process(process* p,int totaltime)
{
    printf("Process Name: %c\n",p->name);
    printf("Process Arrival time: %d\n",p->arrival_time);
    printf("Process Waiting time: %d\n",p->waiting_time);
    printf("Process Service time: %d\n",p->service_time);
    printf("Process Turnaround time: %d\n",p->turnaround_time);
    printf("Process Finish time: %d\n",p->finish_time);
    printf("Process Remaining time: %d\n",p->remaining_time);
    printf("Process Priority: %d\n",p->priority);
    for(int i=0; i<totaltime; i++)
    {
        printf("%c |",(p->timeline)[i]);
    }
    printf("\n");
    printf("--------------------------------------------------\n");

}

void centrePrint(int n, int width)
{
    char s[20] = {'\0'};
    int len;
    sprintf(s, "%d", n);
    len = strlen(s);
    if (len >= width)
        printf(s);
    else
    {
        int remaining = width - len;
        int spacesRight = (remaining / 2 );
        int spacesLeft = remaining - spacesRight-1;
        printf("%*s%s%*s", spacesLeft, "", s, spacesRight, "");
    }
}

int main()
{
    char operation[6],pname,ch;
    char alname [20];
    process*p;
    int qua=0;
    int algorithm,quantum,number,totaltime,arrivaltime,servicetime,priority;
    scanf("%s",operation);
    ch = getchar();
//    cin.ignore(numeric_limits<streamsize>::max(), '\n');
//    cin.sync();
//    printf("operation: %s\n",operation);
    scanf("%d-%d",&algorithm,&quantum);
    ch=getchar();
//    printf("algorithm: %d quantum: %d\n",algorithm,quantum);
    scanf("%d",&totaltime);
    ch=getchar();
//    printf("totaltime: %d\n",totaltime);
    scanf("%d",&number);
    ch=getchar();
//    printf("number: %d\n",number);
    process* plist[number];
    if (algorithm==8)
    {
        for (int i=0;i<number;i++)
        {
            scanf("%c,%d,%d",&pname,&arrivaltime,&priority);
            ch=getchar();
            p=process_init(pname,arrivaltime,0,totaltime,priority);
            plist[i]=p;
        }
    }
    else
    {
        for (int i=0;i<number;i++)
        {
            scanf("%c,%d,%d",&pname,&arrivaltime,&servicetime);
            ch=getchar();
            p=process_init(pname,arrivaltime,servicetime,totaltime,0);
            plist[i]=p;
        }
    }
    switch(algorithm)
    {
        case 1:
            sprintf(alname,"FCFS");
            FCFS(plist,number,totaltime);
            break;
        case 2:
            qua=1;
            sprintf(alname,"RR-%d",quantum);
            RR(plist,number,quantum,totaltime);
//            char qua[2];
//            sprintf(qua, "%d", quantum);
////            qua[1]='\0';
//            strcat(alname,"-");
//            strcat(alname,qua);
            break;
        case 3:
            sprintf(alname,"SPN");
            SPN(plist,number,totaltime);
            break;
        case 4:
            sprintf(alname,"SRT");
            SRT_(plist,number,totaltime);
            break;
        case 5:
            sprintf(alname,"HRRN");
            HRRN(plist,number,totaltime);
            break;
        case 6:
            sprintf(alname,"FB-1");
            FB_1_(plist,number,totaltime);
            break;
        case 7:
            FB_2i_(plist,number,totaltime);
            sprintf(alname,"FB-2i");
            break;
        case 8:
            qua=1;
            Aging(plist,number,quantum,totaltime);
            sprintf(alname,"Aging");
            break;
    }
//    printf("Algorithm name: %s",alname);
//    for(int i=0;i<number;i++)
//    {
//        print_process(plist[i],totaltime);
//    }

    if (!strcmp(operation,"trace"))
    {
        if (qua)
        {
            printf("%-6s",alname);
        }
        else
        {
            printf("%-6s",alname);
        }
        for(int i=0;i<totaltime+1;i++)
        {
//            if (i<totaltime+1)
//            {
                printf("%d ",i%10);
//            }
//            else
//            {
//                printf("%d",i%10);
//            }
        }
        printf("\n");
        printf("------------------------------------------------\n");
        for(int i=0;i<number;i++)
        {
            printf("%c     |",plist[i]->name);
            for(int k=0;k<totaltime;k++)
            {
                if (k==totaltime-1)
                {
                    printf("%c| ",(plist[i]->timeline)[k]);
                }
                else
                {
                    printf("%c|",(plist[i]->timeline)[k]);
                }
            }
            printf("\n");
        }
        printf("------------------------------------------------\n\n");
    }
    else
    {
        if (qua)
        {
            printf("%s\n",alname);
        }
        else
        {
            printf("%s\n",alname);
        }
        printf("Process    |");
        for(int i=0;i<number;i++)
        {
            printf("  %c  |",plist[i]->name);
        }
        printf("\n");
        printf("Arrival    |");
        for(int i=0;i<number;i++)
        {
            centrePrint(plist[i]->arrival_time,6);
            printf("|");
        }
        printf("\n");

        printf("Service    |");
        for(int i=0;i<number;i++)
        {
            centrePrint(plist[i]->service_time,6);
            printf("|");
        }
        printf(" Mean|");
        printf("\n");

        printf("Finish     |");
        for(int i=0;i<number;i++)
        {
            centrePrint(plist[i]->finish_time,6);
            printf("|");
        }
        printf("-----|");
        printf("\n");

        float totalturn=0;
        printf("Turnaround |");
        for(int i=0;i<number;i++)
        {
            centrePrint(plist[i]->turnaround_time,6);
            printf("|");
            totalturn+=plist[i]->turnaround_time;
        }
        totalturn=totalturn*1.0/number;
        printf("%5.02f|",totalturn);
        printf("\n");


        float totalnorm=0;
        printf("NormTurn   |");
        for(int i=0;i<number;i++)
        {
            printf("%5.02f|",plist[i]->nor_turnaround_time);
            totalnorm+=plist[i]->nor_turnaround_time;
        }
        totalnorm=totalnorm*1.0/number;
        printf("%5.02f|",totalnorm);
        printf("\n");
        printf("\n");

    }
//    process* plist[5];
//    process* p1=process_init('A',0,3,20,3);
//    process* p2=process_init('B',2,6,20,6);
//    process* p3=process_init('C',4,4,20,4);
//    process* p4=process_init('D',6,5,20,5);
//    process* p5=process_init('E',8,2,20,2);
//    queue< process*> temp;
//    temp.push(p1);
//    temp.push(p2);
//    temp.push(p3);
//    temp.push(p4);
//    temp.push(p5);
//    plist[0]=p1;
//    plist[1]=p2;
//    plist[2]=p3;
//    plist[3]=p4;
//    plist[4]=p5;
//    int number=(sizeof(plist)/sizeof(plist[0]));
//    stable_sort(begin(temp),end(temp),My_func);
//    temp=Sort_Queue(temp);
//    print_process(temp.front());
//    FCFS(plist,number);
//    RR(plist,number,4);
//    SPN(plist,number);
//    HRRN(plist,number);

//    SRT_(plist,number);
//    FB_1_(plist,number);
//    FB_2i_(plist,number);
//    Aging(plist,number,1);
//    print_process(plist[0],20);
//    print_process(plist[1],20);
//    print_process(plist[2],20);
//    print_process(plist[3],20);
//    print_process(plist[4],20);
    return 0;
}
