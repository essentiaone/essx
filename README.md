ESS Core
=====================================

ESS - is a open-source cryptocurrency with Proof of Stake consensus mechanism and masternode functionality. ESSX provide to user ability to send, spend and earn coins, and with PoS - stake coin or allocate coins in the masternodes. 
ESS provide two tiers of masternodes: 
  - Mercury masternode (needs to allocate 100,000 coins)
  - Mars masternode (needs to allocate 300,000 coins).


Additional information can be found at [essentia.one](http://www.essentia.one) 

### Development
For contributing in ESS create pull request in github repository.

### Building
System requirements:
Unix OS.
at least 1GB 

install dependencies:
apt-get install git checkinstall build-essential libtool autotools-dev automake pkg-config libssl-dev libevent-dev bsdmainutils python3 libboost-system-dev libboost-filesystem-dev libboost-chrono-dev libboost-program-options-dev libboost-test-dev libboost-thread-dev libminiupnpc-dev libzmq3-dev libqt5gui5 libqt5core5a libqt5dbus5 qttools5-dev qttools5-dev-tools libprotobuf-dev protobuf-compiler libqrencode-dev libdb-dev libdb++-dev

### Run:
 - export BDB_PREFIX="$(pwd)/contrib/db4"
 - cd contrib/
 - sh ./install_db4.sh .
 - cd .. 
 - ./autogen.sh
 - ./configure --disable-tests --with-unsupported-ssl --without-gui LDFLAGS="-L${BDB_PREFIX}/lib/" CPPFLAGS="-I${BDB_PREFIX}/include/"
 - make
 - make install

GUI application is not supported.


### Coin Specs
<table>
<tr><td>Algo</td><td>Quark</td></tr>
<tr><td>Block Time</td><td>60 Seconds</td></tr>
<tr><td>Total Supply</td><td> </td></tr>
<tr><td>Currently Circulating</td><td> </td></tr>
<tr><td>Premine</td><td> </td></tr>
</table>

### 2MB Block Size/Height

<p>
<h2>60 second blocks (10,080 per week)</h2>
<ul>
  <li>12 ½ coins minted per block first 2 years</li>
  <li>10 ½ coins for 2 years</li>
  <li>8 ½ coins for 2 years</li>
  <li>6 ½ coins after that</li>
</ul>
</p>

### Rewards / Payout System

<table>
<th colspan=4>PoS - Proof of Stake with Masternodes</th>
<tr><th>Recipients</th><th>Percentage</th></tr>
<tr><td>Staking</td><td>38% *</td></tr>
<tr><td>Masternodes</td><td>45% *</td></tr>
</table>

### Licence
The MIT licence see [here](https://opensource.org/licenses/MIT).
