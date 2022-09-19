#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <naobi/compiler/code_generator.hpp>

using namespace naobi;

TEST(generator, methodToFunction)
{
	EXPECT_EQ(code_generator::methodToFunction("var.print()"), "print(var)");
	EXPECT_EQ(code_generator::methodToFunction("var.print(10)"), "print(var, 10)");
	EXPECT_EQ(code_generator::methodToFunction("var.type().print()"), "print(type(var))");
	EXPECT_EQ(code_generator::methodToFunction("var.print(inner.add(5))"), "print(var, add(inner, 5))");
	EXPECT_EQ(code_generator::methodToFunction("var.sub(inner.add(5)).print(\"check\")"), "print(sub(var, add(inner, 5)), \"check\")");
}