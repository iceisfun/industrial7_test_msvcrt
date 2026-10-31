/* TC
 * id:    exp_stdio_int64_exported
 * area:  exports
 * funcs: 
 * procs: _fseeki64 _ftelli64 _lseeki64 _filelengthi64
 * crt:   >=90
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _fseeki64, _ftelli64, _lseeki64, _filelengthi64
 * note:  msvcr80 and msvcrt are covered separately by exp_stdio_i64_msvcrt_probe (presence in those DLLs is not documented per-version).
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

static const char *const g_fseeki64[] = { "_fseeki64", NULL };
static const char *const g_ftelli64[] = { "_ftelli64", NULL };
static const char *const g_lseeki64[] = { "_lseeki64", NULL };
static const char *const g_filelengthi64[] = { "_filelengthi64", NULL };

TC_CASE(fseeki64) { present(g_fseeki64); }
TC_CASE(ftelli64) { present(g_ftelli64); }
TC_CASE(lseeki64) { present(g_lseeki64); }
TC_CASE(filelengthi64) { present(g_filelengthi64); }

int main(void)
{
    tc_begin();
    TC_RUN(fseeki64);
    TC_RUN(ftelli64);
    TC_RUN(lseeki64);
    TC_RUN(filelengthi64);
    return tc_end();
}
