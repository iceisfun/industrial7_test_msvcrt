/* TC
 * id:    mb_mbslen_932_basic
 * area:  mbcs
 * funcs: _mbslen _setmbcp
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _mbslen: number of multibyte characters in the current multibyte code page
 */
#include "tc.h"
#include <mbstring.h>
#include <mbctype.h>

static volatile int v_cp = 932;

TC_CASE(plain) { static const unsigned char e[] = "", a[] = "abc";
    CHECK_EQ_I(_setmbcp(v_cp), 0);
    CHECK_EQ_U(_mbslen(e), 0); CHECK_EQ_U(_mbslen(a), 3); }
TC_CASE(double_byte) { static const unsigned char s[] = { 0x82, 0xA0, 0x82, 0xA2, 0 };
    CHECK_EQ_I(_setmbcp(v_cp), 0); CHECK_EQ_U(_mbslen(s), 2); }
TC_CASE(mixed) { static const unsigned char s[] = { 'a', 0x82, 0xA0, 'b', 0 };
    CHECK_EQ_I(_setmbcp(v_cp), 0); CHECK_EQ_U(_mbslen(s), 3); }
TC_CASE(kana) { static const unsigned char s[] = { 0xB1, 0xB2, 0 };
    CHECK_EQ_I(_setmbcp(v_cp), 0); CHECK_EQ_U(_mbslen(s), 2); }

int main(void) { tc_begin(); TC_RUN(plain); TC_RUN(double_byte); TC_RUN(mixed); TC_RUN(kana); _setmbcp(_MB_CP_ANSI); return tc_end(); }
