#include <iostream>
#include <windows.h>
#include <string>
#include <vector>
using namespace std;
char maze[18][32];
char gameMaze[18][32] = {
    "+#############################+",
    "|                             |",
    "|                             |",
    "|## ########### ##   #########|",
    "|   |                         |",
    "| | |### |  |           |     |",
    "| |      |  | |###  |   |  |  |",
    "| | #####|  | |      ## |     |",
    "| |           |###  |      |  |",
    "| |##### ###         ##       |",
    "|          ######  ####### ###|",
    "|                             |",
    "|# ### ####      ###   #######|",
    "|                             |",
    "|                             |",
    "|                             |",
    "|                             |",
    "+#############################+"};

void DisplayMap()
{
    for (int i = 0; i < 18; i++)
    {
        printf("%s\n", gameMaze[i]);
    }
}

void DisplayMap(int choice)
{
    if (choice == 1)
    {
        for (int i = 0; i < 18; i++)
        {
            printf("%s\n", gameMaze[i]);
        }
    }
    else if (choice == 2)
    {
        char secondMaze[18][32] = {
            "+#############################+",
            "|                             |",
            "|                             |",
            "|## ########### ###   ########|",
            "|   |                         |",
            "| | |### |  |           |     |",
            "| |      |  | |###  |   |  |  |",
            "| | ##   |  | |      ## |     |",
            "| |           |######      |  |",
            "| |##### ###         ##       |",
            "|          ######  ####### ###|",
            "|                             |",
            "|# ### ####      ###   #######|",
            "|                             |",
            "|                             |",
            "|                             |",
            "|                             |",
            "+#############################+"
        };

        for (int i = 0; i < 18; i++)
        {
            printf("%s\n", secondMaze[i]);
        }
    }
}

void MoveCursor(short x, short y)
{
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD position = {x, y};

    SetConsoleCursorPosition(consoleHandle, position);
}

class Player
{
public:
    Player(int posX, int posY)
    {
        this->posX = posX;
        this->posY = posY;
    }

    void MoveX(int p)
    {
        if (gameMaze[posY][posX + p] == ' ')
            posX += p;
    }

    void MoveY(int p)
    {
        if (gameMaze[posY + p][posX] == ' ')
            posY += p;
    }

    void Move(int p, int q)
    {
        posX += p;
        posY += q;
    }

    int GetX() { return posX; }
    int GetY() { return posY; }

    void Draw(char p)
    {
        gameMaze[posX][posY] = p;
        MoveCursor(posX, posY);
        printf("%c", p);
    }

private:
    int posX;
    int posY;
};

struct WalkPath
{
    short steps;
    short x;
    short y;
    short previousStep;
};

struct TargetDestination
{
    short x;
    short y;
};

vector<TargetDestination> wayPoints;

vector<WalkPath> path;

void AddToArray(int x, int y, int steps, int previousStep)
{
    if (maze[y][x] == ' ' || maze[y][x] == '.')
    {
        maze[y][x] = '#';
        WalkPath tmp;
        tmp.x = x;
        tmp.y = y;
        tmp.steps = steps;
        tmp.previousStep = previousStep;
        path.push_back(tmp);
    }
}

void FindRoute(int startX, int startY, int x, int y)
{
    memcpy(maze, gameMaze, sizeof(gameMaze));
    path.clear();
    WalkPath tmp;
    tmp.x = startX;
    tmp.y = startY;
    tmp.steps = 0;
    tmp.previousStep = -1;
    path.push_back(tmp);

    int i = 0;
    while (i < path.size())
    {
        if (path[i].x == x && path[i].y == y)
        {
            wayPoints.clear();
            TargetDestination tmp2;
            while (path[i].steps != 0)
            {
                tmp2.x = path[i].x;
                tmp2.y = path[i].y;
                wayPoints.push_back(tmp2);

                i = path[i].previousStep;
            }

            break;
        }

        AddToArray(path[i].x + 1, path[i].y, path[i].steps + 1, i);
        AddToArray(path[i].x - 1, path[i].y, path[i].steps + 1, i);
        AddToArray(path[i].x, path[i].y + 1, path[i].steps + 1, i);
        AddToArray(path[i].x, path[i].y - 1, path[i].steps + 1, i);

        i++;
    }

    path.clear();
}

int main()
{
    bool isRunning = true;
    int playerX = 15; // hero x
    int playerY = 16; // hero y
    int oldPlayerX;
    int oldPlayerY;

    int enemyX = 1;
    int enemyY = 1;

    int points = 0;

    printf("Instruction:\n1. WSAD Keys to move The H\n2. Take  dots produced by the villain to take points\n3. Don't get caught by the Eater\n\n");
    printf("H -> Hard\nN -> Normal\nE -> Easy\n\nInput : ");

    char difficulty;
    int speedModifier = 3;

    cin >> difficulty;

    if (difficulty == 'N')
    {
        speedModifier = 2;
    }
    else if (difficulty == 'H')
    {
        speedModifier = 1;
    }
        int mapChoice;
    cout << "Choose a map (1 or 2): ";
    cin >> mapChoice;

    if (mapChoice != 1 && mapChoice != 2)
    {
        cout << "Invalid choice. Using default map (Map 1)." << endl;
        mapChoice = 1;
    }

    system("cls");
    DisplayMap(mapChoice);

    MoveCursor(playerX, playerY);
    cout << "H";

    int frame = 0;

    FindRoute(enemyX, enemyY, playerX, playerY);

    while (isRunning)
    {
        MoveCursor(playerX, playerY);
        cout << " ";

        oldPlayerX = playerX;
        oldPlayerY = playerY;

        if (GetAsyncKeyState('W'))
        {
            if (gameMaze[playerY - 1][playerX] == '.')
            {
                playerY--;
                points++;
            }
            else if (gameMaze[playerY - 1][playerX] == ' ')
                playerY--;
        }
        if (GetAsyncKeyState('S'))
        {
            if (gameMaze[playerY + 1][playerX] == '.')
            {
                playerY++;
                points++;
            }
            else if (gameMaze[playerY + 1][playerX] == ' ')
                playerY++;
        }
        if (GetAsyncKeyState('A'))
        {
            if (gameMaze[playerY][playerX - 1] == '.')
            {
                playerX--;
                points++;
            }
            else if (gameMaze[playerY][playerX - 1] == ' ')
                playerX--;
        }
        if (GetAsyncKeyState('D'))
        {
            if (gameMaze[playerY][playerX + 1] == '.')
            {
                playerX++;
                points++;
            }
            else if (gameMaze[playerY][playerX + 1] == ' ')
                playerX++;
        }

        if (oldPlayerX != playerX || oldPlayerY != playerY)
        {
            FindRoute(enemyX, enemyY, playerX, playerY);
        }

        MoveCursor(playerX, playerY);
        cout << "H";

        gameMaze[enemyY][enemyX] = '.';
        MoveCursor(enemyX, enemyY);
        cout << ".";

        if (frame % speedModifier == 0 && wayPoints.size() != 0)
        {
            enemyX = wayPoints.back().x;
            enemyY = wayPoints.back().y;
            wayPoints.pop_back();
        }

        MoveCursor(enemyX, enemyY);
        cout << "E";

        if (enemyX == playerX && enemyY == playerY)
        {
            break;
        }

        MoveCursor(32, 18);
        MoveCursor(32, 1);
        cout << points;
        Sleep(100);
        frame++;
    }

    system("cls");
    printf("You Lose and your score is : %i", points);

    cin.get();
    cin.get();
    cin.get();
    cin.get();
    cin.get();
    cin.get();
    cin.get();
    cin.get();

    return 0;
}

