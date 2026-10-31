/* TC
 * id:    ct_class_relations_sweep
 * area:  ctype
 * funcs: isalpha isdigit isalnum isupper islower isprint isgraph isspace iscntrl ispunct
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.4.1.x (C locale)
 */
#include "tc.h"
#include <ctype.h>

static volatile int v;
#define B(f) (v = c, (f)(v) != 0)

TC_CASE(alnum_iff_alpha_or_digit) { int c; for (c = -1; c < 128; c++) CHECK_EQ_I(B(isalnum), B(isalpha) || B(isdigit)); }
TC_CASE(alpha_iff_upper_or_lower) { int c; for (c = -1; c < 128; c++) CHECK_EQ_I(B(isalpha), B(isupper) || B(islower)); }
TC_CASE(graph_iff_print_and_not_space_char) { int c; for (c = -1; c < 128; c++) CHECK_EQ_I(B(isgraph), B(isprint) && c != ' '); }
TC_CASE(print_equals_graph_or_space_0x20) { int c; for (c = -1; c < 128; c++) CHECK_EQ_I(B(isprint), B(isgraph) || c == 0x20); }
TC_CASE(punct_iff_graph_and_not_alnum) { int c; for (c = -1; c < 128; c++) CHECK_EQ_I(B(ispunct), B(isgraph) && !B(isalnum)); }
TC_CASE(cntrl_iff_not_print_in_ascii) { int c; for (c = 0; c < 128; c++) CHECK_EQ_I(B(iscntrl), !B(isprint)); }
TC_CASE(space_implies_not_graph) { int c; for (c = -1; c < 128; c++) if (B(isspace)) CHECK(!B(isgraph)); }

int main(void) { tc_begin(); TC_RUN(alnum_iff_alpha_or_digit); TC_RUN(alpha_iff_upper_or_lower); TC_RUN(graph_iff_print_and_not_space_char); TC_RUN(print_equals_graph_or_space_0x20); TC_RUN(punct_iff_graph_and_not_alnum); TC_RUN(cntrl_iff_not_print_in_ascii); TC_RUN(space_implies_not_graph); return tc_end(); }
