#include "FBCache.h"
#include "LinearOperator.h"

FBCache::FBCache(FBProblem & p, Matrix & x, double gamma) : m_prob(p), m_x(x), m_gamma(gamma) {
    // store pointers to problem and all relevant details
    this->m_f1 = p.f1();
    this->m_L1 = p.L1();
    this->m_d1 = p.d1();
    this->m_f2 = p.f2();
    this->m_L2 = p.L2();
    this->m_d2 = p.d2();
    this->m_lin = p.lin();
    this->m_g = p.g();

    // allocate memory for residuals and gradients (where needed)
    if (m_f1 != NULL) {
        m_res1x = new Matrix();
        m_gradf1x = new Matrix();
    } else {
        m_res1x = NULL;
        m_gradf1x = NULL;
    }
    if (m_f2 != NULL) {
        m_res2x = new Matrix();
        m_gradf2x = new Matrix();
    } else {
        m_res2x = NULL;
        m_gradf2x = NULL;
    }
    m_gradfx = new Matrix();
    m_z = new Matrix();
    m_y = new Matrix();
    
    m_flag_evalf = -1;
    m_flag_gradstep = -1;
    m_flag_proxgradstep = -1;
    
    m_f1x = 0.0;
    m_f2x = 0.0;
    m_linx = 0.0;
    m_fx = 0.0;
    m_gz = 0.0;
}

FBCache::~FBCache() {
    if (m_z != NULL) {
        delete m_z;
        m_z = NULL;
    }
    if (m_y != NULL) {
        delete m_y;
        m_y = NULL;
    }
    if (m_res2x != NULL) {
        delete m_res2x;
        m_res2x = NULL;
    }
    if (m_gradf2x != NULL) {
        delete m_gradf2x;
        m_gradf2x = NULL;
    }
    if (m_res1x != NULL) {
        delete m_res1x;
        m_res1x = NULL;
    }
    if (m_gradf1x != NULL) {
        delete m_gradf1x;
        m_gradf1x = NULL;
    }
    if (m_gradfx != NULL){
        delete m_gradfx;
        m_gradfx = NULL;
    }

}

int FBCache::update_eval_f() {
    if (m_flag_evalf == 1) {
        return ForBESUtils::STATUS_OK;
    }

    if (m_f1 != NULL) {
        if (m_L1) {
            *m_res1x = m_L1->call(m_x);
        } else {
            *m_res1x = m_x;
        }
        if (m_d1) {
            *m_res1x += *m_d1;
        }
        int status = m_f1->call(*m_res1x, m_f1x, *m_gradf1x);
        if (ForBESUtils::STATUS_OK != status) {
            return status;
        }
    }

    if (m_f2 != NULL) {
        if (m_L2 != NULL) *m_res2x = m_L2->call(m_x);
        else *m_res2x = m_x;
        if (m_d2 != NULL) *m_res2x += *m_d2;
        int status = m_f2->call(*m_res2x, m_f2x);
        if (ForBESUtils::STATUS_OK != status) {
            return status;
        }
    }

    if (m_lin != NULL) {
        m_linx = ((*m_lin) * m_x)[0];
    }

    m_fx = m_f1x + m_f2x + m_linx;
    m_flag_evalf = 1;
    return ForBESUtils::STATUS_OK;
}

int FBCache::update_forward_step(double gamma) {
    if (m_flag_evalf == 0) {
        this->update_eval_f();
    }
    if (m_flag_gradstep == 1) {
        if (gamma != this->m_gamma) {
            this->m_gamma = gamma;
            *m_y = m_x - gamma * (*m_gradfx);
        }
    }
    if (m_f1 != NULL) {
        if (m_L1) {
            Matrix d_gradfx = m_L1->callAdjoint(*m_gradf1x);
            *m_gradfx += d_gradfx;
        } else {
            *m_gradfx += *m_gradf1x;
        }
    }
    if (m_f2 != NULL) {
        m_f2->call(m_x, m_f2x, *m_gradf2x);
        if (m_L2) {
            Matrix d_gradfx = m_L2->callAdjoint(*m_gradf2x);
            *m_gradfx += d_gradfx;
        } else {
            *m_gradfx += *m_gradf2x;
        }
    }
    if (m_lin) {
        *m_gradfx += (*m_lin);
    }
    *m_y = m_x - gamma * (*m_gradfx);
    m_flag_gradstep = 1;
    return ForBESUtils::STATUS_OK;
}

int FBCache::update_forward_backward_step(double gamma) {
    double gamma0 = this->m_gamma;

    if (m_flag_gradstep == 0 || gamma != gamma0) {
        int status = this->update_forward_step(gamma);
        if (ForBESUtils::STATUS_OK != status) {
            return status;
        }
    }

    if (m_flag_proxgradstep == 0 || gamma != gamma0) {
        int status = m_g->callProx(m_x, gamma, *m_z, m_gz);
        if (ForBESUtils::STATUS_OK != status) {
            return status;
        }
        m_fx = m_f1x + m_f2x + m_linx;
    }
    m_flag_proxgradstep = 1;
    return ForBESUtils::STATUS_OK;
}

int FBCache::update_eval_FBE(double gamma) {
    return ForBESUtils::STATUS_UNDEFINED_FUNCTION;
}

int FBCache::update_grad_FBE(double gamma) {
    return ForBESUtils::STATUS_UNDEFINED_FUNCTION;
}

