// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin developers
// Copyright (c) 2014-2015 The Dash developers
// Copyright (c) 2015-2016 The Medexcoin developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"

#include "random.h"
#include "util.h"
#include "utilstrencodings.h"

#include <assert.h>

#include <boost/assign/list_of.hpp>

using namespace std;
using namespace boost::assign;

struct SeedSpec6 {
    uint8_t addr[16];
    uint16_t port;
};

#include "chainparamsseeds.h"

/**
 * Main network
 */

//! Convert the pnSeeds6 array into usable address objects.
static void convertSeed6(std::vector<CAddress> &vSeedsOut, const SeedSpec6 *data, unsigned int count)
{
    // It'll only connect to one or two seed nodes because once it connects,
    // it'll get a pile of addresses with newer timestamps.
    // Seed nodes are given a random 'last seen time' of between one and two
    // weeks ago.
    const int64_t nOneWeek = 7*24*60*60;
    for (unsigned int i = 0; i < count; i++)
    {
        struct in6_addr ip;
        memcpy(&ip, data[i].addr, sizeof(ip));
        CAddress addr(CService(ip, data[i].port));
        addr.nTime = GetTime() - GetRand(nOneWeek) - nOneWeek;
        vSeedsOut.push_back(addr);
    }
}

//   What makes a good checkpoint block?
// + Is surrounded by blocks with reasonable timestamps
//   (no blocks before with a timestamp after, none after with
//    timestamp before)
// + Contains no strange transactions
static Checkpoints::MapCheckpoints mapCheckpoints =
        boost::assign::map_list_of
	( 0, uint256("0x000004e172e5078a52c56f789dbb259dd8d194067ac6490f73b75ac67cd8473c"));
static const Checkpoints::CCheckpointData data = {
        &mapCheckpoints,
        1534076083, // * UNIX timestamp of last checkpoint block
        0,     // * total number of transactions between genesis and last checkpoint
                    //   (the tx=... number in the SetBestChain debug.log lines)
        2000        // * estimated number of transactions per day after checkpoint
    };

static Checkpoints::MapCheckpoints mapCheckpointsTestnet =
        boost::assign::map_list_of
	( 0, uint256("0x00000db51e2482c743164efdf727d7f0ec8c1c3462d39e6488013fc1fcc7e670"));
static const Checkpoints::CCheckpointData dataTestnet = {
        &mapCheckpointsTestnet,
        1534081478,
        0,
        250
    };

static Checkpoints::MapCheckpoints mapCheckpointsRegtest =
        boost::assign::map_list_of
        ( 0, uint256("0x00000a7f1d0e9fbf24cda3dbebe1984480754ad25d4d386b92d41c28a223f6e4"))
        ;
static const Checkpoints::CCheckpointData dataRegtest = {
        &mapCheckpointsRegtest,
        1534082453,
        0,
        100
    };

