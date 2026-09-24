/*
===========================================================================
Copyright (C) 2026 the OpenMoHAA team

This file is part of OpenMoHAA source code.

OpenMoHAA source code is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the License,
or (at your option) any later version.
===========================================================================
*/

#include "../sv_chatban_core.h"
#include "../sv_admin.h"

#include <ctype.h>
#include <stdio.h>
#include <string.h>

static int failures;

#define CHECK(condition)                                                              \
    do {                                                                              \
        if (!(condition)) {                                                           \
            fprintf(stderr, "FAIL %s:%d: %s\n", __FILE__, __LINE__, #condition);     \
            failures++;                                                               \
        }                                                                             \
    } while (0)

int Q_stricmp(const char *left, const char *right)
{
    unsigned char a;
    unsigned char b;

    if (!left || !right) {
        return left != right;
    }

    do {
        a = (unsigned char)tolower((unsigned char)*left++);
        b = (unsigned char)tolower((unsigned char)*right++);
    } while (a && a == b);

    return (int)a - (int)b;
}

qboolean NET_CompareBaseAdrMask(netadr_t left, netadr_t right, int prefix)
{
    const byte *leftBytes;
    const byte *rightBytes;
    int byteCount;
    int fullBytes;
    int remainingBits;
    byte mask;

    if (left.type != right.type) {
        return qfalse;
    }

    if (left.type == NA_IP) {
        leftBytes = left.ip;
        rightBytes = right.ip;
        byteCount = 4;
    } else if (left.type == NA_IP6) {
        leftBytes = left.ip6;
        rightBytes = right.ip6;
        byteCount = 16;
    } else {
        return qfalse;
    }

    if (prefix < 0 || prefix > byteCount * 8) {
        prefix = byteCount * 8;
    }

    fullBytes = prefix / 8;
    remainingBits = prefix % 8;
    if (fullBytes && memcmp(leftBytes, rightBytes, (size_t)fullBytes)) {
        return qfalse;
    }
    if (!remainingBits) {
        return qtrue;
    }

    mask = (byte)(0xffu << (8 - remainingBits));
    return (leftBytes[fullBytes] & mask) == (rightBytes[fullBytes] & mask);
}

static netadr_t IPv4(byte a, byte b, byte c, byte d)
{
    netadr_t address;

    memset(&address, 0, sizeof(address));
    address.type = NA_IP;
    address.ip[0] = a;
    address.ip[1] = b;
    address.ip[2] = c;
    address.ip[3] = d;
    return address;
}

static netadr_t IPv6(
    unsigned short a,
    unsigned short b,
    unsigned short c,
    unsigned short d,
    unsigned short e,
    unsigned short f,
    unsigned short g,
    unsigned short h
)
{
    const unsigned short words[8] = {a, b, c, d, e, f, g, h};
    netadr_t address;
    int index;

    memset(&address, 0, sizeof(address));
    address.type = NA_IP6;
    for (index = 0; index < 8; index++) {
        address.ip6[index * 2] = (byte)(words[index] >> 8);
        address.ip6[index * 2 + 1] = (byte)(words[index] & 0xff);
    }
    return address;
}

static void TestReasonsAndAddresses(void)
{
    char longestReason[MAX_CHATBAN_REASON_LENGTH + 1];
    char deleteReason[] = {'b', 'a', 'd', 127, '\0'};

    memset(longestReason, 'x', sizeof(longestReason));
    longestReason[MAX_CHATBAN_REASON_LENGTH - 1] = '\0';

    CHECK(SV_ChatBanReasonIsSafe(""));
    CHECK(SV_ChatBanReasonIsSafe("repeated spam"));
    CHECK(SV_ChatBanReasonIsSafe(longestReason));
    CHECK(!SV_ChatBanReasonIsSafe(NULL));
    longestReason[MAX_CHATBAN_REASON_LENGTH - 1] = 'x';
    longestReason[MAX_CHATBAN_REASON_LENGTH] = '\0';
    CHECK(!SV_ChatBanReasonIsSafe(longestReason));
    CHECK(!SV_ChatBanReasonIsSafe("bad\"reason"));
    CHECK(!SV_ChatBanReasonIsSafe("bad\\reason"));
    CHECK(!SV_ChatBanReasonIsSafe("bad;reason"));
    CHECK(!SV_ChatBanReasonIsSafe("bad\nreason"));
    CHECK(!SV_ChatBanReasonIsSafe(deleteReason));

    CHECK(SV_ChatBanAddressTokenIsSafe("203.0.113.7"));
    CHECK(SV_ChatBanAddressTokenIsSafe("2001:db8::7"));
    CHECK(!SV_ChatBanAddressTokenIsSafe("example.invalid"));
    CHECK(!SV_ChatBanAddressTokenIsSafe("203.0.113"));
    CHECK(!SV_ChatBanAddressTokenIsSafe("203.0.113.999"));
    CHECK(!SV_ChatBanAddressTokenIsSafe("203.0.113.07"));
    CHECK(!SV_ChatBanAddressTokenIsSafe("203.0.113.7/32"));
    CHECK(!SV_ChatBanAddressTokenIsSafe("[2001:db8::7]"));
    CHECK(SV_ChatBanAddressTokenIsSafe("::"));
    CHECK(SV_ChatBanAddressTokenIsSafe("::ffff:203.0.113.7"));
    // One colon would be parsed as host:port and resolved through DNS.
    CHECK(!SV_ChatBanAddressTokenIsSafe("db8:1"));
    CHECK(!SV_ChatBanAddressTokenIsSafe("cafe:0"));
    CHECK(!SV_ChatBanAddressTokenIsSafe("deadbeef.cafe:1"));
    CHECK(!SV_ChatBanAddressTokenIsSafe("2001:"));
}

static void TestNumbersAndPaths(void)
{
    char path[64];
    int value;

    CHECK(SV_ChatBanParseInteger("0", 0, 128, &value) && value == 0);
    CHECK(SV_ChatBanParseInteger("128", 0, 128, &value) && value == 128);
    CHECK(!SV_ChatBanParseInteger("129", 0, 128, &value));
    CHECK(!SV_ChatBanParseInteger("-1", 0, 128, &value));
    CHECK(!SV_ChatBanParseInteger("1x", 0, 128, &value));
    CHECK(!SV_ChatBanParseInteger("999999999999999999999999", 0, 128, &value));

    CHECK(SV_ChatBanCanonicalizeRelativePath("chatbans.dat", path, sizeof(path)));
    CHECK(!strcmp(path, "chatbans.dat"));
    CHECK(SV_ChatBanCanonicalizeRelativePath("./SERVERBANS.dat", path, sizeof(path)));
    CHECK(!strcmp(path, "serverbans.dat"));
    CHECK(SV_ChatBanCanonicalizeRelativePath("lists//./ChatBans.dat", path, sizeof(path)));
    CHECK(!strcmp(path, "lists/chatbans.dat"));
    CHECK(!SV_ChatBanCanonicalizeRelativePath("../serverbans.dat", path, sizeof(path)));
    CHECK(!SV_ChatBanCanonicalizeRelativePath("lists/../serverbans.dat", path, sizeof(path)));
    CHECK(!SV_ChatBanCanonicalizeRelativePath("/chatbans.dat", path, sizeof(path)));
    CHECK(!SV_ChatBanCanonicalizeRelativePath("chat bans.dat", path, sizeof(path)));
    CHECK(!SV_ChatBanCanonicalizeRelativePath("chatbans.dat;quit", path, sizeof(path)));
    CHECK(!SV_ChatBanCanonicalizeRelativePath("chatbans.dat", path, 4));
    CHECK(ACCESSLEVEL_DISCHAT == 64);
}

static void TestRecordParsingAndRoundTrip(void)
{
    char blank[] = " \t\r";
    char comment[] = "  // ignored";
    char ipv4[] = "203.0.113.7 32:repeated spam\r";
    char ipv6[] = "2001:db8::7 128";
    char allIPv4[] = "0.0.0.0 0:all IPv4";
    char noPrefix[] = "203.0.113.7";
    char negativePrefix[] = "203.0.113.7 -1";
    char largePrefix[] = "203.0.113.7 129";
    char largeIPv4Prefix[] = "203.0.113.7 33";
    char trailing[] = "203.0.113.7 32 trailing";
    char unsafe[] = "203.0.113.7 32:bad;reason";
    char malformedIPv4[] = "203.0.113.999 32";
    char record[256];
    char roundTrip[256];
    char *address;
    char *reason;
    int prefix;

    CHECK(SV_ChatBanParseRecord(blank, &address, &prefix, &reason) == CHATBAN_PARSE_SKIP);
    CHECK(SV_ChatBanParseRecord(comment, &address, &prefix, &reason) == CHATBAN_PARSE_SKIP);

    CHECK(SV_ChatBanParseRecord(ipv4, &address, &prefix, &reason) == CHATBAN_PARSE_OK);
    CHECK(!strcmp(address, "203.0.113.7"));
    CHECK(prefix == 32);
    CHECK(!strcmp(reason, "repeated spam"));

    CHECK(SV_ChatBanParseRecord(ipv6, &address, &prefix, &reason) == CHATBAN_PARSE_OK);
    CHECK(!strcmp(address, "2001:db8::7"));
    CHECK(prefix == 128);
    CHECK(!reason[0]);

    CHECK(SV_ChatBanParseRecord(allIPv4, &address, &prefix, &reason) == CHATBAN_PARSE_OK);
    CHECK(prefix == 0);
    CHECK(!strcmp(reason, "all IPv4"));

    CHECK(SV_ChatBanParseRecord(noPrefix, &address, &prefix, &reason) == CHATBAN_PARSE_INVALID);
    CHECK(SV_ChatBanParseRecord(negativePrefix, &address, &prefix, &reason) == CHATBAN_PARSE_INVALID);
    CHECK(SV_ChatBanParseRecord(largePrefix, &address, &prefix, &reason) == CHATBAN_PARSE_INVALID);
    CHECK(SV_ChatBanParseRecord(largeIPv4Prefix, &address, &prefix, &reason) == CHATBAN_PARSE_INVALID);
    CHECK(SV_ChatBanParseRecord(trailing, &address, &prefix, &reason) == CHATBAN_PARSE_INVALID);
    CHECK(SV_ChatBanParseRecord(unsafe, &address, &prefix, &reason) == CHATBAN_PARSE_INVALID);
    CHECK(SV_ChatBanParseRecord(malformedIPv4, &address, &prefix, &reason) == CHATBAN_PARSE_INVALID);

    CHECK(SV_ChatBanFormatRecord(
        "2001:db8::7", 64, "repeated spam", record, sizeof(record)));
    CHECK(!strcmp(record, "2001:db8::7 64:repeated spam\n"));
    memcpy(roundTrip, record, strlen(record) + 1);
    CHECK(SV_ChatBanParseRecord(roundTrip, &address, &prefix, &reason) == CHATBAN_PARSE_OK);
    CHECK(!strcmp(address, "2001:db8::7"));
    CHECK(prefix == 64);
    CHECK(!strcmp(reason, "repeated spam"));

    CHECK(SV_ChatBanFormatRecord("0.0.0.0", 0, "", record, sizeof(record)));
    CHECK(!strcmp(record, "0.0.0.0 0\n"));
    CHECK(!SV_ChatBanFormatRecord("203.0.113.7", 129, "", record, sizeof(record)));
    CHECK(!SV_ChatBanFormatRecord("203.0.113.7", 33, "", record, sizeof(record)));
    CHECK(!SV_ChatBanFormatRecord("203.0.113.7", 32, "bad;reason", record, sizeof(record)));
    CHECK(!SV_ChatBanFormatRecord("203.0.113.7", 32, "", record, 4));
}

static void TestCommandClassification(void)
{
    CHECK(SV_ChatBanClassifyCommand("say", NULL) == CHATBAN_COMMAND_TEXT);
    CHECK(SV_ChatBanClassifyCommand("SAYTEAM", NULL) == CHATBAN_COMMAND_TEXT);
    CHECK(SV_ChatBanClassifyCommand("tell", NULL) == CHATBAN_COMMAND_TEXT);
    CHECK(SV_ChatBanClassifyCommand("dmmessage", "hello") == CHATBAN_COMMAND_TEXT);
    CHECK(SV_ChatBanClassifyCommand("dmmessage", NULL) == CHATBAN_COMMAND_TEXT);

    CHECK(SV_ChatBanClassifyCommand("vsay", NULL) == CHATBAN_COMMAND_TAUNT);
    CHECK(SV_ChatBanClassifyCommand("vosay", NULL) == CHATBAN_COMMAND_TAUNT);
    CHECK(SV_ChatBanClassifyCommand("vtell", NULL) == CHATBAN_COMMAND_TAUNT);
    CHECK(SV_ChatBanClassifyCommand("instamsg", NULL) == CHATBAN_COMMAND_TAUNT);
    CHECK(SV_ChatBanClassifyCommand("dmmessage", "*11") == CHATBAN_COMMAND_TAUNT);
    CHECK(SV_ChatBanClassifyCommand("dmmessage", "*99") == CHATBAN_COMMAND_TAUNT);
    CHECK(SV_ChatBanClassifyCommand("dmmessage", "*00") == CHATBAN_COMMAND_TEXT);
    CHECK(SV_ChatBanClassifyCommand("dmmessage", "*111") == CHATBAN_COMMAND_TEXT);

    CHECK(SV_ChatBanClassifyCommand("join_team", NULL) == CHATBAN_COMMAND_NONE);
    CHECK(SV_ChatBanClassifyCommand("download", NULL) == CHATBAN_COMMAND_NONE);
    CHECK(SV_ChatBanClassifyCommand("userinfo", NULL) == CHATBAN_COMMAND_NONE);
    CHECK(SV_ChatBanClassifyCommand("ad_login", NULL) == CHATBAN_COMMAND_NONE);
}

static void TestMatchingAndSubsumption(void)
{
    serverChatBan_t entries[8];
    netadr_t ipv4Range = IPv4(203, 0, 113, 42);
    netadr_t ipv6Range = IPv6(0x2001, 0x0db8, 0x0001, 0x0002, 0, 0, 0, 1);
    int count;

    memset(entries, 0, sizeof(entries));
    entries[0].ip = ipv4Range;
    entries[0].subnet = 24;
    entries[1].ip = IPv4(198, 51, 100, 7);
    entries[1].subnet = 32;
    entries[2].ip = ipv6Range;
    entries[2].subnet = 64;
    count = 3;

    CHECK(SV_ChatBanEntryMatches(&entries[0], IPv4(203, 0, 113, 0)));
    CHECK(SV_ChatBanEntryMatches(&entries[0], IPv4(203, 0, 113, 255)));
    CHECK(!SV_ChatBanEntryMatches(&entries[0], IPv4(203, 0, 112, 255)));
    CHECK(!SV_ChatBanEntryMatches(&entries[0], ipv6Range));
    CHECK(SV_ChatBanEntryMatches(
        &entries[2], IPv6(0x2001, 0x0db8, 0x0001, 0x0002, 0xffff, 0, 0, 9)));
    CHECK(!SV_ChatBanEntryMatches(
        &entries[2], IPv6(0x2001, 0x0db8, 0x0001, 0x0003, 0, 0, 0, 1)));

    CHECK(SV_ChatBanFindExactEntry(entries, count, IPv4(203, 0, 113, 99), 24) == 0);
    CHECK(SV_ChatBanFindExactEntry(entries, count, IPv4(203, 0, 113, 99), 32) == -1);
    CHECK(SV_ChatBanFindCoveringEntry(entries, count, IPv4(203, 0, 113, 99), 32) == 0);
    CHECK(SV_ChatBanFindCoveringEntry(entries, count, IPv4(203, 0, 113, 99), 16) == -1);

    entries[3].ip = IPv4(203, 0, 113, 128);
    entries[3].subnet = 25;
    entries[4].ip = IPv4(203, 0, 113, 200);
    entries[4].subnet = 32;
    entries[5].ip = IPv4(203, 0, 114, 1);
    entries[5].subnet = 32;
    count = 6;
    CHECK(SV_ChatBanDeleteCoveredEntries(entries, &count, ipv4Range, 24) == 2);
    CHECK(count == 4);
    CHECK(SV_ChatBanFindExactEntry(entries, count, IPv4(203, 0, 114, 1), 32) >= 0);

    entries[count].ip = IPv4(1, 2, 3, 4);
    entries[count].subnet = 0;
    count++;
    CHECK(SV_ChatBanFindCoveringEntry(entries, count, IPv4(255, 255, 255, 255), 32) == count - 1);
    CHECK(SV_ChatBanFindCoveringEntry(entries, count, ipv6Range, 128) == 2);
    CHECK(SERVER_MAXCHATBANS == 1024);
}

int main(void)
{
    TestReasonsAndAddresses();
    TestNumbersAndPaths();
    TestRecordParsingAndRoundTrip();
    TestCommandClassification();
    TestMatchingAndSubsumption();

    if (failures) {
        fprintf(stderr, "%d chat-ban test(s) failed\n", failures);
        return 1;
    }

    printf("All chat-ban tests passed\n");
    return 0;
}
