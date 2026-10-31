/* TC
 * id:    sec_neg_strset_s_args
 * area:  secure_crt
 * funcs: _strset_s _strnset_s
 * procs: 
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _strset_s, _strnset_s: EINVAL on NULL str / size 0 / unterminated within size
 * note:  Return values and errno only; buffer contents after failure are not asserted. _strnset_s unterminated case uses count >= size so the
 *        scan must reach the end of the buffer (with a small count the function may return before noticing the missing NUL).
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

static void ini(void) { tc_iph_install(); tc_iph_count = 0; errno = 0; }
TC_CASE(strset_null_EINVAL) { char *volatile p = NULL; ini(); CHECK_EQ_I(_strset_s(p, 8, 'x'), EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(strset_size_zero_EINVAL) { char b[4] = "ab"; volatile size_t n = 0; ini(); CHECK_EQ_I(_strset_s(b, n, 'x'), EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(strset_unterminated_EINVAL) { char b[4] = { 'a', 'b', 'c', 'd' }; volatile size_t n = 4; ini(); CHECK_EQ_I(_strset_s(b, n, 'x'), EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(strnset_null_EINVAL) { char *volatile p = NULL; ini(); CHECK_EQ_I(_strnset_s(p, 8, 'x', 2), EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(strnset_unterminated_EINVAL) { char b[4] = { 'a', 'b', 'c', 'd' }; volatile size_t n = 4; ini(); CHECK_EQ_I(_strnset_s(b, n, 'x', 8), EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(handler_counts)
{
    char *volatile p = NULL; char ok[8] = "abc"; ini();
    _strset_s(p, 4, 'x'); _strnset_s(p, 4, 'x', 1);
    CHECK_EQ_I(tc_iph_count, 2);
    CHECK_EQ_I(_strset_s(ok, sizeof ok, 'z'), 0); CHECK_STR(ok, "zzz");
    CHECK_EQ_I(tc_iph_count, 2);
}
int main(void) { tc_begin(); TC_RUN(strset_null_EINVAL); TC_RUN(strset_size_zero_EINVAL); TC_RUN(strset_unterminated_EINVAL); TC_RUN(strnset_null_EINVAL); TC_RUN(strnset_unterminated_EINVAL); TC_RUN(handler_counts); return tc_end(); }
