/* TC
 * id:    exp_neg_unknown_and_miscased_names
 * area:  exports
 * funcs: 
 * procs: tc_no_such_function strlen_does_not_exist _nonexistent_crt_fn STRLEN Strlen MALLOC Malloc FREE _strlen strlen@4 _malloc@4 ?strlen@@YAXXZ
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  observed
 * ref:   Win32 GetProcAddress: names are case-sensitive, unknown names yield NULL
 * note:  Empty-name case: GetProcAddress with an empty string must return NULL (not crash). Decorated cdecl/stdcall/C++ spellings are not export names in any CRT DLL.
 */
#include "tc.h"

static void present(const char *const *n)
{
    for (; *n; n++) if (!tc_crt_proc(*n)) tc_fail_expr(__FILE__, __LINE__, *n);  /* name = not exported */
}
static void absent(const char *const *n)
{
    for (; *n; n++) if (tc_crt_proc(*n)) tc_fail_expr(__FILE__, __LINE__, *n);  /* name = unexpectedly exported */
}

static const char *const g_nonexistent[] = { "tc_no_such_function", "strlen_does_not_exist", "_nonexistent_crt_fn", NULL };
static const char *const g_wrong_case[] = { "STRLEN", "Strlen", "MALLOC", "Malloc", "FREE", NULL };
static const char *const g_empty_name[] = { "", NULL };
static const char *const g_decorated[] = { "_strlen", "strlen@4", "_malloc@4", "?strlen@@YAXXZ", NULL };

TC_CASE(nonexistent) { absent(g_nonexistent); }
TC_CASE(wrong_case) { absent(g_wrong_case); }
TC_CASE(empty_name) { absent(g_empty_name); }
TC_CASE(decorated) { absent(g_decorated); }

int main(void)
{
    tc_begin();
    TC_RUN(nonexistent);
    TC_RUN(wrong_case);
    TC_RUN(empty_name);
    TC_RUN(decorated);
    return tc_end();
}
