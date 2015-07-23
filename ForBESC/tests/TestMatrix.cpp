/*
 * File:   TestMatrix.cpp
 * Author: chung
 *
 * Created on Jul 7, 2015, 8:07:04 PM
 * 
 * ForBES is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *  
 * ForBES is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with ForBES. If not, see <http://www.gnu.org/licenses/>.
 */

#include "TestMatrix.h"

#include <cstdlib>
#include <stdlib.h>
#include <cmath>
#include <time.h>

CPPUNIT_TEST_SUITE_REGISTRATION(TestMatrix);

#define _ASSERT_OK               CPPUNIT_ASSERT_NO_THROW
#define _ASSERT                  CPPUNIT_ASSERT
#define _ASSERT_NOT(P)           CPPUNIT_ASSERT(!(P))
#define _ASSERT_NUM_EQ(A,B,TOL)  CPPUNIT_ASSERT_DOUBLES_EQUAL((double)(A), (double)(B), (double)(TOL))
#define _ASSERT_EQ               CPPUNIT_ASSERT_EQUAL
#define _ASSERT_NEQ(A,B)         CPPUNIT_ASSERT(A!=B)

TestMatrix::TestMatrix() {
}

TestMatrix::~TestMatrix() {
}

void TestMatrix::setUp() {
    srand(time(NULL));
}

void TestMatrix::tearDown() {
}

void TestMatrix::testQuadratic() {
    /* Test quadratic with diagonal matrices */
    Matrix *f;
    Matrix *x;
    for (int n = 5; n < 12; n++) {
        f = new Matrix(n, n);
        x = new Matrix(n, 1);
        for (int i = 0; i < n; i++) {
            f -> set(i, i, 1.0f);
            (*x)[i] = i + 1;
        }
        float r = f -> quad(*x);
        _ASSERT_EQ(static_cast<float> (n * (n + 1)* (2 * n + 1) / 6), r);
        delete f;
        delete x;
    }
}

void TestMatrix::testQuadratic2() {
    float fdata[9] = {-1.0, 3.0, 1.0, 2.0, -1.0, -1.0, 5.0, 2.0, -5.0};
    float xdata[3] = {1.0, 2.0, 3.0};

    Matrix f(3, 3, fdata);
    Matrix x(3, 1, xdata);

    float r;
    _ASSERT_OK(r = f.quad(x));
    _ASSERT_EQ(-16.0f, r);
}

void TestMatrix::testQuadratic3() {
    float fdata[9] = {-1.0, -3.0, 7.5, 2.0, -1.0, -1.0, 5.0, 2.0, -5.0};
    float xdata[3] = {-1.5, 2.0, 3.0};
    float qdata[3] = {5.0, -6.0, 13.5};

    Matrix f(3, 3, fdata);
    Matrix x(3, 1, xdata);
    Matrix q(3, 1, qdata);

    float r;
    _ASSERT_OK(r = f.quad(x, q));
    _ASSERT_EQ(-77.5f, r);
}

void TestMatrix::testQuadraticDot() {
    float ydata[4] = {-2.0, 5.0, -6.0, -11.0};
    float xdata[4] = {10.0, 2.0, 3.0, 4.0};

    Matrix y(4, 1, ydata);
    Matrix x(4, 1, xdata);

    Matrix r = y*x;

    _ASSERT_EQ(1, r.getNcols());
    _ASSERT_EQ(1, r.getNrows());
    _ASSERT_EQ(1, r.length());
    _ASSERT_EQ(-72.0f, r[0]);
}

void TestMatrix::testMultiplication() {
    float fdata[9] = {-1.0, 3.0, 1.0, 2.0, -1.0, -1.0, 5.0, 2.0, -5.0};
    float xdata[3] = {1.0, 2.0, 3.0};

    Matrix f(3, 3, fdata);
    Matrix x(3, 1, xdata);

    Matrix r;
    r = f*x;

    _ASSERT_EQ(3, r.getNrows());
    _ASSERT_EQ(1, r.getNcols());
    _ASSERT_EQ(3, r.length());
    _ASSERT_EQ(18.f, r[0]);
    _ASSERT_EQ(7.f, r[1]);
    _ASSERT_EQ(-16.f, r[2]);
}

void TestMatrix::testGetSet() {
    Matrix f(10, 10);
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            f.set(i, j, static_cast<float> (3 * i + 5 * j + 13));
        }
    }
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            _ASSERT_EQ(static_cast<float> (3 * i + 5 * j + 13), f.get(i, j));
        }
    }
}

