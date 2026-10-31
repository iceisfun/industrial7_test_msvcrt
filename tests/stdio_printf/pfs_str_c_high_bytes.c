/* TC
 * id:    pfs_str_c_high_bytes
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.21.6.1p8 (c: int converted to unsigned char)
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>

static volatile int e9 = 0xE9, ff = 0xFF, x80 = 0x80, x141 = 0x141, a256 = 'a' + 256;
TC_CASE(e9) { char b[16]; int n = sprintf(b, "%c", e9); CHECK_EQ_I(n, 1); CHECK_EQ_U((unsigned char)b[0], 0xE9); CHECK_EQ_I(b[1], 0); }
TC_CASE(ff_80) { char b[16]; int n = sprintf(b, "%c%c", ff, x80); CHECK_EQ_I(n, 2); CHECK_EQ_U((unsigned char)b[0], 0xFF); CHECK_EQ_U((unsigned char)b[1], 0x80); }
TC_CASE(wrap141) { char b[16]; int n = sprintf(b, "%c", x141); CHECK_EQ_I(n, 1); CHECK_EQ_I(b[0], 'A'); }
TC_CASE(wrap_a) { char b[16]; int n = sprintf(b, "%c", a256); CHECK_EQ_I(n, 1); CHECK_EQ_I(b[0], 'a'); }
TC_CASE(str_bytes) { static const char s[] = "\xe9\xff"; char b[16]; int n = sprintf(b, "%s", (const char *volatile)s); CHECK_EQ_I(n, 2); CHECK_MEM(b, "\xe9\xff\0", 3); }
int main(void) { tc_begin(); TC_RUN(e9); TC_RUN(ff_80); TC_RUN(wrap141); TC_RUN(wrap_a); TC_RUN(str_bytes); return tc_end(); }
