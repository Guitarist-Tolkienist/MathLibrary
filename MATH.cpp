#include "MATH.h"
#include <cmath>
#include "cassert"

namespace Geometry_2D {
    SVector_2D::SVector_2D(float DefaultValue):
    X(DefaultValue),
    Y(DefaultValue) {}

    SVector_2D::SVector_2D(float x, float y) :
    X(x),
    Y(y) {}

    SVector_2D::SVector_2D(const SVector_2D& Vector_2D) {
        X = Vector_2D.X;
        Y = Vector_2D.Y;
    }

    float SVector_2D::Magnitude() const {
        return float(sqrt(double(X*X + Y*Y)));
    }

    void Geometry_2D::SVector_2D::Normalize() {
        float VectorMagnitude = Magnitude();

        X = X/VectorMagnitude;
        Y = Y/VectorMagnitude;
    }


    // OPERATORS
    SVector_2D& SVector_2D::operator=(const SVector_2D& Vector2) {
        if(this == &Vector2) return *this;

        this->X = Vector2.X;
        this->Y = Vector2.Y;
        return *this;
    }

    SVector_2D& SVector_2D::operator+=(const SVector_2D &Vector2) {
        this->X += Vector2.X;
        this->Y += Vector2.Y;
        return *this;
    }
    SVector_2D& SVector_2D::operator-=(const SVector_2D &Vector2) {
        this->X -= Vector2.X;
        this->Y -= Vector2.Y;
        return *this;
    }
    SVector_2D& SVector_2D::operator*=(float Value) {
        this->X *= Value;
        this->Y *= Value;

        return *this;
    }
    SVector_2D& SVector_2D::operator/=(float Value) {
        this->X /= Value;
        this->Y /= Value;

        return *this;
    }

    bool SVector_2D::operator()(const SVector_2D &Vector1, const SVector_2D &Vector2) const {
        return Vector1 == Vector2;
    }

    SVector_2D operator+(const SVector_2D& Vector1, const SVector_2D& Vector2) {
        return SVector_2D(Vector1.X + Vector2.X, Vector1.Y + Vector2.Y);
    }
    SVector_2D operator-(const SVector_2D& Vector1, const SVector_2D& Vector2) {
        return SVector_2D(Vector1.X - Vector2.X, Vector1.Y - Vector2.Y);
    }

    SVector_2D operator*(const SVector_2D& Vector1, float Value) {
        return SVector_2D(Vector1.X * Value, Vector1.Y * Value);
    }
    SVector_2D operator*(float Value, const SVector_2D &Vector1) {
        return Vector1 * Value;
    }

    bool operator==(const SVector_2D &Vector1, const SVector_2D &Vector2) {
        return Vector1.X == Vector2.X && Vector1.Y == Vector2.Y;
    }

    std::ostream& operator<<(std::ostream& out, const SVector_2D& Vector) {
        out << "Vector (" << Vector.X << ", " << Vector.Y << ")\n";
        return out;
    }

    bool operator!=(SVector_2D a, SVector_2D b) {
        return !(a == b);
    }

    bool operator<(SVector_2D a, SVector_2D b) {
        return std::tie(a.X, a.Y) < std::tie(b.X, b.Y);
    }



    float DotProduct(const SVector_2D& Vector1, const SVector_2D& Vector2) {
        return Vector1.X*Vector2.X + Vector1.Y*Vector2.Y;
    }

    float AngleBetweenVectors(const SVector_2D& Vector1, const SVector_2D& Vector2) {
        return std::acos((DotProduct(Vector1, Vector2))/(Vector1.Magnitude()*Vector2.Magnitude()));
    }



    // [POLYGONS]

    // ===== RECTANGLE =====
    SVector_2D CRectangle::GetSize() const  {
        return SVector_2D(BottomRight.X - TopLeft.X,
                          BottomRight.Y - TopLeft.Y);
    }


    bool IsPointInsideRect(const SVector_2D& Point, const CRectangle& Rect) {
        return (Point.X >= Rect.TopLeft.X && Point.X <= Rect.BottomRight.X) &&
        (Point.Y >= Rect.TopLeft.Y && Point.Y <= Rect.BottomRight.Y);
    }

    bool DoRectanglesOverlap(const CRectangle& R1, const CRectangle& R2) {
        if (DoRectanglesOverlap_X(R1, R2) &&
        DoRectanglesOverlap_Y(R1, R2)) return true;

        return false;
    }

    bool DoRectanglesOverlap_X(const CRectangle& R1, const CRectangle& R2) {
        if ((R1.TopLeft.X < R2.BottomRight.X  ) && (R1.BottomRight.X > R2.TopLeft.X  )) {
            return true;
        }

        return false;
    }

    bool DoRectanglesOverlap_Y(const CRectangle& R1, const CRectangle& R2) {
        if ((R1.TopLeft.Y < R2.BottomRight.Y  ) &&
        (R1.BottomRight.Y > R2.TopLeft.Y  ) ) {
            return true;
        }
        return false;
    }

    CRectangle CreateRectangleIncludingTwoPoints(const SVector_2D& Point1, const SVector_2D& Point2) {
        SVector_2D TL;
        TL.X = Point1.X < Point2.X ? Point1.X : Point2.X;
        TL.Y = Point1.Y < Point2.Y ? Point1.Y: Point2.Y;

        SVector_2D BR;
        BR.X = Point1.X < Point2.X ? Point2.X : Point1.X;
        BR.Y = Point1.Y < Point2.Y ? Point2.Y: Point1.Y;


        return CRectangle(TL, BR);
    }

