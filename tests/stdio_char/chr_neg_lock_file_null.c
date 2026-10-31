/* TC
 * id:    chr_neg_lock_file_null
 * area:  stdio_char
 * funcs: _lock_file _unlock_file
 * crt:   win10
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _lock_file, _unlock_file; parameter validation
 * note:  PROBE: the UCRT validates a NULL FILE* in _lock_file/_unlock_file (invalid-parameter handler, errno EINVAL)
 *        instead of crashing. Older CRTs are excluded because they would dereference NULL.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static FILE *volatile nf;
static int child_main(const char *n)
{
    int c0; 
    tc_iph_install();
    c0 = tc_iph_count; errno = 0;
    if (n[0] == 'a') { (_lock_file)(nf); }
    else { (_unlock_file)(nf); }
    if (tc_iph_count - c0 != 1) return 1;
    return errno == EINVAL ? 0 : 2;
}
static void run(const char *n)
{
    DWORD code = 99;
    CHECK(tc_spawn_self(n, &code, 20000));
    CHECK_EQ_U(code, 0);
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
