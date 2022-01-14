#include "pch.h"
#include "CppUnitTest.h"
#include <vector>
#include "../anagram/anagram.cpp"
using std::vector;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{
	TEST_CLASS(UnitTests)
	{
	public:
		/* example test case
		TEST_METHOD(ConvertAndCountZeroA)
		{
			vector<unsigned long long> bitboards;
			bitboards = toBitboards("bridge");
			Assert::AreEqual(0, collectCount(96 + 1, bitboards, 2));
		}
		*/
	};
}
