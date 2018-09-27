// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2016 The Whiff Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"
#include "consensus/merkle.h"

#include "tinyformat.h"
#include "util.h"
#include "utilstrencodings.h"
#include "arith_uint256.h"

#include <assert.h>
#include <limits>

#include "chainparamsseeds.h"

//TODO: Take these out (??? left by Ravencoin team)
extern double algoHashTotal[16];
extern int algoHashHits[16];


static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << CScriptNum(0) << 486604799 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    CBlock genesis;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(MakeTransactionRef(std::move(txNew)));
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}

/**
 * Build the genesis block. Note that the output of its generation
 * transaction cannot be spent since it did not originally exist in the
 * database.
 *
 * CBlock(hash=000000000019d6, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=4a5e1e, nTime=1231006505, nBits=1d00ffff, nNonce=2083236893, vtx=1)
 *   CTransaction(hash=4a5e1e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
 *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d0104455468652054696d65732030332f4a616e2f32303039204368616e63656c6c6f72206f6e206272696e6b206f66207365636f6e64206261696c6f757420666f722062616e6b73)
 *     CTxOut(nValue=50.00000000, scriptPubKey=0x5F1DF16B2B704C8A578D0B)
 *   vMerkleTree: 4a5e1e
 */
static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "Genesis block creatd 04/09/2018";
    const CScript genesisOutputScript = CScript() << ParseHex("04086e3c5f6f176e6b617f67875b696d6e23002a637f4c3b456b8f283d520f1bf2fe223cd7e0bfbbc361ff622ca37a951d539e7510dad5c3110b2f1afe95346dca") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}

void CChainParams::UpdateVersionBitsParameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    consensus.vDeployments[d].nStartTime = nStartTime;
    consensus.vDeployments[d].nTimeout = nTimeout;
}

void CChainParams::TurnOffSegwit() {
	consensus.nSegwitEnabled = false;
}

void CChainParams::TurnOffCSV() {
	consensus.nCSVEnabled = false;
}

void CChainParams::TurnOffBIP34() {
	consensus.nBIP34Enabled = false;
}

void CChainParams::TurnOffBIP65() {
	consensus.nBIP65Enabled = false;
}

void CChainParams::TurnOffBIP66() {
	consensus.nBIP66Enabled = false;
}

bool CChainParams::BIP34() {
	return consensus.nBIP34Enabled;
}

bool CChainParams::BIP65() {
	return consensus.nBIP34Enabled;
}

bool CChainParams::BIP66() {
	return consensus.nBIP34Enabled;
}

bool CChainParams::CSVEnabled() const{
	return consensus.nCSVEnabled;
}


/**
 * Main network
 */
/**
 * What makes a good checkpoint block?
 * + Is surrounded by blocks with reasonable timestamps
 *   (no blocks before with a timestamp after, none after with
 *    timestamp before)
 * + Contains no strange transactions
 */

class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = "main";
        consensus.nSubsidyHalvingInterval = 2100000;  //~ 4 yrs at 1 min block time
        consensus.nBIP34Enabled = true;
        consensus.nBIP65Enabled = true; // 000000000000000004c2b624ed5d7756c508d90fd0da2c7c679febfa6c4735f0
        consensus.nBIP66Enabled = true;
        consensus.nSegwitEnabled = true;
        consensus.nCSVEnabled = true;
        consensus.powLimit = uint256S("00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");

        // original difficulty method, every 2016 blocks
        consensus.nPowTargetTimespan = 2016 * 60; // 1.4 days
        
        // second difficulty method, every 360 blocks
        consensus.nPowDifficultyRetargetHeight = 40000; // blockheight to switch to 360 block retarget rules
        consensus.nPowTargetTimespanShort = 360 * 60; //~6 hours
        
        // final difficulty method, zawy lwma
        consensus.zawyLWMAHeight = 60000; // blockheight to switch to LWMA retarget rules
        consensus.powLimitLegacy = uint256S("00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");

        consensus.nPowTargetSpacing = 1 * 60;
		consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1916; // 95% of 2016
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008


        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00"); 

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00");

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        pchMessageStart[0] = 0x7d;
        pchMessageStart[1] = 0x23;
        pchMessageStart[2] = 0x2e;
        pchMessageStart[3] = 0xf5;
        nDefaultPort = 9836;
        nPruneAfterHeight = 100000;

        genesis = CreateGenesisBlock(1536014911, 14836234, 0x1e00ffff, 4, 300 * COIN);

        //(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)

        consensus.hashGenesisBlock = genesis.GetHash();
        //std::cout << "Genesis: "<< consensus.hashGenesisBlock.GetHex() << "\n"; //
        //std::cout << "Merkle: " << genesis.hashMerkleRoot.GetHex() << "\n"; //
