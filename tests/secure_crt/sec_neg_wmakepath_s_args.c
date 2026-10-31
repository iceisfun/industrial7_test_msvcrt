/* TC
 * id:    sec_neg_wmakepath_s_args
 * area:  secure_crt
 * funcs: _wmakepath_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _makepath_s, _wmakepath_s
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <wchar.h>

static volatile size_t v_n = 8;

TC_CASE(null_path_EINVAL) { int r; errno = 0; r = _wmakepath_s(NULL, 10, L"C:", L"\\d\\", L"n", L".e"); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); }
TC_CASE(size_zero_EINVAL) { wchar_t p[16] = L"q"; int r; errno = 0; r = _wmakepath_s(p, 0, L"C:", L"\\d\\", L"n", L".e"); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); }
TC_CASE(too_small_ERANGE) { wchar_t p[32]; int r; errno = 0; r = _wmakepath_s(p, v_n, L"C:", L"\\d\\", L"n", L".e"); CHECK_EQ_I(r, ERANGE); CHECK_EQ_I(errno, ERANGE); }
TC_CASE(handler_counts)
{
    wchar_t p[32]; tc_iph_count = 0;
    _wmakepath_s(NULL, 10, L"C:", L"\\d\\", L"n", L".e");
    _wmakepath_s(p, 0, L"C:", L"\\d\\", L"n", L".e");
    _wmakepath_s(p, v_n, L"C:", L"\\d\\", L"n", L".e");
    CHECK_EQ_I(tc_iph_count, 3);
}
int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(null_path_EINVAL); TC_RUN(size_zero_EINVAL); TC_RUN(too_small_ERANGE); TC_RUN(handler_counts);
    return tc_end();
}
