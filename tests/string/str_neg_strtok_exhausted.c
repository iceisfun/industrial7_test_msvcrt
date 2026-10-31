/* TC
 * id:    str_neg_strtok_exhausted
 * area:  string
 * funcs: strtok
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.24.5.8 (null pointer if no token)
 */
#include "tc.h"
#include <string.h>

TC_CASE(only_delims) { char b[] = ",,,;;"; CHECK(strtok(b, ",;") == NULL); }
TC_CASE(empty_string) { char b[] = ""; CHECK(strtok(b, ",") == NULL); }
TC_CASE(repeated_null_after_last)
{
    char b[] = "x,y";
    CHECK_STR(strtok(b, ","), "x"); CHECK_STR(strtok(NULL, ","), "y");
    CHECK(strtok(NULL, ",") == NULL); CHECK(strtok(NULL, ",") == NULL); CHECK(strtok(NULL, ",") == NULL);
}
TC_CASE(null_after_exhaustion)
{
    char b[] = "x";
    CHECK_STR(strtok(b, ","), "x"); CHECK(strtok(NULL, ",") == NULL);
    CHECK(strtok(NULL, " ") == NULL);
}
TC_CASE(new_string_restarts)
{
    char b[] = "x,y", c[] = "p,q";
    CHECK_STR(strtok(b, ","), "x"); CHECK(strtok(NULL, ",") != NULL); CHECK(strtok(NULL, ",") == NULL);
    CHECK_STR(strtok(c, ","), "p"); CHECK_STR(strtok(NULL, ","), "q");
}

int main(void) { tc_begin(); TC_RUN(only_delims); TC_RUN(empty_string); TC_RUN(repeated_null_after_last); TC_RUN(null_after_exhaustion); TC_RUN(new_string_restarts); return tc_end(); }
