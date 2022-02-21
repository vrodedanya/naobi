#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <naobi/compiler/compiler.hpp>
#include <naobi/utils/parser.hpp>

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
	auto temp = naobi::parser::removeSym(naobi::parser::removeExtraSpaces(test), '\n');
	auto lines = naobi::parser::split(temp, ";", false);
	ASSERT_THAT(naobi::compiler::collectModules(lines), testing::ElementsAre("lib.file2.naobi", "lib.file3.naobi", "lib.file4.naobi"));
}
