#include "TestUtils.h"
#include "data_structures/KDtree.h"
#include "geometry/BBox.h"

using namespace gtrace;

struct Line;
struct LenghtMeasurable
{
    virtual ~LenghtMeasurable() = default;
    virtual int length() const = 0;
};

struct TreeDumper
{
    virtual ~TreeDumper() = default;
    virtual void dumpTree(std::vector<Line>* dump) const = 0;
};

struct TreeDepth
{
    virtual ~TreeDepth() = default;
    virtual size_t depth() const = 0;
};

struct LineInterface : Bounded<Line>, LenghtMeasurable, TreeDumper, TreeDepth
{
};

struct Line : LineInterface
{
    Line() : start(0), end(0) {}
    Line(int start, int end) : start(start), end(end) {}

    static Line bound(const Line& a, const Line& b) { return {std::min(a.start, b.start), std::max(a.end, b.end)}; }
    Line bound() const override { return *this; }
    int length() const override { return end - start; }
    void dumpTree(std::vector<Line>* dump) const override { dump->emplace_back(*this); }
    size_t depth() const override { return 1; }

    float center(int axis) const
    {
        assert(axis == 0);
        return (start + end) * 0.5f;
    }

    bool operator==(const Line& rhs) const { return start == rhs.start && end == rhs.end; }

    int start, end;
};

struct LineNode : KDTreeNode<Line, LineInterface>
{
    using KDTreeLineNode = KDTreeNode<Line, LineInterface>;
    LineNode(BoundedPtr&& left, BoundedPtr&& right) : KDTreeLineNode(move(left), move(right)) {}
    int length() const override { return left->length() + right->length(); }
    void dumpTree(std::vector<Line>* dump) const override
    {
        dump->emplace_back(bounding);
        left->dumpTree(dump);
        right->dumpTree(dump);
    }
    size_t depth() const { return 1 + std::max(left->depth(), right->depth()); }
};

class KDTree1DTest : public Test
{
public:
    void SetUp() override
    {
        MemoryTracker::reset();
        tree = std::make_unique<KDTree<Line, LineNode, 1>>();
    }

    void TearDown() override
    {
        tree.reset();
        checkMemoryLeaks();
    }

    std::unique_ptr<KDTree<Line, LineNode, 1>> tree;
};

TEST_F(KDTree1DTest, EmptyOnCreation)
{
    ASSERT_TRUE(tree->isEmpty());
    ASSERT_FALSE(tree->isBuilt());
}

TEST_F(KDTree1DTest, Build)
{
    tree->insert(std::make_unique<Line>(0, 1));
    tree->insert(std::make_unique<Line>(1, 2));
    tree->insert(std::make_unique<Line>(2, 3));
    tree->insert(std::make_unique<Line>(3, 4));
    ASSERT_FALSE(tree->isEmpty());
    ASSERT_FALSE(tree->isBuilt());

    tree->build();
    ASSERT_FALSE(tree->isEmpty());
    ASSERT_TRUE(tree->isBuilt());

    const auto* l = tree->data();
    ASSERT_EQ(4, l->length());

    std::vector<Line> expectedTreeDump = {Line(0, 4), Line(0, 2), Line(0, 1), Line(1, 2),
                                          Line(2, 4), Line(2, 3), Line(3, 4)};

    std::vector<Line> treeDump;
    l->dumpTree(&treeDump);
    ASSERT_EQ(expectedTreeDump, treeDump);

    ASSERT_EQ(3, l->depth());
}

TEST_F(KDTree1DTest, Balanced)
{
    const auto depth = 10;
    const auto numElements = 1 << 10;
    for (auto i = 0; i < numElements; ++i)
        tree->insert(std::make_unique<Line>(i, i + 1));

    tree->build();

    const auto* l = tree->data();
    ASSERT_EQ(numElements, l->length());
    ASSERT_EQ(depth + 1, l->depth());
}

struct Point;
struct PointsInRange
{
    virtual ~PointsInRange() = default;
    virtual void pointsInRange(const Point& point, float range, int axis, std::vector<Point>* points) const = 0;
};

static float distance(const Vector3f& a, const Vector3f& b)
{
    return (a - b).length();
}

struct PointInterface : Bounded<BBox>, PointsInRange
{
};

struct Point : PointInterface
{
    Vector3f p;

    explicit Point(const Vector3f& p) : p(p) {}
    BBox bound() const override { return BBox(p, p); }
    void pointsInRange(const Point& point, float range, int axis, std::vector<Point>* points) const override
    {
        if (range >= distance(p, point.p))
            points->emplace_back(*this);
    }

    bool operator==(const Point& rhs) const { return p == rhs.p; }
};

struct PointNode : KDTreeNode<BBox, PointInterface>
{
    using KDTreePointNode = KDTreeNode<BBox, PointInterface>;

    static int skipped;
    PointNode(BoundedPtr&& left, BoundedPtr&& right) : KDTreePointNode(move(left), move(right)) {}

    void pointsInRange(const Point& point, float range, int axis, std::vector<Point>* points) const override
    {
        const float start = bounding.min()[axis];
        const float end = bounding.max()[axis];

        const float pos = point.p[axis];
        const float d1 = sqrtf(fabsf(start * start - pos * pos));
        const float d2 = sqrtf(fabsf(end * end - pos * pos));

        if (start > pos && d1 > range)
        {
            ++skipped;
            return;
        }

        if (pos > end && d2 > range)
        {
            ++skipped;
            return;
        }

        auto nextAxis = (axis + 1) % 3;
        left->pointsInRange(point, range, nextAxis, points);
        right->pointsInRange(point, range, nextAxis, points);
    }
};

int PointNode::skipped = 0;

class KDTreePointTest : public Test
{
public:
    void SetUp() override
    {
        MemoryTracker::reset();
        tree = std::make_unique<KDTree<BBox, PointNode>>();
        PointNode::skipped = 0;
    }

    void TearDown() override
    {
        tree.reset();
        checkMemoryLeaks();
    }

    std::unique_ptr<KDTree<BBox, PointNode>> tree;
};

TEST_F(KDTreePointTest, nearestPointFixedPoints)
{
    const auto start = -100;
    const auto end = 100;

    for (auto i = start; i <= end; ++i)
        tree->insert(std::make_unique<Point>(Vector3f(i)));
    tree->build();

    const float range = 10;
    std::vector<Point> result;
    tree->data()->pointsInRange(Point(Vector3f(0.f)), range, 0, &result);

    const int resStart = static_cast<int>(sqrtf(range * range / 3));
    std::vector<Point> expectedResult;
    for (int i = -resStart; i <= resStart; ++i)
        expectedResult.emplace_back(Point(Vector3f(i)));

    ASSERT_EQ(expectedResult, result);
    ASSERT_GT(PointNode::skipped, 0);
}
