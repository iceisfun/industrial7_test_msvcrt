/* TC
 * id:    prc_spawn_args_unquoted
 * area:  process
 * funcs: _spawnl
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _spawn: arguments with embedded spaces are not quoted automatically; the caller must add quotes
 * note:  Child returns 0 when its command line ends with "--tc-child=args a b c".
 */
#include "tc.h"
#include <process.h>

static int ends_with(const char *s, const char *suf)
{
    size_t a = 0, b = 0; while (s[a]) a++; while (suf[b]) b++;
    if (b > a) return 0;
    return tc_strequ(s + a - b, suf);
}

static int child_main(void)
{
    const char *c = GetCommandLineA(), *k = "--tc-child=args"; int i, tokens = 0, in = 0;
    for (; *c; c++) { for (i = 0; k[i] && c[i] == k[i]; i++) ; if (!k[i]) break; }
    if (!*c) return 2;
    if (!ends_with(GetCommandLineA(), "--tc-child=args a b c")) return 1;
    c += i;
    for (; *c; c++) { if (*c == ' ') in = 0; else if (!in) { in = 1; tokens++; } }
    return tokens == 3 ? 0 : 3;
}

TC_CASE(joined_unquoted)
{
    char exe[MAX_PATH]; intptr_t r;
    GetModuleFileNameA(NULL, exe, MAX_PATH);
    r = _spawnl(_P_WAIT, exe, exe, "--tc-child=args", "a", "b c", NULL);
    CHECK_EQ_I(r, 0);   /* 0: command line ends as expected and 3 tokens follow */
}

int main(void)
{
    tc_begin();
    if (tc_child_name()) return child_main();
    TC_RUN(joined_unquoted);
    return tc_end();
}
