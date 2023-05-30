#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/shm.h>        
#include <errno.h>           
#include <semaphore.h>      
#include <fcntl.h>        



//initialize to 0 function
void initArray(int *array, int size)
{
    for (int i = 0; i < size; i++)
    {
       array[i] = 0;
    }
}



//find max function
int findMax(int *array, int size)
{ 
   int i;
   int maxToken = 0;

  for (i = 0; i < size; i++)
  {
      if(array[i] > maxToken)
      {
          maxToken = array[i];
      }
  }

  return maxToken;
}



//printf array function
void printArray(int *array, int size)
{
    for (int i = 0; i < size; i ++)
    {
        printf("%d\t",array[i]);
    }
}




int main()
{

    int cnt , chStatus;
    
    pid_t deadPid;
    
    key_t shmkey[3];

    int shmid[3];




    printf("Please enter number of child processes to be created: ");
    scanf("%d",&cnt);



    int *SA     =  malloc(cnt * sizeof(*SA));    //shared
    int *token   = malloc(cnt * sizeof(*token));  //shared
    int *choosing = malloc(cnt * sizeof(*choosing));  //shared
    pid_t *pid    = malloc(cnt * sizeof(*pid));  



    if(!SA || !token || !choosing || !pid)
    {
        printf("Failure allocating memory.");
        exit(EXIT_FAILURE);

    }


    
    for(int i = 0; i < 3; i++)
    {
    shmkey[i] = ftok ("/dev/null", i);                            //hashes the pathname and integer to a unique key. returns -1 on failure.
    shmid[i] = shmget(shmkey[i], sizeof (int), 0644 | IPC_CREAT); // allocates memory segment. On sucess returns memory key.
    }


    // shared memory error check 
    for(int i = 0; i < 3; i++)
    {
       if (shmid[i] == -1)                           
       {
        perror ("shmget\n");
        exit (1);
       }
    }

    // memory attachment
     SA = (int *) shmat(shmid[0], NULL, 0);  
     initArray(SA,cnt); 

     token = (int *) shmat(shmid[1], NULL, 0);
     initArray(token,cnt);

     choosing = (int *) shmat(shmid[2], NULL, 0);
     initArray(choosing,cnt);




    for(int i = 0; i < cnt; i++)
    {

        pid[i] = fork();


        if(pid[i] == 0) //if child process
        {   

             sleep(cnt - i);
             //get token
             choosing[i] =  1; 
             token[i] = findMax(token , cnt) + 1;  
             choosing[i] = 0;


             for(int j = 0; j < cnt - 1; j++)
             {
                 while(choosing[j]) {/* wait */}

                 while(token[j] != 0 && (token[j] < token[i] || (token[i] == token[j] && j < i))) {/* still do nothing */}
                 
                 //critical section
                 for (int k = 0; k < cnt; k++)
                 {
                     SA[k] = SA[k] + i;
                 }
                 token[i] = 0; 
                 //end of critical section
                 printf("\nHello, i was process %d with pid: %d and SA values now are:\n",i,getpid());
                 
                 printArray(SA,cnt);

                 printf("\n");

                 exit(i);
             } //end of j loop
        }

         if (pid[i] < 0) 
         {
            perror("Something went wrong with fork..");
            return -1;  
         }

    }// end of i loop


    // wait children first to terminate
    for(int i = 0; i < cnt; i++)
    {
        deadPid = waitpid(pid[i],&chStatus,0);
        
         if(WIFEXITED(chStatus))
         {
            printf("child %d terminated with exit status %d\n", deadPid, WEXITSTATUS(chStatus) );

         }
         else printf("Child %d terminated abnormally\n", deadPid);

    }

    free(pid);
    
    return (0);
}
