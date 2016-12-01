%pure-parser
%parse-param	{ SPerl_PARSER* parser }
%lex-param	{ SPerl_PARSER* parser }

%{
  #include <stdio.h>
  
  #include "sperl_yacc.h"
  #include "sperl_toke.h"
  #include "sperl_parser.h"
  #include "sperl_op.h"
  #include "sperl_op_info.h"
%}

%token <opval> MY HAS SUB PACKAGE IF ELSIF ELSE RETURN FOR WHILE USE
%token <opval> LAST NEXT WORD VAR CONSTVALUE ENUM DESCRIPTER CORETYPE

%type <opval> grammar opt_statements statements statement decl_my decl_field if_statement else_statement
%type <opval> block enum_block class_block decl_sub opt_decl_class_attrs call_sub call_op
%type <opval> opt_terms terms term sub_args sub_arg opt_sub_args decl_use decl_class_attr decl_class_attrs 
%type <opval> opt_descripters list_descripters descripters decl_enum_values decl_enum_value decl_anon_sub
%type <opval> type package_name field_name sub_name decl_package decl_packages opt_decl_enum_values type_array
%type <opval> for_statement while_statement expression opt_decl_packages type_sub types opt_types not_type_sub
%type <opval> enum_name get_enum_value field array_elem convert_type

%right <opval> ASSIGNOP
%left <opval> OROP
%left <opval> ANDOP
%left <opval> BITOROP
%left <opval> BITANDOP
%nonassoc <opval> RELOP
%left <opval> SHIFTOP
%left <opval> '+' '-'
%left <opval> MULOP
%right <opval> NOTOP '~' UMINUS
%nonassoc <opval> INCOP DECOP
%left <opval> ARROW
%nonassoc <opval> ')'
%left <opval> '('
%left <opval> '[' '{'

%%

grammar
  : opt_decl_packages
    {
      $$ = SPerl_OP_build_grammer(parser, $1);

      // Syntax error
      if (parser->error_count) {
        YYABORT;
      }
      else {
        // Dump parser infomation
        SPerl_PARSER_dump_parser(parser);
      }
    }

opt_decl_packages
  :	/* Empty */
    {
      $$ = SPerl_OP_newOP_LIST(parser);
    }
  |	decl_packages
    {
      if ($1->code == SPerl_OP_C_CODE_LIST) {
        $$ = $1;
      }
      else {
        $$ = SPerl_OP_newOP_LIST(parser);
        SPerl_OP_sibling_splice(parser, $$, $$->first, 0, $1);
      }
    }
    
decl_packages
  : decl_packages decl_package
    {
      $$ = SPerl_OP_append_elem(parser, $1, $2);
    }
  | decl_package

decl_package
  : PACKAGE package_name type ';'
    {
      $$ = SPerl_OP_build_decl_package(parser, $1, $2, $3, SPerl_OP_newOP_LIST(parser), SPerl_OP_newOP_NULL(parser));
      if (parser->fatal_error) {
        YYABORT;
      }
    }
  | PACKAGE package_name class_block
    {
      $$ = SPerl_OP_build_decl_package(parser, $1, $2, SPerl_OP_newOP_NULL(parser), SPerl_OP_newOP_LIST(parser), $3);
      if (parser->fatal_error) {
        YYABORT;
      }
    }
  | PACKAGE package_name ':' list_descripters class_block
    {
      $$ = SPerl_OP_build_decl_package(parser, $1, $2, SPerl_OP_newOP_NULL(parser), $4, $5);
      if (parser->fatal_error) {
        YYABORT;
      }
    }
  | PACKAGE package_name ':' ENUM enum_block
    {
      $$ = SPerl_OP_build_decl_package(parser, $1, $2, SPerl_OP_newOP_NULL(parser), $4, $5);
      if (parser->fatal_error) {
        YYABORT;
      }
    }

enum_block 
  : '{' opt_decl_enum_values '}'
    {
      $$ = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_ENUM_BLOCK, $2, NULL);
    }

opt_decl_enum_values
  :	/* Empty */
    {
      $$ = SPerl_OP_newOP_LIST(parser);
    }
  |	decl_enum_values
    {
      if ($1->code == SPerl_OP_C_CODE_LIST) {
        $$ = $1;
      }
      else {
        $$ = SPerl_OP_newOP_LIST(parser);
        SPerl_OP_sibling_splice(parser, $$, $$->first, 0, $1);
      }
    }
    
