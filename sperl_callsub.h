#ifndef SPERL_CALLSUB_H
#define SPERL_CALLSUB_H

#include "sperl_base.h"

// Field information
struct SPerl_callsub {
  SPerl_VAR* var;
  SPerl_WORD* package_name_word;
  SPerl_WORD* sub_name_word;
  SPerl_int argument_count;
  SPerl_boolean anon;
  SPerl_char* sub_complete_name;
};

SPerl_CALLSUB* SPerl_CALLSUB_new(SPerl_PARSER* parser);

#endif
