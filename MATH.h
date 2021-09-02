#ifndef PLATFORMER_MATH_H
#define PLATFORMER_MATH_H

#include <iostream>
#include <vector>
#include <array>
#include <unordered_set>


namespace Geometry_2D {
    // ===== VECTOR 2D =====
    struct SVector_2D;
    bool operator==(const SVector_2D& Vector1, const SVector_2D& Vector2);

    struct SVector_2D {
        float X, Y;

        explicit SVector_2D(float DefaultValue = 0.0f);
        SVector_2D(float x, float y);
        SVector_2D(const SVector_2D& Vector_2D);

        float Magnitude() const;

        void Normalize();

        // OPERATORS
        SVector_2D& operator=(const SVector_2D& Vector2);

        SVector_2D& operator+=(const SVector_2D& Vector2);
        SVector_2D& operator-=(const SVector_2D& Vector2);
        SVector_2D& operator*=(float Value);
        SVector_2D& operator/=(float Value);

        bool operator() (const SVector_2D& Vector1, const SVector_2D& Vector2) const;
    };

    // OPERATORS
    SVector_2D operator+(const SVector_2D& Vector1, const SVector_2D& Vector2);
    SVector_2D operator-(const SVector_2D& Vector1, const SVector_2D& Vector2);

    // Vector * Number
    SVector_2D operator*(const SVector_2D &Vector1, float Value);
    // Number * Vector
    SVector_2D operator*(float Value, const SVector_2D &Vector1);

    std::ostream& operator<<(std::ostream& out, const SVector_2D& Vector);

    bool operator!=(SVector_2D a, SVector_2D b);

    bool operator<(SVector_2D a, SVector_2D b);

    float AngleBetweenVectors(const SVector_2D& Vector1, const SVector_2D& Vector2);

    float DotProduct(const SVector_2D& Vector1, const SVector_2D& Vector2);

    const SVector_2D ZeroVector_2D(0.0f, 0.0f);
    // ===== VECTOR 2D =====



    // [POLYGONS]
    // ===== BASE POLYGON =====
    class CPolygon {
        int VertexMaxQuantity;

    public:
        inline CPolygon(int VertexNumber) : VertexMaxQuantity(VertexNumber) {}
    };
    // ===== BASE POLYGON =====



    // ===== Triangle =====
    class CTriangle : public CPolygon {
    public:
    };
    // ===== Triangle =====


    // ===== BaseFigure =====
    enum EFIGURE_TYPE {
        ERECT,
        ECIRCLE
    };
    class CFigure {
    public:
        EFIGURE_TYPE Type;
        CFigure(EFIGURE_TYPE FigureType) : Type(FigureType) {}
    };

    // ===== Circle =====
    class CCircle : public CFigure {
        SVector_2D Center;
        float Radius;

    public:
        inline CCircle(EFIGURE_TYPE Type = ECIRCLE) : CFigure(Type),
                                           Radius(1.0f),
                                           Center(0.0f, 0.0f) {}
        inline CCircle(float radius,
                       const SVector_2D& CentralPoint,
                       EFIGURE_TYPE Type = ECIRCLE) : CFigure(Type),
                                                      Radius(radius),
                                                      Center(CentralPoint) {}

        float GetRadius() const;
        SVector_2D GetCenter() const;
    };
    // ===== Circle =====



    // ===== RECTANGLE =====
    class CRectangle : public CFigure {
    public:
        SVector_2D TopLeft;
        SVector_2D BottomRight;

        inline CRectangle(EFIGURE_TYPE Type = ERECT) : CFigure(Type),
                                                       TopLeft(SVector_2D()),
                                                       BottomRight(SVector_2D()) {}
        inline CRectangle(const SVector_2D& TL,
                          const SVector_2D& BR,
                          EFIGURE_TYPE Type = ERECT) : CFigure(Type),
                                                       TopLeft(TL),
                                                       BottomRight(BR) {}
        SVector_2D GetSize() const;
    };

    bool IsPointInsideRect(const SVector_2D& Point, const CRectangle& Rect);

    bool DoRectanglesOverlap(const CRectangle& rect1, const CRectangle& rect2);

    bool DoRectanglesOverlap_X(const CRectangle& rect1, const CRectangle& rect2);

    bool DoRectanglesOverlap_Y(const CRectangle& rect1, const CRectangle& rect2);

    CRectangle CreateRectangleIncludingTwoPoints(const SVector_2D&, const SVector_2D&);
    // ===== RECTANGLE =====
}

namespace Math {
    enum AngleQuarter {
        FIRST,
        SECOND,
        THIRD,
        FOURTH,
    };

    const float PI = 3.1415926;

    float RadiansToDegrees(float Rad);

    float DegreesToRadians(float Deg);

    // a and b - values being interpolated between
    // f - fractional value int the range of [0, 1] from a to b
    // `Game Programming Algorithms and Techniques, Vectors, p.55`
    float Lerp(float a, float b, float f);

    // returns an approximate answer whether the two double values are equal
    bool IsNearlyEqual(double a, double b, double epsilon);

    // ===== MATRIX =====
    template<class T>
    class Matrix {
        int N;
        int M;

        T **array;
    public:
        Matrix<T>();

        Matrix<T>(int N, int M);

        Matrix<T>(const Matrix &Matrix);

        ~Matrix<T>();

        void Reset();

        Geometry_2D::SVector_2D GetSize() const;

        Matrix<T> GetTranspose();

        Matrix<T> &operator=(const Matrix<T> &Matrix);

        T* operator[](int index);

        const T* operator[](int index) const;

        friend std::ostream &operator<<(std::ostream &out, const Matrix <T> &Matrix);

        friend Matrix<T> *operator+(const Matrix <T> &Matrix1, const Matrix <T> &Matrix2);

        friend Matrix<T> *operator*(const Matrix <T> &Matrix1, const Matrix <T> &Matrix2);
    };

    // ====================================
}

#endif //PLATFORMER_MATH_H
