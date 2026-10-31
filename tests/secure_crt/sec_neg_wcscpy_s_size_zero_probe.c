/* TC
 * id:    sec_neg_wcscpy_s_size_zero_probe
 * area:  secure_crt
 * funcs: wcscpy_s wcsncpy_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs strcpy_s error table (says 0 or too small => ERANGE); CRT validation macro (size > 0) believed to give EINVAL
 * note:  PROBE: wcscpy_s/wcsncpy_s with non-NULL dest and size 0. The docs table lumps "0 or too small" under ERANGE, but the CRT's
 *        string validation (dest != NULL && size > 0) is believed to return EINVAL and leave dest untouched. Asserts EINVAL, errno
 *        EINVAL, dest[0] unchanged, handler count 1.
 */
#include "tc.h"
#include <wchar.h>
#include <errno.h>

#define RESET() do { tc_iph_count = 0; errno = 0; } while (0)
static volatile const wchar_t *v_s;
static volatile size_t v_sz;
static volatile size_t v_n;

TC_CASE(wcscpy_s_size_zero) { wchar_t d[4] = {7, 7, 7, 0}; v_s = L"abc"; v_sz = 0; RESET();
    CHECK_EQ_I(wcscpy_s(d, v_sz, (const wchar_t *)v_s), EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(d[0], 7); }
TC_CASE(wcsncpy_s_size_zero) { wchar_t d[4] = {7, 7, 7, 0}; v_s = L"abc"; v_sz = 0; v_n = 3; RESET();
    CHECK_EQ_I(wcsncpy_s(d, v_sz, (const wchar_t *)v_s, v_n), EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(d[0], 7); }

int main(void) { tc_begin(); if (!tc_iph_install()) return 3;
    TC_RUN(wcscpy_s_size_zero); TC_RUN(wcsncpy_s_size_zero); return tc_end(); }