decl_enum_values
  : decl_enum_values ',' decl_enum_value 
    {
      $$ = SPerl_OP_append_elem(parser, $1, $3);
    }
  | decl_enum_value
  
decl_enum_value
  : WORD
    {
      $$ = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_DECL_ENUM_VALUE, $1, NULL);
    }
  | WORD ASSIGNOP CONSTVALUE
    {
      $$ = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_DECL_ENUM_VALUE, $1, $3);
    }

opt_statements
  :	/* Empty */
    {
      $$ = SPerl_OP_newOP_LIST(parser);
    }
  |	statements
    {
      if ($1->code == SPerl_OP_C_CODE_LIST) {
        $$ = $1;
      }
      else {
        $$ = SPerl_OP_newOP_LIST(parser);
        SPerl_OP_sibling_splice(parser, $$, $$->first, 0, $1);
      }
    }
    
statements
  : statements statement 
    {
      $$ = SPerl_OP_append_elem(parser, $1, $2);
    }
  | statement

statement
  : if_statement
  | for_statement
  | while_statement
  | block
  | term ';'
    {
      $$ = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_POP, $1, NULL);
      $$->uv.op_info = SPerl_OP_INFO_new(parser);
    }
  | expression ';'
    {
      $$ = $1;
    }
  | ';'
    {
      $$ = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_NULL, NULL, NULL);
    }

for_statement
  : FOR '(' term ';' term ';' term ')' block
    {
      SPerl_OP* op = SPerl_OP_newOP(
        parser,
        SPerl_OP_C_CODE_LOOP,
        $3,
        $5
      );
      SPerl_OP_sibling_splice(parser, op, $5, 0, $9);
      SPerl_OP_sibling_splice(parser, op, $9, 0, $7);
      $$ = op;
    }

while_statement
  : WHILE '(' term ')' block
    {
      SPerl_OP* op = SPerl_OP_newOP(
        parser,
        SPerl_OP_C_CODE_LOOP,
        SPerl_OP_newOP(parser, SPerl_OP_C_CODE_NULL, NULL, NULL),
        $3
      );
      SPerl_OP_sibling_splice(parser, op, $3, 0, $5);
      $$ = op;
    }

if_statement
  : IF '(' term ')' block else_statement
    {
      SPerl_OP* op = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_IF_STATEMENT, $3, $5);
      SPerl_OP_sibling_splice(parser, op, $5, 0, $6);
      $$ = op;
    }

else_statement
  : /* NULL */
    {
      $$ = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_NULL, NULL, NULL);
    };
  | ELSE block
    {
      $$ = $2;
    }
  | ELSIF '(' term ')' block else_statement
    {
      SPerl_OP* op = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_IF_STATEMENT, $3, $5);
      SPerl_OP_sibling_splice(parser, op, $5, 0, $6);
      $$ = op;
    }

decl_use
  : USE package_name ';'
    {
      $$ = SPerl_OP_build_decl_use(parser, $1, $2, SPerl_OP_newOP_NULL(parser));
    }


decl_field
  : HAS field_name ':' opt_descripters type ';'
    {
      $$ = SPerl_OP_build_decl_field(parser, $1, $2, $4, $5);
    }

decl_sub
 : SUB sub_name '(' opt_sub_args ')' ':' opt_descripters type block
     {
       $$ = SPerl_OP_build_decl_sub(parser, $1, $2, $4, $7, $8, $9);
     }

decl_my
  : MY VAR ':' opt_descripters type
    {
      $$ = SPerl_OP_build_decl_my(parser, $1, $2, $4, $5);
    }

decl_anon_sub
 : SUB '(' opt_sub_args ')' ':' opt_descripters type block
     {
       $1->code = SPerl_OP_C_CODE_DECL_ANON_SUB;
       $$ = SPerl_OP_build_decl_sub(parser, $1, SPerl_OP_newOP_NULL(parser), $3, $6, $7, $8);
     }

opt_decl_class_attrs
  :	/* Empty */
    {
      $$ = SPerl_OP_newOP_LIST(parser);
    }
  |	decl_class_attrs
    {
      if ($1->code == SPerl_OP_C_CODE_LIST) {
        $$ = $1;
      }
      else {
        $$ = SPerl_OP_newOP_LIST(parser);
        SPerl_OP_sibling_splice(parser, $$, $$->first, 0, $1);
      }
    }

