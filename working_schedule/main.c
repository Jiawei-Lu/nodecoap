#include <stdio.h>
#include "msg.h"
#include "net/gcoap.h"
#include "shell.h"
/*RPL*/
#include "net/gnrc/rpl.h"
#include "net/gnrc/rpl/structs.h"
#include "net/gnrc/rpl/dodag.h"
/*RTC*/
#include "periph_conf.h"
#include "periph/rtc.h"
/*NETIF*/
#include "xtimer.h"
#include "periph/gpio.h"
#include "net/gnrc/netif.h"
#include "net/netif.h"
#include "net/gnrc/netapi.h"
/*PM*/
#include "periph/pm.h"
// #ifdef MODULE_PM_LAYERED
#include "pm_layered.h"
// #endif

#define PERIOD              (2U)
#define REPEAT              (4U)
#define TM_YEAR_OFFSET      (1900)

gnrc_netif_t* netif = NULL;

static unsigned cnt = 0;
struct tm alarm_time;
struct tm current_time;

#define MAIN_QUEUE_SIZE (512)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

/*RTC struct defination*/
struct tm coaptime;

/*RPL*/
gnrc_ipv6_nib_ft_t entry;      
void *state = NULL;
unsigned iface = 0U;

/* import "ifconfig" shell command - for printing addresses */
extern int _gnrc_netif_config(int argc, char **argv);

/*GCOAP Client*/
extern int gcoap_cli_cmd(int argc, char **argv);
extern void gcoap_cli_init(void);

/*shell*/
static const shell_command_t shell_commands[] = {
    { "coap", "CoAP example", gcoap_cli_cmd },
    { NULL, NULL, NULL }
};


/*Print Time*/
void print_time(const char *label, const struct tm *time)
{
    printf("%s  %04d-%02d-%02d %02d:%02d:%02d\n", label,
            time->tm_year + TM_YEAR_OFFSET,
            time->tm_mon + 1,
            time->tm_mday,
            time->tm_hour,
            time->tm_min,
            time->tm_sec);
}

/*RTC Call back function*/ 
static void cb_rtc(void *arg)
{
    puts(arg); /*print state*/
}


/*Radio ON & OFF*/
void radio_off(gnrc_netif_t *netif){
    netopt_state_t state = NETOPT_STATE_SLEEP;
    while ((netif = gnrc_netif_iter(netif))) {
            /* retry if busy */
            while (gnrc_netapi_set(netif->pid, NETOPT_STATE, 0,
                &state, sizeof(state)) == -EBUSY) {}
    }
}
void radio_on(gnrc_netif_t *netif){
    netopt_state_t state = NETOPT_STATE_IDLE;
    while ((netif = gnrc_netif_iter(netif))) {
            /* retry if busy */
            while (gnrc_netapi_set(netif->pid, NETOPT_STATE, 0,
                &state, sizeof(state)) == -EBUSY) {}
    }
}

int main(void)
{
    
    /* for the thread running the shell */
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);
    struct tm time = {
        .tm_year = 2020 - TM_YEAR_OFFSET,   /* years are counted from 1900 */
        .tm_mon  =  4,                      /* 0 = January, 11 = December */
        .tm_mday = 28,
        .tm_hour = 23,
        .tm_min  = 58,
        .tm_sec  = 57
    };   
    rtc_init();
    // int wakeup_gap =60;
    print_time("Clock setto:", &time);
    rtc_set_time(&time);
    rtc_get_time(&time);	
    print_time("Clock check:", &time);
    gcoap_cli_init();
    puts("gcoap example app");   
    puts("Waiting for address autoconfiguration...");
    xtimer_sleep(3);

    /* print network addresses */
    puts("Configured network interfaces:");
    _gnrc_netif_config(0, NULL);
    
    /*print RPL information*/
    puts("Configured rpl:");
    gnrc_rpl_init(7);
    puts("printing route:");
    while (gnrc_ipv6_nib_ft_iter(NULL, iface, &state, &entry)) {
        char addr_str[IPV6_ADDR_MAX_STR_LEN];
        if ((entry.dst_len == 0) || ipv6_addr_is_unspecified(&entry.dst)) {
            printf("default%s ", (entry.primary ? "*" : ""));
             puts("printing route:");   

        }
        else {
            printf("%s/%u ", ipv6_addr_to_str(addr_str, &entry.dst, sizeof(addr_str)),entry.dst_len);
            puts("printing route:");
        }
        if (!ipv6_addr_is_unspecified(&entry.next_hop)) {
           printf("via %s ", ipv6_addr_to_str(addr_str, &entry.next_hop, sizeof(addr_str)));
           puts("printing route:");
        }
        // printf("dev #%u\n", fte->iface);
        // char a[i][4] = entry->iface;
       // i++;
    }
    /*Time schedule*/
    while (1) {
        // puts("wake up!");
        
        ++cnt;
        rtc_get_time(&current_time);
        print_time("currenttime:\n", &current_time);
        int current_timestamp= mktime(&current_time);
        printf("timestamp: %d\n", current_timestamp);
        int _time_duration = 200;
        int _wakeup_duration = 190;
        int alarm_timestamp = 0;
        if ((int)(current_timestamp % _time_duration) < (_wakeup_duration*1)){
            printf("------Waking Up------\n");
            radio_on(netif);
            int chance = ( _wakeup_duration ) - ( current_timestamp % _time_duration );
            alarm_timestamp = (current_timestamp / _time_duration) *_time_duration + (_wakeup_duration * 1);
            alarm_timestamp = alarm_timestamp- 1577836800;
            rtc_localtime(alarm_timestamp, &alarm_time);
            print_time("Sleep alarm:\n", &alarm_time);
            printf("---------%ds ",chance);
            puts("remain for waking up");
            
            xtimer_sleep(chance);
           
            rtc_set_alarm(&alarm_time, cb_rtc, "Sleep Time");
            radio_off(netif);
            puts("radio is off");
            pm_set(1);
        }
        else{
            printf("-------Sleeping-------\n");
            fflush(stdout);
            radio_off(netif);
            alarm_timestamp =  current_timestamp+(_time_duration- (current_timestamp % _time_duration));
            alarm_timestamp = alarm_timestamp - 1577836800;
            rtc_localtime(alarm_timestamp, &alarm_time);
            print_time("Wake-up alarm:\n", &alarm_time);
            // rtc_set_alarm(&alarm_time, cb_rtc, (void *)modetest);
            
            rtc_set_alarm(&alarm_time, cb_rtc, "Wake-up Time");
            pm_set(1);
            // radio_on(netif);
            xtimer_sleep(1);
            // puts("                                                                 ");

        }
    }

    /* start shell */
    puts("All up, running the shell now");
    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(shell_commands, line_buf, SHELL_DEFAULT_BUFSIZE);

    /* should never be reached */
    return 0;
}
