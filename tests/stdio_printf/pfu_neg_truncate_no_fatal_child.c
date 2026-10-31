/* TC
 * id:    pfu_neg_truncate_no_fatal_child
 * area:  stdio_printf
 * kind:  negative
 * funcs: _snprintf_s _snwprintf_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _snprintf_s, _snwprintf_s ("_TRUNCATE: ... returns -1 ... no invalid parameter"; count < size truncates silently)
 * note:  Must-not-fail check: truncation never invokes the (fatal) default handler. Child exits 0 when return values and contents are as documented.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

TC_CASE(truncate_narrow) { DWORD c = 99; CHECK(tc_spawn_self("tn", &c, 20000)); CHECK_EQ_U(c, 0); }
TC_CASE(truncate_wide)   { DWORD c = 99; CHECK(tc_spawn_self("tw", &c, 20000)); CHECK_EQ_U(c, 0); }
TC_CASE(count_lt_size)   { DWORD c = 99; CHECK(tc_spawn_self("cl", &c, 20000)); CHECK_EQ_U(c, 0); }

int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        char b[16]; wchar_t w[16]; volatile size_t four = 4, eight = 8, t = (size_t)-1;
        if (c[0] == 't' && c[1] == 'n') {
            int r = _snprintf_s(b, four, t, "abcdef");
            return (r == -1 && b[0] == 'a' && b[1] == 'b' && b[2] == 'c' && b[3] == 0) ? 0 : 10;
        }
        if (c[0] == 't') {
            int r = _snwprintf_s(w, four, t, L"abcdef");
            return (r == -1 && w[0] == L'a' && w[1] == L'b' && w[2] == L'c' && w[3] == 0) ? 0 : 11;
        }
        {
            int r = _snprintf_s(b, eight, 3, "abcdef");
            return (r == -1 && b[0] == 'a' && b[2] == 'c' && b[3] == 0) ? 0 : 12;
        }
    }
    TC_RUN(truncate_narrow); TC_RUN(truncate_wide); TC_RUN(count_lt_size);
    return tc_end();
}
