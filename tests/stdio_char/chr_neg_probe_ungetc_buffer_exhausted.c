/* TC
 * id:    chr_neg_probe_ungetc_buffer_exhausted
 * area:  stdio_char
 * kind:  negative
 * funcs: ungetc fgetc fopen fclose
 * crt:   all
 * tier:  p1
 * conf:  probe
 * ref:   C11 7.21.7.10p3 (only one pushback guaranteed); MS ungetc ("may fail if ... used twice")
 * note:  PROBE: MS pushback writes back into the stream buffer, so it is limited by consumed bytes. After reading exactly 1 byte, the
 *        first ungetc succeeds (restores buffer slot), a second ungetc fails with EOF. Stream data afterwards is unchanged.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <wchar.h>

#define TCV_S2(x) #x
#define TCV_S(x) TCV_S2(x)
#define FN "tc_chr_neg_probe_ungetc_buffer_exhausted_" TCV_S(TC_CRT_VER) ".tmp"
static void put_file(const char *data, DWORD n)
{
    DWORD w; HANDLE h = CreateFileA(FN, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h != INVALID_HANDLE_VALUE) { if (n) WriteFile(h, data, n, &w, NULL); CloseHandle(h); }
}
/* returns size (or -1) and copies up to cap bytes into out */
static long get_file(char *out, DWORD cap)
{
    DWORD r = 0, s; HANDLE h = CreateFileA(FN, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h == INVALID_HANDLE_VALUE) return -1;
    s = GetFileSize(h, NULL);
    if (s <= cap) ReadFile(h, out, s, &r, NULL);
    CloseHandle(h);
    return (long)s;
}

TC_CASE(second_unget_fails)
{
    FILE *f; put_file("abcdef", 6);
    f = fopen(FN, "rb"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(fgetc(f), 'a');
    CHECK_EQ_I(ungetc('a', f), 'a');
    CHECK_EQ_I(ungetc('z', f), EOF);
    CHECK_EQ_I(fgetc(f), 'a');
    CHECK_EQ_I(fgetc(f), 'b');
    CHECK_EQ_I(fgetc(f), 'c');
    fclose(f);
}
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(second_unget_fails); remove(FN); return tc_end(); }
