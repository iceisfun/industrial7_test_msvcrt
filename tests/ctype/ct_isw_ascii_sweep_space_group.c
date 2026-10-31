/* TC
 * id:    ct_isw_ascii_sweep_space_group
 * area:  ctype
 * funcs: iswspace iswpunct iswcntrl iswprint iswgraph
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.30.2.1; MS docs iswspace
 */
#include "tc.h"
#include <ctype.h>
#include <wchar.h>

static volatile wint_t v;
#define W(f) (v = (wint_t)c, (f)(v) != 0)
#define ALNUM ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'))

TC_CASE(iswspace_six_chars) { int c; for (c = 0; c < 128; c++) CHECK_EQ_I(W(iswspace), c == ' ' || (c >= 9 && c <= 13)); }
TC_CASE(iswpunct_32) { int c; int n = 0; for (c = 0; c < 128; c++) { int e = c > 32 && c < 127 && !ALNUM; n += W(iswpunct); CHECK_EQ_I(W(iswpunct), e); } CHECK_EQ_I(n, 32); }
TC_CASE(iswcntrl_0_1F_7F) { int c; for (c = 0; c < 128; c++) CHECK_EQ_I(W(iswcntrl), c < 32 || c == 127); }
TC_CASE(iswprint_20_7E) { int c; for (c = 0; c < 128; c++) CHECK_EQ_I(W(iswprint), c >= 0x20 && c <= 0x7E); }
TC_CASE(iswgraph_21_7E) { int c; for (c = 0; c < 128; c++) CHECK_EQ_I(W(iswgraph), c >= 0x21 && c <= 0x7E); }

int main(void) { tc_begin(); TC_RUN(iswspace_six_chars); TC_RUN(iswpunct_32); TC_RUN(iswcntrl_0_1F_7F); TC_RUN(iswprint_20_7E); TC_RUN(iswgraph_21_7E); return tc_end(); }
