/* TC
 * id:    chr_neg_nolock_null_stream
 * area:  stdio_char
 * funcs: _getc_nolock _putc_nolock
 * crt:   win10
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _getc_nolock, _putc_nolock
 * note:  PROBE: whether the UCRT nolock variants validate a NULL stream (EOF, EINVAL, invalid-parameter handler)
 *        rather than crashing.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static FILE *volatile nf;
static int child_main(const char *n)
{
    int c0; int r = 0;
    tc_iph_install();
    c0 = tc_iph_count; errno = 0;
    if (n[0] == 'a') { r = (_getc_nolock)(nf); }
    else { r = (_putc_nolock)('a', nf); }
    if (r != EOF) return 3;
    if (tc_iph_count - c0 != 1) return 1;
    return errno == EINVAL ? 0 : 2;
}
static void run(const char *n)
{
    DWORD code = 99;
    CHECK(tc_spawn_self(n, &code, 20000));
    CHECK_EQ_U(code, 0);
}
TC_CASE(getc_nolock_null) { run("a"); }
TC_CASE(putc_nolock_null) { run("b"); }
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name())) return child_main(c);
    TC_RUN(getc_nolock_null); TC_RUN(putc_nolock_null);
    return tc_end();
}
