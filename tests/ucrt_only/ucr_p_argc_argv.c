/* TC
 * id:    ucr_p_argc_argv
 * area:  ucrt_only
 * funcs: __p___argc __p___argv
 * crt:   win10
 * tier:  p0
 * conf:  documented
 * ref:   MS docs __argc, __argv, __wargv; C11 5.1.2.2.1 (argv[argc] is a null pointer)
 * note:  ucrtbase exposes __argc/__argv through __p___argc/__p___argv. The child is started by tc_spawn_self as
 *        `exe --tc-child=name`, so it sees argc==2 with argv[1] starting "--tc-child=".
 */
#include "tc.h"

extern int * __cdecl __p___argc(void);
extern char *** __cdecl __p___argv(void);

static int g_argc; static char **g_argv;

TC_CASE(argc_matches) { CHECK_EQ_I(*__p___argc(), g_argc); }
TC_CASE(argv0_same)   { char **v = *__p___argv(); CHECK(v != NULL); CHECK(v[0] != NULL); CHECK_STR(v[0], g_argv[0]); }
TC_CASE(argv_terminated) { char **v = *__p___argv(); CHECK(v != NULL); CHECK(v[*__p___argc()] == NULL); }
TC_CASE(child_two_args)
{
    DWORD code = 0; CHECK(tc_spawn_self("args", &code, 20000));
    CHECK_EQ_U(code, 40);
}

int main(int argc, char **argv)
{
    const char *c;
    g_argc = argc; g_argv = argv;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        int n = *__p___argc(); char **v = *__p___argv();
        const char *k = "--tc-child=args"; int i = 0;
        if (n != 2 || !v || !v[1] || v[2] != NULL) return 41;
        while (k[i] && v[1][i] == k[i]) i++;
        if (k[i] || v[1][i]) return 42;
        return 40;
    }
    TC_RUN(argc_matches); TC_RUN(argv0_same); TC_RUN(argv_terminated); TC_RUN(child_two_args);
    return tc_end();
}
