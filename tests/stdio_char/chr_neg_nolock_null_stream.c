/* TC
 * id:    chr_neg_nolock_null_stream
 * area:  stdio_char
 * funcs: _getc_nolock _putc_nolock
 * crt:   win10
 * tier:  p1
 * kind:  negative
 * conf:  observed
 * ref:   MS docs _getc_nolock, _putc_nolock
 * note:  PROBE: whether the UCRT nolock variants validate a NULL stream (EOF, EINVAL, invalid-parameter handler)
 *        rather than crashing.
 *        OBSERVED (Win10 22H2, class C): _getc_nolock(NULL) validates (EOF, EINVAL, handler once); _putc_nolock(c, NULL) is unchecked and the child dies with
 *        0xC0000005 (raw: code == 0 got=0xc0000005), so putc_nolock_null asserts the access violation.
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
static void run(const char *n, DWORD want)
{
    DWORD code = 99;
    CHECK(tc_spawn_self(n, &code, 20000));
    CHECK_EQ_U(code, want);
}
TC_CASE(getc_nolock_null) { run("a", 0); }
/* Win10 22H2 ucrtbase: _fputc_nolock(c, NULL) does no parameter validation and faults (observed rc 0xc0000005);
 * _fgetc_nolock(NULL) does validate (case above). Windows behaviour, so the case asserts the access violation. */
TC_CASE(putc_nolock_null) { run("b", TC_STATUS_ACCESS_VIOLATION); }
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name())) return child_main(c);
    TC_RUN(getc_nolock_null); TC_RUN(putc_nolock_null);
    return tc_end();
}
