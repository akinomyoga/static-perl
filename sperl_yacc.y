%pure-parser
%parse-param	{ SPerl_PARSER* parser }
%lex-param	{ SPerl_PARSER* parser }

%{
  #include <stdio.h>
  
  #include "sperl_yacc.h"
  #include "sperl_toke.h"
  #include "sperl_parser.h"
  #include "sperl_op.h"
%}

%token <opval> '+' '-'
%token <opval> MY HAS SUB PACKAGE IF ELSIF ELSE RETURN FOR WHILE USE
%token <opval> LAST NEXT WORD VAR CONSTVALUE ENUM DESCRIPTER CORETYPE

%type <opval> grammar optstatements statements statement declmy declfield ifstatement elsestatement
%type <opval> block enumblock classblock declsub optdeclclassattrs callsub callop
%type <opval> optterms terms term subargs subarg optsubargs decluse declclassattr declclassattrs 
%type <opval> optdescripters listdescripters descripters enumvalues enumvalue declanonsub
%type <opval> type packagename fieldname subname package packages optenumvalues arraytype
%type <opval> forstatement whilestatement expression optpackages subtype types opttypes notsubtype
%type <opval> enumname getenumvalue getfield getarrayelem converttype

%right <opval> ASSIGNOP
%left <opval> OROP
%left <opval> ANDOP
%left <opval> BITOROP
%left <opval> BITANDOP
%nonassoc <opval> RELOP
%left <opval> SHIFTOP
%left <opval> ADDOP
%left <opval> MULOP
%right <opval> NOTOP '~' UMINUS
%nonassoc <opval> INCOP DECOP
%left <opval> ARROW
%nonassoc <opval> ')'
%left <opval> '('
%left <opval> '[' '{'

%%

grammar
  : optpackages
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

optpackages
  :	/* Empty */
    {
      $$ = SPerl_OP_newOP_LIST(parser);
    }
  |	packages
    {
      if ($1->code == SPerl_OP_C_CODE_LIST) {
        $$ = $1;
      }
      else {
        $$ = SPerl_OP_newOP_LIST(parser);
        SPerl_OP_sibling_splice(parser, $$, $$->first, 0, $1);
      }
    }
    
packages
  : packages package
    {
      $$ = SPerl_OP_append_elem(parser, $1, $2);
    }
  | package

package
  : PACKAGE packagename type ';'
    {
      $$ = SPerl_OP_build_package(parser, $1, $2, $3, SPerl_OP_newOP_LIST(parser), SPerl_OP_newOP_NULL(parser));
      if (parser->fatal_error) {
        YYABORT;
      }
    }
  | PACKAGE packagename classblock
    {
      $$ = SPerl_OP_build_package(parser, $1, $2, SPerl_OP_newOP_NULL(parser), SPerl_OP_newOP_LIST(parser), $3);
      if (parser->fatal_error) {
        YYABORT;
      }
    }
  | PACKAGE packagename ':' listdescripters classblock
    {
      $$ = SPerl_OP_build_package(parser, $1, $2, SPerl_OP_newOP_NULL(parser), $4, $5);
      if (parser->fatal_error) {
        YYABORT;
      }
    }
  | PACKAGE packagename ':' ENUM enumblock
    {
      $$ = SPerl_OP_build_package(parser, $1, $2, SPerl_OP_newOP_NULL(parser), $4, $5);
      if (parser->fatal_error) {
        YYABORT;
      }
    }

enumblock 
  : '{' optenumvalues '}'
    {
      $$ = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_ENUMBLOCK, $2, NULL);
    }

optenumvalues
  :	/* Empty */
    {
      $$ = SPerl_OP_newOP_LIST(parser);
    }
  |	enumvalues
    {
      if ($1->code == SPerl_OP_C_CODE_LIST) {
        $$ = $1;
      }
      else {
        $$ = SPerl_OP_newOP_LIST(parser);
        SPerl_OP_sibling_splice(parser, $$, $$->first, 0, $1);
      }
    }
    
enumvalues
  : enumvalues ',' enumvalue 
    {
      $$ = SPerl_OP_append_elem(parser, $1, $3);
    }
  | enumvalue
  
enumvalue
  : WORD
    {
      $$ = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_ENUMVALUE, $1, NULL);
    }
  | WORD ASSIGNOP CONSTVALUE
    {
      $$ = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_ENUMVALUE, $1, $3);
    }

