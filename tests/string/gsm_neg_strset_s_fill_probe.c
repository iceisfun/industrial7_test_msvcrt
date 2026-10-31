/* TC
 * id:    gsm_neg_strset_s_fill_probe
 * area:  string
 * kind:  negative
 * funcs:
 * procs: _strset_s _strnset_s
 * crt:   >=90
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _strset_s (no documented validation of the fill character)
 * note:  Probes whether a fill value >= 0x80 (or > 255) is rejected. Expectation (most likely): accepted, returns 0 with no handler call,
 *        stored as the low byte. Second probe: _strnset_s with count (100) larger than the buffer size but a terminator found first is not an error.
 *        If real Windows rejects it with EINVAL this test records that.
 */
#include "tc.h"
TC_DECL_PROC(int, _strset_s, (char *, size_t, int));

TC_CASE(high_fill_accepted)
{
    char b[8] = "abc"; tc_iph_count = 0;
    if (!tc_p__strset_s) TC_SKIP("not exported");
    CHECK_EQ_I(tc_p__strset_s(b, sizeof b, 0xE9), 0);
    CHECK_EQ_I(tc_iph_count, 0);
    CHECK_EQ_I((unsigned char)b[0], 0xE9); CHECK_EQ_I(b[3], 0);
}
TC_DECL_PROC(int, _strnset_s, (char *, size_t, int, size_t));
TC_CASE(nset_count_larger_than_size)
{
    char b[8] = "abc"; tc_iph_count = 0;
    if (!tc_p__strnset_s) TC_SKIP("not exported");
    CHECK_EQ_I(tc_p__strnset_s(b, sizeof b, 'z', 100), 0); CHECK_STR(b, "zzz"); CHECK_EQ_I(b[3], 0); CHECK_EQ_I(tc_iph_count, 0);
}
int main(void) { tc_begin(); if (!tc_iph_install()) return 3; TC_LOAD_PROC(_strset_s); TC_LOAD_PROC(_strnset_s); TC_RUN(high_fill_accepted); TC_RUN(nset_count_larger_than_size); return tc_end(); }
