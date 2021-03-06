#ifndef SPERL_OP_H
#define SPERL_OP_H

#include "sperl_base.h"











































/* Operation code */
enum {                          // [GROUP]
  SPerl_OP_C_CODE_IF,           // UNKNOWN
  SPerl_OP_C_CODE_ELSIF,        // UNKNOWN
  SPerl_OP_C_CODE_ELSE,         // UNKNOWN
  SPerl_OP_C_CODE_FOR,          // UNKNOWN
  SPerl_OP_C_CODE_WHILE,        // UNKNOWN
  SPerl_OP_C_CODE_NULL,         // UNKNOWN
  SPerl_OP_C_CODE_LIST,         // UNKNOWN
  SPerl_OP_C_CODE_PUSHMARK,     // UNKNOWN
  SPerl_OP_C_CODE_GRAMMAR,      // UNKNOWN
  SPerl_OP_C_CODE_NAME,         // UNKNOWN
  SPerl_OP_C_CODE_DECL_PACKAGE,      // UNKNOWN
  SPerl_OP_C_CODE_DECL_MY_VAR,   // UNKNOWN
  SPerl_OP_C_CODE_DECL_FIELD,   // UNKNOWN
  SPerl_OP_C_CODE_DECL_SUB,          // UNKNOWN
  SPerl_OP_C_CODE_DECL_ENUM,
  SPerl_OP_C_CODE_DECL_DESCRIPTOR,   // UNKNOWN
  SPerl_OP_C_CODE_DECL_ANON_SUB,      // UNKNOWN
  SPerl_OP_C_CODE_DECL_ENUMERATION_VALUE,    // UNKNOWN
  SPerl_OP_C_CODE_BLOCK,        // UNKNOWN
  SPerl_OP_C_CODE_ENUM_BLOCK,    // UNKNOWN
  SPerl_OP_C_CODE_CLASS_BLOCK,   // UNKNOWN
  SPerl_OP_C_CODE_TYPE,         // UNKNOWN
  SPerl_OP_C_CODE_DESCRIPTOR_ENUM,         // UNKNOWN
  SPerl_OP_C_CODE_CONSTANT,     // CONST
  SPerl_OP_C_CODE_PRE_INC,       // INTDEC
  SPerl_OP_C_CODE_POST_INC,      // INTDEC
  SPerl_OP_C_CODE_PRE_DEC,       // INTDEC
  SPerl_OP_C_CODE_POST_DEC,      // INTDEC
  SPerl_OP_C_CODE_BIT_NOT,       // UNOP
  SPerl_OP_C_CODE_COMPLEMENT,   // UNOP
  SPerl_OP_C_CODE_NEGATE,       // UNOP
  SPerl_OP_C_CODE_PLUS,         // UNOP
  SPerl_OP_C_CODE_EQ,           // BINOP
  SPerl_OP_C_CODE_NE,           // BINOP
  SPerl_OP_C_CODE_LT,           // BINOP
  SPerl_OP_C_CODE_LE,           // BINOP
  SPerl_OP_C_CODE_GT,           // BINOP
  SPerl_OP_C_CODE_GE,           // BINOP
  SPerl_OP_C_CODE_ADD,          // BINOP
  SPerl_OP_C_CODE_SUBTRACT,     // BINOP
  SPerl_OP_C_CODE_MULTIPLY,     // BINOP
  SPerl_OP_C_CODE_DIVIDE,       // BINOP
  SPerl_OP_C_CODE_BIT_AND,       // BINOP
  SPerl_OP_C_CODE_BIT_OR,        // BINOP
  SPerl_OP_C_CODE_REMAINDER,       // BINOP
  SPerl_OP_C_CODE_BIT_XOR,       // BINOP
  SPerl_OP_C_CODE_LEFT_SHIFT,    // BINOP
  SPerl_OP_C_CODE_RIGHT_SHIFT,   // BINOP
  SPerl_OP_C_CODE_RIGHT_SHIFT_UNSIGNED,   // BINOP
  SPerl_OP_C_CODE_AND,          // LOGICALOP
  SPerl_OP_C_CODE_OR,           // LOGICALOP
  SPerl_OP_C_CODE_NOT,          // LOGICALOP
  SPerl_OP_C_CODE_ARRAY_ELEM,        // UNKNOWN
  SPerl_OP_C_CODE_FIELD,        // UNKNOWN
  SPerl_OP_C_CODE_ASSIGN,       // ASSIGN
  SPerl_OP_C_CODE_CALL_SUB,      // UNKNOWN
  SPerl_OP_C_CODE_USE,          // UNKNOWN
  SPerl_OP_C_CODE_RETURN,       // UNKNOWN
  SPerl_OP_C_CODE_LAST,         // UNKNOWN
  SPerl_OP_C_CODE_NEXT,         // UNKNOWN
  SPerl_OP_C_CODE_LOOP,         // UNKNOWN
  SPerl_OP_C_CODE_VAR,          // UNKNOWN
  SPerl_OP_C_CODE_CONVERT,  // UNKNOWN
  SPerl_OP_C_CODE_POP,          // UNKNOWN
  SPerl_OP_C_CODE_NEW_ARRAY,
  SPerl_OP_C_CODE_UNDEF,
  SPerl_OP_C_CODE_NEW,
  SPerl_OP_C_CODE_NEW_TYPE,
  SPerl_OP_C_CODE_NEW_ARRAY_CONSTANT,
  SPerl_OP_C_CODE_ARRAY_LENGTH,
  SPerl_OP_C_CODE_CONDITION,
  SPerl_OP_C_CODE_DIE,
  SPerl_OP_C_CODE_SWITCH,
  SPerl_OP_C_CODE_CASE,
  SPerl_OP_C_CODE_DEFAULT,
  SPerl_OP_C_CODE_SWITCH_CONDITION,
  SPerl_OP_C_CODE_DESCRIPTOR,   // UNKNOWN
  SPerl_OP_C_CODE_VOID,
};

