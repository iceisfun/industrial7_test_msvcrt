/* TC
 * id:    dat_argc_accessor_basic
 * area:  data_exports
 * kind:  positive
 * funcs: __p___argc __p___argv
 * procs: 
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * conf:  observed
 * ref:   MS docs __argc, __argv (the __p___* accessors are undocumented); C11 5.1.2.2.1 argv[argc]==NULL applies to main's argv, which the CRT's __argv mirrors
 * note:  Test exe is launched with no extra arguments, so only argc>=1 is asserted.
 */
#include "tc.h"

extern int *__cdecl __p___argc(void);
extern char ***__cdecl __p___argv(void);

TC_CASE(argc_ptr_nonnull_ge1) { int *p = __p___argc(); CHECK(p != NULL); if (p) CHECK(*p >= 1); }
TC_CASE(argv_ptr_nonnull) { char ***p = __p___argv(); CHECK(p != NULL); if (p) CHECK(*p != NULL); }
TC_CASE(argv0_nonempty) { char ***p = __p___argv(); if (!p || !*p) { CHECK(p && *p); return; } CHECK((*p)[0] != NULL); if ((*p)[0]) CHECK((*p)[0][0] != 0); }
TC_CASE(argv_terminated_null_at_argc)
{
    int *c = __p___argc(); char ***v = __p___argv();
    if (!c || !v || !*v) { CHECK(c && v && *v); return; }
    CHECK((*v)[*c] == NULL);
}
TC_CASE(accessor_stable_across_calls)
{
    CHECK_EQ_P(__p___argc(), __p___argc());
    CHECK_EQ_P(__p___argv(), __p___argv());
    CHECK_EQ_P(*__p___argv(), *__p___argv());
}
TC_CASE(argv_has_no_null_before_argc)
{
    int *c = __p___argc(); char ***v = __p___argv(); int i;
    if (!c || !v || !*v) { CHECK(c && v && *v); return; }
    for (i = 0; i < *c; i++) CHECK((*v)[i] != NULL);
}
int main(void)
{
    tc_begin();
    TC_RUN(argc_ptr_nonnull_ge1); TC_RUN(argv_ptr_nonnull); TC_RUN(argv0_nonempty);
    TC_RUN(argv_terminated_null_at_argc); TC_RUN(accessor_stable_across_calls); TC_RUN(argv_has_no_null_before_argc);
    return tc_end();
}
