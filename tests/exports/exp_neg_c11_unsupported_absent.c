/* TC
 * id:    exp_neg_c11_unsupported_absent
 * area:  exports
 * kind:  negative
 * funcs:
 * procs: malloc
 * crt:   all
 * tier:  p0
 * conf:  observed
 * ref:   MS docs C11/C17 support: aligned_alloc and <threads.h> not supported; MS docs quick_exit (VS2015+ UCRT)
 * note:  aligned_alloc and C11 threads.h functions are not implemented by Microsoft's CRT. quick_exit/_crt_at_quick_exit presence is asserted only for ucrtbase (at_quick_exit is a static wrapper in the import library, not a DLL export).
 */
#include "tc.h"
#include <stddef.h>
#include <errno.h>

static HMODULE crt;
static void absent(const char *n)
{
    FARPROC p = crt ? GetProcAddress(crt, n) : NULL;
    if (p) tc_fail_expr(__FILE__, __LINE__, n);
}
static void present(const char *n)
{
    FARPROC p = crt ? GetProcAddress(crt, n) : NULL;
    if (!p) tc_fail_expr(__FILE__, __LINE__, n);
}
static void check_list(const char *const *v, size_t n, int want)
{
    size_t i;
    for (i = 0; i < n; i++) { if (want) present(v[i]); else absent(v[i]); }
}
#define N(a) (sizeof(a) / sizeof((a)[0]))
static const char *const l_aligned_alloc_absent[] = {
    "aligned_alloc",
};
static const char *const l_threads_absent[] = {
    "thrd_create",
    "thrd_join",
    "thrd_sleep",
    "thrd_current",
    "thrd_exit",
    "mtx_init",
    "mtx_lock",
    "mtx_unlock",
    "mtx_destroy",
    "cnd_init",
    "cnd_signal",
    "cnd_wait",
    "tss_create",
    "tss_get",
    "call_once",
};
static const char *const l_control[] = {
    "malloc",
    "free",
};
TC_CASE(aligned_controls)
{
    CHECK(crt != NULL);
#ifdef TC_UCRT
    present("quick_exit");
    present("_crt_at_quick_exit");
#endif
    if (TC_AT_LEAST(80)) present("_aligned_malloc");
}
TC_CASE(aligned_alloc_absent) { CHECK(crt != NULL); check_list(l_aligned_alloc_absent, N(l_aligned_alloc_absent), 0); }
TC_CASE(threads_absent) { CHECK(crt != NULL); check_list(l_threads_absent, N(l_threads_absent), 0); }
TC_CASE(control) { CHECK(crt != NULL); check_list(l_control, N(l_control), 1); }

int main(void)
{
    tc_begin();
    crt = GetModuleHandleA(TC_CRT_DLL);
    TC_RUN(aligned_alloc_absent);
    TC_RUN(threads_absent);
    TC_RUN(control);
    TC_RUN(aligned_controls);
    return tc_end();
}
