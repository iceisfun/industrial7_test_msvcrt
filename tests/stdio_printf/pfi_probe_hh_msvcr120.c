/* TC
 * id:    pfi_probe_hh_msvcr120
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   msvcr120
 * tier:  p1
 * conf:  probe
 * ref:   C99 7.19.6.1 hh; VS2013 CRT claimed C99 printf support
 * note:  PROBE: whether msvcr120.dll's printf handles the hh length modifier (truncating to char). Expected yes.
 */
#include "tc.h"
#include <stdio.h>

TC_CASE(hh127) { char b[32]; sprintf(b, "%hhd", 127); CHECK_STR(b, "127"); }
TC_CASE(hh300) { char b[32]; volatile int v = 300; sprintf(b, "%hhd", v); CHECK_STR(b, "44"); }
TC_CASE(hhu) { char b[32]; volatile int v = -1; sprintf(b, "%hhu", v); CHECK_STR(b, "255"); }
int main(void) { tc_begin(); TC_RUN(hh127); TC_RUN(hh300); TC_RUN(hhu); return tc_end(); }
