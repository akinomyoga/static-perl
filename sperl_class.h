#ifndef SPERL_CLASS_H
#define SPERL_CLASS_H

#include <stdint.h>

// Field information
struct SPerl_field_info;
typedef struct SPerl_field_info SPerl_FIELD_INFO;
struct SPerl_field_info {
  int8_t* name;
  int16_t desc_flag;
  int8_t* type;
};

// Argument information
struct SPerl_argument_info;
typedef struct SPerl_argument_info SPerl_ARGUMENT_INFO;
struct SPerl_SIGNATURE_INFO {
  int16_t desc_flag;
  int8_t* type;
};

// Method information
struct SPerl_method_info;
typedef struct SPerl_method_info SPerl_METHOD_INFO;
struct SPerl_METHOD_INFO {
  int8_t* name;
  int16_t desc_flag;
  int8_t* return_type;
  SPerl_ARGUMENT_INFO* argument_infos;
  int32_t argument_count;
};

// Constant pool tag
enum SPerl_constant_tag {
  SPerl_CONSTANT_BYTE,
  SPerl_CONSTANT_SHORT,
  SPerl_CONSTANT_INT,
  SPerl_CONSTANT_LONG,
  SPerl_CONSTANT_FLOAT,
  SPerl_CONSTANT_STRING
};

// Constant pool
struct SPerl_constant_pool;
typedef struct SPerl_constant_pool SPerl_CONSTANT_POOL;
struct SPerl_constant_pool {
  int8_t tag;
  union {
    int8_t byte_data;
    int16_t short_data;
    int32_t int_data;
    int64_t long_data;
    float float_data;
    double double_data;
    int8_t* string_data;
  } uv;
};

// Class information
struct SPerl_class_info;
typedef struct SPerl_class_info SPerl_CLASS_INFO;
struct SPerl_class_info {
  int8_t* name;
  int16_t desc_flag;
  int8_t* super_class_name;
  SPerl_FIELD_INFO* field_infos;
  int32_t field_count;
  SPerl_METHOD_INFO* method_infos;
  int32_t method_count;
  SPerl_CONSTANT_POOL* constant_pools;
  int32_t constant_pool_count;
};

#endif
