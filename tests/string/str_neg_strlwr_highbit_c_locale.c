/* TC
 * id:    str_neg_strlwr_highbit_c_locale
 * area:  string
 * funcs: _strlwr _strupr
 * procs: 
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  inferred
 * ref:   MS docs _strlwr/_strupr (LC_CTYPE dependent); C11 7.4.2.1/7.4.2.2 + 7.11.1.1 (C locale case mapping covers only the 26 letters)
 * note:  Must NOT convert high-bit bytes in the C locale (tests start in the C locale; no setlocale call).
 */
#include "tc.h"
#include <string.h>
#include <errno.h>
static char *(*volatile p_lwr)(char *) = _strlwr;
static char *(*volatile p_upr)(char *) = _strupr;
TC_CASE(lwr_unchanged)
{
    char b[4] = { (char)0xC9, (char)0xC0, 0, 0 };
    p_lwr(b);
    CHECK_EQ_U((unsigned char)b[0], 0xC9); CHECK_EQ_U((unsigned char)b[1], 0xC0);
}
TC_CASE(upr_unchanged)
{
    char b[4] = { (char)0xe9, (char)0xe0, (char)0xff, 0 };
    p_upr(b);
    CHECK_EQ_U((unsigned char)b[0], 0xe9); CHECK_EQ_U((unsigned char)b[1], 0xe0); CHECK_EQ_U((unsigned char)b[2], 0xff);
}
TC_CASE(ascii_around)
{
    char b[8] = { 'A', (char)0xC9, 'z', (char)0xe9, 'Q', 0 };
    p_lwr(b);
    CHECK_EQ_U((unsigned char)b[0], 'a'); CHECK_EQ_U((unsigned char)b[1], 0xC9); CHECK_EQ_U((unsigned char)b[2], 'z');
    CHECK_EQ_U((unsigned char)b[3], 0xe9); CHECK_EQ_U((unsigned char)b[4], 'q');
    p_upr(b);
    CHECK_EQ_U((unsigned char)b[0], 'A'); CHECK_EQ_U((unsigned char)b[1], 0xC9); CHECK_EQ_U((unsigned char)b[2], 'Z');
    CHECK_EQ_U((unsigned char)b[3], 0xe9); CHECK_EQ_U((unsigned char)b[4], 'Q');
}
int main(void) { tc_begin(); TC_RUN(lwr_unchanged); TC_RUN(upr_unchanged); TC_RUN(ascii_around); return tc_end(); }
