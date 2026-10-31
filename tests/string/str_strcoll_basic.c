/* TC
 * id:    str_strcoll_basic
 * area:  string
 * funcs: strcoll _stricoll _strncoll _strnicoll
 * procs: 
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   C11 7.24.4.3 (strcoll); MS docs strcoll, _stricoll, _strncoll, _strnicoll
 * note:  C locale: strcoll orders like strcmp. Only the sign of results is checked.
 */
#include "tc.h"
#include <string.h>
#include <errno.h>
static int sgn(int v) { return (v > 0) - (v < 0); }

TC_CASE(strcoll_like_strcmp)
{
    volatile const char *a = "abc", *b = "abd", *c = "abc", *e = "";
    CHECK_EQ_I(sgn(strcoll((const char *)a, (const char *)b)), -1);
    CHECK_EQ_I(sgn(strcoll((const char *)b, (const char *)a)), 1);
    CHECK_EQ_I(strcoll((const char *)a, (const char *)c), 0);
    CHECK_EQ_I(sgn(strcoll((const char *)a, (const char *)e)), 1);
}
TC_CASE(stricoll_fold)
{
    volatile const char *a = "AbC", *b = "aBc", *c = "abd";
    CHECK_EQ_I(_stricoll((const char *)a, (const char *)b), 0);
    CHECK_EQ_I(sgn(_stricoll((const char *)a, (const char *)c)), -1);
    CHECK_EQ_I(sgn(strcoll((const char *)a, (const char *)b)), -1); /* 'A' < 'a' */
}
TC_CASE(strncoll_count)
{
    volatile const char *a = "abcX", *b = "abcY";
    CHECK_EQ_I(_strncoll((const char *)a, (const char *)b, 3), 0);
    CHECK_EQ_I(sgn(_strncoll((const char *)a, (const char *)b, 4)), -1);
    CHECK_EQ_I(_strncoll((const char *)a, (const char *)b, 0), 0);
}
TC_CASE(strnicoll_count_case)
{
    volatile const char *a = "ABCx", *b = "abcY";
    CHECK_EQ_I(_strnicoll((const char *)a, (const char *)b, 3), 0);
    CHECK_EQ_I(sgn(_strnicoll((const char *)a, (const char *)b, 4)), -1);
}
TC_CASE(empty)
{
    volatile const char *e = "", *f = "";
    CHECK_EQ_I(strcoll((const char *)e, (const char *)f), 0);
    CHECK_EQ_I(_stricoll((const char *)e, (const char *)f), 0);
    CHECK_EQ_I(_strncoll((const char *)e, (const char *)f, 5), 0);
    CHECK_EQ_I(_strnicoll((const char *)e, (const char *)f, 5), 0);
}
int main(void) { tc_begin(); TC_RUN(strcoll_like_strcmp); TC_RUN(stricoll_fold); TC_RUN(strncoll_count); TC_RUN(strnicoll_count_case); TC_RUN(empty); return tc_end(); }
