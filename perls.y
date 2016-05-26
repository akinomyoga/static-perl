%{
  #include <stdio.h>
  #include <ctype.h>
  #include <stdlib.h>
  #include <string.h>

  int yylex(void);
  void yyerror(const char* s);

  #define OP_CONST_INT 1
  #define OP_CONST_FLOAT 2
  #define OP_CONST_STRING 3
  #define OP_LT 4
  #define OP_LE 5
  #define OP_GT 6
  #define OP_GE 7
  #define OP_ADD 8
  #define OP_SUBTRACT 9
  #define OP_MULTIPLY 10
  #define OP_DIVIDE 11
  #define OP_AND 12
  #define OP_OR 13
  #define OP_BIT_AND 14
  #define OP_BIT_OR 15
  
  #define BASEOP int type;
  
  struct yy_parser {
    /* Source data */
    char* linestr;

    /* Source buffer size */
    size_t linestr_buf_len;

    /* Source size */
    size_t linestr_len;

    /* Current buffer position */
    char* buf_ptr;

    /* Before token start position */
    char* bef_token_ptr;
  };
  
  struct op {
    BASEOP
  };
  typedef struct op OP;
  
  struct svop {
    BASEOP
    union {
      char* pv;
      int iv;
      double nv;
    } uv;
  };
  typedef struct svop SVOP;

  struct unop {
      BASEOP
      OP *	op_first;
  };
  typedef struct unop UNOP;

  struct binop {
      BASEOP
      OP *	op_first;
      OP *	op_last;
  };
  typedef struct binop BINOP;

  struct listop {
      BASEOP
      OP *	op_first;
      OP *	op_last;
  };
  typedef struct binop LISTOP;
%}

%union {
  OP* opval;
  int	ival;
}

%token <ival> MY OUR HAS SUB PACKAGE IF ELSIF ELSE RETURN FOR
%token <ival> RELOP EQOP
%token <ival> LAST CONTINUE
%token <opval> WORD VAR INT

%right <ival> ASSIGNOP
%left <ival> OROP
%left <ival> ANDOP
%left <ival> BITOROP
%left <ival> BITANDOP
%nonassoc EQOP
%nonassoc RELOP
/* %left <ival> SHIFTOP */
%left ADDOP /* "+" or "-" */
%left MULOP
/* %right <ival> '!' '~' UMINUS */
/* %nonassoc <ival> PREINC PREDEC POSTINC POSTDEC */
/* %left <ival> ARROW */
/* %nonassoc <ival> ')' */
/* %left <ival> '(' */
/* %left '[' '{' */

%%

grammar
  : /* NULL */
    { printf("NULL -> grammar\n"); }
  | statements
    { printf("statements -> grammar\n"); }

statements
  : statement
    { printf("statement -> statements\n"); }
  | statements statement 
    { printf("statements statement -> statements\n"); }

block 
  : '{' '}'
    { printf("{ } -> block\n"); }
  | '{' statements '}'
    { printf("{ statements } -> block\n"); }

statement
  : SUB subname subdefargslist block
    { printf("sub subname subdefargslist block -> statement\n"); }
  | term ';'
    { printf("term ; -> statement\n") }
  | ';'
    { printf("; -> statement\n") }
  | if
    { printf("if -> statement\n"); }
  | declaration
  | FOR '(' declaration term ';' term ')' '{' statements '}'
    { printf("FOR ( declaration term ; term ) { statements }\n"); }
  | PACKAGE WORD ';'
    { printf("PACKAGE WORD ; -> statement\n"); }
  | LAST ';'
    { printf("LAST ; -> statement\n"); }
  | CONTINUE ';'
    { printf("CONTINUE ; -> statement\n"); }
  
declaration
  : declword declvar ';'
    { printf("declword declvar ; -> declaration\n"); }
  | declword declvar ASSIGNOP term ';'
    { printf("declword declvar = term ; -> declaration\n"); }
  | declword declvar ':' type ';'
    { printf("declword declvar : type ; -> declaration\n"); }
  | declword declvar ':' type ASSIGNOP term ';'
    { printf("declword declvar : type ASSIGNOP term ; -> declaration\n"); }

