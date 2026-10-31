/* TC
 * id:    scn_neg_fopen_directory_probe
 * area:  scanf
 * kind:  negative
 * funcs: fopen
 * crt:   all
 * tier:  p2
 * conf:  probe
 * ref:   MS docs fopen
 * note:  PROBE: fopen(".", "r") on a directory returns NULL (CreateFile without FILE_FLAG_BACKUP_SEMANTICS is refused);
 *        errno is probed to be EACCES in a separate case.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static const char *volatile dn = ".";
TC_CASE(returns_null) { FILE *f = fopen(dn, "r"); CHECK(f == NULL); if (f) fclose(f); }
TC_CASE(errno_eacces) { FILE *f; errno = 0; f = fopen(dn, "r"); CHECK_EQ_I(errno, EACCES); if (f) fclose(f); }
int main(void) { tc_begin(); TC_RUN(returns_null); TC_RUN(errno_eacces); return tc_end(); }
