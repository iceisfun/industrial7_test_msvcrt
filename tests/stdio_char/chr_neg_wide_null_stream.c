/* TC
 * id:    chr_neg_wide_null_stream
 * area:  stdio_char
 * kind:  negative
 * funcs: fgetwc fputwc ungetwc fgetws fputws
 * crt:   >=80
 * tier:  p0
 * conf:  observed
 * ref:   MS docs fgetwc, fputwc, ungetwc, fgetws, fputws; parameter validation
 * note:  Each wide function with a NULL FILE*: failure value, errno EINVAL and one handler call.
 *        TRIAGE win10, class A): fputws(L"x", NULL) returns EOF (-1), not WEOF (0xFFFF): observed on ucrtbase 10.0.19045 (got=-1);
 *        C11 7.29.3.4 also specifies EOF for fputws errors. errno EINVAL and one handler call unchanged.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <wchar.h>

#define SETUP() do { tc_iph_count = 0; errno = 0; } while (0)
#define EXPECT() do { CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); } while (0)
TC_CASE(fgetwc_null)  { SETUP(); CHECK_EQ_U(fgetwc(NULL), WEOF); EXPECT(); }
TC_CASE(fputwc_null)  { SETUP(); CHECK_EQ_U(fputwc(L'a', NULL), WEOF); EXPECT(); }
TC_CASE(ungetwc_null) { SETUP(); CHECK_EQ_U(ungetwc(L'a', NULL), WEOF); EXPECT(); }
TC_CASE(fgetws_null)  { wchar_t buf[10]; SETUP(); CHECK(fgetws(buf, 10, NULL) == NULL); EXPECT(); }
TC_CASE(fputws_null)  { SETUP(); CHECK_EQ_I(fputws(L"x", NULL), EOF); EXPECT(); }
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) return 2;
    TC_RUN(fgetwc_null); TC_RUN(fputwc_null); TC_RUN(ungetwc_null); TC_RUN(fgetws_null); TC_RUN(fputws_null);
    return tc_end();
}
