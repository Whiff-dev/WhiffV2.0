// Copyright (c) 2016 The Bitcoin Core developers
// Copyright (c) 2017 The Whiff Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef WHIFF_WALLET_TEST_FIXTURE_H
#define WHIFF_WALLET_TEST_FIXTURE_H

#include "test/test_whiff.h"

/** Testing setup and teardown for wallet.
 */
struct WalletTestingSetup: public TestingSetup {
    explicit WalletTestingSetup(const std::string& chainName = CBaseChainParams::MAIN);
    ~WalletTestingSetup();
};

#endif

