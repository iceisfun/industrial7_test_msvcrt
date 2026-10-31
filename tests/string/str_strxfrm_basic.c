/* TC
 * id:    str_strxfrm_basic
 * area:  string
 * funcs: strxfrm strcoll strcmp
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.24.4.5 (strxfrm); MS docs strxfrm
 * note:  In the "C" locale the transformation is the identity (strcmp of transformed strings == strcoll of the originals, sign-wise).
 */
#include "tc.h"
#include <string.h>

static volatile const char *vs;

TC_CASE(returns_length)
{
    char d[32]; vs = "hello";
    CHECK_EQ_U(strxfrm(d, (const char *)vs, sizeof d), 5);
    vs = "";
    CHECK_EQ_U(strxfrm(d, (const char *)vs, sizeof d), 0);
}
TC_CASE(identity_copy)
{
    char d[32]; vs = "Hello, World";
    memset(d, 'x', sizeof d);
    strxfrm(d, (const char *)vs, sizeof d);
    CHECK_STR(d, "Hello, World");
}
TC_CASE(size_query)
{
    vs = "abcdefg";
    CHECK_EQ_U(strxfrm(NULL, (const char *)vs, 0), 7);
}
TC_CASE(terminated_when_room)
{
    char d[16]; vs = "abc";
    memset(d, 'x', sizeof d);
    CHECK_EQ_U(strxfrm(d, (const char *)vs, 4), 3);
    CHECK_EQ_I(d[3], 0);
    memset(d, 'x', sizeof d);
    CHECK_EQ_U(strxfrm(d, (const char *)vs, 10), 3);
    CHECK_EQ_I(d[3], 0);
}
TC_CASE(order_matches_strcoll)
{
    static const char *v[] = { "apple", "banana", "apple", "Apple", "", "b" };
    int i, j;
    for (i = 0; i < 6; i++) for (j = 0; j < 6; j++) {
        char x[32], y[32]; int a, b, c;
        strxfrm(x, v[i], sizeof x); strxfrm(y, v[j], sizeof y);
        a = strcmp(x, y); b = strcoll(v[i], v[j]);
        c = (a > 0) - (a < 0); a = (b > 0) - (b < 0);
        CHECK_EQ_I(c, a);
    }
}
int main(void)
{
    tc_begin();
    TC_RUN(returns_length); TC_RUN(identity_copy); TC_RUN(size_query); TC_RUN(terminated_when_room); TC_RUN(order_matches_strcoll);
    return tc_end();
}
