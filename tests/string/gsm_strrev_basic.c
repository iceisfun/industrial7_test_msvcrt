/* TC
 * id:    gsm_strrev_basic
 * area:  string
 * funcs: _strrev
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _strrev (reverses order of characters in place, terminator remains, returns pointer to the altered string)
 */
#include "tc.h"
#include <string.h>

TC_CASE(even) { char b[8] = "abcd"; char *volatile r = _strrev(b); CHECK_STR(b, "dcba"); CHECK_EQ_P(r, b); CHECK_EQ_I(b[4], 0); }
TC_CASE(odd)  { char b[8] = "abcde"; char *volatile r = _strrev(b); CHECK_STR(b, "edcba"); CHECK_EQ_P(r, b); CHECK_EQ_I(b[5], 0); }
TC_CASE(tiny)
{
    char one[4] = "x", e[4] = ""; char *volatile r;
    r = _strrev(one); CHECK_STR(one, "x"); CHECK_EQ_P(r, one);
    r = _strrev(e); CHECK_EQ_I(e[0], 0); CHECK_EQ_P(r, e);
}
TC_CASE(double_reverse)
{
    char b[16] = "The quick fox";
    _strrev(b); CHECK_STR(b, "xof kciuq ehT");
    _strrev(b); CHECK_STR(b, "The quick fox");
}
TC_CASE(space_digits)
{
    char b[16] = "a 1 2b"; _strrev(b); CHECK_STR(b, "b2 1 a");
}
TC_CASE(guard)
{
    char b[10]; int i;
    for (i = 0; i < 10; i++) b[i] = 'G';
    b[0] = '1'; b[1] = '2'; b[2] = '3'; b[3] = 0;
    _strrev(b); CHECK_STR(b, "321"); CHECK_EQ_I(b[4], 'G'); CHECK_EQ_I(b[9], 'G');
}
int main(void) { tc_begin(); TC_RUN(even); TC_RUN(odd); TC_RUN(tiny); TC_RUN(double_reverse); TC_RUN(space_digits); TC_RUN(guard); return tc_end(); }
