#include "sperl_type.h"
#include <string.h>
#include <stdlib.h>

#include "sperl_array.h"

SPerl_ARRAY* SPerl_ARRAY_new(SPerl_int capacity) {
  
  SPerl_ARRAY* array = (SPerl_ARRAY*)malloc(sizeof(SPerl_ARRAY));
  array->length = 0;
  
  if (capacity == 0) {
    array->capacity = 128;
  }
  else {
    array->capacity = capacity;
  }
  
  SPerl_int total_byte = sizeof(void*) * array->capacity;
  void** values = (void**)malloc(total_byte);
  memset(values, 0, total_byte);
  
  array->values = values;
  
  return array;
}

void SPerl_ARRAY_push(SPerl_ARRAY* array, void* value) {
  SPerl_int length = array->length;
  SPerl_int capacity = array->capacity;
  
  if (length >= capacity) {
    SPerl_int new_capacity = capacity * 2;
    array->values = realloc(array->values, new_capacity);
    memset(array->values + capacity, 0, new_capacity - capacity);
    array->capacity = new_capacity;
  }
  
  array->values[length] = value;
  array->length++;
}

void* SPerl_ARRAY_fetch(SPerl_ARRAY* array, SPerl_int index) {
  if (array == NULL || index < 0 || index >= array->length) {
    return NULL;
  }
  else {
    return array->values[index];
  }
}
