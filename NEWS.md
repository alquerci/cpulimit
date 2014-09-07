News
----

* 2.1.0 ()

  * Added `-v, --verbose` option to show control statistics

  * Added `-c, --close` option to close the specified cpulimit attached to a TARGET (implies `-z`)

* 2.0.0 ()

  * Removed usage of `cpulimit.ini` configuration file

  * All options has been changed:

    Before:

      - Usual application mode:
            Start it - by start `cpulimit.exe` after configuring `cpulimit.ini`
            Stop it - by start `cpulimit.exe` with key `/q`
            Program must be restarted after changing `cpulimit.ini` to apply new settings

      - System service mode:
            Install - extract `cpulimit.exe` and execute `cpulimit.exe /install`
            Start - net start CPULimit
            Stop - net stop CPULimit
            Uninstall - `cpulimit.exe /uninstall`
            Service must be restarted manually after changing `cpulimit.ini` to apply new settings

    After:

        Usage: cpulimit [OPTIONS...] TARGET
            OPTIONS
               -l, --limit=N   percentage of cpu allowed from 1 to 100
               -z, --lazy      exit if there is no target process, or if it dies
               -I, --idle      To run the process on low priority
               -h, --help      display this help and exit
            TARGET must be exactly one of these:
               -p, --pid=N     pid of the process (implies -z)
               -e, --exe=FILE  name of the executable program file or path name

