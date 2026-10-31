/* TC
 * id:    pfm_3_conv_p_len_probe
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   ucrtbase msvcr120
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   MS docs printf type field p; length modifiers on p
 * note:  PROBING: %Lp %wp %Ip %I64p %jp %tp each behave exactly like plain %p (uppercase hex, zero padded to 2*sizeof(void*) digits) and consume exactly one pointer argument (trailing %d must still print 7).
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

static char b[128];
static volatile uintptr_t pv = 0xABCDEF;
#ifdef _WIN64
#define PTR "0000000000ABCDEF"
#else
#define PTR "00ABCDEF"
#endif
#define BODY_T(f) b[0] = 0; tc_iph_install(); sprintf(b, f "|%d", (void *)pv, 7); CHECK_STR(b, PTR "|7");
TC_CASE(plain) { BODY_T("%p") }
TC_CASE(Lp) { BODY_T("%Lp") }
TC_CASE(wp) { BODY_T("%wp") }
TC_CASE(Ip) { BODY_T("%Ip") }
TC_CASE(I64p) { BODY_T("%I64p") }
TC_CASE(jp) { BODY_T("%jp") }
TC_CASE(tp) { BODY_T("%tp") }
int main(void) { tc_begin(); TC_RUN(plain); TC_RUN(Lp); TC_RUN(wp); TC_RUN(Ip); TC_RUN(I64p); TC_RUN(jp); TC_RUN(tp); return tc_end(); }
