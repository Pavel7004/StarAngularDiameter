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


vector<vector<double>> Trans(vector<vector<double>>A)
{
    vector<vector<double>>tA(3, vector<double>(3));
    tA = A;
    for (int i = 0; i < tA.size(); ++i)
    {
        for (int j = 0; j <= i; ++j)
        {
            swap(tA[i][j], tA[j][i]);
        }
    }
    return tA;
}
vector<vector<double>> xA(vector<vector<double>>A)
{
    vector<vector<double>>x(3, vector<double>(3));
    x[0][0] = A[1][1] * A[2][2] - A[2][1] * A[1][2];
    x[0][1] = A[1][0] * A[2][2] - A[2][0] * A[1][2];
    x[0][2] = A[1][0] * A[2][1] - A[2][0] * A[1][1];

    x[1][0] = A[0][1] * A[2][2] - A[2][1] * A[0][2];
    x[1][1] = A[0][0] * A[2][2] - A[2][0] * A[0][2];
    x[1][2] = A[0][0] * A[2][1] - A[2][0] * A[0][1];

    x[2][0] = A[0][1] * A[1][2] - A[1][1] * A[0][2];
    x[2][1] = A[0][0] * A[1][2] - A[1][0] * A[0][2];
    x[2][2] = A[0][0] * A[1][1] - A[1][0] * A[0][1];

    x[0][1] *= -1.0;
    x[1][0] *= -1.0;
    x[1][2] *= -1.0;
    x[2][1] *= -1.0;
    return x;
}
double det(vector<vector<double>>A)
{
    return  A[0][0] * A[1][1] * A[2][2] + A[0][1] * A[1][2] * A[2][0] + A[1][0] * A[2][1] * A[0][2] - A[2][0] * A[1][1] * A[0][2] - A[0][0] * A[1][2] * A[2][1] - A[1][0] * A[0][1] * A[2][2];
}
vector<vector<double>> r(vector<vector<double>>A)
{
    vector<vector<double>> rA(3, vector<double>(3));
    rA = xA(Trans(A));
   // print(rA);
    double d = det(A);
    if (d == 0)
        return { {0,0,0},{0,0,0},{0,0,0} };
    for (int i = 0; i < rA.size(); ++i)
    {
        for (int j = 0; j < rA.size(); ++j)
        {
            rA[i][j] =rA[i][j]/ d;
        }
    }
    
    return rA;
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
    vector<double> AB(B.size(),0);
    for (int i = 0; i < A.size(); ++i)
    {
        for (int j = 0; j < B.size(); ++j)
        {
            AB[i]+= A[i][j] * B[j];
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

double f(double x,vector<double>B)
{
    return B[0]*x*x+B[1]*x+B[2];
}
double rr(int x,vector<pair<double,double>>data, vector<double> B)
{
    return f(data[x].first, B) - data[x].second;
}
vector<double> df(double x, vector<double>B,vector<pair<double,double>>data)
{
    double dt = 1;
    //cout << rr(x, data, { B[0] + B[0] / dt,B[1],B[2] }) << " " << rr(x, data, B) <<" "<< (B[0] / dt) << endl;
    double dx = (rr(x,data, { B[0] + B[0] / dt,B[1],B[2] }) - rr(x,data, B)) / (B[0] / dt);
    double dy = (rr(x,data, { B[0] ,B[1] + B[1] / dt,B[2] }) - rr(x,data, B)) / (B[1] / dt);
    double dz = (rr(x,data, { B[0] ,B[1],B[2] + B[2] / dt }) - rr(x,data, B)) / (B[2] / dt);

   /* double dx = (f(x, { B[0] + B[0] / 10000.0,B[1],B[2] })-f(x,B))/ (B[0] / 10000.0);
    double dy = (f(x, { B[0] ,B[1]+ B[1] / 10000.0,B[2] }) - f(x, B))/ (B[1] / 10000.0);
    double dz = (f(x, { B[0] ,B[1],B[2] + B[2] / 10000.0 }) - f(x, B))/ (B[2] / 10000.0);*/
    //print({ dx,dy,dz });
    return { dx,dy,dz };
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
    freopen("matrix.txt", "r", stdin);
    vector < pair<double, double>>data = { {-4.5,5.02},{0.2,-4.3},{7.6,1.6} };
    vector<double>B = { 1,1,1 };
    for (int i = 0; i < 3; ++i)
    {
        print(B);
        //cout << "R: " << sr(data, B) << endl;
        B = B-r(Trans(Jacobian(data,B))*Jacobian(data,B))*rR(data,B);
    }
    

   // print(A-C);
   /* vector<vector<double>> J;
    J = Jacobian(data, B);
    //print(J);
    print(r(J)*rR(data,B));
  //  print(rR(data,B));
    vector<vector<double>>M={ { 1,2,1 }, { -2,1,1 }, { 3,1,-2 } };
   /* print(rR(data, B));
    print(B-r(Trans(J) * J) * Trans(J)*rR(data,B));*/
   
}

