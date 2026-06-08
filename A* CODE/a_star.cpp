#include <raylib.h>

#include <algorithm>
#include <cmath>
#include <queue>
#include <unordered_set>
#include <vector>

const int ROWS = 20;
const int COLS = 20;
const int CELL_SIZE = 40;

struct Node {
    int row;
    int col;

    float g = INFINITY;
    float h = 0;
    float f = INFINITY;

    bool wall = false;

    Node* parent = nullptr;

    bool open = false;
    bool closed = false;
    bool path = false;
};

Node grid[ROWS][COLS];

struct Compare {
    bool operator()(Node* a, Node* b) {
        return a->f > b->f;
    }
};

std::priority_queue<Node*, std::vector<Node*>, Compare> openSet;

Node* startNode = nullptr;
Node* endNode = nullptr;

bool searching = false;
bool finished = false;

float heuristic(Node* a, Node* b) {
    return abs(a->row - b->row) +
           abs(a->col - b->col);
}

void initializeGrid() {
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {
            grid[r][c].row = r;
            grid[r][c].col = c;
        }
    }
}

void reconstructPath() {
    Node* current = endNode;

    while (current != nullptr) {
        current->path = true;
        current = current->parent;
    }
}

void startAStar() {
    while (!openSet.empty())
        openSet.pop();

    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {
            grid[r][c].g = INFINITY;
            grid[r][c].h = 0;
            grid[r][c].f = INFINITY;

            grid[r][c].parent = nullptr;

            grid[r][c].open = false;
            grid[r][c].closed = false;
            grid[r][c].path = false;
        }
    }

    startNode->g = 0;
    startNode->h = heuristic(startNode, endNode);
    startNode->f = startNode->h;

    openSet.push(startNode);
    startNode->open = true;

    searching = true;
    finished = false;
}

void processAStarStep() {
    if (openSet.empty()) {
        searching = false;
        return;
    }

    Node* current = openSet.top();
    openSet.pop();

    current->closed = true;

    if (current == endNode) {
        reconstructPath();
        searching = false;
        finished = true;
        return;
    }

    int dr[4] = {-1, 1, 0, 0};
    int dc[4] = {0, 0, -1, 1};

    for (int i = 0; i < 4; i++) {
        int nr = current->row + dr[i];
        int nc = current->col + dc[i];

        if (nr < 0 || nr >= ROWS ||
            nc < 0 || nc >= COLS)
            continue;

        Node* neighbor = &grid[nr][nc];

        if (neighbor->wall || neighbor->closed)
            continue;

        float tentativeG = current->g + 1;

        if (tentativeG < neighbor->g) {
            neighbor->parent = current;

            neighbor->g = tentativeG;
            neighbor->h = heuristic(neighbor, endNode);
            neighbor->f = neighbor->g + neighbor->h;

            openSet.push(neighbor);
            neighbor->open = true;
        }
    }
}

int main() {
    InitWindow(
        COLS * CELL_SIZE,
        ROWS * CELL_SIZE,
        "A* Pathfinding Visualizer"
    );

    SetTargetFPS(60);

    initializeGrid();

    startNode = &grid[2][2];
    endNode = &grid[17][17];

    while (!WindowShouldClose()) {

        if (!searching) {

            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
                Vector2 mouse = GetMousePosition();

                int col = mouse.x / CELL_SIZE;
                int row = mouse.y / CELL_SIZE;

                if (row >= 0 && row < ROWS &&
                    col >= 0 && col < COLS) {

                    Node* node = &grid[row][col];

                    if (node != startNode &&
                        node != endNode)
                        node->wall = true;
                }
            }

            if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON)) {
                Vector2 mouse = GetMousePosition();

                int col = mouse.x / CELL_SIZE;
                int row = mouse.y / CELL_SIZE;

                if (row >= 0 && row < ROWS &&
                    col >= 0 && col < COLS) {

                    grid[row][col].wall = false;
                }
            }
        }

        if (IsKeyPressed(KEY_SPACE)) {
            startAStar();
        }

        if (searching) {
            processAStarStep();
        }

        BeginDrawing();

        ClearBackground(RAYWHITE);

        for (int r = 0; r < ROWS; r++) {
            for (int c = 0; c < COLS; c++) {

                Node& node = grid[r][c];

                Color color = WHITE;

                if (node.wall)
                    color = BLACK;

                if (node.open)
                    color = GREEN;

                if (node.closed)
                    color = ORANGE;

                if (node.path)
                    color = BLUE;

                if (&node == startNode)
                    color = DARKGREEN;

                if (&node == endNode)
                    color = RED;

                DrawRectangle(
                    c * CELL_SIZE,
                    r * CELL_SIZE,
                    CELL_SIZE,
                    CELL_SIZE,
                    color
                );

                DrawRectangleLines(
                    c * CELL_SIZE,
                    r * CELL_SIZE,
                    CELL_SIZE,
                    CELL_SIZE,
                    GRAY
                );
            }
        }

        DrawText(
            "Left Click: Wall | Right Click: Remove | SPACE: Run A*",
            10,
            10,
            20,
            BLACK
        );

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
