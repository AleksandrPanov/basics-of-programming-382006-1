#include <vector>
#include <conio.h>
#include <Windows.h>
#include <iostream>
#include <algorithm>
#include <numeric>
#include <random>
#include <omp.h>
using namespace std;
// 0 -------\ Y (m)
//   -------/
// ||
// ||
// ||
// ||
// \/ X (n)

enum GameEvent
{
    FieldEmpty,
    FieldFull
};
struct Observer
{
    virtual void newEvent(GameEvent event) {};
};
struct Subject
{
    vector<Observer*> observers;
    void subscribe(Observer& tmp)
    {
        observers.push_back(&tmp);
    }
    void update(GameEvent event)
    {
        for (auto observer : observers)
            observer->newEvent(event);
    }
};
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
    Cell operator[](int i) const { return cells[i]; } // const вариант дл€ cout
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
struct iGame : public Subject
{
    int n = 0;
    int m = 0;
    int k = 0;

    int seed = 0; // случайна€ величина дл€ генератора
    double probability = 0.0;  // веро€тность того, что клетка жива€
    int dimension = 1; // размерность

    int radius = 1; // радиус проверки, граница включена
    int loneliness = 2; // с этого числа и меньше клетки умирают от одиночества
    int birth_start = 3; // с этого числа и до birth_end по€вл€етс€ жива€ клетка
    int birth_end = 3;
    int overpopulation = 5; // с этого числа и дальше клетки погибают от перенаселени€

    virtual void setGame(double p, int s = 0) = 0;
    virtual void runGame(int numIt) = 0;
    virtual double getProportion(TypeCell type) = 0;
    virtual void print(ostream& out) const = 0;
    
    virtual void run(int numIt) // –еализовать
    {
        runGame(numIt);
        // делаем проверку на событи€        
        double prop = getProportion(alive);
        if (prop == 0.0) update(FieldEmpty);
        else if (prop == 1.0) update(FieldFull);
    }

    friend ostream& operator<< (ostream& out, const iGame& g) {
        g.print(out);
        return out;
    }
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
            int aliveCount = field.getNum();
            if (aliveCount == 0) update(FieldEmpty);
        }
    }
    double getProportion(TypeCell type)
    {
        return field.getNum(type) / (double)(n*m);
    }
    void print(ostream& out) const
    {
        out << field;
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
            int aliveCount = field.getNum();
            if (aliveCount == 0) update(FieldEmpty);
        }
    }
    double getProportion(TypeCell type)
    {
        return field.getNum(type) / (double)(n * m * k);
    }
    void print(ostream& out) const
    {
        out << field;
    }
};
class View : Observer
{

    iGame* game = 0;
    int game_dim = 0;
    int button = 0;
    enum StatusGame // ”ра, можно внутри классов делать свои enum/struct/class =)
    {
        gameSetDim,
        gameSetSize,
        gameSetP,
        gameReady,
        gameRun,
        gamePause,
        gameOver
    };
    StatusGame status = gameSetDim;
    void goto_xy(int x, int y = 0) {
        COORD pos = { y, x };
        HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleCursorPosition(output, pos);
    }
public:
    View()
    {
        setlocale(0, ""); // добавл€ем ru раскладку
    }
    void start()
    {
        while (1)
        {
            if (status == gameSetDim)
            {
                system("cls");
                cout << "Choose the dimension of the game\n" << "1\n" << "2\n" << "3\n";
                int pos = 0;
                goto_xy(pos + 1);
                while (1)
                {
                    if (_kbhit())
                    {
                        button = _getch();
                        if (button == 'w')
                        {
                            pos = (pos - 1 + 3) % 3;
                            goto_xy(pos + 1);
                            //cout << "Ќажата кнопка " << button << " " << (char)button << endl;
                        }
                        else if (button == 's')
                        {
                            pos = (pos + 1) % 3;
                            goto_xy(pos + 1);
                            //cout << "Ќажата кнопка " << button << " " << (char)button << endl;
                        }
                        else if (button == 13 && pos != 0) // enter
                        {
                            game_dim = pos + 1;
                            goto_xy(4);

                            if (game_dim == 2)
                            {
                                game = new Game2D(5, 5);
                                game->setGame(0.2);
                                game->subscribe((Observer&)*this);
                            }
                            else if (game_dim == 3)
                            {
                                game = new Game3D(5, 5, 5);
                                game->setGame(0.2);
                                game->subscribe((Observer&)*this);
                            }

                            cout << "Ќажата кнопка enter code: " << button << " " << (char)button << endl;
                            break;
                        }
                    }
                }
                system("cls");
                cout << "¬ gameReady можно сделать донастройку игры\n" << *game;
                status = gameReady;
            }
            else if (status == gameReady) // реализовать самоста€тельно
            {
                status = gameRun;
            }
            else if (status == gameRun)
            {
                system("cls");
                game->run(1);
                cout << *game;
                Sleep(600);
                if (_kbhit())
                {
                    button = _getch();
                    if (button == 'p')
                    {
                        status = gamePause;
                    }
                }
            }
            else if (status == gamePause)
            {
                if (_kbhit())
                {
                    button = _getch();
                    if (button == 'p' || button == 'r')
                    {
                        status = gameRun;
                    }
                }
            }
            else if (status == gameOver)
            {
                cout << "\nGame Over\nPress any key to continue\n";
                button = _getch();
                if (button != 27) status = gameSetDim; // escape
                break;
            }
            else
            {
                cout << "Unknown status\n";
                break;
            }
        }
    }
    void newEvent(GameEvent event)
    {
        if (event == FieldEmpty || event == FieldFull)
        {
            status = gameOver;
        }
        else
        {
            cout << "unkown event";
        }
    }
    ~View()
    {
        if (game) delete game;
    }
};
int main()
{
    View view;
    view.start();
    return 0;
}