/* TC
 * id:    hp_neg_heapwalk_bad_entry_probe
 * area:  heap
 * kind:  negative
 * funcs: _heapwalk malloc free
 * crt:   <140
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _heapwalk (returns _HEAPBADPTR if _pentry does not point to a valid heap block)
 * note:  PROBING _heapwalk with a garbage _pentry (0x1234) and with a stale _pentry whose block was freed. Both run in child
 *        processes so a crash cannot take down the parent. Exit code = 100 + (-status) of _heapwalk (_HEAPOK=-2 -> 102);
 *        the status is printed as TC INFO. Only asserted: the child ran to exit, and the garbage walk did not report _HEAPOK.
 */
#include "tc.h"
#include <stdlib.h>
#include <malloc.h>

static volatile size_t v_n = 100;

static void info(const char *n, unsigned long long v) { char b[32]; tc_hex(v, b); tc_write("TC INFO " TC_TEST_ID " "); tc_write(n); tc_write("="); tc_write(b); tc_write("\n"); }

TC_CASE(garbage_entry)
{
    DWORD code = 0; int ran = tc_spawn_self("garbage", &code, 20000);
    CHECK(ran);
    info("garbage_exit", code);
    CHECK(code != 102);
}
TC_CASE(stale_entry)
{
    DWORD code = 0; int ran = tc_spawn_self("stale", &code, 20000);
    CHECK(ran);
    info("stale_exit", code);
}
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        _HEAPINFO e; int r, n = 0;
        if (c[0] == 'g') {
            e._pentry = (int *)(size_t)(v_n + 0x1234 - 100);
            r = _heapwalk(&e);
        } else {
            unsigned char *keep = (unsigned char *)malloc(v_n);
            int found = 0;
            e._pentry = NULL;
            while ((r = _heapwalk(&e)) == _HEAPOK && n++ < 1000000)
                if (e._useflag == _USEDENTRY && (void *)e._pentry == (void *)keep) { found = 1; break; }
            if (!found) return 99;
            free(keep);
            r = _heapwalk(&e);
        }
        return 100 + (-r);
    }
    TC_RUN(garbage_entry); TC_RUN(stale_entry);
    return tc_end();
}
