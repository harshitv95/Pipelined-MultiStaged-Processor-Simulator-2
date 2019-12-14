#include "config.h"

#ifndef _APEX_FWD_H_
#define _APEX_FWD_H_

typedef struct FWD_BUS FWD_BUS;

#include "cpu_commons.h"

/* Forwarding Bus to forward data and tag */
struct FWD_BUS
{
  int tag;
  int data;
  int valid;
};

/* 2 Forwarding Buses, 1 from EX2 and another from MEM2 */
FWD_BUS forward[NUM_FWD_BUSES];
int broadcast[NUM_FWD_BUSES * 2];
FWD_BUS *forward_zero;
FWD_BUS *forward_branch; // To forward branch decisions

void forward_data(CPU_Stage *stage);

void forward_to_lsq(CPU_Stage *stage);
void forward_to_iq(CPU_Stage *stage);
void forward_to_rob(CPU_Stage *stage);

void broadcast_tag(int tag);

void invalidate_forward_buses();


int check_forwarded_register(int regNum);

int check_forwarded_bus_data(int regNum);


#endif