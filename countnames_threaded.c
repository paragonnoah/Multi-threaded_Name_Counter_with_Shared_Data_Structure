#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>
struct Node
{
    struct Node *next; /* succeeding position in the chain */
    char *line;
};
static struct Node *HEAD = NULL;
int var_signal = 0;
int *ptr_sig = &var_signal;
char *dupString(char *s) /* make a copy of s */
{
    //checks if string's last index value is a new line character
    if (s[strlen(s) - 1] == '\n')
    {
    char *curr = (char *)malloc(sizeof(char) * (int)(strlen(s))); /* +1 for \0 */
    if (curr != NULL)
    {
    int i;
    for (i = 0; i < strlen(s) - 1; i++)
    {
    //memory allocation for the string at position i
    curr[i] = s[i];
    }
    curr[i] = '\0';
 }
 return curr;
 }
    else
 {
 //mallocing the memory for string
 char *curr = (char *)malloc(sizeof(char) * (int)(strlen(s) + 1)); /* +1 for \0
*/
 if (curr != NULL)
 {
 strcpy(curr, s);
 }
 return curr;
 }
}
//appending string by first checking if its head is NULL
void append(struct Node **head, char *line)
{
 struct Node *null_pointer;
 if (*head == NULL)
 {
     null_pointer = (struct Node *)malloc(sizeof(struct Node));
 if (null_pointer == NULL || ((null_pointer->line = dupString(line)) == NULL))
 {
 exit(1);
 }
 null_pointer->next = NULL;
 *head = null_pointer;
 return;
 }
 //otherwise store a temporary value pointing with a pointer to the head of thelist
 else
 {
    struct Node *tmp;
    tmp = *head;
    null_pointer = (struct Node *)malloc(sizeof(struct Node));
    if (null_pointer == NULL || ((null_pointer->line = dupString(line)) == NULL))
    {
    exit(1);
    }
    while (tmp->next != NULL)
    {
    tmp = tmp->next;
    }
    tmp->next = null_pointer;
    }
    return;
}
//print node of linked list
void printNode(struct Node *node)
{
 if (node != NULL)
 {
 printf("PrintingNode: \n"
 "line = _%s_\n",
 node->line);
 }
}
//prints the new linked list
void printLinkList(struct Node *head, int place)
{
 if (head == NULL)
 {
 printf(" -- List tail -- \n");
 }
 else
 {
 struct Node *tmp = head;
 printf("Printing Node: place %d\n"
 "line = _%s_\n",
 place, tmp->line);
 tmp = tmp->next;
 place++;
 printLinkList(tmp, place);
 }
}
void PUSH_HEAD(char *p)
{
 struct Node *tnode;
 if (HEAD == NULL)
 {
 // initialize the stack with first data
 HEAD = (struct Node *)malloc(sizeof(struct Node));
 // no recovery needed if allocation failed, only for debugging
 if (HEAD == NULL)
 {
 printf("HEAD: malloc error \n");
 exit(1);
 }
 if ((HEAD->line = dupString(p)) == NULL)
 {
 printf("Failed to implement dupString \n");
 }
 HEAD->next = NULL;
 }
 else
 {
 // generate 'p' node
 tnode = (struct Node *)malloc(sizeof(struct Node));
 // if allocation failed, no recover necessary
 // for debugging purposes
 if ((tnode->line = dupString(p)) == NULL)
 {
 printf("Failed to implement dupString \n");
 }
 if (tnode == NULL)
 {
 printf("HEAD: malloc error \n");
 exit(1);
 }
    tnode->next = HEAD; // fnode will be inserted as first in list
    HEAD = tnode; // first node will be pointed to TRACE_TOP
 }
}
void POP_TRACE()
{
    struct Node *tnode;
    tnode = HEAD;
    HEAD = tnode->next;
    printf("Now freeing the tnode->line = _%s_ and tnode too\n", tnode->line);
    free(tnode->line);
    free(tnode);
    tnode->line = NULL;
    tnode = NULL;
}
void freeLinkedList(struct Node **head)
{
 //since we are freeing the whole node we do not need
 //to worry about modifying original head
 struct Node *tmp;
 while (*head != NULL)
 {
 //printf("freeing linkedList head->line: _%s_ and current head\n", (*head)->line);
 tmp = (*head);
 (*head) = tmp->next;
 free(tmp->line);
 free(tmp);
 tmp->line = NULL;
 tmp = NULL;
 }
 if (*head == NULL)
 {
 //printf("head == NULL\n");
 }
}
void cpyStr(char **dest, char *source, int size)
{
    //size does not include '\0'
    for (int i = 0; i < size; i++)
    {
        //printf("source[%d] = '%c'\n", i, source[i]);
        (*dest)[i] = source[i];
    }
    (*dest)[size + 1] = '\0';
    printf("\n");
    }
    // when accessing the log index, utilize a thread mutex lock
    pthread_mutex_t threadLock1 = PTHREAD_MUTEX_INITIALIZER;
    //thread mutex lock for critical sections of allocating THREADDATA
    //when allocating THREADDATA, utilize a thread mutex lock for critical sections
    pthread_mutex_t threadLock2 = PTHREAD_MUTEX_INITIALIZER;
    //linked list head access only for thread mutex lock
    pthread_mutex_t threadLock3 = PTHREAD_MUTEX_INITIALIZER;
    //following conditions for each pthread case
    pthread_cond_t printStartCond = PTHREAD_COND_INITIALIZER;
    pthread_cond_t stdinCond = PTHREAD_COND_INITIALIZER;
    pthread_cond_t freeListCond = PTHREAD_COND_INITIALIZER;
    pthread_cond_t printDoneCond = PTHREAD_COND_INITIALIZER;
    //function that runs inside each thread
    void *thread_runner(void *);
    pthread_t tid1, tid2;
    //struct points to thread which makes object.
    //Helps us access thread1. Deallocate thread2 for future use
    struct THREADDATA_STRUCT
    {
        pthread_t creator;
    };
    typedef struct THREADDATA_STRUCT THREADDATA;
    THREADDATA *p = NULL;
    //indexing messages by logging
    int logindex = 0;
    int *logip = &logindex;
    //when reading input of one thread completed, throws a flag
    //allows communication for other thread to know when to stop
    bool read_finish = false;
    bool wait_thread = false;
