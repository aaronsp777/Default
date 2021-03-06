#include <ctype.h>
#include <stdio.h>

char *tokens[] = {
  "END", "FOR", "NEXT", "DATA", "INPUT#", "INPUT", "DIM", "READ", "LET",
  "GOTO", "RUN", "IF", "RESTORE", "GOSUB", "RETURN", "REM", "STOP", "ON",
  "WAIT", "LOAD", "SAVE", "VERIFY", "DEF", "POKE", "PRINT#", "PRINT", "CONT",
  "LIST", "CLR", "CMD", "SYS", "OPEN", "CLOSE", "GET", "NEW", "TAB(", "TO",
  "FN", "SPC(", "THEN", "NOT", "STEP", "+", "-", "*", "/", "^", "AND", "OR",
  ">", "=", "<", "SGN", "INT", "ABS", "USR", "FRE", "POS", "SQR", "RND", "LOG",
  "EXP", "COS", "SIN", "TAN", "ATN", "PEEK", "LEN", "STR$", "VAL", "ASC",
  "CHR$", "LEFT$", "RIGHT$", "MID$", "GO"
};

int readpair() {
  unsigned int low = getchar();
  unsigned int high = getchar();
  return low + 256 * high;
}

void decode_basic() {
  
  readpair(); // load address

  while(1) {
    int inside_quotes = 0;
    if (!readpair())
      return;

    printf("%d ", readpair());  // Line number

    while(1) {
      unsigned int c = getchar();
      if (c == EOF) {
        fprintf(stderr, "Truncated input\n");
        return;  // stream broken
      }
      if (c == 0) {
        printf("\n");
        break;
      }
      if (c == '"') {
        inside_quotes = !inside_quotes;
      }
      if (c >= 0x80 && c < 0xcc && !inside_quotes) {
        printf("%s", tokens[c - 0x80]);
      } else {
        printf("%c", c);
      }
    }
  }
}

int main(int argc, char* argv[]) {
  decode_basic();
}
