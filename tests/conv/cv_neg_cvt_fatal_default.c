/* TC
 * id:    cv_neg_cvt_fatal_default
 * area:  conv
 * funcs:
 * procs: _ecvt_s _fcvt_s _gcvt_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   MS docs Parameter validation: without a handler the invalid-parameter path terminates the process
 */
#include "tc.h"

TC_DECL_PROC(int, _ecvt_s, (char *, size_t, double, int, int *, int *));
TC_DECL_PROC(int, _fcvt_s, (char *, size_t, double, int, int *, int *));
TC_DECL_PROC(int, _gcvt_s, (char *, size_t, double, int));
static char *volatile nullp;
static volatile size_t zero_size;

static void run_case(const char *name)
{
    DWORD code = 0; int ran = tc_spawn_self(name, &code, 20000);
    CHECK(ran); CHECK(code != 0); CHECK(code != 77);
}
TC_CASE(sanity) { DWORD code = 0; int ran = tc_spawn_self("sanity", &code, 20000); CHECK(ran); CHECK_EQ_U(code, 77); }
TC_CASE(ecvt_null)  { run_case("ecvtnull"); }
TC_CASE(fcvt_size0) { run_case("fcvtsize0"); }
TC_CASE(gcvt_null)  { run_case("gcvtnull"); }

int main(void)
{
    const char *c; char b[16]; int d, s;
    tc_begin();
    TC_LOAD_PROC(_ecvt_s); TC_LOAD_PROC(_fcvt_s); TC_LOAD_PROC(_gcvt_s);
    if ((c = tc_child_name()) != NULL) {
        if (c[0] == 'e' && tc_p__ecvt_s) tc_p__ecvt_s(nullp, 16, 1.5, 3, &d, &s);
        else if (c[0] == 'f' && tc_p__fcvt_s) tc_p__fcvt_s(b, zero_size, 1.5, 3, &d, &s);
        else if (c[0] == 'g' && tc_p__gcvt_s) tc_p__gcvt_s(nullp, 16, 1.5, 3);
        return 77;
    }
    TC_RUN(sanity); TC_RUN(ecvt_null); TC_RUN(fcvt_size0); TC_RUN(gcvt_null);
    return tc_end();
}
