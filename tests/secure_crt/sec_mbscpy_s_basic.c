/* TC
 * id:    sec_mbscpy_s_basic
 * area:  secure_crt
 * funcs: _mbscpy_s _mbscat_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * kind:  positive
 * conf:  documented
 * ref:   MS docs strcpy_s, wcscpy_s, _mbscpy_s
 * note:  unsigned char* signatures
 */
#define MINGW_HAS_SECURE_API 1
#include "tc.h"
#include <mbstring.h>
#include <errno.h>
static unsigned char d[16];
TC_CASE(mbscpy_simple) { CHECK_EQ_I(_mbscpy_s(d, 16, (const unsigned char *)"hello"), 0); CHECK_STR((char *)d, "hello"); }
TC_CASE(mbscat_append)
{
    CHECK_EQ_I(_mbscpy_s(d, 16, (const unsigned char *)"foo"), 0);
    CHECK_EQ_I(_mbscat_s(d, 16, (const unsigned char *)"bar"), 0);
    CHECK_STR((char *)d, "foobar");
}
TC_CASE(exact_fit)
{
    unsigned char e[6];
    CHECK_EQ_I(_mbscpy_s(e, 6, (const unsigned char *)"hello"), 0);
    CHECK_STR((char *)e, "hello");
}
TC_CASE(empty_src)
{
    CHECK_EQ_I(_mbscpy_s(d, 16, (const unsigned char *)""), 0);
    CHECK_EQ_U(d[0], 0);
    CHECK_EQ_I(_mbscpy_s(d, 16, (const unsigned char *)"ab"), 0);
    CHECK_EQ_I(_mbscat_s(d, 16, (const unsigned char *)""), 0);
    CHECK_STR((char *)d, "ab");
}
int main(void) { tc_begin(); TC_RUN(mbscpy_simple); TC_RUN(mbscat_append); TC_RUN(exact_fit); TC_RUN(empty_src); return tc_end(); }