void TestMatrix::testAssignment() {
    const int nRows = 3;
    const int nCols = 2;
    const int size = nRows * nCols;

    Matrix f = MatrixFactory::MakeRandomMatrix(nRows, nCols, 0.0, 1.0, Matrix::MATRIX_DENSE);
    Matrix g;
    g = f;

    _ASSERT_EQ(Matrix::MATRIX_DENSE, g.getType());
    _ASSERT_EQ(size, g.length());
    for (int i = 0; i < size; i++) {
        _ASSERT(g[i] >= 0);
        _ASSERT(g[i] <= 1);
    }
}

void TestMatrix::testAdditionBad() {
    Matrix A(5, 6);
    Matrix B(7, 8);
    Matrix C;
    CPPUNIT_ASSERT_THROW(C = A + B, std::invalid_argument);
}

void TestMatrix::testAddition() {
    const int nRows = 3;
    const int nCols = 2;
    const int size = nRows * nCols;
    float *a, *b;
    a = new float[size];
    b = new float[size];
    for (int i = 0; i < size; i++) {
        a[i] = i;
        b[i] = 3 * i + 7;
    }

    Matrix A(nRows, nCols, a);
    Matrix B(nRows, nCols, b);


    Matrix C;
    C = A + B;

    for (int i = 0; i < size; i++) {
        _ASSERT_EQ(a[i] + b[i], C[i]);
    }

    delete[] a;
    delete[] b;
}

void TestMatrix::testFBMatrix() {
    /* Test FBMatrix() - empty constructor */
    Matrix *fBMatrix = new Matrix();
    _ASSERT_EQ(0, fBMatrix->getNcols());
    _ASSERT_EQ(0, fBMatrix->getNrows());
    delete fBMatrix;

    /* Test FBMatrix(int, int, float*) - Provide data */
    const int n = 5;
    float *x = new float[n];
    for (int i = 0; i < n; i++) {
        x[i] = 1 + 7 * i;
    }
    Matrix f(n, 1, x);
    delete[] x;
    for (int i = 0; i < n; i++) {
        _ASSERT_EQ(static_cast<float> (1 + 7 * i), f[i]);
    }
}

void TestMatrix::testMakeRandomFBMatrix() {
    const int nRows = 10;
    const int nCols = 20;
    const float offset = 0.1;
    const float scale = 3.5;
    Matrix f = MatrixFactory::MakeRandomMatrix(nRows, nCols, offset, scale, Matrix::MATRIX_DENSE);

    _ASSERT_EQ(nCols, f.getNcols());
    _ASSERT_EQ(nRows, f.getNrows());
    for (int i = 0; i < nRows * nCols; i++) {
        if (i > 0) {
            _ASSERT_NEQ(f[i], f[i - 1]);
        }
        _ASSERT(f[i] >= offset);
        _ASSERT(f[i] <= offset + scale);
    }
}

void TestMatrix::testGetData() {
    int n = 100;
    float *myData = new float[n];
    for (int j = 0; j < n; j++) {
        myData[j] = j;
    }
    Matrix * mat = new Matrix(n, 1, myData);

    float * retrievedData;
    retrievedData = mat->getData();

    for (int j = 0; j < n; j++) {
        _ASSERT_EQ(j, (int) retrievedData[j]);
    }

    retrievedData[0] = 666;
    _ASSERT_EQ(666, (int) (*mat)[0]);

    delete(mat);
}

void TestMatrix::testGetNcols() {
    int y = (int) (5 + 50 * static_cast<float> (std::rand()) / static_cast<float> (RAND_MAX));
    Matrix mat(10, y);
    _ASSERT_EQ(y, mat.getNcols());
}

void TestMatrix::testGetNrows() {
    int x = (int) (5 + 50 * static_cast<float> (std::rand()) / static_cast<float> (RAND_MAX));
    Matrix mat(x, 10);
    _ASSERT_EQ(x, mat.getNrows());

    Matrix *gm = new Matrix(5, 5);
    _ASSERT_NOT(gm->isEmpty());
    _ASSERT_EQ(5, gm->getNrows());
    _ASSERT_EQ(5, gm->getNcols());
    delete gm;
}

void TestMatrix::testIsColumnVector() {
    Matrix rowVec(2345, 1);
    _ASSERT(rowVec.isColumnVector());
}

void TestMatrix::testIsRowVector() {
    Matrix rowVec(1, 100);
    _ASSERT(rowVec.isRowVector());
}

void TestMatrix::testLength() {
    int nRep = 20;
    int x, y;
    Matrix *f;
    f = new Matrix(3, 4);
    for (int i = 0; i < nRep; i++) {
        x = (int) (5 + 50 * static_cast<float> (std::rand()) / static_cast<float> (RAND_MAX));
        y = (int) (5 + 50 * static_cast<float> (std::rand()) / static_cast<float> (RAND_MAX));
        f = new Matrix(x, y);
        _ASSERT_EQ(x*y, f->length());
    }
    delete f;
}

