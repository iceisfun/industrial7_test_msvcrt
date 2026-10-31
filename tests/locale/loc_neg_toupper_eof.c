/* TC
 * id:    loc_neg_toupper_eof
 * area:  locale
 * kind:  negative
 * funcs: tolower toupper towlower towupper
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.4 (EOF is a permitted argument); MS docs tolower, toupper, towlower, towupper
 * note:  EOF (-1) and WEOF (0xFFFF) must pass through unchanged; value 0 maps to 0.
 */
#include "tc.h"
#include <ctype.h>
#include <wchar.h>
#include <stdio.h>

static volatile int v_eof = EOF, v_zero = 0;
static volatile wint_t v_weof = WEOF, v_wzero = 0;

TC_CASE(eof_narrow) { CHECK_EQ_I(tolower(v_eof), EOF); CHECK_EQ_I(toupper(v_eof), EOF); }
TC_CASE(weof_wide) { CHECK_EQ_U(towlower(v_weof), WEOF); CHECK_EQ_U(towupper(v_weof), WEOF); }
TC_CASE(nul_identity) { CHECK_EQ_I(tolower(v_zero), 0); CHECK_EQ_I(toupper(v_zero), 0); CHECK_EQ_U(towlower(v_wzero), 0); CHECK_EQ_U(towupper(v_wzero), 0); }

int main(void) { tc_begin(); TC_RUN(eof_narrow); TC_RUN(weof_wide); TC_RUN(nul_identity); return tc_end(); }
