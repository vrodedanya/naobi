#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <naobi/utils/parser.hpp>

TEST(parser, split)
{
	ASSERT_THAT(naobi::parser::split("a;b;c;d", {";"}, {}), testing::ElementsAre("a", "b", "c", "d"));
	ASSERT_THAT(naobi::parser::split("a;b;c;d;", {";"}, {}), testing::ElementsAre("a", "b", "c", "d"));
	ASSERT_THAT(naobi::parser::split("a;;b;c;d;", {";"}, {}), testing::ElementsAre("a", "b", "c", "d"));
	ASSERT_THAT(naobi::parser::split("a + b = c", {}, {"+", "="}),
				testing::ElementsAre("a ", "+", " b ", "=", " c"));
	ASSERT_THAT(naobi::parser::split("a + \"c+d\" = c", {" "}, {}),
				testing::ElementsAre("a", "+", "\"c+d\"", "=", "c"));
	ASSERT_THAT(naobi::parser::split("a + \"c+d\" = c", {""}, {"+", "="}),
				testing::ElementsAre("a ", "+", " \"c+d\" ", "=", " c"));
	ASSERT_THAT(naobi::parser::split("a+\"c+d\"+\"test\"= c", {""}, {"+", "="}),
				testing::ElementsAre("a", "+", "\"c+d\"", "+", "\"test\"", "=", " c"));
	ASSERT_THAT(naobi::parser::split("c\"test\"", {}, {}),
				testing::ElementsAre("c\"test\""));
	ASSERT_THAT(naobi::parser::split("workflow test {int d=a+b+c;}", {" "}, {}),
				testing::ElementsAre("workflow", "test", "{int d=a+b+c;}"));
	ASSERT_THAT(naobi::parser::split("workflow test{int d=a+b+c;} workflow test2{}", {";", "}"}, {}, naobi::parser::SPLIT_AFTER),
				testing::ElementsAre("workflow test{int d=a+b+c;}", " workflow test2{}"));
}

TEST(parser, removeExtraSpaces)
{
	std::string test = "      test test  test  test test   test  test   ";
	EXPECT_EQ(naobi::parser::removeExtraSpaces(test), " test test test test test test test ");

	test = "\ttest\t\ttest\t\t\ttest";
	EXPECT_EQ(naobi::parser::removeExtraSpaces(test), "\ttest\ttest\ttest");

	EXPECT_EQ(naobi::parser::removeExtraSpaces("test    sub   \"file test   test  xur\""), "test sub \"file test   test  xur\"");
	EXPECT_EQ(naobi::parser::removeExtraSpaces("test    sub   \"file test\t   test  xur\""), "test sub \"file test\t   test  xur\"");
}

TEST(parser, removeSym)
{
	std::string test = "some message for   testing ";
	EXPECT_EQ(naobi::parser::removeSym(test, ' '), "somemessagefortesting");
	EXPECT_EQ(naobi::parser::removeSym("some\nmessage\n\"\n text \n text \n\"", '\n'), "somemessage\"\n text \n text \n\"");
}

TEST(parser, placeAfter)
{
	EXPECT_EQ(naobi::parser::placeAfter("check\ntest\nkek\n", '\n', " -> "), "check\n -> test\n -> kek\n -> ");
	EXPECT_EQ(naobi::parser::placeAfter("check\ntest\ntest\"\ntest\n\"", '\n', ";"), "check\n;test\n;test\"\ntest\n\"");
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

TEST(parser, join)
{
	EXPECT_EQ(naobi::parser::join({"test","kek","alololo","hmm"}, " "), "test kek alololo hmm");
}