void TestMatrix::testReshape() {
    Matrix f(45, 56);
    int status = f.reshape(5, 5);
    _ASSERT_EQ(0, status);
    _ASSERT_EQ(5, f.getNcols());
}

void TestMatrix::testReshapeBad() {
    Matrix f(45, 56);
    int status = f.reshape(45, 57);
    _ASSERT_EQ(-2, status);
    _ASSERT_EQ(45, f.getNrows());
    _ASSERT_EQ(56, f.getNcols());

    status = f.reshape(0, 1);
    _ASSERT_EQ(-1, status);
}

void TestMatrix::testDiagonalGetSet() {
    int n = 10;
    float *myData = new float[n];
    for (int j = 0; j < n; j++) {
        myData[j] = j + 1.0f;
    }

    Matrix *A = new Matrix(n, n, myData, Matrix::MATRIX_DIAGONAL);
    delete[] myData;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            _ASSERT_EQ(static_cast<float> (i + 1)*(i == j), A -> get(i, j));
        }
    }

    _ASSERT_EQ(Matrix::MATRIX_DIAGONAL, A -> getType());

    float t = -1.234;
    A -> set(3, 3, t);
    _ASSERT_EQ(t, A -> get(3, 3));
    CPPUNIT_ASSERT_THROW(A -> set(3, 4, 1.0), std::invalid_argument);

    delete A;
}

void TestMatrix::testDiagonalMultiplication() {
    /* Diag * Dense = Dense */
    int n = 10;
    int m = 3;
    float *myData = new float[n];
    for (int j = 0; j < n; j++) {
        myData[j] = j + 1.0f;
    }
    Matrix *A = new Matrix(n, n, myData, Matrix::MATRIX_DIAGONAL);
    Matrix B(n, m);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            B.set(i, j, i + 1.0f);
        }
    }

    Matrix C = (*A) * B;
    _ASSERT_EQ(Matrix::MATRIX_DENSE, C.getType());
    _ASSERT_EQ(n, C.getNrows());
    _ASSERT_EQ(m, C.getNcols());
    _ASSERT_NOT(C.isEmpty());
    _ASSERT_EQ(n*m, C.length());

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            _ASSERT_EQ(std::pow(i + 1.0f, 2.0f), C.get(i, j));
        }
    }

    delete A;
    delete[] myData;
}

void TestMatrix::testDiagonalMultiplication2() {
    /* Diag * Diag = Diag */
    int n = 10;
    float *aData = new float[n];
    float *bData = new float[n];

    for (int i = 0; i < n; i++) {
        aData[i] = i + 1.0f;
        bData[i] = n - i;
    }
    Matrix *A = new Matrix(n, n, aData, Matrix::MATRIX_DIAGONAL);
    Matrix *B = new Matrix(n, n, bData, Matrix::MATRIX_DIAGONAL);

    delete[] aData;
    delete[] bData;

    Matrix C = (*A) * (*B);
    _ASSERT_EQ(Matrix::MATRIX_DIAGONAL, A -> getType());
    _ASSERT_EQ(Matrix::MATRIX_DIAGONAL, B -> getType());
    _ASSERT_EQ(Matrix::MATRIX_DIAGONAL, C.getType());

    for (int i = 0; i < n; i++) {
        _ASSERT_EQ((i + 1.0f)*(n - i), C.get(i, i));
    }
}

void TestMatrix::testDenseTimesDiagonal() {
    const int nRows = 7;
    const int nCols = 3;
    const int size = nRows * nCols;
    float *aData;
    float *bData;

    aData = new float[size];
    for (int i = 0; i < size; i++) {
        aData[i] = i;
    }

    bData = new float[nCols];
    for (int i = 0; i < nCols; i++) {
        bData[i] = 3.0 * (i + 1);
    }


    Matrix A(nRows, nCols, aData, Matrix::MATRIX_DENSE);
    Matrix B(nCols, nCols, bData, Matrix::MATRIX_DIAGONAL);

    Matrix C = A * B;

    for (int i = 0; i < C.getNrows(); i++) {
        for (int j = 0; j < C.getNcols(); j++) {
            _ASSERT_EQ(A.get(i, j) * B.get(j, j), C.get(i, j));
        }
    }

    delete[] aData;
    delete[] bData;
}

