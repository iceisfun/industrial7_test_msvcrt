/* TC
 * id:    fio_neg_probe_nolock_null_stream
 * area:  stdio_file
 * funcs: _fwrite_nolock _fread_nolock _fclose_nolock
 * crt:   >=80
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _fwrite_nolock ("same as fwrite but not thread-safe")
 * note:  Probes that the _nolock variants validate a NULL stream like the locking versions (return 0 / EOF, one iph call).
 *        Each call runs in a child (exit 0 = validated, 1 = returned unvalidated) so a crash cannot mask the other cases.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static int child(char k)
{
    FILE *volatile p = NULL; char b[2] = "x"; int ok;
    tc_iph_install(); tc_iph_count = 0;
    if (k == 'w') ok = _fwrite_nolock(b, 1, 1, p) == 0;
    else if (k == 'r') ok = _fread_nolock(b, 1, 1, p) == 0;
    else ok = _fclose_nolock(p) == EOF;
    return (ok && tc_iph_count == 1) ? 0 : 1;
}
static void chk(const char *n) { DWORD c = 99; CHECK(tc_spawn_self(n, &c, 20000)); CHECK_EQ_U(c, 0); }
TC_CASE(fwrite_null) { chk("w"); }
TC_CASE(fread_null)  { chk("r"); }
TC_CASE(fclose_null) { chk("c"); }
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) return child(c[0]);
    TC_RUN(fwrite_null); TC_RUN(fread_null); TC_RUN(fclose_null);
    return tc_end();
}
