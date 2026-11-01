/* TC
 * id:    env_neg_getenv_s_null_args
 * area:  env
 * funcs: getenv_s _putenv
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcrt
 * tier:  p0
 * kind:  negative
 * conf:  observed
 * ref:   MS docs getenv_s, _wgetenv_s (parameter table)
 * note:  Handler counts are asserted only where _set_invalid_parameter_handler exists in the DLL. OBSERVED on Win10 22H2
 *        ucrtbase: a NULL buffer with size>0, a non-NULL buffer with size 0, and a NULL preturn are rejected (EINVAL, handler
 *        call), but getenv_s with a NULL varname returns 0, errno 0 and no handler call (differs from the MS docs parameter
 *        table).
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

static int have_iph;
static volatile size_t vsz = 8;

TC_CASE(buffer_null_size_gt0)
{
    size_t ret = 123; int before = tc_iph_count; errno_t e;
    _putenv("TC_E1=value");
    errno = 0; e = getenv_s(&ret, NULL, vsz, "TC_E1");
    CHECK_EQ_I(e, EINVAL); CHECK_EQ_I(errno, EINVAL);
    if (have_iph) CHECK_EQ_I(tc_iph_count - before, 1);
}
TC_CASE(buffer_nonnull_size_0)
{
    size_t ret = 123; char buf[8] = "zz"; int before = tc_iph_count; errno_t e;
    volatile size_t zero = 0;
    errno = 0; e = getenv_s(&ret, buf, zero, "TC_E1");
    CHECK_EQ_I(e, EINVAL); CHECK_EQ_I(errno, EINVAL);
    if (have_iph) CHECK_EQ_I(tc_iph_count - before, 1);
}
TC_CASE(varname_null)
{
    size_t ret = 123; char buf[8] = "zz"; int before = tc_iph_count; errno_t e;
    errno = 0; e = getenv_s(&ret, buf, sizeof buf, NULL);
    CHECK_EQ_I(e, 0); CHECK_EQ_I(errno, 0);   /* observed: a NULL varname is NOT validated, it behaves as "not found" */
    if (have_iph) CHECK_EQ_I(tc_iph_count - before, 0);
}
TC_CASE(preturn_null)
{
    char buf[8] = "zz"; int before = tc_iph_count; errno_t e;
    errno = 0; e = getenv_s(NULL, buf, sizeof buf, "TC_E1");
    CHECK_EQ_I(e, EINVAL); CHECK_EQ_I(errno, EINVAL);
    if (have_iph) CHECK_EQ_I(tc_iph_count - before, 1);
}
TC_CASE(errno_set_each)
{
    size_t ret; char buf[8]; int i;
    for (i = 0; i < 3; i++) {
        errno = 0; ret = 0;
        if (i == 0) getenv_s(&ret, NULL, vsz, "TC_E1");
        else if (i == 1) getenv_s(&ret, buf, sizeof buf, NULL);
        else getenv_s(NULL, buf, sizeof buf, "TC_E1");
        CHECK_EQ_I(errno, i == 1 ? 0 : EINVAL);
    }
}
int main(void)
{
    tc_begin(); have_iph = tc_iph_install();
    TC_RUN(buffer_null_size_gt0); TC_RUN(buffer_nonnull_size_0); TC_RUN(varname_null); TC_RUN(preturn_null); TC_RUN(errno_set_each);
    return tc_end();
}
