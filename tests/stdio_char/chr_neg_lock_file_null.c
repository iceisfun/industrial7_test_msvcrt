/* TC
 * id:    chr_neg_lock_file_null
 * area:  stdio_char
 * funcs: _lock_file _unlock_file
 * crt:   win10
 * tier:  p1
 * kind:  negative
 * conf:  observed
 * ref:   MS docs _lock_file, _unlock_file; parameter validation
 * note:  PROBE: the UCRT validates a NULL FILE* in _lock_file/_unlock_file (invalid-parameter handler, errno EINVAL)
 *        instead of crashing. Older CRTs are excluded because they would dereference NULL.
 *        TRIAGE win10, class C): ucrtbase 10.0.19045 does NOT validate a NULL FILE* in _lock_file/_unlock_file: the child dies with
 *        STATUS_ACCESS_VIOLATION 0xC0000005 (observed got=0xc0000005 for both). The case now asserts that crash status in the child process.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static FILE *volatile nf;
static int child_main(const char *n)
{
    if (n[0] == 'a') { (_lock_file)(nf); }
    else { (_unlock_file)(nf); }
    return 77;   /* reached only if the call neither crashed nor terminated */
}
static void run(const char *n)
{
    DWORD code = 99;
    CHECK(tc_spawn_self(n, &code, 20000));
    CHECK_EQ_U(code, TC_STATUS_ACCESS_VIOLATION);
}
TC_CASE(lock_null) { run("a"); }
TC_CASE(unlock_null) { run("b"); }
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name())) return child_main(c);
    TC_RUN(lock_null); TC_RUN(unlock_null);
    return tc_end();
}
