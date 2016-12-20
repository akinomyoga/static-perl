#include <stdlib.h>
#include <string.h>

#include "sperl_type.h"
#include "sperl_type_component_word.h"
#include "sperl_type_component_array.h"
#include "sperl_type_component_sub.h"
#include "sperl_parser.h"
#include "sperl_word.h"
#include "sperl_array.h"
#include "sperl_type_part.h"
#include "sperl_op.h"
#include "sperl_allocator.h"

uint8_t* const SPerl_TYPE_C_CODE_NAMES[] = {
  "word",
  "array",
  "sub",
};

SPerl_TYPE* SPerl_TYPE_new(SPerl_PARSER* parser) {
  SPerl_TYPE* type = SPerl_ALLOCATOR_alloc_memory_pool(parser, sizeof(SPerl_TYPE));
  
  type->parts = SPerl_ALLOCATOR_new_array(parser, 0);
  
  return type;
}

void SPerl_TYPE_to_parts(SPerl_PARSER* parser, SPerl_TYPE* type, SPerl_ARRAY* parts) {
  
  if (type->code == SPerl_TYPE_C_CODE_WORD) {
    SPerl_TYPE_PART* part = SPerl_TYPE_PART_new(parser);
    part->code = SPerl_TYPE_PART_C_CODE_WORD;
    part->uv.op_name = type->uv.type_component_word->op_name;
    SPerl_ARRAY_push(parts, part);
  }
  else if (type->code == SPerl_TYPE_C_CODE_ARRAY) {
    SPerl_TYPE_COMPONENT_ARRAY* type_component_array = type->uv.type_component_array;
    
    SPerl_TYPE_to_parts(parser, type_component_array->type, parts);
    
    SPerl_TYPE_PART* type_part_openbracket = SPerl_TYPE_PART_new(parser);
    type_part_openbracket->code = SPerl_TYPE_PART_C_CODE_BYTE;
    type_part_openbracket->uv.char_name = "[";
    SPerl_ARRAY_push(parts, type_part_openbracket);
    
    SPerl_TYPE_PART* type_part_closebracket = SPerl_TYPE_PART_new(parser);
    type_part_closebracket->code = SPerl_TYPE_PART_C_CODE_BYTE;
    type_part_closebracket->uv.char_name = "]";
    SPerl_ARRAY_push(parts, type_part_closebracket);
  }
  else if (type->code == SPerl_TYPE_C_CODE_SUB) {
    // (
    SPerl_TYPE_PART* type_part_openparen1 = SPerl_TYPE_PART_new(parser);
    type_part_openparen1->code = SPerl_TYPE_PART_C_CODE_BYTE;
    type_part_openparen1->uv.char_name = "(";
    SPerl_ARRAY_push(parts, type_part_openparen1);
    
    // sub
    SPerl_TYPE_PART* type_part_sub = SPerl_TYPE_PART_new(parser);
    type_part_sub->code = SPerl_TYPE_PART_C_CODE_SUB;
    SPerl_ARRAY_push(parts, type_part_sub);
    
    // (
    SPerl_TYPE_PART* type_part_openparen2 = SPerl_TYPE_PART_new(parser);
    type_part_openparen2->code = SPerl_TYPE_PART_C_CODE_BYTE;
    type_part_openparen2->uv.char_name = "(";
    SPerl_ARRAY_push(parts, type_part_openparen2);
    
    // Argument types
    SPerl_TYPE_COMPONENT_SUB* type_component_sub = type->uv.type_component_sub;
    SPerl_ARRAY* argument_types = type_component_sub->argument_types;
    for (int32_t i = 0; i < argument_types->length; i++) {
      SPerl_TYPE* argument_type = SPerl_ARRAY_fetch(argument_types, i);
      SPerl_TYPE_to_parts(parser, argument_type, parts);
      if (i != argument_types->length - 1) {
        SPerl_TYPE_PART* type_part_comma = SPerl_TYPE_PART_new(parser);
        type_part_comma->code = SPerl_TYPE_PART_C_CODE_BYTE;
        type_part_comma->uv.char_name = ",";
        SPerl_ARRAY_push(parts, type_part_comma);
      }
    }
    
    // )
    SPerl_TYPE_PART* type_part_closeparen1 = SPerl_TYPE_PART_new(parser);
    type_part_closeparen1->code = SPerl_TYPE_PART_C_CODE_BYTE;
    type_part_closeparen1->uv.char_name = ")";
    SPerl_ARRAY_push(parts, type_part_closeparen1);
    
    // Return type
    SPerl_TYPE* return_type = type_component_sub->return_type;
    SPerl_TYPE_to_parts(parser, return_type, parts);
    
    // )
    SPerl_TYPE_PART* type_part_closeparen2 = SPerl_TYPE_PART_new(parser);
    type_part_closeparen2->code = SPerl_TYPE_PART_C_CODE_BYTE;
    type_part_closeparen2->uv.char_name = ")";
    SPerl_ARRAY_push(parts, type_part_closeparen2);
  }
}