optstatements
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
  : ifstatement
  | forstatement
  | whilestatement
  | block
  | term ';'
    {
      $$ = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_POP, $1, NULL);
    }
  | expression ';'
    {
      $$ = $1;
    }
  | ';'
    {
      $$ = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_NULL, NULL, NULL);
    }

forstatement
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

whilestatement
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

ifstatement
  : IF '(' term ')' block elsestatement
    {
      SPerl_OP* op = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_COND, $3, $5);
      SPerl_OP_sibling_splice(parser, op, $5, 0, $6);
      $$ = op;
    }

elsestatement
  : /* NULL */
    {
      $$ = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_NULL, NULL, NULL);
    };
  | ELSE block
    {
      $$ = $2;
    }
  | ELSIF '(' term ')' block elsestatement
    {
      SPerl_OP* op = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_COND, $3, $5);
      SPerl_OP_sibling_splice(parser, op, $5, 0, $6);
      $$ = op;
    }

decluse
  : USE packagename ';'
    {
      $$ = SPerl_OP_build_decluse(parser, $1, $2, SPerl_OP_newOP_NULL(parser));
    }


declfield
  : HAS fieldname ':' optdescripters type ';'
    {
      $$ = SPerl_OP_build_declfield(parser, $1, $2, $4, $5);
    }

declsub
 : SUB subname '(' optsubargs ')' ':' optdescripters type block
     {
       $$ = SPerl_OP_build_declsub(parser, $1, $2, $4, $7, $8, $9);
     }

declmy
  : MY VAR ':' optdescripters type
    {
      $$ = SPerl_OP_build_declmy(parser, $1, $2, $4, $5);
    }

declanonsub
 : SUB '(' optsubargs ')' ':' optdescripters type block
     {
       $1->code = SPerl_OP_C_CODE_ANONSUB;
       $$ = SPerl_OP_build_declsub(parser, $1, SPerl_OP_newOP_NULL(parser), $3, $6, $7, $8);
     }

optdeclclassattrs
  :	/* Empty */
    {
      $$ = SPerl_OP_newOP_LIST(parser);
    }
  |	declclassattrs
    {
      if ($1->code == SPerl_OP_C_CODE_LIST) {
        $$ = $1;
      }
      else {
        $$ = SPerl_OP_newOP_LIST(parser);
        SPerl_OP_sibling_splice(parser, $$, $$->first, 0, $1);
      }
    }

declclassattrs
  : declclassattrs declclassattr
    {
      $$ = SPerl_OP_append_elem(parser, $1, $2);
    }
  | declclassattr

declclassattr
  : decluse
  | declfield
  | declsub

classblock
  : '{' optdeclclassattrs '}'
    {
      $$ = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_CLASSBLOCK, $2, NULL);
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
      $$ = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_RETURN, $2, NULL);
    }

optterms
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
  | declmy
  | declanonsub
  | callsub
  | callop
  | getenumvalue
  | getfield
  | getarrayelem
  | converttype



converttype
  : '(' type ')' term
    {
      $$ = SPerl_OP_build_converttype(parser, $2, $4);
    }

getfield
  : VAR ARROW fieldname
    {
      $$ = SPerl_OP_build_getfield(parser, $1, $3);
    }

getenumvalue
  : enumname
    {
      $$ = SPerl_OP_build_getenumvalue(parser, $1);
    }

