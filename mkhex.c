#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

int usage(char *progname) {
  fprintf(stderr, "Usage: %s [-l addr] [-e addr] input_file output file\n", progname);
  return -1;
}

int main(int argc, char *argv[]) {
  char *srcfn = NULL, *dstfn = NULL;
  FILE *src, *dst;
  int  i, nb, addr = 0, ept = 0;
  unsigned cks, b;
  unsigned char bfr[16];

  if (argc < 3) {
    return usage(argv[0]);
  }
  
  for (i = 1; i < argc; ++i) {
    if (strcmp(argv[i], "-l") == 0) {
      if (++i == argc) return usage(argv[0]);
      addr = strtol(argv[i], NULL, 0);
    } else if (strcmp(argv[i], "-e") == 0) {
      if (++i == argc) return usage(argv[0]);
      ept = strtol(argv[i], NULL, 0);
    } else {
      if (!srcfn) {
        srcfn = argv[i];
      } else if (!dstfn) {
        dstfn = argv[i];
      } else {
        return usage(argv[0]);
      }
    }
  }

  src = fopen(srcfn, "rb");
  if (src == NULL) {
    fprintf(stderr, "Could not open source file \"%s\": %s\n\n",
            srcfn, strerror(errno));
    return -1;
  }

  dst = fopen(dstfn, "w");
  if (dst == NULL) {
    fprintf(stderr, "Could not create destination file \"%s\": %s\n\n",
            dstfn, strerror(errno));
    fclose(src);
    return -1;
  }

  while (!feof(src)) {
    nb = fread(&bfr, 1, 16, src);
    if (nb == 0) break;
    fprintf(dst, ":%02X%04X00", nb, addr);
    cks = nb + ((addr >> 8) & 0xFF) + (addr & 0xFF);
    for (i = 0; i < nb; ++i) {
      cks += bfr[i];
      fprintf(dst, "%02X", (unsigned char) bfr[i]);
    }
    fprintf(dst, "%02X\n", (unsigned char) ((256-cks) & 0xFF));
    addr += nb;
  }

#if 0
  fprintf(dst, ":0000000000\n");
#else
  cks = 0x01 + ((ept >> 8) & 0xFF) + (ept & 0xFF);
  fprintf(dst, ":00%04X01%02X\n", ept, (unsigned char) ((256-cks) & 0xFF));
#endif

  fclose(src);
  fclose(dst);
}
