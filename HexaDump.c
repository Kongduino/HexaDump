#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void hexDump(char* buf, unsigned int len, unsigned int offset) {
  char alphabet[17] = "0123456789abcdef";
  uint16_t i, index;
  printf("%s", "     ▛▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▜ ▛▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▜\n");
  printf("%s", "     ▎.0 .1 .2 .3 .4 .5 .6 .7 .8 .9 .a .b .c .d .e .f ▕ ▎      ASCII     ▕\n");
  printf("%s", "     ▙▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▟ ▙▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▟\n");
  for (i = 0; i < len; i += 16) {
    char s[] = "|                                                | |                |\n";
    // pre-formated line. We will replace the spaces with text when appropriate.
    uint8_t ix = 1, iy = 52, j;
    for (j = 0; j < 16; j++) {
      if (i + j < len) {
        uint8_t c = buf[i + j];
        // fastest way to convert a byte to its 2-digit hex equivalent
        s[ix++] = alphabet[(c >> 4) & 0x0F];
        s[ix++] = alphabet[c & 0x0F];
        ix++;
        if (c > 31 && c < 127) s[iy++] = c;
        else s[iy++] = '.'; // display ASCII code 0x20-0x7F or a dot.
      }
    }
    index = (i + offset) / 16;
    // display line number then the text
    if (index < 4096) printf(" ");
    printf("%03x.%s", index, s);
  }
  printf("%s", "     ▙▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▟ ▙▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▟\n");
}

int main(int argc, char** argv) {
  char fname[256], line[256];
  FILE* fin;
  int i, j, len, thisLen, offset = 0;
  int nameSet = 0;
  size_t optind = 1;
  while (optind < argc && argv[optind][0] == '-') {
    // printf("arg: %s\n", argv[optind]);
    switch (argv[optind][1]) {
      case 'f':
        // printf("param: %s\n", argv[optind+1]);
        strcpy(fname, argv[optind + 1]);
        printf("File name: %s\n", fname);
        nameSet = 1;
        optind += 2;
        break;
      default:
        fprintf(stderr, "Usage: %s [-s addr] [-f file...]\n", argv[0]);
        exit(-1);
    }
  }
  if (nameSet == 0) {
    printf("Enter the file name: ");
    scanf("%s", fname);
  }
  fin = fopen(fname, "r");
  if (!fin) {
    printf("Error open file:%s\n", fname);
    return -1;
  } else {
    fseek(fin, 0L, SEEK_END);
    unsigned int len = ftell(fin);
    rewind(fin);
    do {
      if (len > 255) thisLen = 256;
      else thisLen = len;
      thisLen = fread(line, 1, thisLen, fin);
      hexDump(line, thisLen, offset);
      offset += thisLen;
      len -= thisLen;
    } while (len > 0);
  }
}