declword
  : MY
    { printf("MY -> declword\n"); }
  | OUR
    { printf("OUR -> declword\n"); }
  | HAS
    { printf("HAS -> declword\n"); }

declvar
  : VAR
    { printf("VAR -> declvar\n"); } /* my, our */
  | WORD
    { printf("VAR -> declvar\n"); } /* has */

if
  : IF '(' term ')' block
    { printf("if ( term ) block\n"); }
  | if else
    { printf("if else -> if\n"); }

else
  : ELSE block
    { printf("else block"); }
  | ELSIF '(' term ')' block else
    { printf("elsif ( term ) block else\n"); }

terms
  : term
    { printf("term -> terms\n"); }
  | terms ',' term
    { printf("terms , term -> terms\n"); }

term
  : VAR
    { printf("VAR -> term (%s)\n", ((SVOP*)$1)->uv.pv); }
  | term ADDOP term
    { printf("term + term -> term\n"); }
  | term MULOP term
    { printf("term * term -> term\n"); }
  | INT
    { printf("INT -> term (%d)\n", ((SVOP*)$1)->uv.iv); }
  | subname list
    { printf("subname list -> term\n"); }
  | RETURN term
    { printf("RETURN term -> term\n"); }
  | RETURN list
    { printf("RETURN list -> term\n"); }
  | term RELOP term
    { printf("term RELOP term%d\n", $2); }
  | term EQOP term
    { printf("term EQOP term\n"); }
  | term ASSIGNOP term
    { printf("term ASSIGNOP term\n"); }
  | term ANDOP term
    { printf("term ANDOP term -> term\n"); }
  | term OROP term
    { printf("term OROP term -> term\n"); }
  | term BITANDOP term
    { printf("term BITANDOP term -> term\n"); }
  | term BITOROP term
    { printf("term BITOROP term -> term\n"); }
  | SUB subdefargslist block
    { printf("SUB subdefargslist block -> term\n"); }
    
subname
  : WORD
    { printf("WORD -> subname\n"); }

list
  :  '(' ')'
    { printf("( ) -> list\n"); }
  | '(' terms ')'
    { printf("( terms ) -> list\n"); }

subdefargslist
  : '(' ')'
    { printf("( ) -> subdefargslist\n"); }
  | '(' subdefargs ')'
    { printf("( subdefargs ) -> subdefargslist\n"); }

subdefargs
  : subdefarg
    { printf("subdefarg -> subdefargs\n"); }
  | subdefargs ',' subdefarg
    { printf("subdefargs , subdefarg\n"); }

subdefarg
  : VAR ':' type
    { printf("VAR : type -> subdefarg (%s)\n", ((SVOP*)$1)->uv.pv); }

type
  : WORD
    { printf("WORD -> type (%s)\n", ((SVOP*)$1)->uv.pv); }

%%

struct yy_parser* parser;

