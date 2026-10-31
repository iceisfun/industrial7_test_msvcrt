/* TC
 * id:    gst_strtoi64_l_neg_null_fatal_child
 * area:  conv
 * funcs: _strtoi64_l _strtoui64_l
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs Parameter validation (default invalid-parameter handler terminates the process); strtoll
 * note:  Child processes use the default handler; the exit code must be nonzero and not the 77 sentinel.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>
#include <locale.h>

static const char *volatile nul;
static const char *volatile ok = "123";

static void expect_dies(const char *name)
{
    DWORD code = 0; int ran = tc_spawn_self(name, &code, 20000);
    CHECK(ran); CHECK(code != 0); CHECK(code != 77); CHECK(code != 78);
}
TC_CASE(sanity) { DWORD code = 0; int ran = tc_spawn_self("sanity", &code, 20000); CHECK(ran); CHECK_EQ_U(code, 77); }
TC_CASE(null_signed) { expect_dies("null_signed"); }
TC_CASE(base99_unsigned) { expect_dies("base99_unsigned"); }
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        /* valid locale so that only parameter validation can terminate the child */
        _locale_t l = _create_locale(LC_ALL, "C");
        if (l == NULL) return 78;
        if (c[0] == 'n') (void)_strtoi64_l(nul, NULL, 10, l);
        else if (c[0] == 'b') (void)_strtoui64_l(ok, NULL, 99, l);
        return 77;
    }
    TC_RUN(sanity); TC_RUN(null_signed); TC_RUN(base99_unsigned);
    return tc_end();
}
