#include "config.h"

#ifndef _APEX_FWD_H_
#define _APEX_FWD_H_

/* Forwarding Bus to forward data and tag */
typedef struct FWD_BUS
{
  int tag;
  int data;
  int valid;
} FWD_BUS;

#endif