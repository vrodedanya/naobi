#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <naobi/utils/type.hpp>

using namespace naobi::utils;


TEST(IS_TYPE, isInteger)
{
	EXPECT_EQ(type::isInteger("5"), true);
	EXPECT_EQ(type::isInteger("10000"), true);
	EXPECT_EQ(type::isInteger("\"10000\""), false);
	EXPECT_EQ(type::isInteger("-134"), true);
	EXPECT_EQ(type::isInteger("true"), false);
	EXPECT_EQ(type::isInteger("1"), true);
}

TEST(IS_TYPE, isBoolean)
{
	EXPECT_EQ(type::isBoolean("true"), true);
	EXPECT_EQ(type::isBoolean("false"), true);
	EXPECT_EQ(type::isBoolean("\"true\""), false);
	EXPECT_EQ(type::isBoolean("sfsd"), false);
	EXPECT_EQ(type::isBoolean("fsdf"), false);
	EXPECT_EQ(type::isBoolean("1"), false);
	EXPECT_EQ(type::isBoolean("0"), false);
}

TEST(IS_TYPE, isString)
{
	EXPECT_EQ(type::isString("\"Str\""), true);
	EXPECT_EQ(type::isString("\"fagffdgwg1123123\""), true);
	EXPECT_EQ(type::isString("fagffdgwg1123123"), false);
	EXPECT_EQ(type::isString("5.0"), false);
}

TEST(IS_TYPE, isFloat)
{
	EXPECT_EQ(type::isFloat("5.0"), true);
	EXPECT_EQ(type::isFloat("14.0"), true);
	EXPECT_EQ(type::isFloat("14.425"), true);
	EXPECT_EQ(type::isFloat("-14.425"), true);
	EXPECT_EQ(type::isFloat("-101.425"), true);
	EXPECT_EQ(type::isFloat("-0.425"), true);
	EXPECT_EQ(type::isFloat("14"), false);
	EXPECT_EQ(type::isFloat("-14"), false);
}