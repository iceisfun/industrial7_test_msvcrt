/* TC
 * id:    cpp_neg_bad_cast_null_msg_probe
 * area:  cpp_runtime
 * funcs:
 * procs: ??0bad_cast@std@@QEAA@PEBD@Z ??0bad_typeid@std@@QEAA@PEBD@Z ??0bad_cast@std@@QEAA@AEBV01@@Z ??1bad_cast@std@@UEAA@XZ ??1bad_typeid@std@@UEAA@XZ ?what@exception@std@@UEBAPEBDXZ
 * crt:   msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs bad_cast Class / bad_typeid Class (constructed from a const char* message)
 * note:  PROBE: bad_cast/bad_typeid built with a NULL message must still yield a non-NULL what() (derived std::exception fallback).
 *        The exact fallback text ("Unknown exception") is only logged-as-weak: asserted in its own case. what() is the base class virtual.
 */
#include "tc.h"

typedef void *(__cdecl *ctor_t)(void *, const char *);
typedef void *(__cdecl *copy_t)(void *, const void *);
typedef void (__cdecl *dtor_t)(void *);
typedef const char *(__cdecl *what_t)(const void *);
static ctor_t p_bc, p_bt; static copy_t p_bccopy; static dtor_t p_bcd, p_btd; static what_t p_what;
#define READY() (p_bc && p_bt && p_bccopy && p_bcd && p_btd && p_what)

TC_CASE(exported) { CHECK(READY()); }
TC_CASE(bad_cast_null_what_nonnull)
{
    static char o[64]; const char *w;
    if (!READY()) TC_SKIP("n/a");
    p_bc(o, NULL); w = p_what(o);
    CHECK(w != NULL);
    p_bcd(o);
}
TC_CASE(bad_cast_null_what_exact_probe)
{
    static char o[64]; const char *w;
    if (!READY()) TC_SKIP("n/a");
    p_bc(o, NULL); w = p_what(o);
    CHECK_STR(w, "Unknown exception");
    p_bcd(o);
}
TC_CASE(bad_typeid_null_what_nonnull)
{
    static char o[64]; const char *w;
    if (!READY()) TC_SKIP("n/a");
    p_bt(o, NULL); w = p_what(o);
    CHECK(w != NULL);
    p_btd(o);
}
TC_CASE(bad_cast_copy_of_null_msg_what_nonnull)
{
    static char a[64], b[64]; const char *w;
    if (!READY()) TC_SKIP("n/a");
    p_bc(a, NULL); p_bccopy(b, a); w = p_what(b);
    CHECK(w != NULL);
    p_bcd(b); p_bcd(a);
}
TC_CASE(bad_cast_msg_roundtrip)
{
    static char o[64];
    if (!READY()) TC_SKIP("n/a");
    p_bc(o, "x");
    CHECK_STR(p_what(o), "x");
    p_bcd(o);
}
TC_CASE(bad_cast_dtor_no_crash)
{
    static char o[64]; int i;
    if (!READY()) TC_SKIP("n/a");
    for (i = 0; i < 3; i++) { p_bc(o, i ? "msg" : NULL); p_bcd(o); }
    CHECK(1);
}
int main(void)
{
    tc_begin();
    p_bc = (ctor_t)tc_crt_proc("??0bad_cast@std@@QEAA@PEBD@Z");
    p_bt = (ctor_t)tc_crt_proc("??0bad_typeid@std@@QEAA@PEBD@Z");
    p_bccopy = (copy_t)tc_crt_proc("??0bad_cast@std@@QEAA@AEBV01@@Z");
    p_bcd = (dtor_t)tc_crt_proc("??1bad_cast@std@@UEAA@XZ");
    p_btd = (dtor_t)tc_crt_proc("??1bad_typeid@std@@UEAA@XZ");
    p_what = (what_t)tc_crt_proc("?what@exception@std@@UEBAPEBDXZ");
    TC_RUN(exported); TC_RUN(bad_cast_null_what_nonnull); TC_RUN(bad_cast_null_what_exact_probe); TC_RUN(bad_typeid_null_what_nonnull);
    TC_RUN(bad_cast_copy_of_null_msg_what_nonnull); TC_RUN(bad_cast_msg_roundtrip); TC_RUN(bad_cast_dtor_no_crash);
    return tc_end();
}
