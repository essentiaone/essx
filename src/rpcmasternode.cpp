// Copyright (c) 2009-2012 The Bitcoin developers
// Copyright (c) 2015-2017 The PIVX Developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "activemasternode.h"
#include "db.h"
#include "init.h"
#include "main.h"
#include "masternode-budget.h"
#include "masternode-payments.h"
#include "masternodeconfig.h"
#include "activemasternode.h"
#include "masternodeman.h"
#include "rpcserver.h"
#include "utilmoneystr.h"
#include "script/standard.h"

#include <boost/tokenizer.hpp>
#include <boost/algorithm/string.hpp>
#include <fstream>
using namespace json_spirit;

static CMasternode::Tier GetMasternodeTierFromString(std::string str)
{
    boost::algorithm::to_lower(str); // modifies str

    if(str == "mercury") {
        return CMasternode::MASTERNODE_TIER_MERCURY;
    }
    else if(str == "mars") {
        return CMasternode::MASTERNODE_TIER_MARS;
    }
 
    return CMasternode::MASTERNODE_TIER_INVALID;
}

Value debug(const Array& params, bool fHelp)
{
	Object obj;

	// obj.push_back(Pair("pubKey", pubkey.GetHash().GetHex()));

	return obj;
}

Value allocatefunds(const Array& params, bool fHelp)
{
	if (fHelp || params.size() != 3)
		throw runtime_error(
			"allocatefunds purpose alias amount ( \"pay wallet\" ( \"voting wallet\" ) )\n"
			"\nStarts escrows funds for some purpose.\n"

			"\nArguments:\n"
			"1. purpose			(string, required) Helpful identifier to recognize this allocation later.  Currently only masternode is recognized. \n"
			"2. identifier      (string, required) Helpful Identifier to recognize the specific instance of the funding type that the funding is for. \n"
			"3. amount			(mars, mercury) tier of masternode. \n"
			"      <future>     (numeric, required) amount of ess funded will also be accepted for partially funding master nodes and other purposes.\n"

			"\nResult:\n"
			"\"vin\"			(string) funding transaction id necessary for next step.\n");

    if (params[0].get_str() != "masternode")
        throw runtime_error("Surely you meant the first argument to be ""masternode"" . . . . ");
	CBitcoinAddress acctAddr = GetAccountAddress("alloc->" + params[1].get_str());
	string strAmt = params[2].get_str();

    auto nMasternodeTier = GetMasternodeTierFromString(strAmt);
    if(!CMasternode::IsTierValid(nMasternodeTier))
        throw JSONRPCError(RPC_INVALID_PARAMETER, "Invalid masternode tier");

	CWalletTx wtx;
    SendMoney(acctAddr.Get(), CMasternode::GetTierCollateralAmount(nMasternodeTier), wtx);

	Object obj;
    obj.push_back(Pair("txhash", wtx.GetHash().GetHex()));
	return obj;
}

