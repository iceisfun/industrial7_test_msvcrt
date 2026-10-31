/* TC
 * id:    env_neg_probe_dupenv_s_error_outputs
 * area:  env
 * kind:  negative
 * funcs: _dupenv_s _wdupenv_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _dupenv_s (only the EINVAL return is documented)
 * note:  PROBE: output state after EINVAL. With varname==NULL, *buffer is expected to be NULL and *count 0 (the CRT clears
 *        outputs before validating varname). With buffer==NULL, *count is expected to be left untouched.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

static char sentinel[2] = "s";
static wchar_t wsentinel[2] = L"s";
static const char *volatile nullp = NULL;
static const wchar_t *volatile wnullp = NULL;

TC_CASE(varname_null_buffer_nulled)
{
    char *b = sentinel; size_t n = 7;
    CHECK_EQ_I(_dupenv_s(&b, &n, nullp), EINVAL);
    CHECK(b == NULL);
}
TC_CASE(varname_null_count_zero)
{
    char *b = sentinel; size_t n = 7;
    _dupenv_s(&b, &n, nullp);
    CHECK_EQ_U(n, 0);
}
TC_CASE(buffer_null_count_untouched)
{
    size_t n = 7;
    CHECK_EQ_I(_dupenv_s(NULL, &n, "X"), EINVAL);
    CHECK_EQ_U(n, 7);
}
TC_CASE(wide_varname_null_buffer_nulled)
{
    wchar_t *b = wsentinel; size_t n = 7;
    CHECK_EQ_I(_wdupenv_s(&b, &n, wnullp), EINVAL);
    CHECK(b == NULL);
    CHECK_EQ_U(n, 0);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC SKIP " TC_TEST_ID " no invalid parameter handler API\n"); return tc_end(); }
    TC_RUN(varname_null_buffer_nulled); TC_RUN(varname_null_count_zero);
    TC_RUN(buffer_null_count_untouched); TC_RUN(wide_varname_null_buffer_nulled);
    return tc_end();
}
