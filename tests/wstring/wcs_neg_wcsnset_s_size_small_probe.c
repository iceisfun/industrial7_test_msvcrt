/* TC
 * id:    wcs_neg_wcsnset_s_size_small_probe
 * area:  wstring
 * funcs:
 * procs: _wcsnset_s
 * crt:   >=80
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _wcsnset_s (_VALIDATE_STRING style: str must be NUL-terminated within size)
 * note:  Probes: size (3) smaller than wcslen+1 (7). With count 10 (> size) EINVAL + one handler call is expected whether the
 *        CRT validates the terminator up front or fails after filling; with count 2 (< size) the outcome depends on
 *        whether the up-front check exists (EINVAL, string untouched) or not (success), so those are separate probe cases.
 */
#include "tc.h"
#include <wchar.h>
#include <errno.h>

TC_DECL_PROC(errno_t, _wcsnset_s, (wchar_t *, size_t, wchar_t, size_t));

TC_CASE(size_too_small_count_big_einval)
{
    wchar_t b[8] = { L'a', L'b', L'c', L'd', L'e', L'f', 0 }; int r;
    if (!tc_p__wcsnset_s || !tc_iph_install()) TC_SKIP("missing");
    tc_iph_count = 0;
    r = tc_p__wcsnset_s(b, 3, L'x', 10);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(size_too_small_count_small_einval)
{
    wchar_t b[8] = { L'a', L'b', L'c', L'd', L'e', L'f', 0 }; int r;
    if (!tc_p__wcsnset_s || !tc_iph_install()) TC_SKIP("missing");
    tc_iph_count = 0;
    r = tc_p__wcsnset_s(b, 3, L'x', 2);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(string_not_modified)
{
    wchar_t b[8] = { L'a', L'b', L'c', L'd', L'e', L'f', 0 };
    if (!tc_p__wcsnset_s || !tc_iph_install()) TC_SKIP("missing");
    tc_p__wcsnset_s(b, 3, L'x', 2);
    CHECK_WSTR(b, L"abcdef");
}
int main(void)
{
    tc_begin(); TC_LOAD_PROC(_wcsnset_s);
    TC_RUN(size_too_small_count_big_einval); TC_RUN(size_too_small_count_small_einval); TC_RUN(string_not_modified);
    return tc_end();
}
