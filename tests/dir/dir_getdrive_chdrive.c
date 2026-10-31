/* TC
 * id:    dir_getdrive_chdrive
 * area:  dir
 * funcs: _getdrive _chdrive _getdrives
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _getdrive, _chdrive, _getdrives
 * note:  Skips when the current directory is a UNC path (no drive letter). Never changes to another drive.
 */
#include "tc.h"
#include <direct.h>

static int cwd_is_drive(char *letter)
{
    char cwd[MAX_PATH];
    if (!GetCurrentDirectoryA(MAX_PATH, cwd) || cwd[1] != ':') return 0;
    *letter = cwd[0]; return 1;
}
TC_CASE(getdrive_matches_cwd)
{
    char l; int d = _getdrive();
    if (!cwd_is_drive(&l)) TC_SKIP("cwd has no drive letter");
    CHECK(d >= 1 && d <= 26);
    if (l >= 'a') l = (char)(l - 32);
    CHECK_EQ_I(d, l - 'A' + 1);
}
TC_CASE(getdrives_bit)
{
    char l; int d = _getdrive(); unsigned long m = _getdrives();
    if (!cwd_is_drive(&l)) TC_SKIP("cwd has no drive letter");
    CHECK((m >> (d - 1)) & 1);
}
TC_CASE(chdrive_current)
{
    char l, cwd[MAX_PATH]; int d = _getdrive();
    if (!cwd_is_drive(&l)) TC_SKIP("cwd has no drive letter");
    GetCurrentDirectoryA(MAX_PATH, cwd);
    CHECK_EQ_I(_chdrive(d), 0);
    CHECK_EQ_I(_getdrive(), d);
    SetCurrentDirectoryA(cwd);
}
int main(void) { tc_begin(); TC_RUN(getdrive_matches_cwd); TC_RUN(getdrives_bit); TC_RUN(chdrive_current); return tc_end(); }
