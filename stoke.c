#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sperl.h"
#include "sperl.tab.h"

/* Get token */
int yylex(YYSTYPE* yylvalp, yy_parser* parser)
{
  char* bufptr = parser->bufptr;
  
  while(1) {
    /* Get current character */
    char c = *bufptr;
    
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
        bufptr++;
        parser->bufptr = bufptr;
        continue;
      
      /* Addition */
      case '+':
        bufptr++;
        
        if (*bufptr == '+') {
          bufptr++;
          parser->bufptr = bufptr;
          yylvalp->ival = OP_INC;
          return INCOP;
        }
        else if (*bufptr == '=') {
          bufptr++;
          parser->bufptr = bufptr;
          yylvalp->ival = OP_ADD;
          return ASSIGNOP;
        }
        else {
          parser->bufptr = bufptr;
          return '+';
        }
      
      /* Subtract */
      case '-':
        bufptr++;
        
        if (*bufptr == '>') {
          bufptr++;
          parser->bufptr = bufptr;
          return ARROW;
        }
        else if (*bufptr == '-') {
          bufptr++;
          parser->bufptr = bufptr;
          yylvalp->ival = OP_DEC;
          return DECOP;
        }
        else if (*bufptr == '=') {
          bufptr++;
          parser->bufptr = bufptr;
          yylvalp->ival = OP_SUBTRACT;
          return ASSIGNOP;
        }
        else {
          parser->bufptr = bufptr;
          return '-';
        }
      /* Multiply */
      case '*':
        bufptr++;
        if (*bufptr == '=') {
          bufptr++;
          parser->bufptr = bufptr;
          yylvalp->ival = OP_MULTIPLY;
          return ASSIGNOP;
        }
        else {
          parser->bufptr = bufptr;
          yylvalp->ival = OP_MULTIPLY;
          return MULOP;
        }
      
      /* Divide */
      case '/':
        bufptr++;
        if (*bufptr == '=') {
          bufptr++;
          parser->bufptr = bufptr;
          yylvalp->ival = OP_DIVIDE;
          return ASSIGNOP;
        }
        else {
          parser->bufptr = bufptr;
          yylvalp->ival = OP_DIVIDE;
          return MULOP;
        }

      case '%':
        bufptr++;
        if (*bufptr == '=') {
          bufptr++;
          parser->bufptr = bufptr;
          yylvalp->ival = OP_MODULO;
          return ASSIGNOP;
        }
        else {
          parser->bufptr = bufptr;
          yylvalp->ival = OP_MODULO;
          return MULOP;
        }

      case '^':
        bufptr++;
        if (*bufptr == '=') {
          bufptr++;
          parser->bufptr = bufptr;
          yylvalp->ival = OP_BIT_XOR;
          return ASSIGNOP;
        }
        else {
          parser->bufptr = bufptr;
          yylvalp->ival = OP_BIT_XOR;
          return MULOP;
        }
            
      case '|':
        bufptr++;
        /* Or */
        if (*bufptr == '|') {
          bufptr++;
          if (*bufptr == '=') {
            bufptr++;
            parser->bufptr = bufptr;
            yylvalp->ival = OP_OR;
            return ASSIGNOP;
          }
          else {
            parser->bufptr = bufptr;
            yylvalp->ival = OP_OR;
            return OROP;
          }
        }
        /* Bit or */
        else {
          parser->bufptr = bufptr;
          yylvalp->ival = OP_BIT_OR;
          return BITOROP;
        }

      case '&':
        bufptr++;
        /* Or */
        if (*bufptr == '&') {
          bufptr++;
          if (*bufptr == '=') {
            bufptr++;
            parser->bufptr = bufptr;
            yylvalp->ival = OP_AND;
            return ASSIGNOP;
          }
          else {
            parser->bufptr = bufptr;
            yylvalp->ival = OP_AND;
            return ANDOP;
          }
        }
        /* Bit and */
        else {
          parser->bufptr = bufptr;
          yylvalp->ival = OP_BIT_AND;
          return BITANDOP;
        }
      
      /* Comment */
      case '#':
        bufptr++;
        while(1) {
          if (*bufptr == '\r' || *bufptr == '\n' || *bufptr == EOF || *bufptr == '\0') {
            break;
          }
          bufptr++;
        }
        parser->bufptr = bufptr;
        continue;
      
      case '=':
        bufptr++;
        
        /* == */
        if (*bufptr == '=') {
          bufptr++;
          
          parser->bufptr = bufptr;
          return EQOP;
        }
        /* = */
        else {
          parser->bufptr = bufptr;
          yylvalp->ival = 0;
          return ASSIGNOP;
        }
        
      case '<':
        bufptr++;
        
        if (*bufptr == '<') {
          bufptr++;
          
          parser->bufptr = bufptr;
          yylvalp->ival = OP_LEFT_SHIFT;
          return SHIFTOP;
        }
        /* <= */
        else if (*bufptr == '=') {
          bufptr++;
          
          parser->bufptr = bufptr;
          yylvalp->ival = OP_LE;
          return RELOP;
        }
        /* < */
        else {
          parser->bufptr = bufptr;
          yylvalp->ival = OP_LT;
          return RELOP;
        }
      
      case '>':
        bufptr++;
        
        if (*bufptr == '>') {
          bufptr++;
          
          parser->bufptr = bufptr;
          yylvalp->ival = OP_RIGHT_SHIFT;
          return SHIFTOP;
        }
        /* >= */
        else if (*bufptr == '=') {
          bufptr++;
          
          parser->bufptr = bufptr;
          yylvalp->ival = OP_GE;
          return RELOP;
        }
        /* < */
        else {
          parser->bufptr = bufptr;
          yylvalp->ival = OP_GT;
          return RELOP;
        }
      case '!':
        bufptr++;
        
        if (*bufptr == '=') {
          bufptr++;
          parser->bufptr = bufptr;
          yylvalp->ival = OP_NOT;
          return ASSIGNOP;
        }
        else {
          parser->bufptr = bufptr;
          yylvalp->ival = OP_NOT;
          return NOTOP;
        }
        
      case '~':
        bufptr++;
        
        parser->bufptr = bufptr;
        yylvalp->ival = OP_BIT_NOT;
        return BITNOTOP;
      
      case '"':
        bufptr++;
        
        /* Save current position */
        char* cur_token_ptr = bufptr;
        
        char* str;
        if (*(bufptr + 1) == '"') {
          str = malloc(1);
          str[0] = '\0';
          bufptr++;
          bufptr++;
        }
        else {
          while(*bufptr != '"' && *bufptr != '\0') {
            bufptr++;
          }
          if (*bufptr == '\0') {
            fprintf(stderr, "syntax error: string don't finish\n");
            exit(1);
          }
          
          size_t str_len = bufptr - cur_token_ptr;
          str = malloc(str_len + 1);
          memcpy(str, cur_token_ptr, str_len);
          str[str_len] = '\0';

          bufptr++;
        }
        
        SVOP* op = malloc(sizeof(SVOP));
        op->type = OP_CONST_STRING;
        op->uv.pv = str;
        
        parser->bufptr = bufptr;
        yylvalp->opval = (OP*)op;
        return STRING;
      
      default:
        /* Variable */
        if (c == '$') {
          /* Save current position */
          char* cur_token_ptr = bufptr;
          
          bufptr++;

          /* Next is graph */
          while(isalnum(*bufptr) || (*bufptr) == '_' || (*bufptr) == ':') {
            bufptr++;
          }

          size_t str_len = bufptr - cur_token_ptr;
          char* var = malloc(str_len + 1);
          memcpy(var, cur_token_ptr, str_len);
          var[str_len] = '\0';
          
          SVOP* op = malloc(sizeof(SVOP));
          op->type = OP_CONST_STRING;
          op->uv.pv = var;
          
          parser->bufptr = bufptr;
          yylvalp->opval = (OP*)op;
          return VAR;
        }
        /* Number literal */
        else if (isdigit(c)) {
          char* cur_token_ptr = bufptr;
          
          bufptr++;
          
          /* Scan number */
          while(isdigit(*bufptr)) {
            bufptr++;
          }
          
          if (isalpha(*bufptr)) {
            fprintf(stderr, "syntax error: invalid variable name\n");
            exit(1);
          }
          
          /* Convert to integer */
          size_t str_len = bufptr - cur_token_ptr;
          char* num_str = malloc(str_len + 1);
          memcpy(num_str, cur_token_ptr, str_len);
          num_str[str_len] = '\0';
          int num = atoi(num_str);
          free(num_str);
          
          SVOP* op = malloc(sizeof(SVOP));
          op->type = OP_CONST_INT;
          op->uv.iv = num;
          
          parser->bufptr = bufptr;
          yylvalp->opval = (OP*)op;
          return INT;
        }
        /* Keyword or word */
        else if (isalpha(c) || c == '_') {
          /* Save current position */
          char* cur_token_ptr = bufptr;
          
          bufptr++;
          
          while(isalnum(*bufptr) || (*bufptr) == '_' || (*bufptr) == ':') {
            bufptr++;
          }
          
          size_t str_len = bufptr - cur_token_ptr;
          char* keyword = malloc(str_len + 1);
          memcpy(keyword, cur_token_ptr, str_len);
          keyword[str_len] = '\0';
          
          if (memcmp(keyword, "my", str_len) == 0) {
            parser->bufptr = bufptr;
            return MY;
          }
          else if (memcmp(keyword, "our", str_len) == 0) {
            parser->bufptr = bufptr;
            return OUR;
          }
          else if (memcmp(keyword, "has", str_len) == 0) {
            parser->bufptr = bufptr;
            return HAS;
          }
          else if (memcmp(keyword, "sub", str_len) == 0) {
            parser->bufptr = bufptr;
            return SUB;
          }
          else if (memcmp(keyword, "package", str_len) == 0) {
            parser->bufptr = bufptr;
            return PACKAGE;
          }
          else if (memcmp(keyword, "if", str_len) == 0) {
            parser->bufptr = bufptr;
            return IF;
          }
          else if (memcmp(keyword, "elsif", str_len) == 0) {
            parser->bufptr = bufptr;
            return ELSIF;
          }
          else if (memcmp(keyword, "else", str_len) == 0) {
            parser->bufptr = bufptr;
            return ELSE;
          }
          else if (memcmp(keyword, "return", str_len) == 0) {
            parser->bufptr = bufptr;
            return RETURN;
          }
          else if (memcmp(keyword, "for", str_len) == 0) {
            parser->bufptr = bufptr;
            return FOR;
          }
          else if (memcmp(keyword, "last", str_len) == 0) {
            parser->bufptr = bufptr;
            return LAST;
          }
          else if (memcmp(keyword, "continue", str_len) == 0) {
            parser->bufptr = bufptr;
            return CONTINUE;
          }
          else if (memcmp(keyword, "use", str_len) == 0) {
            parser->bufptr = bufptr;
            return USE;
          }
          else if (memcmp(keyword, "while", str_len) == 0) {
            parser->bufptr = bufptr;
            return WHILE;
          }
          else if (memcmp(keyword, "true", str_len) == 0) {
            SVOP* op = malloc(sizeof(SVOP));
            op->type = OP_CONST_BOOL;
            op->uv.iv = 1;

            parser->bufptr = bufptr;
            yylvalp->opval = (OP*)op;

            return BOOL;
          }
          else if (memcmp(keyword, "false", str_len) == 0) {
            SVOP* op = malloc(sizeof(SVOP));
            op->type = OP_CONST_BOOL;
            op->uv.iv = 0;

            parser->bufptr = bufptr;
            yylvalp->opval = (OP*)op;

            return BOOL;
          }
          
          SVOP* op = malloc(sizeof(SVOP));
          op->type = OP_CONST_STRING;
          op->uv.pv = keyword;
          
          parser->bufptr = bufptr;
          yylvalp->opval = (OP*)op;
          return WORD;
        }
        
        /* Return character */
        bufptr++;
        parser->bufptr = bufptr;
        return c;
    }
  }
}

/* Function for error */
void yyerror(yy_parser* parser, const char* s)
{
  fprintf(stderr, "error: %s\n", s);
}
