#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <naobi/utils/parser.hpp>

TEST(parser, split)
{
	std::string test = "test";
	auto buffer = naobi::parser::split(test, ";", naobi::parser::split_mods::SAVE_SPLITTER);
	ASSERT_THAT(buffer, testing::ElementsAre("test"));

	test = "test;check;  keks;\n tatata     cool; tatat";
	buffer = naobi::parser::split(test, ";", naobi::parser::split_mods::SAVE_SPLITTER);
	ASSERT_THAT(buffer, testing::ElementsAre("test;", "check;", "  keks;", "\n tatata     cool;", " tatat"));

	test = "t;g;f;s;;df;sdfsdf;rwer;fscv;";
	buffer = naobi::parser::split(test, ";", naobi::parser::split_mods::SAVE_SPLITTER);
	ASSERT_THAT(buffer, testing::ElementsAre("t;", "g;", "f;", "s;","df;","sdfsdf;","rwer;", "fscv;"));

	test = "abc test   track  test";
	buffer = naobi::parser::split(test, " ", naobi::parser::split_mods::SAVE_SPLITTER);
	ASSERT_THAT(buffer, testing::ElementsAre("abc ", "test ", "track ", "test"));

	test = "abc test   track  test";
	buffer = naobi::parser::split(test, " ");
	ASSERT_THAT(buffer, testing::ElementsAre("abc", "test", "track", "test"));

	test = "some string";
	buffer = naobi::parser::split(test, "");
	ASSERT_THAT(buffer, testing::ElementsAre());

	test = R"(some string "text test txt" str " ")";
	buffer = naobi::parser::split(test, " ", naobi::parser::split_mods::SAVE_BLOCKS);
	ASSERT_THAT(buffer, testing::ElementsAre("some", "string", "\"text test txt\"", "str", "\" \""));

	test = R"(some string "text test txt" str " )";
	buffer = naobi::parser::split(test, " ", naobi::parser::split_mods::SAVE_BLOCKS);
	ASSERT_THAT(buffer, testing::ElementsAre("some", "string", "\"text test txt\"", "str", "\""));
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

TEST(parser, placeAfter)
{
	EXPECT_EQ(naobi::parser::placeAfter("check\ntest\nkek\n", '\n', " -> "), "check\n -> test\n -> kek\n -> ");
}

TEST(parser, dirName)
{
	EXPECT_EQ(naobi::parser::dirName("test/file/name.txt"), "test/file");
	EXPECT_EQ(naobi::parser::dirName("test\\file\\name.txt"), "test\\file");
	EXPECT_EQ(naobi::parser::dirName("../test/check/name.txt"), "../test/check");
	EXPECT_EQ(naobi::parser::dirName("../keep/get/check/"), "../keep/get/check");
}

TEST(parser, fileName)
{
	EXPECT_EQ(naobi::parser::fileName("test/file/name.txt"), "name.txt");
	EXPECT_EQ(naobi::parser::fileName("test\\file\\name.txt"), "name.txt");
	EXPECT_EQ(naobi::parser::fileName("../test/check/name.txt"), "name.txt");
	EXPECT_EQ(naobi::parser::fileName("../keep/get/check/"), "");
}