void TestMatrix::testQuadDiagonal() {
    int n = 10;
    float *aData = new float[n];
    float *xData = new float[n];
    for (int i = 0; i < n; i++) {
        aData[i] = i + 1.0f;
        xData[i] = 3.0f * (i + 1.0f);
    }
    Matrix *A = new Matrix(n, n, aData, Matrix::MATRIX_DIAGONAL);
    Matrix *x = new Matrix(n, 1, xData);
    float val = A -> quad(*x, *x);
    const float correct = 30690.0f;
    _ASSERT_EQ(correct, val);

    val = A -> quad(*x);
    const float correct2 = 27225.0f;
    _ASSERT_EQ(correct2, val);

    delete A;
    delete x;
    delete[] aData;
    delete[] xData;
}

void TestMatrix::testSubtract() {
    const int n = 10;
    Matrix X = MatrixFactory::MakeRandomMatrix(n, n, 0.0, 1.0, Matrix::MATRIX_DENSE);
    Matrix Y = X - X;
    for (int i = 0; i < n * n; i++) {
        _ASSERT_EQ(0.0f, Y.getData()[i]);
    }

    X = MatrixFactory::MakeRandomMatrix(n, n, 0.0, 1.0, Matrix::MATRIX_DENSE);
    X -= X;
    for (int i = 0; i < n * n; i++) {
        _ASSERT_EQ(0.0f, X.getData()[i]);
    }

    X = MatrixFactory::MakeRandomMatrix(n, n, 0.0, 1.0, Matrix::MATRIX_DENSE);
    Matrix B = MatrixFactory::MakeRandomMatrix(n, n, 0.0, 1.0, Matrix::MATRIX_DENSE);
    Matrix Z = X - B;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < 0; j++) {
            _ASSERT_EQ(X.get(i, j) - B.get(i, j), Z.get(i, j));
        }
    }

}

void TestMatrix::testCholesky() {
    const int n = 3;
    float a[n * n] = {14, 32, 2,
        32, 77, 5,
        2, 5, 3};
    Matrix A(n, n, a, Matrix::MATRIX_DENSE);
    Matrix L;
    int info = A.cholesky(L);
    _ASSERT_EQ(0, info);
    _ASSERT_EQ(n, L.getNcols());
    _ASSERT_EQ(n, L.getNrows());
    _ASSERT_EQ(Matrix::MATRIX_DENSE, L.getType());

}

void TestMatrix::testSolveCholesky() {
    const int n = 3;
    float a[n * n] = {14, 32, 2,
        32, 77, 5,
        2, 5, 3};
    Matrix A(n, n, a, Matrix::MATRIX_DENSE);
    Matrix L;
    _ASSERT_EQ(0, A.cholesky(L));

    float bData[n] = {-1, 2, -3};
    Matrix b(n, 1, bData);
    Matrix x; // the solution!
    float tol = 1e-5;
    _ASSERT_EQ(0, L.solveCholeskySystem(x, b));
    _ASSERT_NUM_EQ(-2.75f, x[0], tol);
    _ASSERT_NUM_EQ(1.25f, x[1], tol);
    _ASSERT_NUM_EQ(-1.25f, x[2], tol);

    Matrix C = A * x - b;
    for (int i = 0; i < n; i++) {
        _ASSERT(std::fabs(C[i]) < tol);
    }
}

void TestMatrix::testSolveCholeskyMulti() {
    const int n = 4;
    const int m = 2;
    float a[n * n] = {7, 2, -2, -1,
        2, 3, 0, -1,
        -2, 0, 3, -1,
        -1, -1, -1, 1};
    Matrix A(n, n, a, Matrix::MATRIX_DENSE);
    Matrix L;
    _ASSERT_EQ(0, A.cholesky(L));
    float tol = 1e-5;
    _ASSERT_NUM_EQ(2.64575f, L[0], tol);
    _ASSERT_NUM_EQ(0.7559289f, L[1], tol);

    float bData[n * m] = {1, 2.5, 3, 4,
        -1, -2.5, -3, -4};
    Matrix b(n, m, bData);
    Matrix x; // the solution!

    _ASSERT_EQ(0, L.solveCholeskySystem(x, b));

    Matrix C = A * x - b;
    for (int i = 0; i < C.getNrows(); i++) {
        for (int j = 0; j < C.getNcols(); j++) {
            _ASSERT(std::fabs(C.get(i, j)) < 1e-4);
        }
    }
}

