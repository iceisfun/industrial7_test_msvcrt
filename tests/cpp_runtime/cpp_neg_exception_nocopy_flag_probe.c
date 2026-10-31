/* TC
 * id:    cpp_neg_exception_nocopy_flag_probe
 * area:  cpp_runtime
 * funcs:
 * procs: ??0exception@std@@QEAA@AEBQEBDH@Z ??1exception@std@@UEAA@XZ ?what@exception@std@@UEBAPEBDXZ
 * crt:   msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs std::exception Class: exception(const char* const&, int) is a non-standard constructor ("no copy" form)
 * note:  PROBE: the two-argument constructor stores the message pointer WITHOUT copying and its dtor must not free it. The int is
 *        believed to be an ignored dummy, so any value (0, 1, 2, -1) is expected to keep pointer identity. Not documented in detail.
 */
#include "tc.h"

typedef void *(__cdecl *ctorf_t)(void *, const char *const *, int);
typedef void (__cdecl *dtor_t)(void *);
typedef const char *(__cdecl *what_t)(const void *);
static ctorf_t p_ctorf; static dtor_t p_dtor; static what_t p_what;
static const char lit[] = "static literal message";

static const char *ident(int flag)
{
    static char obj[64]; const char *volatile m = lit; const char *w;
    p_ctorf(obj, (const char *const *)&m, flag);
    w = p_what(obj);
    p_dtor(obj);
    return w;
}
#define READY() (p_ctorf && p_dtor && p_what)
TC_CASE(exported) { CHECK(READY()); }
TC_CASE(flag1_what_pointer_identity) { if (!READY()) TC_SKIP("n/a"); CHECK_EQ_P(ident(1), lit); }
TC_CASE(flag1_dtor_does_not_free_static)
{
    int i; void *p;
    if (!READY()) TC_SKIP("n/a");
    for (i = 0; i < 4; i++) ident(1);
    p = HeapAlloc(GetProcessHeap(), 0, 64);
    CHECK(p != NULL);
    HeapFree(GetProcessHeap(), 0, p);
    CHECK_STR(lit, "static literal message");
}
TC_CASE(flag2_treated_as_true) { if (!READY()) TC_SKIP("n/a"); CHECK_EQ_P(ident(2), lit); CHECK_EQ_P(ident(-1), lit); }
TC_CASE(flag0_pointer_identity_too) { if (!READY()) TC_SKIP("n/a"); CHECK_EQ_P(ident(0), lit); }
TC_CASE(flag1_null_msg_what_unknown)
{
    static char obj[64]; const char *volatile m = NULL; const char *w;
    if (!READY()) TC_SKIP("n/a");
    p_ctorf(obj, (const char *const *)&m, 1);
    w = p_what(obj);
    CHECK(w != NULL);
    CHECK_STR(w, "Unknown exception");
    p_dtor(obj);
}
int main(void)
{
    tc_begin();
    p_ctorf = (ctorf_t)tc_crt_proc("??0exception@std@@QEAA@AEBQEBDH@Z");
    p_dtor  = (dtor_t)tc_crt_proc("??1exception@std@@UEAA@XZ");
    p_what  = (what_t)tc_crt_proc("?what@exception@std@@UEBAPEBDXZ");
    TC_RUN(exported); TC_RUN(flag1_what_pointer_identity); TC_RUN(flag1_dtor_does_not_free_static); TC_RUN(flag2_treated_as_true);
    TC_RUN(flag0_pointer_identity_too); TC_RUN(flag1_null_msg_what_unknown);
    return tc_end();
}
