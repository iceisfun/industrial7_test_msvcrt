/* TC
 * id:    exp_neg_splitpath_s_small_buffer
 * area:  exports
 * funcs:
 * procs: _splitpath_s _makepath_s
 * crt:   ucrtbase
 * tier:  p2
 * kind:  negative
 * conf:  observed
 * ref:   MS docs _splitpath_s / _makepath_s: ERANGE when a buffer is too small, EINVAL for NULL path
 * note:  OBSERVED (Win10 22H2 19045; raw "tc_iph_count > 0" failed in fname_too_small_erange): a too-small component buffer returns
 *        ERANGE without invoking the invalid-parameter handler (NULL path and _makepath_s still do). Class A. Case 1 now observed-conf.
 */
#include "tc.h"
#include <errno.h>

typedef int (__cdecl *splitpath_s_t)(const char *, char *, size_t, char *, size_t, char *, size_t, char *, size_t);
typedef int (__cdecl *makepath_s_t)(char *, size_t, const char *, const char *, const char *, const char *);
static splitpath_s_t p_split;
static makepath_s_t p_make;

TC_CASE(fname_too_small_erange)
{
    char dr[8], di[64], fn[3], ex[8]; int r;
    if (!p_split) TC_SKIP("_splitpath_s not exported");
    tc_iph_install(); tc_iph_count = 0;
    r = p_split("C:\\dir\\longname.txt", dr, sizeof dr, di, sizeof di, fn, sizeof fn, ex, sizeof ex);
    CHECK_EQ_I(r, ERANGE);
    CHECK_EQ_I(tc_iph_count, 0);   /* observed Win10 22H2: ERANGE for a too-small component buffer without calling the handler */
}
TC_CASE(null_path_einval)
{
    char dr[8], di[64], fn[16], ex[8]; int r;
    if (!p_split) TC_SKIP("_splitpath_s not exported");
    tc_iph_install(); tc_iph_count = 0;
    r = p_split(NULL, dr, sizeof dr, di, sizeof di, fn, sizeof fn, ex, sizeof ex);
    CHECK_EQ_I(r, EINVAL);
    CHECK(tc_iph_count > 0);
}
TC_CASE(makepath_small_erange)
{
    char b[4]; int r;
    if (!p_make) TC_SKIP("_makepath_s not exported");
    tc_iph_install(); tc_iph_count = 0;
    r = p_make(b, sizeof b, "C:", "\\dir\\", "name", ".ext");
    CHECK_EQ_I(r, ERANGE);
    CHECK(tc_iph_count > 0);
}
int main(void)
{
    tc_begin();
    p_split = (splitpath_s_t)tc_crt_proc("_splitpath_s");
    p_make = (makepath_s_t)tc_crt_proc("_makepath_s");
    TC_RUN(fname_too_small_erange); TC_RUN(null_path_einval); TC_RUN(makepath_small_erange);
    return tc_end();
}
