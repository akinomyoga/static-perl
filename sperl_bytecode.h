#ifndef SPERL_BYTECODE_H
#define SPERL_BYTECODE_H

#include "sperl_base.h"




enum {
  SPerl_BYTECODE_C_CODE_NOP,
  SPerl_BYTECODE_C_CODE_POP,
  SPerl_BYTECODE_C_CODE_POP2,
  SPerl_BYTECODE_C_CODE_ADD_INT,
  SPerl_BYTECODE_C_CODE_ADD_LONG,
  SPerl_BYTECODE_C_CODE_ADD_FLOAT,
  SPerl_BYTECODE_C_CODE_ADD_DOUBLE,
  SPerl_BYTECODE_C_CODE_SUBTRACT_INT,
  SPerl_BYTECODE_C_CODE_SUBTRACT_LONG,
  SPerl_BYTECODE_C_CODE_SUBTRACT_FLOAT,
  SPerl_BYTECODE_C_CODE_SUBTRACT_DOUBLE,
  SPerl_BYTECODE_C_CODE_MULTIPLY_INT,
  SPerl_BYTECODE_C_CODE_MULTIPLY_LONG,
  SPerl_BYTECODE_C_CODE_MULTIPLY_FLOAT,
  SPerl_BYTECODE_C_CODE_MULTIPLY_DOUBLE,
  SPerl_BYTECODE_C_CODE_DIVIDE_INT,
  SPerl_BYTECODE_C_CODE_DIVIDE_LONG,
  SPerl_BYTECODE_C_CODE_DIVIDE_FLOAT,
  SPerl_BYTECODE_C_CODE_DIVIDE_DOUBLE,
  SPerl_BYTECODE_C_CODE_CONVERT_DOUBLE_TO_FLOAT,
  SPerl_BYTECODE_C_CODE_CONVERT_DOUBLE_TO_INT,
  SPerl_BYTECODE_C_CODE_CONVERT_DDOUBLE_TO_LONG,
  SPerl_BYTECODE_C_CODE_CONVERT_FLOAT_TO_DOUBLE,
  SPerl_BYTECODE_C_CODE_CONVERT_FLOAT_TO_INT,
  SPerl_BYTECODE_C_CODE_CONVERT_FLOAT_TO_LONG,
  SPerl_BYTECODE_C_CODE_CONVERT_INT_TO_BYTE,
  SPerl_BYTECODE_C_CODE_CONVERT_INT_TO_CHAR,
  SPerl_BYTECODE_C_CODE_CONVERT_INT_TO_DOUBLE,
  SPerl_BYTECODE_C_CODE_CONVERT_INT_TO_FLOAT,
  SPerl_BYTECODE_C_CODE_CONVERT_INT_TO_LONG,
  SPerl_BYTECODE_C_CODE_CONVERT_INT_TO_SHORT,
  SPerl_BYTECODE_C_CODE_CONVERT_LONG_TO_DOUBLE,
  SPerl_BYTECODE_C_CODE_CONVERT_LONG_TO_FLOAT,
  SPerl_BYTECODE_C_CODE_CONVERT_LONG_TO_INT,
  SPerl_BYTECODE_C_CODE_CONSTANT_DOUBLE_0,
  SPerl_BYTECODE_C_CODE_CONSTANT_DOUBLE_1,
  SPerl_BYTECODE_C_CODE_CONSTANT_FLOAT_0,
  SPerl_BYTECODE_C_CODE_CONSTANT_FLOAT_1,
  SPerl_BYTECODE_C_CODE_CONSTANT_FLOAT_2,
  SPerl_BYTECODE_C_CODE_CONSTANT_INT_M1,
  SPerl_BYTECODE_C_CODE_CONSTANT_INT_0,
  SPerl_BYTECODE_C_CODE_CONSTANT_INT_1,
  SPerl_BYTECODE_C_CODE_CONSTANT_INT_2,
  SPerl_BYTECODE_C_CODE_CONSTANT_INT_3,
  SPerl_BYTECODE_C_CODE_CONSTANT_INT_4,
  SPerl_BYTECODE_C_CODE_CONSTANT_INT_5,
  SPerl_BYTECODE_C_CODE_CONSTANT_LONG_0,
  SPerl_BYTECODE_C_CODE_CONSTANT_LONG_1,
  SPerl_BYTECODE_C_CODE_LOAD_CONSTANT,
  SPerl_BYTECODE_C_CODE_LOAD_CONSTANT_W,
  SPerl_BYTECODE_C_CODE_LOAD_CONSTANT2_W,
  SPerl_BYTECODE_C_CODE_NEGATE_INT,
  SPerl_BYTECODE_C_CODE_NEGATE_LONG,
  SPerl_BYTECODE_C_CODE_NEGATE_FLOAT,
  SPerl_BYTECODE_C_CODE_NEGATE_DOUBLE,
  SPerl_BYTECODE_C_CODE_REMAINDER_INT,
  SPerl_BYTECODE_C_CODE_REMAINDER_LONG,
  SPerl_BYTECODE_C_CODE_REMAINDER_FLOAT,
  SPerl_BYTECODE_C_CODE_REMAINDER_DOUBLE,
  SPerl_BYTECODE_C_CODE_RETURN_INT,
  SPerl_BYTECODE_C_CODE_RETURN_LONG,
  SPerl_BYTECODE_C_CODE_FETURN_FLOAT,
  SPerl_BYTECODE_C_CODE_RETURN_DOUBLE,
  SPerl_BYTECODE_C_CODE_RETURN_REF,
  SPerl_BYTECODE_C_CODE_RETURN,
  SPerl_BYTECODE_C_CODE_LOAD_INT,
  SPerl_BYTECODE_C_CODE_LOAD_INT_0,
  SPerl_BYTECODE_C_CODE_LOAD_INT_1,
  SPerl_BYTECODE_C_CODE_LOAD_INT_2,
  SPerl_BYTECODE_C_CODE_LOAD_INT_3,
  SPerl_BYTECODE_C_CODE_LOAD_LONG,
  SPerl_BYTECODE_C_CODE_LOAD_LONG_0,
  SPerl_BYTECODE_C_CODE_LOAD_LONG_1,
  SPerl_BYTECODE_C_CODE_LOAD_LONG_2,
  SPerl_BYTECODE_C_CODE_LOAD_LONG_3,
  SPerl_BYTECODE_C_CODE_LOAD_FLOAT,
  SPerl_BYTECODE_C_CODE_LOAD_FLOAT_0,
  SPerl_BYTECODE_C_CODE_LOAD_FLOAT_1,
  SPerl_BYTECODE_C_CODE_LOAD_FLOAT_2,
  SPerl_BYTECODE_C_CODE_LOAD_FLOAT_3,
  SPerl_BYTECODE_C_CODE_LOAD_DOUBLE,
  SPerl_BYTECODE_C_CODE_LOAD_DOUBLE_0,
  SPerl_BYTECODE_C_CODE_LOAD_DOUBLE_1,
  SPerl_BYTECODE_C_CODE_LOAD_DOUBLE_2,
  SPerl_BYTECODE_C_CODE_LOAD_DOUBLE_3,
  SPerl_BYTECODE_C_CODE_LOAD_REF,
  SPerl_BYTECODE_C_CODE_LOAD_REF_0,
  SPerl_BYTECODE_C_CODE_LOAD_REF_1,
  SPerl_BYTECODE_C_CODE_LOAD_REF_2,
  SPerl_BYTECODE_C_CODE_LOAD_REF_3,
  SPerl_BYTECODE_C_CODE_STORE_INT,
  SPerl_BYTECODE_C_CODE_STORE_INT_0,
  SPerl_BYTECODE_C_CODE_STORE_INT_1,
  SPerl_BYTECODE_C_CODE_STORE_INT_2,
  SPerl_BYTECODE_C_CODE_STORE_INT_3,
  SPerl_BYTECODE_C_CODE_STORE_LONG,
  SPerl_BYTECODE_C_CODE_STORE_LONG_0,
  SPerl_BYTECODE_C_CODE_STORE_LONG_1,
  SPerl_BYTECODE_C_CODE_STORE_LONG_2,
  SPerl_BYTECODE_C_CODE_STORE_LONG_3,
  SPerl_BYTECODE_C_CODE_STORE_FLOAT,
  SPerl_BYTECODE_C_CODE_STORE_FLOAT_0,
  SPerl_BYTECODE_C_CODE_STORE_FLOAT_1,
  SPerl_BYTECODE_C_CODE_STORE_FLOAT_2,
  SPerl_BYTECODE_C_CODE_STORE_FLOAT_3,
  SPerl_BYTECODE_C_CODE_STORE_DOUBLE,
  SPerl_BYTECODE_C_CODE_STORE_DOUBLE_0,
  SPerl_BYTECODE_C_CODE_STORE_DOUBLE_1,
  SPerl_BYTECODE_C_CODE_STORE_DOUBLE_2,
  SPerl_BYTECODE_C_CODE_STORE_DOUBLE_3,
  SPerl_BYTECODE_C_CODE_STORE_REF,
  SPerl_BYTECODE_C_CODE_STORE_REF_0,
  SPerl_BYTECODE_C_CODE_STORE_REF_1,
  SPerl_BYTECODE_C_CODE_STORE_REF_2,
  SPerl_BYTECODE_C_CODE_STORE_REF_3,
  SPerl_BYTECODE_C_CODE_LOAD_ARRAY_CHAR,
  SPerl_BYTECODE_C_CODE_LOAD_ARRAY_BYTE,
  SPerl_BYTECODE_C_CODE_LOAD_ARRAY_SHORT,
  SPerl_BYTECODE_C_CODE_LOAD_ARRAY_INT,
  SPerl_BYTECODE_C_CODE_LOAD_ARRAY_LONG,
  SPerl_BYTECODE_C_CODE_LOAD_ARRAY_FLOAT,
  SPerl_BYTECODE_C_CODE_LOAD_ARRAY_DOUBLE,
  SPerl_BYTECODE_C_CODE_LOAD_ARRAY_REF,
  SPerl_BYTECODE_C_CODE_STORE_ARRAY_CHAR,
  SPerl_BYTECODE_C_CODE_STORE_ARRAY_BYTE,
  SPerl_BYTECODE_C_CODE_STORE_ARRAY_SHORT,
  SPerl_BYTECODE_C_CODE_STORE_ARRAY_INT,
  SPerl_BYTECODE_C_CODE_STORE_ARRAY_LONG,
  SPerl_BYTECODE_C_CODE_STORE_ARRAY_FLOAT,
  SPerl_BYTECODE_C_CODE_STORE_ARRAY_DOUBLE,
  SPerl_BYTECODE_C_CODE_STORE_ARRAY_REF,
  SPerl_BYTECODE_C_CODE_BIT_OR_INT,
  SPerl_BYTECODE_C_CODE_BIT_OR_LONG,
  SPerl_BYTECODE_C_CODE_BIT_AND_INT,
  SPerl_BYTECODE_C_CODE_BIT_AND_LONG,
  SPerl_BYTECODE_C_CODE_BIT_XOR_INT,
  SPerl_BYTECODE_C_CODE_BIT_XOR_LONG,
  SPerl_BYTECODE_C_CODE_IINC,
  SPerl_BYTECODE_C_CODE_LINC,
  SPerl_BYTECODE_C_CODE_ACONST_NULL,
  SPerl_BYTECODE_C_CODE_ISHL,
  SPerl_BYTECODE_C_CODE_ISHR,
  SPerl_BYTECODE_C_CODE_IUSHR,
  SPerl_BYTECODE_C_CODE_LSHL,
  SPerl_BYTECODE_C_CODE_LSHR,
  SPerl_BYTECODE_C_CODE_LUSHR,
  SPerl_BYTECODE_C_CODE_BIPUSH,
  SPerl_BYTECODE_C_CODE_SIPUSH,
  SPerl_BYTECODE_C_CODE_NEW,
  SPerl_BYTECODE_C_CODE_NEWARRAY,
  SPerl_BYTECODE_C_CODE_ANEWARRAY,
  SPerl_BYTECODE_C_CODE_MULTIANEWARRAY,
  SPerl_BYTECODE_C_CODE_ARRAYLENGTH,
  SPerl_BYTECODE_C_CODE_WIDE,
  SPerl_BYTECODE_C_CODE_IF_ACMPEQ,
  SPerl_BYTECODE_C_CODE_IF_ACMPNE,
  SPerl_BYTECODE_C_CODE_IF_ICMPEQ,
  SPerl_BYTECODE_C_CODE_IF_ICMPNE,
  SPerl_BYTECODE_C_CODE_IF_ICMPLT,
  SPerl_BYTECODE_C_CODE_IF_ICMPGE,
  SPerl_BYTECODE_C_CODE_IF_ICMPGT,
  SPerl_BYTECODE_C_CODE_IF_ICMPLE,
  SPerl_BYTECODE_C_CODE_IFEQ,
  SPerl_BYTECODE_C_CODE_IFNE,
  SPerl_BYTECODE_C_CODE_IFLT,
  SPerl_BYTECODE_C_CODE_IFGE,
  SPerl_BYTECODE_C_CODE_IFGT,
  SPerl_BYTECODE_C_CODE_IFLE,
  SPerl_BYTECODE_C_CODE_IFNULL,
  SPerl_BYTECODE_C_CODE_IFNONNULL,
  SPerl_BYTECODE_C_CODE_LCMP,
  SPerl_BYTECODE_C_CODE_FCMPG,
  SPerl_BYTECODE_C_CODE_FCMPL,
  SPerl_BYTECODE_C_CODE_DCMPG,
  SPerl_BYTECODE_C_CODE_DCMPL,
  SPerl_BYTECODE_C_CODE_GOTO,
  SPerl_BYTECODE_C_CODE_ATHROW,
  SPerl_BYTECODE_C_CODE_INVOKESTATIC,
  SPerl_BYTECODE_C_CODE_GETFIELD,
  SPerl_BYTECODE_C_CODE_PUTFIELD,
  SPerl_BYTECODE_C_CODE_LOOKUPSWITCH, // The bellow codes is TODO
  SPerl_BYTECODE_C_CODE_TABLESWITCH,
  SPerl_BYTECODE_C_CODE_INVOKEDYNAMIC, // The bellow codes is not used
  SPerl_BYTECODE_C_CODE_INVOKEINTERFACE,
  SPerl_BYTECODE_C_CODE_INVOKESPECIAL,
  SPerl_BYTECODE_C_CODE_INVOKEVIRTUAL,
  SPerl_BYTECODE_C_CODE_INSTANCEOF,
  SPerl_BYTECODE_C_CODE_MONITORENTER,
  SPerl_BYTECODE_C_CODE_MONITOREXIT,
  SPerl_BYTECODE_C_CODE_GOTO_W,
  SPerl_BYTECODE_C_CODE_CHECKCAST, 
  SPerl_BYTECODE_C_CODE_PUTSTATIC,
  SPerl_BYTECODE_C_CODE_GETSTATIC,
  SPerl_BYTECODE_C_CODE_JSR,
  SPerl_BYTECODE_C_CODE_JSR_W,
  SPerl_BYTECODE_C_CODE_DUP,
  SPerl_BYTECODE_C_CODE_DUP_X1,
  SPerl_BYTECODE_C_CODE_DUP_X2,
  SPerl_BYTECODE_C_CODE_DUP2,
  SPerl_BYTECODE_C_CODE_DUP2_X1,
  SPerl_BYTECODE_C_CODE_DUP2_X2,
  SPerl_BYTECODE_C_CODE_RET,
  SPerl_BYTECODE_C_CODE_SWAP,
};

extern SPerl_char* const SPerl_BYTECODE_C_CODE_NAMES[];

/*
tableswitch if the following
4 + range <= (3 + 2 * entries) * SWITCHRTIO
*/

#endif
