#ifndef SPERL_ARGUMENT_INFO_H
#define SPERL_ARGUMENT_INFO_H

#include "sperl_type.h"

// Field information
struct SPerl_argument_info {
  SPerl_char* type;
  SPerl_char desc_flags;
  SPerl_char* name;
};

SPerl_ARGUMENT_INFO* SPerl_ARGUMENT_INFO_new();


#endif