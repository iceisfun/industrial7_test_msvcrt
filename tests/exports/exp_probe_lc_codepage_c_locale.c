/* TC
 * id:    exp_probe_lc_codepage_c_locale
 * area:  exports
 * funcs: 
 * procs: setlocale ___lc_codepage_func
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p2
 * conf:  probe
 * ref:   internal accessor behind _getmbcp-like state
 * note:  Probing: ___lc_codepage_func() returns 0 in the "C" locale (no ANSI code page associated).
 */
#include "tc.h"

typedef unsigned int (__cdecl *cpf_t)(void);
typedef char *(__cdecl *setlocale_t)(int, const char *);

TC_CASE(codepage_func_returns_0_in_c_locale)
{
    setlocale_t sl = (setlocale_t)tc_crt_proc("setlocale");
    cpf_t f = (cpf_t)tc_crt_proc("___lc_codepage_func");
    CHECK(sl != NULL); CHECK(f != NULL);
    if (!sl || !f) return;
    CHECK(sl(0, "C") != NULL);
    CHECK_EQ_U(f(), 0);
}
int main(void) { tc_begin(); TC_RUN(codepage_func_returns_0_in_c_locale); return tc_end(); }
