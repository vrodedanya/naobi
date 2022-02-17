#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <naobi/compiler/compiler.hpp>

using namespace testing;

TEST(compiler, split)
{
	std::string test = "test";
	auto buffer = naobi::compiler::split(test, ";", true);
	ASSERT_THAT(buffer, testing::ElementsAre("test"));
	test = "test;check;  keks;\n tatata     cool; tatat";
	buffer = naobi::compiler::split(test, ";", true);
	ASSERT_THAT(buffer, testing::ElementsAre("test;", "check;", "  keks;", "\n tatata     cool;", " tatat"));
	test = "t;g;f;s;;df;sdfsdf;rwer;fscv;";
	buffer = naobi::compiler::split(test, ";", true);
	ASSERT_THAT(buffer, testing::ElementsAre("t;", "g;", "f;", "s;","df;","sdfsdf;","rwer;", "fscv;"));
	test = "abc test   track  test";
	buffer = naobi::compiler::split(test, " ", true);
	ASSERT_THAT(buffer, testing::ElementsAre("abc ", "test ", "track ", "test"));
}

TEST(compiler, collectModules)
{
	std::string test = R"(
import lib.file2.naobi;
import lib.file3.naobi;
test command some text;
also ;
  text;
import lib.file4.naobi;
			 )";
	ASSERT_THAT(naobi::compiler::collectModules(test), testing::ElementsAre("lib.file2.naobi", "lib.file3.naobi", "lib.file4.naobi"));
}

TEST(compiler, removeExtraSpaces)
{
	std::string test = "      test test  test  test test   test  test   ";
	auto removedExtra = naobi::compiler::removeExtraSpaces(test);
	EXPECT_EQ(naobi::compiler::removeExtraSpaces(test), " test test test test test test test ");
	test = "\ttest\t\ttest\t\t\ttest";
	EXPECT_EQ(naobi::compiler::removeExtraSpaces(test), "\ttest\ttest\ttest");
}