/*
//////////////
//////////////

        // calculate Genesis Block
        // Reset genesis
        consensus.hashGenesisBlock = uint256S("0x");
        std::cout << std::string("Begin calculating Mainnet Genesis Block:\n");
        if (true && (genesis.GetHash() != consensus.hashGenesisBlock)) {
            LogPrintf("Calculating Mainnet Genesis Block:\n");
            arith_uint256 hashTarget = arith_uint256().SetCompact(genesis.nBits);
            uint256 hash;
            genesis.nNonce = 0;
            // This will figure out a valid hash and Nonce if you're
            // creating a different genesis block:
            // uint256 hashTarget = CBigNum().SetCompact(genesis.nBits).getuint256();
            // hashTarget.SetCompact(genesis.nBits, &fNegative, &fOverflow).getuint256();
            // while (genesis.GetHash() > hashTarget)
            while (UintToArith256(genesis.GetHash()) > hashTarget)
            {
                ++genesis.nNonce;
                if (genesis.nNonce == 0)
                {
                    LogPrintf("NONCE WRAPPED, incrementing time");
                    std::cout << std::string("NONCE WRAPPED, incrementing time:\n");
                    ++genesis.nTime;
                }
                if (genesis.nNonce % 10000 == 0)
                {
                    LogPrintf("Mainnet: nonce %08u: hash = %s \n", genesis.nNonce, genesis.GetHash().ToString().c_str());
                    // std::cout << strNetworkID << " nonce: " << genesis.nNonce << " time: " << genesis.nTime << " hash: " << genesis.GetHash().ToString().c_str() << "\n";
                }
            }
            std::cout << "Mainnet ---\n";
            std::cout << "  nonce: " << genesis.nNonce <<  "\n";
            std::cout << "   time: " << genesis.nTime << "\n";
            std::cout << "   hash: " << genesis.GetHash().ToString().c_str() << "\n";
            std::cout << "   merklehash: "  << genesis.hashMerkleRoot.ToString().c_str() << "\n";
            // Mainnet --- nonce: 296277 time: 1390095618 hash: 000000bdd771b14e5a031806292305e563956ce2584278de414d9965f6ab54b0
        }
        std::cout << std::string("Finished calculating Mainnet Genesis Block:\n");



*/
//////////////
//////////////



        assert(consensus.hashGenesisBlock == uint256S("0x00000005121e21a4ec6f8a9f51712524f818caec60d5c003b0d47fa0e26fe79c")); // WITH 0X !!!
        assert(genesis.hashMerkleRoot == uint256S("0x51d022478e572b0e26a94de4dd78bb5f756690f956f2e6b69ac740f87a23aefe"));

        vSeeds.emplace_back("194.182.69.120", false);
        vSeeds.emplace_back("217.61.106.233", false);
        vSeeds.emplace_back("217.61.107.9", false);

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,55); // changed 60 to 55
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,122); // changed 122 to 123
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,128);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x08, 0x77, 0xA1, 0x2A}; // was {0x04, 0x88, 0xB2, 0x1E}
        base58Prefixes[EXT_SECRET_KEY] = {0x08, 0x77, 0xA1, 0x2A}; // was {0x04, 0x88, 0xB2, 0x1E}

        bech32_hrp = "pc"; // changed from "rc" to "pc"

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fMiningRequiresPeers = true;

 /*       checkpointData = (CCheckpointData) {                           //MAINNET CHECKPOINT
            {

            }
        };

        chainTxData = ChainTxData{
            // Update as we know more about the contents of the Whiff chain
            1509572692, // * UNIX timestamp of last known number of transactions
            1,          // * total number of transactions between genesis and that timestamp
                        //   (the tx=... number in the SetBestChain debug.log lines)
            3.1         // * estimated number of transactions per second after that timestamp
        }; */
    }
};

/**
 * Testnet (v3)
 */
