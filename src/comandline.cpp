#include "stdafx.h"

void Cmd::PrintUsage(FILE * stream, int exit_code)
{
    fprintf(stream, "Usage: cpulimit [OPTIONS...] TARGET\n");
    fprintf(stream, " OPTIONS\n");
    fprintf(stream, " -h, --help display this help and exit\n");
    exit(exit_code);
}