decl_class_attrs
  : decl_class_attrs decl_class_attr
    {
      $$ = SPerl_OP_append_elem(parser, $1, $2);
    }
  | decl_class_attr

decl_class_attr
  : decl_use
  | decl_field
  | decl_sub

class_block
  : '{' opt_decl_class_attrs '}'
    {
      $$ = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_CLASS_BLOCK, $2, NULL);
    }

expression
  : LAST
    {
      $$ = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_LAST, NULL, NULL);
    }
  | NEXT
    {
      $$ = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_NEXT, NULL, NULL);
    }
  | RETURN term
    {
      $$ = $1;
      SPerl_OP_sibling_splice(parser, $$, NULL, 0, $2);
      $$->uv.op_info = SPerl_OP_INFO_new(parser);
    }

opt_terms
  :	/* Empty */
    {
      $$ = SPerl_OP_newOP_LIST(parser);
    }
  |	terms
    {
      if ($1->code == SPerl_OP_C_CODE_LIST) {
        $$ = $1;
      }
      else {
        $$ = SPerl_OP_newOP_LIST(parser);
        SPerl_OP_sibling_splice(parser, $$, $$->first, 0, $1);
      }
    }
    
terms
  : terms ',' term
    {
      $$ = SPerl_OP_append_elem(parser, $1, $3);
    }
  | term

term
  : VAR
  | CONSTVALUE
  | decl_my
  | decl_anon_sub
  | call_sub
  | call_op
  | get_enum_value
  | field
  | array_elem
  | convert_type



convert_type
  : '(' type ')' term
    {
      $$ = SPerl_OP_build_convert_type(parser, $2, $4);
    }

field
  : VAR ARROW field_name
    {
      $$ = SPerl_OP_build_field(parser, $1, $3);
    }

get_enum_value
  : enum_name
    {
      $$ = SPerl_OP_build_get_enum_value(parser, $1);
    }

call_op
  : '+' term %prec UMINUS
    {
      SPerl_OP* op = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_PLUS, NULL, NULL);
      op->file = $1->file;
      op->line = $1->line;
      $$ = SPerl_OP_build_call_op(parser, op, $2, NULL);
    }
  | '-' term %prec UMINUS
    {
      SPerl_OP* op = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_NEGATE, NULL, NULL);
      op->file = $1->file;
      op->line = $1->line;
      $$ = SPerl_OP_build_call_op(parser, op, $2, NULL);
    }
  | INCOP term
    {
      SPerl_OP* op = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_PRE_INC, NULL, NULL);
      op->file = $1->file;
      op->line = $1->line;
      $$ = SPerl_OP_build_call_op(parser, op, $2, NULL);
    }
  | term INCOP
    {
      SPerl_OP* op = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_POST_INC, NULL, NULL);
      op->file = $2->file;
      op->line = $2->line;
      $$ = SPerl_OP_build_call_op(parser, op, $1, NULL);
    }
  | DECOP term
    {
      SPerl_OP* op = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_PRE_DEC, NULL, NULL);
      op->file = $1->file;
      op->line = $1->line;
      $$ = SPerl_OP_build_call_op(parser, op, $2, NULL);
    }
  | term DECOP
    {
      SPerl_OP* op = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_POST_DEC, NULL, NULL);
      op->file = $2->file;
      op->line = $2->line;
      $$ = SPerl_OP_build_call_op(parser, op, $1, NULL);
    }
  | '~' term
    {
      $$ = SPerl_OP_build_call_op(parser, $1, $2, NULL);
    }
  | term '+' term
    {
      SPerl_OP* op = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_ADD, NULL, NULL);
      op->file = $2->file;
      op->line = $2->line;
      $$ = SPerl_OP_build_call_op(parser, op, $1, $3);
    }
  | term '-' term
    {
      SPerl_OP* op = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_SUBTRACT, NULL, NULL);
      op->file = $2->file;
      op->line = $2->line;
      $$ = SPerl_OP_build_call_op(parser, op, $1, $3);
    }
  | term MULOP term
    {
      $$ = SPerl_OP_build_call_op(parser, $2, $1, $3);
    }
  | term BITANDOP term
    {
      $$ = SPerl_OP_build_call_op(parser, $2, $1, $3);
    }
  | term BITOROP term
    {
      $$ = SPerl_OP_build_call_op(parser, $2, $1, $3);
    }
  | term SHIFTOP term
    {
      $$ = SPerl_OP_build_call_op(parser, $2, $1, $3);
    }
  | term RELOP term
    {
      $$ = SPerl_OP_build_call_op(parser, $2, $1, $3);
    }
  | term ASSIGNOP term
    {
      $$ = SPerl_OP_build_call_op(parser, $2, $1, $3);
    }
  | term ANDOP term
    {
      $$ = SPerl_OP_build_call_op(parser, $2, $1, $3);
    }
  | term OROP term
    {
      $$ = SPerl_OP_build_call_op(parser, $2, $1, $3);
    }
  | NOTOP term
    {
      $$ = SPerl_OP_build_call_op(parser, $1, $2, NULL);
    }
  | '(' term ')'
    {
      $$ = $2;
    }

