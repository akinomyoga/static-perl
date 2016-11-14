#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "sperl_toke.h"
#include "sperl_yacc.h"
#include "sperl_yacc.tab.h"
#include "sperl_op.h"
#include "sperl_parser.h"
#include "sperl_const_value.h"
#include "sperl_var.h"
#include "sperl_array.h"
#include "sperl_hash.h"
#include "sperl_word.h"
#include "sperl_descripter.h"
#include "sperl_use.h"
#include "sperl_body_core.h"
#include "sperl_type.h"

static SPerl_OP* _newOP(SPerl_PARSER* parser, SPerl_char type) {
  SPerl_OP* op = SPerl_OP_newOP(parser, type, NULL, NULL);
  op->file = parser->cur_file;
  op->line = parser->cur_line;
  
  return op;
}

// Get token
int SPerl_yylex(SPerl_YYSTYPE* yylvalp, SPerl_PARSER* parser) {

  // Get expected and retrun back to normal
  SPerl_char expect = parser->expect;
  parser->expect = SPerl_TOKE_C_EXPECT_NORMAL;
  
  // Save buf pointer
  parser->befbufptr = parser->bufptr;
  
  while(1) {
    // Get current character
    SPerl_char c = *parser->bufptr;
    
    // line end
    switch(c) {
      case '\0':
        parser->cur_file = NULL;
        parser->cur_src = NULL;
        
        // If there are more module, load it
        SPerl_ARRAY* use_stack = parser->use_stack;
        
        
        while (1) {
          SPerl_USE* use = SPerl_ARRAY_pop(use_stack);
          if (use) {
            SPerl_char* package_name = use->package_name_word->value;

            SPerl_BODY_CLASS* found_package = SPerl_HASH_search(parser->package_symtable, package_name, strlen(package_name));
            if (found_package) {
              continue;
            }
            else {
              // Search class file
              SPerl_char* cur_file;
              FILE* fh;
              for (SPerl_int i = 0; i < parser->include_pathes->length; i++) {
                SPerl_char* include_path = SPerl_ARRAY_fetch(parser->include_pathes, i);
                
                // Change :: to /
                SPerl_char* package_name_for_path = SPerl_PARSER_new_string(parser, strlen(package_name));
                SPerl_char* bufptr_orig = package_name;
                SPerl_char* bufptr_to = package_name_for_path;
                while (*bufptr_orig) {
                  if (*bufptr_orig == ':' && *(bufptr_orig + 1) == ':') {
                    *bufptr_to = '/';
                    bufptr_orig += 2;
                    bufptr_to++;
                  }
                  else {
                    *bufptr_to = *bufptr_orig;
                    bufptr_orig++;
                    bufptr_to++;
                  }
                }
                *bufptr_orig = '\0';
                
                // File name
                cur_file = SPerl_PARSER_new_string(parser, strlen(include_path) + strlen(package_name_for_path) + 6);
                sprintf(cur_file, "%s/%s.spvm", include_path, package_name_for_path);
                
                // Open source file
                fh = fopen(cur_file, "r");
                if (fh) {
                  parser->cur_file = cur_file;
                  break;
                }
              }
              if (!fh) {
                if (use->op) {
                  fprintf(stderr, "Can't find package \"%s\" at %s line %d\n", use->package_name_word->value, use->op->file, use->op->line);
                }
                else {
                  fprintf(stderr, "Can't find file %s\n", cur_file);
                }
                exit(1);
              }
              
              parser->cur_file = cur_file;
              
              
              // Read file content
              fseek(fh, 0, SEEK_END);
              SPerl_int file_size = ftell(fh);
              fseek(fh, 0, SEEK_SET);
              SPerl_char* src = SPerl_PARSER_new_string(parser, file_size);
              if (fread(src, 1, file_size, fh) == -1) {
                if (use->op) {
                  fprintf(stderr, "Can't read file %s at %s line %d\n", cur_file, use->op->file, use->op->line);
                }
                else {
                  fprintf(stderr, "Can't read file %s\n", cur_file);
                }
                exit(1);
              }
              fclose(fh);
              src[file_size] = '\0';
              parser->cur_src = src;
              parser->bufptr = src;
              parser->befbufptr = src;
              parser->current_package_count = 0;
              parser->current_use_package_name = package_name;
              parser->cur_line = 1;
              break;
            }
          }
          else {
            return 0;
          }
        }
        if (parser->cur_src) {
          continue;
        }
        else {
          return 0;
        }
      
      /* Skip space character */
      case ' ':
      case '\t':
        parser->bufptr++;
        continue;

      case '\n':
        parser->bufptr++;
        parser->cur_line++;
        continue;
      
      /* Addition */
      case '+':
        parser->bufptr++;
        
        if (*parser->bufptr == '+') {
          parser->bufptr++;
          yylvalp->opval = _newOP(parser, SPerl_OP_C_CODE_NULL);
          return INCOP;
        }
        else {
          yylvalp->opval = _newOP(parser, SPerl_OP_C_CODE_NULL);
          return '+';
        }
      
      /* Subtract */
      case '-':
        parser->bufptr++;
        
        if (*parser->bufptr == '>') {
          parser->bufptr++;
          parser->expect = SPerl_TOKE_C_EXPECT_WORD;
          yylvalp->opval = _newOP(parser, SPerl_OP_C_CODE_NULL);
          return ARROW;
        }
        else if (*parser->bufptr == '-') {
          parser->bufptr++;
          yylvalp->opval = _newOP(parser, SPerl_OP_C_CODE_NULL);
          return DECOP;
        }
        else {
          yylvalp->opval = _newOP(parser, SPerl_OP_C_CODE_NULL);;
          return '-';
        }
      /* Multiply */
      case '*':
        parser->bufptr++;
        SPerl_OP* op = _newOP(parser, SPerl_OP_C_CODE_MULTIPLY);
        op->group = SPerl_OP_C_GROUP_BINOP;
        yylvalp->opval = op;
        return MULOP;
      
      /* Divide */
      case '/': {
        parser->bufptr++;
        SPerl_OP* op = _newOP(parser, SPerl_OP_C_CODE_DIVIDE);
        op->group = SPerl_OP_C_GROUP_BINOP;
        yylvalp->opval = op;
        return MULOP;
      }
      case '%': {
        parser->bufptr++;
        SPerl_OP* op = _newOP(parser, SPerl_OP_C_CODE_MODULO);
        op->group = SPerl_OP_C_GROUP_BINOP;
        yylvalp->opval = op;
        return MULOP;
      }
      case '^': {
        parser->bufptr++;
        SPerl_OP* op = _newOP(parser, SPerl_OP_C_CODE_BIT_XOR);
        op->group = SPerl_OP_C_GROUP_BINOP;
        yylvalp->opval = op;
        return MULOP;
      }
      case '|':
        parser->bufptr++;
        /* Or */
        if (*parser->bufptr == '|') {
          parser->bufptr++;
          SPerl_OP* op = _newOP(parser, SPerl_OP_C_CODE_OR);
          op->group = SPerl_OP_C_GROUP_BINOP;
          yylvalp->opval = op;
          return OROP;
        }
        /* Bit or */
        else {
          SPerl_OP* op = _newOP(parser, SPerl_OP_C_CODE_BIT_OR);
          op->group = SPerl_OP_C_GROUP_BINOP;
          yylvalp->opval = op;
          return BITOROP;
        }

      case '&':
        parser->bufptr++;
        /* Or */
        if (*parser->bufptr == '&') {
          parser->bufptr++;
          SPerl_OP* op = _newOP(parser, SPerl_OP_C_CODE_AND);
          op->group = SPerl_OP_C_GROUP_LOGICALOP;
          yylvalp->opval = op;
          return ANDOP;
        }
        /* Bit and */
        else {
          SPerl_OP* op = _newOP(parser, SPerl_OP_C_CODE_BIT_AND);
          op->group = SPerl_OP_C_GROUP_LOGICALOP;
          yylvalp->opval = op;
          return BITANDOP;
        }
      
      /* Comment */
      case '#':
        parser->bufptr++;
        while(1) {
          if (*parser->bufptr == '\r' || *parser->bufptr == '\n' || *parser->bufptr == '\0') {
            break;
          }
          parser->bufptr++;
        }
        
        continue;
      
      case '=':
        parser->bufptr++;
        
        /* == */
        if (*parser->bufptr == '=') {
          parser->bufptr++;
          SPerl_OP* op = _newOP(parser, SPerl_OP_C_CODE_EQ);
          op->group = SPerl_OP_C_GROUP_BINOP;
          yylvalp->opval = op;
          return RELOP;
        }
        /* = */
        else {
          SPerl_OP* op = _newOP(parser, SPerl_OP_C_CODE_ASSIGN);
          op->group = SPerl_OP_C_GROUP_ASSIGNOP;
          yylvalp->opval = op;
          return ASSIGNOP;
        }
        
      case '<':
        parser->bufptr++;
        
        if (*parser->bufptr == '<') {
          parser->bufptr++;
          SPerl_OP* op = _newOP(parser, SPerl_OP_C_CODE_LEFT_SHIFT);
          op->group = SPerl_OP_C_GROUP_BINOP;
          yylvalp->opval = op;
          return SHIFTOP;
        }
        /* <= */
        else if (*parser->bufptr == '=') {
          parser->bufptr++;
          SPerl_OP* op = _newOP(parser, SPerl_OP_C_CODE_LE);
          op->group = SPerl_OP_C_GROUP_BINOP;
          yylvalp->opval = op;
          return RELOP;
        }
        /* < */
        else {
          SPerl_OP* op = _newOP(parser, SPerl_OP_C_CODE_LT);
          op->group = SPerl_OP_C_GROUP_BINOP;
          yylvalp->opval = op;
          return RELOP;
        }
      
      case '>':
        parser->bufptr++;
        
        if (*parser->bufptr == '>') {
          parser->bufptr++;
          SPerl_OP* op = _newOP(parser, SPerl_OP_C_CODE_RIGHT_SHIFT);
          op->group = SPerl_OP_C_GROUP_BINOP;
          yylvalp->opval = op;
          return SHIFTOP;
        }
        /* >= */
        else if (*parser->bufptr == '=') {
          parser->bufptr++;
          SPerl_OP* op = _newOP(parser, SPerl_OP_C_CODE_GE);
          op->group = SPerl_OP_C_GROUP_BINOP;
          yylvalp->opval = op;
          return RELOP;
        }
        /* < */
        else {
          SPerl_OP* op = _newOP(parser, SPerl_OP_C_CODE_GT);
          op->group = SPerl_OP_C_GROUP_BINOP;
          yylvalp->opval = op;
          return RELOP;
        }
      case '!':
        parser->bufptr++;
        
        if (*parser->bufptr == '=') {
          parser->bufptr++;
          SPerl_OP* op = _newOP(parser, SPerl_OP_C_CODE_NE);
          op->group = SPerl_OP_C_GROUP_BINOP;
          yylvalp->opval = op;
          return RELOP;
        }
        else {
          SPerl_OP* op = _newOP(parser, SPerl_OP_C_CODE_NOT);
          op->group = SPerl_OP_C_GROUP_UNIOP;
          yylvalp->opval = op;
          return NOTOP;
        }
        
      case '~': {
        parser->bufptr++;
          SPerl_OP* op = _newOP(parser, SPerl_OP_C_CODE_COMPLEMENT);
          op->group = SPerl_OP_C_GROUP_UNIOP;
          yylvalp->opval = op;
        return '~';
      }
      case '\'': {
        parser->bufptr++;
        
        /* Save current position */
        SPerl_char* cur_token_ptr = parser->bufptr;
        
        SPerl_char ch;
        if (*parser->bufptr == '\'') {
          ch = '\0';
          parser->bufptr++;
        }
        else {
          ch = *parser->bufptr;
          parser->bufptr++;
          if (*parser->bufptr != '\'') {
            fprintf(stderr, "syntax error: string don't finish\n");
            exit(1);
          }
          parser->bufptr++;
        }
        
        // Constant 
        SPerl_OP* op = _newOP(parser, SPerl_OP_C_CODE_CONSTCHAR);
        SPerl_CONST_VALUE* const_value = SPerl_CONST_VALUE_new(parser);
        const_value->code = SPerl_CONST_VALUE_C_CODE_CHAR;
        const_value->uv.int_value = ch;
        SPerl_TYPE* type = SPerl_TYPE_create_word_type(parser, "char");
        const_value->type = type;
        
        op->info = const_value;
        yylvalp->opval = op;
        
        return CONSTVALUE;
      }
      case '"': {
        parser->bufptr++;
        
        /* Save current position */
        SPerl_char* cur_token_ptr = parser->bufptr;
        
        SPerl_char* str;
        if (*(parser->bufptr + 1) == '"') {
          str = SPerl_PARSER_new_string(parser, 0);
          str[0] = '\0';
          parser->bufptr++;
          parser->bufptr++;
        }
        else {
          while(*parser->bufptr != '"' && *parser->bufptr != '\0') {
            parser->bufptr++;
          }
          if (*parser->bufptr == '\0') {
            fprintf(stderr, "syntax error: string don't finish\n");
            exit(1);
          }
          
          size_t str_len = parser->bufptr - cur_token_ptr;
          str = SPerl_PARSER_new_string(parser, str_len);
          memcpy(str, cur_token_ptr, str_len);
          str[str_len] = '\0';

          parser->bufptr++;
        }
        
        SPerl_OP* op = _newOP(parser, SPerl_OP_C_CODE_CONSTSTRING);
        SPerl_CONST_VALUE* const_value = SPerl_CONST_VALUE_new(parser);
        const_value->code = SPerl_CONST_VALUE_C_CODE_STRING;
        const_value->uv.string_value = str;
        SPerl_TYPE* type = SPerl_TYPE_create_array_type(parser, "char");
        const_value->type = type;
        op->info = const_value;
        yylvalp->opval = (SPerl_OP*)op;

        return CONSTVALUE;
      }
      default:
        /* Variable */
        if (c == '$') {
          /* Save current position */
          SPerl_char* cur_token_ptr = parser->bufptr;
          
          parser->bufptr++;

          /* Next is graph */
          while(isalnum(*parser->bufptr) || (*parser->bufptr) == '_' || (*parser->bufptr) == ':') {
            parser->bufptr++;
          }

          size_t str_len = parser->bufptr - cur_token_ptr;
          SPerl_char* var_name = SPerl_PARSER_new_string(parser, str_len);
          memcpy(var_name, cur_token_ptr, str_len);
          var_name[str_len] = '\0';
          
          SPerl_WORD* var_name_word = SPerl_WORD_new(parser);
          var_name_word->value = var_name;
          
          SPerl_OP* op = _newOP(parser, SPerl_OP_C_CODE_VAR);
          SPerl_VAR* var = SPerl_VAR_new(parser);
          var->name_word = var_name_word;
          var->op = op;
          op->info = var;
          yylvalp->opval = op;
          
          return VAR;
        }
        /* Number literal */
        else if (isdigit(c)) {
          SPerl_char* cur_token_ptr = parser->bufptr;
          
          SPerl_int point_count = 0;
          
          parser->bufptr++;
          
          // Scan number
          while(isdigit(*parser->bufptr) || *parser->bufptr == '.') {
            if (*parser->bufptr == '.') {
              point_count++;
            }
            parser->bufptr++;
          }
          
          if (point_count >= 2) {
            fprintf(stderr, "Invalid number literal\n");
            exit(1);
          }
          
          // Number literal
          size_t str_len = parser->bufptr - cur_token_ptr;
          SPerl_char* num_str = malloc(str_len + 1);
          memcpy(num_str, cur_token_ptr, str_len);
          num_str[str_len] = '\0';
          
          // Convert to double
          if (point_count) {
            char* ends;
            double num = strtod(num_str, &ends);
            free(num_str);

            SPerl_OP* op = _newOP(parser, SPerl_OP_C_CODE_CONSTDOUBLE);
            SPerl_CONST_VALUE* const_value = SPerl_CONST_VALUE_new(parser);
            const_value->code = SPerl_CONST_VALUE_C_CODE_DOUBLE;
            const_value->uv.double_value = num;
            SPerl_TYPE* type = SPerl_TYPE_create_word_type(parser, "double");
            const_value->type = type;
            op->info = const_value;
            yylvalp->opval = (SPerl_OP*)op;
            
            return CONSTVALUE;
          }
          // Convert to integer
          else {
            SPerl_int num = atoi(num_str);
            free(num_str);
            
            SPerl_OP* op = _newOP(parser, SPerl_OP_C_CODE_CONSTINT);
            SPerl_CONST_VALUE* const_value = SPerl_CONST_VALUE_new(parser);
            const_value->code = SPerl_CONST_VALUE_C_CODE_INT;
            const_value->uv.int_value = num;
            SPerl_TYPE* type = SPerl_TYPE_create_word_type(parser, "int");
            const_value->type = type;
            op->info = const_value;
            yylvalp->opval = (SPerl_OP*)op;
            
            return CONSTVALUE;
          }
        }
        // Keyword or word
        else if (isalpha(c) || c == '_') {
          // Save current position
          SPerl_char* cur_token_ptr = parser->bufptr;
          
          parser->bufptr++;
          
          while(isalnum(*parser->bufptr)
            || *parser->bufptr == '_'
            || *parser->bufptr == ':' && *(parser->bufptr + 1) == ':')
          {
            if (*parser->bufptr == ':' && *(parser->bufptr + 1) == ':') {
              parser->bufptr += 2;
            }
            else {
              parser->bufptr++;
            }
          }
          
          size_t str_len = parser->bufptr - cur_token_ptr;
          SPerl_char* keyword = SPerl_PARSER_new_string(parser, str_len);
          memcpy(keyword, cur_token_ptr, str_len);
          keyword[str_len] = '\0';
          
          // Keyword
          if (expect != SPerl_TOKE_C_EXPECT_WORD) {
            if (memcmp(keyword, "my", str_len) == 0) {
              yylvalp->opval = _newOP(parser, SPerl_OP_C_CODE_MY);
              return MY;
            }
            else if (memcmp(keyword, "has", str_len) == 0) {
              yylvalp->opval = _newOP(parser, SPerl_OP_C_CODE_HAS);
              parser->expect = SPerl_TOKE_C_EXPECT_WORD;
              return HAS;
            }
            else if (memcmp(keyword, "sub", str_len) == 0) {
              yylvalp->opval = _newOP(parser, SPerl_OP_C_CODE_SUB);
              parser->expect = SPerl_TOKE_C_EXPECT_WORD;
              return SUB;
            }
            else if (memcmp(keyword, "package", str_len) == 0) {
              // File can contains only one package
              if (parser->current_package_count) {
                fprintf(stderr, "Can't write second package declaration in file at %s line %d\n", parser->cur_file, parser->cur_line);
                exit(1);
              }
              parser->current_package_count++;
              
              // Next is package name
              parser->expect = SPerl_TOKE_C_EXPECT_PACKAGENAME;
              
              yylvalp->opval = _newOP(parser, SPerl_OP_C_CODE_PACKAGE);
              return PACKAGE;
            }
            else if (memcmp(keyword, "if", str_len) == 0) {
              yylvalp->opval = _newOP(parser, SPerl_OP_C_CODE_IF);
              return IF;
            }
            else if (memcmp(keyword, "elsif", str_len) == 0) {
              yylvalp->opval = _newOP(parser, SPerl_OP_C_CODE_ELSIF);
              return ELSIF;
            }
            else if (memcmp(keyword, "else", str_len) == 0) {
              yylvalp->opval = _newOP(parser, SPerl_OP_C_CODE_ELSE);
              return ELSE;
            }
            else if (memcmp(keyword, "return", str_len) == 0) {
              yylvalp->opval = _newOP(parser, SPerl_OP_C_CODE_RETURN);
              return RETURN;
            }
            else if (memcmp(keyword, "for", str_len) == 0) {
              yylvalp->opval = _newOP(parser, SPerl_OP_C_CODE_FOR);
              return FOR;
            }
            else if (memcmp(keyword, "last", str_len) == 0) {
              yylvalp->opval = _newOP(parser, SPerl_OP_C_CODE_LAST);
              return LAST;
            }
            else if (memcmp(keyword, "next", str_len) == 0) {
              yylvalp->opval = _newOP(parser, SPerl_OP_C_CODE_NEXT);
              return NEXT;
            }
            else if (memcmp(keyword, "use", str_len) == 0) {
              yylvalp->opval = _newOP(parser, SPerl_OP_C_CODE_USE);
              return USE;
            }
            else if (memcmp(keyword, "while", str_len) == 0) {
              yylvalp->opval = _newOP(parser, SPerl_OP_C_CODE_WHILE);
              return WHILE;
            }
            else if (memcmp(keyword, "enum", str_len) == 0) {
              yylvalp->opval = _newOP(parser, SPerl_OP_C_CODE_ENUM);
              return ENUM;
            }
            else if (memcmp(keyword, "value", str_len) == 0) {
              SPerl_OP* op = _newOP(parser, SPerl_OP_C_CODE_DESCRIPTER);
              SPerl_DESCRIPTER* descripter = SPerl_DESCRIPTER_new(parser);
              descripter->code = SPerl_DESCRIPTER_C_CODE_VALUE;
              descripter->op = op;
              op->info = descripter;
              yylvalp->opval = op;
              
              return DESCRIPTER;
            }
            else if (memcmp(keyword, "true", str_len) == 0) {
              SPerl_OP* op = _newOP(parser, SPerl_OP_C_CODE_CONSTBOOLEAN);
              SPerl_CONST_VALUE* const_value = SPerl_CONST_VALUE_new(parser);
              const_value->code = SPerl_CONST_VALUE_C_CODE_BOOLEAN;
              const_value->uv.int_value = 1;
              SPerl_TYPE* type = SPerl_TYPE_create_word_type(parser, "boolean");
              const_value->type = type;
              op->info = const_value;
              yylvalp->opval = op;

              return CONSTVALUE;
            }
            else if (memcmp(keyword, "false", str_len) == 0) {
              SPerl_OP* op = _newOP(parser, SPerl_OP_C_CODE_CONSTBOOLEAN);
              SPerl_CONST_VALUE* const_value = SPerl_CONST_VALUE_new(parser);
              const_value->code = SPerl_CONST_VALUE_C_CODE_BOOLEAN;
              const_value->uv.int_value = 0;
              SPerl_TYPE* type = SPerl_TYPE_create_word_type(parser, "boolean");
              const_value->type = type;
              op->info = const_value;
              yylvalp->opval = op;

              return CONSTVALUE;
            }
          }
          
          SPerl_OP* op = _newOP(parser, SPerl_OP_C_CODE_WORD);
          SPerl_WORD* word = SPerl_WORD_new(parser);
          word->value = keyword;
          word->op = op;
          op->info = word;
          yylvalp->opval = (SPerl_OP*)op;

          if (expect == SPerl_TOKE_C_EXPECT_PACKAGENAME) {
            // Template class
            if (strchr(keyword, '_')) {

            }
            // Class
            else {
              if (strcmp(keyword, parser->current_use_package_name) != 0) {
                fprintf(stderr, "Package name \"%s\" must be \"%s\" at %s line %d\n",
                  keyword, parser->current_use_package_name, parser->cur_file, parser->cur_line);
                exit(1);
              }
            }
          }
          
          return WORD;
        }
        
        /* Return character */
        parser->bufptr++;
        yylvalp->opval = _newOP(parser, SPerl_OP_C_CODE_NULL);
        
        return c;
    }
  }
}

