#!/bin/sh

. /etc/BEV/scripts/BlueSystem/Logs.inc
. /etc/BEV/scripts/BTopLocalServer/BTopServerTask.inc

#
# fab: could push and process at other location
#
if [ -n "$1" ] ; then
    BTOP_SERVER_DBPATH=$1
fi

get_fresh_counters()
{
    sqlite3 $(BTopServer_getCountersDBName) <<EOF
select distinct strftime('%Y%m%d%H%M00', time) from counting where state=0 order by time asc;
EOF
}

echo "begin transaction;"
get_fresh_counters | (while read start ; do
			  echo "insert into tasks (str) values('ZoneCounting MINUTE 22 $start');"
			  echo "insert into tasks (str) values('ZoneCounting MINUTE 23 $start');"
			  echo "insert into tasks (str) values('ZoneCounting MINUTE 24 $start');"
			  echo "insert into tasks (str) values('FlowCounting MINUTE 35 $start');"
			  echo "insert into tasks (str) values('FlowCounting MINUTE 36 $start');"
			  echo "insert into tasks (str) values('FlowCounting MINUTE 37 $start');"
			  echo "insert into tasks (str) values('FlowCounting MINUTE 38 $start');"
		      done)
echo "commit;"