callop
  : '+' term %prec UMINUS
    {
      SPerl_OP* op = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_PLUS, NULL, NULL);
      op->file = $1->file;
      op->line = $1->line;
      $$ = SPerl_OP_build_callop(parser, op, $2, NULL);
    }
  | '-' term %prec UMINUS
    {
      SPerl_OP* op = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_NEGATE, NULL, NULL);
      op->file = $1->file;
      op->line = $1->line;
      $$ = SPerl_OP_build_callop(parser, op, $2, NULL);
    }
  | INCOP term
    {
      SPerl_OP* op = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_PREINC, NULL, NULL);
      op->file = $1->file;
      op->line = $1->line;
      $$ = SPerl_OP_build_callop(parser, op, $2, NULL);
    }
  | term INCOP
    {
      SPerl_OP* op = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_POSTINC, NULL, NULL);
      op->file = $2->file;
      op->line = $2->line;
      $$ = SPerl_OP_build_callop(parser, op, $1, NULL);
    }
  | DECOP term
    {
      SPerl_OP* op = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_PREDEC, NULL, NULL);
      op->file = $1->file;
      op->line = $1->line;
      $$ = SPerl_OP_build_callop(parser, op, $2, NULL);
    }
  | term DECOP
    {
      SPerl_OP* op = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_POSTDEC, NULL, NULL);
      op->file = $2->file;
      op->line = $2->line;
      $$ = SPerl_OP_build_callop(parser, op, $1, NULL);
    }
  | '~' term
    {
      $$ = SPerl_OP_build_callop(parser, $1, $2, NULL);
    }
  | term '+' term %prec ADDOP
    {
      SPerl_OP* op = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_ADD, NULL, NULL);
      op->file = $2->file;
      op->line = $2->line;
      $$ = SPerl_OP_build_callop(parser, op, $1, $3);
    }
  | term '-' term %prec ADDOP
    {
      SPerl_OP* op = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_SUBTRACT, NULL, NULL);
      op->file = $2->file;
      op->line = $2->line;
      $$ = SPerl_OP_build_callop(parser, op, $1, $3);
    }
  | term MULOP term
    {
      $$ = SPerl_OP_build_callop(parser, $2, $1, $3);
    }
  | term BITANDOP term
    {
      $$ = SPerl_OP_build_callop(parser, $2, $1, $3);
    }
  | term BITOROP term
    {
      $$ = SPerl_OP_build_callop(parser, $2, $1, $3);
    }
  | term SHIFTOP term
    {
      $$ = SPerl_OP_build_callop(parser, $2, $1, $3);
    }
  | term RELOP term
    {
      $$ = SPerl_OP_build_callop(parser, $2, $1, $3);
    }
  | term ASSIGNOP term
    {
      $$ = SPerl_OP_build_callop(parser, $2, $1, $3);
    }
  | term ANDOP term
    {
      $$ = SPerl_OP_build_callop(parser, $2, $1, $3);
    }
  | term OROP term
    {
      $$ = SPerl_OP_build_callop(parser, $2, $1, $3);
    }
  | NOTOP term
    {
      $$ = SPerl_OP_build_callop(parser, $1, $2, NULL);
    }

getarrayelem
  : VAR ARROW '[' term ']'
    {
      $$ = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_AELEM, $1, $4);
    }

callsub
  : subname '(' optterms  ')'
    {
      $$ = SPerl_OP_build_callsub(parser, SPerl_OP_newOP_NULL(parser), $1, $3, 0);
    }
  | VAR ARROW subname '(' optterms ')'
    {
      $$ = SPerl_OP_build_callsub(parser, $1, $3, $5, 0);
    }
  | VAR ARROW '(' optterms ')'
    {
      $$ = SPerl_OP_build_callsub(parser, $1, SPerl_OP_newOP_NULL(parser), $4, 1);
    }

block 
  : '{' optstatements '}'
    {
      $$ = SPerl_OP_newOP(parser, SPerl_OP_C_CODE_BLOCK, $2, NULL);
    }

optsubargs
  :	/* Empty */
    {
      $$ = SPerl_OP_newOP_LIST(parser);
    }
  |	subargs
    {
      if ($1->code == SPerl_OP_C_CODE_LIST) {
        $$ = $1;
      }
      else {
        $$ = SPerl_OP_newOP_LIST(parser);
        SPerl_OP_sibling_splice(parser, $$, $$->first, 0, $1);
      }
    }

subargs
  : subargs ',' subarg
    {
      $$ = SPerl_OP_append_elem(parser, $1, $3);
    }
  | subarg

subarg
  : VAR ':' optdescripters type
    {
      $$ = SPerl_OP_build_declmy(parser, SPerl_OP_newOP(parser, SPerl_OP_C_CODE_MY, NULL, NULL), $1, $3, $4);
    }
    
optdescripters
  :	/* Empty */
    {
      $$ = SPerl_OP_newOP_LIST(parser);
    }
  |	listdescripters

listdescripters
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

opttypes
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
  : notsubtype
  | subtype

notsubtype
  : WORD
    {
      $$ = SPerl_OP_build_wordtype(parser, $1);
    }
  | CORETYPE
  | arraytype
  
subtype
  : SUB '(' opttypes ')' type
    {
      $$ = SPerl_OP_build_subtype(parser, $3, $5);
    }

arraytype
  : notsubtype '[' ']'
    {
      $$ = SPerl_OP_build_arraytype(parser, $1);
    }
  | '(' subtype ')' '[' ']'
    {
      $$ = SPerl_OP_build_arraytype(parser, $2);
    }

enumname : WORD
fieldname : WORD
subname : WORD
packagename : WORD

%%