/* Get token */
int yylex(void)
{
  char* buf_ptr = parser->buf_ptr;
  
  while(1) {
    /* Get current character */
    char c = *buf_ptr;
    
    /* line end */
    switch(c) {
      /* End */
      case '\0':
        return 0;
      
      /* Skip space character */
      case ' ':
      case '\t':
      case '\n':
      case '\r':
      case EOF :
        buf_ptr++;
        parser->buf_ptr = buf_ptr;
        continue;
      
      /* Addition */
      case '+':
        buf_ptr++;
        
        if (*buf_ptr == '=') {
          buf_ptr++;
          parser->buf_ptr = buf_ptr;
          yylval.ival = OP_ADD;
          return ASSIGNOP;
        }
        else {
          parser->buf_ptr = buf_ptr;
          yylval.ival = OP_ADD;
          return ADDOP;
        }
      
      /* Subtract */
      case '-':
        buf_ptr++;
        if (*buf_ptr == '=') {
          buf_ptr++;
          parser->buf_ptr = buf_ptr;
          yylval.ival = OP_SUBTRACT;
          return ASSIGNOP;
        }
        else {
          parser->buf_ptr = buf_ptr;
          yylval.ival = OP_SUBTRACT;
          return ADDOP;
        }
      /* Multiply */
      case '*':
        buf_ptr++;
        if (*buf_ptr == '=') {
          buf_ptr++;
          parser->buf_ptr = buf_ptr;
          yylval.ival = OP_MULTIPLY;
          return ASSIGNOP;
        }
        else {
          parser->buf_ptr = buf_ptr;
          yylval.ival = OP_MULTIPLY;
          return MULOP;
        }
      
      /* Divide */
      case '/':
        buf_ptr++;
        if (*buf_ptr == '=') {
          buf_ptr++;
          parser->buf_ptr = buf_ptr;
          yylval.ival = OP_DIVIDE;
          return ASSIGNOP;
        }
        else {
          parser->buf_ptr = buf_ptr;
          yylval.ival = OP_DIVIDE;
          return MULOP;
        }
      
      case '|':
        buf_ptr++;
        /* Or */
        if (*buf_ptr == '|') {
          buf_ptr++;
          parser->buf_ptr = buf_ptr;
          yylval.ival = OP_OR;
          return OROP;
        }
        /* Bit or */
        else {
          parser->buf_ptr = buf_ptr;
          yylval.ival = OP_BIT_OR;
          return BITOROP;
        }

      case '&':
        buf_ptr++;
        /* Or */
        if (*buf_ptr == '&') {
          buf_ptr++;
          parser->buf_ptr = buf_ptr;
          yylval.ival = OP_AND;
          return ANDOP;
        }
        /* Bit and */
        else {
          parser->buf_ptr = buf_ptr;
          yylval.ival = OP_BIT_AND;
          return BITANDOP;
        }
      
      /* Comment */
      case '#':
        buf_ptr++;
        while(1) {
          if (*buf_ptr == '\r' || *buf_ptr == '\n' || *buf_ptr == EOF || *buf_ptr == '\0') {
            break;
          }
          buf_ptr++;
        }
        parser->buf_ptr = buf_ptr;
        continue;
      
      case '=':
        buf_ptr++;
        
        /* == */
        if (*buf_ptr == '=') {
          buf_ptr++;
          
          parser->buf_ptr = buf_ptr;
          return EQOP;
        }
        /* = */
        else {
          parser->buf_ptr = buf_ptr;
          yylval.ival = 0;
          return ASSIGNOP;
        }
        
      case '<':
        buf_ptr++;
        
        /* <= */
        if (*buf_ptr == '=') {
          buf_ptr++;
          
          parser->buf_ptr = buf_ptr;
          yylval.ival = OP_LE;
          return RELOP;
        }
        /* < */
        else {
          parser->buf_ptr = buf_ptr;
          yylval.ival = OP_LT;
          return RELOP;
        }
      
      case '>':
        buf_ptr++;
        
        /* >= */
        if (*buf_ptr == '=') {
          buf_ptr++;
          
          parser->buf_ptr = buf_ptr;
          yylval.ival = OP_GE;
          return RELOP;
        }
        /* < */
        else {
          parser->buf_ptr = buf_ptr;
          yylval.ival = OP_GT;
          return RELOP;
        }
      
      default:
        /* Variable */
        if (c == '$') {
          /* Save current position */
          char* cur_token_ptr = buf_ptr;
          
          buf_ptr++;

          /* Next is graph */
          while(isalnum(*buf_ptr)) {
            buf_ptr++;
          }

          size_t str_len = buf_ptr - cur_token_ptr;
          char* var = malloc(str_len + 1);
          memcpy(var, cur_token_ptr, str_len);
          var[str_len] = '\0';
          
          SVOP* op = malloc(sizeof(SVOP));
          op->type = OP_CONST_STRING;
          op->uv.pv = var;
          
          parser->buf_ptr = buf_ptr;
          yylval.opval = (OP*)op;
          return VAR;
        }
        /* Number */
        else if (isdigit(c)) {
          char* cur_token_ptr = buf_ptr;
          
          buf_ptr++;
          
          /* Scan number */
          while(isdigit(*buf_ptr)) {
            buf_ptr++;
          }
          
          if (isalpha(*buf_ptr)) {
            fprintf(stderr, "syntax error: invalid variable name\n");
            exit(1);
          }
          
          /* Convert to integer */
          size_t str_len = buf_ptr - cur_token_ptr;
          char* num_str = malloc(str_len + 1);
          memcpy(num_str, cur_token_ptr, str_len);
          num_str[str_len] = '\0';
          int num = atoi(num_str);
          free(num_str);
          
          SVOP* op = malloc(sizeof(SVOP));
          op->type = OP_CONST_INT;
          op->uv.iv = num;
          
          parser->buf_ptr = buf_ptr;
          yylval.opval = (OP*)op;
          return INT;
        }
        /* Keyword or word */
        else if (isalnum(c)) {
          /* Save current position */
          char* cur_token_ptr = buf_ptr;
          
          buf_ptr++;
          
          /* Next is graph */
          while(isalnum(*buf_ptr)) {
            buf_ptr++;
          }
          
          size_t str_len = buf_ptr - cur_token_ptr;
          char* keyword = malloc(str_len + 1);
          memcpy(keyword, cur_token_ptr, str_len);
          keyword[str_len] = '\0';
          
          if (memcmp(keyword, "my", str_len) == 0) {
            parser->buf_ptr = buf_ptr;
            return MY;
          }
          else if (memcmp(keyword, "our", str_len) == 0) {
            parser->buf_ptr = buf_ptr;
            return OUR;
          }
          else if (memcmp(keyword, "has", str_len) == 0) {
            parser->buf_ptr = buf_ptr;
            return HAS;
          }
          else if (memcmp(keyword, "sub", str_len) == 0) {
            parser->buf_ptr = buf_ptr;
            return SUB;
          }
          else if (memcmp(keyword, "package", str_len) == 0) {
            parser->buf_ptr = buf_ptr;
            return PACKAGE;
          }
          else if (memcmp(keyword, "if", str_len) == 0) {
            parser->buf_ptr = buf_ptr;
            return IF;
          }
          else if (memcmp(keyword, "elsif", str_len) == 0) {
            parser->buf_ptr = buf_ptr;
            return ELSIF;
          }
          else if (memcmp(keyword, "else", str_len) == 0) {
            parser->buf_ptr = buf_ptr;
            return ELSE;
          }
          else if (memcmp(keyword, "return", str_len) == 0) {
            parser->buf_ptr = buf_ptr;
            return RETURN;
          }
          else if (memcmp(keyword, "for", str_len) == 0) {
            parser->buf_ptr = buf_ptr;
            return FOR;
          }
          else if (memcmp(keyword, "last", str_len) == 0) {
            parser->buf_ptr = buf_ptr;
            return LAST;
          }
          else if (memcmp(keyword, "continue", str_len) == 0) {
            parser->buf_ptr = buf_ptr;
            return CONTINUE;
          }
          
          SVOP* op = malloc(sizeof(SVOP));
          op->type = OP_CONST_STRING;
          op->uv.pv = keyword;
          
          parser->buf_ptr = buf_ptr;
          yylval.opval = (OP*)op;
          return WORD;
        }
        
        /* Return character */
        buf_ptr++;
        parser->buf_ptr = buf_ptr;
        return c;
    }
  }
}

/* Function for error */
void yyerror(const char* s)
{
  fprintf(stderr, "error: %s\n", s);
}

int main(int argc, char *argv[])
{
  if (argc < 2) {
    fprintf(stderr, "Not script\n");
    exit(1);
  }
  
  /* Source file */
  char* file = argv[1];
  
  /* Open source file */
  FILE* fp = fopen(file, "r");
  if (fp == NULL) {
    printf("Can't open file %s\n", file);
    return -1;
  }
  
  /* initialize parser */
  parser = malloc(sizeof(struct yy_parser));

  /* Read source file */
  parser->linestr_len = getdelim(&(parser->linestr), &(parser->linestr_buf_len), EOF, fp);
  
  /* Close file */
  fclose(fp);
  
  /* Initialize parser information */
  parser->buf_ptr = parser->linestr;
  parser->bef_token_ptr = parser->linestr;
  
  /* call yyparse */
  int parse_success = yyparse();
  
  free(parser->linestr);
  
  return parse_success;
}