    // ===== RECTANGLE =====



    // ===== Circle =====
    float CCircle::GetRadius() const {
        return Radius;
    }

    SVector_2D CCircle::GetCenter() const {
        return Center;
    }
    // ===== Circle =====
}

namespace Math {
    float RadiansToDegrees(float Rad) {
        return Rad * (180 / PI);
    }

    float DegreesToRadians(float Deg) {
        return Deg / (180 * PI);
    }

    float Lerp(float a, float b, float f) {
        if (f < 0.0f || f > 1.0f) return 0.0f;

        return (1 - f) * a + f * b;
    }

    bool IsNearlyEqual(double a, double b, double epsilon){
        return fabs(a - b) <= ( (fabs(a) < fabs(b) ? fabs(b) : fabs(a)) * epsilon);
    }


    // MATRICES

    // CONSTRUCTORS/DESTRUCTOR
    template<typename T>
    Matrix<T>::Matrix() :
            N(0),
            M(0) {}

    template<typename T>
    Matrix<T>::Matrix(int N, int M) : N(N), M(M), array(nullptr) {
        array = new T*[N];
        for (int i = 0; i < N; ++i) {
            array[i] = new T[M];
        }

        Reset();
    }

    template<typename T>
    Matrix<T>::Matrix(const Matrix<T>& Matrix) : N(Matrix.N), M(Matrix.M) {
        array = new T*[N];
        for (int i = 0; i < N; ++i) {
            array[i] = new T[M];
        }

        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < M; ++j) {
                array[i][j] = Matrix[i][j];
            }
        }
    }

    template<typename T>
    Matrix<T>::~Matrix() {
        for (int i = 0; i < N; ++i) {
            delete[] array[i]; // delete rows
        }

        // delete columns
        delete[] array;

        std::cout << "deleted\n";
    }

    template<typename T>
    void Matrix<T>::Reset() {
        for (int i = 0; i < N; ++i) {
            for (int j = 0; +j < M; ++j) {
                array[i][j] = 0;
            }
        }
    }

    template<class T>
    Matrix<T> Matrix<T>::GetTranspose() {
        Matrix<T> TransposeMatrix(M, N);

        for (int i = 0; i < M; ++i) {
            for (int j = 0; j < N; ++j) {
                TransposeMatrix[i][j] = array[j][i];
            }
        }

        return TransposeMatrix;
    }

    // OPERATORS
    template<typename T>
    T* Matrix<T>::operator[](int index) {
        if (index < N || index >= N) {
            assert("index is out of range");
        }
        return array[index];
    }

    template<typename T>
    const T* Matrix<T>::operator[](int index) const {
        if (index < N || index >= N) {
            assert("index is out of range");
        }
        return array[index];
    }

    template<typename T>
    std::ostream& operator<<(std::ostream& out, const Matrix<T>& Matrix) {
        for (int i = 0; i < Matrix.N; ++i) {
            out << "\n";
            for (int j = 0; j < Matrix.M; ++j) {
                out << Matrix[i][j] << " ";
            }
        }

        return out;
    }
    std::ostream& operator<<(std::ostream& out, const Matrix<int>& Matrix) {
        for (int i = 0; i < Matrix.N; ++i) {
            out << "\n";
            for (int j = 0; j < Matrix.M; ++j) {
                out << Matrix[i][j] << " ";
            }
        }

        return out;
    }

    template<typename T>
    Matrix<T>* operator+(const Matrix<T>& Matrix1, const Matrix<T>& Matrix2) {
        if (Matrix1.N != Matrix2.N ||
            Matrix1.M != Matrix2.M) return nullptr;

        Matrix<T>* Result = new Matrix<T>(Matrix1.N, Matrix1.M);

        for (int i = 0; i < Matrix1.N; ++i) {
            for (int j = 0; j < Matrix1.M; ++j) {
                int res1 = Matrix1[i][j];
                int res2 = Matrix2[i][j];
                (*Result)[i][j] = res1 + res2;
            }
        }

        return Result;
    }
    Matrix<int>* operator+(const Matrix<int>& Matrix1, const Matrix<int>& Matrix2) {
        if (Matrix1.N != Matrix2.N ||
            Matrix1.M != Matrix2.M) return nullptr;

        Matrix<int>* Result = new Matrix<int>(Matrix1.N, Matrix1.M);

        for (int i = 0; i < Matrix1.N; ++i) {
            for (int j = 0; j < Matrix1.M; ++j) {
                int res1 = Matrix1[i][j];
                int res2 = Matrix2[i][j];
                (*Result)[i][j] = res1 + res2;
            }
        }

        return Result;
    }

    template<class T>
    Matrix<T>* operator*(const Matrix<T>& Matrix1, const Matrix<T>& Matrix2) {
        if (Matrix1.M != Matrix2.N) return nullptr;

        Matrix<T>* Result = new Matrix<T>(Matrix1.N, Matrix1.M);


        return Result;
    }

    template<typename T>
    Matrix<T>& Matrix<T>::operator=(const Matrix<T>& Matrix) {
        if (this->N != Matrix.N || this->M != Matrix.M) {
            assert("not the same size of matrices");
        }

        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < M; ++j) {
                array[i][j] = Matrix.array[i][j];
            }
        }
        return *this;
    }

    template<class T>
            Geometry_2D::SVector_2D Matrix<T>::GetSize() const{
        return Geometry_2D::SVector_2D(N, M);
    }


    template class Matrix<int>;

}
