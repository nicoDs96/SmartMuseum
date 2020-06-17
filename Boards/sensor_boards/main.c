#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "shell.h"
#include "thread.h"
#include "xtimer.h"
#include "read_sensors.h"
#include "msg.h"
#include "lora_util.h"
#include "net/loramac.h"
#include "semtech_loramac.h"

static msg_t _recv_queue[RECV_MSG_QUEUE];
static char _recv_stack[THREAD_STACKSIZE_DEFAULT];

char stack[THREAD_STACKSIZE_MAIN];

/* The loramac stack device descriptor */
semtech_loramac_t loramac;

/*
Prints in the std out the cause of PUB_FAIL in pub_msg function
*/
void print_lora_pub_error(uint8_t return_code)
{
    puts("semtech_loramac_send error: ");
    switch (return_code)
    {
        case SEMTECH_LORAMAC_JOIN_SUCCEEDED:
            printf("SEMTECH_LORAMAC_JOIN_SUCCEEDED\n");
            break; //causa l'uscita immediata dallo switch
        case SEMTECH_LORAMAC_JOIN_FAILED:
            printf("SEMTECH_LORAMAC_JOIN_FAILED\n");
            break; //causa l'uscita immediata dallo switch
        case SEMTECH_LORAMAC_NOT_JOINED:
            printf("SEMTECH_LORAMAC_NOT_JOINED\n");
            break; //causa l'uscita immediata dallo switch
        case SEMTECH_LORAMAC_ALREADY_JOINED:
            printf("SEMTECH_LORAMAC_ALREADY_JOINED\n");
            break; //causa l'uscita immediata dallo switch
        case SEMTECH_LORAMAC_TX_OK:
            printf("SEMTECH_LORAMAC_TX_OK\n");
            break; //causa l'uscita immediata dallo switch
        case SEMTECH_LORAMAC_TX_SCHEDULE:
            printf("SEMTECH_LORAMAC_TX_SCHEDULE\n");
            break; //causa l'uscita immediata dallo switch
        case SEMTECH_LORAMAC_TX_DONE:
            printf("SEMTECH_LORAMAC_TX_DONE\n");
            break; //causa l'uscita immediata dallo switch
        case SEMTECH_LORAMAC_TX_CNF_FAILED:
            printf("SEMTECH_LORAMAC_TX_CNF_FAILED\n");
            break; //causa l'uscita immediata dallo switch
        case SEMTECH_LORAMAC_TX_ERROR:
            printf("SEMTECH_LORAMAC_TX_ERROR\n");
            break; //causa l'uscita immediata dallo switch
        case SEMTECH_LORAMAC_RX_DATA:
            printf("SEMTECH_LORAMAC_RX_DATA");
            break; //causa l'uscita immediata dallo switch
        case SEMTECH_LORAMAC_RX_LINK_CHECK:
            printf("SEMTECH_LORAMAC_RX_LINK_CHECK");
            break; //causa l'uscita immediata dallo switch
        case SEMTECH_LORAMAC_RX_CONFIRMED:
            printf("SEMTECH_LORAMAC_RX_CONFIRMED");
            break; //causa l'uscita immediata dallo switch
        case SEMTECH_LORAMAC_BUSY:
            printf("SEMTECH_LORAMAC_BUSY\n");
            break; //causa l'uscita immediata dallo switch
        case SEMTECH_LORAMAC_DUTYCYCLE_RESTRICTED:
            printf("SEMTECH_LORAMAC_DUTYCYCLE_RESTRICTED\n");
            break; //causa l'uscita immediata dallo switch
        default:
            printf("wtf is happening???\n");
    }
}

