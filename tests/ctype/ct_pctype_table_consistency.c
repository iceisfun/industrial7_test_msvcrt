/* TC
 * id:    ct_pctype_table_consistency
 * area:  ctype
 * funcs: isalpha isdigit isspace ispunct iscntrl isxdigit isupper islower
 * procs: __pctype_func
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  observed
 * ref:   ctype.h macros index the table from -1 (EOF) to 255
 */
#include "tc.h"
#include <ctype.h>

static volatile int v;
TC_DECL_PROC(const unsigned short *, __pctype_func, (void));
#define ALPHAM 0x103
#define B(f) (v = c, (f)(v) != 0)

TC_CASE(table_index_minus1_is_EOF_entry) { const unsigned short *t = tc_p___pctype_func(); CHECK(t != NULL); if (t) CHECK_EQ_I(t[-1] & 0x1FF, 0); }
TC_CASE(alpha_matches_0x103_mask) { const unsigned short *t = tc_p___pctype_func(); int c; for (c = -1; c < 256; c++) CHECK_EQ_I((t[c] & ALPHAM) != 0, B(isalpha)); }
TC_CASE(digit_space_punct_cntrl_xdigit_upper_lower_match)
{
    const unsigned short *t = tc_p___pctype_func(); int c;
    for (c = -1; c < 256; c++) {
        CHECK_EQ_I((t[c] & 0x4) != 0, B(isdigit)); CHECK_EQ_I((t[c] & 0x8) != 0, B(isspace));
        CHECK_EQ_I((t[c] & 0x10) != 0, B(ispunct)); CHECK_EQ_I((t[c] & 0x20) != 0, B(iscntrl));
        CHECK_EQ_I((t[c] & 0x80) != 0, B(isxdigit)); CHECK_EQ_I((t[c] & 0x1) != 0, B(isupper));
        CHECK_EQ_I((t[c] & 0x2) != 0, B(islower));
    }
}
TC_CASE(high_range_128_255_has_no_class_bits) { const unsigned short *t = tc_p___pctype_func(); int c; for (c = 128; c < 256; c++) CHECK_EQ_I(t[c] & 0x13F, 0); }

int main(void) { tc_begin(); TC_LOAD_PROC(__pctype_func); if (!tc_p___pctype_func) { tc_write("TC FAIL pctype::__pctype_func not exported\n"); return 1; } TC_RUN(table_index_minus1_is_EOF_entry); TC_RUN(alpha_matches_0x103_mask); TC_RUN(digit_space_punct_cntrl_xdigit_upper_lower_match); TC_RUN(high_range_128_255_has_no_class_bits); return tc_end(); }