void TestMatrix::testLowerTriangular_getSet() {
    for (int n = 1; n < 20; n++) {
        Matrix *A = new Matrix(n, n, Matrix::MATRIX_LOWERTR);

        for (int i = 0; i < n; i++) {
            for (int j = 0; j <= i; j++) {
                A -> set(i, j, 3.2 * i + 7.5 * j + 0.45);
            }
        }

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (i >= j) {
                    _ASSERT_NUM_EQ(3.2f * i + 7.5f * j + 0.45f, A->get(i, j), 1e-3);
                } else {
                    _ASSERT_EQ(0.0f, A->get(i, j));
                }
            }
        }

        int exp_length = n * (n + 1) / 2;
        _ASSERT_EQ(exp_length, A->length());
        delete A;
    }
}

void TestMatrix::testLowerTriangularTraspose_getSet() {
    int n = 10;
    Matrix A = MatrixFactory::MakeRandomMatrix(n, n, 0.0, 1.0, Matrix::MATRIX_LOWERTR);
    Matrix AT(A);
    AT.transpose();
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            _ASSERT_EQ(A.get(i, j), AT.get(j, i));
        }
    }
    AT.transpose();
    _ASSERT_EQ(A, AT);
}

void TestMatrix::testSymmetric_getSet() {
    const int n = 4;
    Matrix *A = new Matrix(n, n, Matrix::MATRIX_SYMMETRIC);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j <= i; j++) {
            A -> set(i, j, 3.2 * i + 0.2 * j + 0.45);
        }
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i >= j) {
                _ASSERT_NUM_EQ(3.2f * i + 0.2f * j + 0.45f, A->get(i, j), 1e-5);
            } else {
                _ASSERT_NUM_EQ(A -> get(j, i), A->get(i, j), 1e-6);
            }
        }
    }
    int exp_length = n * (n + 1) / 2;
    _ASSERT_EQ(exp_length, A->length());

    Matrix *L = new Matrix();
    int status = A -> cholesky(*L); // A is not positive definite (Cholesky shouldn't be allowed!)
    _ASSERT_NEQ(0, status);
    delete L;

    delete A;
}

void TestMatrix::testSymmetricCholesky() {
    const int n = 4;
    Matrix *A = new Matrix(n, n, Matrix::MATRIX_SYMMETRIC);
    _ASSERT_EQ(Matrix::MATRIX_SYMMETRIC, A -> getType());
    for (int i = 0; i < n; i++) {
        for (int j = 0; j <= i; j++) {
            A -> set(i, j, 3.2 * i + 0.2 * j + 0.45);
            if (i == j) A -> set(i, i, A->get(i, i) + 20.0f);
        }
    }

    Matrix L;
    _ASSERT_EQ(0, A -> cholesky(L));
    float tol = 1e-4;
    _ASSERT_NUM_EQ(4.52217, L.get(0, 0), tol);
    _ASSERT_NUM_EQ(0.807135, L.get(1, 0), tol);
    _ASSERT_NUM_EQ(1.51476, L.get(2, 0), tol);
    _ASSERT_NUM_EQ(2.22239, L.get(3, 0), tol);
    _ASSERT_NUM_EQ(4.81649, L.get(1, 1), tol);

    delete A;
}

void TestMatrix::testTranspose() {
    int n = 5;
    int m = 6;
    Matrix A(n, m);
    A.transpose();
    _ASSERT_EQ(n, A.getNcols());
    _ASSERT_EQ(m, A.getNrows());
    A.transpose();
    _ASSERT_EQ(n, A.getNrows());
    _ASSERT_EQ(m, A.getNcols());

    Matrix X(n, m);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            X.set(i, j, 7.5 * i - 2.8 * j - 1.0f);
        }
    }
    X.transpose();
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            _ASSERT_OK(X.get(i, j));
        }
    }

    Matrix Y(n, m);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            Y.set(i, j, 7.5 * i - 2.8 * j - 1.0f);
        }
    }

    Matrix YT(m, n); // Construct the transpose of Y as YT
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            YT.set(i, j, Y.get(j, i));
        }
    }

    Y.transpose();
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            _ASSERT_EQ(YT.get(i, j), Y.get(i, j));
        }
    }

}

void TestMatrix::testDiagonalTimesSymmetric() {
    int n = 10;
    Matrix D(n, n, Matrix::MATRIX_DIAGONAL);
    for (int i = 0; i < n; i++) {
        D[i] = i + 1.0f;
    }

    Matrix S(n, n, Matrix::MATRIX_SYMMETRIC);
    for (int i = 0; i < n; i++) {
        for (int j = i; j < n; j++) {
            S.set(i, j, -3.1f * i + 3.25f * j + 5.35f);
        }
    }

    Matrix DS = D * S;
    _ASSERT_EQ(Matrix::MATRIX_SYMMETRIC, DS.getType());
    for (int i = 0; i < n; i++) {
        for (int j = i; j < n; j++) {
            _ASSERT_EQ((i + 1.0f)*(-3.1f * i + 3.25f * j + 5.35f), DS.get(i, j));
        }
    }
}

