/* TC
 * id:    gst_strtoumax_neg_null_fatal_child
 * area:  conv
 * funcs: strtoumax _strtoimax_l _strtoumax_l
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs Parameter validation (default invalid-parameter handler terminates); strtoimax/strtoumax
 * note:  Children use the default handler; exit code must be nonzero and not the 77 sentinel.
 */
#include "tc.h"
#include <stdlib.h>
#include <locale.h>
#include <inttypes.h>
#include <stdint.h>
extern intmax_t __cdecl _strtoimax_l(const char *, char **, int, _locale_t);
extern uintmax_t __cdecl _strtoumax_l(const char *, char **, int, _locale_t);

static const char *volatile nul;
static const char *volatile ok = "1";

static void expect_dies(const char *name)
{
    DWORD code = 0; int ran = tc_spawn_self(name, &code, 20000);
    CHECK(ran); CHECK(code != 0); CHECK(code != 77); CHECK(code != 78);
}
TC_CASE(sanity) { DWORD code = 0; int ran = tc_spawn_self("sanity", &code, 20000); CHECK(ran); CHECK_EQ_U(code, 77); }
TC_CASE(strtoumax_null) { expect_dies("umax_null"); }
TC_CASE(strtoimax_l_base40) { expect_dies("imax_base40"); }
TC_CASE(strtoumax_l_null) { expect_dies("umaxl_null"); }
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) { tc_iph_default();
        _locale_t l = _create_locale(LC_ALL, "C");
        if (l == NULL) return 78;
        if (c[0] == 'u' && c[4] == '_') (void)strtoumax(nul, NULL, 10);
        else if (c[0] == 'i') (void)_strtoimax_l(ok, NULL, 40, l);
        else if (c[0] == 'u') (void)_strtoumax_l(nul, NULL, 10, l);
        return 77;
    }
    TC_RUN(sanity); TC_RUN(strtoumax_null); TC_RUN(strtoimax_l_base40); TC_RUN(strtoumax_l_null);
    return tc_end();
}
