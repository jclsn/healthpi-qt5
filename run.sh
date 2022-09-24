#!/usr/bin/env sh

######################################################################
# @author      : Jan Claussen (claussen.j@eppendorf.de)
# @file        : run
# @created     : Mittwoch Sep 14, 2022 12:58:08 CEST
#
# @description :
######################################################################

ssh root@healthpi64 'killall HealthPi'; ssh root@healthpi64 'QT_QPA_PLATFORM=eglfs XDG_RUNTIME_DIR=/home/root HealthPi'


