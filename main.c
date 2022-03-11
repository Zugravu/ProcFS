#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define PROP_SIZE_MAX 5000
#define COMM_SIZE_MAX 10000
#define NUM_PROC_MAX 10000
#define PID_MAX 1000000

typedef struct process
{

    char user[PROP_SIZE_MAX];
    int pid;
    int ppid;
    char cpu[PROP_SIZE_MAX];
    char mem[PROP_SIZE_MAX];
    char vsz[PROP_SIZE_MAX];
    char rss[PROP_SIZE_MAX];
    char tty[PROP_SIZE_MAX];
    char stat[PROP_SIZE_MAX];
    char start[PROP_SIZE_MAX];
    char time[PROP_SIZE_MAX];
    char command[COMM_SIZE_MAX];

};

struct process processes[NUM_PROC_MAX];
int nrProc = 0, successors[NUM_PROC_MAX][NUM_PROC_MAX], nrSucc[NUM_PROC_MAX];
char usrPath[PROP_SIZE_MAX] = "";

int mapProcPids[PID_MAX];

void extractData()
{
    system("ps -aux > process_details.txt");
    system("ps xao pid,ppid > process_parents.txt");
}

void parseFiles()
{

    FILE *fs1, *fs2;
    fs1 = fopen("process_details.txt", "r");
    fs2 = fopen("process_parents.txt", "r");

    char it1, it2;

    while(fscanf(fs1, "%c", &it1))
    {
        if(it1 == '\n')
            break;
    }
    while(fscanf(fs2, "%c", &it2))
    {
        if(it2 == '\n')
            break;
    }

    while(1)
    {
        nrProc++;

        fscanf(fs1, "%c", &it1);
        processes[nrProc].user[0] = it1;
        if (it1 == '\n')
        {
            break;
        }
        while(fscanf(fs1, "%c", &it1))
        {
            if(it1 == ' ')
                break;
            processes[nrProc].user[strlen(processes[nrProc].user)] = it1;
        }
        while(fscanf(fs1, "%c", &it1))
        {
            if(it1 != ' ')
            {
                processes[nrProc].pid = (it1 - '0');
                break;
            }
        }
        while(fscanf(fs1, "%c", &it1))
        {
            if(it1 == ' ')
                break;
            processes[nrProc].pid = processes[nrProc].pid * 10 + (it1 - '0');
        }
        while(fscanf(fs1, "%c", &it1))
        {
            if(it1 != ' ')
            {
                processes[nrProc].cpu[0] = it1;
                break;
            }
        }
        while(fscanf(fs1, "%c", &it1))
        {
            if(it1 == ' ')
                break;
            processes[nrProc].cpu[strlen(processes[nrProc].cpu)] = it1;
        }
        while(fscanf(fs1, "%c", &it1))
        {
            if(it1 != ' ')
            {
                processes[nrProc].mem[0] = it1;
                break;
            }
        }
        while(fscanf(fs1, "%c", &it1))
        {
            if(it1 == ' ')
                break;
            processes[nrProc].mem[strlen(processes[nrProc].mem)] = it1;
        }
        while(fscanf(fs1, "%c", &it1))
        {
            if(it1 != ' ')
            {
                processes[nrProc].vsz[0] = it1;
                break;
            }
        }
        while(fscanf(fs1, "%c", &it1))
        {
            if(it1 == ' ')
                break;
            processes[nrProc].vsz[strlen(processes[nrProc].vsz)] = it1;
        }
        while(fscanf(fs1, "%c", &it1))
        {
            if(it1 != ' ')
            {
                processes[nrProc].rss[0] = it1;
                break;
            }
        }
        while(fscanf(fs1, "%c", &it1))
        {
            if(it1 == ' ')
                break;
            processes[nrProc].rss[strlen(processes[nrProc].rss)] = it1;
        }
        while(fscanf(fs1, "%c", &it1))
        {
            if(it1 != ' ')
            {
                processes[nrProc].tty[0] = it1;
                break;
            }
        }
        while(fscanf(fs1, "%c", &it1))
        {
            if(it1 == ' ')
                break;
            processes[nrProc].tty[strlen(processes[nrProc].tty)] = it1;
        }
        while(fscanf(fs1, "%c", &it1))
        {
            if(it1 != ' ')
            {
                processes[nrProc].stat[0] = it1;
                break;
            }
        }
        while(fscanf(fs1, "%c", &it1))
        {
            if(it1 == ' ')
                break;
            processes[nrProc].stat[strlen(processes[nrProc].stat)] = it1;
        }
        while(fscanf(fs1, "%c", &it1))
        {
            if(it1 != ' ')
            {
                processes[nrProc].start[0] = it1;
                break;
            }
        }
        while(fscanf(fs1, "%c", &it1))
        {
            if(it1 == ' ')
                break;
            processes[nrProc].start[strlen(processes[nrProc].start)] = it1;
        }
        while(fscanf(fs1, "%c", &it1))
        {
            if(it1 != ' ')
            {
                processes[nrProc].time[0] = it1;
                break;
            }
        }
        while(fscanf(fs1, "%c", &it1))
        {
            if(it1 == ' ')
                break;
            processes[nrProc].time[strlen(processes[nrProc].time)] = it1;
        }
        while (fscanf(fs1, "%c", &it1))
        {
            if (it1 != ' ')
            {
                processes[nrProc].command[0] = it1;
                break;
            }
        }
        while (fscanf(fs1, "%c", &it1))
        {
            if (it1 == '\n')
                break;
            processes[nrProc].command[strlen(processes[nrProc].command)] = it1;
        }
        fscanf(fs2, "%c", &it2);
        if (it2 == EOF)
            break;
        fscanf(fs2, "%d %d", &processes[nrProc].pid, &processes[nrProc].ppid);
        mapProcPids[processes[nrProc].pid] =  nrProc;
    }
    fclose(fs1);
    fclose(fs2);
}

