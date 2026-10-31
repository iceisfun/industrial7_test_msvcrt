/* TC
 * id:    env_dupenv_s_basic
 * area:  env
 * funcs: _dupenv_s _putenv_s free
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0

 * conf:  documented
 * ref:   MS docs _dupenv_s, _wdupenv_s
 * note:  Buffer is freed with free() from the same CRT (imported from the target DLL).
 */
#include "tc.h"
#include <stdlib.h>
#include <string.h>

TC_CASE(found_returns_0_buffer_nonnull_len_plus_1)
{
    char *b = NULL; size_t n = 99; errno_t e;
    _putenv_s("TC_D1", "hello");
    e = _dupenv_s(&b, &n, "TC_D1");
    CHECK_EQ_I(e, 0); CHECK(b != NULL); CHECK_EQ_U(n, 6);
    if (b) CHECK_STR(b, "hello");
    free(b);
}
TC_CASE(copy_independent_of_later_putenv)
{
    char *b = NULL; size_t n = 0;
    _putenv_s("TC_D2", "first");
    CHECK_EQ_I(_dupenv_s(&b, &n, "TC_D2"), 0);
    _putenv_s("TC_D2", "second");
    if (b) CHECK_STR(b, "first");
    free(b);
}
TC_CASE(not_found_returns_0_buffer_NULL_len_0)
{
    char *b = (char *)0x1; size_t n = 99; errno_t e;
    e = _dupenv_s(&b, &n, "TC_D_NOSUCH_VAR");
    CHECK_EQ_I(e, 0); CHECK(b == NULL); CHECK_EQ_U(n, 0);
}
TC_CASE(case_insensitive)
{
    char *b = NULL; size_t n = 0;
    _putenv_s("TC_DCase", "Mixed");
    CHECK_EQ_I(_dupenv_s(&b, &n, "tc_dcASE"), 0);
    CHECK(b != NULL); CHECK_EQ_U(n, 6);
    if (b) CHECK_STR(b, "Mixed");
    free(b);
}
TC_CASE(caller_free_succeeds)
{
    char *b = NULL; size_t n = 0; int i;
    _putenv_s("TC_D3", "freeme");
    for (i = 0; i < 100; i++) {
        CHECK_EQ_I(_dupenv_s(&b, &n, "TC_D3"), 0);
        CHECK(b != NULL);
        free(b); b = NULL;
    }
    CHECK(b == NULL);
}
int main(void)
{
    tc_begin();
    TC_RUN(found_returns_0_buffer_nonnull_len_plus_1); TC_RUN(copy_independent_of_later_putenv);
    TC_RUN(not_found_returns_0_buffer_NULL_len_0); TC_RUN(case_insensitive); TC_RUN(caller_free_succeeds);
    return tc_end();
}
