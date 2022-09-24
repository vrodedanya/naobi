#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <naobi/compiler/code_generator.hpp>

using namespace naobi;

TEST(generator, methodToFunction)
{
	EXPECT_EQ(code_generator::methodToFunction("var.print()"), "print(var)");
	EXPECT_EQ(code_generator::methodToFunction("var.print(10)"), "print(var, 10)");
	EXPECT_EQ(code_generator::methodToFunction("var.type().print()"), "print(var.type())");
	EXPECT_EQ(code_generator::methodToFunction("var.print(inner.add(5))"), "print(var, inner.add(5))");
	EXPECT_EQ(code_generator::methodToFunction("var.print(test(6, 45, third: 134))"), "print(var, test(6, 45, third: 134))");
	EXPECT_EQ(code_generator::methodToFunction("var.sub(inner.add(5)).print(\"check\")"), "print(var.sub(inner.add(5)), \"check\")");
}