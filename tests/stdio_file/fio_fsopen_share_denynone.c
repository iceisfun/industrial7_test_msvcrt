/* TC
 * id:    fio_fsopen_share_denynone
 * area:  stdio_file
 * funcs: _fsopen fclose fwrite fread fopen fflush
 * crt:   all
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _fsopen, _wfsopen; share.h _SH_DENYNO/_SH_DENYWR
 */
#include "tc.h"
#define TC_S2(x) #x
#define TC_S1(x) TC_S2(x)
#define TC_VS TC_S1(TC_CRT_VER)  /* per-CRT file names: runs may be concurrent */
#define TC_W2(x) L##x
#define TC_WID(x) TC_W2(x)
#include <stdio.h>
#include <share.h>

static const char NAME[] = "tc_fio_fsopen_share_denynone_" TC_VS ".tmp";
TC_CASE(denyno_both_open)
{
    char b[8] = {0}; FILE *w = _fsopen(NAME, "wb", _SH_DENYNO), *r;
    CHECK(w != NULL); if (!w) return;
    CHECK_EQ_U(fwrite("hello", 1, 5, w), 5);
    CHECK_EQ_I(fflush(w), 0);
    r = fopen(NAME, "rb"); CHECK(r != NULL);
    if (r) { CHECK_EQ_U(fread(b, 1, 8, r), 5); CHECK_MEM(b, "hello", 5); fclose(r); }
    fclose(w); DeleteFileA(NAME);
}
TC_CASE(denywr_allows_reader)
{
    char b[8] = {0}; FILE *w = _fsopen(NAME, "wb", _SH_DENYWR), *r;
    CHECK(w != NULL); if (!w) return;
    CHECK_EQ_U(fwrite("abc", 1, 3, w), 3);
    CHECK_EQ_I(fflush(w), 0);
    r = fopen(NAME, "rb"); CHECK(r != NULL);
    if (r) { CHECK_EQ_U(fread(b, 1, 8, r), 3); CHECK_MEM(b, "abc", 3); fclose(r); }
    fclose(w); DeleteFileA(NAME);
}
int main(void)
{
    tc_begin(); DeleteFileA(NAME);
    TC_RUN(denyno_both_open); TC_RUN(denywr_allows_reader);
    return tc_end();
}
