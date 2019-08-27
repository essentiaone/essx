FROM ubuntu:latest
MAINTAINER Andrew Horbach <betuha3@gmail.com>

RUN apt-get -y update && \
	apt-get --no-install-recommends -yq install software-properties-common && \
	add-apt-repository ppa:bitcoin/bitcoin && \
	apt-get install -y curl nano wget jq net-tools checkinstall build-essential libtool autotools-dev automake pkg-config libssl-dev libevent-dev bsdmainutils python3 libboost-system-dev libboost-filesystem-dev libboost-chrono-dev libboost-program-options-dev libboost-test-dev libboost-thread-dev libminiupnpc-dev libzmq3-dev libprotobuf-dev protobuf-compiler libqrencode-dev libdb-dev libdb++-dev

RUN mkdir -p /ess ; cd /ess ; wget https://bin.essentia.one/bin/essd-linux.tar.gz ; tar xf essd-linux.tar.gz ; cp src/* . ; rm essd-linux.tar.gz ; rm -rf src ; rm -rf /var/lib/apt/lists/*

ARG RPCUSER=user
ARG RPCPASS=123
ARG RPCPORT=41313
ARG PORT=41312

ENV RPCUSER=${RPCUSER}
ENV RPCPASS=${RPCPASS}
ENV RPCPORT=${RPCPORT}
ENV PORT=${PORT}

EXPOSE ${PORT} ${RPCPORT} 8080

WORKDIR /ess

VOLUME /root/.ess

RUN echo "#!/bin/bash \n export COIN_TYPE=bitcoin \n \
	export COIN_HOST=127.0.0.1:\${RPCPORT} \n \
	export COIN_USER=\${RPCUSER} \n \
	export COIN_PASSWORD=\${RPCPASS} \n \
	if [ ! -e /root/.ess/ess.conf ]; then \n \
	echo \"rpcuser=\${RPCUSER}\\nrpcpassword=\${RPCPASS}\\nrpcallowip=172.17.0.0/8\\nrest=1\\nrpcport=\${RPCPORT}\\nport=\${PORT}\\n \" > /root/.ess/ess.conf \n \
	echo \$PARAMS| tr \" \" \"\\n\" >> /root/.ess/ess.conf \n \
	fi \n \
	/ess/essd -printtoconsole" > /entrypoint.sh && chmod +x /entrypoint.sh

CMD ["/entrypoint.sh"]