class CTestNetParams : public CChainParams {
public:
    CTestNetParams() {
        strNetworkID = "test";
        consensus.nSubsidyHalvingInterval = 2100000;  //~ 4 yrs at 1 min block time
        consensus.nBIP34Enabled = true;
        consensus.nBIP65Enabled = true; // 000000000000000004c2b624ed5d7756c508d90fd0da2c7c679febfa6c4735f0
        consensus.nBIP66Enabled = true;
        consensus.nSegwitEnabled = true;
        consensus.nCSVEnabled = true;

        consensus.powLimit = uint256S("00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");


        // original difficulty method, every 2016 blocks
        consensus.nPowTargetTimespan = 2016 * 60; // 1.4 days
        
        // second difficulty method, every 360 blocks
        consensus.nPowDifficultyRetargetHeight = 2017; // blockheight to switch to 360 block retarget rules
        consensus.nPowTargetTimespanShort = 360 * 60; //~6 hours
        
        // final difficulty method, zawy lwma
        consensus.zawyLWMAHeight = 2222; // blockheight to switch to LWMA retarget rules
        consensus.powLimitLegacy = uint256S("00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");

        consensus.nPowTargetSpacing = 1 * 60;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1512; // 75% for testchains
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008


        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00");


        pchMessageStart[0] = 0x4e;
        pchMessageStart[1] = 0x2b;
        pchMessageStart[2] = 0x29;
        pchMessageStart[3] = 0xf7;
        nDefaultPort = 16824;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1536014995, 22213251, 0x1e00ffff, 4, 300 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();

        //Test MerkleRoot and GenesisBlock
//////////////
//////////////
/*
        // calculate Genesis Block
        // Reset genesis
        consensus.hashGenesisBlock = uint256S("0x");
        std::cout << std::string("Begin calculating Testnet Genesis Block:\n");
        if (true && (genesis.GetHash() != consensus.hashGenesisBlock)) {
            LogPrintf("Calculating Mainnet Genesis Block:\n");
            arith_uint256 hashTarget = arith_uint256().SetCompact(genesis.nBits);
            uint256 hash;
            genesis.nNonce = 0;
            // This will figure out a valid hash and Nonce if you're
            // creating a different genesis block:
            // uint256 hashTarget = CBigNum().SetCompact(genesis.nBits).getuint256();
            // hashTarget.SetCompact(genesis.nBits, &fNegative, &fOverflow).getuint256();
            // while (genesis.GetHash() > hashTarget)
            while (UintToArith256(genesis.GetHash()) > hashTarget)
            {
                ++genesis.nNonce;
                if (genesis.nNonce == 0)
                {
                    LogPrintf("NONCE WRAPPED, incrementing time");
                    std::cout << std::string("NONCE WRAPPED, incrementing time:\n");
                    ++genesis.nTime;
                }
                if (genesis.nNonce % 10000 == 0)
                {
                    LogPrintf("Mainnet: nonce %08u: hash = %s \n", genesis.nNonce, genesis.GetHash().ToString().c_str());
                    // std::cout << strNetworkID << " nonce: " << genesis.nNonce << " time: " << genesis.nTime << " hash: " << genesis.GetHash().ToString().c_str() << "\n";
                }
            }
            std::cout << "Mainnet ---\n";
            std::cout << "  nonce: " << genesis.nNonce <<  "\n";
            std::cout << "   time: " << genesis.nTime << "\n";
            std::cout << "   hash: " << genesis.GetHash().ToString().c_str() << "\n";
            std::cout << "   merklehash: "  << genesis.hashMerkleRoot.ToString().c_str() << "\n";
            // Mainnet --- nonce: 296277 time: 1390095618 hash: 000000bdd771b14e5a031806292305e563956ce2584278de414d9965f6ab54b0
        }
        std::cout << std::string("Finished calculating Mainnet Genesis Block:\n");



*/
//////////////
//////////////

        assert(consensus.hashGenesisBlock == uint256S("0x0000002ac491035775deaac33097d55fe6a99d5f586d1c5bad4d08fa91ad1e3a")); // WITH 0X !!!
        assert(genesis.hashMerkleRoot == uint256S("0x51d022478e572b0e26a94de4dd78bb5f756690f956f2e6b69ac740f87a23aefe")); // WITH 0X !!!

        vFixedSeeds.clear();
        vSeeds.clear();

        vSeeds.emplace_back("", false);
        vSeeds.emplace_back("", false);

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,112); // from 111
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,193); // from 196
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,233); // from 239
        base58Prefixes[EXT_PUBLIC_KEY] = {0x03, 0x53, 0x77, 0xCA}; // from {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x03, 0x53, 0x88, 0x91}; // {0x04, 0x35, 0x83, 0x94};

        bech32_hrp = "tr";

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;
        fMiningRequiresPeers = true;

       /* checkpointData = (CCheckpointData) {
            {
            }
        };

        chainTxData = ChainTxData{
            // Update as we know more about the contents of the Whiff chain
            1513705170, // * UNIX timestamp of last known number of transactions
            1,          // * total number of transactions between genesis and that timestamp
                        //   (the tx=... number in the SetBestChain debug.log lines)
            3.1         // * estimated number of transactions per second after that timestamp
        };*/

    }
};

