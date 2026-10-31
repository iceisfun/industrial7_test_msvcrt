/* TC
 * id:    fio_neg_fsopen_sharing_violation
 * area:  stdio_file
 * funcs: _fsopen fopen fclose
 * crt:   all
 * tier:  p1
 * kind:  negative
 * conf:  observed
 * ref:   MS docs _fsopen (share flags); Win32 ERROR_SHARING_VIOLATION maps to EACCES in the CRT
 */
#include "tc.h"
#define TC_S2(x) #x
#define TC_S1(x) TC_S2(x)
#define TC_VS TC_S1(TC_CRT_VER)  /* per-CRT file names: runs may be concurrent */
#define TC_W2(x) L##x
#define TC_WID(x) TC_W2(x)
#include <stdio.h>
#include <errno.h>
#include <share.h>

static const char NAME[] = "tc_fio_neg_fsopen_sharing_violation_" TC_VS ".tmp";
TC_CASE(denyrw_blocks_read)
{
    FILE *a = _fsopen(NAME, "wb", _SH_DENYRW), *b;
    CHECK(a != NULL); if (!a) return;
    errno = 0; b = fopen(NAME, "r");
    CHECK(b == NULL); CHECK_EQ_I(errno, EACCES);
    if (b) fclose(b);
    fclose(a);
    b = fopen(NAME, "r"); CHECK(b != NULL);
    if (b) fclose(b);
    DeleteFileA(NAME);
}
TC_CASE(denywr_blocks_write)
{
    FILE *a = _fsopen(NAME, "wb", _SH_DENYWR), *b;
    CHECK(a != NULL); if (!a) return;
    errno = 0; b = fopen(NAME, "w");
    CHECK(b == NULL); CHECK_EQ_I(errno, EACCES);
    if (b) fclose(b);
    fclose(a);
    b = fopen(NAME, "w"); CHECK(b != NULL);
    if (b) fclose(b);
    DeleteFileA(NAME);
}
int main(void)
{
    tc_begin(); DeleteFileA(NAME);
    TC_RUN(denyrw_blocks_read); TC_RUN(denywr_blocks_write);
    return tc_end();
}
