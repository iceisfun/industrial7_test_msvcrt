/* TC
 * id:    exp_stdio_wide_exported
 * area:  exports
 * funcs: 
 * procs: _wfopen _wfsopen _wfreopen _wfdopen _wopen fgetws fputws fgetwc fputwc ungetwc getwchar putwchar _wremove _wrename _wperror _putws _getws _wtmpnam
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs wide stdio (stdio.h / wchar.h)
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

static const char *const g_wide_open_group[] = { "_wfopen", "_wfsopen", "_wfreopen", "_wfdopen", "_wopen", NULL };
static const char *const g_wide_io_group[] = { "fgetws", "fputws", "fgetwc", "fputwc", "ungetwc", "getwchar", "putwchar", NULL };
static const char *const g_wide_misc_group[] = { "_wremove", "_wrename", "_wperror", "_putws", "_getws", "_wtmpnam", NULL };

TC_CASE(wide_open_group) { present(g_wide_open_group); }
TC_CASE(wide_io_group) { present(g_wide_io_group); }
TC_CASE(wide_misc_group) { present(g_wide_misc_group); }

int main(void)
{
    tc_begin();
    TC_RUN(wide_open_group);
    TC_RUN(wide_io_group);
    TC_RUN(wide_misc_group);
    return tc_end();
}