extern const char* const SPerl_OP_C_CODE_NAMES[];

enum {
  // Block flag
  SPerl_OP_C_FLAG_BLOCK_IF = 1,
  SPerl_OP_C_FLAG_BLOCK_ELSE = 2,
  SPerl_OP_C_FLAG_BLOCK_LOOP = 4,
  SPerl_OP_C_FLAG_BLOCK_SWITCH = 8,
  SPerl_OP_C_FLAG_BLOCK_HAS_ELSE = 16,
};

enum {
  // Condition flag
  SPerl_OP_C_FLAG_CONDITION_IF = 1,
  SPerl_OP_C_FLAG_CONDITION_LOOP = 2,
};

enum {
  // Case flag
  SPerl_OP_C_FLAG_CONSTANT_CASE = 1,
};

/* Binary operation */
struct SPerl_op {
  SPerl_OP* first;
  SPerl_OP* last;
  SPerl_OP* sibparent;
  const char* file;
  union {
    const char* name;
    SPerl_MY_VAR* my_var;
    SPerl_SUB* sub;
    SPerl_CONSTANT* constant;
    SPerl_NAME_INFO* name_info;
    SPerl_TYPE* type;
    SPerl_USE* use;
    SPerl_VAR* var;
    SPerl_FIELD* field;
    SPerl_PACKAGE* package;
    SPerl_ENUMERATION* enumeration;
    SPerl_SWITCH_INFO* switch_info;
  } uv;
  int32_t code;
  int32_t flag;
  int32_t line;
  _Bool moresib;
  _Bool lvalue;
  _Bool condition;
};

void SPerl_OP_convert_to_op_constant_true(SPerl* sperl, SPerl_OP* op);
void SPerl_OP_convert_to_op_constant_false(SPerl* sperl, SPerl_OP* op);

void SPerl_OP_convert_not_to_if(SPerl* sperl, SPerl_OP* op);
void SPerl_OP_convert_and_to_if(SPerl* sperl, SPerl_OP* op);
void SPerl_OP_convert_or_to_if(SPerl* sperl, SPerl_OP* op);

SPerl_OP* SPerl_OP_build_switch_statement(SPerl* sperl, SPerl_OP* op_switch, SPerl_OP* op_term, SPerl_OP* op_block);
SPerl_OP* SPerl_OP_build_case_statement(SPerl* sperl, SPerl_OP* op_case, SPerl_OP* op_term);

SPerl_OP* SPerl_OP_build_logical_op(SPerl* sperl, SPerl_OP* op_logical_op, SPerl_OP* op_first, SPerl_OP* op_last);

SPerl_OP* SPerl_OP_build_for_statement(SPerl* sperl, SPerl_OP* op_for, SPerl_OP* op_term_loop_var, SPerl_OP* op_term_condition, SPerl_OP* op_term_next_value, SPerl_OP* op_block);

SPerl_OP* SPerl_OP_build_while_statement(SPerl* sperl, SPerl_OP* op_while, SPerl_OP* op_term_condition, SPerl_OP* op_block);


SPerl_OP* SPerl_OP_build_if_statement(SPerl* sperl, SPerl_OP* op_if, SPerl_OP* op_term, SPerl_OP* op_block, SPerl_OP* op_else_statement);

SPerl_OP* SPerl_OP_build_array_length(SPerl* sperl, SPerl_OP* op_array_length, SPerl_OP* op_term);

