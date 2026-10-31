/* TC
 * id:    cpp_neg_exception_default_what_unknown
 * area:  cpp_runtime
 * funcs:
 * procs: ??0exception@std@@QEAA@XZ ??0exception@std@@QEAA@AEBQEBD@Z ??1exception@std@@UEAA@XZ ?what@exception@std@@UEBAPEBDXZ
 * crt:   msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * kind:  negative
 * conf:  observed
 * ref:   MS docs std::exception Class / exception::what; the text "Unknown exception" is real-CRT behaviour (not spelled out in the docs)
 * note:  x64 decorated names only (objects are raw 64-byte buffers: vptr, _Mywhat, _Mydofree). Negative: no message supplied.
 */
#include "tc.h"

typedef void *(__cdecl *ctor0_t)(void *);
typedef void *(__cdecl *ctorm_t)(void *, const char *const *);
typedef void (__cdecl *dtor_t)(void *);
typedef const char *(__cdecl *what_t)(const void *);
static ctor0_t p_ctor0; static ctorm_t p_ctorm; static dtor_t p_dtor; static what_t p_what;

TC_CASE(exported) { CHECK(p_ctor0 && p_ctorm && p_dtor && p_what); }
TC_CASE(default_ctor_what_is_unknown_exception)
{
    static char obj[64]; const char *w;
    if (!p_ctor0 || !p_what || !p_dtor) TC_SKIP("not exported");
    p_ctor0(obj);
    w = p_what(obj);
    CHECK(w != NULL);
    CHECK_STR(w, "Unknown exception");
    p_dtor(obj);
}
TC_CASE(null_msg_ptr_what_unknown)
{
    static char obj[64]; const char *volatile m = NULL; const char *w;
    if (!p_ctorm || !p_what || !p_dtor) TC_SKIP("not exported");
    p_ctorm(obj, (const char *const *)&m);
    w = p_what(obj);
    CHECK(w != NULL);
    CHECK_STR(w, "Unknown exception");
    p_dtor(obj);
}
TC_CASE(dtor_of_default_object_no_free_crash)
{
    static char obj[64]; int i;
    if (!p_ctor0 || !p_dtor) TC_SKIP("not exported");
    for (i = 0; i < 3; i++) { p_ctor0(obj); p_dtor(obj); }
    CHECK(1);  /* reaching here = no crash / no free of a NULL or static message */
}
int main(void)
{
    tc_begin();
    p_ctor0 = (ctor0_t)tc_crt_proc("??0exception@std@@QEAA@XZ");
    p_ctorm = (ctorm_t)tc_crt_proc("??0exception@std@@QEAA@AEBQEBD@Z");
    p_dtor  = (dtor_t)tc_crt_proc("??1exception@std@@UEAA@XZ");
    p_what  = (what_t)tc_crt_proc("?what@exception@std@@UEBAPEBDXZ");
    TC_RUN(exported); TC_RUN(default_ctor_what_is_unknown_exception); TC_RUN(null_msg_ptr_what_unknown);
    TC_RUN(dtor_of_default_object_no_free_crash);
    return tc_end();
}
