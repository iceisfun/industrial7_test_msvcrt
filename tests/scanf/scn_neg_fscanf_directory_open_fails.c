/* TC
 * id:    scn_neg_fscanf_directory_open_fails
 * area:  scanf
 * kind:  negative
 * funcs: fopen fscanf
 * crt:   all
 * tier:  p2
 * conf:  documented
 * ref:   MS docs fopen (returns NULL on error; errno set), fscanf (NULL stream -> invalid parameter handler, EOF, errno EINVAL; VS2005+)
 * note:  A failed fopen result is fed to fscanf. The fscanf(NULL) case is skipped before msvcr80 (no parameter validation).
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

#define NOFILE "tc_scn_neg_does_not_exist.tmp"
static const char *volatile nm = NOFILE;

TC_CASE(fopen_enoent) { FILE *f; errno = 0; f = fopen(nm, "r"); CHECK(f == NULL); CHECK_EQ_I(errno, ENOENT); if (f) fclose(f); }
TC_CASE(fscanf_on_failed_open)
{
    FILE *f; int a = 5, r;
    if (!tc_iph_install()) TC_SKIP("no parameter validation before msvcr80");
    f = fopen(nm, "r"); CHECK(f == NULL);
    tc_iph_count = 0; errno = 0;
    r = fscanf(f, "%d", &a);
    CHECK_EQ_I(r, EOF); CHECK_EQ_I(a, 5); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
int main(void) { tc_begin(); TC_RUN(fopen_enoent); TC_RUN(fscanf_on_failed_open); return tc_end(); }