void TestMatrix::testDiagonalTimesLowerTri() {
    int n = 6;
    Matrix D(n, n, Matrix::MATRIX_DIAGONAL);
    for (int i = 0; i < n; i++) {
        D[i] = i + 1.0f;
        ;
    }

    Matrix L(n, n, Matrix::MATRIX_LOWERTR);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j <= i; j++) {
            L.set(i, j, -0.1f * i + 0.45f * j + 1.01f);
        }
    }

    Matrix DL = D * L;
    _ASSERT_EQ(Matrix::MATRIX_LOWERTR, DL.getType());
    _ASSERT_EQ(n, DL.getNcols());
    _ASSERT_EQ(n, DL.getNrows());
    _ASSERT_NOT(DL.isEmpty());

    for (int i = 0; i < n; i++) {
        for (int j = 0; j <= i; j++) {
            _ASSERT_EQ(0.0f, DL.get(i, j) - D[i] * L.get(i, j));
        }
    }
}

void TestMatrix::testDenseTimesSymmetric() {
    const int n = 4;
    float a[n * n] = {5, 11, -2, -1,
        6, 3, 7, -1,
        -21, 0, 13, -1,
        -18, -1, -17, 30};
    Matrix A(n, n, a, Matrix::MATRIX_DENSE);

    Matrix S(n, n, Matrix::MATRIX_SYMMETRIC);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j <= i; j++) {
            S.set(i, j, 3 * i + 2 * j + 4.0f);
        }
    }

    Matrix AS = A*S;
    _ASSERT_NOT(AS.isEmpty());
    _ASSERT_NOT(AS.isColumnVector());
    _ASSERT_NOT(AS.isRowVector());
    _ASSERT_EQ(Matrix::MATRIX_DENSE, AS.getType());
    _ASSERT_EQ(n, AS.getNcols());
    _ASSERT_EQ(n, AS.getNrows());

    float asData[n * n] = {-382.0f, 52.0f, -50.0f, 369.0f,
        -433.0f, 89.0f, -50.0f, 422.0f,
        -478.0f, 129.0f, -43.0f, 474.0f,
        -544.0f, 169.0f, -23.0f, 525.0f};
    Matrix AS_correct(n, n, asData, Matrix::MATRIX_DENSE);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            _ASSERT_EQ(AS_correct.get(i, j), AS.get(i, j));
        }
    }
}

void TestMatrix::testDenseTimesLowerTriangular() {
    const int n = 4;
    float a[n * n] = {5, 11, -2, -1,
        6, 3, 7, -1,
        -21, 0, 13, -1,
        -18, -1, -17, 30};
    Matrix A(n, n, a, Matrix::MATRIX_DENSE);

    Matrix L(n, n, Matrix::MATRIX_LOWERTR);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j <= i; j++) {
            L.set(i, j, 3 * i + 2 * j + 4.0f);
        }
    }

    Matrix AL = A*L;
    float alCorrect[n * n] = {-382.0f, 52.0f, -50.0f, 369.0f, -468.0f, 12.0f, -36.0f, 429.0f,
        -600.0f, -17.0f, -107.0f, 496.0f, -342.0f, -19.0f, -323.0f, 570.0f};
    Matrix AL_correct(n, n, alCorrect, Matrix::MATRIX_DENSE);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j <= i; j++) {
            _ASSERT_EQ(AL.get(i, j), AL_correct.get(i, j));
        }
    }
}

void TestMatrix::testQuadSymmetric() {
    int n = 10;

    Matrix A(n, n, Matrix::MATRIX_SYMMETRIC);
    Matrix x(n, 1);

    for (int i = 0; i < n; i++) {
        x.set(i, 0, 3.0f * (i + 1.0f));
        for (int j = 0; j <= i; j++) {
            A.set(i, j, 4.0f * i + 7.0f * j + 1.0f);
        }
    }
    float correct_val = 1711809.0f;
    float val = A.quad(x);
    _ASSERT_EQ(correct_val, val);

    Matrix q(n, 1);
    for (int i = 0; i < n; i++) {
        q.set(i, 0, -5.0f * i - 1.0f);
    }
    val = A.quad(x, q);
    correct_val = 1706694.0f;
    _ASSERT_EQ(correct_val, val);
}

