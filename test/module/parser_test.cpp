#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <naobi/utils/parser.hpp>
#include <set>

using namespace naobi::parser;

TEST(parser, split)
{
	ASSERT_THAT(split("a;b;c;d", isAnyOf(";")), testing::ElementsAre("a", "b", "c", "d"));
	ASSERT_THAT(split("a+b-c*d/e 234", isAnyOf(" "), isAnyOf("+-*")),
				testing::ElementsAre("a", "+", "b", "-", "c", "*", "d/e", "234"));
	ASSERT_THAT(split("import standard;"
									 "workflow main"
									 "{"
									 "println(6+6);"
									 "}"
									 "workflow test"
									 "{"
									 "println(\"Hello\");"
									 "}"
									 , isAnyOf(";}"),
									 {}, {{'{', '}'}}),
				testing::ElementsAre("import standard", "workflow main{println(6+6);}", "workflow test{println(\"Hello\");}"));
	ASSERT_THAT(split("println(\"Hello\");"
									 "integer a = b + c;"
									 "string res = \"Hello\";"
									 "testing();",
					  				isAnyOf(";"),{},{{'(',')'},{'"', '"'}}),
				testing::ElementsAre("println(\"Hello\")", "integer a = b + c", "string res = \"Hello\"", "testing()"));
	ASSERT_THAT(split("println(\"Ssss\");", isAnyOf("};"), {}, {{'"','"'}}),
				testing::ElementsAre(("println(\"Ssss\")")));
}

TEST(parser, removeExtraSpaces)
{
	std::string test = "      test test  test  test test   test  test   ";
	EXPECT_EQ(removeExtraSpaces(test), " test test test test test test test ");

	test = "\ttest\t\ttest\t\t\ttest";
	EXPECT_EQ(removeExtraSpaces(test), "\ttest\ttest\ttest");

	EXPECT_EQ(removeExtraSpaces("test    sub   \"file test   test  xur\""), "test sub \"file test   test  xur\"");
	EXPECT_EQ(removeExtraSpaces("test    sub   \"file test\t   test  xur\""), "test sub \"file test\t   test  xur\"");
}

TEST(parser, removeSym)
{
	std::string test = "some message for   testing ";
	EXPECT_EQ(removeSym(test, ' '), "somemessagefortesting");
	EXPECT_EQ(removeSym("some\nmessage\n\"\n text \n text \n\"", '\n'), "somemessage\"\n text \n text \n\"");
}

TEST(parser, placeAfter)
{
	EXPECT_EQ(placeAfter("check\ntest\nkek\n", '\n', " -> "), "check\n -> test\n -> kek\n -> ");
	EXPECT_EQ(placeAfter("check\ntest\ntest\"\ntest\n\"", '\n', ";"), "check\n;test\n;test\"\ntest\n\"");
}

TEST(parser, dirName)
{
	EXPECT_EQ(dirName("test/file/name.txt"), "test/file");
	EXPECT_EQ(dirName("test\\file\\name.txt"), "test\\file");
	EXPECT_EQ(dirName("../test/check/name.txt"), "../test/check");
	EXPECT_EQ(dirName("../keep/get/check/"), "../keep/get/check");
}

TEST(parser, fileName)
{
	EXPECT_EQ(fileName("test/file/name.txt"), "name.txt");
	EXPECT_EQ(fileName("test\\file\\name.txt"), "name.txt");
	EXPECT_EQ(fileName("../test/check/name.txt"), "name.txt");
	EXPECT_EQ(fileName("../keep/get/check/"), "");
}

TEST(parser, join)
{
	EXPECT_EQ(join({"test","kek","alololo","hmm"}, " "), "test kek alololo hmm");
}

TEST(parser, removeFirstSym)
{
	EXPECT_EQ(removeFirstSym(" testing", ' '), "testing");
}