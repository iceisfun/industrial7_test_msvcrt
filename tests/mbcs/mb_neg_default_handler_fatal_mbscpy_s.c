/* TC
 * id:    mb_neg_default_handler_fatal_mbscpy_s
 * area:  mbcs
 * kind:  negative
 * funcs:
 * procs: _mbscpy_s
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs "Parameter validation", _mbscpy_s
 * note:  Without an invalid-parameter handler a failing _mbscpy_s terminates the process. Fatal calls run in child
 *        processes; only abnormal termination is asserted here (exact status is in mb_neg_default_handler_status_mbscpy_s).
 */
#include "tc.h"

typedef int (__cdecl *mbscpy_s_t)(unsigned char *, size_t, const unsigned char *);
static mbscpy_s_t p_mbscpy_s;

static void run_child(const char *name)
{
    DWORD code = 0; int ran = tc_spawn_self(name, &code, 20000);
    CHECK(ran);
    CHECK(code != 0);
    CHECK(code != 77);
    CHECK(code != 78);  /* 78 = proc missing in child */
}
TC_CASE(overflow_child) { run_child("overflow"); }
TC_CASE(null_dest_child) { run_child("nulldest"); }
TC_CASE(null_src_child) { run_child("nullsrc"); }

int main(void)
{
    const char *c;
    tc_begin();
    p_mbscpy_s = (mbscpy_s_t)tc_crt_proc("_mbscpy_s");
    if ((c = tc_child_name()) != NULL) {
        unsigned char buf[2]; static const unsigned char *volatile longs = (const unsigned char *)"toolong";
        static unsigned char *volatile nul = NULL; static const unsigned char *volatile nuls = NULL;
        if (!p_mbscpy_s) return 78;
        if (c[0] == 'o') p_mbscpy_s(buf, 2, longs);
        else if (c[4] == 'd') p_mbscpy_s(nul, 4, longs);
        else p_mbscpy_s(buf, 2, nuls);
        return 77;
    }
    CHECK(p_mbscpy_s != NULL);
    CHECK(p_mbscpy_s != NULL);
    TC_RUN(overflow_child); TC_RUN(null_dest_child); TC_RUN(null_src_child);
    return tc_end();
}
