#include "stdafx.h"

void Cmd::PrintUsage(FILE * stream, int exit_code)
{
    fprintf(stream, "Usage: cpulimit [OPTIONS...] TARGET\n");
    fprintf(stream, " OPTIONS\n");
    fprintf(stream, " -l, --limit=N percentage of cpu allowed from 0 to 100 (required)\n");
    fprintf(stream, " -v, --verbose show control statistics\n");
    fprintf(stream, " -z, --lazy exit if there is no target process, or if it dies\n");
    fprintf(stream, " -i, --ignore-children don't limit children processes\n");
    fprintf(stream, " -h, --help display this help and exit\n");
    fprintf(stream, " TARGET must be exactly one of these:\n");
    fprintf(stream, " -p, --pid=N pid of the process (implies -z)\n");
    fprintf(stream, " -e, --exe=FILE name of the executable program file or path name\n");
    fprintf(stream, " COMMAND [ARGS] run this command and limit it (implies -z)\n");
    fprintf(stream, "\nReport bugs to <marlonx80@hotmail.com>.\n");
    exit(exit_code);
}