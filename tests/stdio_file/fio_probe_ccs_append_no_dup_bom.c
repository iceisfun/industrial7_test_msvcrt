/* TC
 * id:    fio_probe_ccs_append_no_dup_bom
 * area:  stdio_file
 * funcs: fopen fclose fputwc
 * crt:   >=80
 * tier:  p2
 * conf:  probe
 * ref:   MS docs fopen (BOM handling for ccs); append behaviour not clearly documented
 * note:  PROBE: opening an existing BOM file with 'a, ccs=UTF-8' should not write a second BOM; opening a
 *        new file with 'a, ccs=UTF-8' should write one BOM.
 */
#include "tc.h"
#define TC_S2(x) #x
#define TC_S1(x) TC_S2(x)
#define TC_VS TC_S1(TC_CRT_VER)  /* per-CRT file names: runs may be concurrent */
#define TC_W2(x) L##x
#define TC_WID(x) TC_W2(x)
#include <stdio.h>
#include <wchar.h>

static const char NAME[] = "tc_fio_probe_ccs_append_no_dup_bom_" TC_VS ".tmp";
static int rawread(const char *n, unsigned char *b, int max)
{
    DWORD got = 0; HANDLE h = CreateFileA(n, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
    if (h == INVALID_HANDLE_VALUE) return -1;
    if (!ReadFile(h, b, (DWORD)max, &got, NULL)) got = (DWORD)-1;
    CloseHandle(h); return (int)got;
}
static void rawwrite(const char *n, const void *b, int len)
{
    DWORD put; HANDLE h = CreateFileA(n, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
    if (h == INVALID_HANDLE_VALUE) return;
    WriteFile(h, b, (DWORD)len, &put, NULL); CloseHandle(h);
}
TC_CASE(append_existing)
{
    unsigned char b[16] = {0}; FILE *f = fopen(NAME, "w, ccs=UTF-8");
    CHECK(f != NULL); if (!f) return;
    fputwc(L'A', f); fclose(f);
    f = fopen(NAME, "a, ccs=UTF-8"); CHECK(f != NULL); if (!f) { DeleteFileA(NAME); return; }
    fputwc(L'B', f); fclose(f);
    CHECK_EQ_I(rawread(NAME, b, 16), 5);
    CHECK_EQ_U(b[0], 0xEF); CHECK_EQ_U(b[1], 0xBB); CHECK_EQ_U(b[2], 0xBF);
    CHECK_EQ_U(b[3], 'A'); CHECK_EQ_U(b[4], 'B');
    DeleteFileA(NAME);
}
TC_CASE(append_new)
{
    unsigned char b[16] = {0}; FILE *f = fopen(NAME, "a, ccs=UTF-8");
    CHECK(f != NULL); if (!f) return;
    fputwc(L'C', f); fclose(f);
    CHECK_EQ_I(rawread(NAME, b, 16), 4);
    CHECK_EQ_U(b[0], 0xEF); CHECK_EQ_U(b[1], 0xBB); CHECK_EQ_U(b[2], 0xBF); CHECK_EQ_U(b[3], 'C');
    DeleteFileA(NAME);
}
int main(void) { tc_begin(); DeleteFileA(NAME); TC_RUN(append_existing); TC_RUN(append_new); return tc_end(); }
