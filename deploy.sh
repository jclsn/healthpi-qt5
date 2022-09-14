#!/usr/bin/env sh

######################################################################
# @author      : Jan Claussen (claussen.j@eppendorf.de)
# @file        : deploy
# @created     : Mittwoch Sep 14, 2022 12:58:08 CEST
#
# @description :
######################################################################

ssh root@healthpi64 'killall HealthPi'; scp HealthPi root@healthpi64:/usr/bin/HealthPi


