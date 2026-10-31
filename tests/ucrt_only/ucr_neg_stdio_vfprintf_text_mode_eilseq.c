/* TC
 * id:    ucr_neg_stdio_vfprintf_text_mode_eilseq
 * area:  ucrt_only
 * kind:  negative
 * funcs: fopen fclose fflush fseek ftell remove
 * procs: __stdio_common_vfprintf
 * crt:   win10
 * tier:  p0
 * conf:  observed
 * ref:   MS docs printf type field %ls (wide-to-multibyte conversion failure -> -1, errno EILSEQ), C11 7.21.6.1
 * note:  "C" locale: L'\x0100' is not representable. Output may contain the text before the bad char at most
 *        (length <= 1). temp file tc_ucr_neg_stdio_vfprintf_text_mode_eilseq_a.tmp
 */
#include "tc.h"
#include <stdarg.h>
#include <stdio.h>
#include <errno.h>

typedef int (__cdecl *vf_t)(unsigned long long, FILE *, const char *, void *, va_list);
static vf_t p_vf;
#define TMP "tc_ucr_neg_stdio_vfprintf_text_mode_eilseq_a.tmp"
static int callf(FILE *f, const char *fmt, ...)
{ va_list ap; int r; va_start(ap, fmt); r = p_vf(0, f, fmt, NULL, ap); va_end(ap); return r; }

static const wchar_t *volatile bad = L"a\x0100";

TC_CASE(bad_wide_char)
{
    FILE *f = fopen(TMP, "w"); int r; long len;
    CHECK(f != NULL); if (!f) return;
    errno = 0;
    r = callf(f, "%ls", bad);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EILSEQ);
    fflush(f);
    fclose(f);
    f = fopen(TMP, "rb");
    CHECK(f != NULL); if (!f) { remove(TMP); return; }
    fseek(f, 0, SEEK_END); len = ftell(f);
    CHECK(len >= 0 && len <= 1);
    fclose(f); remove(TMP);
}

int main(void)
{
    tc_begin(); tc_iph_install();
    p_vf = (vf_t)tc_crt_proc("__stdio_common_vfprintf");
    if (!p_vf) { tc_write("TC FAIL " TC_TEST_ID "::__proc missing\n"); return 1; }
    TC_RUN(bad_wide_char);
    return tc_end();
}
