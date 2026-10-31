/* TC
 * id:    dir_probe_mkdir_empty
 * area:  dir
 * kind:  positive
 * funcs: _mkdir _wmkdir
 * crt:   all
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _mkdir (ENOENT: path not found)
 * note:  Probes _mkdir("") on real Windows: expected -1 with errno ENOENT (CreateDirectory("") fails with ERROR_PATH_NOT_FOUND).
 *        Not documented explicitly. Kept separate from the documented negative tests.
 */
#include "tc.h"
#include <direct.h>
#include <errno.h>

static const char * volatile empty = "";
static const wchar_t * volatile wempty = L"";

TC_CASE(empty_narrow) { errno = 0; CHECK_EQ_I(_mkdir(empty), -1); CHECK_EQ_I(errno, ENOENT); }
TC_CASE(empty_wide)   { errno = 0; CHECK_EQ_I(_wmkdir(wempty), -1); CHECK_EQ_I(errno, ENOENT); }
int main(void) { tc_begin(); TC_RUN(empty_narrow); TC_RUN(empty_wide); return tc_end(); }
