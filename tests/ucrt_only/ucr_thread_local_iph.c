/* TC
 * id:    ucr_thread_local_iph
 * area:  ucrt_only
 * funcs: _set_invalid_parameter_handler strcpy_s
 * procs: _set_thread_local_invalid_parameter_handler _get_thread_local_invalid_parameter_handler
 * crt:   win10
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _set_invalid_parameter_handler, _set_thread_local_invalid_parameter_handler
 */
#include "tc.h"
#include <string.h>
#include <errno.h>

typedef void (__cdecl *iph_t)(const wchar_t *, const wchar_t *, const wchar_t *, unsigned int, uintptr_t);
typedef iph_t (__cdecl *seth_t)(iph_t);
typedef iph_t (__cdecl *geth_t)(void);
static int cnt_a, cnt_b;
static void __cdecl ha(const wchar_t *e, const wchar_t *f, const wchar_t *fl, unsigned int l, uintptr_t r) { (void)e;(void)f;(void)fl;(void)l;(void)r; cnt_a++; }
static void __cdecl hb(const wchar_t *e, const wchar_t *f, const wchar_t *fl, unsigned int l, uintptr_t r) { (void)e;(void)f;(void)fl;(void)l;(void)r; cnt_b++; }
static seth_t tset; static geth_t tget;

static void bad(void) { char d[4]; char *volatile dst = d; errno = 0; strcpy_s(dst, sizeof d, NULL); }

TC_CASE(set_get)
{
    if (!tset || !tget) TC_SKIP("not exported");
    CHECK(tget() == NULL);
    CHECK(tset(hb) == NULL);
    CHECK(tget() == hb);
}
TC_CASE(thread_local_wins)
{
    if (!tset) TC_SKIP("not exported");
    _set_invalid_parameter_handler(ha);
    tset(hb);
    cnt_a = cnt_b = 0;
    bad();
    CHECK_EQ_I(cnt_b, 1);
    CHECK_EQ_I(cnt_a, 0);
    CHECK_EQ_I(errno, EINVAL);
}
TC_CASE(null_falls_back_to_global)
{
    if (!tset) TC_SKIP("not exported");
    _set_invalid_parameter_handler(ha);
    tset(NULL);
    cnt_a = cnt_b = 0;
    bad();
    CHECK_EQ_I(cnt_a, 1);
    CHECK_EQ_I(cnt_b, 0);
    CHECK(tget() == NULL);
}
int main(void)
{
    tc_begin();
    tset = (seth_t)tc_crt_proc("_set_thread_local_invalid_parameter_handler");
    tget = (geth_t)tc_crt_proc("_get_thread_local_invalid_parameter_handler");
    TC_RUN(set_get); TC_RUN(thread_local_wins); TC_RUN(null_falls_back_to_global);
    return tc_end();
}
