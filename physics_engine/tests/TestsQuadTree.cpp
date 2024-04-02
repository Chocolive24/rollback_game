#include "QuadTree.h"
#include "World.h"

#include "gtest/gtest.h"
#include "Random.h"

using namespace PhysicsEngine;
using namespace Math;

static HeapAllocator TestHeapAllocator;

struct BoundaryFixture : public ::testing::TestWithParam<RectangleF> {};

struct ColliderNumberFixture : public ::testing::TestWithParam<int> {};

INSTANTIATE_TEST_SUITE_P(QuadTree, ColliderNumberFixture, testing::Values(0, 1, 10, 100, 100, 321, 14, 5789));

TEST(QuadNode, DefaultConstructor)
{
    QuadNode node{TestHeapAllocator};

    EXPECT_EQ(node.Boundary.MinBound(), Vec2F::Zero());
    EXPECT_EQ(node.Boundary.MaxBound(), Vec2F::Zero());

    for (const auto& child : node.Children)
    {
        EXPECT_EQ(child, nullptr);
    }

    EXPECT_EQ(node.Colliders.size(), 0);
}

TEST(QuadTree, DefaultConstructor)
{
    QuadTree quadTree;

    EXPECT_EQ(quadTree.PossiblePairs().size(), 0);
}

void InitRecursive(const QuadNode& node)
{
    EXPECT_EQ(node.Colliders.capacity(), QuadNode::MaxColliderNbr + 1);

    if (node.Children[0] != nullptr)
    {
        for (const auto& child : node.Children)
        {
            InitRecursive(*child);
        }
    }
}

template<typename T>
constexpr T QuadCount(T depth)
{
    T result = 0;

    for (T i = 0; i <= depth; i++)
    {
        result += Math::Pow(QuadNode::BoundaryDivisionCount, i);
    }

    return result;
}

void TestInit(std::vector<QuadNode>& expectedNodes)
{
    expectedNodes.resize(QuadCount(QuadTree::MaxDepth()), QuadNode({ TestHeapAllocator }));

    for (auto& node : expectedNodes)
    {
        node.Colliders.reserve(QuadNode::MaxColliderNbr + 1);
    }
}

TEST(QuadTree, Init)
{
    QuadTree quadTree;
    quadTree.Init();

    InitRecursive(quadTree.RootNode());
}

void CheckRecursive(const QuadNode& node, const QuadNode& expectedNode)
{
    for (std::size_t i = 0; i < node.Colliders.size(); i++)
    {
        EXPECT_EQ(node.Colliders[i].Rectangle.MinBound(), expectedNode.Colliders[i].Rectangle.MinBound());
        EXPECT_EQ(node.Colliders[i].Rectangle.MaxBound(), expectedNode.Colliders[i].Rectangle.MaxBound());
        EXPECT_EQ(node.Colliders[i].ColRef, expectedNode.Colliders[i].ColRef);
    }

    if (node.Children[0] != nullptr)
    {
        for (std::size_t i = 0; i < node.Children.size(); i++)
        {
            CheckRecursive(*node.Children[i], *expectedNode.Children[i]);
        }
    }
}

