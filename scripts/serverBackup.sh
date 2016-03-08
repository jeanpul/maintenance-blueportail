#!/bin/bash
#
# Backup the BluePortail configuration
# 
# Parameters :
# $1 : directory destination

BLUEPORTAILPATH=/home/DATA/BluePortail

usage()
{
    echo "serverBackup.sh dst"
    exit -1
}

DST=$1
SRC=${BLUEPORTAILPATH}

if [ -z "$DST" ] ; then
    usage
    exit -1
fi

if [ ! -d $SRC ] ; then
    echo "Cannot find directory $SRC for server"
    exit -2
fi

if [ ! -d $DST ] ; then
    echo "Cannot find directory $DST"
    exit -3
fi

DST_FILE=$(date +%Y%m%d)_BluePortail.tgz

tar zcf ${DST}/${DST_FILE} --exclude='clients' $SRC

if [ $? -eq 0 ] ; then
    echo "Server backup to ${DST}/${DST_FILE} done"
    exit 0
else
    echo "Server backup to ${DST}/${DST_FILE} failed"
    exit -4
fi
