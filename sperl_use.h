#ifndef SPERL_USE_H
#define SPERL_USE_H

#include "sperl_base.h"

// Use information
struct SPerl_use {
  SPerl_OP* op_package_name;
};

SPerl_USE* SPerl_USE_new(SPerl* sperl);

#endif
