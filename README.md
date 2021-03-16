# ESS Core

This is the main repository for the [Essentia one](https://essentia.one) peer-to-peer network. **ESS** - is an open-source cryptocurrency with Proof of Stake consensus mechanism and masternode functionality. ESS provide to user ability to send, spend and earn coins, and with PoS - stake coin or allocate coins in the masternodes.
ESS provide two tiers of masternodes:
  - Mercury masternode (needs to allocate 100,000 coins)
  - Mars masternode (needs to allocate 300,000 coins)

Essentia cash system is completely decentralized, consequently, no central server or trusted parties needed. Users keep the crypto keys and interact with each other directly with a peer-to-peer network to check for double-spending. Essentia one produces utilities that enable the use of ESS in everyday situations. As the Essentia technology evolves, we aim to simplify staking and masternode ownership procedures.

Additional information can be found at [essentia.one](http://www.essentia.one)

Essentia Wallet
-----------
Essentia one offers multi-platform support for wallet application. Please click on the required package to get the latest version.
- [Windows](https://essentia.one/installations/Essentia-windows-0.0.2.exe)
- [Linux](https://essentia.one/installations/Essentia-0.0.5.pkg)
- [macOS](https://essentia.one/installations/Essentia-0.0.5.pkg)
- [iOS](https://itunes.apple.com/ua/app/essentia/id1438535734?mt=8)
- [Android](https://play.google.com/store/apps/details?id=one.essentia.app)

Development
-----------

The `master` branch is regularly built and tested by Essentia developers team to ensure a stable and secure release. Here you will find materials which contain relevant information on the development and testing process.

- [Developer Notes](developer-notes.md)
- [Release Notes](release-notes.md)
- [Release Process](release-process.md)
- [Translation Process](translation_process.md)
- [Unit Tests](unit-tests.md)
- [Unauthenticated REST Interface](REST-interface.md)
- [Dnsseed Policy](dnsseed-policy.md)


>**NOTE:** For contributing in ESS create a pull request in the Github repository.

Build
-----------
**Technical Requirements:**

- Unix OS (Ubuntu 18.04 recommended).
- at least 2GB of memory (RAM).
- at least 80GB hard drive (increases over time).

Below you will find simplified step-by-step instruction on how to build ESSX on Unix based system. Please be informed, you can find additional materials which include notes on the necessary libraries, compile flags, etc. by following these links:

- [OSX Build Notes](build-osx.md)
- [Unix Build Notes](build-unix.md)
- [Gitian Building Guide](gitian-building.md)

It should be noted that ESS client builds the network backbone.  Therefore, the entire history of ESS transactions must be downloaded and stored on your computer. The synchronization process can take from a few hours to a day or more depending on the speed of your network connection. Fortunately, you have got to do that once.

Please do not forget to install required dependencies first:
```sh
apt-get install git checkinstall build-essential libtool autotools-dev automake pkg-config libssl-dev libevent-dev bsdmainutils python3 libboost-system-dev libboost-filesystem-dev libboost-chrono-dev libboost-program-options-dev libboost-test-dev libboost-thread-dev libminiupnpc-dev libzmq3-dev libqt5gui5 libqt5core5a libqt5dbus5 qttools5-dev qttools5-dev-tools libprotobuf-dev protobuf-compiler libqrencode-dev libdb-dev libdb++-dev
```
Run
-----------
As soon as the project was cloned and all of the dependencies installed, perform these actions to get things up and running.

1. Set proper path configuration:

```sh
export BDB_PREFIX="$(pwd)/contrib/db4"
```

2.  Go to *'contrib'* folder:
```sh
cd contrib/
```
3. Install *Berkeley DB* by executing the following:
```sh
sh ./install_db4.sh
```
4. Go back to the main folder:
```sh
cd ..
```
5. Execute *'autogen.sh'* to generate configuration script:
```sh
./autogen.sh
```
6. Proceed to the configuration by performing this command:
```sh
./configure --disable-tests --with-unsupported-ssl --without-gui LDFLAGS="-L${BDB_PREFIX}/lib/" CPPFLAGS="-I${BDB_PREFIX}/include/"
```
7. Build a generated package:
```sh
 make
```
8. Finally, install the software:
```sh
make install
```

Once you have everything installed, it is necessary to create  `~/.ess/ess.conf` file with the following preferences:

```
rpcuser=%username%
rpcpassword=%password%
daemon=1
```
If all went well, you should execute one last command to start node:
```
essd
```
>**NOTE:** If you are new to ESSX and having trouble dealing with node preferences try to execute `./essd --help` to get more details about available commands.

For more information, please do not hesitate to visit [Essentia Testnet: Node & Masternode Set Up Guide](https://medium.com/essentia_one/essentia-testnet-node-masternode-set-up-guide-3440bd26e6e)

>**NOTE:** GUI application is not supported yet.


Coin Specifications
-----------
<table>
<tr><td>Specification</td><td>Value</td></tr>
<tr><td>Block Time</td><td>60 Seconds</td></tr>
<tr><td>Total Supply</td><td>1.755.313.373</td></tr>
<tr><td>Premine</td><td>1.317.488.573</td></tr>
</table>


>**NOTE:** Please take into consideration, the reward decreases every two years by 200 ESS

**Coins mined**

- PoW blocks: 100 blocks
- PoS blocks: after 100 blocks

**Block Size/Height**
- 2 MB

**Coins per block  during the year**
<ul>
<li>190 - 1st year</li>
<li>167 - 2nd year</li>
<li>142 - 3rd year</li>
<li>117 - 4th year</li>
<li>92 - 5th year</li>
<li>75 - 6th year</li>
<li>50 - 7th year</li>
<li>0 - 8th year</li>
</ul>



Rewards distribution
-----------
<table>
<th colspan=4>PoS - Proof of Stake with Masternodes</th>
<tr><th>Recipients</th><th>Percentage</th></tr>
<tr><td>Staking</td><td>38% </td></tr>
<tr><td>Masternodes</td><td>45% </td></tr>
<tr><td>Network Support</td><td>17% </td></tr>
</table>

Boot addresses
-----------
Following addresses can be used in peers list.
- 95.217.57.62
- 95.216.102.220

Licence
-----------

ESS Core is released under the MIT license, please see [Opensource: MIT license](https://opensource.org/licenses/MIT) for more information.
