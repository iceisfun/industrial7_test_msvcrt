/* TC
 * id:    sec_neg_wcscat_s_dest_probe
 * area:  secure_crt
 * funcs:
 * procs: wcscat_s wcsncat_s _set_invalid_parameter_handler
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs wcscat_s / wcsncat_s (table lists dest as not modified for NULL/unterminated dest)
 * note:  PROBE: (1) unterminated dest (no NUL within size): EINVAL, handler once, and dest[0] is reset to 0
 *        (the secure-CRT macros reset the string; docs table may say "not modified").
 *        (2) size 0 with a valid dest: EINVAL (not ERANGE), handler once, dest untouched.
 */
#define MINGW_HAS_SECURE_API 1
#include "tc.h"
#include <stdlib.h>
#include <string.h>
#include <errno.h>
TC_DECL_PROC(errno_t, wcscat_s, (wchar_t*,size_t,const wchar_t*));
TC_DECL_PROC(errno_t, wcsncat_s, (wchar_t*,size_t,const wchar_t*,size_t));
static volatile size_t v_zero;
static void setup(void) { tc_iph_install(); tc_iph_count = 0; errno = 0; }

TC_CASE(cat_unterminated_dest) { wchar_t d[4] = {L'a',L'b',L'c',L'd'}; setup(); CHECK_EQ_I(tc_p_wcscat_s(d, 4, L"x"), EINVAL); CHECK_EQ_I(d[0], 0); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(ncat_unterminated_dest) { wchar_t d[4] = {L'a',L'b',L'c',L'd'}; setup(); CHECK_EQ_I(tc_p_wcsncat_s(d, 4, L"x", 1), EINVAL); CHECK_EQ_I(d[0], 0); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(cat_size0) { wchar_t d[8] = L"ab"; setup(); CHECK_EQ_I(tc_p_wcscat_s(d, v_zero, L"c"), EINVAL); CHECK_WSTR(d, L"ab"); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(ncat_size0) { wchar_t d[8] = L"ab"; setup(); CHECK_EQ_I(tc_p_wcsncat_s(d, v_zero, L"c", 1), EINVAL); CHECK_WSTR(d, L"ab"); CHECK_EQ_I(tc_iph_count, 1); }

int main(void) { tc_begin(); TC_LOAD_PROC(wcscat_s); TC_LOAD_PROC(wcsncat_s); TC_RUN(cat_unterminated_dest); TC_RUN(ncat_unterminated_dest); TC_RUN(cat_size0); TC_RUN(ncat_size0); return tc_end(); }
