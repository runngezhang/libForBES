/*
 * File:   TestSumOfNorm2.cpp
 * Author: chung
 *
 * Created on Feb 29, 2016, 5:18:46 PM
 */

#include "TestSumOfNorm2.h"
#include "SumOfNorm2.h"


CPPUNIT_TEST_SUITE_REGISTRATION(TestSumOfNorm2);

TestSumOfNorm2::TestSumOfNorm2() {
}

TestSumOfNorm2::~TestSumOfNorm2() {
}

void TestSumOfNorm2::setUp() {
}

void TestSumOfNorm2::tearDown() {
}

void TestSumOfNorm2::testCall() {
    const size_t k = 2;
    const double mu = 1.0;
    const double tolerance = 1e-8;
    Norm * f = new SumOfNorm2(mu, k);
    Matrix x(4, 1);
    double f_val = -1.0;

    int status = f->call(x, f_val);
    _ASSERT(ForBESUtils::is_status_ok(status));
    _ASSERT_NUM_EQ(0.00, f_val, tolerance);

    x[0] = -1.0;
    x[1] = 1.0;
    x[2] = 2.0;
    x[3] = -1.0;

    const double correct_val = 3.65028153987288;
    status = f->call(x, f_val);
    _ASSERT(ForBESUtils::is_status_ok(status));
    _ASSERT_NUM_EQ(correct_val, f_val, tolerance);

    delete f;
}

void TestSumOfNorm2::testDualNorm() {
    const size_t k = 3;
    const size_t n = 12;
    const double mu = 1.0;
    const double tolerance = 1e-8;

    double x_data[n] = {
        1, -2, -1,
        2, -1, 1,
        0, 1, 3,
        2, 1.5, 0.5
    };

    const double correct_val = 10.6107669025311;
    const double correct_val_dual = 3.16227766016838;

    Norm * son = new SumOfNorm2(mu, k);

    int status = -1;

    Matrix x = MatrixFactory::ShallowVector(x_data, n, static_cast<size_t> (0));
    double f_val;
    status = son->call(x, f_val);
    _ASSERT(ForBESUtils::is_status_ok(status));
    _ASSERT_NUM_EQ(correct_val, f_val, tolerance);

    double f_val_dual;
    status = son->dualNorm(x, f_val_dual);
    _ASSERT(ForBESUtils::is_status_ok(status));
    _ASSERT_NUM_EQ(correct_val_dual, f_val_dual, tolerance);

    delete son;
}

void TestSumOfNorm2::testProx() {
    const size_t k = 3;
    const size_t n = 12;
    const double mu = 1.0;
    double gamma = 0.8;

    double v_data[n] = {
        -1, 1, -1,
        3, -1, 1,
        1, -1.5, 2,
        -2, 1.5, 0.5
    };

    Norm * son = new SumOfNorm2(mu, k);

    Matrix prox(n, 1);
    Matrix v = MatrixFactory::ShallowVector(v_data, n, static_cast<size_t> (0));
    int status;
    status = son->callProx(v, gamma, prox);


    _ASSERT(ForBESUtils::is_status_ok(status));

    double prox_correct_data[] = {
        -0.538119784648299,
         0.538119784648299,
        -0.538119784648299,
         2.276372773013367,
        -0.758790924337789,
         0.758790924337789,
         0.702887458916717,
        -1.054331188375075,
         1.405774917833434,
        -1.372428367557811,
         1.029321275668358,
         0.343107091889453
    };
    
    Matrix prox_correct = MatrixFactory::ShallowVector(prox_correct_data, n, 0);
    _ASSERT_EQ(prox_correct, prox);

    delete son;



}



