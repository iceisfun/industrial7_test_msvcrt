/* TC
 * id:    loc_neg_mbstrlen_null
 * area:  locale
 * funcs: _mbstrlen
 * procs: _mbstrlen_l _create_locale _free_locale
 * crt:   >=90
 * tier:  p1
 * kind:  negative
 * conf:  observed
 * ref:   MS docs _mbstrlen (parameter validation, VS2005+)
 * note:  NULL string: (size_t)-1, errno EINVAL, invalid-parameter handler invoked once.
 */
#include "tc.h"
#include <locale.h>
#include <errno.h>

typedef size_t (__cdecl *mbstrlen_l_t)(const char *, void *);
typedef void *(__cdecl *create_locale_t)(int, const char *);
typedef void (__cdecl *free_locale_t)(void *);
static mbstrlen_l_t p_mbstrlen_l;
static create_locale_t p_create;
static free_locale_t p_free;
static const char * volatile v_null;

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
int main(void)
{
    tc_begin();
    tc_iph_install();
    p_mbstrlen_l = (mbstrlen_l_t)tc_crt_proc("_mbstrlen_l");
    p_create = (create_locale_t)tc_crt_proc("_create_locale");
    p_free = (free_locale_t)tc_crt_proc("_free_locale");
    TC_RUN(null_minus1); TC_RUN(errno_einval); TC_RUN(iph_count);
    return tc_end();
}
