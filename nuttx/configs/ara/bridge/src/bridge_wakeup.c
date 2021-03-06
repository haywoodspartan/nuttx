/*
 * Copyright (c) 2016 Google Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from this
 * software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include <stdint.h>
#include <sys/types.h>
#include <nuttx/gpio.h>
#include <tsb_scm.h>
#include <nuttx/greybus/timesync.h>

#include "bridge_wakeup.h"

#ifdef CONFIG_APBRIDGEA
    /* QLIN_DB3_SPK_MODULE-SA-EE20151009A.pdf page 4/5 - APB1_BOOTRET_AND_GATE */
    #define WD_PIN       23
#else
    /* QLIN-MAIN-BD-SD-2016208c.pdf page 40/60 - WAKE_MOD */
    #define WD_PIN       25
#endif

/* TODO: hook some actions to this event handler */
static int wakeup_irq_event(int irq, void *context, void *priv) {
    gpio_irq_mask(WD_PIN);
    timesync_strobe_handler();
    gpio_irq_unmask(WD_PIN);

    return OK;
}

int bridge_wakeup_init(void) {

    gpio_activate(WD_PIN);
    gpio_direction_in(WD_PIN);
    gpio_irq_mask(WD_PIN);
    gpio_irq_settriggering(WD_PIN, IRQ_TYPE_EDGE_RISING);
    gpio_irq_attach(WD_PIN, wakeup_irq_event, NULL);
    gpio_irq_unmask(WD_PIN);

    return 0;
}