SPerl_OP* SPerl_OP_build_new_object(SPerl* sperl, SPerl_OP* op_new, SPerl_OP* op_type);

SPerl_OP* SPerl_OP_build_array_init(SPerl* sperl, SPerl_OP* op_opt_terms);
void SPerl_OP_resolve_type(SPerl* sperl, SPerl_TYPE* type, int32_t name_length);
void SPerl_OP_check(SPerl* sperl);
void SPerl_OP_resolve_sub_name(SPerl* sperl, SPerl_OP* op_package, SPerl_OP* op_name);
void SPerl_OP_resolve_field_name(SPerl* sperl, SPerl_OP* op_name);

SPerl_RESOLVED_TYPE* SPerl_OP_get_resolved_type(SPerl* sperl, SPerl_OP* op);
SPerl_OP* SPerl_OP_build_call_op(SPerl* sperl, SPerl_OP* op_call_op, SPerl_OP* op_first, SPerl_OP* op_last);
void SPerl_OP_insert_op_convert(SPerl* sperl, SPerl_OP* op);

SPerl_OP* SPerl_OP_build_type_name(SPerl* sperl, SPerl_OP* op_type_name);
SPerl_OP* SPerl_OP_build_type_array(SPerl* sperl, SPerl_OP* op_type, SPerl_OP* op_term);
SPerl_OP* SPerl_OP_build_type_sub(SPerl* sperl, SPerl_OP* op_argument_types, SPerl_OP* op_return_type);

SPerl_OP* SPerl_OP_build_field(SPerl* sperl, SPerl_OP* op_packagename, SPerl_OP* op_fieldname);
SPerl_OP* SPerl_OP_build_decl_package(SPerl* sperl, SPerl_OP* op_package, SPerl_OP* op_package_name, SPerl_OP* op_block);
SPerl_OP* SPerl_OP_build_decl_sub(SPerl* sperl, SPerl_OP* op_sub, SPerl_OP* op_subname, SPerl_OP* op_subargs, SPerl_OP* op_descriptors, SPerl_OP* type, SPerl_OP* op_block);
SPerl_OP* SPerl_OP_build_CONSTVALUE(SPerl* sperl, SPerl_OP* op_const);
SPerl_OP* SPerl_OP_build_decl_field(SPerl* sperl, SPerl_OP* op_has, SPerl_OP* op_field_base_name, SPerl_OP* type);
SPerl_OP* SPerl_OP_build_decl_my(SPerl* sperl, SPerl_OP* op_my, SPerl_OP* op_var, SPerl_OP* op_type);
SPerl_OP* SPerl_OP_build_grammar(SPerl* sperl, SPerl_OP* op_packages);
SPerl_OP* SPerl_OP_build_decl_use(SPerl* sperl, SPerl_OP* op_use, SPerl_OP* op_package_name);
SPerl_OP* SPerl_OP_build_call_sub(SPerl* sperl, SPerl_OP* op_invocant, SPerl_OP* op_subname, SPerl_OP* op_terms, _Bool anon);
SPerl_OP* SPerl_OP_newOP_LIST(SPerl* sperl, const char* file, uint32_t line);
SPerl_OP* SPerl_OP_build_convert_type(SPerl* sperl, SPerl_OP* op_type, SPerl_OP* op_term);
void SPerl_OP_resolve_op_convert_type(SPerl* sperl, SPerl_OP* op_convert_type);

const char* SPerl_OP_create_abs_name(SPerl* sperl, const char* package_name, const char* base_name);

SPerl_OP* SPerl_OP_sibling_splice(SPerl* sperl, SPerl_OP* parent, SPerl_OP* start, int32_t del_count, SPerl_OP *insert);

SPerl_OP* SPerl_OP_sibling(SPerl* sperl, SPerl_OP* o);
void SPerl_OP_moresib_set(SPerl* sperl, SPerl_OP* o, SPerl_OP* sib);
void SPerl_OP_lastsib_set(SPerl* sperl, SPerl_OP* o, SPerl_OP* parent);
void SPerl_OP_maybesib_set(SPerl* sperl, SPerl_OP* o, SPerl_OP* sib, SPerl_OP* parent);

SPerl_OP* SPerl_OP_build_decl_enum(SPerl* sperl, SPerl_OP* op_enum, SPerl_OP* op_enum_block);

SPerl_OP* SPerl_OP_newOP(SPerl* sperl, int32_t code, const char* file, uint32_t line);

SPerl_OP* SPerl_OP_append_elem(SPerl* sperl, SPerl_OP *first, SPerl_OP *last, const char* file, uint32_t line);

#endif
