/* TC
 * id:    hp_neg_heapwalk_null_probe
 * area:  heap
 * kind:  negative
 * funcs: _heapwalk
 * crt:   msvcr80 msvcr90 msvcr100 msvcr110 msvcr120
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _heapwalk; MS docs Parameter Validation (_heapwalk validates its argument in the secure CRTs)
 * note:  PROBING what _heapwalk(NULL) returns (_HEAPBADPTR?) and whether errno becomes EINVAL. Runs in a child with the IPH
 *        installed; exit code = 100 + (-return) (+50 if errno==EINVAL), 98 if the IPH ran more than once; printed as TC INFO.
 *        Only asserted: the child ran to exit and IPH count was not above 1. msvcrt.dll (60) excluded (may dereference NULL);
 *        ucrtbase excluded (_heapwalk not part of the Windows 10 UCRT contract).
 */
#include "tc.h"
#include <malloc.h>
#include <errno.h>

static void info(const char *n, unsigned long long v) { char b[32]; tc_hex(v, b); tc_write("TC INFO " TC_TEST_ID " "); tc_write(n); tc_write("="); tc_write(b); tc_write("\n"); }

TC_CASE(null_entry)
{
    DWORD code = 0; int ran = tc_spawn_self("null", &code, 20000);
    CHECK(ran);
    info("exit", code);
    CHECK(code != 98);
}
int main(void)
{
    tc_begin();
    if (tc_child_name()) {
        int r;
        if (!tc_iph_install()) return 97;
        r = _heapwalk((_HEAPINFO *)(volatile void *)0);
        if (tc_iph_count > 1) return 98;
        return 100 + (-r) + (errno == EINVAL ? 50 : 0);
    }
    TC_RUN(null_entry);
    return tc_end();
}