/*
Recieve loop (thread)
Parameters: 
- [in] arg: not used, every parameter passed will be ignored
 
*/
static void *_recv(void *arg)
{
    msg_init_queue(_recv_queue, RECV_MSG_QUEUE);
    (void)arg;
    while (1) {
        /* blocks until some data is received */
        semtech_loramac_recv(&loramac);
        loramac.rx_data.payload[loramac.rx_data.payload_len] = 0;
        printf("Data received: %s, port: %d\n",
               (char *)loramac.rx_data.payload, loramac.rx_data.port);

               //TODO: handle the received message (namely switch comunication method)
    }
    
    return NULL;
}


/*
Tryies to connect to the lora network with otaa procedure

returns:
 -int: CONNECTION_FAILED if the connection procedure does not succeed or 
                CONNECTION_OK if we can join the authentication process successfully
*/
int connect(void)
{
    /* 1. initialize the LoRaMAC MAC layer */
    semtech_loramac_init(&loramac);
    /* 2. set the keys identifying the device */
    semtech_loramac_set_deveui(&loramac, deveui);
    semtech_loramac_set_appeui(&loramac, appeui);
    semtech_loramac_set_appkey(&loramac, appkey);
    /* 2.1 setting device rate */
    semtech_loramac_set_dr(&loramac, DEVICE_RATE);
    /* 3. join the network */
    if (semtech_loramac_join(&loramac, LORAMAC_JOIN_OTAA) != SEMTECH_LORAMAC_JOIN_SUCCEEDED)
    {
        return CONNECTION_FAILED;
    }
    puts("\nJoin procedure succeeded, otaa net joined.\n");

    return CONNECTION_OK;
}

/*
returns:
 - char[] date_time: the current (local) datetime in format 'yyyy-mm-dd hh:mm:ss'
*/
char[] get_curr_datetime(void){
    char date_time[30];
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(date_time,"%d-%02d-%02d %02d:%02d:%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    return date_time;
}

static int cmd_test_sensors(int argc, char **argv)
{
    (void)argc;
    (void)argv; 
    int16_t temp;
    uint16_t pres;   
    if( read_sensors(&pres, &temp) == READ_FAIL){ //error reading
            puts("Unable to read sensors at this round. Not sending anything.\n");
            
    }else{ //read sensor ok then print data
        
        //Get the current time
        char date_time[30];
        date_time = get_curr_datetime();
        
        //define the message as a string and print values in the message string
        char message[200];

        //split the raw temperature value into integer part and decimal part
        int temp_abs = temp / 100;
        temp -= temp_abs * 100;

        sprintf(message, "{\"station_id\":\"%s\",\"timestamp\":\"%s\",\"temperature\":%2i.%02i,\"pres\":%d}",
                 "stat_1",date_time,temp_abs,temp,pres);
        
        //Print  data (DEBUG) TODO: removeit.
        puts("--------------------------------\n\n");
        printf("Values Read:\n%s\n",message);
        puts("\n\n--------------------------------\n");
        
        /*
        TEST LORA 
        */
        if(connect(&loramac)==CONNECTION_FAILED){
           puts("LORA connection failed");
           return CONNECTION_FAILED;
        }
        uint8_t return_code = semtech_loramac_send(&loramac, (uint8_t *)message, strlen(message) ); 
        if(return_code != SEMTECH_LORAMAC_TX_DONE){
            puts("PUB FAILURE:\n");
            print_lora_pub_error(return_code); // for debug purposes
            return PUB_FAIL;
        }

        thread_create(_recv_stack, sizeof(_recv_stack), THREAD_PRIORITY_MAIN - 1, 0, _recv, NULL, "recv thread");

    }

    return 0;
}


static const shell_command_t shell_commands[] = {
        {"test_sensor", "Read sensors without sending their values and print values in the console. Test LoRaWAN send and receive loops",cmd_test_sensors},
        { NULL, NULL, NULL }
};

int main(void)
{
    puts("Welcome to RIOT!\n");
    puts("Type `help` for help, type `saul` to see all SAUL devices\n");

    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(shell_commands, line_buf, SHELL_DEFAULT_BUFSIZE);


    return 0;
}
