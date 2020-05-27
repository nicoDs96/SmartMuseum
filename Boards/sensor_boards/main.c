/*
SAUL TEST
*/
#include <stdio.h>
#include "saul.h"
#include "shell.h"
#include "thread.h"
#include "xtimer.h"

char stack[THREAD_STACKSIZE_MAIN];

void *thread_handler(void *arg)
{
    (void) arg;
    puts("I'm in \"thread\" now. Try to read sensors using SAUL");
    puts("Initializing sensors");
    static saul_driver_t dev;
    printf("%d ",saul_read_t.read);
    /*saul_init_devs();
    phydat_t readings;
    if( saul_read_t(SAUL_SENSE_ID_TEMP,&readings) == ENOTSUP){
        puts("Not Supported");
        exit(1);
    }else{
        printf("%d %u %d", readings.val,readings.unit,readings.scale);
    }*/
    xtimer_sleep(1);
    return NULL;
}

static int saul_test_(int argc, char **argv){

    (void)argc;
    (void)argv;
    puts("Initializing sensors");
    static saul_driver_t dev;
   // void const devvv;
    phydat_t res;
    dev.read(&dev,&res);
    printf("%d",res.val[0]);
    return 0;
}

static const shell_command_t shell_commands[] = {
        {"prova", "SAUL",saul_test_},
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
