#!/bin/bash

install_name_tool -add_rpath $1 $2 2> /dev/null

exit 0
