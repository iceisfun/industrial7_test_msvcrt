/* TC
 * id:    str_strtok_modifies_and_pointer_identity
 * area:  string
 * funcs: strtok
 * crt:   all
 * tier:  p1
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.24.5.8 (tokens are within the original string; delimiter overwritten with a null character)
 */
#include "tc.h"
#include <string.h>

TC_CASE(pointers)
{
    char b[] = "ab,cde;f";
    char *t1 = strtok(b, ",;"), *t2 = strtok(NULL, ",;"), *t3 = strtok(NULL, ",;");
    CHECK_EQ_P(t1, b); CHECK_EQ_P(t2, b + 3); CHECK_EQ_P(t3, b + 7);
    CHECK(strtok(NULL, ",;") == NULL);
}
TC_CASE(nul_at_delimiter)
{
    char b[] = "ab,cd";
    char *t = strtok(b, ",");
    CHECK_EQ_I(b[2], 0); CHECK_EQ_I(b[3], 'c'); CHECK_STR(t, "ab");
    t = strtok(NULL, ",");
    CHECK_STR(t, "cd");
}
TC_CASE(buffer_after_full_tokenisation)
{
    static const char want[] = { 'a', 0, 'b', 'b', 0, 'c', 0 };
    char b[] = "a,bb;c";
    while (strtok(b[0] == 'a' && b[1] == ',' ? b : NULL, ",;") != NULL) { }
    CHECK_MEM(b, want, 7);
}
TC_CASE(trailing_delimiter)
{
    char b[] = "x,y,";
    CHECK_STR(strtok(b, ","), "x"); CHECK_STR(strtok(NULL, ","), "y");
    CHECK_EQ_I(b[3], 0);
    CHECK(strtok(NULL, ",") == NULL);
    CHECK_EQ_I(b[4], 0);
}

int main(void) { tc_begin(); TC_RUN(pointers); TC_RUN(nul_at_delimiter); TC_RUN(buffer_after_full_tokenisation); TC_RUN(trailing_delimiter); return tc_end(); }
