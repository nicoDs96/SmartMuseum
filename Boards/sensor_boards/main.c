#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
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
/* 
define the required keys for OTAA, e.g over-the-air activation (the
null arrays need to be updated with valid LoRa values) 
*/
static const uint8_t deveui[LORAMAC_DEVEUI_LEN] = {0x00, 0x8B, 0x51, 0x44,
                                                   0x3D, 0x92, 0xFC, 0x6F};
static const uint8_t appeui[LORAMAC_APPEUI_LEN] = {0x70, 0xB3, 0xD5, 0x7E,
                                                   0xD0, 0x02, 0xD5, 0x8A};
static const uint8_t appkey[LORAMAC_APPKEY_LEN] = {0xD1, 0x4D, 0x14, 0xAC,
                                                   0x89, 0xA4, 0xF8, 0x03,
                                                   0x02, 0x31, 0x07, 0x50,
                                                   0x89, 0x78, 0x8A, 0x84};
/* The loramac stack device descriptor */
semtech_loramac_t loramac;

/* Globals data structure used to measure and send aggregated data */
int16_t T_MIN=30000;
int16_t T_MAX=-30000;
uint16_t P_MIN=65000;
uint16_t P_MAX=0;
int8_t WINDOW_SIZE = 30; //send each 3 sec if sleep time is 100 ms
float TMP_AGG = 0;
float PRS_AGG = 0;
int8_t w_i = 0;
bool REALTIME = false;

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
    /* 2.1 setting device tx power */
    semtech_loramac_set_tx_power(&loramac, DEVICE_POWER);
    /* 3. join the network */
    if (semtech_loramac_join(&loramac, LORAMAC_JOIN_OTAA) != SEMTECH_LORAMAC_JOIN_SUCCEEDED)
    {
        return CONNECTION_FAILED;
    }
    puts("\nJoin procedure succeeded, otaa net joined.\n");

    return CONNECTION_OK;
}

void compose_message(char *message){
    //Get the current time
    char date_time[30];
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(date_time,"%d-%02d-%02d %02d:%02d:%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

    //split the raw temperature value into integer part and decimal part
    int temp_agg_abs = TMP_AGG / 100;
    int temp_agg_dec = TMP_AGG - temp_agg_abs * 100;

    int temp_min_abs = T_MIN / 100;
    int temp_min_dec = T_MIN - temp_min_abs * 100;

    int temp_max_abs = T_MAX / 100;
    int temp_max_dec = T_MAX - temp_max_abs * 100;


    sprintf(*message, "{\"room_id\":\"%s\",\"timestamp\":\"%s\",\"tmp_avg\":%2i.%02i,\"tmp_max\":%2i.%02i,\"tmp_min\":%2i.%02i,\"prs_avg\":%f,\"prs_min\":%d,\"prs_max\":%d}",
                "1",date_time, temp_agg_abs, temp_agg_dec, temp_min_abs, temp_min_dec, temp_max_abs,temp_max_dec, PRS_AGG, P_MIN, P_MAX);
 
}

/*
PRE: execute connect() funciton to initialize lora 
*/
int send_lora(void){
    
    char message[200];
    compose_message(&message);

    //Print  data (DEBUG) 
    //TODO: remove it.
    printf("Sending message:\n%s\n\n",message);
    printf("strlen(message):\n%d\n\n",strlen(message));
   
    uint8_t return_code = semtech_loramac_send(&loramac, (uint8_t *)message, strlen(message) ); 
    if(return_code != SEMTECH_LORAMAC_TX_DONE){
        puts("PUB FAILURE:\n");
        print_lora_pub_error(return_code); // for debug purposes
        return PUB_FAIL;
    }
    return (int)return_code;

}

int send_mqtt(void){
    puts("Soon available");
    return 1;
}

int send_data(void){
    return REALTIME ? send_mqtt() : send_lora();
}


/*
Read Sensors loop (thread)
Parameters: 
- [in] arg: not used, every parameter passed will be ignored
- [global] WINDOW_SIZE size of the window.
- [global] TMP_AGG temperature aggregate values
- [global] PRS_AGG air pressure aggregate values
- [global] w_i counter of measures, if w_i%WINDOW_SIZE==0 send data to the server
- [global] T_MIN temperature minimum
- [global] T_MAX temperature maximum
- [global] P_MIN air pressure minimum
- [global] P_MAX air pressure maximum
*/
void *compose_window(void *arg)
{
    (void)arg; 
    //      1. read sensors
    //      2. update aggregate values
    //      3. if window is complete send data
    //      4. sleep (100 ms).
    while(1){

        int16_t temp;
        uint16_t pres;   
        if( read_sensors(&pres, &temp) == READ_FAIL){ 
            // TODO: error reading sleep and print error in the console (assuming this is a very rare fault so ignore it)
            // without any counter upgrade
        }else{
            TMP_AGG += temp/WINDOW_SIZE;
            PRS_AGG += pres/WINDOW_SIZE;
            if( T_MIN > temp || T_MIN==NAN ){ T_MIN = temp; }
            if( T_MAX < temp || T_MAX==NAN ){ T_MAX = temp; }
            if( P_MIN > pres || P_MIN==NAN ){ P_MIN = pres; }
            if( P_MAX < pres || P_MAX==NAN ){ P_MAX = pres; }

            ++w_i;
            
            if( w_i%WINDOW_SIZE==0 ){
                send_data();
                w_i=0;
                T_MIN=30000;
                T_MAX=-30000;
                P_MIN=65000;
                P_MAX=0;
                TMP_AGG = 0;
                PRS_AGG = 0;
            }
        }
        xtimer_usleep(100000);//1 microsecond (us) = 10^(-6) seconds -> 100 ms = 100000 us
    }
    
    return NULL;
}



static int cmd_start_sensors(int argc, char **argv)
{
    (void)argc;
    (void)argv;
    // 1. init connections
    if(connect()==CONNECTION_OK){
         // 2. start threads   
        thread_create(_recv_stack, sizeof(_recv_stack), THREAD_PRIORITY_MAIN - 1, 0, _recv, NULL, "recv thread"); //receive thread
        thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN + 1, THREAD_CREATE_STACKTEST, compose_window, NULL, "thread");//send messages thread
        return 0;
    }else{
        puts("connection error.\n EXIT 1");
    }
    return 1;
    
}


static const shell_command_t shell_commands[] = {
        {"start_sensor", "Read sensors, send their values and print values in the console.",cmd_start_sensors},
        { NULL, NULL, NULL }
};

int main(void)
{
    puts("Welcome to RIOT!\n");
    puts("Type `help` for help, Type `start_sensor` to star measuring.\n");

    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(shell_commands, line_buf, SHELL_DEFAULT_BUFSIZE);


    return 0;
}
