/* TC
 * id:    mb_probe_mbctolower_latin1_1252
 * area:  mbcs
 * funcs: _mbctolower _mbctoupper _setmbcp
 * crt:   all
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _mbctolower, _mbctoupper
 * note:  PROBE: under _setmbcp(1252) the single-byte case maps 0xC0-0xDE <-> 0xE0-0xFE apply (0xD7 and 0xF7 are not letters).
 *        Unsure whether every CRT generation builds its case table for a single-byte code page.
 */
#include "tc.h"
#include <mbstring.h>
#include <mbctype.h>

static volatile unsigned int v;

TC_CASE(latin1_lower) { CHECK_EQ_U(_setmbcp(1252), 0); v = 0xC0; CHECK_EQ_U(_mbctolower(v), 0xE0); v = 0xDE; CHECK_EQ_U(_mbctolower(v), 0xFE); }
TC_CASE(latin1_upper) { CHECK_EQ_U(_setmbcp(1252), 0); v = 0xE0; CHECK_EQ_U(_mbctoupper(v), 0xC0); v = 0xFE; CHECK_EQ_U(_mbctoupper(v), 0xDE); }
TC_CASE(non_letters) { CHECK_EQ_U(_setmbcp(1252), 0); v = 0xD7; CHECK_EQ_U(_mbctolower(v), 0xD7); CHECK_EQ_U(_mbctoupper(v), 0xD7); v = 0xF7; CHECK_EQ_U(_mbctoupper(v), 0xF7); }

int main(void) { tc_begin(); TC_RUN(latin1_lower); TC_RUN(latin1_upper); TC_RUN(non_letters); _setmbcp(-3); return tc_end(); }