void insertRecursive(std::vector<QuadNode>& expectedNodes,
                     QuadNode& node,
                     Math::RectangleF simplifiedShape,
                     ColliderRef colliderRef,
                     int depth,
                     int maxDepth) noexcept
{
    int nodeIndex = 0;

    // If the node doesn't have any children.
    if (node.Children[0] == nullptr)
    {
        // Add the simplified collider to the node.
        SimplifiedCollider simplifiedCollider = {colliderRef, simplifiedShape};
        node.Colliders.push_back(simplifiedCollider);

        // If the node has fewer colliders than the max number and the depth is not equal to the max depth.
        if (node.Colliders.size() > QuadNode::MaxColliderNbr && depth != maxDepth)
        {
            // Subdivide the node rectangle in 4 rectangle.
            const auto center = node.Boundary.Center();
            const auto halfSize = node.Boundary.HalfSize();

            const auto topMiddle = Math::Vec2F(center.X, center.Y + halfSize.Y);
            const auto topRightCorner = center + halfSize;
            const auto rightMiddle = Math::Vec2F(center.X + halfSize.X, center.Y);
            const auto bottomMiddle = Math::Vec2F(center.X, center.Y - halfSize.Y);
            const auto bottomLeftCorner = center - halfSize;
            const auto leftMiddle = Math::Vec2F(center.X - halfSize.X, center.Y);

            expectedNodes[nodeIndex].Boundary = Math::RectangleF(leftMiddle, topMiddle);
            expectedNodes[nodeIndex + 1].Boundary = Math::RectangleF(center, topRightCorner);
            expectedNodes[nodeIndex + 2].Boundary = Math::RectangleF(bottomLeftCorner, center);
            expectedNodes[nodeIndex + 3].Boundary = Math::RectangleF(bottomMiddle, rightMiddle);

            node.Children[0] = &expectedNodes[nodeIndex];
            node.Children[1] = &expectedNodes[nodeIndex + 1];
            node.Children[2] = &expectedNodes[nodeIndex + 2];
            node.Children[3] = &expectedNodes[nodeIndex + 3];

            nodeIndex += 4;

            std::vector<SimplifiedCollider> remainingColliders;

            for (const auto &col: node.Colliders)
            {
                int boundInterestCount = 0;
                QuadNode *intersectNode = nullptr;

                for (const auto &child: node.Children)
                {
                    if (Math::Intersect(child->Boundary, col.Rectangle))
                    {
                        boundInterestCount++;
                        intersectNode = child;
                    }
                }

                if (boundInterestCount == 1)
                {
                    insertRecursive(expectedNodes,
                                    *intersectNode,
                                    col.Rectangle,
                                    col.ColRef,
                                    depth + 1,
                                    maxDepth);
                }
                else
                {
                    remainingColliders.push_back(col);
                }
            }

            node.Colliders.clear();

            for (const auto& col : remainingColliders)
            {
                node.Colliders.push_back(col);
            }
        }
    }

    // If the node has children.
    else
    {
        int boundInterestCount = 0;
        QuadNode* intersectNode = nullptr;

        for (const auto& child : node.Children)
        {
            if (Math::Intersect(child->Boundary, simplifiedShape))
            {
                boundInterestCount++;
                intersectNode = child;
            }
        }

        if (boundInterestCount == 1)
        {
            insertRecursive(expectedNodes,
                            *intersectNode,
                            simplifiedShape,
                            colliderRef,
                            depth + 1,
                            maxDepth);
        }
        else
        {
            SimplifiedCollider simplifiedCollider = {colliderRef, simplifiedShape};
            node.Colliders.push_back(simplifiedCollider);
        }
    }
}

TEST_P(ColliderNumberFixture, Insert)
{
    World world;
    QuadTree quadTree;
    const auto colNbr = GetParam();

    std::vector<QuadNode> expectedNodes;

    world.Init();
    quadTree.Init();

    std::vector<Collider> colliders;
    colliders.reserve(colNbr);

    TestInit(expectedNodes);

    for (std::size_t i = 0; i < colNbr; i++)
    {
        Math::Vec2F rndScreenPos(Math::Random::Range(1.f, 7.f),
                                 Math::Random::Range(-1.f, -5.f));

        Math::Vec2F rndVelocity(Math::Random::Range(-2.f, 2.f),
                                Math::Random::Range(-2.f, 2.f));

        // Body.
        BodyRef bodyRef = world.CreateBody();
        auto& body = world.GetBody(bodyRef);
        body = PhysicsEngine::Body(rndScreenPos, rndVelocity, 1.f);

        // Collider.
        ColliderRef colRef = world.CreateCollider(bodyRef);
        auto& collider = world.GetCollider(colRef);
        collider.SetIsTrigger(true);
        collider.SetShape(Math::CircleF(Math::Vec2F::Zero(),
                                        Math::Random::Range(0.1f, 0.15f)));

        colliders.push_back(collider);
    }

    for (std::size_t i = 0; i < colliders.size(); i++)
    {
        ColliderRef colliderRef = {i, 0};
        const auto& col = world.GetCollider(colliderRef);

        const auto colShape = col.Shape();

        const auto circle = std::get<Math::CircleF>(colShape);
        const auto radius = circle.Radius();
        const auto simplifiedCircle = Math::RectangleF::FromCenter(
                world.GetBody(col.GetBodyRef()).Position(),
                Math::Vec2F(radius, radius));

        quadTree.Insert(simplifiedCircle, colliderRef);

        insertRecursive(expectedNodes,
                        expectedNodes[0],
                        simplifiedCircle,
                        colliderRef,
                        0,
                        QuadTree::MaxDepth());
    }

    CheckRecursive(quadTree.RootNode(), expectedNodes[0]);
}

