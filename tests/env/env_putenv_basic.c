/* TC
 * id:    env_putenv_basic
 * area:  env
 * funcs: _putenv getenv
 * crt:   all
 * tier:  p0

 * conf:  documented
 * ref:   MS docs _putenv, _wputenv
 */
#include "tc.h"
#include <stdlib.h>

TC_CASE(add_new_returns_0)
{
    CHECK(getenv("TC_PB1") == NULL);
    CHECK_EQ_I(_putenv("TC_PB1=one"), 0);
    CHECK_STR(getenv("TC_PB1"), "one");
}
TC_CASE(replace_existing_value)
{
    _putenv("TC_PB2=old");
    CHECK_EQ_I(_putenv("TC_PB2=new"), 0);
    CHECK_STR(getenv("TC_PB2"), "new");
}
TC_CASE(value_with_equals_sign)
{
    CHECK_EQ_I(_putenv("TC_PB3=b=c"), 0);
    CHECK_STR(getenv("TC_PB3"), "b=c");
}
TC_CASE(prefix_names_distinct)
{
    _putenv("TC_PB4AB=1");
    _putenv("TC_PB4=2");
    CHECK_STR(getenv("TC_PB4"), "2");
    CHECK_STR(getenv("TC_PB4AB"), "1");
    _putenv("TC_PB4=");
    CHECK(getenv("TC_PB4") == NULL);
    CHECK_STR(getenv("TC_PB4AB"), "1");
    CHECK(getenv("TC_PB") == NULL);
}
TC_CASE(repeat_set_same_value)
{
    CHECK_EQ_I(_putenv("TC_PB5=same"), 0);
    CHECK_EQ_I(_putenv("TC_PB5=same"), 0);
    CHECK_STR(getenv("TC_PB5"), "same");
}
TC_CASE(case_insensitive_lookup)
{
    _putenv("TC_PbCase=v");
    CHECK_STR(getenv("tc_pbcase"), "v");
    CHECK_STR(getenv("TC_PBCASE"), "v");
}
TC_CASE(value_empty_after_set_nonempty)
{
    _putenv("TC_PB6=x");
    CHECK_EQ_I(_putenv("TC_PB6=xy"), 0);
    CHECK_STR(getenv("TC_PB6"), "xy");
}
int main(void)
{
    tc_begin();
    TC_RUN(add_new_returns_0); TC_RUN(replace_existing_value); TC_RUN(value_with_equals_sign); TC_RUN(prefix_names_distinct);
    TC_RUN(repeat_set_same_value); TC_RUN(case_insensitive_lookup); TC_RUN(value_empty_after_set_nonempty);
    return tc_end();
}
