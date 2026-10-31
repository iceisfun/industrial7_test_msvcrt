/* TC
 * id:    mb_mbctolower_1252
 * area:  mbcs
 * funcs: _mbctolower _mbctoupper _setmbcp
 * crt:   all
 * tier:  p1
 * conf:  observed
 * ref:   MS docs _mbctolower, _mbctoupper
 * note:  ASCII only under cp1252; Latin-1 case mapping is in mb_probe_mbctolower_latin1_1252.
 */
#include "tc.h"
#include <mbstring.h>
#include <mbctype.h>

static volatile unsigned int v;

TC_CASE(ascii) { CHECK_EQ_U(_setmbcp(1252), 0); v = 'Q'; CHECK_EQ_U(_mbctolower(v), 'q'); v = 'q'; CHECK_EQ_U(_mbctoupper(v), 'Q'); v = '5'; CHECK_EQ_U(_mbctolower(v), '5'); }

int main(void) { tc_begin(); TC_RUN(ascii); _setmbcp(-3); return tc_end(); }