class CMainParams : public CChainParams {
public:
    CMainParams() {
        networkID = CBaseChainParams::MAIN;
        strNetworkID = "main";
        /** 
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 4-byte int at any alignment.
         */
        pchMessageStart[0] = 0xaf;
        pchMessageStart[1] = 0xbe;
        pchMessageStart[2] = 0xcd;
        pchMessageStart[3] = 0xdc;
        vAlertPubKey = ParseHex("045b7b748fed1020b0ac0fa385bd2debd4614d14f92a359bdfe429bec72bd9af3068a3c9bb194ecc9a915b8ce37ba52babe1d3189f6d44847cdb8e7b6e71f22168");
        nDefaultPort = 11732;
        bnProofOfWorkLimit = ~uint256(0) >> 20;  // Medexcoin starting difficulty is 1 / 2^12
        nSubsidyHalvingInterval = 210000;
        nMaxReorganizationDepth = 12;
        nEnforceBlockUpgradeMajority = 750;
        nRejectBlockOutdatedMajority = 950;
        nToCheckBlockUpgradeMajority = 1000;
        nMinerThreads = 0;
        nTargetTimespan = 1 * 60; // Medexcoin: 1 minute
        nTargetSpacing = 1 * 60; // Medexcoin: 1 minute

        /**
         * Build the genesis block. Note that the output of the genesis coinbase cannot
         * be spent as it did not originally exist in the database.
         * 
         * CBlock(hash=00000ffd590b14, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=e0028e, nTime=1390095618, nBits=1e0ffff0, nNonce=28917698, vtx=1)
         *   CTransaction(hash=e0028e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
         *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d01044c5957697265642030392f4a616e2f3230313420546865204772616e64204578706572696d656e7420476f6573204c6976653a204f76657273746f636b2e636f6d204973204e6f7720416363657074696e6720426974636f696e73)
         *     CTxOut(nValue=50.00000000, scriptPubKey=0xA9037BAC7050C479B121CF)
         *   vMerkleTree: e0028e
         */
        const char* pszTimestamp = "Medexcoin was created on Sunday the 12th of August 2018";
        CMutableTransaction txNew;
        txNew.vin.resize(1);
        txNew.vout.resize(1);
        txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
        txNew.vout[0].nValue = 50 * COIN;
        txNew.vout[0].scriptPubKey = CScript() << ParseHex("043b24f848188990e25c412a581f3b3176a161db1952ce878b69738f211626109176260aa2d5312ec6241b7216409e77ad4f01a6a4a8f626f6351ed56d3042fb53") << OP_CHECKSIG;
        genesis.vtx.push_back(txNew);
        genesis.hashPrevBlock = 0;
        genesis.hashMerkleRoot = genesis.BuildMerkleTree();
        genesis.nVersion = 1;
        genesis.nTime    = 1534076083;
        genesis.nBits    = 0x1e0ffff0;
        genesis.nNonce   = 1506511;
	
	    hashGenesisBlock = genesis.GetHash();
        assert(hashGenesisBlock == uint256("0x000004e172e5078a52c56f789dbb259dd8d194067ac6490f73b75ac67cd8473c"));
        assert(genesis.hashMerkleRoot == uint256("0xb8694a0c9f6af44d128bc102d3a11a83cf14469a57bb63cee258b3e74c2a7dd4"));

        vSeeds.push_back(CDNSSeedData("medexcoin.com", "46.166.160.7"));
        vSeeds.push_back(CDNSSeedData("medexcoin.com", "46.166.165.68"));
        vSeeds.push_back(CDNSSeedData("medexcoin.com", "188.214.128.154"));
        vSeeds.push_back(CDNSSeedData("medexcoin.com", "188.214.133.130"));

        base58Prefixes[PUBKEY_ADDRESS] = list_of(110);                    
        base58Prefixes[SCRIPT_ADDRESS] = list_of( 13);                    
        base58Prefixes[SECRET_KEY] =     list_of(212);                    
        base58Prefixes[EXT_PUBLIC_KEY] = list_of(0x02)(0x2D)(0x25)(0x33); 
        base58Prefixes[EXT_SECRET_KEY] = list_of(0x02)(0x21)(0x31)(0x2B); 
        base58Prefixes[EXT_COIN_TYPE]  = list_of(0x80000013);

        convertSeed6(vFixedSeeds, pnSeed6_main, ARRAYLEN(pnSeed6_main));

        fRequireRPCPassword = true;
        fMiningRequiresPeers = true;
        fAllowMinDifficultyBlocks = false;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fSkipProofOfWorkCheck = false;
        fTestnetToBeDeprecatedFieldRPC = false;

        nPoolMaxTransactions = 3;
        strSporkKey = "04691d79bee8df3213004dc78120dba440cde3cf755cc10a678a353a4d44bfb45094583f4aa3356fb701ddedf53b209dda89b15c81c7f6485fc07f2415e5554465";
        strMasternodePaymentsPubKey = "04691d79bee8df3213004dc78120dba440cde3cf755cc10a678a353a4d44bfb45094583f4aa3356fb701ddedf53b209dda89b15c81c7f6485fc07f2415e5554465";
        strObfuscationPoolDummyAddress = "m87q2gC9j6nNrnzCsg4aY6bHMLsT9nUhEw";
        nStartMasternodePayments = 1534076083; //Sun, 12 Aug 2018 12:14:43 GMT
    }

    const Checkpoints::CCheckpointData& Checkpoints() const 
    {
        return data;
    }
};
static CMainParams mainParams;

