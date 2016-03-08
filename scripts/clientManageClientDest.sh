#!/bin/bash
#
# Remove the link between the client id
# and the blueportail adress stored in
# the local file clientDest.txt on the BluePortail admin server
#
# Parameters : 
# $1 : client host file
# $2 : action (add, del, list)
# $3 : clientBluePortail (STRING)
# [$4] :  blueportail adress in the form http://serveur/

. /etc/BEV/scripts/BlueSystem/Logs.inc

function usage()
{
    echo "clientManageClientDest.sh clienthostfile action clientBluePortail [blueportail adress]"
    echo " clienthostfile : the file where the links are stored"
    echo " action : could be add, del or list"
    echo " clientBluePortail : the client BluePortail unique id"
    echo " if the action is add then use this value for the blueportail server"
    echo " this should be in the form http://server/"
}

Logs_add BluePortail::clientManageClientDest "call with $*"

if [ "$#" -lt 3 ] ; then
    usage
    Logs_add BluePortail:clientManageClientDest "missing arguments"
    exit -1
fi

# global variables
CLIENTHOSTFILE=$1
ACTION=$2
CLIENTID=$3

if [ ! -f $CLIENTHOSTFILE ] ; then
    Logs_add BluePortail:clientManageClientDest "$CLIENTHOSTFILE not found"
    exit -1
fi


if [ $ACTION = "add" ] ; then
    if [ -z "$4" ] ; then
	Logs_add BluePortail:clientManageClientDest "missing blueportail server"
	exit -1
    fi
    echo $CLIENTID $4 >> $CLIENTHOSTFILE
    Logs_add BluePortail:clientManageClientDest "client $CLIENTID added"
elif [ $ACTION = "del" ] ; then    
    sed -i "/^$CLIENTID/d" $CLIENTHOSTFILE 
    Logs_add BluePortail:clientManageClientDest "client $CLIENTID removed"
elif [ $ACTION = "list" ] ; then
    grep ^$CLIENTID $CLIENTHOSTFILE
else
    Logs_add BluePortail::clientManageClientDest "unknown action $ACTION"
    exit -1
fi

