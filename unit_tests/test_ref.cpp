#include "gtest/gtest.h"
#include "Ref.h"

using namespace gtrace;

static int cnt;
struct RefTester : ReferenceCounted {
	RefTester() { ++cnt; }
	~RefTester() { --cnt; }
};

TEST(RefTester, is_deleted) {
	cnt = 0;
	{
		Ref<RefTester> ref(new RefTester);
		EXPECT_EQ(1, cnt);
	}
	EXPECT_EQ(0, cnt);
}

TEST(RefTester, copy) {
	cnt = 0;
	{
		Ref<RefTester> ref(new RefTester);
		Ref<RefTester> copy(ref);
	}
	EXPECT_EQ(0, cnt);
}

TEST(RefTester, move) {
	cnt = 0;
	{
		Ref<RefTester> ref(new RefTester);
		Ref<RefTester> mv(std::move(ref));
		EXPECT_EQ(1, cnt);
	}
	EXPECT_EQ(0, cnt);
}

TEST(RefTester, op_copy) {
	cnt = 0;
	{
		Ref<RefTester> ref(new RefTester);
		Ref<RefTester> copy = ref;
	}
	EXPECT_EQ(0, cnt);
}

TEST(RefTester, op_move) {
	cnt = 0;
	{
		Ref<RefTester> ref(new RefTester);
		Ref<RefTester> mv = std::move(ref);
		EXPECT_EQ(1, cnt);
	}
	EXPECT_EQ(0, cnt);
}