#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
#include <cmath>

using namespace std;

class simplex 
    {
    public:
    vector<double> c; 
    vector<vector<double>> A; 
    vector<double> b; 
    vector<vector<double>> table; 
    vector<string> SvX; 
    vector<string> ZX; 
    string method; 
    int razStl = -1; 
    int razStr = -1; 

    simplex (vector<double> c, vector<vector<double>> A, vector<double> b, string method) 
        {
        if (method == "min") 
            {
            this->c = c;
            } 
            else 
            {
            for (int i = 0; i < c.size(); i++) 
                {
                c[i] = -c[i];
                }
            this->c = c;
            }
            
        this->A = A;
        this->b = b;
        this->method = method;
        FillTable();

        SvX.resize(c.size());
        int k = 0; 
        for (int i = 0; i < c.size(); i++) 
            {
            SvX [i] = "X" + to_string(i + 1);
            k++;
            }

        ZX.resize(b.size()); 
        for (int i = 0; i < b.size(); i++) 
            {
            k++;
            ZX [i] = "X" + to_string(k);
            }
        }

    bool Solution() 
        {
        cout << "Начальная задача: " << endl;
        Print();
        cout << endl;

        if (FindOptSolve()) 
            {
            if (method == "max") 
                {
                table[0][b.size()] = -table[0][b.size()];
                Print();
                cout << endl;
                }
            Check();
            return true;
            }
        return false;
        }

    bool FindOptSolve() 
        {
        if (FindOprSolve()) 
            {
            int l = 0; 
            for (int t = 1; t < c.size() + 1; t++) if (table[t][b.size()] < 0) { l++; }
            if (l == c.size()) { return true; } 

            for (int i = 1; i < c.size() + 1; i++) 
                {
                if (table[i][b.size()] > 0) 
                    {
                    int razr_stolb = i;

                    double min = numeric_limits<double>::max(); 
                    
                    int flag2 = 0; 
                    int razr_str = -1;

                    for (int z = 0; z < b.size(); z++) 
                        {
                        double k = table[0][z] / table[razr_stolb][z];
                        if (k < min && k > 0) 
                            {
                            min = k;
                            razr_str = z;
                            flag2++;
                            }
                        }
                    if (flag2 == 0) { continue; }
                    if (razr_str != razStr && razr_stolb != razStl) 
                        {
                        razStl = razr_stolb;
                        razStr = razr_str;
                        NewTable (razr_str, razr_stolb);
                        } 
                        else 
                        {
                        continue;
                        }
                    return FindOptSolve();
                    }
                }
            }
        return false;
        }

    void FillTable() 
        { 
        table.resize(c.size() + 1, vector<double>(b.size() + 1));

        for (int i = 0; i < b.size(); i++) { table[0][i] = b[i]; } 
        table[0][b.size()] = 0;

        for (int i = 0; i < b.size(); i++) 
            {
            for (int j = 1; j < c.size() + 1; j++) 
                {
                table[j][i] = A[i][j - 1];
                }
            }

        for (int i = 1; i < c.size() + 1; i++) { table[i][b.size()] = -c[i - 1]; } 
        }

    bool FindOprSolve() 
        { 
        int l = 0; 
        for (int t = 0; t < b.size(); t++) if (table[0][t] >= 0) { l++; }
        if (l == b.size()) { return true; }

        for (int i = 0; i < b.size(); i++) 
            {
            if (table[0][i] < 0) 
                {
                for (int j = 1; j < c.size() + 1; j++) 
                    {
                    if (table[j][i] < 0) 
                        {
                        int razr_stolb = j;

                        double min = numeric_limits<double>::max(); 

                        int flag1 = 0; 
                        int razr_str = -1;

                        for (int z = 0; z < b.size(); z++) 
                            {
                            double k = table[0][z] / table[razr_stolb][z];
                            if (k < min && k > 0)
                                {
                                min = k;
                                razr_str = z;
                                flag1++;
                                }
                            }
                        if (flag1 == 0) { continue; }

                        if (razr_str != razStr && razr_stolb != razStl) 
                            {
                            razStl = razr_stolb;
                            razStr = razr_str;
                            NewTable (razr_str, razr_stolb);
                            } 
                            else 
                            {
                            continue;
                            }
                        return FindOprSolve();
                        }
                    }
                }
            }
        return false;
        }

    void NewTable (int razr_str, int razr_stolb) 
        { 
        vector<vector<double>> table1(c.size() + 1, vector<double>(b.size() + 1));
        double RazElem = table[razr_stolb][razr_str]; 

        string _x = SvX [razr_stolb - 1];
        SvX [razr_stolb - 1] = ZX [razr_str];
        ZX [razr_str] = _x;

        table1[razr_stolb][razr_str] = 1 / RazElem;

        for (int i = 0; i < c.size() + 1; i++)
            {
            if (i != razr_stolb) table1[i][razr_str] = table[i][razr_str] / RazElem;
            }

        for (int i = 0; i < b.size() + 1; i++) 
            {
            if (i != razr_str) table1[razr_stolb][i] = -table[razr_stolb][i] / RazElem;
            }

        for (int i = 0; i < b.size() + 1; i++) 
            {
            for (int j = 0; j < c.size() + 1; j++) 
                {
                if (i != razr_str && j != razr_stolb) table1[j][i] = table[j][i] - (table[razr_stolb][i] * table[j][razr_str]) / RazElem;
                }
            }
        table = table1;

        Print();
        cout << endl;
        }

    void Print() 
        { 
        cout << "\t";
        cout << " S" << "\t";
        for (const auto& obj : SvX) { cout << " " << obj << "\t"; }
        cout << endl;
        cout << endl;

        for (int i = 0; i < b.size() + 1; i++) 
            {
            if (i != b.size()) cout << ZX[i] << "\t"; else cout << "F" << "\t";

            for (int j = 0; j < c.size() + 1; j++) 
                {
                if (table[j][i] >= 0) cout << " " << round(table[j][i] * 100.0) / 100.0 << "\t"; 
                else cout << round(table[j][i] * 100.0) / 100.0 << "\t";
                }
            cout << endl;
            }
        cout << "______________________________________________" << endl;
        }

    void Check() 
        { 
        cout << "Проверка решения: " << endl;
        cout << endl;
        cout << "Функция: " << endl;
        vector<double> solve_x(table.size() + table[0].size() - 2); 
        vector<string> str_X(solve_x.size()); 
        for (int i = 0; i < solve_x.size(); i++) str_X[i] = "X" + to_string(i + 1);

        for (int i = 0; i < ZX.size(); i++) 
            {
            int k = find(str_X.begin(), str_X.end(), ZX[i]) - str_X.begin(); 
            solve_x[k] = table[0][i];
            }

        for (int i = 0; i < solve_x.size() / 2; i++) 
            {
            if (method == "min" && i != solve_x.size() / 2 - 1) cout << c[i] << "*" << solve_x[i] << " + ";
            if (method == "min" && i == solve_x.size() / 2 - 1) cout << c[i] << "*" << solve_x[i] << " = " << table[0][table[0].size() - 1] << endl;
            if (method == "max" && i != solve_x.size() / 2 - 1) cout << -c[i] << "*" << solve_x[i] << " + ";
            if (method == "max" && i == solve_x.size() / 2 - 1) cout << -c[i] << "*" << solve_x[i] << " = " << table[0][table[0].size() - 1] << endl;
            }

        cout << endl;
        cout << "Неравенства: " << endl;

        for (int i = 0; i < b.size(); i++) 
            {
            for (int j = 0; j < solve_x.size() / 2; j++) 
                {
                if (j != solve_x.size() / 2 - 1) cout << A[i][j] << "*" << solve_x[j] << " + ";
                if (j == solve_x.size() / 2 - 1) cout << A[i][j] << "*" << solve_x[j] << " <= " << b[i] << endl;
                }
            }
        cout << "______________________________________________" << endl;
        cout << endl;
        }
    };

int main() 
    {
    vector<double> c = {5.0, 3.0, 6.0};
    vector<double> b = {7.0, 3.0, 3.0};
    vector<vector<double>> A = {{1.0, 1.0, 1.0}, {1.0, 3.0, 0.0}, {0.0, 0.5, 2.0}};

    simplex t(c, A, b, "max");
    if (!t.Solution()) 
        {
        cout << "Решение не найдено" << endl;
        }
        
    return 0;
    }
