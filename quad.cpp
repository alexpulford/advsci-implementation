#include "quad.h"

Float Quad::apply(Function* f) const {
    Float o = 0.0;
    for(int i = 0; i < size(); i+=2) {
        o += f->evaluate(operator()(i)) * operator()(i+1);
    }
    return o;
}

Float Quad::error(Spline s) {
    Float error = 0.0;
    Float tmp;
    for(PPoly b : s.basis) {
        tmp = abs(b.integral(0, 1) - apply(&b));
        error += tmp*tmp;
    }
    return std::sqrt(error.val())/(Float)s.basis.size();
}

Quad Quad::system(const VectorXFloat& q, Spline& s) {
    Quad o(s.basis.size());
    for(size_t i = 0; i < s.basis.size(); i++) {
        o(i) = ((Quad)q).apply(&s.basis[i]) - s.integrals(i);
    }
    return o;
}

Quad Quad::_newtons(Spline s) {
    VectorXFloat f, q = *this;
    MatrixXFloat j = jacobian(system, wrt(q), at(q, s), f);
    return q + j.colPivHouseholderQr().solve(-f);
}

Quad Quad::newtons(Spline s, Float error) {
    Float err = 1.0;
    auto q = Quad(*this);
    while(std::isfinite(err.val()) and err > error) {
        q = q._newtons(s);
        err = q.error(s);
        if(LOG_ERROR)
            std::cout << "ERROR: " << err << "\n";
    }
    if(!std::isfinite(err.val())) //jacobian is not invertible
        std::cout << "DIVERGED: " << err << "\n";
    return q;
}

Quad Quad::newtons_min(Spline s, int iterations) {
    Float err = 1.0;
    auto q = Quad(*this);

    Float mn_err = 1.0;
    auto mn_q = Quad(*this);

    for(int i = 0; i < iterations && std::isfinite(err.val()); i++) {
        q = q._newtons(s);
        err = q.error(s);

        if(err < mn_err) {
            mn_err = err;
            mn_q = Quad(q);
        }
        if(LOG_ERROR)
            std::cout << "ERROR: " << err << "\n";
    }
    if(!std::isfinite(err.val())) //jacobian is not invertible
        std::cout << "DIVERGED: " << err << "\n";
    return mn_q;
}


Quad Quad::continuation_edge(Knot a, Knot b, const int steps, const Float err) {
    Float diff = 0;
    for(int i = 0; i < a.size(); i++) {
        diff += abs(b(i) - a(i));
    }
    auto step = diff.val()/(Float)steps;

    Quad q = Quad(*this);
    Spline s;
    Float t = 0;
    int i, k;
    for(i = 0; i <= steps; i++) {
        t = i * step;

        s = Spline(a);
        k = 0;
        //traverse the hyper cube edge
        while(k < a.size() and t >= 0) {
            diff = b(k)-a(k);
            if(t > abs(diff)) {
                s(k) += diff;
                k++;
            } else {
                s(k) += t * (diff >= 0 ? 1 : -1);
                break;
            }
            t -= abs(diff).val();
        }
        s.compute();

        q = q.newtons(s, err);

        if(LOG_STEPS && i%(steps/10) == 0)
            std::cout << "STEP: " << i << " of " << steps << ", ERROR: " << q.error(s) << "\n";
    }

    return q;
}


Quad Quad::continuation(Knot a, Knot b, const int steps, const Float err) {
    const Float step = 1/(Float)steps;

    Quad q = Quad(*this);
    Spline s;
    Float t = 0;
    int i;
    for(i = 0; i <= steps; i++) {
        t = i * step;
        s = Spline((1-t)*a + t*b);
        s.compute();

        q = q.newtons(s, err);

        if(LOG_STEPS && i%(steps/10) == 0)
            std::cout << "STEP: " << i << " of " << steps << ", ERROR: " << q.error(s) << "\n";
    }

    return q;
}

Quad Quad::constructGuess(int N, int order, Knot k) {
    Quad q(N+order);
    int x = 0;
    for(int i = 0; i < N; i++) {
        if(i%2 == 1 or i ==0 or i == N-1) {
            q(x) = (k(i+order)+k(i+order+1))/2;
            q(x+1) = 0.1;
            x += 2;
        }
    }
    return q;
}


std::ostream& operator<<(std::ostream& os, const Quad& q) {
    os << '[';
    for(int i = 0; i < q.size(); i+=2) {
        std::cout << "(" << q(i) << ", " << q(i+1) << ")";
        if(i+1 != q.size()-1)
            std::cout << ",\n";
    }
    os << "]";
    return os;
}
