/* TC
 * id:    env_embedded_special_chars
 * area:  env
 * funcs: _putenv getenv
 * crt:   all
 * tier:  p2
 * conf:  observed
 * ref:   Windows environment block conventions (e.g. ProgramFiles(x86)); MS docs _putenv
 */
#include "tc.h"
#include <stdlib.h>

TC_CASE(name_with_parens)
{
    CHECK_EQ_I(_putenv("TC_PF(x86)=C:\\Program Files (x86)"), 0);
    CHECK_STR(getenv("TC_PF(x86)"), "C:\\Program Files (x86)");
    _putenv("TC_PF(x86)=");
    CHECK(getenv("TC_PF(x86)") == NULL);
}
TC_CASE(value_semicolons)
{
    CHECK_EQ_I(_putenv("TC_SC_PATH=C:\\a;C:\\b;;D:\\c"), 0);
    CHECK_STR(getenv("TC_SC_PATH"), "C:\\a;C:\\b;;D:\\c");
    _putenv("TC_SC_PATH=");
}
TC_CASE(trailing_spaces_kept)
{
    CHECK_EQ_I(_putenv("TC_SC_SP=abc  "), 0);
    CHECK_STR(getenv("TC_SC_SP"), "abc  ");
    _putenv("TC_SC_SP=");
}
TC_CASE(value_contains_equals)
{
    CHECK_EQ_I(_putenv("TC_SC_EQ=a=b=c"), 0);
    CHECK_STR(getenv("TC_SC_EQ"), "a=b=c");
    _putenv("TC_SC_EQ=");
}
TC_CASE(leading_digit_name)
{
    CHECK_EQ_I(_putenv("1TC_SC_DIG=digit"), 0);
    CHECK_STR(getenv("1TC_SC_DIG"), "digit");
    _putenv("1TC_SC_DIG=");
}
int main(void)
{
    tc_begin();
    TC_RUN(name_with_parens); TC_RUN(value_semicolons); TC_RUN(trailing_spaces_kept);
    TC_RUN(value_contains_equals); TC_RUN(leading_digit_name);
    return tc_end();
}