/**
 * Regression test
 */
class CRegTestParams : public CChainParams {
public:
    CRegTestParams() {
        strNetworkID = "regtest";
        consensus.nBIP34Enabled = true;
        consensus.nBIP65Enabled = true; // 000000000000000004c2b624ed5d7756c508d90fd0da2c7c679febfa6c4735f0
        consensus.nBIP66Enabled = true;
        consensus.nSegwitEnabled = true;
        consensus.nCSVEnabled = true;
        consensus.nSubsidyHalvingInterval = 150;
        consensus.powLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 2016 * 60; // 1.4 days
        consensus.nPowTargetSpacing = 1 * 60;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;
        consensus.nRuleChangeActivationThreshold = 108; // 75% for testchains
        consensus.nMinerConfirmationWindow = 144; // Faster than normal for regtest (144 instead of 2016)
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 999999999999ULL;

        // quick regnet paste, we never use regnet
        consensus.zawyLWMAHeight = -1; // Activated on regnet
        consensus.powLimitLegacy = uint256S("00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");


        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00");

        pchMessageStart[0] = 0x43;
        pchMessageStart[1] = 0x52;
        pchMessageStart[2] = 0x4F;
        pchMessageStart[3] = 0x57;
        nDefaultPort = 18444;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1521663333, 5898126, 0x207fffff, 4, 5000 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();

        std::cout << "HGB: " << consensus.hashGenesisBlock.GetHex() << std::endl;
        std::cout << "HGR: " << genesis.hashMerkleRoot.GetHex() << std::endl;

        assert(consensus.hashGenesisBlock == uint256S("0x000000c0407bb6b446d75201d50724ce23d61b970700827b02632b4c47714761"));
        assert(genesis.hashMerkleRoot == uint256S("0xf0cc5f92b11a6655a4939fc239e8bf960cd0453b87b5a0820ab36904279341a5"));

        vFixedSeeds.clear(); //!< Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();      //!< Regtest mode doesn't have any DNS seeds.

        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;

        checkpointData = (CCheckpointData) {
            {
            }
        };

        chainTxData = ChainTxData{
            0,
            0,
            0
        };

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,112); // from 111
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,193); // from 196
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,233); // from 239
        base58Prefixes[EXT_PUBLIC_KEY] = {0x03, 0x53, 0x77, 0xCA}; // from {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x03, 0x53, 0x88, 0x91}; // {0x04, 0x35, 0x83, 0x94};

        bech32_hrp = "rcrt";
    }
};

static std::unique_ptr<CChainParams> globalChainParams;

const CChainParams &Params() {
    assert(globalChainParams);
    return *globalChainParams;
}

std::unique_ptr<CChainParams> CreateChainParams(const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN)
        return std::unique_ptr<CChainParams>(new CMainParams());
    else if (chain == CBaseChainParams::TESTNET)
        return std::unique_ptr<CChainParams>(new CTestNetParams());
    else if (chain == CBaseChainParams::REGTEST)
        return std::unique_ptr<CChainParams>(new CRegTestParams());
    throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network)
{
    SelectBaseParams(network);
    globalChainParams = CreateChainParams(network);
}

void UpdateVersionBitsParameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    globalChainParams->UpdateVersionBitsParameters(d, nStartTime, nTimeout);
}

void TurnOffSegwit(){
	globalChainParams->TurnOffSegwit();
}

void TurnOffCSV() {
	globalChainParams->TurnOffCSV();
}

void TurnOffBIP34() {
	globalChainParams->TurnOffBIP34();
}

void TurnOffBIP65() {
	globalChainParams->TurnOffBIP65();
}

void TurnOffBIP66() {
	globalChainParams->TurnOffBIP66();
}
