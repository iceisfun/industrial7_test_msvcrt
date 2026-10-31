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
 */
#include "tc.h"
static void has(const char *n, int line)
{
    if (!tc_crt_proc(n)) { tc_write("TC NOTE missing export: "); tc_write(n); tc_write("\n"); tc_fail_ints(__FILE__, line, n, "exported", 0, 1); }
}
TC_CASE(xustrcmpi) { has("_strcmpi", __LINE__); }
TC_CASE(xiscsym) { has("iscsym", __LINE__); has("iscsymf", __LINE__); }
TC_CASE(xtoascii) { has("toascii", __LINE__); }
TC_CASE(xstrcmpi) { has("strcmpi", __LINE__); }
TC_CASE(xstricoll) { has("stricoll", __LINE__); }
TC_CASE(xwcscmpi) { has("wcscmpi", __LINE__); }
TC_CASE(xwcsicoll) { has("wcsicoll", __LINE__); }
TC_CASE(xmemicmp) { has("memicmp", __LINE__); }
int main(void) { tc_begin(); TC_RUN(xustrcmpi); TC_RUN(xiscsym); TC_RUN(xtoascii); TC_RUN(xstrcmpi); TC_RUN(xstricoll); TC_RUN(xwcscmpi); TC_RUN(xwcsicoll); TC_RUN(xmemicmp); return tc_end(); }
