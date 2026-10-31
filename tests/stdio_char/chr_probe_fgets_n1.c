/* TC
 * id:    chr_probe_fgets_n1
 * area:  stdio_char
 * funcs: fgets fgetws
 * crt:   >=80
 * tier:  p1
 * conf:  probe
 * ref:   C11 7.21.7.2 / 7.29.3.2 (n==1: only the terminating null is stored)
 * note:  PROBE: with n==1 the standard says the array holds only a null and buf is returned. The MS implementation may
 *        instead return NULL for n==1 (loop never reads a char). Probes which; stream data must remain unread either way.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
static void wr(const char *n, const void *d, size_t len)
{ FILE *f = fopen(n, "wb"); if (f) { if (len) fwrite(d, 1, len, f); fclose(f); } }
static size_t rd(const char *n, unsigned char *b, size_t max)
{ FILE *f = fopen(n, "rb"); size_t r = 0; if (f) { r = fread(b, 1, max, f); fclose(f); } return r; }
#define TC_S2(x) #x
#define TC_S1(x) TC_S2(x)
#define FN "tc_chr_probe_fgets_n1_a_" TC_S1(TC_CRT_VER) ".tmp"
TC_CASE(fgets_n1)
{
    char b[4] = {'Z','Z','Z','Z'}; FILE *f; wr(FN, "abc", 3);
    f = fopen(FN, "r"); CHECK(f != NULL); if (!f) return;
    CHECK(fgets(b, 1, f) == b); CHECK_EQ_U(b[0], 0);
    CHECK_EQ_I(fgetc(f), 'a');
    fclose(f); remove(FN);
}
TC_CASE(fgetws_n1)
{
    wchar_t b[4] = {L'Z',L'Z',L'Z',L'Z'}; FILE *f; wr(FN, "abc", 3);
    f = fopen(FN, "r"); CHECK(f != NULL); if (!f) return;
    CHECK(fgetws(b, 1, f) == b); CHECK_EQ_U(b[0], 0);
    CHECK_EQ_I(fgetc(f), 'a');
    fclose(f); remove(FN);
}
TC_CASE(fgetws_n1_utf16)
{
    static const unsigned char d[] = {0xFF,0xFE,'a',0,'b',0};
    wchar_t b[4] = {L'Z',L'Z',L'Z',L'Z'}; FILE *f; wr(FN, d, sizeof d);
    f = fopen(FN, "r,ccs=UTF-16LE"); CHECK(f != NULL); if (!f) return;
    CHECK(fgetws(b, 1, f) == b); CHECK_EQ_U(b[0], 0);
    CHECK_EQ_U(fgetwc(f), L'a');
    fclose(f); remove(FN);
}
int main(void) { tc_begin(); TC_RUN(fgets_n1); TC_RUN(fgetws_n1); TC_RUN(fgetws_n1_utf16); return tc_end(); }
