/*
 * Copyright (C) 2015 Freie Universität Berlin
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     examples
 * @{
 *
 * @file
 * @brief       Example application for demonstrating the RIOT network stack
 *
 * @author      Hauke Petersen <hauke.petersen@fu-berlin.de>
 *
 * @}
 */

#include <stdio.h>

#include "shell.h"
#include "msg.h"
#include "pm_layered.h"
#include "periph/pm.h"
#include "net/netdev.h"
#include "periph/gpio.h"
// #include "common.h"
#include "periph/rtc.h"
#include <time.h>
#include "periph_conf.h"
#include "xtimer.h"
#include "shell.h"
#include "shell_commands.h"
#include "net/gnrc/netif.h"
#include "net/netif.h"
#include "net/gnrc/netapi.h"
#define MAIN_QUEUE_SIZE     (8)

// at86rf2xx_t devs[AT86RF2XX_NUM];
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

extern int udp_cmd(int argc, char **argv);

static const shell_command_t shell_commands[] = {
    { "udp", "send data over UDP and listen on UDP ports", udp_cmd },
    { NULL, NULL, NULL }
};

// static void _gnrc_netapi_set_all(netopt_state_t state)
// {
//     gnrc_netif_t* netif = NULL;
//     while ((netif = gnrc_netif_iter(netif))) {
//         /* retry if busy */
//         while (gnrc_netapi_set(netif->pid, NETOPT_STATE, 0,
//                                &state, sizeof(state)) == -EBUSY) {}
//     }
// }

// void radio_off() {
//     netopt_state_t devstate = NETOPT_STATE_SLEEP;
//     _gnrc_netapi_set_all(devstate);
// }

// void radio_on() {
//     netopt_state_t devstate = NETOPT_STATE_IDLE;
//     _gnrc_netapi_set_all(devstate);
// }

int main(void)
{
    /* we need a message queue for the thread running the shell in order to
     * receive potentially fast incoming networking packets */
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);
    puts("RIOT network stack example application");
    puts("AT86RF2xx device driver test");

    // unsigned dev_success = 0;
    // for (unsigned i = 0; i < AT86RF2XX_NUM; i++) {
    //     netopt_enable_t en = NETOPT_ENABLE;
    //     const at86rf2xx_params_t *p = &at86rf2xx_params[i];
    //     netdev_t *dev = (netdev_t *)(&devs[i]);

    //     printf("Initializing AT86RF2xx radio at SPI_%d\n", p->spi);
    //     at86rf2xx_setup(&devs[i], p);
    //     dev->event_callback = _event_cb;
    //     if (dev->driver->init(dev) < 0) {
    //         continue;
    //     }
    //     dev->driver->set(dev, NETOPT_RX_END_IRQ, &en, sizeof(en));
    //     dev_success++;
    // }

    // if (!dev_success) {
    //     puts("No device could be initialized");
    //     return 1;
    // }

    // at86rf2xx_set_state(devs, AT86RF2XX_STATE_FORCE_TRX_OFF);
    // gpio_set(AT86RF2XX_PARAM_RESET);
    // /* start shell */
    netopt_state_t state = NETOPT_STATE_SLEEP;
    gnrc_netif_t* netif = NULL;
    while ((netif = gnrc_netif_iter(netif))) {
        /* retry if busy */
        while (gnrc_netapi_set(netif->pid, NETOPT_STATE, 0,
                               &state, sizeof(state)) == -EBUSY) {}
    }
    
    // _gnrc_netapi_set_all(devstate);
    puts("All up, running the shell now");
    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(shell_commands, line_buf, SHELL_DEFAULT_BUFSIZE);

    /* should be never reached */
    return 0;
}
