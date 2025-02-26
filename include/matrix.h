#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <cassert>

#include "vec3.h"

template <typename T>
class Mat
{
    public:
        Mat(int nrows, int ncols)
        {
            m = std::vector<std::vector<T>>(nrows, std::vector<T>(ncols, static_cast<T>(0)));
            rows = nrows;
            cols = ncols;
        }

        std::vector<T>& operator[](const int idx)
        {
            assert(idx >= 0 && idx < rows);
            return m[idx];
        }

        const std::vector<T>& operator[](const int idx) const
        {
            assert(idx >= 0 && idx < rows);
            return m[idx];
        }

        Mat<T> operator *(const Mat<T>& ma)
        {
            assert(ma.rows == cols);
            Mat<T> retmat(rows, ma.cols);
            for (int i = 0; i < rows; i++)
                for (int j = 0; j < ma.cols; j++)
                {
                    retmat[i][j] = 0;
                    for (int k = 0; k < cols; k++)
                        retmat.m[i][j] += m[i][k] * ma.m[k][j];
                }

            return retmat;

        }

        Mat<T> operator +(const Mat<T>& ma)
        {
            assert(ma.rows == rows && ma.cols == cols);  
            Mat<T> retmat(rows, cols);  
               
            for (int i = 0; i < rows; i++)
                for (int j = 0; j < cols; j++)
                    retmat.m[i][j] = m[i][j] + ma.m[i][j];  

            return retmat;
        }

        Mat<T> operator -(const Mat<T>& ma)
        {
            assert(ma.rows == rows && ma.cols == cols);  
            Mat<T> retmat(rows, cols);  
               
            for (int i = 0; i < rows; i++)
                for (int j = 0; j < cols; j++)
                    retmat.m[i][j] = m[i][j] - ma.m[i][j];  

            return retmat;
        }

        const int get_nrows() { return rows;}  
        const int get_ncols() { return cols;}  


    
        friend std::ostream& operator<<(std::ostream& s, Mat<T>& ma) {
            for (int i=0; i<ma.get_nrows(); i++)  {
                for (int j=0; j<ma.get_ncols(); j++) {
                    s << '|' << ma[i][j] << '|';
                    if (j<ma.get_ncols()-1) s << "";
                }
                s << "\n";
            }
            return s;
        }

    private:
        std::vector<std::vector<T>>m;
        int rows, cols;
};

using Mati = Mat<int>;
using Matf = Mat<float>;
using Matd = Mat<double>;

template <typename T>
inline Mat<T> vec3_to_mat(const Vec3<T>& v)
{
    Mat<T> retmat(4, 1);
    retmat[0][0] = v.x();
    retmat[1][0] = v.y();
    retmat[2][0] = v.z();
    retmat[3][0] = static_cast<T>(1);
    return retmat;
}

template <typename T>
inline Vec3<T> mat_to_vec3(Mat<T> ma)
{
    T x = ma[0][0]/ma[3][0];
    T y = ma[1][0]/ma[3][0];
    T z = ma[2][0]/ma[3][0];
    return Vec3<T>(x, y, z);
}

template <typename T>
inline Mat<T> get_identity(int r, int c)
{
    Mat<T> ma(r, c);
    for (int i = 0; i < r; i++)
        for (int j = 0; j < c; j++)
        {
            if (i == j)
                ma[i][j] = 1;
            else 
                ma[i][j] = 0;
        }
    return ma;
}
#endif
