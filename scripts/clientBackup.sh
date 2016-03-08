#!/bin/sh

BLUEPORTAILPATH=/home/DATA/BluePortail/

usage() 
{
    echo "clientBackup.sh clientId dst"
    exit -1
}

if [ $# -lt 2 ] ; then
    usage
fi

CLIENTID=$1
DST=$2
SRC=${BLUEPORTAILPATH}/clients/${CLIENTID}

if [ ! -d $SRC ] ; then
    echo "Cannot find directory $SRC for client ${CLIENTID}"
    exit -2
fi

if [ ! -d $DST ] ; then
    echo "Cannot find dst directory $DST"
    exit -3
fi

DST_FILE=$(date +%Y%m%d)_BluePortail_${CLIENTID}.tgz

tar zcf ${DST}/${DST_FILE} $SRC 

if [ $? -eq 0 ] ; then
    echo "Backup from $SRC to ${DST}/${DST_FILE} done"
    exit 0
else
    echo "Backup from $SRC to ${DST}/${DST_FILE} failed"
    exit -4
fi






