# msvcrt / ucrtbase test corpus - schema

A vendor-neutral corpus of many small C tests for the Microsoft C runtimes. Expected values come from
the C standard and Microsoft's documented behaviour, NEVER from Wine's source or Wine's output.
Wine (via `tools/tc.py run`) is only a smoke runner; a test that fails on Wine is not wrong because of that.

Primary target: Windows 10 `ucrtbase.dll` (p0). Secondary: Windows 7 era `msvcr120/110/100`, `msvcr90/80`, `msvcrt.dll`.

## Layout
    tests/<area>/<id>.c        one file = one small test (1-8 cases, <=220 lines)
    include/tc.h               harness (read it)
    tools/tc.py                lint | build | run | all    (read its docstring)
    build/<crt>/<id>.exe       outputs (never commit-edit)
`<id>` is `[a-z0-9_]+`, equals the file basename, starts with a short area prefix, and is globally unique.
`<area>` is the directory name.

## Header (first thing in the file, exact format)
    /* TC
     * id:    str_strlen_basic          == basename
     * area:  string                    == directory
     * funcs: strlen                    names that MUST appear in the PE import table of the target DLL
     * procs: strnlen                   (optional) names resolved at run time with tc_crt_proc()
     * crt:   all                       targets, space separated: names | all | win10 | win7 | >=N | <N
     * tier:  p0                        p0 = win10/ucrtbase matters most, p1 = win7 era, p2 = obscure
     * kind:  positive                  positive (default) | negative
     * conf:  standard                  standard | documented | observed | inferred | probe
     * ref:   C11 7.24.6.3; MS docs strlen
     * note:  (optional) anything surprising; continuation lines allowed
     */
- Targets and generations: ucrtbase=140, msvcr120=120, msvcr110=110, msvcr100=100, msvcr90=90, msvcr80=80, msvcrt=60.
  `>=100` = msvcr100, 110, 120, ucrtbase. `win7` = every non-ucrtbase target.
- `crt:` lists targets where the test is EXPECTED TO PASS on real Windows. If behaviour differs by version, either
  list only the targets where it holds and write a second test for the others, or branch in-source with
  `TC_AT_LEAST(110)` / `TC_BELOW(140)` / `#ifdef TC_UCRT`. Prefer splitting into separate tests when the difference is the point.
- `conf`: `standard` = ISO C says so. `documented` = Microsoft docs say so (cite the page). `observed` = you know it from
  real Windows behaviour but it is not documented. `inferred` = reasoning only. Do not write `observed` unless you are sure.
  When unsure whether Windows does X, DO NOT assert X: test only what you are sure of.
- `funcs` is enforced: the build fails the target (NOT_IMPORTED) if the DLL's import table does not list the name.
  This catches GCC folding the call (we use -fno-builtin and volatile inputs) and mingw libmingwex silently
  supplying a static copy. For ucrtbase the printf/scanf families are satisfied by `__stdio_common_v*` imports.
  If mingw's import library lacks a function that the real DLL exports, put it in `procs:` and call via
  `tc_crt_proc("name")` with a hand-written typedef (see tests/string/str_strnlen_dynamic.c). Do not claim an export
  exists in a CRT unless you are confident (MS docs "Requirements" tables list which DLLs/versions).

## Negative tests (things that SHOULD fail) - first class
`kind: negative` (default `positive`); the id must contain `_neg_` (e.g. `str_neg_strcpy_s_null`). A negative test asserts the
documented failure behaviour of bad input: error return value, `errno`, that the invalid-parameter handler ran (`tc_iph_count`),
the output buffer's state after failure, truncation/ERANGE/EDOM/EINVAL/EBADF/EILSEQ paths, NULL / zero-size / overlapping /
out-of-range / unsupported-flag arguments, wrong-mode streams, double-close, closed fds, invalid format strings, etc. It also covers
"must NOT succeed" (e.g. a function that must not write past count) and "must not be exported / must not be available" claims.
Fatal paths (default invalid-parameter handler, `abort()`, purecall, unhandled `raise`, `_exit` codes, stack-check failures) MUST run
in a child process: `main` does `if ((c = tc_child_name())) { ...trigger it...; return 77; }` and a case calls
`tc_spawn_self("name", &code, ms)`; assert on the exit code (see tests/errno_param/err_neg_default_handler_fatal.c).
Never put a fatal call in the parent. Aim for at least a third of the tests in every area to be negative.