void buildTree()
{

    int i;
    int proc_pid, parent_pid, parent_index;
    for (i = 0; i < nrProc; i++)
    {
        proc_pid = processes[i].pid;
        parent_pid = processes[i].ppid;
        parent_index = mapProcPids[parent_pid];
        if (proc_pid != parent_pid)
        {
            nrSucc[parent_index]++;
            successors[parent_index][nrSucc[parent_index]] = i;
        }
    }
}


void buildFiles(int current_proc, char current_path[PROP_SIZE_MAX])
{
    if (current_proc != 0)
    {
        FILE *outputFile;
        char pathAuxiliar[PROP_SIZE_MAX] = "";
        strcpy(pathAuxiliar, current_path);
        strcat(pathAuxiliar, "stats");
        outputFile = fopen(pathAuxiliar, "w+r");
        fprintf(outputFile, "USER: %s\nPID: %d\nPPID: %d\nCPU: %s\nMEM: %s\nVSZ: %s\nRSS: %s\nTTY: %s\nSTAT: %s\nSTART: %s\nTIME: %s\nCOMMAND: %s\n",
                processes[current_proc].user,
                processes[current_proc].pid,
                processes[current_proc].ppid,
                processes[current_proc].cpu,
                processes[current_proc].mem,
                processes[current_proc].vsz,
                processes[current_proc].rss,
                processes[current_proc].tty,
                processes[current_proc].stat,
                processes[current_proc].start,
                processes[current_proc].time,
                processes[current_proc].command);
        fclose(outputFile);
    }
    int i, child_pid, j, child;
    for (i = 1; i <= nrSucc[current_proc]; i++)
    {
        child = successors[current_proc][i];
        child_pid = processes[child].pid;
        char pidToString[PROP_SIZE_MAX] = "";
        sprintf(pidToString, "%d", child_pid);
        char childPath[PROP_SIZE_MAX] = "";
        strcpy(childPath, current_path);
        strcat(childPath, "/");
        strcat(childPath, pidToString);

        struct stat st = {};
        
        if (stat(childPath, &st) == -1)
            mkdir(childPath, 0777);

        strcat(childPath, "/");
        buildFiles(child, childPath);
    }

}

int main(int argc, char* argv[])
{

    extractData();
    parseFiles();
    buildTree();

    strcpy(usrPath, get_current_dir_name());

    struct stat st = {0};
    char DirPath[PROP_SIZE_MAX] = "";
    strcpy(DirPath, usrPath);
    strcat(DirPath, "/processes");

    if (stat(DirPath, &st) == -1)
        mkdir(DirPath, 0777);
    strcat(DirPath, "/");
    buildFiles(0, DirPath);

    return 0;
}