/**
 * Testnet (v3)
 */
class CTestNetParams : public CMainParams {
public:
    CTestNetParams() {
        networkID = CBaseChainParams::TESTNET;
        strNetworkID = "test";
        pchMessageStart[0] = 0xf9;
        pchMessageStart[1] = 0xa1;
        pchMessageStart[2] = 0xb2;
        pchMessageStart[3] = 0xe5;
        vAlertPubKey = ParseHex("0432bcdcbcb77a39fe8edc9995086646452cad3e008d91eb50f4972870f74eef73ce4c972d65734e1128d966541823a381da8e636323ad9195f7f1210b3348e36a");
        nDefaultPort = 11734;
        nEnforceBlockUpgradeMajority = 51;
        nRejectBlockOutdatedMajority = 75;
        nToCheckBlockUpgradeMajority = 100;
        nMinerThreads = 0;
        nTargetTimespan = 1 * 60; // Medexcoin: 1 minute
        nTargetSpacing = 1 * 60; // Medexcoin: 1 minute

        //! Modify the testnet genesis block so the timestamp is valid for a later start.
        genesis.nTime = 1534081478;
        genesis.nNonce = 2270444;
	
        hashGenesisBlock = genesis.GetHash();
        assert(hashGenesisBlock == uint256("0x00000db51e2482c743164efdf727d7f0ec8c1c3462d39e6488013fc1fcc7e670"));

        vFixedSeeds.clear();
        vSeeds.clear();

        base58Prefixes[PUBKEY_ADDRESS] = list_of(128);                    // Testnet medexcoin addresses start with 'x' or 'y'
        base58Prefixes[SCRIPT_ADDRESS] = list_of( 19);                    // Testnet medexcoin script addresses start with '8' or '9'
        base58Prefixes[SECRET_KEY]     = list_of(239);                    // Testnet private keys start with '9' or 'c' (Bitcoin defaults)
        base58Prefixes[EXT_PUBLIC_KEY] = list_of(0x3a)(0x80)(0x61)(0xa0); // Testnet medexcoin BIP32 pubkeys start with 'DRKV'
        base58Prefixes[EXT_SECRET_KEY] = list_of(0x3a)(0x80)(0x58)(0x37); // Testnet medexcoin BIP32 prvkeys start with 'DRKP'
        base58Prefixes[EXT_COIN_TYPE]  = list_of(0x80000001);             // Testnet medexcoin BIP44 coin type is '5' (All coin's testnet default)

        convertSeed6(vFixedSeeds, pnSeed6_test, ARRAYLEN(pnSeed6_test));

        fRequireRPCPassword = true;
        fMiningRequiresPeers = true;
        fAllowMinDifficultyBlocks = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = true;

        nPoolMaxTransactions = 2;
        strSporkKey = "04e96add31dd751a45fe86912e77f7ce8d664650cae059878457ae67717cb97ea1b7e4b19b4450901753d310346a4acbb9152f63778f89b17f38675cd21fc68e9d";
        strMasternodePaymentsPubKey = "04e96add31dd751a45fe86912e77f7ce8d664650cae059878457ae67717cb97ea1b7e4b19b4450901753d310346a4acbb9152f63778f89b17f38675cd21fc68e9d";
        strObfuscationPoolDummyAddress = "t87q2gC9j6nNrnzCsg4aY6bHMLsT9nUhEw";
        nStartMasternodePayments = 1534081478; //Sun, 12 Aug 2018 13:44:38 GMT
    }
    const Checkpoints::CCheckpointData& Checkpoints() const 
    {
        return dataTestnet;
    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CTestNetParams {
public:
    CRegTestParams() {
        networkID = CBaseChainParams::REGTEST;
        strNetworkID = "regtest";
        pchMessageStart[0] = 0xa1;
        pchMessageStart[1] = 0xb2;
        pchMessageStart[2] = 0xc3;
        pchMessageStart[3] = 0xd4;
        nSubsidyHalvingInterval = 150;
        nEnforceBlockUpgradeMajority = 750;
        nRejectBlockOutdatedMajority = 950;
        nToCheckBlockUpgradeMajority = 1000;
        nMinerThreads = 1;
        nTargetTimespan = 1 * 60; // Medexcoin: 1 minute
        nTargetSpacing = 1 * 60; // Medexcoin: 1 minute
        bnProofOfWorkLimit = ~uint256(0) >> 1;
        genesis.nTime = 1534082453;
        genesis.nBits = 0x207fffff;
        genesis.nNonce = 1741878;

        hashGenesisBlock = genesis.GetHash();
        nDefaultPort = 11736;
        assert(hashGenesisBlock == uint256("0x00000a7f1d0e9fbf24cda3dbebe1984480754ad25d4d386b92d41c28a223f6e4"));

        vFixedSeeds.clear(); //! Testnet mode doesn't have any fixed seeds.
        vSeeds.clear();  //! Testnet mode doesn't have any DNS seeds.

        fRequireRPCPassword = false;
        fMiningRequiresPeers = false;
        fAllowMinDifficultyBlocks = true;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;
        fTestnetToBeDeprecatedFieldRPC = false;
    }
    const Checkpoints::CCheckpointData& Checkpoints() const 
    {
        return dataRegtest;
    }
};
static CRegTestParams regTestParams;

/**
 * Unit test
 */
class CUnitTestParams : public CMainParams, public CModifiableParams {
public:
    CUnitTestParams() {
        networkID = CBaseChainParams::UNITTEST;
        strNetworkID = "unittest";
        nDefaultPort = 18445;
        vFixedSeeds.clear(); //! Unit test mode doesn't have any fixed seeds.
        vSeeds.clear();  //! Unit test mode doesn't have any DNS seeds.

        fRequireRPCPassword = false;
        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fAllowMinDifficultyBlocks = false;
        fMineBlocksOnDemand = true;
    }

    const Checkpoints::CCheckpointData& Checkpoints() const 
    {
        // UnitTest share the same checkpoints as MAIN
        return data;
    }

    //! Published setters to allow changing values in unit test cases
    virtual void setSubsidyHalvingInterval(int anSubsidyHalvingInterval)  { nSubsidyHalvingInterval=anSubsidyHalvingInterval; }
    virtual void setEnforceBlockUpgradeMajority(int anEnforceBlockUpgradeMajority)  { nEnforceBlockUpgradeMajority=anEnforceBlockUpgradeMajority; }
    virtual void setRejectBlockOutdatedMajority(int anRejectBlockOutdatedMajority)  { nRejectBlockOutdatedMajority=anRejectBlockOutdatedMajority; }
    virtual void setToCheckBlockUpgradeMajority(int anToCheckBlockUpgradeMajority)  { nToCheckBlockUpgradeMajority=anToCheckBlockUpgradeMajority; }
    virtual void setDefaultConsistencyChecks(bool afDefaultConsistencyChecks)  { fDefaultConsistencyChecks=afDefaultConsistencyChecks; }
    virtual void setAllowMinDifficultyBlocks(bool afAllowMinDifficultyBlocks) {  fAllowMinDifficultyBlocks=afAllowMinDifficultyBlocks; }
    virtual void setSkipProofOfWorkCheck(bool afSkipProofOfWorkCheck) { fSkipProofOfWorkCheck = afSkipProofOfWorkCheck; }
};
static CUnitTestParams unitTestParams;


static CChainParams *pCurrentParams = 0;

CModifiableParams *ModifiableParams()
{
   assert(pCurrentParams);
   assert(pCurrentParams==&unitTestParams);
   return (CModifiableParams*)&unitTestParams;
}

const CChainParams &Params() {
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams &Params(CBaseChainParams::Network network) {
    switch (network) {
        case CBaseChainParams::MAIN:
            return mainParams;
        case CBaseChainParams::TESTNET:
            return testNetParams;
        case CBaseChainParams::REGTEST:
            return regTestParams;
        case CBaseChainParams::UNITTEST:
            return unitTestParams;
        default:
            assert(false && "Unimplemented network");
            return mainParams;
    }
}

void SelectParams(CBaseChainParams::Network network) {
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}

bool SelectParamsFromCommandLine()
{
    CBaseChainParams::Network network = NetworkIdFromCommandLine();
    if (network == CBaseChainParams::MAX_NETWORK_TYPES)
        return false;

    SelectParams(network);
    return true;
}
