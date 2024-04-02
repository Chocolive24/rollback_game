/**
 * @headerfile QuadTree.h
 * This header file defines the various data required to calculate the quad-tree which is an 
 * algorithm that divides the physical world into sub-spaces to facilitate comparison between physical objects.
 *
 * @author Olivier Pachoud
 */

#pragma once

#include "Allocator.h"
#include "Collider.h"
#include "UniquePtr.h"

namespace PhysicsEngine
{
    /**
     * @brief SimplifiedCollider is a struct that stores the data of a collider in a simplified way (aka it stores
     * its collider reference in the world and its shape in a rectangle form).
     */
    struct SimplifiedCollider
    {
        ColliderRef ColRef{0, 0};
        Math::RectangleF Rectangle{Math::Vec2F::Zero(), Math::Vec2F::Zero()};
    };

    /**
     * @brief QuadNode is a struct representing a node in a quad-tree data structure used for spatial 
     partitioning in a 2D space.
     */
    struct QuadNode
    {
        /**
         * @brief MaxColliderNbr is the maximum number of colliders that the node can stores before a subdivision
         * of the world.
         */
        static constexpr int MaxColliderNbr = 8;

        /**
         * @brief BoundaryDivisionCount is the number of space boundary subdivision for the node.
         */
        static constexpr int BoundaryDivisionCount = 4;

        Math::RectangleF Boundary{Math::Vec2F::Zero(), Math::Vec2F::Zero()};
        std::array<QuadNode*, BoundaryDivisionCount> Children{ nullptr, nullptr, nullptr, nullptr };
        AllocVector<SimplifiedCollider> Colliders;

        explicit QuadNode(Allocator& allocator) noexcept : 
            Colliders{ StandardAllocator<SimplifiedCollider>{allocator} } {}
    };

    /**
     * @brief QuadTree is a class that represents a quad-tree used for spatial partitioning of the world space.
     */
    class QuadTree
    {
    private:
        HeapAllocator _heapAllocator;

        AllocVector<QuadNode> _nodes{ StandardAllocator<QuadNode> {_heapAllocator} };
        AllocVector<ColliderPair> _possiblePairs{ StandardAllocator<ColliderPair> {_heapAllocator} };

        int _nodeIndex = 1;

        /**
         * @brief MaxDepth is the maximum depth of the quad-tree recursive space subdivision.
         */
        static constexpr int _maxDepth = 5;

        /**
         * @brief PossiblePairReserveFactor is the factor to mutliply with the total number of node in 
         * the quad-tree to reserve this capacity in the _possiblePairs vector. 
         * This factor totally arbitrary.
         */
        static constexpr float _possiblePairReserveFactor = 3.f;

        /**
         * @brief insertInNode is a method that insert a collider in the node given in parameter
         * (in its simplified shape) in the given node in parameter.
         * @param node The node in which the collider must be inserted.
         * @param simplifiedShape The simplified shape of the collider (aka its shape in rectangle).
         * @param colliderRef The collider reference in the world.
         * @param depth The depth in which the node is.
         */
        void insertInNode(QuadNode& node,
                          Math::RectangleF simplifiedShape,
                          ColliderRef colliderRef,
                          int depth) noexcept;

        /**
         * @brief calculateNodePossiblePairs is a method that calculates the possible pair of collider
         * in the node given in parameter.
         * @param node
         */
        void calculateNodePossiblePairs(const QuadNode& node) noexcept;

        /**
         * @brief calculateNodePossiblePairs is a method that calculates the possible pair of collider
         * in the node given in parameter.
         * @param node
         */
        void calculateChildrenNodePossiblePairs(const QuadNode& node, SimplifiedCollider simplCol) noexcept;

    public:
        QuadTree() noexcept = default;

        /**
         * @brief Init is a method that initialize the quad-tree by allocating the needed amount of memory to
         * store the quad-nodes.
         */
        void Init() noexcept;

        /**
         * @brief Insert is a method that insert a collider (in its simplified shape) in the quad-tree from its
         * root node.
         * @param simplifiedShape The simplified shape of the collider (aka its shape in rectangle).
         * @param colliderRef The collider reference in the world.
         */
        void Insert(Math::RectangleF simplifiedShape, ColliderRef colliderRef) noexcept;

        /**
         * @brief CalculatePossiblePairs is a method which calculates the potential pairs of colliders in each
         * tree node that could touch each other by comparing their simplified shapes.
         */
        void CalculatePossiblePairs() noexcept;

        /**
         * @brief Clear is a method that removes all colliders from each node and removes possible pairs.
         */
        void Clear() noexcept;

        /**
         * @brief Deinit is a method that deinitialize the quad-tree by removing all data from it.
         */
        void Deinit() noexcept;

        /**
         * @brief RootNode is a method that gives the root node of the quad-tree (aka its first node).
         * @return The root node of the quad-tree (aka its first node).
         */
        [[nodiscard]] const QuadNode& RootNode() const noexcept { return _nodes[0]; }

        /**
         * @brief SetRoodNodeBoundary is a method that sets the boundary of the root node (aka the first space
         * subdivision) to the new one given in parameter.
         * @param boundary The boundary of the first space subdivision.
         */
        void SetRootNodeBoundary(const Math::RectangleF boundary) noexcept
        {
            _nodes[0].Boundary = boundary;
        };

        /**
         * @brief PossiblePairs is a method that gives the possible pairs of collider whose simplified shapes
         * touch each other.
         * @return The possible pairs of collider whose simplified shapes touch each other.
         */
        [[nodiscard]] const AllocVector<ColliderPair>& PossiblePairs() const noexcept { return _possiblePairs; }

        /**
         * @brief MaxDepth is a method that gives the maximum depth of the quad-tree recursive space subdivision.
         * @return The maximum depth of the quad-tree recursive space subdivision.
         */
        [[nodiscard]] static constexpr int MaxDepth() noexcept { return _maxDepth; }
    };
}