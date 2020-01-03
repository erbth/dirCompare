dirCompare
=======================

This is dirCompare, a multi platform directory comparison utilitiy completely
written in C++. It is designed to be easily extendable regarding the supported
comparison strategies and the supported platforms. Currently there is a Windows
(Win32 API) and a Linux version.

Both my still be bogous and I think they actually are. I think both follow
symlinks, junctions, etc. However there may be (and probably are) more bugs
than that. However I took all care I could take to open all files and
directories in read-only modes so nothing should get corrupted by running this
tool. But that's not a guarantee ;-) .

dirCompare is licensed under the Apache License, Version 2.0.

status: no release yet

Thomas Erbesdobler <t.erbesdobler@team103.com> -- Developer
