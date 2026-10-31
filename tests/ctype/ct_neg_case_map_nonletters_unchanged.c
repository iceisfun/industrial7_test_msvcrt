/* TC
 * id:    ct_neg_case_map_nonletters_unchanged
 * area:  ctype
 * funcs: toupper tolower towupper towlower
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.4.2, 7.30.3.1; WEOF = 0xFFFF as wint_t
 */
#include "tc.h"
#include <ctype.h>
#include <wctype.h>

static volatile int vc;
static volatile unsigned int vw;
#define ISLET(c) (((c) >= 'A' && (c) <= 'Z') || ((c) >= 'a' && (c) <= 'z'))
#define UNCHANGED_RANGE(lo, hi) do { int c, bad = 0; for (c = (lo); c <= (hi); c++) { if (ISLET(c)) continue; vc = c; vw = (unsigned)c; \
    if (toupper(vc) != c || tolower(vc) != c || towupper(vw) != (unsigned)c || towlower(vw) != (unsigned)c) bad++; } CHECK_EQ_I(bad, 0); } while (0)

TC_CASE(digits_unchanged) { UNCHANGED_RANGE('0', '9'); }
TC_CASE(punct_unchanged) { UNCHANGED_RANGE(0x21, 0x2F); UNCHANGED_RANGE(0x3A, 0x40); UNCHANGED_RANGE(0x5B, 0x60); UNCHANGED_RANGE(0x7B, 0x7E); }
TC_CASE(control_and_nul_unchanged) { UNCHANGED_RANGE(0, 0x20); UNCHANGED_RANGE(0x7F, 0x7F); }
TC_CASE(tolower_of_lower_unchanged) { int c, bad = 0; for (c = 'a'; c <= 'z'; c++) { vc = c; vw = (unsigned)c; if (tolower(vc) != c || towlower(vw) != (unsigned)c) bad++; } CHECK_EQ_I(bad, 0); }
TC_CASE(toupper_of_upper_unchanged) { int c, bad = 0; for (c = 'A'; c <= 'Z'; c++) { vc = c; vw = (unsigned)c; if (toupper(vc) != c || towupper(vw) != (unsigned)c) bad++; } CHECK_EQ_I(bad, 0); }
TC_CASE(EOF_and_WEOF_unchanged)
{
    vc = -1; CHECK_EQ_I(toupper(vc), -1); CHECK_EQ_I(tolower(vc), -1);
    vw = 0xFFFF; CHECK_EQ_U(towupper(vw), 0xFFFF); CHECK_EQ_U(towlower(vw), 0xFFFF);
}

int main(void) { tc_begin(); TC_RUN(digits_unchanged); TC_RUN(punct_unchanged); TC_RUN(control_and_nul_unchanged); TC_RUN(tolower_of_lower_unchanged); TC_RUN(toupper_of_upper_unchanged); TC_RUN(EOF_and_WEOF_unchanged); return tc_end(); }
