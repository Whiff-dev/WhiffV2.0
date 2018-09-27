// Copyright (c) 2011-2014 The Bitcoin Core developers
// Copyright (c) 2017 The Whiff Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef WHIFF_QT_WHIFFADDRESSVALIDATOR_H
#define WHIFF_QT_WHIFFADDRESSVALIDATOR_H

#include <QValidator>

/** Base58 entry widget validator, checks for valid characters and
 * removes some whitespace.
 */
class WhiffAddressEntryValidator : public QValidator
{
    Q_OBJECT

public:
    explicit WhiffAddressEntryValidator(QObject *parent);

    State validate(QString &input, int &pos) const;
};

/** Whiff address widget validator, checks for a valid whiff address.
 */
class WhiffAddressCheckValidator : public QValidator
{
    Q_OBJECT

public:
    explicit WhiffAddressCheckValidator(QObject *parent);

    State validate(QString &input, int &pos) const;
};

#endif // WHIFF_QT_WHIFFADDRESSVALIDATOR_H
