AC_INIT(
    [cpulimit],
    [2.1.0-DEV],
    [alquerci@email.com],
)
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_HEADERS([config.h])
AC_LANG([C++])
AM_INIT_AUTOMAKE([foreign -Wall -Werror subdir-objects])


# checks for programs
AC_PROG_CXX


# checks for libraries
AC_CHECK_LIB([kernel32], [main], , AC_MSG_ERROR(lib missing))


# checks for header files
AC_CHECK_HEADER([windows.h], , AC_MSG_ERROR([header missing]))
AC_CHECK_HEADER([stdlib.h], , AC_MSG_ERROR([header missing]))
AC_CHECK_HEADER([string.h], , AC_MSG_ERROR([header missing]))
AC_CHECK_HEADER([stdio.h], , AC_MSG_ERROR([header missing]))
AC_CHECK_HEADER([signal.h], , AC_MSG_ERROR([header missing]))
AC_CHECK_HEADERS([getopt.h], , AC_MSG_ERROR([header missing]))

AC_MSG_CHECKING([tlhelp32.h presence])
AC_PREPROC_IFELSE(
    [AC_LANG_PROGRAM([#include <tlhelp32.h>])],
    [AC_MSG_RESULT([yes])],
    [AC_MSG_FAILURE([Required header <tlhelp32.h>.])]
)


# checks for types
AC_CHECK_TYPE([int], , AC_MSG_ERROR([type missing]))
AC_CHECK_TYPE([unsigned int], , AC_MSG_ERROR([type missing]))
AC_CHECK_TYPE([char *], , AC_MSG_ERROR([type missing]))
AC_CHECK_TYPE([char **], , AC_MSG_ERROR([type missing]))
AC_CHECK_TYPE([bool], , AC_MSG_ERROR([type missing]))
AC_CHECK_TYPE([size_t], , AC_MSG_ERROR([type missing]))
AC_CHECK_TYPE([FILE *], , AC_MSG_ERROR([type missing]))
AC_CHECK_TYPE([BOOL], , AC_MSG_ERROR([type missing]), [
    #include <windows.h>
])
AC_CHECK_TYPE([TOKEN_PRIVILEGES], , AC_MSG_ERROR([type missing]), [
    #include <windows.h>
])
AC_CHECK_TYPE([HANDLE], , AC_MSG_ERROR([type missing]), [
    #include <windows.h>
])
AC_CHECK_TYPE([DWORD], , AC_MSG_ERROR([type missing]), [
    #include <windows.h>
])
AC_CHECK_TYPE([SIZE_T], , AC_MSG_ERROR([type missing]), [
    #include <windows.h>
])
AC_CHECK_TYPE([THREADENTRY32], , AC_MSG_ERROR([type missing]), [
    #include <windows.h>
    #include <tlhelp32.h>
])
AC_CHECK_TYPE([PROCESSENTRY32], , AC_MSG_ERROR([type missing]), [
    #include <windows.h>
    #include <tlhelp32.h>
])


# checks for structures
AC_CHECK_MEMBER([struct option.name], , AC_MSG_ERROR([struct missing]), [
    #include <getopt.h>
])
AC_CHECK_MEMBER([struct option.has_arg], , AC_MSG_ERROR([struct missing]), [
    #include <getopt.h>
])
AC_CHECK_MEMBER([struct option.flag], , AC_MSG_ERROR([struct missing]), [
    #include <getopt.h>
])
AC_CHECK_MEMBER([struct option.val], , AC_MSG_ERROR([struct missing]), [
    #include <getopt.h>
])
AC_MSG_CHECKING([for WINAPI])
AC_COMPILE_IFELSE(
    [AC_LANG_PROGRAM([
        #include <windows.h>
        typedef void (WINAPI *foo)(void);
    ])],
    [AC_MSG_RESULT([yes])],
    [AC_MSG_RESULT([no])]
    [AC_MSG_FAILURE([constant missing])]
)
AC_MSG_CHECKING([for PROCESS_SUSPEND_RESUME])
AC_COMPILE_IFELSE(
    [AC_LANG_PROGRAM([
        #include <windows.h>
        int foo = PROCESS_SUSPEND_RESUME;
    ])],
    [AC_MSG_RESULT([yes])],
    [AC_MSG_RESULT([no])]
    [AC_DEFINE([PROCESS_SUSPEND_RESUME], [0x0800], [Required to suspend or resume a process])]
)

# checks for compiler characteristics
AS_CASE(
    [AS_TR_SH(["$host"])],
    [*mingw*],
    [
        AX_CHECK_LINK_FLAG([[-static-libgcc]], [LDFLAGS="$LDFLAGS -static-libgcc"], [AC_MSG_ERROR(required -static-libgcc)])
        AX_CHECK_LINK_FLAG([[-static-libstdc++]], [LDFLAGS="$LDFLAGS -static-libstdc++"], [AC_MSG_ERROR(required -static-libstdc++)])
    ],
    []
)

# checks for library functions


# checks for system services


AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT
