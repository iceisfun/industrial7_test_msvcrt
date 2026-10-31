/* TC
 * id:    dat_sys_nerr_errlist
 * area:  data_exports
 * funcs: __sys_nerr __sys_errlist
 * crt:   ucrtbase
 * tier:  p1
 * conf:  observed
 * ref:   MS docs _sys_errlist, _sys_nerr (deprecated; ucrt exposes them through accessor functions)
 * note:  _sys_nerr >= 43 (the classic table has 43 entries, errno 0..42); entry for ENOENT is the the usual message text.
 *        Message texts and the exact count are not documented, hence conf observed; only >= 43 is asserted.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

TC_CASE(nerr_ptr_nonnull) { CHECK(__sys_nerr() != NULL); }
TC_CASE(nerr_ge_43) { CHECK(*__sys_nerr() >= 43); }
TC_CASE(errlist_nonnull) { CHECK(__sys_errlist() != NULL); }
TC_CASE(errlist_entries_nonnull_nonempty)
{
    char **l = __sys_errlist(); int n = *__sys_nerr(), i, bad = 0;
    for (i = 0; i < n; i++) if (!l[i] || !l[i][0]) bad++;
    CHECK_EQ_I(bad, 0);
}
TC_CASE(errlist_matches_strerror_for_ENOENT) { CHECK_STR(__sys_errlist()[ENOENT], "No such file or directory"); }

int main(void)
{
    tc_begin();
    TC_RUN(nerr_ptr_nonnull); TC_RUN(nerr_ge_43); TC_RUN(errlist_nonnull);
    TC_RUN(errlist_entries_nonnull_nonempty); TC_RUN(errlist_matches_strerror_for_ENOENT);
    return tc_end();
}
