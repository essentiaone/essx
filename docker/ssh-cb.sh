#! /bin/bash
PRESENTDIR=$(pwd)
if [ ! -d /shared/ess ] ; then
	git clone -b "$GITBRANCH" --depth 1 "$GITURI" /shared
	if [ -d /shared/ess ] ; then
  		cd /shared/ess/
  		./autogen.sh
  		./configure --disable-zerocoin --without-gui --enable-debug
  		make
  		make install
  		mkdir /root/.ess
  		cp /root/ess.conf /root/.ess/ess.conf
  		cd $PRESENTDIR
	else
  		echo "There was a problem with the clone"
	fi
else
	echo "/shared is not empty, should you be running pbr.sh?"	
fi
