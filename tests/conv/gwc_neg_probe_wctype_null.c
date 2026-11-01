/* TC
 * id:    gwc_neg_probe_wctype_null
 * area:  conv
 * funcs:
 * procs: wctype
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * kind:  negative
 * conf:  observed
 * ref:   MS docs wctype (no NULL contract documented)
 * note:  OBSERVED (Win10 22H2 19045 ucrtbase, run twice; class C): wctype(NULL) crashes with STATUS_ACCESS_VIOLATION (0xC0000005) both with a
 *        counting invalid-parameter handler installed and with the default one; the CRT does not validate it (the MS docs list no validation). The
 *        original probe expected handler + return 0. Children assert the crash exit code.
 */
#include "tc.h"

typedef unsigned short (__cdecl *wctype_fn)(const char *);
static wctype_fn p_wctype;
static const char *volatile vnull = NULL;

static int child_main(const char *c)
{
    if (c[0] == 'h') {   /* "h": counting handler installed; 77 = returned 0 and handler ran once */
        unsigned short r;
        if (!tc_iph_install()) return 79;
        r = p_wctype(vnull);
        return (r == 0 && tc_iph_count == 1) ? 77 : 78;
    }
    return p_wctype(vnull) == 0 ? 77 : 78;
}

TC_CASE(exported) { CHECK(p_wctype != NULL); }
/* run in a child too: if the CRT does not validate, the NULL dereference must not take the parent down */
TC_CASE(null_handler_installed_still_crashes)
{
    DWORD code = 0; int ran;
    if (!p_wctype) TC_SKIP("missing");
    ran = tc_spawn_self("h_wctype_null", &code, 20000);
    CHECK(ran);
    /* observed Win10 22H2 ucrtbase: wctype does NOT validate its argument; wctype(NULL) dereferences NULL even with a handler installed */
    CHECK_EQ_U(code, TC_STATUS_ACCESS_VIOLATION);
}
TC_CASE(null_child_terminates)
{
    DWORD code = 0; int ran;
    if (!p_wctype) TC_SKIP("missing");
    ran = tc_spawn_self("d_wctype_null", &code, 20000);
    CHECK(ran);
    CHECK_EQ_U(code, TC_STATUS_ACCESS_VIOLATION);   /* observed: same crash with the default handler */
    tc_write("TC NOTE gwc_neg_probe_wctype_null child exit="); { char b[24]; tc_hex(code, b); tc_write(b); } tc_write("\n");
}

int main(void)
{
    tc_begin();
    p_wctype = (wctype_fn)tc_crt_proc("wctype");
    if (tc_child_name()) return child_main(tc_child_name());
    TC_RUN(exported);
    TC_RUN(null_handler_installed_still_crashes);
    TC_RUN(null_child_terminates);
    return tc_end();
}
