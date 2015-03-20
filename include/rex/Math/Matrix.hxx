#pragma once

#include "../Config.hxx"

REX_NS_BEGIN

// NOTE : M** is M[X-1][Y-1]

/// <summary>
/// Defines a matrix.
/// </summary>
struct Matrix
{
    union
    {
        struct
        {
            real64 M11, M12, M13, M14;
            real64 M21, M22, M23, M24;
            real64 M31, M32, M33, M34;
            real64 M41, M42, M43, M44;
        };
        real64 M[ 4 ][ 4 ];
    };

    /// <summary>
    /// Creates a new matrix.
    /// </summary>
    __both__ Matrix();

    /// <summary>
    /// Destroys this matrix.
    /// </summary>
    __both__ ~Matrix();

    /// <summary>
    /// Sets this matrix to be the identity matrix.
    /// </summary>
    __both__ void SetIdentity();

    __both__ bool operator==( const Matrix& ) const;
    __both__ bool operator!=( const Matrix& ) const;
};

__both__ Matrix operator*( const Matrix&, const Matrix& );
__both__ Matrix operator*( const Matrix&, real64 );
__both__ Matrix operator*( real64, const Matrix& );
__both__ Matrix operator/( const Matrix&, real64 );

REX_NS_END