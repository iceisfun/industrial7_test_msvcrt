/* TC
 * id:    gwc_neg_probe_wctype_null
 * area:  conv
 * funcs:
 * procs: wctype
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs wctype (no NULL contract documented)
 * note:  PROBE: wctype(NULL). The call is believed to hit the invalid-parameter handler (once) and return 0; asserted in a
 *        child with a counting handler (child exit 77). A second child makes the call with the default behaviour; only
 *        "the child terminated by itself" is asserted - the exit code is recorded in the output.
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
TC_CASE(null_handler_returns_0_and_counts)
{
    DWORD code = 0; int ran;
    if (!p_wctype) TC_SKIP("missing");
    ran = tc_spawn_self("h_wctype_null", &code, 20000);
    CHECK(ran);
    CHECK_EQ_U(code, 77);
}
TC_CASE(null_child_terminates)
{
    DWORD code = 0; int ran;
    if (!p_wctype) TC_SKIP("missing");
    ran = tc_spawn_self("d_wctype_null", &code, 20000);
    CHECK(ran);
    tc_write("TC NOTE gwc_neg_probe_wctype_null child exit="); { char b[24]; tc_hex(code, b); tc_write(b); } tc_write("\n");
}

int main(void)
{
    tc_begin();
    p_wctype = (wctype_fn)tc_crt_proc("wctype");
    if (tc_child_name()) return child_main(tc_child_name());
    TC_RUN(exported);
    TC_RUN(null_handler_returns_0_and_counts);
    TC_RUN(null_child_terminates);
    return tc_end();
}
