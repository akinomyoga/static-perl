#ifndef SPERL_SUB_H
#define SPERL_SUB_H

#include "sperl_base.h"

// Method information
struct SPerl_sub {
  SPerl_OP* op_package;
  SPerl_OP* op_name;
  SPerl_OP* op_return_type;
  SPerl_ARRAY* op_sub_args;
  SPerl_ARRAY* op_my_vars;
  SPerl_OP* op_block;
  _Bool anon;
  int32_t id;
  int32_t bytecode_start_pos;
  int32_t bytecode_length;
  int32_t operand_stack_max;
  int32_t my_vars_size;
  int32_t sub_args_size;
};

SPerl_SUB* SPerl_SUB_new(SPerl* sperl);

#endif
