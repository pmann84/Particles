#pragma once

#include <array>
#include <valarray>
#include <ostream>

namespace ps
{
    template<class TYPE>
    class Vector3
    {
    public:
        Vector3();
        Vector3(TYPE x, TYPE y, TYPE z);
        Vector3(const Vector3<TYPE>& other);

        // Random Access Operators
        TYPE& operator[] (unsigned int n)
        {
            return m_data[n];
        }

        const TYPE& operator[] (unsigned int n) const
        {
            return m_data[n];
        }

        // Arithmetic Operators for other vectors
        Vector3<TYPE> operator+(const Vector3<TYPE>& rhs)
        {
            Vector3<TYPE> ret(*this);
            ret += rhs;
            return ret;
        }

        Vector3<TYPE>& operator+=(const Vector3<TYPE>& rhs)
        {
            for (unsigned int i = 0; i < 3; ++i)
            {
                m_data[i] += rhs[i];
            }
            return *this;
        }

        Vector3<TYPE> operator-(const Vector3<TYPE>& rhs)
        {
            Vector3<TYPE> ret(*this);
            ret -= rhs;
            return ret;
        }

        Vector3<TYPE>& operator-=(const Vector3<TYPE>& rhs)
        {
            for (unsigned int i = 0; i < 3; ++i)
            {
                m_data[i] -= rhs[i];
            }
            return *this;
        }

        friend Vector3<TYPE> operator*(Vector3 lhs, const TYPE& rhs)
        {
            return lhs *= rhs;
        }

        friend Vector3<TYPE> operator*(const TYPE& lhs, Vector3 rhs)
        {
            return rhs *= lhs;
        }

        Vector3<TYPE>& operator*=(const TYPE& rhs)
        {
            for (unsigned int i = 0; i < 3; ++i)
            {
                m_data[i] *= rhs;
            }
            return *this;
        }

        friend Vector3<TYPE> operator/(Vector3 lhs, const TYPE& rhs)
        {
            return lhs /= rhs;
        }

        Vector3<TYPE>& operator/=(const TYPE& rhs)
        {
            for (unsigned int i = 0; i < 3; ++i)
            {
                m_data[i] /= rhs;
            }
            return *this;
        }

        Vector3<TYPE> operator/(const Vector3<TYPE>& rhs)
        {
            Vector3<TYPE> ret(*this);
            ret /= rhs;
            return ret;
        }

        friend std::ostream& operator<< (std::ostream& stream, const Vector3<TYPE>& rhs)
        {
            stream << "[" << rhs[0] << ", " << rhs[1] << ", " << rhs[2] << "]";
            return stream;
        }

        // Mathematical vector operations
        TYPE norm_squared()
        {
            // The euclidean norm squared - the sum of the squares of all the vector elements
            // This is basically just the inner product with itself
            return inner_product(*this, *this);
        }

        TYPE norm()
        {
            // Also known as length or magnitude
            // The euclidean norm - defined as the square root of sum of the squares of all the vector elements
            // Physically this is the length of the vector
            return std::sqrt(norm_squared());
        }

        void normalise()
        {
            auto magnitude = norm();
            for (auto i = 0; i<3; ++i)
            {
                m_data[i] /= magnitude;
            }
        }

        TYPE distance(Vector3 vector)
        {
            return std::sqrt(inner_product(*this, vector));
        }

        // Dot/Scalar Product a . b, this is the Inner Product as defined for Rn,
        // which is a way to multiply vectors together that results in a scalar
        friend TYPE inner_product(const Vector3<TYPE>& lhs, const Vector3<TYPE>& rhs)
        {
            TYPE result = static_cast<TYPE>(0);
            for (int i = 0; i < 3; ++i)
            {
                result += lhs[i] * rhs[i];
            }
            return result;
        }

    private:
        std::array<TYPE, 3> m_data;
    };

    template <class TYPE>
    Vector3<TYPE>::Vector3() : Vector3(0, 0, 0)
    {
    }

    template <class TYPE>
    Vector3<TYPE>::Vector3(TYPE x, TYPE y, TYPE z)
    {
        m_data[0] = x;
        m_data[1] = y;
        m_data[2] = z;
    }

    template <class TYPE>
    Vector3<TYPE>::Vector3(const Vector3<TYPE>& other)
    {
        m_data[0] = other[0];
        m_data[1] = other[1];
        m_data[2] = other[2];
    }

    using Vector3d = Vector3<double>;
    using Vector3f = Vector3<float>;
    using Vector3i = Vector3<int>;
}