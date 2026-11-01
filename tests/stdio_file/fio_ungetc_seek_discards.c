/* TC
 * id:    fio_ungetc_seek_discards
 * area:  stdio_file
 * kind:  positive
 * funcs: ungetc fseek fgetc ftell fopen fgetpos fsetpos rewind
 * crt:   all
 * tier:  p0
 * conf:  observed
 * ref:   C11 7.21.7.10 (pushed-back characters discarded by file positioning); MS docs ungetc
 * note:  deviates from C11 7.21.9.2p5 / 7.21.9.3 (TRIAGE win1093[34])?#934, class B): on ucrtbase 10.0.19045 fseek(f,0,SEEK_SET) and fsetpos after ungetc('Z') do not
 *        discard the pushed-back char (ungetc overwrote the buffered 'A'; the seek stays inside the buffer): fgetc returns 'Z' (got=90) and the next 'B'. ftell is 0 as the
 *        standard requires. rewind() does discard (case unchanged).
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static void put(const char *n, const char *d, int len)
{
    HANDLE h = CreateFileA(n, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL); DWORD w;
    if (h != INVALID_HANDLE_VALUE) { if (len) WriteFile(h, d, (DWORD)len, &w, NULL); CloseHandle(h); }
}
static int slurp(const char *n, char *b, int cap)
{
    HANDLE h = CreateFileA(n, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL); DWORD r = 0;
    if (h == INVALID_HANDLE_VALUE) return -1;
    if (!ReadFile(h, b, (DWORD)cap, &r, NULL)) r = 0;
    CloseHandle(h); return (int)r;
}
static void del(const char *n) { DeleteFileA(n); }
#define F "tc_fio_ungetc_seek_discards.tmp"

static FILE *openr(void) { put(F, "ABC", 3); return fopen(F, "rb"); }
TC_CASE(fseek_discards)
{
    FILE *f = openr(); CHECK(f != NULL); if (!f) { del(F); return; }
    CHECK_EQ_I(fgetc(f), 'A');
    CHECK_EQ_I(ungetc('Z', f), 'Z');
    CHECK_EQ_I(fseek(f, 0, SEEK_SET), 0);
    CHECK_EQ_I(ftell(f), 0);
#if TC_AT_LEAST(140)
    CHECK_EQ_I(fgetc(f), 'Z');     /* deviates from C11 7.21.9.2p5: observed ucrtbase 10.0.19045 (in-buffer seek keeps the pushed byte) */
#else
    CHECK_EQ_I(fgetc(f), 'A');
#endif
    CHECK_EQ_I(fgetc(f), 'B');
    fclose(f); del(F);
}
TC_CASE(rewind_discards)
{
    FILE *f = openr(); CHECK(f != NULL); if (!f) { del(F); return; }
    CHECK_EQ_I(fgetc(f), 'A');
    CHECK_EQ_I(ungetc('Z', f), 'Z');
    rewind(f);
    CHECK_EQ_I(fgetc(f), 'A');
    fclose(f); del(F);
}
TC_CASE(fsetpos_discards)
{
    fpos_t p; FILE *f = openr(); CHECK(f != NULL); if (!f) { del(F); return; }
    CHECK_EQ_I(fgetpos(f, &p), 0);
    CHECK_EQ_I(fgetc(f), 'A');
    CHECK_EQ_I(ungetc('Z', f), 'Z');
    CHECK_EQ_I(fsetpos(f, &p), 0);
#if TC_AT_LEAST(140)
    CHECK_EQ_I(fgetc(f), 'Z');     /* deviates from C11 7.21.9.3: observed ucrtbase 10.0.19045 */
#else
    CHECK_EQ_I(fgetc(f), 'A');
#endif
    fclose(f); del(F);
}
int main(void) { tc_begin(); TC_RUN(fseek_discards); TC_RUN(rewind_discards); TC_RUN(fsetpos_discards); return tc_end(); }
