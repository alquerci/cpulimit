/**       GNU GENERAL PUBLIC LICENSE, Version 3
 * cpulimit - CPU usage limiter for windows (cygwin)
 * Copyright (C) 2012 Alexandre Quercia <alquerci@email.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "stdafx.h"

void Cmd::PrintUsage(FILE * stream, int exit_code)
{
    fprintf(stream, "Usage: cpulimit [OPTIONS...] TARGET\n");
    fprintf(stream, "    OPTIONS\n");
    fprintf(stream, "       -l, --limit=N   percentage of cpu allowed from 1 to 100\n");
    fprintf(stream, "       -z, --lazy      exit if there is no target process, or if it dies\n");
    fprintf(stream, "       -I, --idle      To run the process on low priority\n");
    fprintf(stream, "       -h, --help      display this help and exit\n");
    fprintf(stream, "    TARGET must be exactly one of these:\n");
    fprintf(stream, "       -p, --pid=N     pid of the process (implies -z)\n");
    fprintf(stream, "       -e, --exe=FILE  name of the executable program file or path name\n");
    fprintf(stream, "\nReport bugs to <alquerci@email.com>.\n");
    ExitProcess(exit_code);
}
