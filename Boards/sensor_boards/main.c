/*
SAUL TEST
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "shell.h"
#include "thread.h"
#include "xtimer.h"
#include "lps331ap.h" //sensors driver 
#include "lps331ap_params.h" //sensors driver 
#include "read_sensors.h"


char stack[THREAD_STACKSIZE_MAIN];

void *thread_handler(void *arg)
{
    (void) arg;

    while (true)
    {
        puts("I'm in \"thread\" now.");
        puts("I will read sensors periodically");
        xtimer_sleep(5);
    }
    
    return NULL;
}

int read_sensors(int *pres, int *temp)
{
   
    static lps331ap_t dev;
    
    if (lps331ap_init(&dev, &lps331ap_params[0]) != 0) {
        puts("Initialization failed");
        return READ_FAIL;
    }

    lps331ap_enable(&dev);
    xtimer_sleep(1); /* wait a bit for the measurements to complete */

    *temp = lps331ap_read_temp(&dev); //temp and pres are already pointers
    *pres = lps331ap_read_pres(&dev);
    lps331ap_disable(&dev); //do not waste energy, disable sensors after measurements

    int temp_abs = *temp / 100;
    *temp -= temp_abs * 100;

    printf("Pressure value: %ihPa - Temperature: %2i.%02i°C\n",
           *pres, temp_abs, *temp);
           
    return READ_OK;
}

static int cmd_test_sensors(int argc, char **argv)
{
    (void)argc;
    (void)argv; 
    int temp;
    int pres;   
    if( read_sensors(&pres, &temp) == READ_FAIL){ //error reading
            puts("Unable to read sensors at this round. Not sending anything.\n");
            
    }else{ //read sensor ok then print data
        //Get the current time and print it into the string date_time
        char date_time[30];
        time_t t = time(NULL);
        struct tm tm = *localtime(&t);
        sprintf(date_time,"%d-%02d-%02d %02d:%02d:%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

        //define the message as a string and print values in the message string
        char message[200];
        sprintf(message, "{\"station_id\":\"%s\",\"timestamp\":\"%s\",\"temperature\":%d,\"pres\":%d}",
                 "stat_1",date_time,temp,pres);
        
        //Print  data.
        
        puts("--------------------------------\n\n");
        printf("Values Read:\n%s\n",message);
        puts("\n\n--------------------------------\n");
        
        }
    return 0;
}


static const shell_command_t shell_commands[] = {
        {"test_sensor", "Read sensors without sending their values and print values in the console",cmd_test_sensors},
        { NULL, NULL, NULL }
};

int main(void)
{
    puts("Welcome to RIOT!\n");
    puts("Type `help` for help, type `saul` to see all SAUL devices\n");

    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(shell_commands, line_buf, SHELL_DEFAULT_BUFSIZE);

    thread_create(stack, sizeof(stack),
                    THREAD_PRIORITY_MAIN - 1,
                    THREAD_CREATE_STACKTEST,
                    thread_handler,
                    NULL, "thread");

    return 0;
}
