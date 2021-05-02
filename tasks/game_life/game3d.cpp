#include <vector>
#include <Windows.h>
#include <iostream>
#include <algorithm>
#include <numeric>
#include <random>
using namespace std;
// 0 -------\ Y (m)
//   -------/
// ||
// ||
// ||
// ||
// \/ X (n)
enum TypeCell
{
    env,
    alive
};
struct Cell
{
    TypeCell type;
    friend ostream& operator<<(ostream& out, const Cell& cell)
    {
        if (cell.type == env) out << '.';
        else if (cell.type == alive) out << '#';
        return out;
    }
};
struct Field1D
{
    int n = 0;
    vector<Cell> cells;
    Field1D(int n):n(n), cells(vector<Cell>(n)) {}
    int getNumNeighbors(int pos, TypeCell type = alive, int radius = 1) const
    {
        int count = 0;
        for (int i = pos - radius; i <= pos + radius; i++)
            if (cells[(i + n) % n].type == type)
                count++;
        return count;
    }
    int getNum(TypeCell type = alive)
    {
        int count = 0;
        for (int i = 0; i < n; i++)
            if (cells[i].type == type) count++;
        return count;
    }
    Cell& operator[](int i) { return cells[i]; }
    Cell operator[](int i) const { return cells[i]; } // const вариант для cout
    friend ostream& operator<<(ostream& out, const Field1D& field)
    {
        for (int i = 0; i < field.n; i++)
            out << field[i];
        return out;
    }
};
struct Field2D
{
    int n = 0;
    int m = 0;
    vector<Field1D> cells;
    Field2D() {}
    Field2D(int n, int m): n(n), m(m), cells(vector<Field1D>(n, Field1D(m))) {}
    int getNumNeighbors(int posX, int posY, TypeCell type = alive, int radius = 1) const
    {
        int count = 0;
        for (int i = posX - radius; i <= posX + radius; i++)
            count += cells[(i + n) % n].getNumNeighbors(posY, type, radius);
        return count;
    }
    int getNum(TypeCell type = alive)
    {
        int count = 0;
        for (int i = 0; i < n; i++)
            count += cells[i].getNum(type);
        return count;
    }
    Field1D& operator[](int i) { return cells[i]; }
    Field1D operator[](int i) const { return cells[i]; }
    friend ostream& operator<<(ostream& out, const Field2D& field)
    {
        for (int i = 0; i < field.n; i++)
            out << field[i] << "\n";
        return out;
    }
};
struct Field3D
{
    int n = 0;
    int m = 0;
    int k = 0;
    vector<Field2D> cells;
    Field3D() {}
    Field3D(int n, int m, int k) : n(n), m(m), k(k), cells(vector<Field2D>(k, Field2D(n, m))) {}
    int getNumNeighbors(int posZ, int posX, int posY, TypeCell type = alive, int radius = 1) const
    {
        int count = 0;
        for (int i = posZ - radius; i <= posZ + radius; i++)
        {
            count += cells[(i + k) % k].getNumNeighbors(posX, posY, type, radius);
        }
        return count;
    }
    int getNum(TypeCell type = alive)
    {
        int count = 0;
        for (int i = 0; i < n; i++)
            count += cells[i].getNum(type);
        return count;
    }
    Field2D& operator[](int i) { return cells[i]; }
    Field2D operator[](int i) const { return cells[i]; }
    friend ostream& operator<<(ostream& out, const Field3D& field)
    {
        for (int i = 0; i < field.k; i++)
            out << i << ":\n" << field[i] << "\n";
        return out;
    }
};
struct iGame
{
    int n = 0;
    int m = 0;
    int k = 0;

    int seed = 0; // случайная величина для генератора
    double probability = 0.0;  // вероятность того, что клетка живая
    int dimension = 1; // размерность

