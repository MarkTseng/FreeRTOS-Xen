/* -*-  Mode:C; c-basic-offset:4; tab-width:4 -*-
 ****************************************************************************
 * (C) 2003 - Rolf Neugebauer - Intel Research Cambridge
 * (C) 2005 - Grzegorz Milos - Intel Reseach Cambridge
 ****************************************************************************
 *
 *        File: events.h
 *      Author: Rolf Neugebauer (neugebar@dcs.gla.ac.uk)
 *     Changes: Grzegorz Milos (gm281@cam.ac.uk)
 *              
 *        Date: Jul 2003, changes Jun 2005
 * 
 * Environment: Xen Minimal OS
 * Description: Deals with events on the event channels
 *
 ****************************************************************************
 */

#ifndef _EVENTS_H_
#define _EVENTS_H_

#include <stdint.h>
#include <stdlib.h>
#include "pt_regs.h"
#include "hypercalls.h"
#include "console.h"
#include <xen/event_channel.h>

void init_events(void);

extern shared_info_t * HYPERVISOR_shared_info;

typedef void (*evtchn_handler_t)(evtchn_port_t, struct pt_regs *, void *);

/* prototypes */
int do_event(evtchn_port_t port, struct pt_regs *regs);
evtchn_port_t bind_virq(uint32_t virq, evtchn_handler_t handler, void *data);
evtchn_port_t bind_pirq(uint32_t pirq, int will_share, evtchn_handler_t handler, void *data);
evtchn_port_t bind_evtchn(evtchn_port_t port, evtchn_handler_t handler,
						  void *data);
void unbind_evtchn(evtchn_port_t port);
void init_events(void);
int evtchn_alloc_unbound(domid_t pal, evtchn_port_t *port);
int evtchn_bind_interdomain(domid_t pal, evtchn_port_t remote_port, evtchn_port_t *local_port);
int evtchn_get_peercontext(evtchn_port_t local_port, char *ctx, int size);
void unbind_all_ports(void);

static inline int notify_remote_via_evtchn(evtchn_port_t port)
{
	int ret;

    evtchn_send_t op;
    op.port = port;
    ret = HYPERVISOR_event_channel_op(EVTCHNOP_send, &op);
    if (ret != 0) {
	    printk("BUG: event channel notification failed, ret %d\n", ret);
    }

    return ret;
}

void fini_events(void);

#endif /* _EVENTS_H_ */