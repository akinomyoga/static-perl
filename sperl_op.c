#include "sperl.h"
#include "sperl_op.h"
#include <malloc.h>


void* SPerl_Slab_Alloc(size_t sz) {
  /* TODO */
  return malloc(sizeof(char) * sz);
}

SPerl_OP* SPerl_newLISTOP(I32 type, I32 flags, SPerl_OP *first, SPerl_OP *last) {
  SPerl_LISTOP* listop;

  SPerl_NewOp(listop, 1, SPerl_LISTOP);

  SPerl_OpTYPE_set(listop, type);
  if (first || last)
    flags |= SPerl_OPf_KIDS;
  listop->op_flags = (U8)flags;

  if (!last && first)
    last = first;
  else if (!first && last)
    first = last;
  else if (first)
    SPerl_OpMORESIB_set(first, last);

  listop->op_first = first;
  listop->op_last = last;

  if (type == SPerl_OP_LIST) {
    SPerl_OP* const pushop = SPerl_newOP(SPerl_OP_PUSHMARK, 0);
    SPerl_OpMORESIB_set(pushop, first);
    listop->op_first = pushop;
    listop->op_flags |= SPerl_OPf_KIDS;
    if (!last)
      listop->op_last = pushop;
  }
  if (listop->op_last)
    SPerl_OpLASTSIB_set(listop->op_last, (SPerl_OP*)listop);

  return (SPerl_OP*)listop;
}

SPerl_OP* SPerl_newOP(I32 type, I32 flags) {
  SPerl_OP *o;
  
  SPerl_NewOp(o, 1, SPerl_OP);
  
  SPerl_OpTYPE_set(o, type);
  o->op_flags = (U8)flags;
  o->op_next = o;

  return o;;
}

SPerl_OP* SPerl_newUNOP(I32 type, I32 flags, SPerl_OP* first) {
  SPerl_UNOP *unop;

  if (!first)
    first = SPerl_newOP(SPerl_OP_STUB, 0);

  SPerl_NewOp(unop, 1, SPerl_UNOP);
  SPerl_OpTYPE_set(unop, type);
  unop->op_first = first;
  unop->op_flags = (U8)(flags | SPerl_OPf_KIDS);
  unop->op_private = (U8)(1 | (flags >> 8));

  if (!SPerl_OpHAS_SIBLING(first)) /* true unless weird syntax error */
    SPerl_OpLASTSIB_set(first, (SPerl_OP*)unop);

  if (unop->op_next)
    return (SPerl_OP*)unop;

  return (SPerl_OP *)unop;
}

SPerl_OP* SPerl_newBINOP(I32 type, I32 flags, SPerl_OP* first, SPerl_OP* last) {
  SPerl_BINOP *binop;

  SPerl_NewOp(binop, 1, SPerl_BINOP);

  if (!first)
    first = SPerl_newOP(SPerl_OP_NULL, 0);

  SPerl_OpTYPE_set(binop, type);
  binop->op_first = first;
  binop->op_flags = (U8)(flags | SPerl_OPf_KIDS);
  if (!last) {
    last = first;
    binop->op_private = (U8)(1 | (flags >> 8));
  }
  else {
    binop->op_private = (U8)(2 | (flags >> 8));
      SPerl_OpMORESIB_set(first, last);
  }

  if (!SPerl_OpHAS_SIBLING(last)) /* true unless weird syntax error */
    SPerl_OpLASTSIB_set(last, (SPerl_OP*)binop);

  binop->op_last = SPerl_OpSIBLING(binop->op_first);
  if (binop->op_last)
    SPerl_OpLASTSIB_set(binop->op_last, (SPerl_OP*)binop);

  if (binop->op_next || binop->op_type != (SPerl_OPCODE)type)
    return (SPerl_OP*)binop;

  return (SPerl_OP *)binop;
}
