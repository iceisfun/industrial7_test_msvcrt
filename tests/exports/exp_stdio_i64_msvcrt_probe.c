/* TC
 * id:    exp_stdio_i64_msvcrt_probe
 * area:  exports
 * funcs: 
 * procs: _fseeki64 _ftelli64 _lseeki64 _filelengthi64
 * crt:   msvcrt msvcr80
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   MS docs _fseeki64
 * note:  PROBE: whether msvcrt.dll and msvcr80.dll export the *i64 file APIs (believed yes for msvcrt on Vista+, unsure on XP; msvcr80 _fseeki64/_ftelli64 believed yes).
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

static const char *const g_a[] = { "_fseeki64", NULL };
static const char *const g_b[] = { "_ftelli64", NULL };
static const char *const g_c[] = { "_lseeki64", NULL };
static const char *const g_d[] = { "_filelengthi64", NULL };

TC_CASE(fseeki64) { present(g_a); }
TC_CASE(ftelli64) { present(g_b); }
TC_CASE(lseeki64) { present(g_c); }
TC_CASE(filelengthi64) { present(g_d); }

int main(void)
{
    tc_begin();
    TC_RUN(fseeki64); TC_RUN(ftelli64); TC_RUN(lseeki64); TC_RUN(filelengthi64);
    return tc_end();
}
