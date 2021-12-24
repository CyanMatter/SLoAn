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
		
		TEST_METHOD(ConvertAndCountZeroA)
		{
			vector<unsigned long long> bitboards;
			bitboards = toBitboards("bridge");
			Assert::AreEqual(0, collectCount(96 + 1, bitboards, 2));
		}
		TEST_METHOD(ConvertAndCountThreeA)
		{
			vector<unsigned long long> bitboards;
			bitboards = toBitboards("aaa");
			Assert::AreEqual(3, collectCount(96 + 1, bitboards, 2));
		}

		TEST_METHOD(ConvertAndCountElevenA)
		{
			vector<unsigned long long> bitboards;
			bitboards = toBitboards("aaaaaaaaaaa");
			Assert::AreEqual(11, collectCount(96 + 1, bitboards, 2));
		}

		TEST_METHOD(ConvertAndCountThreeP)
		{
			vector<unsigned long long> bitboards;
			bitboards = toBitboards("ppp");
			Assert::AreEqual(3, collectCount(96 + 16, bitboards, 2));
		}

		TEST_METHOD(ConvertAndCountOneZ)
		{
			vector<unsigned long long> bitboards;
			bitboards = toBitboards("z");
			Assert::AreEqual(1, collectCount(96 + 26, bitboards, 2));
		}

		TEST_METHOD(ConvertAndCountFourZPizzazz)
		{
			vector<unsigned long long> bitboards;
			bitboards = toBitboards("pizzazz");
			Assert::AreEqual(4, collectCount(96 + 26, bitboards, 2));
		}
	};
}
