/* TC
 * id:    pfw_neg_swprintf_c99_count0_probe
 * area:  stdio_printf
 * funcs: swprintf
 * crt:   ucrtbase
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs swprintf (count form)
 * note:  PROBE: swprintf(buf, 0, ...) with a non-NULL buffer. Only asserts the weak disjunction: either returns -1 or the invalid parameter handler ran (exact result, errno and handler behaviour unknown). Also asserts the buffer is not written.
 */
#include "tc.h"
#include <stdarg.h>
#include <errno.h>
#include <wchar.h>

static const wchar_t * volatile vfmt = L"%d";
TC_CASE(count0)
{
    wchar_t b[8]; int n, i; for (i = 0; i < 8; i++) b[i] = L'Z';
    tc_iph_count = 0; n = swprintf(b, 0, vfmt, 1);
    CHECK(n == -1 || tc_iph_count > 0);
    CHECK(b[0] == L'Z');
}
int main(void) { tc_begin(); CHECK(tc_iph_install()); TC_RUN(count0); return tc_end(); }
