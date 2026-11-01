/* TC
 * id:    loc_neg_mbstrlen_null
 * area:  locale
 * funcs: _mbstrlen
 * procs: _mbstrlen_l _create_locale _free_locale
 * crt:   >=90
 * tier:  p1
 * kind:  negative
 * conf:  observed
 * ref:   MS docs _mbstrlen (parameter validation, VS2005+); observed Windows 10 22H2 behaviour
 * note:  Win10 triage (class C): on ucrtbase 19045 _mbstrlen(NULL) does NOT return -1/EINVAL via the invalid-parameter handler; it dereferences
 *        NULL and dies with STATUS_ACCESS_VIOLATION (0xC0000005) in the C locale (single-byte path = strlen). The crash is the only behaviour, so each
 *        call runs in a child process and the parent asserts the exit status. Observed on Windows 10 22H2 build 19045.
 *        [scope] the access violation is observed on ucrtbase 19045 only (TC_AT_LEAST(140)); legacy CRTs keep the original -1/EINVAL/handler
 *        expectation, [unverified] until the P3 runs (a legacy CRT might crash too).
 */
#include "tc.h"
#include <locale.h>
#include <errno.h>

typedef size_t (__cdecl *mbstrlen_l_t)(const char *, void *);
typedef void *(__cdecl *create_locale_t)(int, const char *);

static const char * volatile v_null;

#if TC_AT_LEAST(140)
TC_CASE(null_access_violation)
{
    DWORD code = 0; int ran = tc_spawn_self("mbstrlen", &code, 20000);
    CHECK(ran);
    CHECK_EQ_U(code, TC_STATUS_ACCESS_VIOLATION);
}
TC_CASE(null_l_access_violation)
{
    DWORD code = 0; int ran = tc_spawn_self("mbstrlen_l", &code, 20000);
    CHECK(ran);
    CHECK_EQ_U(code, TC_STATUS_ACCESS_VIOLATION);
}
#else
typedef void (__cdecl *free_locale_t)(void *);
static mbstrlen_l_t p_mbstrlen_l;
static create_locale_t p_create;
static free_locale_t p_free;
TC_CASE(null_minus1)
{
    void *loc;
    CHECK_EQ_U(_mbstrlen(v_null), (size_t)-1);
    if (!p_mbstrlen_l || !p_create || !p_free) TC_SKIP("_l functions missing");
    loc = p_create(LC_ALL, "C");
    if (!loc) TC_SKIP("no C locale object");
    CHECK_EQ_U(p_mbstrlen_l(v_null, loc), (size_t)-1);
    p_free(loc);
}
TC_CASE(errno_einval)
{
    errno = 0;
    CHECK_EQ_U(_mbstrlen(v_null), (size_t)-1);
    CHECK_EQ_I(errno, EINVAL);
}
TC_CASE(iph_count)
{
    if (!tc_iph_install()) TC_SKIP("no IPH api");
    tc_iph_count = 0;
    CHECK_EQ_U(_mbstrlen(v_null), (size_t)-1);
    CHECK_EQ_I(tc_iph_count, 1);
}
#endif
int main(void)
{
    const char *c;
    tc_begin();
#if TC_AT_LEAST(140)
    if ((c = tc_child_name()) != NULL) {
        tc_iph_install();   /* the handler would return; a crash is the expected outcome */
        setlocale(LC_ALL, "C");
        if (c[8] == 0) { volatile size_t r = _mbstrlen(v_null); (void)r; return 77; }
        {
            mbstrlen_l_t p_l = (mbstrlen_l_t)tc_crt_proc("_mbstrlen_l");
            create_locale_t p_c = (create_locale_t)tc_crt_proc("_create_locale");
            void *loc = p_c ? p_c(LC_ALL, "C") : NULL;
            if (!p_l || !loc) return 78;
            { volatile size_t r = p_l(v_null, loc); (void)r; }
            return 77;
        }
    }
#else
    c = NULL; (void)c;
    tc_iph_install();
    p_mbstrlen_l = (mbstrlen_l_t)tc_crt_proc("_mbstrlen_l");
    p_create = (create_locale_t)tc_crt_proc("_create_locale");
    p_free = (free_locale_t)tc_crt_proc("_free_locale");
    TC_RUN(null_minus1); TC_RUN(errno_einval); TC_RUN(iph_count);
#endif
#if TC_AT_LEAST(140)
    TC_RUN(null_access_violation); TC_RUN(null_l_access_violation);
#endif
    return tc_end();
}
