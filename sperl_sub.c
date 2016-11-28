#include <stdlib.h>

#include "sperl_sub.h"
#include "sperl_array.h"
#include "sperl_hash.h"
#include "sperl_parser.h"
#include "sperl_constant_pool.h"

SPerl_SUB* SPerl_SUB_new(SPerl_PARSER* parser) {
  SPerl_SUB* sub = (SPerl_SUB*)calloc(1, sizeof(SPerl_SUB));
  
  sub->op_my_vars = SPerl_PARSER_new_array(parser, 0);
  sub->op_constants = SPerl_PARSER_new_array(parser, 0);
  sub->constant_pool = SPerl_CONSTANT_POOL_new();
  
  return sub;
}