void TestMatrix::testLeftTransposeMultiply() {
    int n = 10;
    int m = 5;
    int k = 8;

    Matrix A = MatrixFactory::MakeRandomMatrix(m, n, 0.0f, 1.0, Matrix::MATRIX_DENSE);
    Matrix B = MatrixFactory::MakeRandomMatrix(m, k, 0.0f, 1.0, Matrix::MATRIX_DENSE);

    A.transpose();
    _ASSERT_EQ(n, A.getNrows());
    _ASSERT_EQ(m, A.getNcols());

    Matrix C = A * B;

    _ASSERT_EQ(n, C.getNrows());
    _ASSERT_EQ(k, C.getNcols());

    A.transpose();
    Matrix AT(n, m, Matrix::MATRIX_DENSE);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            AT.set(i, j, A.get(j, i));
        }
    }
    Matrix C0 = AT * B;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            _ASSERT_EQ(C0.get(i, j), C.get(i, j));
        }
    }

}

void TestMatrix::testRightTransposeMultiply() {
    int n = 10;
    int m = 5;
    int k = 8;

    Matrix A = MatrixFactory::MakeRandomMatrix(n, m, 0.0f, 1.0, Matrix::MATRIX_DENSE);
    Matrix B = MatrixFactory::MakeRandomMatrix(k, m, 0.0f, 1.0, Matrix::MATRIX_DENSE);

    B.transpose();
    Matrix C = A*B;

    B.transpose();
    Matrix BT(m, k);
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < k; j++) {
            BT.set(i, j, B.get(j, i));
        }
    }

    Matrix C_correct = A*BT;
    _ASSERT_EQ(C_correct.getNrows(), C.getNrows());
    _ASSERT_EQ(C_correct.getNcols(), C.getNcols());
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < k; j++) {
            _ASSERT_EQ(C_correct.get(i, j), C.get(i, j));
        }
    }
}

void TestMatrix::testLeftSymmetricMultiply() {
    int n = 5;
    Matrix S = MatrixFactory::MakeRandomMatrix(n, n, 0.0, 1.0, Matrix::MATRIX_SYMMETRIC);
    Matrix A = MatrixFactory::MakeRandomMatrix(n, n, 0.0, 1.0, Matrix::MATRIX_DENSE);
    Matrix C = S*A;

    Matrix S2D(n, n, Matrix::MATRIX_DENSE);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            S2D.set(i, j, S.get(i, j));
        }
    }

    Matrix C_correct = S2D*A;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            _ASSERT_EQ(C_correct.get(i, j), C.get(i, j));
        }
    }

    Matrix x = MatrixFactory::MakeRandomMatrix(n, 1, 0.0, 1.0, Matrix::MATRIX_DENSE);
    C = S*x;

    C_correct = S2D * x;

    for (int i = 0; i < n; i++) {
        _ASSERT_NUM_EQ(C_correct.get(i, 0), C.get(i, 0), 1e-4);
    }
}

void TestMatrix::testSparseGetSet() {
    int n = 5;
    int m = 10;
    int max_nnz = 3;
    Matrix M = MatrixFactory::MakeSparse(n, m, max_nnz, Matrix::SPARSE_UNSYMMETRIC);

    float r[3] = {4.576, 3.645, 1.092};
    M.set(0, 0, r[0]);
    M.set(0, 1, r[1]);
    M.set(1, 1, r[2]);

    _ASSERT_EQ(r[0], M.get(0, 0));
    _ASSERT_EQ(r[1], M.get(0, 1));
    _ASSERT_EQ(r[2], M.get(1, 1));
    _ASSERT_EQ(n, M.getNrows());
    _ASSERT_EQ(m, M.getNcols());
    _ASSERT_EQ(Matrix::MATRIX_SPARSE, M.getType());

}

void TestMatrix::testSparseCholesky() {

    int n = 3;
    int m = 3;
    int max_nnz = 4;

    Matrix A = MatrixFactory::MakeSparse(n, m, max_nnz, Matrix::SPARSE_SYMMETRIC_L);
    A.set(0, 0, 4.0f);
    A.set(1, 0, 1.0f); // A is declared as SPARSE_SYMMETRIC - no need to define A(0,1).
    A.set(1, 1, 5.0f);
    A.set(2, 2, 10.0f);

    Matrix L;
    A.cholesky(L);

    Matrix rhs(3, 1);
    rhs.set(0, 0, 1.0f);
    rhs.set(1, 0, 1.0f);
    rhs.set(2, 0, 1.0f);

    Matrix xsol;
    L.solveCholeskySystem(xsol, rhs);

    const double tol = 1e-6;
    _ASSERT_NUM_EQ(0.210526, xsol.get(0, 0), tol);
    _ASSERT_NUM_EQ(0.157895, xsol[1], tol);
    _ASSERT_NUM_EQ(0.1, xsol[2], tol);

    _ASSERT_EQ(0, Matrix::cholmod_handle()->status);

}

