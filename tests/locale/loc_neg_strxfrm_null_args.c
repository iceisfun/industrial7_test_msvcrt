/* TC
 * id:    loc_neg_strxfrm_null_args
 * area:  locale
 * funcs: strxfrm wcsxfrm
 * crt:   >=80
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs strxfrm (parameter validation); CRT source convention
 * note:  PROBE: NULL source, or NULL dest with count>0, sets errno EINVAL and invokes the handler. Return value on error is NOT asserted.
 */
#include "tc.h"
#include <string.h>
#include <wchar.h>
#include <errno.h>

static const char *volatile v_null_s;
static const wchar_t *volatile v_null_w;
static const char *volatile v_a = "a";
static const wchar_t *volatile v_wa = L"a";

TC_CASE(src_null_errno_einval)
{
    char b[8]; wchar_t w[8];
    tc_iph_install();
    errno = 0; strxfrm(b, v_null_s, 8); CHECK_EQ_I(errno, EINVAL);
    errno = 0; wcsxfrm(w, v_null_w, 8); CHECK_EQ_I(errno, EINVAL);
}
TC_CASE(dest_null_nonzero_n_errno_einval)
{
    tc_iph_install();
    errno = 0; strxfrm(NULL, v_a, 5); CHECK_EQ_I(errno, EINVAL);
    errno = 0; wcsxfrm(NULL, v_wa, 5); CHECK_EQ_I(errno, EINVAL);
}
TC_CASE(iph_counts)
{
    char b[8]; wchar_t w[8];
    if (!tc_iph_install()) TC_SKIP("no IPH api");
    tc_iph_count = 0;
    strxfrm(b, v_null_s, 8); wcsxfrm(w, v_null_w, 8); strxfrm(NULL, v_a, 5); wcsxfrm(NULL, v_wa, 5);
    CHECK_EQ_I(tc_iph_count, 4);
}
int main(void)
{
    tc_begin();
    TC_RUN(src_null_errno_einval); TC_RUN(dest_null_nonzero_n_errno_einval); TC_RUN(iph_counts);
    return tc_end();
}
