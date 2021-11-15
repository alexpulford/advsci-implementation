#include <iostream>
#include <iomanip>
#include "spline.h"
#include "quad.h"

using namespace std;

int main()
{
    cout.precision(std::numeric_limits<double>::max_digits10-1);

    const auto epsilon = std::numeric_limits<double>::epsilon()/2.0;

    if(true) {
        cout << "DEMO: C1 to C2 via Homotopy Continuation\n";
        cout << "N = 39\n";

        Knot src = Spline::constructKnot(39, 3);
        Knot tgt = Spline::constructKnotM2(39, 3);

        //First compute an accurate C2 rule by guessing
        Spline s = Spline(src);
        s.compute();
        Quad q = Quad::constructGuess(39, 3, src);
        q = q.newtons_min(s, 20);

        //Now find the C1 rule
        q = q.continuation_edge(src, tgt, 100, epsilon);
        //Iterate result
        s = Spline(tgt);
        s.compute();
        q = q.newtons(s, epsilon);
        //Print quad
        cout << "C1 ERROR: " << q.error(s) << "\n";
        cout << "C1 QUAD:\n" << q << "\n";

        //Now do it all in reverse to get C2
        q = q.continuation_edge(tgt, src, 100, epsilon);
        //Iterate result
        s = Spline(src);
        s.compute();
        q = q.newtons_min(s, 20);
        //Print quad
        //q = q.truncate();
        cout << "C2 ERROR: " << q.error(s) << "\n";
        cout << "C2 QUAD:\n" << q << "\n\n\n";
    }

    //Construct Quad via Homotopy continuation (C2 Uniform to C2 Non-Uniform)
    if(true) {
        cout << "DEMO: C2 Uniform to C2 Non-Uniform via Homotopy Continuation\n";

        Knot a = Spline::constructKnot(5, 3);
        Knot b(12);
        b << -0.6, -0.55, -0.5, 0, 0.4, 0.45, 0.55, 0.6, 1, 1.5, 1.55, 1.6;

        Spline s = Spline(b);
        s.compute();

        Quad q = Quad::constructGuess(5, 3, a);
        q = q.continuation_edge(a, b, 1000, epsilon);
        cout << "CONT ERROR: " << q.error(s) << "\n";
        cout << "CONT QUAD:\n" << q << "\n\n\n";
    }

    //Construct Quad from guess
    if(true) {
        cout << "DEMO: C2 Uniform from Guess\n";
        cout << "N = 101\n";
        Knot k = Spline::constructKnot(101, 3);
        Quad q = Quad::constructGuess(101, 3, k);
        Spline s = Spline(k);
        s.compute();

        cout << "NEWTONS START\n";
        q = q.newtons_min(s, 20);
        cout << "GUESS ERROR: " << q.error(s) << "\n";
        cout << "GUESS QUAD:\n" << q << "\n\n\n";
    }

    //report graph 1
    if(false){
        for(int i = 5; i <= 501; i+=2) {
            Knot k = Spline::constructKnot(i, 3);
            Quad q = Quad::constructGuess(i, 3, k);

            Spline s = Spline(k);
            s.compute();

            q = q.newtons_min(s, 25);

            cout << "[" << i << ", " << q.error(s) << "],\n";
        }
    }

    //report graph 2
    if(false){
        cout << "[";
        for(int i = 0; i < 9; i++) {
            auto n = i*100 + 101;
            Knot k = Spline::constructKnot(n, 3);
            Quad q = Quad::constructGuess(n, 3, k);
            Spline s = Spline(k);
            s.compute();
            cout << "[";
            q = q.newtons_min(s, 25);
            cout << "]," << "\n";
        }
        cout << "]" << "\n";
    }

    //report graph 3
    if(false){
        Knot k = Spline::constructKnot(81, 3);
        Quad q = Quad::constructGuess(81, 3, k);
        Spline s = Spline(k);
        s.compute();
        q = q.newtons_min(s, 10000);
    }
}
