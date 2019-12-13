#include "config.h"
#include "cpu_commons.h"

#ifndef _APEX_FWD_H_
#define _APEX_FWD_H_

/* Forwarding Bus to forward data and tag */
typedef struct FWD_BUS
{
  int tag;
  int data;
  int valid;
} FWD_BUS;

void forward(CPU_Stage* stage);

void forward_to_lsq(CPU_Stage* stage);
void forward_to_iq(CPU_Stage* stage);
void forward_to_rob(CPU_Stage* stage);

#endif