/* TC
 * id:    pfv_probe_pointer_alt_flag
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   win10
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   none (undocumented)
 * note:  PROBES: %#p gives 0X-prefixed zero-padded text; %20p right-justifies; %-20p left-justifies.
 */
#include "tc.h"
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <string.h>

#if TC_X64
#define S8 "00000000"
#define SP "    "
#else
#define S8 ""
#define SP "            "
#endif
static char b[64];
TC_CASE(hash_p) { void *volatile p = (void *)(size_t)0x1234; sprintf(b, "%#p", p); CHECK_STR(b, "0X" S8 "00001234"); }
TC_CASE(width_20_p) { void *volatile p = (void *)(size_t)0x1234; sprintf(b, "[%20p]", p); CHECK_STR(b, "[" SP S8 "00001234]"); }
TC_CASE(minus_p) { void *volatile p = (void *)(size_t)0x1234; sprintf(b, "[%-20p]", p); CHECK_STR(b, "[" S8 "00001234" SP "]"); }

int main(void) { tc_begin(); TC_RUN(hash_p); TC_RUN(width_20_p); TC_RUN(minus_p); return tc_end(); }
