/* TC
 * id:    gcv_ltoa_plain_name_probe
 * area:  conv
 * funcs:
 * procs: ltoa _ltoa
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcr80 msvcrt
 * tier:  p2
 * conf:  probe
 * ref:   MS docs: POSIX names ltoa etc. deprecated aliases of _ltoa
 * note:  PROBING: whether un-prefixed ltoa is an exported symbol of each CRT (presence is printed as a TC NOTE).
 *        _ltoa must always be present. If ltoa is present it must behave like _ltoa. Absence passes.
 */
#include "tc.h"

typedef char *(__cdecl *ltoa_t)(long, char *, int);

TC_CASE(probe)
{
    ltoa_t pu = (ltoa_t)tc_crt_proc("ltoa"), pl = (ltoa_t)tc_crt_proc("_ltoa");
    char a[40], b[40];
    CHECK(pl != NULL);
    tc_write("TC NOTE " TC_TEST_ID " ltoa "); tc_write(pu ? "present\n" : "absent\n");
    if (pu && pl) {
        CHECK(pu(-5, a, 10) == a); pl(-5, b, 10);
        CHECK_STR(a, b); CHECK_STR(a, "-5");
    }
}

int main(void) { tc_begin(); TC_RUN(probe); return tc_end(); }
