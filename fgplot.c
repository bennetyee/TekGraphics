#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
/* Tek fast-graphics (405x) to plot converter - Bsy, 4/29/87 */

/* no plot library */
#define ALWAYS  0
#define TEK4014 0
#define outArr(arr) do { fwrite(arr, 1, sizeof arr, stdout); } while(0)

void  erase(void) {
  static unsigned char  eraseTek[] = {
    033, 014,
  };
  outArr(eraseTek);
}

void  openpl(void) {
  static unsigned char  toTek[] = {
    033, '[', '?', '3', '8', 'h', 037,
  };
  outArr(toTek);
  erase();
}

void  closepl(void) {
  static unsigned char  toVT[] = {
    033, 3,
  };
  fflush(stdout);
  /*
   * xterm Tek emulator drops final graphics ops if we switched to VT100
   * immediately
   */
  usleep(1000);
  outArr(toVT);
}

int hi_x = 0, lo_x = 0, hi_y = 0, lo_y = 0;
#if TEK4014
int lsb = 0;
#endif

void  cont(int x, int y) {
  int hx, lx, hy, ly;

#if TEK4014
  int nlsb;

  /* assume 4096 resolution, so 12 bits */
  hx = ((x >> 7) & 0x1f) + 0x20;
  lx = ((x >> 2) & 0x1f) + 0x40;
  hy = ((y >> 7) & 0x1f) + 0x20;
  ly = ((y >> 2) & 0x1f) + 0x60;
  nlsb = (((y & 0x3) << 2) | (x & 0x3)) + 0x60;
#else
  /* assume 1024 resolution, so 10 bits */
  hx = ((x >> 5) & 0x1f) + 0x20;
  lx = (x & 0x1f) + 0x40;
  hy = ((y >> 5) & 0x1f) + 0x20;
  ly = (y & 0x1f) + 0x60;
#endif
  if (hy != hi_y || ALWAYS)
    putchar(hy);
#if TEK4014
  if (nlsb != lsb || ALWAYS)
    putchar(nlsb);
#endif
  if (ly != lo_y || hx != hi_x ||
#if TEK4014
      nlsb != lsb ||
#endif
      ALWAYS)
    putchar(ly);
  if (hx != hi_x || ALWAYS)
    putchar(hx);
  putchar(lx);
  hi_x = hx;
  lo_x = lx;
  hi_y = hy;
  lo_y = ly;
#if TEK4014
  lsb = nlsb;
#endif
}

void  move(int x, int y) {
  putchar(035);
  cont(x,y);
}

int main(void) {
  char    code[3];
  int   m,x,y;

  openpl();
  erase();
  while (fread(code,1,3,stdin) == 3) {
    m = code[0] & 0100;
    x = ((code[0] & 070) << 4) | code[1];
    y = ((code[0] & 007) << 7) | code[2];
    if (m) {
      move(x,y);
    } else cont(x,y);
  }
  closepl();
  exit(0);
}