Value fundmasternode(const Array& params, bool fHelp)
{
    if (fHelp || params.size() != 4)
		throw runtime_error(
			"fundmasternode alias amount TxID masternode ( \"pay wallet\" ( \"voting wallet\" ) )\n"
			"\nVerifies the escrowed funds for the masternode and returns the necessary info for your and its configuration files.\n"

			"\nArguments:\n"
			"1. alias			(string, required) helpful identifier to recognize this allocation later.\n"
			"2. amount			(mars, mercury) tier of masternode. \n"
			"      <future>     (numeric, required) amount of ess funded will also be accepted for partially funding master nodes and other purposes.\n"
			"3. TxID			(string, required) funding transaction id .\n"
            "4. masternode		(string, required) ip address of masternode.\n"
			"(use an empty string for the pay wallet if the same as the funding wallet and you wish to assign a different voting wallet).\n"

			"\nResult:\n"
			"\"config line\"	(string) the above details for the masternode & wallet config files & cryptographic signature proving that you authorized this.\n");

    auto alias = params[0].get_str();
    auto nMasternodeTier = GetMasternodeTierFromString(params[1].get_str());

    if(!CMasternode::IsTierValid(nMasternodeTier))
        throw JSONRPCError(RPC_INVALID_PARAMETER, "Invalid masternode tier");

	uint256 txHash = uint256(params[2].get_str());
	std::string mnAddress = params[3].get_str();

	bool found = false;
    auto nAmount = CMasternode::GetTierCollateralAmount(nMasternodeTier);
    bool outputIndex = -1;
    if(auto wtx = pwalletMain->GetWalletTx(txHash))
    {
        for(size_t i = 0; i < wtx->vout.size(); ++i)
        {
            if(wtx->vout[i].nValue == nAmount && !pwalletMain->IsSpent(txHash, i))
            {
                found = true;
                outputIndex = i;
                break;
            }
        }
    }

    if (!found) {
        throw JSONRPCError(RPC_VERIFY_ERROR, "Couldn't verify transaction");
    }

    if (!pwalletMain->IsLocked())
        pwalletMain->TopUpKeyPool();

    // Generate a new key that is added to wallet
    CBitcoinAddress address = GetAccountAddress("reserved->" + alias);

    CKeyID keyID;
    if (!address.GetKeyID(keyID))
        throw JSONRPCError(RPC_TYPE_ERROR, "Address does not refer to a key");

    CKey vchSecret;
    if (!pwalletMain->GetKey(keyID, vchSecret))
        throw JSONRPCError(RPC_WALLET_ERROR, "Private key for address " + CBitcoinAddress(keyID).ToString() + " is not known");

    auto tokens = {
        alias,
        mnAddress + ":" + std::to_string(Params().GetDefaultPort()),
        CBitcoinSecret(vchSecret).ToString(),
        txHash.ToString(),
        std::to_string(outputIndex)
    };

	Object obj;
    obj.push_back(Pair("config line", boost::algorithm::join(tokens, " ")));
	return obj;
}

Value getpoolinfo(const Array& params, bool fHelp)
{
	throw runtime_error("Obfuscation, in general, and getpoolinfo, in particular, are deprecated!");
}

Value masternode(const Array& params, bool fHelp)
{
	throw runtime_error("masternode is deprecated!  Use one of the newer, clearer commands.");
}

string nodeHelp(string indent = "")
{
	string ret = indent + "\"address\": \"address\",    (string) Masternode ESS address\n";
	ret += indent + "\"protocol\": xxxx,        (numeric) Protocol version\n";
	ret += indent + "\"netaddr\": \"xxxx\",       (string) Masternode network address\n";;
	ret += indent + "\"lastseen\": ttt,			(numeric) The time in seconds since last seen\n";
	ret += indent + "\"activetime\": ttt,		(numeric) The time in seconds masternode has been active\n";
	ret += indent + "\"lastpaid\": ttt,			(numeric) The time in seconds since masternode was last paid\n";
	return ret;
}

