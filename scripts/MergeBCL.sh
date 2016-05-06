#!/bin/bash

BACKUP_DST="/tmp/BCL.db."$(date +%Y%m%d)

usage()
{
    echo "$0 <srcBCL> <dstBCL> <YYYYMMDD>"
    echo "Replace all the counting data from <srcBCL> to <dstBCL>."
    echo "A backup is automatically done before the operation to $BACKUP_DST"
    echo -e "\t<srcBCL>\t database filename where we read the counting data"
    echo -e "\t<dstBCL>\t database filename where we write the counting data"
    echo -e "\t<YYYYMMDD>\t selected day"
    exit -1
}

merge_Databases()
{
    local src=$1
    local date=$2

    echo "-- merge data from $src for day $date"

    cat <<EOF
attach database '$src' as src;
begin transaction;
replace into FlowCounting_DAY select * from src.FlowCounting_DAY where strftime("%Y%m%d",start)="$date";
replace into ZoneCounting_DAY select * from src.ZoneCounting_DAY where strftime("%Y%m%d",start)="$date";
commit;
detach database src;
EOF
}

if [ $# -lt 3 ] ; then
    usage
fi

if [ ! -f $1 ] ; then
    echo "Error: cannot find $1"
    exit -1
fi

if [ ! -f $2 ] ; then
    echo "Error : cannot find $2"
    exit -1
fi

SRC=$1
DST=$2
DATE=$3

echo "-- backup $SRC to $BACKUP_DST"
cp -f $SRC $BACKUP_DST
[ $? -ne 0 ] && echo "Error: cannot backup $SRC !" && exit -1

merge_Databases $SRC $DATE | sqlite3 $DST