array_elem
  : VAR ARROW '[' term ']'
    {
      $$ = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_ARRAY_ELEM, $1, $4);
      $$->file = $1->file;
      $$->line = $1->line;
      $$->uv.op_info = SPerl_OP_INFO_new(parser);
    }

call_sub
  : sub_name '(' opt_terms  ')'
    {
      $$ = SPerl_OP_build_call_sub(parser, SPerl_OP_newOP_NULL(parser), $1, $3, 0);
    }
  | VAR ARROW sub_name '(' opt_terms ')'
    {
      $$ = SPerl_OP_build_call_sub(parser, $1, $3, $5, 0);
    }
  | VAR ARROW '(' opt_terms ')'
    {
      $$ = SPerl_OP_build_call_sub(parser, $1, SPerl_OP_newOP_NULL(parser), $4, 1);
    }

block 
  : '{' opt_statements '}'
    {
      $$ = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_BLOCK, $2, NULL);
    }

opt_sub_args
  :	/* Empty */
    {
      $$ = SPerl_OP_newOP_LIST(parser);
    }
  |	sub_args
    {
      if ($1->code == SPerl_OP_C_CODE_LIST) {
        $$ = $1;
      }
      else {
        $$ = SPerl_OP_newOP_LIST(parser);
        SPerl_OP_sibling_splice(parser, $$, $$->first, 0, $1);
      }
    }

sub_args
  : sub_args ',' sub_arg
    {
      $$ = SPerl_OP_append_elem(parser, $1, $3);
    }
  | sub_arg

sub_arg
  : VAR ':' opt_descripters type
    {
      $$ = SPerl_OP_build_decl_my(parser, SPerl_OP_newOP(parser, SPerl_OP_C_CODE_DECL_MY_VAR, NULL, NULL), $1, $3, $4);
    }
    
opt_descripters
  :	/* Empty */
    {
      $$ = SPerl_OP_newOP_LIST(parser);
    }
  |	list_descripters

list_descripters
  :	descripters
    {
      if ($1->code == SPerl_OP_C_CODE_LIST) {
        $$ = $1;
      }
      else {
        $$ = SPerl_OP_newOP_LIST(parser);
        SPerl_OP_sibling_splice(parser, $$, $$->first, 0, $1);
      }
    }

descripters
  : descripters DESCRIPTER
    {
      $$ = SPerl_OP_append_elem(parser, $1, $2);
    }
  | DESCRIPTER

opt_types
  :	/* Empty */
    {
      $$ = SPerl_OP_newOP_LIST(parser);
    }
  |	types
    {
      if ($1->code == SPerl_OP_C_CODE_LIST) {
        $$ = $1;
      }
      else {
        $$ = SPerl_OP_newOP_LIST(parser);
        SPerl_OP_sibling_splice(parser, $$, $$->first, 0, $1);
      }
    }

types
  : types ',' type
    {
      $$ = SPerl_OP_append_elem(parser, $1, $3);
    }
  | type

type
  : not_type_sub
  | type_sub

not_type_sub
  : WORD
    {
      $$ = SPerl_OP_build_type_word(parser, $1);
    }
  | type_array
  
type_sub
  : SUB '(' opt_types ')' type
    {
      $$ = SPerl_OP_build_type_sub(parser, $3, $5);
    }

type_array
  : not_type_sub '[' ']'
    {
      $$ = SPerl_OP_build_type_array(parser, $1);
    }
  | '(' type_sub ')' '[' ']'
    {
      $$ = SPerl_OP_build_type_array(parser, $2);
    }

enum_name : WORD
field_name : WORD
sub_name : WORD
package_name : WORD

%%