## Real-Windows validation and `probe`
The corpus is built here and executed on real Windows machines (tools/run_on_windows.ps1, then `tc.py ingest`). Real Windows is the
ground truth, so tests are valuable even when we are not 100% sure: use `conf: probe` for an expectation derived from reasoning or
memory but not provable from the standard/docs. Rules for probes: (a) a `probe` test needs a `note:` stating exactly what is being
probed; (b) keep probe assertions in their OWN tests/cases - never mix a guess into a test whose other assertions are documented,
so one wrong guess cannot mask or contaminate known-good checks; (c) choose the most likely value; never write an assertion that is
wrong on purpose. `standard`/`documented` assertions must still be justified by a citation. Delete only assertions you believe are wrong.
Because mingw import libraries are incomplete for old CRTs (e.g. libmsvcr80.a lacks the `_s` functions the real DLL exports),
when a link fails with `undefined reference to __imp_X` for a function the real DLL does export, move X to `procs:` and call it through
`TC_DECL_PROC` / `TC_LOAD_PROC` (tc.h) instead of dropping that CRT from `crt:`.

## Rules for tests
1. Small and focused: one function (or one tight group), 1-8 `TC_CASE`s, `main` calls `tc_begin()`, `TC_RUN(...)`, `return tc_end()`.
2. No CRT-independent shortcuts: pass inputs through `volatile` or non-const statics so the compiler cannot fold the call.
3. Never use the CRT under test for reporting (tc.h reports through kernel32). Test files may call other CRT functions as
   helpers, but a helper outside the test's stated subject must be trivial (e.g. strlen) - keep tests self-contained.
4. Use `CHECK*` macros (non-fatal). Use `tc_iph_install()` + `tc_iph_count` for invalid-parameter paths (ucrt/msvcr80+ abort
   by default); guard with `TC_AT_LEAST(80)` / the `crt:` list. A crash = fail, so never trigger undefined behaviour the
   docs do not define. Do not rely on stack garbage, pointers, or uninitialised memory.
5. Floating point: use `CHECK_BITS_D/F` for exact results (zeros, inf, nan, exactly-representable); `CHECK_NEAR` with a stated
   epsilon otherwise. Never assert last-ulp transcendental results.
6. No timing, no network, no global machine state, no registry. Temp files: unique names in the current directory
   (`tc_<id>_*.tmp`), always removed. No console I/O (stdin/_getch/_cprintf). No GUI. No threads unless the area is threads.
7. Locale: tests start in the "C" locale; restore it. Do not assume any non-C locale is installed.
8. 64-bit x64 only today (mingw-w64); write code that is also valid for x86 (use `size_t`, `intptr_t`, no pointer-size assumptions).
9. Win10-specific: ucrtbase is a single version; do not branch on UCRT build numbers.

## Workflow for the author of a test
    python3 tools/tc.py lint  <id>
    python3 tools/tc.py all   <id>          # builds for each crt in the header, runs under Wine
Compile errors, NOT_IMPORTED and EXTRA_DLL are authoring bugs - fix them. A runtime FAIL on some CRT: decide from the
C standard / MS docs whether your expectation or the runtime is wrong. If your expectation is documented/standard,
KEEP the test even though Wine fails it (that is a finding). If you cannot justify the expectation from a citation,
narrow `crt:`, downgrade or delete the assertion. Never edit an expected value just to make Wine pass.
Run with `export WINEPREFIX=$HOME/test_msvcrt/.pfx WINEDEBUG=-all TC_JOBS=2`.
