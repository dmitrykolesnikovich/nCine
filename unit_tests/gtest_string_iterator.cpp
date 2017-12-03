#include "gtest_string.h"

namespace {

class StringIteratorTest : public ::testing::Test
{
  public:
	StringIteratorTest() : string_(Capacity) { }

  protected:
	void SetUp() override { string_ = "String1String2"; }

	nctl::String string_;
};

TEST_F(StringIteratorTest, ForLoopIteration)
{
	unsigned int n = 0;

	printf("Iterating through string characters with for loop:");
	for (nctl::String::ConstIterator i = string_.begin(); i != string_.end(); ++i)
	{
		printf(" %c", *i);
		ASSERT_EQ(*i, string_[n++]);
	}
	printf("\n");
}

TEST_F(StringIteratorTest, ForLoopEmptyIteration)
{
	nctl::String newString(Capacity);

	printf("Iterating over an empty string with for loop:\n");
	for (nctl::String::ConstIterator i = newString.begin(); i != newString.end(); ++i)
		ASSERT_TRUE(false); // should never reach this point
	printf("\n");
}

TEST_F(StringIteratorTest, ReverseForLoopIteration)
{
	unsigned int n = string_.length() - 1;

	printf("Iterating through string characters with for loop (reverse):");
	for (nctl::String::ConstIterator r = string_.rBegin(); r != string_.rEnd(); --r)
	{
		printf(" %c", *r);
		ASSERT_EQ(*r, string_[n--]);
	}
	printf("\n");
}

TEST_F(StringIteratorTest, ReverseForLoopEmptyIteration)
{
	nctl::String newString(Capacity);

	printf("Iterating over an empty string with for loop (reverse):\n");
	for (nctl::String::ConstIterator r = newString.rBegin(); r != newString.rEnd(); --r)
		ASSERT_TRUE(false); // should never reach this point
	printf("\n");
}

TEST_F(StringIteratorTest, WhileLoopIteration)
{
	unsigned int n = 0;

	printf("Iterating through string characters with while loop:");
	nctl::String::ConstIterator i = string_.begin();
	while (i != string_.end())
	{
		printf(" %c", *i);
		ASSERT_EQ(*i, string_[n]);
		++i; ++n;
	}
	printf("\n");
}

TEST_F(StringIteratorTest, WhileLoopEmptyIteration)
{
	nctl::String newString(Capacity);

	printf("Iterating over an empty string with while loop:\n");
	nctl::String::ConstIterator i = newString.begin();
	while (i != newString.end())
	{
		ASSERT_TRUE(false); // should never reach this point
		++i;
	}
	printf("\n");
}

TEST_F(StringIteratorTest, ReverseWhileLoopIteration)
{
	unsigned int n = string_.length() - 1;

	printf("Iterating through string characters with while loop (reverse):");
	nctl::String::ConstIterator r = string_.rBegin();
	while (r != string_.rEnd())
	{
		printf(" %c", *r);
		ASSERT_EQ(*r, string_[n]);
		--r; --n;
	}
	printf("\n");
}

TEST_F(StringIteratorTest, ReverseWhileLoopEmptyIteration)
{
	nctl::String newString(Capacity);

	printf("Iterating over an empty string with while loop (reverse):\n");
	nctl::String::ConstIterator r = newString.rBegin();
	while (r != newString.rEnd())
	{
		ASSERT_TRUE(false); // should never reach this point
		--r;
	}
	printf("\n");
}

TEST_F(StringIteratorTest, AddIndexToIterator)
{
	nctl::String::ConstIterator it = string_.begin();

	printf("Accessing characters with an iterator and a positive index\n");
	for (int i = 0; i < static_cast<int>(string_.length()); i++)
	{
		printf(" %c", *(it + i));
		ASSERT_EQ(*(it + i), string_[i]);
	}
	printf("\n");
}

TEST_F(StringIteratorTest, AddIndexToIteratorInPlace)
{
	printf("Accessing characters with an iterator and a positive index\n");
	for (int i = 0; i < static_cast<int>(string_.length()); i++)
	{
		nctl::String::ConstIterator it = string_.begin();

		it += i;
		printf(" %c", *it);
		ASSERT_EQ(*it, string_[i]);
	}
	printf("\n");
}

TEST_F(StringIteratorTest, SubtractIndexToIterator)
{
	nctl::String::ConstIterator it = string_.end();

	printf("Accessing characters with an iterator and a negative index\n");
	for (int i = 1; i <= static_cast<int>(string_.length()); i++)
	{
		printf(" %c", *(it - i));
		ASSERT_EQ(*(it - i), string_[string_.length() - i]);
	}
	printf("\n");
}

TEST_F(StringIteratorTest, SubtractIndexToIteratorInPlace)
{
	printf("Accessing characters with an iterator and a negative index\n");
	for (unsigned int i = 1; i <= static_cast<int>(string_.length()); i++)
	{
		nctl::String::ConstIterator it = string_.end();

		it -= i;
		printf(" %c", *it);
		ASSERT_EQ(*it, string_[string_.length() - i]);
	}
	printf("\n");
}

TEST_F(StringIteratorTest, SubtractIterators)
{
	const int diff = string_.end() - string_.begin();
	printf("Difference between end and begin iterators: %d\n", diff);
	ASSERT_EQ(diff, string_.length());
}

TEST_F(StringIteratorTest, SubscriptOperator)
{
	nctl::String::ConstIterator it = string_.begin();

	printf("Accessing characters with the iterator subscript operator\n");
	for (int i = 0; i < static_cast<int>(string_.length()); i++)
	{
		printf(" %c", it[i]);
		ASSERT_EQ(it[i], string_[i]);
	}
	printf("\n");
}

}
