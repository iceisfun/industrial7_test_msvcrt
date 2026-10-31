/* TC
 * id:    sec_neg_wcscat_s_args
 * area:  secure_crt
 * funcs: wcscat_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs strcat_s, wcscat_s (error conditions table)
 */
#include "tc.h"
#include <wchar.h>
#include <errno.h>

#define RESET() do { tc_iph_count = 0; errno = 0; } while (0)
static volatile const wchar_t *v_s;

TC_CASE(null_dest_EINVAL) { v_s = L"x"; RESET(); CHECK_EQ_I(wcscat_s(NULL, 8, (const wchar_t *)v_s), EINVAL); CHECK_EQ_I(errno, EINVAL); }
TC_CASE(null_src_EINVAL) { wchar_t d[8] = L"ab"; RESET(); v_s = NULL;
    CHECK_EQ_I(wcscat_s(d, 8, (const wchar_t *)v_s), EINVAL); CHECK_EQ_I(errno, EINVAL); }
TC_CASE(unterminated_dest_EINVAL) { wchar_t d[4] = {L'a', L'b', L'c', L'd'}; v_s = L"x"; RESET();
    CHECK_EQ_I(wcscat_s(d, 4, (const wchar_t *)v_s), EINVAL); CHECK_EQ_I(errno, EINVAL); }
TC_CASE(too_small_ERANGE_dest_emptied) { wchar_t d[4] = L"ab"; v_s = L"cde"; RESET();
    CHECK_EQ_I(wcscat_s(d, 4, (const wchar_t *)v_s), ERANGE); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_I(d[0], 0); }
TC_CASE(handler_counts) { wchar_t d[4] = L"ab", e[4] = {L'a', L'b', L'c', L'd'}; v_s = L"cde"; RESET();
    wcscat_s(NULL, 8, (const wchar_t *)v_s); CHECK_EQ_I(tc_iph_count, 1);
    wcscat_s(e, 4, (const wchar_t *)v_s); CHECK_EQ_I(tc_iph_count, 2);
    wcscat_s(d, 4, (const wchar_t *)v_s); CHECK_EQ_I(tc_iph_count, 3); }

int main(void) { tc_begin(); if (!tc_iph_install()) return 3;
    TC_RUN(null_dest_EINVAL); TC_RUN(null_src_EINVAL); TC_RUN(unterminated_dest_EINVAL); TC_RUN(too_small_ERANGE_dest_emptied); TC_RUN(handler_counts); return tc_end(); }
