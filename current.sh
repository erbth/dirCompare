#!/bin/bash

set -e

grep --color -n make_shared\<LinuxFile\> *.cpp

grep --color -n make_shared\<LinuxDirectory\> *.cpp
