/* TC
 * id:    env_putenv_delete
 * area:  env
 * funcs: _putenv getenv
 * crt:   all
 * tier:  p0

 * conf:  documented
 * ref:   MS docs _putenv: delete a variable by giving an empty value, e.g. _putenv("X=")
 */
#include "tc.h"
#include <stdlib.h>

TC_CASE(delete_existing_returns_0_getenv_null)
{
    _putenv("TC_PD1=v");
    CHECK_STR(getenv("TC_PD1"), "v");
    CHECK_EQ_I(_putenv("TC_PD1="), 0);
    CHECK(getenv("TC_PD1") == NULL);
}
TC_CASE(delete_case_insensitive)
{
    _putenv("TC_PD2=v");
    CHECK_EQ_I(_putenv("tc_pd2="), 0);
    CHECK(getenv("TC_PD2") == NULL);
}
TC_CASE(delete_then_readd)
{
    _putenv("TC_PD3=a"); _putenv("TC_PD3=");
    CHECK(getenv("TC_PD3") == NULL);
    CHECK_EQ_I(_putenv("TC_PD3=b"), 0);
    CHECK_STR(getenv("TC_PD3"), "b");
}
int main(void)
{
    tc_begin();
    TC_RUN(delete_existing_returns_0_getenv_null);
    TC_RUN(delete_case_insensitive); TC_RUN(delete_then_readd);
    return tc_end();
}
