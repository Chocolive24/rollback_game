#pragma once

#include "Vec2.h"

#include <vector>

namespace Math
{
    enum class ShapeType
    {
        Circle, Rectangle, Polygon, None
    };

    template <typename T>
    class Circle
    {
    public:
        /**
         * @brief Construct a new Circle object
         * @param center Center of the circle, also the position of the circle
         * @param radius Radius of the circle, if negative, it will be converted to its positive value
         */
        constexpr Circle(Vec2<T> center, T radius) noexcept : _center(center), _radius(Math::Abs(radius)) {}
        /**
         * @brief Construct a new Circle object with a center in (0, 0)
         * @param radius Radius of the circle, if negative, it will be converted to its positive value
         */
        constexpr explicit Circle(T radius) noexcept : _center(Vec2<T>::Zero()), _radius(Math::Abs(radius)) {}

    private:
        Vec2<T> _center = Vec2<T>::Zero();
        T _radius = 0;

    public:
        [[nodiscard]] constexpr Vec2<T> Center() const noexcept { return _center; }
        [[nodiscard]] constexpr T Radius() const noexcept { return _radius; }

        void SetCenter(Vec2<T> center) noexcept { _center = center; }
        void SetRadius(T radius) noexcept { _radius = Math::Abs(radius); }

        /**
         * @brief Check if the circle contains a point
         * @param point the point to check
         * @return true if the point is inside the circle, false otherwise
         */
        [[nodiscard]] bool Contains(Vec2<T> point) const
        {
            return (_center - point).SquareLength() <= _radius * _radius;
        }

		[[nodiscard]] constexpr Circle<T> operator+(const Vec2<T>& vec) const noexcept
		{
			return Circle<T>(_center + vec, _radius);
		}

    };

    using CircleF = Circle<float>;
    using CircleI = Circle<int>;

    template <typename T>
    class Rectangle
    {
    public:
        /**
         * @brief Construct a new Rectangle object
         * @param position the position of the rectangle
         * @param size the size of the rectangle
         */
        constexpr Rectangle(Vec2<T> minBound, Vec2<T> maxBound) noexcept : _minBound(minBound), _maxBound(maxBound) {}

    private:
        Vec2<T> _minBound = Vec2<T>::Zero();
        Vec2<T> _maxBound = Vec2<T>::Zero();

    public:
        [[nodiscard]] constexpr Vec2<T> MinBound() const noexcept { return _minBound; }
        [[nodiscard]] constexpr Vec2<T> MaxBound() const noexcept { return _maxBound; }

        void SetMinBound(Vec2<T> minBound) noexcept { _minBound = minBound; }
        void SetMaxBound(Vec2<T> maxBound) noexcept { _maxBound = maxBound; }

        /**
         * @brief Check if the rectangle contains a point
         * @param point the point to check
         * @return true if the point is inside the rectangle, false otherwise
         */
        [[nodiscard]] bool Contains(Vec2<T> point) const
        {
            if (_maxBound.X < point.X || _minBound.X > point.X) return false;
            if (_maxBound.Y < point.Y || _minBound.Y > point.Y) return false;

            return true;
        }

        [[nodiscard]] constexpr Vec2<T> Center() const noexcept
        {
            return (_minBound + _maxBound) / 2;
        }

        [[nodiscard]] constexpr Vec2<T> Size() const noexcept
        {
            return _maxBound - _minBound;
        }

        [[nodiscard]] constexpr Vec2<T> HalfSize() const noexcept
        {
            return (_maxBound - _minBound) / 2;
        }

		[[nodiscard]] constexpr Rectangle<T> operator+(const Vec2<T>& vec) const noexcept
	    {
		    return Rectangle<T>(_minBound + vec, _maxBound + vec);
	    }

        [[nodiscard]] static constexpr Rectangle<T> FromCenter(Math::Vec2<T> center, Math::Vec2<T> halfSize) noexcept
        {
            return Rectangle<T>(center - halfSize, center + halfSize);
        }
    };

    using RectangleF = Rectangle<float>;
    using RectangleI = Rectangle<int>;

    template <typename T>
    class Polygon
    {
    public:
        /**
         * @brief Construct a new Polygon object
         * @param vertices the vertices of the polygon
         */
        constexpr explicit Polygon(std::vector<Vec2<T>> vertices) noexcept : _vertices(vertices) {}

    private:
        std::vector<Vec2<T>> _vertices;

