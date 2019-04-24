ESS Core version 3.0.6 is now available from:

  <https://github.com/essentiaone/ess/releases>

This is a new minor-revision version release, including various bug fixes and
performance improvements, as well as updated translations.

Please report bugs using the issue tracker at github:

  <https://github.com/essentiaone/ess/issues>


How to Upgrade
==============

If you are running an older version, shut it down. Wait until it has completely shut down (which might take a few minutes for older versions), then run the installer (on Windows).


Compatibility
==============

ESS Core is extensively tested on multiple operating systems using
the Linux kernel, macOS 10.8+, and Windows Vista and later.

Microsoft ended support for Windows XP on [April 8th, 2014](https://www.microsoft.com/en-us/WindowsForBusiness/end-of-xp-support),
No attempt is made to prevent installing or running the software on Windows XP, you
can still do so at your own risk but be aware that there are known instabilities and issues.
Please do not report issues about Windows XP to the issue tracker.

ESS Core should also work on most other Unix-like systems but is not
frequently tested on them.

### :exclamation::exclamation::exclamation: MacOS 10.13 High Sierra :exclamation::exclamation::exclamation:

**Currently there are issues with the 3.0.x gitian releases on MacOS version 10.13 (High Sierra), no reports of issues on older versions of MacOS. As such, a High Sierra Only version is included below.**


Notable Changes
===============

Automated Database Corruption Repair
---------------------
There have been cases of blockchain database corruption that can occur when ESS client is not closed gracefully. The most common cases of corruption have been identified and the wallet will now automatically fix most of these corruptions. Certain corruption states are still unable to be fixed, but now provide more detailed error messages to the user as well as prompting the user to reindex their database.

More Accurate Error Messages
---------------------
Some error messages in the wallet have been too vague and done little to help developers and the support team properly identify issues. Error messages have been refined and are now more specific.

Reduction of Debug Log Spam
---------------------
Many 3rd party services have reported that their debug logs have been overloaded with messages about unknown transaction types. This log spam has been fixed.

Removal of Heavy Running Transaction Search Code
---------------------
Many areas of the block validation code use a "slow" transaction search, which searches redundantly for transactions. This "slow" search has been removed upstream in Bitcoin and is now removed in ESS. This provides a more efficient syncing process and generally better performing wallet.

Sync Fix for Block 908000
---------------------
Many wallets were having trouble getting past block 908000. This block recalculates certain aspects of the money supply and zDIV transactions, and is known to take longer to sync. Code has been added to allow block 908000 to be validated without the user needing to enter any special commands into the debug console.

Working Testnet
---------------------
Testnet is now accessible with this release of the wallet. Testnet can be accessed using the `-testnet` startup flag.

zESS Spending Fix
---------------------
zESS that were minted between block 891730 and 895400 were experiencing an error initializing the accumulator witness data correctly, causing an inability to spend those mints. This has been fixed.
