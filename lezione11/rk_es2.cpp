/*
  Amerio Aurelio
  Algoritmi numerici per la fisica
  Lezione 11

  runge-kutta method
*/

#include <fstream>
#include <iomanip>
#include <iostream>
#include <math.h>
using namespace std;

void rhs(double x, double *Y, double *R) {
  double xdot, ydot;
  xdot = Y[0];
  ydot = Y[1];
  R[0] = ydot;
  R[1] = -xdot;
  return;
}

void RKStep(double t, double *Y, void (*Ydot)(double, double *, double *),
            double dt, int neq) {
  // Runge-Kutta step using Midpoint integration

  double Y1[neq]; // il vettore che contiene le soluzioni approssimate
  double k1[neq]; // rhs 1
  double k2[neq]; // rhs 2

  Ydot(t, Y, k1); // calcola il primo rhs per determinare Y1 approx
  for (int n = 0; n < neq; n++) {
    Y1[n] = Y[n] + 0.5 * dt * k1[n];
  }

  Ydot(t + 0.5 * dt, Y1,
       k2); // calcola il secondo rhs per determinare Y raffinato
  for (int n = 0; n < neq; n++) {
    Y[n] += dt * k2[n];
  }

  return;
}

void RK4Step(double t, double *Y, void (*Ydot)(double, double *, double *),
             double dt, int neq) {
  // Runge-Kutta step using Midpoint integration

  double Y1[neq], Y2[neq], Y3[neq];
  double k1[neq];
  double k2[neq];
  double k3[neq];
  double k4[neq];

  Ydot(t, Y, k1); // set k1

  // compute Y1 for k2
  for (int n = 0; n < neq; n++) {
    Y1[n] = Y[n] + 0.5 * dt * k1[n];
  }

  Ydot(t + 0.5 * dt, Y1, k2); // set k2

  // compute Y2 for k3
  for (int n = 0; n < neq; n++) {
    Y2[n] = Y[n] + 0.5 * dt * k2[n];
  }

  // compute Y3 for k4
  Ydot(t + 0.5 * dt, Y2, k3); // set k3
  for (int n = 0; n < neq; n++) {
    Y3[n] = Y[n] + dt * k3[n];
  }

  Ydot(t + dt, Y3, k4); // set k4

  // sum intermediate computations
  for (int n = 0; n < neq; n++) {
    Y[n] += dt / 6. * (k1[n] + 2 * k2[n] + 2 * k3[n] + k4[n]);
  }

  return;
}

void EulerStep(double t, double *Y, void (*Ydot)(double, double *, double *),
               double dt, int neq) {
  double rhs[neq]; // il vettore che contiene gli elementi rhs
  Ydot(t, Y,
       rhs); // la funzione che calcole gli elemnti di destra del sist di eq
  for (int n = 0; n < neq; n++) {
    Y[n] += dt * rhs[n];
  }
  return;
}

// double exact(double x) { return exp(-x * x / 2); }

int main() {
  ofstream file, file2, file3;
  file.open("rk_ode.dat");
  file2.open("rk4_ode.dat");
  file3.open("eu_ode.dat");
  // file2.open("euler_ode.dat");
  double pi=3.1415926;
  double tMAX = 20. * pi;
  int n_steps = 200;
  double dt = tMAX / n_steps;
  int neq = 2;

  double Y[] = {1, 0};
  double t = 0;
  file << Y[0] << " " << Y[1] << endl;

  cout << endl;
  cout << "rk: \n";
  for (int n = 0; n < n_steps; n++) {
    RKStep(t, Y, rhs, dt, neq);
    t += dt;

    file << Y[0] << " " << Y[1] << endl;
    cout << "x^2 + y^2 = " << Y[0] * Y[0] + Y[1] * Y[1] << endl;
  }
  cout << "x(20π): " << Y[0] << "\ty(20π): " << Y[1] << endl;

  Y[0] = 1;
  Y[1] = 0;
  t = 0;

  cout << endl;
  cout << "euler: \n";
  file3 << Y[0] << " " << Y[1] << endl;
  for (int n = 0; n < n_steps; n++) {
    EulerStep(t, Y, rhs, dt, neq);
    t += dt;

    file3 << Y[0] << " " << Y[1] << endl;
    cout << "x^2 + y^2 = " << Y[0] * Y[0] + Y[1] * Y[1] << endl;
  }

  cout << "x(20π): " << Y[0] << "\ty(20π): " << Y[1] << endl;
  // file << t << " " << Y[0] << " " << Y[1] << endl;

  Y[0] = 1;
  Y[1] = 0;
  t = 0;
  file2 << Y[0] << " " << Y[1] << endl;

  cout << endl;
  cout << "rk4: \n";
  for (int n = 0; n < n_steps; n++) {
    RK4Step(t, Y, rhs, dt, neq);
    t += dt;

    file2 <<Y[0] << " " << Y[1] << endl;
    cout << "x^2 + y^2 = " << Y[0] * Y[0] + Y[1] * Y[1] << endl;
  }

  cout << "x(20π): " << Y[0] << "\ty(20π): " << Y[1] << endl;
  // file << t << " " << Y[0] << " " << Y[1] << endl;

  file.close();
  file2.close();
  file3.close();
  return 0;
}
