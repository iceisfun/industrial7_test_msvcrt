/* TC
 * id:    mem_neg_memicmp_l_null_locale
 * area:  memory
 * kind:  negative
 * funcs:
 * procs: _memicmp_l
 * crt:   >=80
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _memicmp_l (locale parameter); "Locale" topic: NULL _locale_t conventionally means current locale
 * note:  PROBES what a NULL _locale_t does in _memicmp_l. Believed: treated as the current locale (no handler, no EINVAL,
 *        "ABC" vs "abc" compares equal). If real Windows invokes the handler instead, this probe will show it.
 *        Handler is installed so a fatal call cannot kill the process.
 */
#include "tc.h"
#include <errno.h>

typedef int (__cdecl *mi_t)(const void *, const void *, size_t, void *);
static mi_t p_mi;
static char a[4] = "ABC", b[4] = "abc";

TC_CASE(null_locale_no_handler_equal)
{
    int r; if (!p_mi) TC_SKIP("not exported");
    tc_iph_count = 0; r = p_mi(a, b, 3, NULL);
    CHECK_EQ_I(tc_iph_count, 0);
    CHECK_EQ_I(r, 0);
}
TC_CASE(errno_untouched)
{
    if (!p_mi) TC_SKIP("not exported");
    errno = 0; (void)p_mi(a, b, 3, NULL);
    CHECK_EQ_I(errno, 0);
}
int main(void)
{
    tc_begin();
    p_mi = (mi_t)tc_crt_proc("_memicmp_l");
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::setup no handler API\n"); return 1; }
    TC_RUN(null_locale_no_handler_equal); TC_RUN(errno_untouched);
    return tc_end();
}
