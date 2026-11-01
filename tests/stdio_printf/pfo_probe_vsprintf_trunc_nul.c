/* TC
 * id:    pfo_probe_vsprintf_trunc_nul
 * area:  stdio_printf
 * funcs: __stdio_common_vsprintf
 * crt:   ucrtbase
 * tier:  p2
 * kind:  positive
 * conf:  observed
 * ref:   MS docs _snprintf remarks
 * note:  options=0 NUL-termination state on truncation and on exact fit (count == len). Observed Win10 22H2 ucrtbase raw worker __stdio_common_vsprintf(0,...): the buffer is ALWAYS NUL-terminated (the last char is dropped: 'abcdefg\0'), and the return on truncation / exact fit is -2 (not len, not -1).
 *        The legacy no-NUL / return-len behaviour of _snprintf needs the LEGACY_VSPRINTF_NULL_TERMINATION option bit, which the SDK inline _vsnprintf passes.
 *        Raw evidence (baseline): exact_fit_return n == 8 got=-2 want=8 ; exact_fit_no_nul b[7] == 'h' got=0 ; trunc_last_byte b[3] == 'd' got=0.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

static int vs(unsigned long long o, char *b, size_t n, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = __stdio_common_vsprintf(o, b, n, f, NULL, ap); va_end(ap); return r; }
static volatile int vi = 42;

TC_CASE(exact_fit_return)
{
    char b[16]; int n;
    memset(b, 'Z', sizeof b);
    n = vs(0, b, 8, "abcdefgh");
    CHECK_EQ_I(n, -2);   /* observed: raw worker returns -2 when the output (8) does not fit count 8 + NUL; SDK inline wrappers (_vsnprintf) map <0 to -1 */
}
TC_CASE(exact_fit_no_nul)
{
    char b[16];
    memset(b, 'Z', sizeof b);
    vs(0, b, 8, "abcdefgh");
    CHECK_EQ_I(b[6], 'g'); CHECK_EQ_I(b[7], 0); CHECK_EQ_I(b[8], 'Z');   /* observed: options 0 always NUL-terminates, dropping the last char */
}
TC_CASE(trunc_last_byte)
{
    char b[16];
    memset(b, 'Z', sizeof b);
    vs(0, b, 4, "abcdefgh");
    CHECK_EQ_I(b[2], 'c'); CHECK_EQ_I(b[3], 0); CHECK_EQ_I(b[4], 'Z');
}
int main(void)
{
    tc_begin();
    TC_RUN(exact_fit_return); TC_RUN(exact_fit_no_nul); TC_RUN(trunc_last_byte);
    return tc_end();
}