//return day & time with function
void printDateTime()
{
    // stores date and time as variables
    int hours, minutes, seconds, day, month, year;
    // time_t: defined time type
    time_t now;
    // Obtain current time
    // time() returns the current time of the system as a time_t value
    time(&now);
    // localtime converts a time_t value to calendar time and
    // returns a pointer to a tm structure with its members
    // filled with the corresponding values
    struct tm *local = localtime(&now);
    hours = local->tm_hour; // get hours since midnight (0-23)
    minutes = local->tm_min; // get minutes passed after the hour (0-59)
    seconds = local->tm_sec; // get seconds passed after minute (0-59)
    day = local->tm_mday; // get day of month (1 to 31)
    month = local->tm_mon + 1; // get month of year (0 to 11)
    year = local->tm_year + 1900; // get year since 1900
    printf("%02d/%02d/%d ", day, month, year);
    // print local time
    if (hours < 12)
    { // before midday
        printf("%02d:%02d:%02d am: ", hours, minutes, seconds);
        }
        else
    { // after midday
        printf("%02d:%02d:%02d pm: ", hours - 12, minutes, seconds);
        }
}
void printThreadLog(int logI, long threadId, int pid, char *str1, char *str2)
{
    printf("Logindex: %d, thread: %ld, PID: %d, ",
    logI, (long)threadId, pid);
    printDateTime();
    printf("%s", str1);
    printf("%s\n", str2);
}
int main()
{
    printf("create first thread\n");
    pthread_create(&tid1, NULL, thread_runner, NULL);
    printf("create second thread\n");
    pthread_create(&tid2, NULL, thread_runner, NULL);
    printf("wait for first thread to exit\n");
    pthread_join(tid1, NULL);
    printf("first thread exited\n");
    printf("wait for second thread to exit\n");
    pthread_join(tid2, NULL);
    printf("second thread exited\n");
    exit(0);
} //end main
/**********************************************************************
// function thread_runner runs inside each thread
**********************************************************************/
void *thread_runner(void *x)
{
    char *line = NULL;
    //so tid1 will get to this part first, then tid2
    pthread_mutex_lock(&threadLock2);
    pthread_t me; //for storing the ID of the current thread
    me = pthread_self(); //stored ID of current thread into me
    printf("This is thread %ld (p=%p)\n", (long)me, p); //prints current thread and NULL for tid1
    // beginning of critical section
    if (p == NULL)
    {
    printThreadLog((*logip)++, (long)me, (int)getpid(), "",
    "Allocated memory for p (THREADDATA obj).");
    p = (THREADDATA *)malloc(sizeof(THREADDATA)); //THREADDATA object created
    p->creator = me;
    pthread_cond_wait(&stdinCond, &threadLock2);
 }
 else
 {
     pthread_cond_signal(&stdinCond);
 }
 pthread_mutex_unlock(&threadLock2); // temrination of critical section
 //printing 'p' just prints the address it points to
 //this is the address of a THREADDATA object

//*********************************************************************************
 if(p!= NULL && p->creator == me)
 {

    pthread_mutex_lock(&threadLock3);
    while (!read_finish)
    {
        printThreadLog((*logip)++, (long)me, (int)getpid(), "",
        "Getting input from user.");
        printf("Please enter a line (or 'Control' + 'D' to stop):\n");
        size_t len = 21; //max of input = 20
        ssize_t lineSize = 0;
        if ((lineSize = getline(&line, &len, stdin)) != -1)
        {

            PUSH_HEAD(line);
            }
            else
            {
            printThreadLog((*logip)++, (long)me, (int)getpid(), "",
            "User stopped input collection.");
            read_finish = true;
            free(line);
            }
        pthread_cond_signal(&printStartCond);
        if (!read_finish)
        {
        pthread_cond_wait(&printDoneCond, &threadLock3);
    }
 }
 pthread_mutex_unlock(&threadLock3);
 }
//*********************************************************************************
 else
 {
 //printf("T2 at the first threadLock3\n");
 pthread_mutex_lock(&threadLock3);
 //printf("T2 inside threadLock3\n");
 while (!read_finish)
 {
 if (HEAD != NULL && HEAD->line != NULL)
 {
 printThreadLog((*logip)++, (long)me, (int)getpid(), "HEAD of linked listcontains line: ",
 HEAD->line);
 }
 pthread_cond_signal(&printDoneCond);
 pthread_cond_wait(&printStartCond, &threadLock3);
 }
 pthread_mutex_unlock(&threadLock3);
 }

//*********************************************************************************

//*********************************************************************************
 // TODO use mutex to make this a start of a critical section
 if (p != NULL && p->creator == me)
 {
    pthread_mutex_lock(&threadLock3);
    printThreadLog((*logip)++, (long)me, (int)getpid(), "", "Waiting for THREADDATA to be Freed by thread 2 and then exiting.");
    wait_thread = true;
    pthread_cond_signal(&freeListCond);
    pthread_cond_wait(&freeListCond, &threadLock3);
    printf("This is thread %ld and I did not touch THREADDATA\n", (long)me);
    pthread_mutex_unlock(&threadLock3);
 }
//*********************************************************************************
 else
    {
    /**
    * TODO Free the THREADATA object.
    * Freeing should be done by the other thread from the one that created it.
    * See how the THREADDATA was created for an example of how this is done.
    */
    pthread_mutex_lock(&threadLock3);
    printf("This is thread %ld and I deleted the THREADDATA\n", (long)me);
    printThreadLog((*logip)++, (long)me, (int)getpid(), "",
    "Freeing linkedLisT and THREADDATA.");
    freeLinkedList(&HEAD);
    free(p);
    p = NULL;
    printf("This is thread %ld and I deleted the THREADDATA\n", (long)me);
    printThreadLog((*logip)++, (long)me, (int)getpid(), "",
    "Thread 2 done and Exiting.");
    while (!wait_thread)
    {
        pthread_cond_wait(&freeListCond, &threadLock3);
    }
pthread_cond_signal(&freeListCond);
pthread_mutex_unlock(&threadLock3);
}

//*********************************************************************************
// TODO critical section ends
pthread_exit(NULL);
return NULL;
} //end thread_runner