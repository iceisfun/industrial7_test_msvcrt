/* TC
 * id:    chr_neg_fgetc_fputc_null_stream
 * area:  stdio_char
 * kind:  negative
 * funcs: fgetc fputc
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs fgetc/fputc (NULL stream: invalid parameter handler, EOF, errno EINVAL)
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static void mk(const char *p, const void *d, DWORD n)
{
    DWORD w; HANDLE h = CreateFileA(p, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h != INVALID_HANDLE_VALUE) { WriteFile(h, d, n, &w, NULL); CloseHandle(h); }
}
static DWORD slurp(const char *p, unsigned char *b, DWORD cap)
{
    DWORD r = 0; HANDLE h = CreateFileA(p, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h != INVALID_HANDLE_VALUE) { if (!ReadFile(h, b, cap, &r, NULL)) r = 0; CloseHandle(h); }
    return r;
}

static FILE *volatile nf = NULL;
TC_CASE(fgetc_null) { int r; tc_iph_count = 0; errno = 0; r = (fgetc)(nf); CHECK_EQ_I(r, EOF); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(fputc_null) { int r; tc_iph_count = 0; errno = 0; r = (fputc)('a', nf); CHECK_EQ_I(r, EOF); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(getc_putc_null) { tc_iph_count = 0; errno = 0; CHECK_EQ_I((getc)(nf), EOF); CHECK_EQ_I(errno, EINVAL); errno = 0; CHECK_EQ_I((putc)('a', nf), EOF); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 2); }
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_skip_total++; tc_write("TC SKIP " TC_TEST_ID " no iph api\n"); return tc_end(); }
    TC_RUN(fgetc_null); TC_RUN(fputc_null); TC_RUN(getc_putc_null);
    
    return tc_end();
}
