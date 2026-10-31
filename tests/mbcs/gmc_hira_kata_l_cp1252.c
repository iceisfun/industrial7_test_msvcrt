/* TC
 * id:    gmc_hira_kata_l_cp1252
 * area:  mbcs
 * funcs: _ismbchira_l _ismbckata_l _setmbcp
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   MS docs _ismbchira, _ismbckata
 * note:  PROBE: range tests are gated on code page 932 (false under 1252). ASCII 'a' false is certain and kept in its own case.
 */
#include "tc.h"
#include <mbstring.h>
#include <mbctype.h>

static volatile unsigned int v;
static int mb(unsigned int c, int (__cdecl *f)(unsigned int)) { v = c; return f(v) != 0; }
TC_CASE(ascii) { CHECK_EQ_I(_setmbcp(1252), 0); v = 'a'; CHECK(!_ismbchira_l(v, NULL)); CHECK(!_ismbckata_l(v, NULL)); }
TC_CASE(gated) { CHECK_EQ_I(_setmbcp(1252), 0); v = 0x829F; CHECK(!_ismbchira_l(v, NULL)); v = 0x8340; CHECK(!_ismbckata_l(v, NULL)); }
int main(void) { tc_begin(); TC_RUN(ascii); TC_RUN(gated); _setmbcp(-3); return tc_end(); }
