#!/bin/bash
#
# Manage the key 
#
# Parameters : 
# $1 : action (add, del)
# $2 : keyId
# [$3] : clientId

. /etc/BEV/scripts/BlueSystem/Logs.inc
. /etc/BEV/modules/BluePortail/Certificate.inc

function usage()
{
    echo "clientManageKey.sh action keyId clientId [startDate] [endDate]"
    echo " action : could be add, del or delClient"
    echo " keyId : the key unique id"
    echo " clientId : the client unique id"
    echo " [startDate] : certificate start date in the format YYMMDDHHMMSSZ where Z is the char Z"
    echo " [endDate] : certificate end date in the format YYMMDDHHMMSSZ where Z is the char Z"
}


Logs_add BluePortail::clientManageKey "call with $*"

if [ "$#" -lt 3 ] ; then
    usage
    Logs_add BluePortail:clientManageKey "missing arguments"
    exit -1
fi

# global variables
ACTION=$1
KEYID=$2
CLIENTID=$3

if [ ! -d $CERTIFICATE_SSLDIR ] ; then
    Logs_add BluePortail:clientManageKey "missing CA directory"
    exit -1
fi

Certificate_setClient $CLIENTID

if [ $? -ne 0 ] ; then
    Logs_add BluePortail:clientManageKey "cannot set client $CLIENTID"
    exit -1
fi

if [ $ACTION = "add" ] ; then

    Certificate_setKey $KEYID

    Certificate_createPrivateKey
    if [ $? -ne 0 ] ; then
	Logs_add BluePortail:clientManageKey "cannot create private key and request for client $CLIENTID and key $KEYID"
	exit -1
    fi

    STARTDATE=$4
    ENDDATE=$5

    Certificate_sign false $STARTDATE $ENDDATE
    if [ $? -ne 0 ] ; then
	Logs_add BluePortail:clientManageKey "cannot sign request for client $CLIENTID and key $KEYID"
	exit -1
    fi

    Certificate_createP12
    if [ $? -ne 0 ] ; then
	Logs_add BluePortail:clientManageKey "cannot create pkcs12 for client $CLIENTID and key $KEYID"
	exit -1
    fi

    Certificate_updateCRL
    if [ $? -ne 0 ] ; then
	Logs_add BluePortail:clientManageKey "cannot update revocation list"
	exit -1
    fi

    Logs_add BluePortail:clientManageKey "key $KEYID for client $CLIENTID added"

elif [ $ACTION = "del" ] ; then
    Certificate_revokeKey 
    if [ $? -ne 0 ] ; then
	Logs_add BluePortail:clientManageKey "cannot revoke key"
	exit -1
    fi

    Certificate_updateCRL
    if [ $? -ne 0 ] ; then
	Logs_add BluePortail:clientManageKey "cannot update revocation list"
	exit -1
    fi

    Logs_add BluePortail:clientManageKey "revocation for key $KEYID and client $CLIENTID done"

elif [ $ACTION = "delClient" ] ; then
    Certificate_revokeClient
    if [ $? -ne 0 ] ; then
	Logs_add BluePortail:clientManageKey "cannot revoke Client"
	exit -1
    fi

    Certificate_updateCRL
    if [ $? -ne 0 ] ; then
	Logs_add BluePortail:clientManageKey "cannot update revocation list"
	exit -1
    fi

    Logs_add BluePortail:clientManageKey "revocation for client $CLIENTID done"
else        
    Logs_add BluePortail:clientManageKey "unknown action $ACTION"
    exit -1
fi
