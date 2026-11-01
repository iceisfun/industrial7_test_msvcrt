/* TC
 * id:    fio_neg_probe_lock_file_null
 * area:  stdio_file
 * funcs: _lock_file _unlock_file
 * crt:   >=140
 * tier:  p2
 * kind:  negative
 * conf:  observed
 * ref:   MS docs _lock_file (no parameter validation described)
 * note:  Narrowed to ucrtbase: pre-UCRT _lock_file/_unlock_file are believed to have no NULL check (they index the FILE
 *        directly and would fault), so the validation assertion is only plausible on ucrtbase.
 *         Probes whether _lock_file(NULL)/_unlock_file(NULL) validate the argument (iph + EINVAL) instead of crashing.
 *        Run in a child with an iph handler installed so a crash does not kill the parent.
 *        Exit codes: 0 = validated (iph 1, EINVAL), 1 = returned without validation, crash = non-zero other.
 *        TRIAGE win1093[34])?#934, class C): ucrtbase 10.0.19045 does not validate NULL in _lock_file/_unlock_file; the child crashes with
 *        STATUS_ACCESS_VIOLATION 0xC0000005 (observed for both). The cases now assert that crash status. Exit codes: 0xC0000005 = crash, 77 = returned.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static int child(int unlock)
{
    FILE *volatile p = NULL;
    if (unlock) _unlock_file(p); else _lock_file(p);
    return 77;   /* reached only if the call neither crashed nor terminated */
}
TC_CASE(lock_null)   { DWORD c = 99; CHECK(tc_spawn_self("lock", &c, 20000)); CHECK_EQ_U(c, TC_STATUS_ACCESS_VIOLATION); }
TC_CASE(unlock_null) { DWORD c = 99; CHECK(tc_spawn_self("unlock", &c, 20000)); CHECK_EQ_U(c, TC_STATUS_ACCESS_VIOLATION); }
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) return child(c[0] == 'u');
    TC_RUN(lock_null); TC_RUN(unlock_null);
    return tc_end();
}
