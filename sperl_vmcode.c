#include "sperl_vmcode.h"







SPerl_uchar* const SPerl_VMCODE_C_CODE_NAMES[] = {
  "POP",
  "POP2",
  "ADD_INT",
  "ADD_LONG",
  "ADD_FLOAT",
  "ADD_DOUBLE",
  "SUBTRACT_INT",
  "SUBTRACT_LONG",
  "SUBTRACT_FLOAT",
  "SUBTRACT_DOUBLE",
  "MULTIPLY_INT",
  "MULTIPLY_LONG",
  "MULTIPLY_FLOAT",
  "MULTIPLY_DOUBLE",
  "DIVIDE_INT",
  "DIVIDE_LONG",
  "DIVIDE_FLOAT",
  "DIVIDE_DOUBLE",
  "D2F",
  "D2I",
  "D2L",
  "F2D",
  "F2I",
  "F2L",
  "I2C",
  "I2B",
  "I2D",
  "I2F",
  "I2L",
  "I2S",
  "L2D",
  "L2F",
  "L2I",
  "DCONST_0",
  "DCONST_1",
  "FCONST_0",
  "FCONST_1",
  "FCONST_2",
  "ICONST_M1",
  "ICONST_0",
  "ICONST_1",
  "ICONST_2",
  "ICONST_3",
  "ICONST_4",
  "ICONST_5",
  "LCONST_0",
  "LCONST_1",
  "LDC",
  "LDC_W",
  "LDC2_W",
  "INEGATE",
  "LNEGATE",
  "FNEGATE",
  "DNEGATE",
  "IREMAINDER",
  "LREMAINDER",
  "FREMAINDER",
  "DREMAINDER",
  "IRETURN",
  "LRETURN",
  "FRETURN",
  "DRETURN",
  "ARETURN",
  "RETURN",
  "ILOAD",
  "ILOAD_0",
  "ILOAD_1",
  "ILOAD_2",
  "ILOAD_3",
  "LLOAD",
  "LLOAD_0",
  "LLOAD_1",
  "LLOAD_2",
  "LLOAD_3",
  "FLOAD",
  "FLOAD_0",
  "FLOAD_1",
  "FLOAD_2",
  "FLOAD_3",
  "DLOAD",
  "DLOAD_0",
  "DLOAD_1",
  "DLOAD_2",
  "DLOAD_3",
  "ALOAD",
  "ALOAD_0",
  "ALOAD_1",
  "ALOAD_2",
  "ALOAD_3",
  "ISTORE",
  "ISTORE_0",
  "ISTORE_1",
  "ISTORE_2",
  "ISTORE_3",
  "LSTORE",
  "LSTORE_0",
  "LSTORE_1",
  "LSTORE_2",
  "LSTORE_3",
  "FSTORE",
  "FSTORE_0",
  "FSTORE_1",
  "FSTORE_2",
  "FSTORE_3",
  "DSTORE",
  "DSTORE_0",
  "DSTORE_1",
  "DSTORE_2",
  "DSTORE_3",
  "ASTORE",
  "ASTORE_0",
  "ASTORE_1",
  "ASTORE_2",
  "ASTORE_3",
  "BALOAD",
  "CALOAD",
  "SALOAD",
  "IALOAD",
  "LALOAD",
  "FALOAD",
  "DALOAD",
  "AALOAD",
  "BASTORE",
  "CASTORE",
  "SASTORE",
  "IASTORE",
  "LASTORE",
  "FASTORE",
  "DASTORE",
  "AASTORE",
  "IAND",
  "LAND",
  "IOR",
  "LOR",
  "IXOR",
  "ACONST_NULL",
  "ANEWARRAY",
  "ARRAYLENGTH",
  "ATHROW",
  "BIPUSH",
  "CHECKCAST",
  "DCMPG",
  "DCMPL",
  "DUP",
  "DUP_X1",
  "DUP_X2",
  "DUP2",
  "DUP2_X1",
  "DUP2_X2",
  "FCMPG",
  "FCMPL",
  "GETFIELD",
  "GOTO",
  "GOTO_W",
  "IF_ACMPEQ",
  "IF_ACMPNE",
  "IF_ICMPEQ",
  "IF_ICMPNE",
  "IF_ICMPLT",
  "IF_ICMPGE",
  "IF_ICMPGT",
  "IF_ICMPLE",
  "IFEQ",
  "IFNE",
  "IFLT",
  "IFGE",
  "IFGT",
  "IFLE",
  "IFNONNULL",
  "IFNULL",
  "IINC",
  "INSTANCEOF",
  "INVOKESTATIC",
  "ISHL",
  "ISHR",
  "IUSHR",
  "JSR",
  "JSR_W",
  "LCMP",
  "LSHL",
  "LSHR",
  "LUSHR",
  "MONITORENTER",
  "MONITOREXIT",
  "MULTIANEWARRAY",
  "NEW",
  "NEWARRAY",
  "NOP",
  "PUTFIELD",
  "RET",
  "SIPUSH",
  "SWAP",
  "WIDE",
  // "LOOKUPSWITCH",
  // "TABLESWITCH",
  // "PUTSTATIC",
  // "GETSTATIC",
  // "INVOKEDYNAMIC",
  // "INVOKEINTERFACE",
  // "INVOKESPECIAL",
  // "INVOKEVIRTUAL",
};
