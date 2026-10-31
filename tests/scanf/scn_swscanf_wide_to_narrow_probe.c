/* TC
 * id:    scn_swscanf_wide_to_narrow_probe
 * area:  scanf
 * funcs: swscanf
 * crt:   all
 * tier:  p2
 * conf:  probe
 * ref:   MS docs scanf type field characters (%hs in wide functions reads narrow string)
 * note:  PROBE: %hs in swscanf converts the wide input to multibyte in the "C" locale. For U+00E9 the
 *        outcome is uncertain (ret 0 or 1; if 1 the byte may be 0xE9); for U+4E2D (not representable
 *        in the C locale) we expect the conversion to fail so ret != 1 or buffer not holding a
 *        full match. Only the least controversial facts are asserted. Must not crash.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

static void info(const char *n, int r, char b0)
{ char x[32]; tc_write("TC INFO " TC_TEST_ID "::"); tc_write(n); tc_write(" ret="); tc_i64(r, x); tc_write(x); tc_write(" b0="); tc_i64((unsigned char)b0, x); tc_write(x); tc_write("\n"); }

TC_CASE(e_acute) { char b[16]; int r; b[0] = 'Q'; b[1] = 0; r = swscanf(L"é", L"%hs", b); info("e_acute", r, b[0]); CHECK(r == 0 || r == 1 || r == EOF); if (r == 1) CHECK_EQ_U((unsigned char)b[0], 0xE9); }
TC_CASE(cjk) { char b[16]; int r; b[0] = 'Q'; b[1] = 0; r = swscanf(L"中", L"%hs", b); info("cjk", r, b[0]); CHECK(r == 0); }
TC_CASE(ascii_control) { char b[16]; int r; b[0] = 0; r = swscanf(L"abc", L"%hs", b); CHECK_EQ_I(r, 1); CHECK_STR(b, "abc"); }

int main(void) { tc_begin(); TC_RUN(e_acute); TC_RUN(cjk); TC_RUN(ascii_control); return tc_end(); }
