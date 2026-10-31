/* TC
 * id:    pfm_1_len_i_msvcrt_probe
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   msvcrt
 * tier:  p1
 * conf:  probe
 * ref:   MS docs printf Size prefixes (I, I32, I64)
 * note:  PROBES the Windows 7 msvcrt.dll: I32 and I64 are expected to work ("-5", "-5000000000"); plain I (pointer-sized) expected to work too.
 */
#include "tc.h"
#include <stdio.h>
#include <stddef.h>

static char b[64];
TC_CASE(I32i) { volatile int v = -5; sprintf(b, "%I32i", v); CHECK_STR(b, "-5"); }
TC_CASE(I64i) { volatile long long v = -5000000000LL; sprintf(b, "%I64i", v); CHECK_STR(b, "-5000000000"); }
TC_CASE(Iu)   { volatile size_t v = 7; sprintf(b, "%Iu", v); CHECK_STR(b, "7"); }

int main(void) { tc_begin(); TC_RUN(I32i); TC_RUN(I64i); TC_RUN(Iu); return tc_end(); }