void CalculatePairsInChildrenNodes(std::vector<ColliderPair>& possiblePairs,
    const QuadNode& node, 
    const SimplifiedCollider& simplCol) noexcept
{
    // For each colliders in the current node, compare it with the simplified collider from its parent node.
    for (const auto& nodeSimplCol : node.Colliders)
    {
        if (Math::Intersect(simplCol.Rectangle, nodeSimplCol.Rectangle))
        {
            possiblePairs.push_back(ColliderPair{ simplCol.ColRef, nodeSimplCol.ColRef });
        }
    }

    // If the current node has children, we need to compare the simplified collider from its parent node with its children.
    if (node.Children[0] != nullptr)
    {
        for (const auto& child : node.Children)
        {
            CalculatePairsInChildrenNodes(possiblePairs, *child, simplCol);
        }
    }
}

void CalculatePairsInNode(std::vector<ColliderPair>& possiblePairs, const QuadNode& node) noexcept
{
    for (std::size_t i = 0; i < node.Colliders.size(); i++)
    {
        const auto& simplColA = node.Colliders[i];

        for (std::size_t j = i + 1; j < node.Colliders.size(); j++)
        {
            const auto& simplColB = node.Colliders[j];

            if (Math::Intersect(simplColA.Rectangle, simplColB.Rectangle))
            {
                possiblePairs.push_back(ColliderPair{simplColA.ColRef, simplColB.ColRef});
            }
        }

        // If the node has children.
        if (node.Children[0] != nullptr)
        {
            for (const auto& child : node.Children)
            {
                CalculatePairsInChildrenNodes(possiblePairs, *child, simplColA);
            }
        }
    }

    // If the node has children.
    if (node.Children[0] != nullptr)
    {
        for (const auto& child : node.Children)
        {
            CalculatePairsInNode(possiblePairs, *child);
        }
    }
}

TEST_P(ColliderNumberFixture, CalculatePossiblePairs)
{
    World world;
    QuadTree quadTree;

    world.Init();
    quadTree.Init();

    const auto colNbr = GetParam();

    std::vector<Collider> colliders;
    colliders.reserve(colNbr);

    std::vector<ColliderPair> possiblePairs;

    for (std::size_t i = 0; i < colNbr; i++)
    {
        Math::Vec2F rndScreenPos(Math::Random::Range(1.f, 7.f),
                                 Math::Random::Range(-1.f, -5.f));

        Math::Vec2F rndVelocity(Math::Random::Range(-2.f, 2.f),
                                Math::Random::Range(-2.f, 2.f));

        // Body.
        BodyRef bodyRef = world.CreateBody();
        auto& body = world.GetBody(bodyRef);
        body = PhysicsEngine::Body(rndScreenPos, rndVelocity, 1.f);

        // Collider.
        ColliderRef colRef = world.CreateCollider(bodyRef);
        auto& collider = world.GetCollider(colRef);
        collider.SetIsTrigger(true);
        collider.SetShape(Math::CircleF(Math::Vec2F::Zero(),
                                        Math::Random::Range(0.1f, 0.15f)));

        colliders.push_back(collider);
    }

    for (std::size_t i = 0; i < colliders.size(); i++)
    {
        ColliderRef colliderRef = {i, 0};
        const auto& col = world.GetCollider(colliderRef);

        const auto colShape = col.Shape();

        const auto circle = std::get<Math::CircleF>(colShape);
        const auto radius = circle.Radius();
        const auto simplifiedCircle = Math::RectangleF::FromCenter(
                world.GetBody(col.GetBodyRef()).Position(),
                Math::Vec2F(radius, radius));

        quadTree.Insert(simplifiedCircle, colliderRef);
    }

    quadTree.CalculatePossiblePairs();
    CalculatePairsInNode(possiblePairs, quadTree.RootNode());

    const auto& quadPossiblePairs = quadTree.PossiblePairs();

    for (std::size_t  i = 0; i < quadPossiblePairs.size(); i++)
    {
        EXPECT_EQ(quadPossiblePairs[i], possiblePairs[i]);
    }
}