    int radius = 1; // радиус проверки, граница включена
    int loneliness = 2; // с этого числа и меньше клетки умирают от одиночества
    int birth_start = 3; // с этого числа и до birth_end появляется живая клетка
    int birth_end = 3;
    int overpopulation = 5; // с этого числа и дальше клетки погибают от перенаселения
    virtual void runGame(int numIt) = 0;
    virtual double getProportion(TypeCell type) = 0;
};
struct Game2D : public iGame
{
    Field2D field;
    Field2D fieldNext;
    Game2D() { dimension = 2; }
    Game2D(int n, int m){
        this->n = n;
        this->m = m;
        dimension = 2;
        field = fieldNext = Field2D(n, m);
    }
    void setGame(double p, int s = 0)
    {
        probability = p;
        seed = s;
        field = Field2D(n, m);
        fieldNext = Field2D(n, m);
        vector<int> tmp(n * m);
        iota(tmp.begin(), tmp.end(), 0);
        shuffle(tmp.begin(), tmp.end(), std::mt19937(seed));
        for (int i = 0; i < (int)(p * n * m + 0.5); i++)
        {
            int x = tmp[i] / m; // была опечатка, было деление на n
            int y = tmp[i] % m;
            field[x][y].type = TypeCell::alive;
        }
    }
    void runGame(int numIt) override
    {
        for (int it = 0; it < numIt; it++)
        {
            for (int i = 0; i < n; i++)
            {
                for (int j = 0; j < m; j++)
                {
                    int count = field.getNumNeighbors(i, j);
                    fieldNext[i][j].type = field[i][j].type;
                    if (count <= loneliness || count >= overpopulation) fieldNext[i][j].type = TypeCell::env;
                    else if (count >= birth_start && count <= birth_end) fieldNext[i][j].type = TypeCell::alive;
                }
            }
            field = fieldNext;
        }
    }
    double getProportion(TypeCell type)
    {
        return field.getNum(type) / (double)(n*m);
    }
};
struct Game3D : public iGame
{
    Field3D field;
    Field3D fieldNext;
    Game3D() { dimension = 3; }
    Game3D(int n, int m, int k) {
        this->n = n;
        this->m = m;
        this->k = k;
        dimension = 3;
        field = fieldNext = Field3D(n, m, k);
    }
    void setGame(double p, int s = 0)
    {
        probability = p;
        seed = s;
        field = Field3D(n, m, k);
        fieldNext = Field3D(n, m, k);
        vector<int> tmp(n * m * k);
        iota(tmp.begin(), tmp.end(), 0);
        shuffle(tmp.begin(), tmp.end(), std::mt19937(seed));
        for (int i = 0; i < (int)(p * n * m * k + 0.5); i++)
        {
            int z = tmp[i] / (n*m);
            int x = (tmp[i] % (n*m)) / m;
            int y = (tmp[i] % (n*m)) % m;
            field[z][x][y].type = TypeCell::alive;
        }
    }
    void runGame(int numIt) override
    {
        for (int it = 0; it < numIt; it++)
        {
#pragma omp parallel for
            for (int z = 0; z < k; z++)
            {
                for (int i = 0; i < n; i++)
                {
                    for (int j = 0; j < m; j++)
                    {
                        int count = field.getNumNeighbors(z, i, j, alive, radius);
                        fieldNext[z][i][j].type = field[z][i][j].type;
                        if (count <= loneliness || count >= overpopulation) fieldNext[z][i][j].type = TypeCell::env;
                        else if (count >= birth_start && count <= birth_end) fieldNext[z][i][j].type = TypeCell::alive;
                    }
                }
            }
            field = fieldNext;
        }
    }
    double getProportion(TypeCell type)
    {
        return field.getNum(type) / (double)(n * m * k);
    }
};
int main()
{
    int n = 3, m = 4, k = 5;
    Field3D field(n, m, k);
    field[0][1][2].type = TypeCell::alive;
    field[0][2][1].type = TypeCell::alive;
    field[1][1][1].type = TypeCell::alive;
    field[4][2][2].type = TypeCell::alive;
    cout << "Set type cell:\n" << field;
    cout << "Numbers of alive: " << field.getNumNeighbors(0, 1, 1) << " " << field.getNumNeighbors(3, 1, 1) << "\n\n";

    Game2D game1 = Game2D(5, 5);
    game1.field[2][1].type = TypeCell::alive;
    game1.field[2][2].type = TypeCell::alive;
    game1.field[2][3].type = TypeCell::alive;
    cout << "Set field for 2d game:\n" << game1.field << "\n";
    game1.runGame(1);
    cout << game1.field << "\n";
    game1.runGame(1);
    cout << game1.field << "\n";

    game1.setGame(0.28);
    cout << "test probability. Start: \n" << game1.field << "\n";
    game1.runGame(20);
    cout << "test probability. End 20 it: \n" << game1.field << "\n";

    Game3D game2 = Game3D(4, 4, 4);
    Game3D gameTmp;
    double p = 0.95; //0.95 0.1
    game2.setGame(p, 0);
    cout << game2.field;
    const auto time1 = clock();
    for (int radius = 1; radius <= 2; radius++)
    {
        game2.radius = radius;
        for (int loneliness = 0; loneliness <= 25; loneliness++)
        {
            game2.loneliness = loneliness;
            for (int birth_start = loneliness + 1; birth_start <= 26; birth_start++)
            {
                game2.birth_start = birth_start;
                for (int birth_end = birth_start; birth_end <= 26; birth_end++)
                {
                    game2.birth_end = birth_end;
                    for (int overpopulation = birth_end + 1; overpopulation <= 27; overpopulation++)
                    {
                        game2.overpopulation = overpopulation;
                        game2.setGame(p);
                        game2.runGame(20);
                        double prop = game2.getProportion(TypeCell::alive);
                        if (prop < 0.2 && prop > 0.15)
                        {
                            cout << radius << " " << loneliness << " " << birth_start << " " << birth_end << " " << overpopulation << "\n";
                        }
                    }
                }
            }
        }
    }
    cout << "time, sec: " << (clock() - time1) / 1000.0 << "\n";
    cout << gameTmp.field;
    return 0;
}