Value listmasternodes(const Array& params, bool fHelp)
{
    std::string strFilter = "";

    if (params.size() == 1) strFilter = params[0].get_str();

    if (fHelp || (params.size() > 1))
        throw runtime_error(
            "listmasternodes ( \"filter\" )\n"
            "\nGet a ranked list of masternodes\n"

            "\nArguments:\n"
            "1. \"filter\"    (string, optional) Filter search text. Partial match by txhash, status, or addr.\n"

            "\nResult:\n"
            "[\n"
            "  {\n"
            "    \"rank\": n,           (numeric) Masternode Rank (or 0 if not enabled)\n"
            "    \"txhash\": \"hash\",    (string) Collateral transaction hash\n"
            "    \"outidx\": n,         (numeric) Collateral transaction output index\n"
            "    \"status\": s,         (string) Status (ENABLED/EXPIRED/REMOVE/etc)\n"
            "    \"addr\": \"addr\",      (string) Masternode PIVX address\n"
            "    \"version\": v,        (numeric) Masternode protocol version\n"
            "    \"lastseen\": ttt,     (numeric) The time in seconds since epoch (Jan 1 1970 GMT) of the last seen\n"
            "    \"activetime\": ttt,   (numeric) The time in seconds since epoch (Jan 1 1970 GMT) masternode has been active\n"
            "    \"lastpaid\": ttt,     (numeric) The time in seconds since epoch (Jan 1 1970 GMT) masternode was last paid\n"
            "  }\n"
            "  ,...\n"
            "]\n"
            "\nExamples:\n" +
            HelpExampleCli("masternodelist", "") + HelpExampleRpc("masternodelist", ""));

    Array ret;
    int nHeight;
    {
        LOCK(cs_main);
        CBlockIndex* pindex = chainActive.Tip();
        if(!pindex) return 0;
        nHeight = pindex->nHeight;
    }
    for(auto &&entry : mnodeman.GetFullMasternodeVector()) {
        Object obj;
        std::string strVin = entry.vin.prevout.ToStringShort();
        std::string strTxHash = entry.vin.prevout.hash.ToString();
        uint32_t oIdx = entry.vin.prevout.n;

        CMasternode* mn = mnodeman.Find(entry.vin);

        if (mn != NULL) {
            if (strFilter != "" && strTxHash.find(strFilter) == string::npos &&
                mn->Status().find(strFilter) == string::npos &&
                CBitcoinAddress(mn->pubKeyCollateralAddress.GetID()).ToString().find(strFilter) == string::npos) continue;

            std::string strStatus = mn->Status();
            std::string strHost;
            int port;
            SplitHostPort(mn->addr.ToString(), port, strHost);
            CNetAddr node = CNetAddr(strHost, false);
            std::string strNetwork = GetNetworkName(node.GetNetwork());

            obj.push_back(Pair("network", strNetwork));
            obj.push_back(Pair("txhash", strTxHash));
            obj.push_back(Pair("outidx", (uint64_t)oIdx));
            obj.push_back(Pair("status", strStatus));
            obj.push_back(Pair("addr", CBitcoinAddress(mn->pubKeyCollateralAddress.GetID()).ToString()));
            obj.push_back(Pair("version", mn->protocolVersion));
            obj.push_back(Pair("lastseen", (int64_t)mn->lastPing.sigTime));
            obj.push_back(Pair("activetime", (int64_t)(mn->lastPing.sigTime - mn->sigTime)));
            obj.push_back(Pair("lastpaid", (int64_t)mn->GetLastPaid()));
            obj.push_back(Pair("tier", CMasternode::TierToString(static_cast<CMasternode::Tier>(mn->nTier))));

            ret.push_back(obj);
        }
    }

    return ret;
}

Value masternodeconnect(const Array& params, bool fHelp)
{
	if (fHelp || (params.size() != 1))
		throw runtime_error(
			"masternodeconnect \"address\"\n"
			"\nAttempts to connect to specified masternode address\n"

			"\nArguments:\n"
			"1. \"address\"     (string, required) IP or net address to connect to\n"

			"\nExamples:\n" +
			HelpExampleCli("masternodeconnect", "\"192.168.0.6:31488\"") + HelpExampleRpc("masternodeconnect", "\"192.168.0.6:31488\""));

	std::string strAddress = params[0].get_str();

	CService addr = CService(strAddress);

	CNode* pnode = ConnectNode((CAddress)addr, NULL, false);
	if (pnode) {
		pnode->Release();
		return Value::null;
	}
	else {
		throw runtime_error("error connecting\n");
	}
}

Value getmasternodecount (const Array& params, bool fHelp)
{
    if (fHelp || (params.size() > 0))
        throw runtime_error(
            "getmasternodecount\n"
            "\nGet masternode count values\n"

            "\nResult:\n"
            "{\n"
            "  \"total\": n,        (numeric) Total masternodes\n"
            "  \"stable\": n,       (numeric) Stable count\n"
            "  \"obfcompat\": n,    (numeric) Obfuscation Compatible\n"
            "  \"enabled\": n,      (numeric) Enabled masternodes\n"
            "  \"inqueue\": n       (numeric) Masternodes in queue\n"
            "}\n"
            "\nExamples:\n" +
            HelpExampleCli("getmasternodecount", "") + HelpExampleRpc("getmasternodecount", ""));

    Object obj;
    int nCount = 0;
    int ipv4 = 0, ipv6 = 0, onion = 0;

    if (chainActive.Tip())
        mnodeman.GetNextMasternodeInQueueForPayment(chainActive.Tip()->nHeight, true, nCount);

    mnodeman.CountNetworks(ActiveProtocol(), ipv4, ipv6, onion);

    obj.push_back(Pair("total", mnodeman.size()));
    obj.push_back(Pair("stable", mnodeman.stable_size()));
    obj.push_back(Pair("obfcompat", mnodeman.CountEnabled(ActiveProtocol())));
    obj.push_back(Pair("enabled", mnodeman.CountEnabled()));
    obj.push_back(Pair("inqueue", nCount));
    obj.push_back(Pair("ipv4", ipv4));
    obj.push_back(Pair("ipv6", ipv6));
    obj.push_back(Pair("onion", onion));

    return obj;
}

