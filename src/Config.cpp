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
// vendor
#ifdef __CYGWIN__
#include <getopt.h>
#else
#define EXPORTS_GETOPT
#include "../vendor/ermshiperete/getopt/getoptLib/getopt.h"
#endif
#include "Config.h"

const unsigned int Config::TIME_SLOT = 1000;

Config::Config(int argc, char **argv)
{
    this->m_error = 0;
    this->m_codeExePriority = NORMAL_PRIORITY_CLASS;
    this->m_ExeName = NULL;
    this->m_isHighPriority = 1;
    this->m_isNtDll = 1;
    this->m_isLazy = 0;
    this->m_nbTimeOff = 0;
    this->m_nbTimeOn = 0;
    this->m_pid = 0;
    this->m_Close = 0;
    this->m_Verbose = 0;
    this->m_CpuUsage = 100;

    this->GetOpt(argc, argv);
}

Config::~Config(void)
{
}

void Config::PrintUsage(FILE * stream)
{
    fprintf(stream, "cpulimit, version 2.1.0-BETA3\n"); // make a globale VERSION
    fprintf(stream, "cpulimit Copyright (C) 2012 Alexandre Quercia\n");
    fprintf(stream, "This program comes with ABSOLUTELY NO WARRANTY. This is free software,\n");
    fprintf(stream, "and you are welcome to redistribute it under certain conditions.\n");
    fprintf(stream, "\n");
    fprintf(stream, "Usage: cpulimit [OPTIONS...] TARGET\n");
    fprintf(stream, "    OPTIONS\n");
    fprintf(stream, "       -l, --limit=N   percentage of cpu allowed from 1 to 100\n");
    fprintf(stream, "       -v, --verbose   show control statistics\n");
    fprintf(stream, "       -z, --lazy      exit if there is no target process, or if it dies\n");
    fprintf(stream, "       -I, --idle      To run the process on low priority\n");
    fprintf(stream, "       -c, --close     Close the specified cpulimit attached to a TARGET (implies -z)\n");
    fprintf(stream, "       -h, --help      display this help and exit\n");
    fprintf(stream, "    TARGET must be exactly one of these:\n");
    fprintf(stream, "       -p, --pid=N     pid of the process (implies -z)\n");
    fprintf(stream, "       -e, --exe=FILE  name of the executable program file or path name\n");
    fprintf(stream, "\n");
    fprintf(stream, "Report bugs to <alquerci@email.com>.\n");
}

int Config::GetError()
{
    return this->m_error;
}

void Config::GetOpt(int argc, char **argv)
{
    //argument variables
    int exe_ok = 0;
    int pid_ok = 0;
    int help_ok = 0;

    char *exe = NULL;
    int pid = 0;

    //parse arguments
    int next_option = 0;
    int option_index = 0;
    //A string listing valid short options letters
    const char *short_options = "+p:e:l:Izhcv";
    //An array describing valid long options
    const struct option long_options[] = {
		{ "pid",       required_argument,  NULL,   'p' },
        { "exe",       required_argument,  NULL,   'e' },
        { "limit",     required_argument,  NULL,   'l' },
        { "verbose",   no_argument,        NULL,   'v' },
        { "lazy",      no_argument,        NULL,   'z' },
        { "idle",      no_argument,        NULL,   'I' },
        { "close",     no_argument,        NULL,   'c' },
        { "help",      no_argument,        NULL,   'h' },
        { 0,            0,                  0,      0 }
    };

    do
    {
        next_option = getopt_long(argc, argv, short_options, long_options, &option_index);
        switch(next_option)
        {
            case 'p':
				pid = atoi(optarg);
                pid_ok = 1;
                break;
            case 'e':
                exe = optarg;
                exe_ok = 1;
                break;
            case 'l':
                this->SetLimit(atoi(optarg));
                break;
            case 'v':
                this->SetVerbose(1);
                break;
            case 'z':
                this->SetLazy(1);
                break;
            case 'I':
                this->SetCodeExePriority(IDLE_PRIORITY_CLASS);
                break;
            case 'c':
                this->SetClose(1);
                this->SetLazy(1);
                break;
            case 'h':
                Config::PrintUsage(stdout);
                help_ok = 1;
                break;
            case '?':
                help_ok = 1;
                Config::PrintUsage(stderr);
                break;
            case -1:
                break;
            default:
                break;
        }
    } while(next_option != -1);

    if(!help_ok)
    {
        if (exe_ok && pid_ok)
        {
            fprintf(stderr, "Error: You must specify exactly one target process, either by name or pid.\n");
            this->m_error += 1;
            Config::PrintUsage(stderr);
        }
        else if (exe_ok)
        {
            this->SetExeName(exe);
        }
        else if (pid_ok)
        {
            this->SetLazy(1);
            this->SetProcessId(pid);
        }
        else
        {
            fprintf(stderr, "Error: You must specify one target process, either by name or pid.\n");
            this->m_error += 1;
            Config::PrintUsage(stderr);
        }


        if (this->m_Verbose)
        {
            printf("\n | %%CPU\t| work quantum\t| sleep quantum\t|\n");
            printf("   %d%%\t  %6ld ms\t  %6ld ms \n", 
                this->m_CpuUsage, this->m_nbTimeOn, this->m_nbTimeOff);
        }
    }
    else
    {
        this->m_error += 1;
    }
}

void Config::SetExeName(char * e)
{
    this->m_ExeName = e;
}

void Config::SetProcessId(DWORD p)
{
    this->m_pid = p;
}

void Config::SetLimit(int l)
{
    if (l < 1 || l > 100)
    {
        fprintf(stderr,"Error: Limit must be in the range 1-100.\n");
        this->m_error += 1;
    }
    else
    {
        this->m_nbTimeOn = (Config::TIME_SLOT / 100) * l;
        this->m_nbTimeOff = Config::TIME_SLOT - this->GetTimeOn();
        this->m_CpuUsage = l;
    }
}

void Config::SetHighPriority(int p)
{
    this->m_isHighPriority = p;
}

void Config::SetNtDll(int n)
{
    this->m_isNtDll = n;
}

void Config::SetCodeExePriority(int p)
{
    this->m_codeExePriority = p;
}

void Config::SetLazy(int l)
{
    this->m_isLazy = l;
}

void Config::SetClose(int close)
{
    this->m_Close = close;
}

void Config::SetVerbose(int v)
{
    this->m_Verbose = v;
}


char * Config::GetExeName()
{
    return this->m_ExeName;
}

DWORD Config::GetProcessId()
{
    return this->m_pid;
}

int Config::GetTimeOn()
{
    return this->m_nbTimeOn;
}

int Config::GetTimeOff()
{
    return this->m_nbTimeOff;
}

int Config::GetHighPriority()
{
    return this->m_isHighPriority;
}

int Config::GetNtDll()
{
    return this->m_isNtDll;
}

int Config::GetCodeExePriority()
{
    return this->m_codeExePriority;
}

int Config::GetLazy()
{
    return this->m_isLazy;
}

int Config::GetClose()
{
    return this->m_Close;
}

int Config::GetVerbose()
{
    return this->m_Verbose;
}
