/* TC
 * id:    gct_neg_strcmpi_default_handler_fatal
 * area:  ctype
 * funcs: _stricmp _wcsicoll _memicmp _stricoll
 * crt:   >=140
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs "Parameter validation"; default invalid-parameter handler terminates
 * note:  Children install no handler; the UCRT default handler terminates the process. Only termination is asserted
 *        (non-zero, not the survival code 77). The exact status is OS dependent (0xC0000409 fast-fail where available,
 *        0xC0000417 otherwise) and is probed in err_neg_default_handler_fatal, not here.
 */
#include "tc.h"
#include <string.h>
#include <wchar.h>

static const char * volatile nul = NULL;
static const char * volatile a = "a";
static const wchar_t * volatile wa = L"a";
static const wchar_t * volatile wnul = NULL;

static int run(const char *n, DWORD *code) { return tc_spawn_self(n, code, 20000); }

TC_CASE(strcmpi_dies)   { DWORD c = 0; CHECK(run("strcmpi", &c));   CHECK(c != 77); CHECK(c != 0); }
TC_CASE(memicmp_dies)   { DWORD c = 0; CHECK(run("memicmp", &c));   CHECK(c != 77); CHECK(c != 0); }
TC_CASE(wcsicoll_dies)  { DWORD c = 0; CHECK(run("wcsicoll", &c));  CHECK(c != 77); CHECK(c != 0); }
TC_CASE(stricoll_dies)  { DWORD c = 0; CHECK(run("stricoll", &c));  CHECK(c != 77); CHECK(c != 0); }

int main(void)
{
    const char *n;
    tc_begin();
    if ((n = tc_child_name()) != NULL) {
        if (tc_strequ(n, "strcmpi")) (void)_stricmp(nul, a);
        else if (tc_strequ(n, "memicmp")) (void)_memicmp(nul, a, 1);
        else if (tc_strequ(n, "wcsicoll")) (void)_wcsicoll(wa, wnul);
        else if (tc_strequ(n, "stricoll")) (void)_stricoll(nul, nul);
        return 77;
    }
    TC_RUN(strcmpi_dies); TC_RUN(memicmp_dies); TC_RUN(wcsicoll_dies); TC_RUN(stricoll_dies);
    return tc_end();
}