void TestMatrix::testSparseDenseMultiply() {

    int n = 3;
    int m = 3;
    int max_nnz = 4;

    Matrix A = MatrixFactory::MakeSparse(n, m, max_nnz, Matrix::SPARSE_UNSYMMETRIC);
    A.set(0, 0, 4.0f);
    A.set(1, 0, 1.0f); // A is declared as SPARSE_SYMMETRIC - no need to define A(0,1).
    A.set(1, 1, 5.0f);
    A.set(2, 2, 10.0f);


    Matrix B(m, n);
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            B.set(i, j, 3.0 * i + 4.23 * j + 1.10);
        }
    }


    _ASSERT_EQ(m, B.getNrows());
    _ASSERT_EQ(n, B.getNcols());

    Matrix C;
    _ASSERT_OK(C = A * B);

    float correctData[9] = {
        4.4000, 21.6000, 71.0000,
        21.3200, 46.9800, 113.3000,
        38.2400, 72.3600, 155.6000
    };

    Matrix C_correct(n, n, correctData, Matrix::MATRIX_DENSE);


    _ASSERT_EQ(C_correct, C);
    _ASSERT_EQ(0, Matrix::cholmod_handle()->status);

}

void TestMatrix::testSparseAddDense() {
    int n = 5;
    int m = 7;
    int nnz = 6;
    Matrix A = MatrixFactory::MakeSparse(n, m, nnz, Matrix::SPARSE_UNSYMMETRIC);
    A.set(0, 0, 0.5);
    A.set(0, 1, 0.2);
    A.set(1, 0, 1.2);
    A.set(1, 1, 3.6);
    A.set(4, 5, 6.2);
    A.set(3, 6, 9.9);

    Matrix B(n, m, Matrix::MATRIX_DENSE);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            B.set(i, j, -i - 3 * j - 1.5f);
        }
    }

    Matrix A_init(A);
    A += B;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            _ASSERT_EQ(A.get(i, j), A_init.get(i, j) + B.get(i, j));
        }
    }


}

void TestMatrix::testSparseAddSparse() {
    FILE *fp_A, *fp_B;
    fp_A = fopen("matrices/sparse2.mx", "r");
    fp_B = fopen("matrices/sparse3.mx", "r");

    CPPUNIT_ASSERT_MESSAGE("Can't open sparse2.mx", fp_A != NULL);
    CPPUNIT_ASSERT_MESSAGE("Can't open sparse3.mx", fp_B != NULL);

    Matrix A = MatrixFactory::ReadSparse(fp_A);
    Matrix B = MatrixFactory::ReadSparse(fp_B);

    /* close file handlers */
    _ASSERT_EQ(0, fclose(fp_A));
    _ASSERT_EQ(0, fclose(fp_B));

    Matrix A0 = A;
    _ASSERT_OK(A += B);

    for (int i = 0; i < A.getNrows(); i++) {
        for (int j = 0; j < A.getNcols(); j++) {
            _ASSERT_EQ(A0.get(i, j) + B.get(i, j), A.get(i, j));
        }
    }

}

void TestMatrix::testSparseAddSparse2() {
    int n = 100;
    int m = 250;
    int nnz = 200;
    Matrix A = MatrixFactory::MakeRandomSparse(n, m, nnz, -1.0, 2.0);
    Matrix B = MatrixFactory::MakeRandomSparse(n, m, nnz, -1.0, 2.0);
    Matrix C;
    _ASSERT_OK(C = A + B);
    _ASSERT_EQ(Matrix::MATRIX_SPARSE, C.getType());
    _ASSERT_EQ(n, C.getNrows());
    _ASSERT_EQ(m, C.getNcols());

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            _ASSERT_EQ(A.get(i, j) + B.get(i, j), C.get(i, j));
        }
    }

    _ASSERT_OK(C += A);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            _ASSERT_EQ(2 * A.get(i, j) + B.get(i, j), C.get(i, j));
        }
    }

    _ASSERT_OK(Matrix::destroy_handle());
}

void TestMatrix::testSparseQuad() {
    int n = 10;
    int nnz = std::floor(1.2f * n);
    Matrix A = MatrixFactory::MakeRandomSparse(n, n, nnz, 0.0, 10.0);
    Matrix x = MatrixFactory::MakeRandomMatrix(n, 1, 1.0, 2.0, Matrix::MATRIX_DENSE);

    float r;
    _ASSERT_OK(r = A.quad(x));
    
    std::cout << A;
    std::cout << x;
    std::cout << r;

}
