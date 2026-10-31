/* TC
 * id:    mb_ismbc_class_932
 * area:  mbcs
 * funcs: _ismbcalpha _ismbcdigit _ismbcspace _ismbcpunct _ismbcupper _ismbclower _setmbcp
 * crt:   all
 * tier:  p1
 * conf:  observed
 * ref:   MS docs _ismbcalpha etc.
 * note:  Single-byte ASCII classification under code page 932 only; full-width in mb_ismbc_fullwidth_932.
 */
#include "tc.h"
#include <mbstring.h>
#include <mbctype.h>

static volatile unsigned int v;

TC_CASE(alpha) { CHECK_EQ_U(_setmbcp(932), 0); v = 'a'; CHECK(_ismbcalpha(v)); v = 'Z'; CHECK(_ismbcalpha(v)); v = '1'; CHECK(!_ismbcalpha(v)); v = ' '; CHECK(!_ismbcalpha(v)); }
TC_CASE(digit) { CHECK_EQ_U(_setmbcp(932), 0); v = '5'; CHECK(_ismbcdigit(v)); v = 'a'; CHECK(!_ismbcdigit(v)); }
TC_CASE(space) { CHECK_EQ_U(_setmbcp(932), 0); v = 0x20; CHECK(_ismbcspace(v)); v = '\t'; CHECK(_ismbcspace(v)); v = 'x'; CHECK(!_ismbcspace(v)); }
TC_CASE(punct) { CHECK_EQ_U(_setmbcp(932), 0); v = '!'; CHECK(_ismbcpunct(v)); v = 'a'; CHECK(!_ismbcpunct(v)); v = '5'; CHECK(!_ismbcpunct(v)); }
TC_CASE(case_) { CHECK_EQ_U(_setmbcp(932), 0); v = 'A'; CHECK(_ismbcupper(v)); CHECK(!_ismbclower(v)); v = 'a'; CHECK(_ismbclower(v)); CHECK(!_ismbcupper(v)); }

int main(void) { tc_begin(); TC_RUN(alpha); TC_RUN(digit); TC_RUN(space); TC_RUN(punct); TC_RUN(case_); _setmbcp(-3); return tc_end(); }
