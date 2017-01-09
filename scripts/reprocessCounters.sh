#!/bin/sh

. /etc/BEV/scripts/BlueSystem/Logs.inc
. /etc/BEV/scripts/BTopLocalServer/BTopServerTask.inc

HOUR_START="00"
HOUR_END="23"

[ -n "$1" ] && HOUR_START=$1
[ -n "$2" ] && HOUR_END=$2
#
# fab: could push and process at other location
#
[ -n "$3" ] && BTOP_SERVER_DBPATH=$3

# $1 hour start HH
# $2 hour end HH
get_fresh_counters()
{
    sqlite3 $(BTopServer_getCountersDBName) <<EOF
select distinct strftime('%Y%m%d%H%M00', time) from counting where state=0 and strftime('%H', time) >= '$1' and strftime('%H', time) <= '$2' order by time asc;
EOF
}

get_flow_counting_ids()
{
    sqlite3 $(BTopServer_getProcessedDBName) <<EOF
select id from FlowCountingProcessing;
EOF
}

get_zone_counting_ids()
{
    sqlite3 $(BTopServer_getProcessedDBName) <<EOF
select id from ZoneCountingProcessing;
EOF
}

export FCPS=$(get_flow_counting_ids)
export ZCPS=$(get_zone_counting_ids)

echo "begin transaction;"
get_fresh_counters $HOUR_START $HOUR_END | (while read start ; do
			  for i in $FCPS ; do
			      echo "insert into tasks (str) values('FlowCounting MINUTE $i $start');"
			  done
			  for i in $ZCPS ; do
			      echo "insert into tasks (str) values('ZoneCounting MINUTE $i $start');"
			  done
		      done)
echo "commit;"
