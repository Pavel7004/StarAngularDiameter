#include <iostream>
#include <math.h>


using namespace std;

const double pi=acos(-1.0);         //число е
const double I0=100;                    //интенсивность свечения
const double Radius=24.;                     //радиус апетуры
const double lyambda1=6250,lyambda2=7750;     //интервал длинны волны
const double l=3.64825e8;                      //расстояние до Луны
const double P1=1.;                    //хз
const double P2=0.;                     //хз
const double L0=2.;                     //хз
const double deltat=2e-3;
const double V=700.;
const double t0=8e4;
const double R0=2.5; 
 
double sigma(double y)
{
    return 2.*sqrt(Radius*Radius-y*y);
}

/*double E0(double lyambda)   //возможно константа
{
    return 0;
}
double P(double lyambda)    //возможно константа
{
    return 0;
}
double R(double lyambda)    //возможно константа
{
    return 0;
}
*/
double E(double lyambda)
{
    return 1;
}

double S(double omega)
{
    return 2./3.*omega*sin(pi/8.*omega*omega)+sin(pi/2.*omega*omega);
}
double C(double omega)
{
    return omega/6.+2./3.*omega*cos(pi/8.*omega*omega)+cos(pi/2.*omega*omega);
}

double G0(double x)
{
    return I0/8.*(2.*x+4*x*C(x)-4./pi*sin(pi/2.*x*x)+4.*x*S(x)+4./pi*cos(pi/2.*x*x)+4.*x*C(x)*C(x)-8./pi*sin(pi/2.*x*x)*C(x)+4.*x*S(x)*S(x)+8./pi*cos(pi/2.*x*x)*S(x));
}
double g(double x,double lyambda)
{
    return sqrt(lyambda*l/2.)*G0(x*sqrt(2./lyambda*l)*E(lyambda));
}
double G1(double x)
{
    return (lyambda2-lyambda1)/6.*(g(x,lyambda1)+4*g(x,(lyambda2+lyambda1)/2.)+g(x,lyambda2));
}
double G2(double x)
{
    return 8./3.*Radius*Radius*G1(x);
}
double G3(double x,double R0)
{
    return R0*R0/3.*G2(x);
}
double G4(double x,double R0)
{
    return R0*R0*G2(x);
}

double T1(double t,double V,double t0,double R0)
{
    
    return 1./V*(G3(V*(t+deltat-t0),R0)-G3(V*(t-deltat-t0),R0));
}
double T2(double t,double V,double t0,double R0)
{
    return 1./V*(G4(V*(t+deltat-t0),R0)-G4(V*(t-deltat-t0),R0));
}

double T(double t)
{
    return P1*T1(t,V,t0,R0)+P2*T2(t,V,t0,R0)+L0;
}

int main()
{
    for(double i=t0;i<t0+deltat*1000.;i+=deltat)
    {
        printf("%.10e %.10e\n",i-t0,T(i));
    }
    return 0;
}