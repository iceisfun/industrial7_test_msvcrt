/* TC
 * id:    fio_neg_probe_lock_file_null
 * area:  stdio_file
 * funcs: _lock_file _unlock_file
 * crt:   >=140
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _lock_file (no parameter validation described)
 * note:  Narrowed to ucrtbase: pre-UCRT _lock_file/_unlock_file are believed to have no NULL check (they index the FILE
 *        directly and would fault), so the validation assertion is only plausible on ucrtbase.
 *         Probes whether _lock_file(NULL)/_unlock_file(NULL) validate the argument (iph + EINVAL) instead of crashing.
 *        Run in a child with an iph handler installed so a crash does not kill the parent.
 *        Exit codes: 0 = validated (iph 1, EINVAL), 1 = returned without validation, crash = non-zero other.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static int child(int unlock)
{
    FILE *volatile p = NULL;
    tc_iph_install(); tc_iph_count = 0; errno = 0;
    if (unlock) _unlock_file(p); else _lock_file(p);
    return (tc_iph_count == 1 && errno == EINVAL) ? 0 : 1;
}
TC_CASE(lock_null)   { DWORD c = 99; CHECK(tc_spawn_self("lock", &c, 20000)); CHECK_EQ_U(c, 0); }
TC_CASE(unlock_null) { DWORD c = 99; CHECK(tc_spawn_self("unlock", &c, 20000)); CHECK_EQ_U(c, 0); }
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) return child(c[0] == 'u');
    TC_RUN(lock_null); TC_RUN(unlock_null);
    return tc_end();
}
