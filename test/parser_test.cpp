#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <naobi/utils/parser.hpp>

TEST(parser, split)
{
	std::string test = "test";
	auto buffer = naobi::parser::split(test, ";", true);
	ASSERT_THAT(buffer, testing::ElementsAre("test"));

	test = "test;check;  keks;\n tatata     cool; tatat";
	buffer = naobi::parser::split(test, ";", true);
	ASSERT_THAT(buffer, testing::ElementsAre("test;", "check;", "  keks;", "\n tatata     cool;", " tatat"));

	test = "t;g;f;s;;df;sdfsdf;rwer;fscv;";
	buffer = naobi::parser::split(test, ";", true);
	ASSERT_THAT(buffer, testing::ElementsAre("t;", "g;", "f;", "s;","df;","sdfsdf;","rwer;", "fscv;"));

	test = "abc test   track  test";
	buffer = naobi::parser::split(test, " ", true);
	ASSERT_THAT(buffer, testing::ElementsAre("abc ", "test ", "track ", "test"));
}

TEST(parser, removeExtraSpaces)
{
	std::string test = "      test test  test  test test   test  test   ";
	EXPECT_EQ(naobi::parser::removeExtraSpaces(test), " test test test test test test test ");

	test = "\ttest\t\ttest\t\t\ttest";
	EXPECT_EQ(naobi::parser::removeExtraSpaces(test), "\ttest\ttest\ttest");
}

TEST(parser, removeSym)
{
	std::string test = "some message for   testing ";
	EXPECT_EQ(naobi::parser::removeSym(test, ' '), "somemessagefortesting");
}