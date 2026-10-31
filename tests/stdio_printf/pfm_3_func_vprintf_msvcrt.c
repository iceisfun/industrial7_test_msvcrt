/* TC
 * id:    pfm_3_func_vprintf_msvcrt
 * area:  stdio_printf
 * funcs: vprintf vfprintf
 * crt:   msvcrt
 * tier:  p1
 * conf:  documented
 * ref:   C11 7.21.6.8 (vprintf returns number of characters transmitted); MS docs vprintf, vfprintf
 * note:  vprintf writes to stdout, so the cases run in children that freopen stdout to a temp file; the parent reads it back with plain
 *        file I/O. The parent's own stdout (used by tc.h through kernel32) is never touched.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>

#define FN "tc_pfm_3_vpm.tmp"

static int call_vprintf(const char *volatile f, ...) { va_list ap; int r; va_start(ap, f); r = vprintf(f, ap); va_end(ap); return r; }
static int call_vfprintf(FILE *fp, const char *volatile f, ...) { va_list ap; int r; va_start(ap, f); r = vfprintf(fp, f, ap); va_end(ap); return r; }

static int slurp(char *out, int cap)
{
    FILE *fp = fopen(FN, "rb"); int n;
    if (!fp) return -1;
    n = (int)fread(out, 1, (size_t)cap - 1, fp); out[n] = 0; fclose(fp);
    return n;
}

static int child_main(const char *c)
{
    char buf[64]; int r, n;
    if (!freopen(FN, "w", stdout)) return 10;
    if (c[0] == 'a') {
        r = call_vprintf("%d|%s", 42, "ab");
        fflush(stdout); fclose(stdout);
        n = slurp(buf, sizeof buf);
        return (r == 5 && n == 5 && buf[0] == '4' && buf[1] == '2' && buf[2] == '|' && buf[3] == 'a' && buf[4] == 'b') ? 0 : 20;
    }
    if (c[0] == 'b') {   /* many conversions */
        r = call_vprintf("%5.2f|%x|%c|%%", 3.14159, 255, 'z');
        fflush(stdout); fclose(stdout);
        n = slurp(buf, sizeof buf);
        return (r == 12 && n == 12 && buf[0] == ' ' && buf[1] == '3' && buf[6] == 'f' && buf[7] == 'f' && buf[9] == 'z' && buf[11] == '%') ? 0 : 21;
    }
    return 99;
}

TC_CASE(vprintf_int_str) { DWORD code = 99; CHECK(tc_spawn_self("a", &code, 20000)); CHECK_EQ_U(code, 0); }
TC_CASE(vprintf_mixed)   { DWORD code = 99; CHECK(tc_spawn_self("b", &code, 20000)); CHECK_EQ_U(code, 0); }
TC_CASE(vfprintf_file)
{
    char buf[64]; int r, n; FILE *fp = fopen(FN, "w");
    CHECK(fp != NULL); if (!fp) return;
    r = call_vfprintf(fp, "%d|%s", 42, "ab"); fclose(fp);
    n = slurp(buf, sizeof buf); remove(FN);
    CHECK_EQ_I(r, 5); CHECK_EQ_I(n, 5); CHECK_STR(buf, "42|ab");
}

int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) return child_main(c);
    TC_RUN(vprintf_int_str); TC_RUN(vprintf_mixed); TC_RUN(vfprintf_file);
    remove(FN);
    return tc_end();
}
