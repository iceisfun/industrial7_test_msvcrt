/* TC
 * id:    gsf_neg_fseek_nolock_default_handler_fatal
 * area:  stdio_file
 * funcs: _fseek_nolock
 * crt:   >=80
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   MS docs Parameter validation: default handler terminates
 * note:  Child process only; default handler terminates on msvcr80+ (exact status not asserted).
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>

int __cdecl _fseek_nolock(FILE *, long, int);
static FILE *volatile nullfp;
static volatile int vorg;

TC_CASE(null_stream_fatal)
{
    DWORD code = 0; CHECK(tc_spawn_self("nullseek", &code, 20000));
    CHECK(code != 0); CHECK(code != 77);
}
TC_CASE(bad_origin_fatal)
{
#if !defined(TC_UCRT)
    TC_SKIP("bad whence fatal only asserted for ucrtbase");
#else
    DWORD code = 0; CHECK(tc_spawn_self("badorigin", &code, 20000));
    CHECK(code != 0); CHECK(code != 77);
#endif
}
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        if (c[0] == 'n') { _fseek_nolock(nullfp, 0, SEEK_SET); }
        else { FILE *f = fopen("tc_gsf_neg_fseek_nolock_default_handler_fatal.tmp", "wb"); vorg = 3; if (f) _fseek_nolock(f, 0, vorg); }
        return 77;
    }
    TC_RUN(null_stream_fatal); TC_RUN(bad_origin_fatal);
    remove("tc_gsf_neg_fseek_nolock_default_handler_fatal.tmp");
    return tc_end();
}
