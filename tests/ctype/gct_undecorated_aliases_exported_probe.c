/* TC
 * id:    gct_undecorated_aliases_exported_probe
 * area:  ctype
 * funcs:
 * procs: iscsym iscsymf toascii _strcmpi strcmpi stricoll wcscmpi wcsicoll memicmp
 * crt:   all
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   MS docs list only the underscore forms for most of these; msvcrt.dll historically also exported the POSIX-style aliases
 * note:  PROBE: whether each undecorated POSIX-style alias is exported by the real DLL. Each name (iscsym/iscsymf share a case) is its own
 *        case so one absent export is visible in isolation. Which CRTs lack which names is exactly what is being measured.
 *        ucrtbase (observed Win10 22H2 19041 export table via GetProcAddress): NONE of the undecorated names is exported
 *        (_strcmpi strcmpi iscsym iscsymf toascii stricoll wcscmpi wcsicoll memicmp all absent); the real exports are
 *        _stricmp _stricoll _wcsicoll _memicmp _wcsicmp __iscsym __iscsymf __toascii. The ucrtbase build asserts absent (and the real
 *        name present); other CRTs keep the original expectation [unverified on real msvcr100-120 / msvcrt].
 */
#include "tc.h"
#if TC_AT_LEAST(140)   /* ucrtbase */
/* ucrtbase: the undecorated alias must be absent and the real (decorated) export present */
static void has2(const char *n, const char *real, int line)
{
    if (tc_crt_proc(n)) { tc_write("TC NOTE unexpected export: "); tc_write(n); tc_write("\n"); tc_fail_ints(__FILE__, line, n, "absent", 1, 0); }
    if (real && !tc_crt_proc(real)) { tc_write("TC NOTE missing export: "); tc_write(real); tc_write("\n"); tc_fail_ints(__FILE__, line, real, "exported", 0, 1); }
}
#define HAS(n, real, line) has2(n, real, line)
#else
static void has(const char *n, int line)
{
    if (!tc_crt_proc(n)) { tc_write("TC NOTE missing export: "); tc_write(n); tc_write("\n"); tc_fail_ints(__FILE__, line, n, "exported", 0, 1); }
}
#define HAS(n, real, line) has(n, line)
#endif
TC_CASE(xustrcmpi) { HAS("_strcmpi", "_stricmp", __LINE__); }
TC_CASE(xiscsym) { HAS("iscsym", "__iscsym", __LINE__); HAS("iscsymf", "__iscsymf", __LINE__); }
TC_CASE(xtoascii) { HAS("toascii", "__toascii", __LINE__); }
TC_CASE(xstrcmpi) { HAS("strcmpi", "_stricmp", __LINE__); }
TC_CASE(xstricoll) { HAS("stricoll", "_stricoll", __LINE__); }
TC_CASE(xwcscmpi) { HAS("wcscmpi", "_wcsicmp", __LINE__); }
TC_CASE(xwcsicoll) { HAS("wcsicoll", "_wcsicoll", __LINE__); }
TC_CASE(xmemicmp) { HAS("memicmp", "_memicmp", __LINE__); }
int main(void) { tc_begin(); TC_RUN(xustrcmpi); TC_RUN(xiscsym); TC_RUN(xtoascii); TC_RUN(xstrcmpi); TC_RUN(xstricoll); TC_RUN(xwcscmpi); TC_RUN(xwcsicoll); TC_RUN(xmemicmp); return tc_end(); }
