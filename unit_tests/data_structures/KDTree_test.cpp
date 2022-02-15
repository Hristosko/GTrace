#include "TestUtils.h"
#include "data_structures/KDTree.h"

using namespace gtrace;

struct Line : Bounded<Line>
{
    Line(int start, int end) : start(start), end(end) {}

    Line bound() const override { return *this; }
    static Line bound(const Line& a, const Line& b) { return {std::min(a.start, b.start), std::max(a.end, b.end)}; }

    int start, end;
};

class KDTree1DTest : public Test
{
public:
    KDTree<Line, 1> tree;
};

TEST_F(KDTree1DTest, EmptyOnCreation)
{
    ASSERT_TRUE(tree.isEmpty());
}
