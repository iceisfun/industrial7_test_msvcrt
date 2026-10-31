/* TC
 * id:    pfi_neg_sprintf_l_null_locale
 * area:  stdio_printf
 * kind:  negative
 * funcs: _sprintf_l _sprintf_s_l _snprintf_l
 * crt:   ucrtbase
 * tier:  p0
 * conf:  observed
 * note:  NULL locale meaning "current locale" and the ERANGE errno on overflow are widely relied upon but not spelled out in the docs.
 * ref:   MS docs _sprintf_l/_sprintf_s_l/_snprintf_l (NULL locale = current locale; NULL buffer EINVAL; overflow ERANGE)
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

TC_CASE(null_locale_ok) { char b[32]; volatile int v = 5; int r; tc_iph_count = 0; r = _sprintf_l(b, "%d", NULL, v); CHECK_EQ_I(r, 1); CHECK_STR(b, "5"); CHECK_EQ_I(tc_iph_count, 0); }
TC_CASE(s_l_null_buffer) { volatile int v = 5; int r; errno = 0; tc_iph_count = 0; r = _sprintf_s_l(NULL, 10, "%d", NULL, v); CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(s_l_overflow) { char b[8]; volatile int v = 123; int r; errno = 0; tc_iph_count = 0; r = _sprintf_s_l(b, 2, "%d", NULL, v); CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(snprintf_l_guard)
{
    char b[16]; volatile int v = 12345; int r, i;
    for (i = 0; i < 16; i++) b[i] = 'G';
    r = _snprintf_l(b, 3, "%d", NULL, v);
    CHECK(r == -1 || r == 5);
    for (i = 3; i < 16; i++) CHECK_EQ_I(b[i], 'G');
    CHECK(b[0] == '1' && b[1] == '2' && b[2] == '3');
}
int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
    TC_RUN(null_locale_ok); TC_RUN(s_l_null_buffer); TC_RUN(s_l_overflow); TC_RUN(snprintf_l_guard);
    return tc_end();
}
