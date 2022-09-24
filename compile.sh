#!/usr/bin/env sh

######################################################################
# @author      : Jan Claussen (claussen.j@eppendorf.de)
# @file        : compile
# @created     : Mittwoch Sep 14, 2022 12:58:08 CEST
#
# @description :
######################################################################


source /opt/poky/4.0.1/environment-setup-cortexa72-poky-linux
qmake
make -j${nproc}