Value masternodecurrent(const Array& params, bool fHelp)
{
	throw runtime_error("masternodecurrent is deprecated!  masternode payments always rely upon votes");
}

Value startmasternode(const Array& params, bool fHelp)
{
    if (fHelp || params.size() < 1)
        throw runtime_error(
            "startmasternode alias\n"
            "\nVerifies the escrowed funds for the masternode and returns the necessary info for your and its configuration files.\n"

            "\nArguments:\n"
            "1. alias			(string, required) helpful identifier to recognize this allocation later.\n"
            "\nResult:\n"
            "\"status\"	(string) status of masternode\n");

    auto alias = params.at(0).get_str();

    Object result;
    bool fFound = false;
    for(auto &&configEntry : masternodeConfig.getEntries())
    {
        if(configEntry.getAlias() == alias)
        {
            fFound = true;
            std::string strError;
            if(CActiveMasternode::Register(configEntry.getIp(), configEntry.getPrivKey(), configEntry.getTxHash(), configEntry.getOutputIndex(), strError))
            {
                result.push_back(Pair("status", "success"));
            }
            else
            {
                result.push_back(Pair("status", "failed"));
                result.push_back(Pair("error", strError));
            }

            break;
        }
    }

    if(!fFound)
        throw JSONRPCError(RPC_INVALID_PARAMETER, "Invalid alias, couldn't find MN. Check your masternode.conf file");

    return result;
}

Value createmasternodekey(const Array& params, bool fHelp)
{
	throw runtime_error("createmasternodekey is deprecated!  A masternodekey is no longer necessary for setup.");
}

Value getmasternodeoutputs(const Array& params, bool fHelp)
{
	throw runtime_error("getmasternodeoutputs is deprecated!  It has been replaced by fundmasternode for setup.");
}

Value listmasternodeconf(const Array& params, bool fHelp)
{
	throw runtime_error("listmasternodeconf is deprecated!  It is not necessary for setup.");
}

Value getmasternodestatus (const Array& params, bool fHelp)
{
    if (fHelp || (params.size() != 0))
        throw runtime_error(
            "getmasternodestatus\n"
            "\nPrint masternode status\n"

            "\nResult:\n"
            "{\n"
            "  \"txhash\": \"xxxx\",      (string) Collateral transaction hash\n"
            "  \"outputidx\": n,        (numeric) Collateral transaction output index number\n"
            "  \"netaddr\": \"xxxx\",     (string) Masternode network address\n"
            "  \"addr\": \"xxxx\",        (string) PIVX address for masternode payments\n"
            "  \"status\": \"xxxx\",      (string) Masternode status\n"
            "  \"message\": \"xxxx\"      (string) Masternode status message\n"
            "}\n"

            "\nExamples:\n" +
            HelpExampleCli("getmasternodestatus", "") + HelpExampleRpc("getmasternodestatus", ""));

    if (!fMasterNode) throw runtime_error("This is not a masternode");

    CMasternode* pmn = mnodeman.Find(activeMasternode.vin);

    if (pmn) {
        Object mnObj;
        mnObj.push_back(Pair("txhash", activeMasternode.vin.prevout.hash.ToString()));
        mnObj.push_back(Pair("outputidx", (uint64_t)activeMasternode.vin.prevout.n));
        mnObj.push_back(Pair("netaddr", activeMasternode.service.ToString()));
        mnObj.push_back(Pair("addr", CBitcoinAddress(pmn->pubKeyCollateralAddress.GetID()).ToString()));
        mnObj.push_back(Pair("status", activeMasternode.status));
        mnObj.push_back(Pair("message", activeMasternode.GetStatus()));
        return mnObj;
    }
    throw runtime_error("Masternode not found in the list of available masternodes. Current status: "
                        + activeMasternode.GetStatus());
}

