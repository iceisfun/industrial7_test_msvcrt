/* TC
 * id:    cpp_current_exception_probe
 * area:  cpp_runtime
 * funcs:
 * procs: __current_exception __current_exception_context __processing_throw
 * crt:   ucrtbase
 * tier:  p2
 * conf:  probe
 * ref:   UCRT per-thread exception state accessors (api-ms-win-crt-runtime)
 * note:  PROBE: outside any catch/throw, *__current_exception()==NULL, *__current_exception_context()==NULL,
 *        *__processing_throw()==0, and the accessors return a stable per-thread address. Resolved at run time so a missing
 *        export skips instead of preventing the image from loading.
 */
#include "tc.h"

typedef void **(__cdecl *pv_t)(void);
typedef int *(__cdecl *pi_t)(void);
static pv_t p_cur, p_ctx;
static pi_t p_proc;

TC_CASE(current_exception_null) { if (!p_cur) TC_SKIP("not exported"); CHECK(*p_cur() == NULL); }
TC_CASE(context_null) { if (!p_ctx) TC_SKIP("not exported"); CHECK(*p_ctx() == NULL); }
TC_CASE(processing_throw_zero) { if (!p_proc) TC_SKIP("not exported"); CHECK_EQ_I(*p_proc(), 0); }
TC_CASE(pointers_stable)
{
    if (!p_cur || !p_ctx || !p_proc) TC_SKIP("not exported");
    CHECK_EQ_P(p_cur(), p_cur()); CHECK_EQ_P(p_ctx(), p_ctx()); CHECK_EQ_P(p_proc(), p_proc());
}

int main(void)
{
    tc_begin();
    p_cur = (pv_t)tc_crt_proc("__current_exception"); p_ctx = (pv_t)tc_crt_proc("__current_exception_context");
    p_proc = (pi_t)tc_crt_proc("__processing_throw");
    TC_RUN(current_exception_null); TC_RUN(context_null); TC_RUN(processing_throw_zero); TC_RUN(pointers_stable);
    return tc_end();
}
