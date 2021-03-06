#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#include "sperl.h"
#include "sperl_parser.h"
#include "sperl_vm.h"
#include "sperl_allocator.h"
#include "sperl_array.h"
#include "sperl_hash.h"
#include "sperl_bytecode_array.h"
#include "sperl_bytecode.h"
#include "sperl_sub.h"
#include "sperl_op.h"
#include "sperl_constant_pool.h"
#include "sperl_frame.h"

SPerl_VM* SPerl_VM_new(SPerl* sperl) {
  SPerl_VM* vm = SPerl_ALLOCATOR_alloc_memory_pool(sperl, sizeof(SPerl_VM));
  
  vm->call_stack_capacity = 255;
  vm->call_stack = malloc(sizeof(int64_t) * vm->call_stack_capacity);
  vm->frame_capacity = 255;
  vm->frame = malloc(sizeof(SPerl_FRAME) * vm->frame_capacity);
  
  return vm;
}

void SPerl_VM_call_sub(SPerl* sperl, SPerl_VM* vm, const char* sub_base_name) {
  
  // Subroutine
  SPerl_PARSER* parser = sperl->parser;
  
  // Constant pool
  int32_t* constant_pool = sperl->constant_pool->values;
  
  // Bytecode
  uint8_t* bytecodes = sperl->bytecode_array->values;

  // Operand stack
  int64_t* operand_stack = vm->call_stack;
  
  // Variables
  int64_t* vars = NULL;
  
  // Program counter
  register int32_t pc = 0;
  
  // Top position of operand stack
  register int32_t operand_stack_top = -1;
  
  // Get subroutine
  SPerl_SUB* sub = SPerl_HASH_search(parser->sub_name_symtable, sub_base_name, strlen(sub_base_name));
  
  register _Bool condition;
  
  // Frame stack
  SPerl_FRAME* frame_stack = malloc(sizeof(SPerl_FRAME) * 255);
  
  // Frame stack next
  int32_t frame_stack_top = -1;
  
  // Goto subroutine
  goto CALLSUB_COMMON;
  
  while (1) {
    
    switch (bytecodes[pc]) {
      case SPerl_BYTECODE_C_CODE_NOP:
        // Not used
        assert(0);
      case SPerl_BYTECODE_C_CODE_ACONST_NULL:
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_ICONST_M1:
        operand_stack_top++;
        *(int32_t*)&operand_stack[operand_stack_top] = -1;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_ICONST_0:
        operand_stack_top++;
        *(int32_t*)&operand_stack[operand_stack_top] = 0;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_ICONST_1:
        operand_stack_top++;
        *(int32_t*)&operand_stack[operand_stack_top] = 1;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_ICONST_2:
        operand_stack_top++;
        *(int32_t*)&operand_stack[operand_stack_top] = 2;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_ICONST_3:
        operand_stack_top++;
        *(int32_t*)&operand_stack[operand_stack_top] = 3;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_ICONST_4:
        operand_stack_top++;
        *(int32_t*)&operand_stack[operand_stack_top] = 4;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_ICONST_5:
        operand_stack_top++;
        *(int32_t*)&operand_stack[operand_stack_top] = 5;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_LCONST_0:
        operand_stack_top++;
        operand_stack[operand_stack_top] = 0L;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_LCONST_1:
        operand_stack_top++;
        operand_stack[operand_stack_top] = 1L;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_FCONST_0:
        operand_stack_top++;
        *((float*)&operand_stack[operand_stack_top]) = 0.F;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_FCONST_1:
        operand_stack_top++;
        *((float*)&operand_stack[operand_stack_top]) = 1.F;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_FCONST_2:
        operand_stack_top++;
        *((float*)&operand_stack[operand_stack_top]) = 2.F;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_DCONST_0:
        operand_stack_top++;
        *(double*)&operand_stack[operand_stack_top] = 0.0;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_DCONST_1:
        operand_stack_top++;
        *(double*)&operand_stack[operand_stack_top] = 1.0;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_BIPUSH:
        operand_stack_top++;
        *(int32_t*)&operand_stack[operand_stack_top] = (int8_t)bytecodes[pc + 1];
        pc += 2;
        continue;
      case SPerl_BYTECODE_C_CODE_SIPUSH:
        operand_stack_top++;
        *(int32_t*)&operand_stack[operand_stack_top] = (int16_t)((int16_t)(bytecodes[pc + 1] << 8) +  bytecodes[pc + 2]);
        pc += 3;
        continue;
      case SPerl_BYTECODE_C_CODE_LDC:
        // Not used
        assert(0);
      case SPerl_BYTECODE_C_CODE_LDC_W:
        // Not used
        assert(0);
      case SPerl_BYTECODE_C_CODE_LDC2_W:
        // Not used
        assert(0);
      case SPerl_BYTECODE_C_CODE_ILOAD:
        operand_stack_top++;
        *(int32_t*)&operand_stack[operand_stack_top] = *(int32_t*)&vars[bytecodes[pc + 1]];
        pc += 2;
        continue;
      case SPerl_BYTECODE_C_CODE_LLOAD:
        operand_stack_top++;
        operand_stack[operand_stack_top] = vars[bytecodes[pc + 1]];
        pc += 2;
        continue;
      case SPerl_BYTECODE_C_CODE_FLOAD:
        operand_stack_top++;
        *(int32_t*)&operand_stack[operand_stack_top] = *(int32_t*)&vars[bytecodes[pc + 1]];
        pc += 2;
        continue;
      case SPerl_BYTECODE_C_CODE_DLOAD:
        operand_stack_top++;
        operand_stack[operand_stack_top] = vars[bytecodes[pc + 1]];
        pc += 2;
        continue;
      case SPerl_BYTECODE_C_CODE_ALOAD:
        operand_stack_top++;
        operand_stack[operand_stack_top] = vars[bytecodes[pc + 1]];
        pc += 2;
        continue;
      case SPerl_BYTECODE_C_CODE_ILOAD_0:
        operand_stack_top++;
        *(int32_t*)&operand_stack[operand_stack_top] = *(int32_t*)&vars[0];
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_ILOAD_1:
        operand_stack_top++;
        *(int32_t*)&operand_stack[operand_stack_top] = *(int32_t*)&vars[1];
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_ILOAD_2:
        operand_stack_top++;
        *(int32_t*)&operand_stack[operand_stack_top] = *(int32_t*)&vars[2];
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_ILOAD_3:
        operand_stack_top++;
        *(int32_t*)&operand_stack[operand_stack_top] = *(int32_t*)&vars[3];
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_LLOAD_0:
        operand_stack_top++;
        operand_stack[operand_stack_top] = vars[0];
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_LLOAD_1:
        operand_stack_top++;
        operand_stack[operand_stack_top] = vars[1];
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_LLOAD_2:
        operand_stack_top++;
        operand_stack[operand_stack_top] = vars[2];
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_LLOAD_3:
        operand_stack_top++;
        operand_stack[operand_stack_top] = vars[3];
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_FLOAD_0:
        operand_stack_top++;
        *(int32_t*)&operand_stack[operand_stack_top] = *(int32_t*)&vars[0];
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_FLOAD_1:
        operand_stack_top++;
        *(int32_t*)&operand_stack[operand_stack_top] = *(int32_t*)&vars[1];
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_FLOAD_2:
        operand_stack_top++;
        *(int32_t*)&operand_stack[operand_stack_top] = *(int32_t*)&vars[2];
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_FLOAD_3:
        operand_stack_top++;
        *(int32_t*)&operand_stack[operand_stack_top] = *(int32_t*)&vars[3];
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_DLOAD_0:
        operand_stack_top++;
        operand_stack[operand_stack_top] = vars[0];
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_DLOAD_1:
        operand_stack_top++;
        operand_stack[operand_stack_top] = vars[1];
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_DLOAD_2:
        operand_stack_top++;
        operand_stack[operand_stack_top] = vars[2];
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_DLOAD_3:
        operand_stack_top++;
        operand_stack[operand_stack_top] = vars[3];
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_ALOAD_0:
        operand_stack_top++;
        operand_stack[operand_stack_top] = vars[0];
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_ALOAD_1:
        operand_stack_top++;
        operand_stack[operand_stack_top] = vars[1];
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_ALOAD_2:
        operand_stack_top++;
        operand_stack[operand_stack_top] = vars[2];
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_ALOAD_3:
        operand_stack_top++;
        operand_stack[operand_stack_top] = vars[3];
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_IALOAD:
        pc++;
        continue;
        
        // TODO
        *(int32_t*)&operand_stack[operand_stack_top - 1] = *(int32_t*)(operand_stack[operand_stack_top - 1] + *(int32_t*)&operand_stack[operand_stack_top]);
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_LALOAD:
        pc++;
        continue;

        // TODO
        operand_stack[operand_stack_top - 1] = *(int64_t*)(operand_stack[operand_stack_top - 1] + *(int32_t*)&operand_stack[operand_stack_top]);
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_FALOAD:
        pc++;
        continue;

        // TODO
        *(float*)&operand_stack[operand_stack_top - 1] = *(float*)(operand_stack[operand_stack_top - 1] + *(int32_t*)&operand_stack[operand_stack_top]);
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_DALOAD:
        pc++;
        continue;

        // TODO
        *(double*)&operand_stack[operand_stack_top - 1] = *(double*)(operand_stack[operand_stack_top - 1] + *(int32_t*)&operand_stack[operand_stack_top]);
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_AALOAD:
        pc++;
        continue;
        
        // TODO
        operand_stack[operand_stack_top - 1] = *(int64_t*)(operand_stack[operand_stack_top - 1] + *(int32_t*)&operand_stack[operand_stack_top]);
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_BALOAD:
        pc++;
        continue;
        
        // TODO
        *(int8_t*)&operand_stack[operand_stack_top - 1] = (int32_t)*(int8_t*)(operand_stack[operand_stack_top - 1] + *(int32_t*)&operand_stack[operand_stack_top]);
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_CALOAD:
        // Not used
        assert(0);
      case SPerl_BYTECODE_C_CODE_SALOAD:
        pc++;
        continue;
        
        // TODO
        *(int16_t*)&operand_stack[operand_stack_top - 1] = (int32_t)*(int16_t*)(operand_stack[operand_stack_top - 1] + *(int32_t*)&operand_stack[operand_stack_top]);
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_ISTORE:
        *(int32_t*)&vars[bytecodes[pc + 1]] = *(int32_t*)&operand_stack[operand_stack_top];
        operand_stack_top--;
        pc += 2;
        continue;
      case SPerl_BYTECODE_C_CODE_LSTORE:
        vars[bytecodes[pc + 1]] = operand_stack[operand_stack_top];
        operand_stack_top--;
        pc += 2;
        continue;
      case SPerl_BYTECODE_C_CODE_FSTORE:
        *(int32_t*)&vars[bytecodes[pc + 1]] = *(int32_t*)&operand_stack[operand_stack_top];
        operand_stack_top--;
        pc += 2;
        continue;
      case SPerl_BYTECODE_C_CODE_DSTORE:
        vars[bytecodes[pc + 1]] = operand_stack[operand_stack_top];
        operand_stack_top--;
        pc += 2;
        continue;
      case SPerl_BYTECODE_C_CODE_ASTORE:
        vars[bytecodes[pc + 1]] = operand_stack[operand_stack_top];
        operand_stack_top--;
        pc += 2;
        continue;
      case SPerl_BYTECODE_C_CODE_ISTORE_0:
        *(int32_t*)&vars[0] = *(int32_t*)&operand_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_ISTORE_1:
        *(int32_t*)&vars[1] = *(int32_t*)&operand_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_ISTORE_2:
        *(int32_t*)&vars[2] = *(int32_t*)&operand_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_ISTORE_3:
        *(int32_t*)&vars[3] = *(int32_t*)&operand_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_LSTORE_0:
        vars[0] = operand_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_LSTORE_1:
        vars[1] = operand_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_LSTORE_2:
        vars[2] = operand_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_LSTORE_3:
        vars[3] = operand_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_FSTORE_0:
        *(int32_t*)&vars[0] = *(int32_t*)&operand_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_FSTORE_1:
        *(int32_t*)&vars[1] = *(int32_t*)&operand_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_FSTORE_2:
        *(int32_t*)&vars[2] = *(int32_t*)&operand_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_FSTORE_3:
        *(int32_t*)&vars[3] = *(int32_t*)&operand_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_DSTORE_0:
        vars[0] = operand_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_DSTORE_1:
        vars[1] = operand_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_DSTORE_2:
        vars[2] = operand_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_DSTORE_3:
        vars[3] = operand_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_ASTORE_0:
        vars[0] = operand_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_ASTORE_1:
        vars[1] = operand_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_ASTORE_2:
        vars[2] = operand_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_ASTORE_3:
        vars[3] = operand_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_BASTORE:
        pc++;
        continue;
        
        // TODO
        *(int8_t*)(operand_stack[operand_stack_top - 2] + *(int32_t*)&operand_stack[operand_stack_top - 1])
          = (int8_t)*(int32_t*)&operand_stack[operand_stack_top];
        operand_stack_top -= 3;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_CASTORE:
        // Not used
        assert(0);
      case SPerl_BYTECODE_C_CODE_SASTORE:
        pc++;
        continue;
        
        // TODO
        *(int16_t*)(operand_stack[operand_stack_top - 2] + *(int32_t*)&operand_stack[operand_stack_top - 1])
          = (int16_t)*(int32_t*)&operand_stack[operand_stack_top];
        operand_stack_top -= 3;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_IASTORE:
        pc++;
        continue;
        
        // TODO
        *(int32_t*)(operand_stack[operand_stack_top - 2] + *(int32_t*)&operand_stack[operand_stack_top - 1])
          = *(int32_t*)&operand_stack[operand_stack_top];
        operand_stack_top -= 3;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_LASTORE:
        pc++;
        continue;

        // TODO
        *(int64_t*)(operand_stack[operand_stack_top - 2] + *(int32_t*)&operand_stack[operand_stack_top - 1])
          = operand_stack[operand_stack_top];
        operand_stack_top -= 3;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_FASTORE:
        pc++;
        continue;

        // TODO
        *(float*)(operand_stack[operand_stack_top - 2] + *(int32_t*)&operand_stack[operand_stack_top - 1])
          = *(float*)&operand_stack[operand_stack_top];
        operand_stack_top -= 3;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_DASTORE:
        pc++;
        continue;

        // TODO
        *(double*)(operand_stack[operand_stack_top - 2] + *(int32_t*)&operand_stack[operand_stack_top - 1])
          = *(double*)&operand_stack[operand_stack_top];
        operand_stack_top -= 3;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_AASTORE:
        pc++;
        continue;

        // TODO
        *(int64_t*)(operand_stack[operand_stack_top - 2] + *(int32_t*)&operand_stack[operand_stack_top - 1])
          = operand_stack[operand_stack_top];
        operand_stack_top -= 3;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_POP:
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_POP2:
        operand_stack_top -= 2;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_DUP:
        // Not used
        assert(0);
      case SPerl_BYTECODE_C_CODE_DUP_X1:
        // Not used
        assert(0);
      case SPerl_BYTECODE_C_CODE_DUP_X2:
        // Not used
        assert(0);
      case SPerl_BYTECODE_C_CODE_DUP2:
        // Not used
        assert(0);
      case SPerl_BYTECODE_C_CODE_DUP2_X1:
        // Not used
        assert(0);
      case SPerl_BYTECODE_C_CODE_DUP2_X2:
        // Not used
        assert(0);
      case SPerl_BYTECODE_C_CODE_SWAP:
        // Not used
        assert(0);
      case SPerl_BYTECODE_C_CODE_IADD:
        
        *(int32_t*)&operand_stack[operand_stack_top - 1] += *(int32_t*)&operand_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_LADD:
        operand_stack[operand_stack_top - 1] += operand_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_FADD:
        *((float*)&operand_stack[operand_stack_top -1]) += *(float*)&operand_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_DADD:
        *((double*)&operand_stack[operand_stack_top - 1]) += *(double*)&operand_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_ISUB:
        *(int32_t*)&operand_stack[operand_stack_top - 1] -= *(int32_t*)&operand_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_LSUB:
        operand_stack[operand_stack_top - 1] -= operand_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_FSUB:
        *((float*)&operand_stack[operand_stack_top - 1]) -= *(float*)&operand_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_DSUB:
        *((double*)&operand_stack[operand_stack_top - 1]) -= *(double*)&operand_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_IMUL:
        *(int32_t*)&operand_stack[operand_stack_top - 1] *= *(int32_t*)&operand_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_LMUL:
        operand_stack[operand_stack_top - 1] *= operand_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_FMUL:
        *(float*)&operand_stack[operand_stack_top - 1] *= *(float*)&operand_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_DMUL:
        *(double*)&operand_stack[operand_stack_top - 1] *= *(double*)&operand_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_IDIV:
        *(int32_t*)&operand_stack[operand_stack_top - 1] /= *(int32_t*)&operand_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_LDIV:
        operand_stack[operand_stack_top - 1] /= operand_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_FDIV:
        *(float*)&operand_stack[operand_stack_top - 1] /= *(float*)&operand_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_DDIV:
        *(double*)&operand_stack[operand_stack_top - 1] /= *(double*)&operand_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_IREM:
        *(int32_t*)&operand_stack[operand_stack_top - 1] = *(int32_t*)&operand_stack[operand_stack_top - 1] % *(int32_t*)&operand_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_LREM:
        // z = a - (a/b) * b;
        operand_stack[operand_stack_top - 1] = operand_stack[operand_stack_top - 1] % operand_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_FREM:
        *((float*)&operand_stack[operand_stack_top - 1]) = (float)fmod((double)*((float*)&operand_stack[operand_stack_top - 1]), (double)*(float*)&operand_stack[operand_stack_top]);
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_DREM:
        *((double*)&operand_stack[operand_stack_top - 1]) = fmod(*(double*)&operand_stack[operand_stack_top - 1], *(double*)&operand_stack[operand_stack_top]);
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_INEG:
        *(int32_t*)&operand_stack[operand_stack_top] = -*(int32_t*)&operand_stack[operand_stack_top];
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_LNEG:
        operand_stack[operand_stack_top] = -operand_stack[operand_stack_top];
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_FNEG:
        *((float*)&operand_stack[operand_stack_top]) = -*((float*)&operand_stack[operand_stack_top]);
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_DNEG:
        *((double*)&operand_stack[operand_stack_top]) = -*((double*)&operand_stack[operand_stack_top]);
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_ISHL:
        *(int32_t*)&operand_stack[operand_stack_top - 1] <<= *(int32_t*)&operand_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_LSHL:
        operand_stack[operand_stack_top - 1] <<= operand_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_ISHR:
        *(int32_t*)&operand_stack[operand_stack_top - 1] >>= *(int32_t*)&operand_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_LSHR:
        operand_stack[operand_stack_top - 1] >>= operand_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_IUSHR:
        *(int32_t*)&operand_stack[operand_stack_top - 1] = (*(int32_t*)&operand_stack[operand_stack_top - 1] >> *(int32_t*)&operand_stack[operand_stack_top]) & 0xFFFFFFFF;
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_LUSHR:
        operand_stack[operand_stack_top - 1] = (operand_stack[operand_stack_top - 1] >> operand_stack[operand_stack_top]) & 0xFFFFFFFFFFFFFFFF;
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_IAND:
        *(int32_t*)&operand_stack[operand_stack_top - 1] &= *(int32_t*)&operand_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_LAND:
        operand_stack[operand_stack_top - 1] &= operand_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_IOR:
        *(int32_t*)&operand_stack[operand_stack_top - 1] |= *(int32_t*)&operand_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_LOR:
        operand_stack[operand_stack_top - 1] |= operand_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_IXOR:
        *(int32_t*)&operand_stack[operand_stack_top - 1] ^= *(int32_t*)&operand_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_LXOR:
        operand_stack[operand_stack_top - 1] ^= operand_stack[operand_stack_top];
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_IINC:
        vars[bytecodes[pc + 1]] += bytecodes[pc + 2];
        pc += 3;
        continue;
      case SPerl_BYTECODE_C_CODE_I2L:
        operand_stack[operand_stack_top] = (int64_t)*(int32_t*)&operand_stack[operand_stack_top];
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_I2F:
        *((float*)&operand_stack[operand_stack_top]) = (float)*(int32_t*)&operand_stack[operand_stack_top];
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_I2D:
        *((double*)&operand_stack[operand_stack_top]) = (double)*(int32_t*)&operand_stack[operand_stack_top];
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_L2I:
        operand_stack[operand_stack_top] = (int32_t)operand_stack[operand_stack_top];
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_L2F:
        *((float*)&operand_stack[operand_stack_top]) = (float)*((int64_t*)&operand_stack[operand_stack_top]);
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_L2D:
        *((double*)&operand_stack[operand_stack_top]) = (double)*((int64_t*)&operand_stack[operand_stack_top]);
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_F2I:
        *(int32_t*)&operand_stack[operand_stack_top] = (int32_t)*((float*)&operand_stack[operand_stack_top]);
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_F2L:
        operand_stack[operand_stack_top] = (int64_t)*((float*)&operand_stack[operand_stack_top]);
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_F2D:
        *((double*)&operand_stack[operand_stack_top]) = (double)*((float*)&operand_stack[operand_stack_top]);
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_D2I:
        *(int32_t*)&operand_stack[operand_stack_top] = (int32_t)*((double*)&operand_stack[operand_stack_top]);
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_D2L:
        operand_stack[operand_stack_top] = (int64_t)*((double*)&operand_stack[operand_stack_top]);
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_D2F:
        *((float*)&operand_stack[operand_stack_top]) = (float)*((double*)&operand_stack[operand_stack_top]);
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_I2B:
        *(int32_t*)&operand_stack[operand_stack_top] = *(int32_t*)&operand_stack[operand_stack_top] & 1;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_I2C:
        // Not used
        assert(0);
      case SPerl_BYTECODE_C_CODE_I2S:
        *(int32_t*)&operand_stack[operand_stack_top] = *(int32_t*)&operand_stack[operand_stack_top] & 0xF;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_LCMP:
        // z = (x > y) + (x < y) * -1
        *(int32_t*)&operand_stack[operand_stack_top - 1]
          = (operand_stack[operand_stack_top - 1] > operand_stack[operand_stack_top])
          + (operand_stack[operand_stack_top - 1] < operand_stack[operand_stack_top]) * -1;
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_FCMPL:
        *(int32_t*)&operand_stack[operand_stack_top - 1]
          = (*(float*)&operand_stack[operand_stack_top - 1] > *(float*)&operand_stack[operand_stack_top])
          + (*(float*)&operand_stack[operand_stack_top - 1] < *(float*)&operand_stack[operand_stack_top]) * -1
          + -!!(
            isnan(*(float*)&operand_stack[operand_stack_top - 1])
            | isnan(*(float*)&operand_stack[operand_stack_top])
          );
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_FCMPG:
        *(int32_t*)&operand_stack[operand_stack_top - 1]
          = (*(float*)&operand_stack[operand_stack_top - 1] > *(float*)&operand_stack[operand_stack_top])
          + (*(float*)&operand_stack[operand_stack_top - 1] < *(float*)&operand_stack[operand_stack_top]) * -1
          + !!(
            isnan(*(float*)&operand_stack[operand_stack_top - 1])
            | isnan(*(float*)&operand_stack[operand_stack_top])
          );
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_DCMPL:
        *(int32_t*)&operand_stack[operand_stack_top - 1]
          = (*(double*)&operand_stack[operand_stack_top - 1] > *(double*)&operand_stack[operand_stack_top])
          + (*(double*)&operand_stack[operand_stack_top - 1] < *(double*)&operand_stack[operand_stack_top]) * -1
          + -!!(
            isnan(*(double*)&operand_stack[operand_stack_top - 1])
            | isnan(*(double*)&operand_stack[operand_stack_top])
          );
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_DCMPG:
        *(int32_t*)&operand_stack[operand_stack_top - 1]
          = (*(double*)&operand_stack[operand_stack_top - 1] > *(double*)&operand_stack[operand_stack_top])
          + (*(double*)&operand_stack[operand_stack_top - 1] < *(double*)&operand_stack[operand_stack_top]) * -1
          + !!(
            isnan(*(double*)&operand_stack[operand_stack_top - 1])
            | isnan(*(double*)&operand_stack[operand_stack_top])
          );
        operand_stack_top--;
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_IFEQ:
        condition = *(int32_t*)&operand_stack[operand_stack_top] == 0;
        pc = (pc + (int16_t)(bytecodes[pc + 1] << 8) +  bytecodes[pc + 2]) * condition + (pc + 3) * !condition;
        operand_stack_top--;
        continue;
      case SPerl_BYTECODE_C_CODE_IFNE:
        condition = *(int32_t*)&operand_stack[operand_stack_top] != 0;
        pc = (pc + (int16_t)(bytecodes[pc + 1] << 8) +  bytecodes[pc + 2]) * condition + (pc + 3) * !condition;
        operand_stack_top--;
        continue;
      case SPerl_BYTECODE_C_CODE_IFLT:
        condition = *(int32_t*)&operand_stack[operand_stack_top] < 0;
        pc = (pc + (int16_t)(bytecodes[pc + 1] << 8) +  bytecodes[pc + 2]) * condition + (pc + 3) * !condition;
        operand_stack_top--;
        continue;
      case SPerl_BYTECODE_C_CODE_IFGE:
        condition = *(int32_t*)&operand_stack[operand_stack_top] >= 0;
        pc = (pc + (int16_t)(bytecodes[pc + 1] << 8) +  bytecodes[pc + 2]) * condition + (pc + 3) * !condition;
        operand_stack_top--;
        continue;
      case SPerl_BYTECODE_C_CODE_IFGT:
        condition = *(int32_t*)&operand_stack[operand_stack_top] > 0;
        pc = (pc + (int16_t)(bytecodes[pc + 1] << 8) +  bytecodes[pc + 2]) * condition + (pc + 3) * !condition;
        operand_stack_top--;
        continue;
      case SPerl_BYTECODE_C_CODE_IFLE:
        condition = *(int32_t*)&operand_stack[operand_stack_top] <= 0;
        pc = (pc + (int16_t)(bytecodes[pc + 1] << 8) +  bytecodes[pc + 2]) * condition + (pc + 3) * !condition;
        operand_stack_top--;
        continue;
      case SPerl_BYTECODE_C_CODE_IF_ICMPEQ:
        condition = *(int32_t*)&operand_stack[operand_stack_top - 1] == *(int32_t*)&operand_stack[operand_stack_top];
        pc = (pc + (int16_t)(bytecodes[pc + 1] << 8) +  bytecodes[pc + 2]) * condition + (pc + 3) * !condition;
        operand_stack_top -= 2;
        continue;
      case SPerl_BYTECODE_C_CODE_IF_ICMPNE:
        condition = *(int32_t*)&operand_stack[operand_stack_top - 1] != *(int32_t*)&operand_stack[operand_stack_top];
        pc = (pc + (int16_t)(bytecodes[pc + 1] << 8) +  bytecodes[pc + 2]) * condition + (pc + 3) * !condition;
        operand_stack_top -= 2;
        continue;
      case SPerl_BYTECODE_C_CODE_IF_ICMPLT:
        condition = *(int32_t*)&operand_stack[operand_stack_top - 1] < *(int32_t*)&operand_stack[operand_stack_top];
        pc = (pc + (int16_t)(bytecodes[pc + 1] << 8) +  bytecodes[pc + 2]) * condition + (pc + 3) * !condition;
        operand_stack_top -= 2;
        continue;
      case SPerl_BYTECODE_C_CODE_IF_ICMPGE:
        condition = *(int32_t*)&operand_stack[operand_stack_top - 1] >= *(int32_t*)&operand_stack[operand_stack_top];
        pc = (pc + (int16_t)(bytecodes[pc + 1] << 8) +  bytecodes[pc + 2]) * condition + (pc + 3) * !condition;
        operand_stack_top -= 2;
        continue;
      case SPerl_BYTECODE_C_CODE_IF_ICMPGT:
        condition = *(int32_t*)&operand_stack[operand_stack_top - 1] > *(int32_t*)&operand_stack[operand_stack_top];
        pc = (pc + (int16_t)(bytecodes[pc + 1] << 8) +  bytecodes[pc + 2]) * condition + (pc + 3) * !condition;
        operand_stack_top -= 2;
        continue;
      case SPerl_BYTECODE_C_CODE_IF_ICMPLE:
        condition = *(int32_t*)&operand_stack[operand_stack_top - 1] <= *(int32_t*)&operand_stack[operand_stack_top];
        pc = (pc + (int16_t)(bytecodes[pc + 1] << 8) +  bytecodes[pc + 2]) * condition + (pc + 3) * !condition;
        operand_stack_top -= 2;
        continue;
      case SPerl_BYTECODE_C_CODE_IF_ACMPEQ:
        condition = operand_stack[operand_stack_top - 1] == operand_stack[operand_stack_top];
        pc = (pc + (int16_t)(bytecodes[pc + 1] << 8) +  bytecodes[pc + 2]) * condition + (pc + 3) * !condition;
        operand_stack_top -= 2;
        continue;
      case SPerl_BYTECODE_C_CODE_IF_ACMPNE:
        condition = operand_stack[operand_stack_top - 1] != operand_stack[operand_stack_top];
        pc = (pc + (int16_t)(bytecodes[pc + 1] << 8) +  bytecodes[pc + 2]) * condition + (pc + 3) * !condition;
        operand_stack_top -= 2;
        continue;
      case SPerl_BYTECODE_C_CODE_GOTO:
        pc += (int16_t)((bytecodes[pc + 1] << 8) +  bytecodes[pc + 2]);
        continue;
      case SPerl_BYTECODE_C_CODE_JSR:
        // Not used
        assert(0);
      case SPerl_BYTECODE_C_CODE_RET:
        // Not used
        assert(0);
      case SPerl_BYTECODE_C_CODE_TABLESWITCH: {
        
        // Padding
        int32_t padding = 3 - (pc & 3);

        // default offset
        int32_t default_offset
          = (bytecodes[pc + padding + 1] << 24)
          + (bytecodes[pc + padding + 2] << 16)
          + (bytecodes[pc + padding + 3] << 8)
          + bytecodes[pc + padding + 4];
        
        /*
        // min
        int32_t min
          = (bytecodes[pc + padding + 5] << 24)
          + (bytecodes[pc + padding + 6] << 16)
          + (bytecodes[pc + padding + 7] << 8)
          + bytecodes[pc + padding + 8];
        
        // max
        int32_t max
          = (bytecodes[pc + padding + 9] << 24)
          + (bytecodes[pc + padding + 10] << 16)
          + (bytecodes[pc + padding + 11] << 8)
          + bytecodes[pc + padding + 12];
        */
        
        pc += default_offset;
        
        continue;
      }
      case SPerl_BYTECODE_C_CODE_LOOKUPSWITCH: {
        
        // Padding
        int32_t padding = 3 - (pc & 3);

        // default offset
        int32_t default_offset
          = (bytecodes[pc + padding + 1] << 24)
          + (bytecodes[pc + padding + 2] << 16)
          + (bytecodes[pc + padding + 3] << 8)
          + bytecodes[pc + padding + 4];
        
        /*
        // npare
        int32_t pair_count
          = (bytecodes[pc + padding + 5] << 24)
          + (bytecodes[pc + padding + 6] << 16)
          + (bytecodes[pc + padding + 7] << 8)
          + bytecodes[pc + padding + 8];
        */
        
        pc += default_offset;
        
        continue;
      }
      case SPerl_BYTECODE_C_CODE_IRETURN: {
        
        // Return value
        int32_t return_value = *(int32_t*)&operand_stack[operand_stack_top];
        
        // Return address
        int32_t return_address = vm->frame->return_address;
        
        // Restore frame
        frame_stack_top--;
        if (frame_stack_top == -1) {
          *(int32_t*)&vm->call_stack[0] = return_value;
          return;
        }
        
        vm->frame = &frame_stack[frame_stack_top];
        
        // Restore operand stack
        operand_stack = &vm->call_stack[vm->frame->operand_stack_base];
        
        // Restore vars
        vars = &vm->call_stack[vm->frame->vars_base];
        
        // Restore operand stack top
        operand_stack_top = vm->frame->operand_stack_top;
        
        // Push return value
        operand_stack_top++;
        *(int32_t*)&operand_stack[operand_stack_top] = return_value;
        
        pc = return_address;
        
        continue;
      }
      case SPerl_BYTECODE_C_CODE_LRETURN: {
        // Return value
        int64_t return_value = operand_stack[operand_stack_top];
        
        // Return address
        int32_t return_address = vm->frame->return_address;
        
        // Restore frame
        frame_stack_top--;
        if (frame_stack_top == -1) {
          *(int32_t*)&vm->call_stack[0] = return_value;
          return;
        }
        
        vm->frame = &frame_stack[frame_stack_top];
        
        // Restore operand stack
        operand_stack = &vm->call_stack[vm->frame->operand_stack_base];
        
        // Restore vars
        vars = &vm->call_stack[vm->frame->vars_base];
        
        // Restore operand stack top
        operand_stack_top = vm->frame->operand_stack_top;
        
        // Push return value
        operand_stack_top++;
        operand_stack[operand_stack_top] = return_value;
        
        pc = return_address;
        
        continue;
      }
      case SPerl_BYTECODE_C_CODE_FRETURN: {
        // Return value
        float return_value = *(float*)&operand_stack[operand_stack_top];
        
        // Return address
        int32_t return_address = vm->frame->return_address;
        
        // Restore frame
        frame_stack_top--;
        if (frame_stack_top == -1) {
          *(int32_t*)&vm->call_stack[0] = return_value;
          return;
        }
        
        vm->frame = &frame_stack[frame_stack_top];
        
        // Restore operand stack
        operand_stack = &vm->call_stack[vm->frame->operand_stack_base];
        
        // Restore vars
        vars = &vm->call_stack[vm->frame->vars_base];
        
        // Restore operand stack top
        operand_stack_top = vm->frame->operand_stack_top;
        
        // Push return value
        operand_stack_top++;
        *(float*)&operand_stack[operand_stack_top] = return_value;
        
        pc = return_address;
        
        continue;
      }
      case SPerl_BYTECODE_C_CODE_DRETURN: {
        // Return value
        double return_value = *(double*)&operand_stack[operand_stack_top];
        
        // Return address
        int32_t return_address = vm->frame->return_address;
        
        // Restore frame
        frame_stack_top--;
        if (frame_stack_top == -1) {
          *(int32_t*)&vm->call_stack[0] = return_value;
          return;
        }
        
        vm->frame = &frame_stack[frame_stack_top];
        
        // Restore operand stack
        operand_stack = &vm->call_stack[vm->frame->operand_stack_base];
        
        // Restore vars
        vars = &vm->call_stack[vm->frame->vars_base];
        
        // Restore operand stack top
        operand_stack_top = vm->frame->operand_stack_top;
        
        // Push return value
        operand_stack_top++;
        *(double*)&operand_stack[operand_stack_top] = return_value;
        
        pc = return_address;
        
        continue;
      }
      case SPerl_BYTECODE_C_CODE_ARETURN: {
        // Return value
        int64_t return_value = operand_stack[operand_stack_top];
        
        // Return address
        int32_t return_address = vm->frame->return_address;
        
        // Restore frame
        frame_stack_top--;
        if (frame_stack_top == -1) {
          *(int32_t*)&vm->call_stack[0] = return_value;
          return;
        }
        
        vm->frame = &frame_stack[frame_stack_top];
        
        // Restore operand stack
        operand_stack = &vm->call_stack[vm->frame->operand_stack_base];
        
        // Restore vars
        vars = &vm->call_stack[vm->frame->vars_base];
        
        // Restore operand stack top
        operand_stack_top = vm->frame->operand_stack_top;
        
        // Push return value
        operand_stack_top++;
        operand_stack[operand_stack_top] = return_value;
        
        pc = return_address;
        
        continue;
      }
      case SPerl_BYTECODE_C_CODE_RETURN: {
        
        // Return address
        int32_t return_address = vm->frame->return_address;
        
        // Restore frame
        frame_stack_top--;
        if (frame_stack_top == -1) {
          return;
        }
        
        vm->frame = &frame_stack[frame_stack_top];
        
        // Restore operand stack
        operand_stack = &vm->call_stack[vm->frame->operand_stack_base];
        
        // Restore vars
        vars = &vm->call_stack[vm->frame->vars_base];
        
        // Restore operand stack top
        operand_stack_top = vm->frame->operand_stack_top;
        
        pc = return_address;
        
        continue;
      }
      case SPerl_BYTECODE_C_CODE_GETSTATIC:
        // Not used
        assert(0);
      case SPerl_BYTECODE_C_CODE_PUTSTATIC:
        // Not used
        assert(0);
      case SPerl_BYTECODE_C_CODE_GETFIELD:
        pc += 3;
        continue;
      case SPerl_BYTECODE_C_CODE_PUTFIELD:
        pc += 3;
        continue;
      case SPerl_BYTECODE_C_CODE_INVOKEVIRTUAL:
        // Not used
        assert(0);
      case SPerl_BYTECODE_C_CODE_INVOKESPECIAL:
        // Not used
        assert(0);
      case SPerl_BYTECODE_C_CODE_INVOKESTATIC:
        // Not used
        assert(0);
      case SPerl_BYTECODE_C_CODE_INVOKEINTERFACE:
        // Not used
        assert(0);
      case SPerl_BYTECODE_C_CODE_INVOKEDYNAMIC:
        // Not used
        assert(0);
      case SPerl_BYTECODE_C_CODE_NEW:
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_NEWARRAY:
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_ANEWARRAY:
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_ARRAYLENGTH:
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_MULTIANEWARRAY:
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_ATHROW:
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_CHECKCAST:
        // Not used
        assert(0);
      case SPerl_BYTECODE_C_CODE_INSTANCEOF:
        // Not used
        assert(0);
      case SPerl_BYTECODE_C_CODE_MONITORENTER:
        // Not used
        pc++;
        continue;
      case SPerl_BYTECODE_C_CODE_MONITOREXIT:
        // Not used
        assert(0);
      case SPerl_BYTECODE_C_CODE_WIDE:
        // iload, fload, aload, lload, dload, istore, fstore, astore, lstore, dstore, or iinc
        
        if (bytecodes[pc + 1] == SPerl_BYTECODE_C_CODE_ILOAD) {
          operand_stack_top++;
          *(int32_t*)&operand_stack[operand_stack_top] = *(int32_t*)&vars[bytecodes[pc + 2] << 8 + bytecodes[pc + 3]];
          pc +=4;
        }
        else if (bytecodes[pc + 1] == SPerl_BYTECODE_C_CODE_LLOAD) {
          operand_stack_top++;
          operand_stack[operand_stack_top] = vars[bytecodes[pc + 2] << 8 + bytecodes[pc + 3]];
          pc +=4;
        }
        else if (bytecodes[pc + 1] == SPerl_BYTECODE_C_CODE_FLOAD) {
          operand_stack_top++;
          *(int32_t*)&operand_stack[operand_stack_top] = *(int32_t*)&vars[bytecodes[pc + 2] << 8 + bytecodes[pc + 3]];
          pc +=4;
        }
        else if (bytecodes[pc + 1] == SPerl_BYTECODE_C_CODE_DLOAD) {
          operand_stack_top++;
          operand_stack[operand_stack_top] = vars[bytecodes[pc + 2] << 8 + bytecodes[pc + 3]];
          pc +=4;
        }
        else if (bytecodes[pc + 1] == SPerl_BYTECODE_C_CODE_ALOAD) {
          operand_stack_top++;
          operand_stack[operand_stack_top] = vars[bytecodes[pc + 2] << 8 + bytecodes[pc + 3]];
          pc +=4;
        }
        else if (bytecodes[pc + 1] == SPerl_BYTECODE_C_CODE_ISTORE) {
          *(int32_t*)&vars[bytecodes[pc + 2] << 8 + bytecodes[pc + 3]] = *(int32_t*)&operand_stack[operand_stack_top];
          operand_stack_top--;
          pc +=4;
        }
        else if (bytecodes[pc + 1] == SPerl_BYTECODE_C_CODE_LSTORE) {
          vars[bytecodes[pc + 2] << 8 + bytecodes[pc + 3]] = operand_stack[operand_stack_top];
          operand_stack_top--;
          pc +=4;
        }
        else if (bytecodes[pc + 1] == SPerl_BYTECODE_C_CODE_FSTORE) {
          *(int32_t*)&vars[bytecodes[pc + 2] << 8 + bytecodes[pc + 3]] = *(int32_t*)&operand_stack[operand_stack_top];
          operand_stack_top--;
          pc +=4;
        }
        else if (bytecodes[pc + 1] == SPerl_BYTECODE_C_CODE_DSTORE) {
          vars[bytecodes[pc + 2] << 8 + bytecodes[pc + 3]] = operand_stack[operand_stack_top];
          operand_stack_top--;
          pc +=4;
        }
        else if (bytecodes[pc + 1] == SPerl_BYTECODE_C_CODE_ASTORE) {
          vars[bytecodes[pc + 2] << 8 + bytecodes[pc + 3]] = operand_stack[operand_stack_top];
          operand_stack_top--;
          pc +=4;
        }
        else if (bytecodes[pc + 1] == SPerl_BYTECODE_C_CODE_IINC) {
          vars[bytecodes[pc + 2] << 8 + bytecodes[pc + 3]] += bytecodes[pc + 4] << 8 + bytecodes[pc + 5];
          pc += 6;
        }
        continue;
      case SPerl_BYTECODE_C_CODE_IFNULL:
        pc += 3;
        continue;
      case SPerl_BYTECODE_C_CODE_IFNONNULL:
        pc += 3;
        continue;
      case SPerl_BYTECODE_C_CODE_GOTO_W:
        // Not used
        assert(0);
      case SPerl_BYTECODE_C_CODE_JSR_W:
        // Not used
        assert(0);
      case SPerl_BYTECODE_C_CODE_BREAKPOINT:
        // Not used
        assert(0);
      case SPerl_BYTECODE_C_CODE_CALLSUB:
      {
        // Get subroutine ID
        int32_t sub_id = (bytecodes[pc + 1] << 24) + (bytecodes[pc + 2] << 16) + (bytecodes[pc + 3] << 8) + bytecodes[pc + 4];
        
        // Subroutine
        SPerl_OP* op_sub = SPerl_ARRAY_fetch(parser->op_subs, sub_id);
        sub = op_sub->uv.sub;
        
        // Extend call stack(current size + lexical variable area + operand_stack area)
        int32_t call_stack_max = (vm->frame->operand_stack_base + vm->frame->operand_stack_top)
          + sub->my_vars_size + sub->operand_stack_max;
        
        while (call_stack_max > vm->call_stack_capacity) {
          vm->call_stack_capacity = vm->call_stack_capacity * 2;
          vm->call_stack = realloc(vm->call_stack, sizeof(int64_t) * vm->call_stack_capacity);
        }
        
        operand_stack_top -= sub->op_args->length;
        
        int32_t next_vars_base;
        
        CALLSUB_COMMON:
        
        next_vars_base = vm->frame->operand_stack_base + operand_stack_top + 1;
        
        // Initialize my variables and prepare arguments
        memset(&vm->call_stack[next_vars_base + sub->op_args->length], 0, (sub->op_my_vars->length - sub->op_args->length) * 8);
        
        // Save current operand stack top
        vm->frame->operand_stack_top = operand_stack_top;
        
        frame_stack_top++;
        vm->frame = &frame_stack[frame_stack_top];
       
        // Return address
        vm->frame->return_address = pc + 5;
        
        // Set new frame variables base
        vm->frame->vars_base = next_vars_base;
        
        // Set new frame operand stack base
        vm->frame->operand_stack_base = next_vars_base + sub->op_my_vars->length;
        
        // Initialize new frame operand stack top
        vm->frame->operand_stack_top = -1;
        
        // Set operand stack to local variable
        operand_stack = &vm->call_stack[vm->frame->operand_stack_base];
        
        // Set variables to local variable
        vars = &vm->call_stack[vm->frame->vars_base];
        
        // Call native subroutine
        if (sub->is_native) {
          vm->frame->operand_stack_top++;
          operand_stack_top = vm->frame->operand_stack_top;
          
          (*sub->native_address)(vm);
          
          // Return value
          int64_t return_value = operand_stack[operand_stack_top];
          
          // Restore frame
          frame_stack_top--;
          vm->frame = &frame_stack[frame_stack_top];
          
          // Restore operand stack
          operand_stack = &vm->call_stack[vm->frame->operand_stack_base];
          
          // Restore vars
          vars = &vm->call_stack[vm->frame->vars_base];
          
          // Restore operand stack top
          operand_stack_top = vm->frame->operand_stack_top;
          
          // Push return value
          operand_stack_top++;
          operand_stack[operand_stack_top] = return_value;
          
          pc += 5;
        }
        // Call normal subroutine
        else {
          pc = sub->bytecode_base;
        }
        continue;
      }
      case SPerl_BYTECODE_C_CODE_LOADCONST:
        operand_stack_top++;
        *(int32_t*)&operand_stack[operand_stack_top]
          = constant_pool[(bytecodes[pc + 1] << 24) + (bytecodes[pc + 2] << 16) + (bytecodes[pc + 3] << 8) + bytecodes[pc + 4]];
        pc += 5;
        continue;
      case SPerl_BYTECODE_C_CODE_LOADCONST2:
        operand_stack_top++;
        operand_stack[operand_stack_top]
          = *(int64_t*)&constant_pool[(bytecodes[pc + 1] << 24) + (bytecodes[pc + 2] << 16) + (bytecodes[pc + 3] << 8) + bytecodes[pc + 4]];
        pc += 5;
        continue;
    }
    
    assert(0);
  }
}
