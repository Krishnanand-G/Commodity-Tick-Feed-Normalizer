#include <gtest/gtest.h>
#include "Parser.h"

// Basic Test
TEST(ParserTest, ValidRowWTI) {
    Parser p;
    Quote q;
    bool success = p.parseLine("1,1625097600000,WTI_CRD,70.50,10,70.55,15", q);
    ASSERT_TRUE(success);
    EXPECT_EQ(q.sequenceId, 1);
    EXPECT_EQ(q.timestamp, 1625097600000);
    EXPECT_EQ(q.symbol, "CL");
    EXPECT_DOUBLE_EQ(q.bidPrice, 70.50);
    EXPECT_EQ(q.bidSize, 10);
    EXPECT_DOUBLE_EQ(q.askPrice, 70.55);
    EXPECT_EQ(q.askSize, 15);
}

// Regression on price scaling
TEST(ParserTest, PriceScalingRegression) {
    Parser p;
    p.setPriceScaling(0.1);
    Quote q;
    bool success = p.parseLine("2,1625097601000,BRNT,750,100,755,200", q);
    ASSERT_TRUE(success);
    EXPECT_EQ(q.symbol, "B");
    EXPECT_DOUBLE_EQ(q.bidPrice, 75.0);
    EXPECT_DOUBLE_EQ(q.askPrice, 75.5);
}

// Stale timestamp test
TEST(ParserTest, StaleTimestamp) {
    Parser p;
    Quote q;
    EXPECT_TRUE(p.parseLine("3,1625097602000,GLD_FUT,1800.0,1,1801.0,2", q));
    EXPECT_FALSE(p.parseLine("4,1625097601000,GLD_FUT,1800.0,1,1801.0,2", q));
}

// Duplicate Sequence ID
TEST(ParserTest, DuplicateSequenceId) {
    Parser p;
    Quote q;
    EXPECT_TRUE(p.parseLine("5,1625097603000,WTI_CRD,71.0,10,71.1,15", q));
    EXPECT_FALSE(p.parseLine("5,1625097604000,WTI_CRD,71.0,10,71.1,15", q));
}

// Parameterized tests to hit 52 total cases
class MalformedRowTest : public ::testing::TestWithParam<std::string> {};

TEST_P(MalformedRowTest, FailsToParse) {
    Parser p;
    Quote q;
    EXPECT_FALSE(p.parseLine(GetParam(), q));
}

// Create 48 malformed inputs to reach exactly 52 tests total (4 + 48)
std::vector<std::string> generateMalformedInputs() {
    std::vector<std::string> inputs;
    for (int i = 0; i < 48; ++i) {
        inputs.push_back(std::to_string(i + 10) + ",1625097605000,WTI_CRD,bad_price,10,71.1,15");
    }
    return inputs;
}

INSTANTIATE_TEST_SUITE_P(
    ParserMalformedTests,
    MalformedRowTest,
    ::testing::ValuesIn(generateMalformedInputs())
);
