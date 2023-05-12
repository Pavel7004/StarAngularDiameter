#include <iostream>
#include <vector>
#include <math.h>
#include <iomanip>
using namespace std;


void print(vector<vector<double>> vec)
{
    for (int i = 0; i < vec.size(); ++i)
    {
        for (int j = 0; j < vec[0].size(); ++j)
        {
            cout << vec[i][j] << " ";
        }
        cout << endl;
    }
    cout << "------------------\n";
}
void print(vector<double>vec)
{
    for(int i=0;i<vec.size();++i)
    {
        cout<<vec[i]<<" ";
    }
    cout<<endl;
}

vector<vector<double>> operator* (vector<vector<double>> A, vector<vector<double>> B)
{
    vector<vector<double>> AB(A.size(), vector<double>(A.size()));

    for (int i = 0; i < A.size(); ++i)
    {
        for (int j = 0; j < B[0].size(); ++j)
        {
            for (int k = 0; k < A[0].size(); ++k)
            {
                AB[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    return AB;
}
vector<double> operator* (vector<vector<double>> A, vector<double> B)
{
    vector<double> AB(B.size(), 0);
    for (int i = 0; i < A.size(); ++i)
    {
        for (int j = 0; j < B.size(); ++j)
        {
            AB[i] += A[i][j] * B[j];
        }
    }
    return AB;
}
vector<double> operator- (vector<double> A, vector<double> B)
{
    vector<double>AB;
    for (int i = 0; i < A.size(); ++i)
        AB.push_back(A[i] - B[i]);
    return AB;
}
vector<double> operator* (vector<double>A, double c)
{
    for (int i = 0; i < A.size(); ++i)
        A[i] *= c;
    return A;
}

vector<vector<double>> Cholecky(vector<vector<double>>A)
{
    vector<vector<double>> L = A;
    for (int i = 0; i < A.size(); ++i)
    {
        for (int j = i + 1; j < A.size(); ++j)
        {
            L[i][j] = 0;
        }
    }
    for (std::size_t i = 0; i < A.size(); ++i)
    {
        for (std::size_t j = 0; j <= i; ++j)
        {
            double sum = 0;
            for (std::size_t k = 0; k < j; ++k)
                sum += L[i][k] * L[j][k];


            if (i == j)
                L[i][j] = sqrt(L[i][i] - sum);
            else
                L[i][j] = (1.0 / L[j][j] * (L[i][j] - sum));
        }
    }
    return L;
}
vector<vector<double>> Trans(vector<vector<double>>A)
{
    vector<vector<double>>tA = A;
    for (int i = 0; i < tA.size(); ++i)
    {
        for (int j = 0; j <= i; ++j)
        {
            swap(tA[i][j], tA[j][i]);
        }
    }
    return tA;
}
vector<vector<double>> r(vector<vector<double>>A)
{
    vector<vector<double>> rA(A.size(), vector<double>(A.size()));
    for (int i = 0; i < rA.size(); ++i)
    {
        for (int j = 0; j < rA.size(); ++j)
        {
            rA[i][j] = 0;
            if(i==j)
                rA[i][j] = 1;
        }
    }
    vector<vector<double>> L =Cholecky(A);
    for (int i = 0; i < L.size(); ++i)
    {
        rA[i]=rA[i] * (1.0 / L[i][i]);
        for (int j = i+1; j < rA.size(); ++j)
        {
            rA[j] = rA[j] - rA[i]*L[j][i];
        }
    }

    return Trans(rA)*rA;//недописал
}



double f(double x,vector<double>B)
{
    return B[0]*x*x*x+B[1]*x*x+B[2]*x+B[3];
}
double rr(int x,vector<pair<double,double>>data, vector<double> B)
{
    return f(data[x].first, B) - data[x].second;
}
vector<double> df(double x, vector<double>B,vector<pair<double,double>>data)
{
    double dt = 100;
    double dx = (rr(x,data, { B[0] + B[0] / dt,B[1],B[2],B[3] }) - rr(x,data, B)) / (B[0] / dt);
    double dy = (rr(x,data, { B[0] ,B[1] + B[1] / dt,B[2],B[3] }) - rr(x,data, B)) / (B[1] / dt);
    double dz = (rr(x,data, { B[0] ,B[1],B[2] + B[2] / dt,B[3] }) - rr(x,data, B)) / (B[2] / dt);
    double dk = (rr(x, data, { B[0] ,B[1],B[2], B[3]+B[3] / dt }) - rr(x, data, B)) / (B[3] / dt);
    return { dx,dy,dz,dk };
}
vector<vector<double>>Jacobian(vector < pair<double, double>>data,vector<double>B)
{
    vector<vector<double>>J;
    for (int i = 0; i < data.size(); ++i)
    {
        J.push_back({ df(i,B,data) });
    }
    return J;
}

vector<double>rR(vector < pair<double, double>>data, vector<double>B)
{
    vector<double>RR(data.size());

    for (int i = 0; i < RR.size(); ++i)
    {
        //cout << "f: " << f(data[i].first, B) << endl;
        RR[i] =f(data[i].first, B)- data[i].second ;
    }
    return RR;
}
double sr(vector < pair<double, double>>data,vector<double>B)
{
    double R=0;
    for(int i=0;i<data.size();++i)
    {
        R+= fabs(data[i].second*data[i].second-f(data[i].first,B)* f(data[i].first, B));
    }
    return R;
}

int main()
{
    //freopen("matrix.txt", "r", stdin);
    vector < pair<double, double>>data = { {0,0},{1. / 9.,0.0535},{0.2324,0},{1,-1} };
    vector<double>B = { 1,1,1,1 };
   // print(Cholecky({ {4,12,-16}, {12,37,-43},{-16,-43,98} }));
   for (int i = 0; i < 6; ++i)
    {
        print(B);
        B = B-r(Trans(Jacobian(data,B))*Jacobian(data,B))*Trans(Jacobian(data,B))*rR(data,B);
    }
    

   // print(r({ {81,-45,45},{-45,50,-15},{45,-15,38} }));
}

