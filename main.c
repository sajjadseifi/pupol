#include <stdio.h>
#include <math.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/time.h>

#define MAX_THREAD_COUNT 5
// #define DEFAULT_OFFSET 1024
// #define DEFAULT_OFFSET 2048
// #define DEFAULT_OFFSET 4096
#define DEFAULT_OFFSET 8196
// #define DEFAULT_OFFSET 16392
// #define DEFAULT_OFFSET 32784
#define CHUNK 1
FILE *_fp;
int complet = 0;
int offset = 0;
int eof_file = 0;
int lc=0;
int end_pos = 0;
pthread_mutex_t com_mutex,lc_mutex;
struct FO
{
    FILE *fp;
    int origin;
};
char *file_addr; 
const char *r = "r+";

//declaration
int new_distance_allocation();
void *call_thread_to_read(void *args);
int acceptor(char *buffer);
char *getln(FILE *fp);
char ** split(const char * str, const char * delim);
void skeep_to_first_new_line(FILE *fp,int origin);
int splitor(char ** res,char *buffer);
//color
char *conc_color(char *code,char *str);
char *red(char *str);
char *yellow(char *str);
char *reset(char *str);
char *black(char *str);
char *blue(char *str);
char *green(char *str);
char *purple(char *str);
char *cyan(char *str);
char *white(char *str);

char *itos(int i)
{
    char *s = malloc(sizeof(char) * 20);
    
    sprintf(s,"%d",i);

    return s;
}

int main(int argc,char *argv[])
{
    if(argc < 2)
    {
        
        printf("%s\n",red("enter file address in second arguments"));
        return 1;
    }
    file_addr = malloc(sizeof(char) * strlen(argv[1]) + 1);
    strcpy(file_addr,argv[1]);
    
    _fp = fopen(file_addr, r);

    if(_fp == NULL)
    {
        
        printf("%s %s%s%s\n",
            red("can't open the file"),
            yellow("\""),
            yellow(file_addr),
            yellow("\"")
        );
        return 0;
    }

    fseek(_fp, 0L, SEEK_END);
    end_pos = ftell(_fp);

    printf("%s : %s\n",purple("maximum chars"),cyan(itos(end_pos)));
    
    struct timeval start, end;
    offset = DEFAULT_OFFSET;
    // offset = end_pos;
    pthread_t threads[MAX_THREAD_COUNT];
    //init lock
    pthread_mutex_init(&com_mutex, NULL);
    pthread_mutex_init(&lc_mutex, NULL);
    //
    void *thb[MAX_THREAD_COUNT];
    gettimeofday(&start, NULL);
    //init
    while(!eof_file)
    {
        for (int i = 0; i < MAX_THREAD_COUNT; i++)
            pthread_create(threads + i, NULL,
             &call_thread_to_read, (int *)i);

        for (int i = 0; i < MAX_THREAD_COUNT; i++)
            pthread_join(threads[i],NULL);   
    }
    //destroy lock
    pthread_mutex_destroy(&com_mutex);
    pthread_mutex_destroy(&lc_mutex);
    //

    printf("\n%s\n",green("Tasks Complet."));

    gettimeofday(&end, NULL);
    //
    double time_taken = end.tv_sec + end.tv_usec / 1e6 -
                        start.tv_sec - start.tv_usec / 1e6;
    // in seconds
    printf("%s %f %s\n",
        blue("Time :"),
        time_taken,
        blue("Seconds To Execute.")
    );
    return 0;
}


int new_distance_allocation()
{
    //lock
    int origin = -1;
    pthread_mutex_lock(&com_mutex);
    if(eof_file == 1) return origin;
    //assignment
    origin = complet;
    complet += offset;
    //got to position new complet
    if(complet > end_pos) eof_file = 1;
    //
    if(eof_file)
    {   
        printf("\n%s\n",yellow("end file and close the re-request"));
    }
    //unlock
    pthread_mutex_unlock(&com_mutex);
  
    return origin;
}
void *call_thread_to_read(void *args)
{
    FILE *fp = NULL;
    char *buffer;
    int *tid = (int *)args;
    int _lc = 0;
    int origin = new_distance_allocation();

    if(origin > -1) {
        int max_pos = origin + offset;
        //
        while(fp == NULL) fp = fopen(file_addr,r);
        //skeep to get first line
        skeep_to_first_new_line(fp,origin);
        //get linesز to seen orgigin + offset index
        //
        fseek(fp, origin, SEEK_SET);
        //
        while (!feof(fp) && ftell(fp) < max_pos)
        {    
            buffer = getln(fp);
            if(!acceptor(buffer)) continue;
            //
            printf("%s %s %s[%s, %s] : %s%s%s\n",
                blue("Task") ,
                purple(itos((int)tid)) ,
                green("Range") ,
                cyan(itos(origin)),
                cyan(itos(max_pos)),
                yellow("\""),
                yellow(buffer),
                yellow("\"")
            );
        }
        fclose(fp);
    }
    
    pthread_exit(NULL);
}

int acceptor(char *buffer)
{   
    if(buffer == NULL) return 0;

    char *p = strtok(strdup(buffer), ",");
    int index = 1;
    int acc= 0;

    while (p) {
        if(acc==0 && index ==2)
        {
            if(strcmp(p,"pixar") == 0) {
                acc=1;
            }
            else break;
        }
        else if(acc==1 && index == 6)
        {
            if(strcmp(p,"Ubuntu OS") == 0) acc=2;
            break;
        }
        index++;
        p = strtok(NULL, ",");
    }

    if(acc == 2) return 1;

    return 0;
}

char *getln(FILE *fp)
{
    char *line = NULL, *tmp = NULL;
    size_t size = 0, index = 0;
    int ch = EOF;
    while (ch) {
        ch = fgetc(fp);
        /* Check if we need to stop. */
        if (ch == EOF || ch == '\n')
            ch = 0;
        /* Check if we need to expand. */
        if (size <= index) {
            size += CHUNK;
            tmp = realloc(line, size);
            if (!tmp) {
                free(line);
                line = NULL;
                break;
            }
            line = tmp;
        }
        if(line != NULL) line[index++] = ch;
    }
    return line;
}

void skeep_to_first_new_line(FILE *fp,int origin)
{
    int org_tmp = origin;
    if(org_tmp > 0)
    {
        int ch = EOF;
        while (ch) {
            ch = fgetc(fp);
            org_tmp++;
            if (ch == EOF || ch == '\n') ch = 0;
        }
    }
}

char *conc_color(char *code,char *str)
{
    int ssz = strlen(code) + strlen(str) + strlen("\033[0m") + 1;
    char *buf = malloc(ssz * sizeof(char));
    
    sprintf(buf,"%s%s%s",code,str,"\033[0m");

    return buf;
}
char * red (char *str)
{
  return conc_color("\033[1;31m",str);
}
char *yellow(char *str)
{
  return conc_color("\033[1;33m",str);
}
char *reset (char *str) {
  return conc_color("\033[0m",str);
}
char *black(char *str)
{
  return conc_color("\033[1;30m",str);
}
char *blue(char *str)
{
  return conc_color("\033[1;34m",str);
}
char *green(char *str)
{
  return conc_color("\033[1;32m",str);
}
char *purple(char *str)
{
  return conc_color("\033[1;35m",str);
}
char *cyan(char *str)
{
  return conc_color("\033[1;36m",str);
}
char *white(char *str)
{
  return conc_color("\033[1;37m",str);
}