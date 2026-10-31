/* TC
 * id:    sec_neg_wcsncat_s_args
 * area:  secure_crt
 * funcs: wcsncat_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs strncat_s, wcsncat_s (_TRUNCATE returns STRUNCATE without invoking the handler)
 */
#include "tc.h"
#include <wchar.h>
#include <errno.h>

#define RESET() do { tc_iph_count = 0; errno = 0; } while (0)
static volatile const wchar_t *v_s;
static volatile size_t v_n;

TC_CASE(null_dest_EINVAL) { v_s = L"x"; v_n = 1; RESET(); CHECK_EQ_I(wcsncat_s(NULL, 8, (const wchar_t *)v_s, v_n), EINVAL); CHECK_EQ_I(errno, EINVAL); }
TC_CASE(unterminated_dest_EINVAL) { wchar_t d[4] = {L'a', L'b', L'c', L'd'}; v_s = L"x"; v_n = 1; RESET();
    CHECK_EQ_I(wcsncat_s(d, 4, (const wchar_t *)v_s, v_n), EINVAL); CHECK_EQ_I(errno, EINVAL); }
TC_CASE(does_not_fit_ERANGE) { wchar_t d[6] = L"ab"; v_s = L"cdefgh"; v_n = 6; RESET();
    CHECK_EQ_I(wcsncat_s(d, 6, (const wchar_t *)v_s, v_n), ERANGE); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_I(d[0], 0); }
TC_CASE(truncate_returns_80_no_handler) { wchar_t d[5] = L"ab"; v_s = L"cdefgh"; v_n = (size_t)-1; RESET();
    CHECK_EQ_I(wcsncat_s(d, 5, (const wchar_t *)v_s, v_n), 80); CHECK_EQ_I(STRUNCATE, 80); CHECK_EQ_I(tc_iph_count, 0);
    CHECK_WSTR(d, L"abcd"); }
TC_CASE(handler_counts) { wchar_t d[6] = L"ab", e[4] = {L'a', L'b', L'c', L'd'}; v_s = L"cdefgh"; v_n = 6; RESET();
    wcsncat_s(NULL, 8, (const wchar_t *)v_s, v_n); wcsncat_s(e, 4, (const wchar_t *)v_s, v_n); wcsncat_s(d, 6, (const wchar_t *)v_s, v_n);
    CHECK_EQ_I(tc_iph_count, 3); }

int main(void) { tc_begin(); if (!tc_iph_install()) return 3;
    TC_RUN(null_dest_EINVAL); TC_RUN(unterminated_dest_EINVAL); TC_RUN(does_not_fit_ERANGE); TC_RUN(truncate_returns_80_no_handler); TC_RUN(handler_counts); return tc_end(); }
