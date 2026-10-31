/* TC
 * id:    ct_neg_class_disjointness_sweep
 * area:  ctype
 * funcs: isalnum ispunct iscntrl isprint isspace isalpha isdigit isgraph
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.4.1.x (isalnum, ispunct, iscntrl, isprint, isgraph, isspace)
 */
#include "tc.h"
#include <ctype.h>

static volatile int vc;
#define NEVER_BOTH(a, b) do { int c, bad = 0; for (c = 0; c <= 255; c++) { vc = c; if (a(vc) && b(vc)) bad++; } CHECK_EQ_I(bad, 0); } while (0)

TC_CASE(punct_and_alnum_never_both) { NEVER_BOTH(ispunct, isalnum); }
TC_CASE(cntrl_and_print_never_both) { NEVER_BOTH(iscntrl, isprint); }
TC_CASE(alpha_and_digit_never_both) { NEVER_BOTH(isalpha, isdigit); }
TC_CASE(graph_and_space_char_0x20_never_both)
{
    NEVER_BOTH(isgraph, isspace);
    vc = 0x20; CHECK_EQ_I(isgraph(vc), 0); CHECK(isprint(vc) != 0); CHECK(isspace(vc) != 0);
}
TC_CASE(punct_and_cntrl_never_both) { NEVER_BOTH(ispunct, iscntrl); }

int main(void) { tc_begin(); TC_RUN(punct_and_alnum_never_both); TC_RUN(cntrl_and_print_never_both); TC_RUN(alpha_and_digit_never_both); TC_RUN(graph_and_space_char_0x20_never_both); TC_RUN(punct_and_cntrl_never_both); return tc_end(); }