void SPerl_TYPE_build_parts(SPerl_PARSER* parser, SPerl_TYPE* type) {
  SPerl_ARRAY* parts = SPerl_ALLOCATOR_new_array(parser, 0);
  SPerl_TYPE_to_parts(parser, type, parts);
  type->parts = parts;
}

void SPerl_TYPE_print(SPerl_PARSER* parser, SPerl_TYPE* type, FILE* fh) {
  SPerl_ARRAY* parts = type->parts;
  for (int32_t i = 0; i < parts->length; i++) {
    SPerl_TYPE_PART* part = SPerl_ARRAY_fetch(parts, i);
    uint8_t code = part->code;
    
    if (code == SPerl_TYPE_PART_C_CODE_BYTE) {
      fprintf(fh, "%s", part->uv.char_name);
    }
    else if (code == SPerl_TYPE_PART_C_CODE_WORD) {
      fprintf(fh, "%s", part->uv.op_name->uv.word->value);
    }
    else if (code == SPerl_TYPE_PART_C_CODE_SUB) {
      fprintf(fh, "sub");
    }
  }
}

void SPerl_TYPE_build_name(SPerl_PARSER* parser, SPerl_TYPE* type) {
  
  if (type->name) {
    return;
  }
  else {
    SPerl_ARRAY* parts = type->parts;
    int32_t total_length = 0;

    for (int32_t i = 0; i < parts->length; i++) {
      SPerl_TYPE_PART* part = SPerl_ARRAY_fetch(parts, i);
      uint8_t code = part->code;
      
      if (code == SPerl_TYPE_PART_C_CODE_BYTE) {
        total_length += 1;
      }
      else if (code == SPerl_TYPE_PART_C_CODE_WORD) {
        total_length += strlen(part->uv.op_name->uv.word->value);
      }
      else if (code == SPerl_TYPE_PART_C_CODE_SUB) {
        total_length += 3;
      }
    }
    
    uint8_t* type_name = SPerl_ALLOCATOR_new_string(parser, total_length);
    
    int32_t pos = 0;
    for (int32_t i = 0; i < parts->length; i++) {
      SPerl_TYPE_PART* part = SPerl_ARRAY_fetch(parts, i);
      uint8_t code = part->code;
      
      int32_t length = 0;
      if (code == SPerl_TYPE_PART_C_CODE_BYTE) {
        length = 1;
        memcpy(type_name + pos, part->uv.char_name, length);
      }
      else if (code == SPerl_TYPE_PART_C_CODE_WORD) {
        length = strlen(part->uv.op_name->uv.word->value);
        memcpy(type_name + pos, part->uv.op_name->uv.word->value, length);
      }
      else if (code == SPerl_TYPE_PART_C_CODE_SUB) {
        length = 3;
        memcpy(type_name + pos, "sub", length);
      }
      pos += length;
    }
    
    type->name = type_name;
  }
}
