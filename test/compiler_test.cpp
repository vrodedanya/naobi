#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <naobi/compiler/compiler.hpp>

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
