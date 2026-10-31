/* TC
 * id:    str_strrev_basic
 * area:  string
 * funcs: _strrev
 * procs: 
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _strrev

 */
#include "tc.h"
#include <string.h>
#include <errno.h>
static char *(*volatile p_rev)(char *) = _strrev;
TC_CASE(odd)  { char b[8] = "abcde"; CHECK_STR(p_rev(b), "edcba"); }
TC_CASE(even) { char b[8] = "abcd"; CHECK_STR(p_rev(b), "dcba"); CHECK_EQ_U(strlen(b), 4); }
TC_CASE(single) { char b[4] = "x"; CHECK_STR(p_rev(b), "x"); }
TC_CASE(empty) { char b[4] = ""; CHECK_STR(p_rev(b), ""); }
TC_CASE(same_ptr) { char b[8] = "abc"; CHECK_EQ_P(p_rev(b), b); }
TC_CASE(palindrome) { char b[8] = "abcba"; CHECK_STR(p_rev(b), "abcba"); }
TC_CASE(highbit)
{
    char b[5] = { (char)0x81, (char)0x82, (char)0xe9, (char)0xff, 0 };
    p_rev(b);
    CHECK_EQ_U((unsigned char)b[0], 0xff); CHECK_EQ_U((unsigned char)b[1], 0xe9);
    CHECK_EQ_U((unsigned char)b[2], 0x82); CHECK_EQ_U((unsigned char)b[3], 0x81); CHECK_EQ_U(b[4], 0);
}
int main(void) { tc_begin(); TC_RUN(odd); TC_RUN(even); TC_RUN(single); TC_RUN(empty); TC_RUN(same_ptr); TC_RUN(palindrome); TC_RUN(highbit); return tc_end(); }