Value getmasternodewinners (const Array& params, bool fHelp)
{
    if (fHelp || params.size() > 3)
        throw runtime_error(
            "getmasternodewinners ( blocks \"filter\" )\n"
            "\nPrint the masternode winners for the last n blocks\n"

            "\nArguments:\n"
            "1. blocks      (numeric, optional) Number of previous blocks to show (default: 10)\n"
            "2. filter      (string, optional) Search filter matching MN address\n"

            "\nResult (single winner):\n"
            "[\n"
            "  {\n"
            "    \"nHeight\": n,           (numeric) block height\n"
            "    \"winner\": {\n"
            "      \"address\": \"xxxx\",    (string) PIVX MN Address\n"
            "      \"nVotes\": n,          (numeric) Number of votes for winner\n"
            "    }\n"
            "  }\n"
            "  ,...\n"
            "]\n"

            "\nResult (multiple winners):\n"
            "[\n"
            "  {\n"
            "    \"nHeight\": n,           (numeric) block height\n"
            "    \"winner\": [\n"
            "      {\n"
            "        \"address\": \"xxxx\",  (string) PIVX MN Address\n"
            "        \"nVotes\": n,        (numeric) Number of votes for winner\n"
            "      }\n"
            "      ,...\n"
            "    ]\n"
            "  }\n"
            "  ,...\n"
            "]\n"
            "\nExamples:\n" +
            HelpExampleCli("getmasternodewinners", "") + HelpExampleRpc("getmasternodewinners", ""));

    int nHeight;
    {
        LOCK(cs_main);
        CBlockIndex* pindex = chainActive.Tip();
        if(!pindex) return 0;
        nHeight = pindex->nHeight;
    }

    int nLast = 10;
    std::string strFilter = "";

    if (params.size() >= 1)
        nLast = atoi(params[0].get_str());

    if (params.size() == 2)
        strFilter = params[1].get_str();

    Array ret;

    for (int i = nHeight - nLast; i < nHeight + 20; i++) {
        Object obj;
        obj.push_back(Pair("nHeight", i));

        std::string strPayment = GetRequiredPaymentsString(i);
        if (strFilter != "" && strPayment.find(strFilter) == std::string::npos) continue;

        if (strPayment.find(',') != std::string::npos) {
            Array winner;
            boost::char_separator<char> sep(",");
            boost::tokenizer< boost::char_separator<char> > tokens(strPayment, sep);
            BOOST_FOREACH (const string& t, tokens) {
                Object addr;
                std::size_t pos = t.find(":");
                std::string strAddress = t.substr(0,pos);
                uint64_t nVotes = atoi(t.substr(pos+1));
                addr.push_back(Pair("address", strAddress));
                addr.push_back(Pair("nVotes", nVotes));
                winner.push_back(addr);
            }
            obj.push_back(Pair("winner", winner));
        } else if (strPayment.find("Unknown") == std::string::npos) {
            Object winner;
            std::size_t pos = strPayment.find(":");
            std::string strAddress = strPayment.substr(0,pos);
            uint64_t nVotes = atoi(strPayment.substr(pos+1));
            winner.push_back(Pair("address", strAddress));
            winner.push_back(Pair("nVotes", nVotes));
            obj.push_back(Pair("winner", winner));
        } else {
            Object winner;
            winner.push_back(Pair("address", strPayment));
            winner.push_back(Pair("nVotes", 0));
            obj.push_back(Pair("winner", winner));
        }

            ret.push_back(obj);
    }

    return ret;
}
Value getmasternodescores(const Array& params, bool fHelp)
{
	throw runtime_error("getmasternodescores is deprecated!  masternode payments always rely upon votes not current scores");
}
