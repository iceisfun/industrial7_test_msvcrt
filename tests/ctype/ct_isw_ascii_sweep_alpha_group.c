/* TC
 * id:    ct_isw_ascii_sweep_alpha_group
 * area:  ctype
 * funcs: iswalpha iswdigit iswalnum iswupper iswlower iswxdigit
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.30.2.1; MS docs isalpha, iswalpha
 */
#include "tc.h"
#include <ctype.h>
#include <wchar.h>

static volatile wint_t v;
#define W(f) (v = (wint_t)c, (f)(v) != 0)

TC_CASE(iswalpha) { int c; for (c = 0; c < 128; c++) CHECK_EQ_I(W(iswalpha), (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')); }
TC_CASE(iswdigit) { int c; for (c = 0; c < 128; c++) CHECK_EQ_I(W(iswdigit), c >= '0' && c <= '9'); }
TC_CASE(iswalnum) { int c; for (c = 0; c < 128; c++) CHECK_EQ_I(W(iswalnum), (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9')); }
TC_CASE(iswupper) { int c; for (c = 0; c < 128; c++) CHECK_EQ_I(W(iswupper), c >= 'A' && c <= 'Z'); }
TC_CASE(iswlower) { int c; for (c = 0; c < 128; c++) CHECK_EQ_I(W(iswlower), c >= 'a' && c <= 'z'); }
TC_CASE(iswxdigit) { int c; for (c = 0; c < 128; c++) CHECK_EQ_I(W(iswxdigit), (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F')); }

int main(void) { tc_begin(); TC_RUN(iswalpha); TC_RUN(iswdigit); TC_RUN(iswalnum); TC_RUN(iswupper); TC_RUN(iswlower); TC_RUN(iswxdigit); return tc_end(); }