    public:
        [[nodiscard]] constexpr std::vector<Vec2<T>> Vertices() const noexcept { return _vertices; }
        [[nodiscard]] constexpr int VerticesCount() const noexcept { return _vertices.size(); }

        void SetVertices(std::vector<Vec2<T>> vertices) noexcept { _vertices = vertices; }

        [[nodiscard]] constexpr Vec2<T> Center() const noexcept
        {
            Vec2<T> center = Vec2<T>::Zero();

            for (const auto& vertex : _vertices)
            {
                center += vertex;
            }

            return center / _vertices.size();
        }

        [[nodiscard]] constexpr Vec2<T> Size() const noexcept
        {
            Vec2<T> minBound = Vec2<T>::Zero();
            Vec2<T> maxBound = Vec2<T>::Zero();

            for (const auto& vertex : _vertices)
            {
                minBound.X = Math::Min(minBound.X, vertex.X);
                minBound.Y = Math::Min(minBound.Y, vertex.Y);

                maxBound.X = Math::Max(maxBound.X, vertex.X);
                maxBound.Y = Math::Max(maxBound.Y, vertex.Y);
            }

            return maxBound - minBound;
        }
		
		[[nodiscard]] constexpr Polygon<T> operator+(const Vec2<T>& vec) const noexcept
	    {
		    std::vector<Vec2<T>> vertices = _vertices;

		    for (auto& vertex : vertices)
		    {
			    vertex += vec;
		    }

		    return Polygon<T>(vertices);
	    }
    };

    using PolygonF = Polygon<float>;
    using PolygonI = Polygon<int>;

    // Intersect functions

    template<typename T>
    [[nodiscard]] constexpr bool Intersect(const Circle<T> circle1, const Circle<T> circle2) noexcept
    {
        const T radiusSum = (circle1.Radius() + circle2.Radius()) * (circle1.Radius() + circle2.Radius());
        const T distanceBetweenCenters = (circle1.Center() - circle2.Center()).SquareLength();

        return distanceBetweenCenters <= radiusSum;
    }

    template<typename T>
    [[nodiscard]] constexpr bool Intersect(const Rectangle<T> rectangle1, const Rectangle<T> rectangle2) noexcept
    {
        if (rectangle1.MaxBound().X < rectangle2.MinBound().X || rectangle1.MinBound().X > rectangle2.MaxBound().X) return false;
        if (rectangle1.MaxBound().Y < rectangle2.MinBound().Y || rectangle1.MinBound().Y > rectangle2.MaxBound().Y) return false;

        return true;
    }

    template <typename T>
    [[nodiscard]] constexpr bool Intersect(const Rectangle<T> rectangle, const Circle<T> circle) noexcept
    {
        const auto center = circle.Center();

        if (rectangle.Contains(center)) return true;

        const auto minBound = rectangle.MinBound();
        const auto maxBound = rectangle.MaxBound();
        const auto radius = circle.Radius();

        const auto minBoundRect1 = minBound - Vec2<T>(radius, 0);
        const auto minBoundRect2 = minBound - Vec2<T>(0, radius);
        const auto maxBoundRect1 = maxBound + Vec2<T>(radius, 0);
        const auto maxBoundRect2 = maxBound + Vec2<T>(0, radius);

        // Added circle radius to rectangle bounds
        const Rectangle<T> rectangle1(minBoundRect1, maxBoundRect1);
        const Rectangle<T> rectangle2(minBoundRect2, maxBoundRect2);

        // Check rectangle bounds
        if (rectangle1.Contains(center)) return true;
        if (rectangle2.Contains(center)) return true;

        // Check circles at rectangle corners
        const auto corners = {
            minBound,
            maxBound,
            Vec2<T>(minBound.X, maxBound.Y),
            Vec2<T>(maxBound.X, minBound.Y)
        };

        for (const auto& corner : corners)
        {
            if (circle.Contains(corner)) return true;
        }

        return false;
    }

    template <typename T>
    [[nodiscard]] constexpr bool Intersect(const Circle<T> circle, const Rectangle<T> rectangle) noexcept
    {
        return Intersect(rectangle, circle);
    }

