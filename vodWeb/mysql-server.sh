#!/bin/sh

DB_DIR=/var/db/mysql
PIDFILE=${DB_DIR}/`/bin/hostname -s`.pid

case "$1" in
	start)
		if [ -x /usr/local/bin/mysqld_safe ]; then
			/usr/bin/limits -U mysql \
			/usr/local/bin/mysqld_safe --user=mysql --datadir=${DB_DIR} --pid-file=${PIDFILE} > /dev/null &
			echo -n ' mysqld'
		fi
		;;
	stop)
		if [ -f ${PIDFILE} ]; then
			/bin/kill `cat ${PIDFILE}` > /dev/null 2>&1 && echo -n ' mysqld'
		else
			echo "mysql-server isn't running"
		fi
		;;
	*)
		echo ""
		echo "Usage: `basename $0` { start | stop }"
		echo ""
		exit 64
		;;
esac
