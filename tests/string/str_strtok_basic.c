/* TC
 * id:    str_strtok_basic
 * area:  string
 * funcs: strtok
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.24.5.8; MS docs strtok
 */
#include "tc.h"
#include <string.h>

TC_CASE(split_comma)
{
    char b[] = "a,bb,ccc";
    char *t = strtok(b, ",");
    CHECK_STR(t, "a"); t = strtok(NULL, ","); CHECK_STR(t, "bb"); t = strtok(NULL, ","); CHECK_STR(t, "ccc");
    CHECK(strtok(NULL, ",") == NULL);
}
TC_CASE(leading_skipped)
{
    char b[] = ",,;x,y";
    char *t = strtok(b, ",;");
    CHECK_EQ_P(t, b + 3); CHECK_STR(t, "x"); t = strtok(NULL, ",;"); CHECK_STR(t, "y");
    CHECK(strtok(NULL, ",;") == NULL);
}
TC_CASE(consecutive_collapse)
{
    char b[] = "a,,,b";
    CHECK_STR(strtok(b, ","), "a"); CHECK_STR(strtok(NULL, ","), "b"); CHECK(strtok(NULL, ",") == NULL);
}
TC_CASE(change_delims)
{
    char b[] = "a b,c;d";
    CHECK_STR(strtok(b, " "), "a");
    CHECK_STR(strtok(NULL, ","), "b");
    CHECK_STR(strtok(NULL, ";"), "c");
    CHECK_STR(strtok(NULL, ""), "d");
}
TC_CASE(pointers_into_original)
{
    char b[] = "ab cd";
    char *t1 = strtok(b, " "), *t2 = strtok(NULL, " ");
    CHECK_EQ_P(t1, b); CHECK_EQ_P(t2, b + 3);
    CHECK_EQ_I(b[2], 0);
    CHECK_STR(b, "ab");
    CHECK(strtok(NULL, " ") == NULL);
}

int main(void) { tc_begin(); TC_RUN(split_comma); TC_RUN(leading_skipped); TC_RUN(consecutive_collapse); TC_RUN(change_delims); TC_RUN(pointers_into_original); return tc_end(); }