    template <typename T>
    [[nodiscard]] constexpr bool Intersect(const Polygon<T> polygon1, const Polygon<T> polygon2) noexcept
    {
        // Separate axis theorem
        // Check if any of the edges of polygon1 is a separating axis
        for (int i = 0, j = polygon1.VerticesCount() - 1; i < polygon1.VerticesCount(); j = i++)
        {
            const auto edge = polygon1.Vertices()[i] - polygon1.Vertices()[j];
            const auto normal = Vec2<T>(-edge.Y, edge.X);

            const auto startProjection1 = polygon1.Vertices()[0].Dot(normal);
            const auto startProjection2 = polygon2.Vertices()[0].Dot(normal);

            Vec2<T> projection1 = Vec2<T>(startProjection1, startProjection1);
            Vec2<T> projection2 = Vec2<T>(startProjection2, startProjection2);

            for (const auto& vertex : polygon1.Vertices())
            {
                const auto projection = vertex.Dot(normal);

                projection1 = Vec2<T>(Math::Min(projection1.X, projection), Math::Max(projection1.Y, projection));
            }

            for (const auto& vertex : polygon2.Vertices())
            {
                const auto projection = vertex.Dot(normal);

                projection2 = Vec2<T>(Math::Min(projection2.X, projection), Math::Max(projection2.Y, projection));
            }

            if (projection1.Y < projection2.X || projection2.Y < projection1.X) return false;
        }

        // Check if any of the edges of polygon2 is a separating axis
        for (int i = 0, j = polygon2.VerticesCount() - 1; i < polygon2.VerticesCount(); j = i++)
        {
            const auto edge = polygon2.Vertices()[i] - polygon2.Vertices()[j];
            const auto normal = Vec2<T>(-edge.Y, edge.X);

            const auto startProjection1 = polygon1.Vertices()[0].Dot(normal);
            const auto startProjection2 = polygon2.Vertices()[0].Dot(normal);

            Vec2<T> projection1 = Vec2<T>(startProjection1, startProjection1);
            Vec2<T> projection2 = Vec2<T>(startProjection2, startProjection2);

            for (const auto& vertex : polygon1.Vertices())
            {
                const auto projection = vertex.Dot(normal);

                projection1 = Vec2<T>(Math::Min(projection1.X, projection), Math::Max(projection1.Y, projection));
            }

            for (const auto& vertex : polygon2.Vertices())
            {
                const auto projection = vertex.Dot(normal);

                projection2 = Vec2<T>(Math::Min(projection2.X, projection), Math::Max(projection2.Y, projection));
            }

            if (projection1.Y < projection2.X || projection2.Y < projection1.X) return false;
        }

        return true;
    }

    template<typename T>
    Vec2<T> ClosestPointOnSegment(const Vec2<T> &A, const Vec2<T> &B, const Vec2<T> &P)
    {
        const Vec2<T> AB = B - A;
        const T t = ((P - A).X * AB.X + (P - A).Y * AB.Y) / AB.SquareLength();
        if (t <= 0)
        {
            return A;
        } else if (t >= 1)
        {
            return B;
        } else
        {
            return A + t * AB;
        }
    }

    template <typename T>
    [[nodiscard]] constexpr bool Intersect(const Polygon<T> polygon, const Circle<T> circle) noexcept
    {
        const auto center = circle.Center();
        const auto radius = circle.Radius();

        for (const auto &vertex: polygon.Vertices())
        {
            if (circle.Contains(vertex))
            {
                return true;
            }
        }

        for (int i = 0, j = polygon.VerticesCount() - 1; i < polygon.VerticesCount(); j = i++)
        {
            const auto p1 = polygon.Vertices()[i];
            const auto p2 = polygon.Vertices()[j];

            // Calculate the closest point on the edge to the circle's center.
            Vec2<T> closest = ClosestPointOnSegment(p1, p2, center);

            // Check if the closest point is within the circle's radius.
            if (Math::Vec2F(center - closest).SquareLength() <= radius * radius)
            {
                return true;
            }
        }

        return false;
    }

    template <typename T>
    [[nodiscard]] constexpr bool Intersect(const Circle<T> circle, const Polygon<T> polygon) noexcept
    {
        return Intersect(polygon, circle);
    }

    template <typename T>
    [[nodiscard]] constexpr bool Intersect(const Polygon<T> polygon, const Rectangle<T> rectangle) noexcept
    {
        Polygon<T> rectToPolygon = Polygon<T>({
            rectangle.MinBound(),
            Vec2<T>(rectangle.MinBound().X, rectangle.MaxBound().Y),
            rectangle.MaxBound(),
            Vec2<T>(rectangle.MaxBound().X, rectangle.MinBound().Y)
        });

        return Intersect(polygon, rectToPolygon);
    }

    template <typename T>
    [[nodiscard]] constexpr bool Intersect(const Rectangle<T> rectangle, const Polygon<T> polygon) noexcept
    {
        return Intersect(polygon, rectangle);
    }
}