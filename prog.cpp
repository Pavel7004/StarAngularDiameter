#include <cmath>
#include <iostream>
const double pi = acos(-1.0); // Число е
const double I0 = 5.49;        // Интенсивность свечения ?
const double Radius = 24.0;   // Радиус апетуры
const double lyambda1 = 6250; // Интервал(начало) длины волны
const double lyambda2 = 7750; // Интервал(конец) длины волны
const double l = 3.64825e8; // Расстояние до Луны
const double P1 = 1.0;      // хз
const double P2 = 0.0;      // хз
const double L0 = 2.0;      // хз
const double deltat = 2e-3; // Шаг данных t ???
const double V = 726.0;     // Скорость центра диска луны
const double t0 = 8e-3;  // Начальное t
const double R0 = 2.5;      // Радиус проекции звезды
                       // на плоскость видимого диска луны


double C(double x)
{
  return x/6.0 * (1+4.0*cos(pi*x*x/8.0)+cos(pi*x*x/2.0));
}


double S(double x)
{
  return x/6.0 * (4.0*sin(pi*x*x/8.0)+sin(pi*x*x/2.0));
}

double G0(double x)
{
  return I0/8.0* (
    2.0*x+4.0*x*C(x)-4.0/pi*sin(pi*x*x/2.0)
    +4.0*S(x)+4.0/pi*cos(pi*x*x/2.0)+4.0*C(x)*C(x)-8.0/pi*sin(pi*x*x/2.0)*C(x)
    +4*x*S(x)*S(x)*S(x)+8.0/pi*cos(pi*x*x/2.0)*S(x)
  );
}
double g1(double x,double lyambda)
{
  return sqrt(lyambda*l/2.0)*G0(x*sqrt(2.0/(lyambda*l)));
}
double G1(double x)
{
  return (lyambda2-lyambda1)/6.0 * (g1(x,lyambda1)+4.0*g1(x,(lyambda1+lyambda2)/2.0)+g1(x,lyambda2));
}
double G2(double x)
{
  return 8.0 * Radius * Radius * G1(x) / 3.0;
}

double G4(double x)
{
  return 4.0/3.0*R0*G2(x);
}
double G3(double x)
{
  return 4.0/3.0*R0*R0*G2(x);
}
double T2(double t)
{
  return 1.0/V * ( G4(V*(t+deltat-t0))-G4(V*(t-deltat-t0)) ); 
}
double T1(double t)
{
  return 1.0/V * ( G3( V*(t+deltat-t0))-G3(V* (t-deltat-t0)) ); 
}

double T(double t)
{
  return P1*T1(t)+P2*T2(t)+L0;
}
int main()
{
  double t=t0+deltat;

  for(int i=0;i<55;++i)
  {
    printf("%f %f\n",t,T(t));
    t=t+2*deltat;
  }
  return 0;
}