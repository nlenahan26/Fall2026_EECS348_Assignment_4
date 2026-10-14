// Sudoku brute-force solver (object-oriented, recursive backtracking)
// Compile: g++ -std=c++11 -o sudoku sudoku.cpp
// Run:     ./sudoku            (reads puzzle1.txt ... puzzle5.txt from the current directory)

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cctype>

using namespace std;

// ---------------------------------------------------------------------------
// Board: holds a 9x9 grid and knows how to load, print, and validate placements
// ---------------------------------------------------------------------------
class Board {
public:
    static const int SIZE = 9;
    static const int BLOCK = 3;

    Board() {
        for (int r = 0; r < SIZE; r++)
            for (int c = 0; c < SIZE; c++)
                cells[r][c] = 0;
    }

    // Reads a puzzle from a file. Digits 1-9 are givens; '_' (or '0' / '.') is blank.
    // Whitespace is ignored, so both "53__7____" and "5 3 _ _ 7 ..." layouts work.
    bool load(const string& filename) {
        ifstream in(filename.c_str());
        if (!in) return false;

        int count = 0;
        char ch;
        while (count < SIZE * SIZE && in.get(ch)) {
            if (isspace(static_cast<unsigned char>(ch))) continue;
            int r = count / SIZE;
            int c = count % SIZE;
            if (ch >= '1' && ch <= '9') cells[r][c] = ch - '0';
            else                        cells[r][c] = 0;   // '_', '0', '.', anything else = blank
            count++;
        }
        return count == SIZE * SIZE;
    }

    int  get(int r, int c) const { return cells[r][c]; }
    void set(int r, int c, int v) { cells[r][c] = v; }

    // Finds the first empty cell (row-major order). Returns false if the board is full.
    bool findEmpty(int& row, int& col) const {
        for (int r = 0; r < SIZE; r++)
            for (int c = 0; c < SIZE; c++)
                if (cells[r][c] == 0) {
                    row = r;
                    col = c;
                    return true;
                }
        return false;
    }

    // True if value v does not already appear in the row, column, or block of (row, col).
    bool canPlace(int row, int col, int v) const {
        for (int i = 0; i < SIZE; i++) {
            if (cells[row][i] == v) return false;   // row
            if (cells[i][col] == v) return false;   // column
        }
        int br = (row / BLOCK) * BLOCK;
        int bc = (col / BLOCK) * BLOCK;
        for (int r = br; r < br + BLOCK; r++)
            for (int c = bc; c < bc + BLOCK; c++)
                if (cells[r][c] == v) return false; // block
        return true;
    }

    // Checks that the given clues don't already violate any rule.
    bool givensAreConsistent() {
        for (int r = 0; r < SIZE; r++)
            for (int c = 0; c < SIZE; c++) {
                int v = cells[r][c];
                if (v == 0) continue;
                cells[r][c] = 0;                    // temporarily remove to test against the rest
                bool ok = canPlace(r, c, v);
                cells[r][c] = v;
                if (!ok) return false;
            }
        return true;
    }

    void print(ostream& out) const {
        for (int r = 0; r < SIZE; r++) {
            if (r > 0 && r % BLOCK == 0) out << "------+-------+------\n";
            for (int c = 0; c < SIZE; c++) {
                if (c > 0 && c % BLOCK == 0) out << "| ";
                if (cells[r][c] == 0) out << "_";
                else                  out << cells[r][c];
                out << (c < SIZE - 1 ? " " : "");
            }
            out << "\n";
        }
    }

private:
    int cells[SIZE][SIZE];
};

// ---------------------------------------------------------------------------
// SudokuSolver: recursive brute-force search that collects ALL solutions
// ---------------------------------------------------------------------------
class SudokuSolver {
public:
    explicit SudokuSolver(const Board& puzzle) : board(puzzle) {}

    // Runs the search and returns every solution found.
    const vector<Board>& solveAll() {
        solutions.clear();
        if (board.givensAreConsistent())
            search();
        return solutions;
    }

private:
    Board board;
    vector<Board> solutions;

    // Recursive step: pick an empty cell, try each candidate, recurse, then undo.
    void search() {
        int row, col;
        if (!board.findEmpty(row, col)) {   // no empty cells left -> all 81 filled
            solutions.push_back(board);
            return;
        }

        for (int v = 1; v <= 9; v++) {
            if (board.canPlace(row, col, v)) {
                board.set(row, col, v);     // try candidate
                search();                   // recurse with k+1 cells filled
                board.set(row, col, 0);     // backtrack
            }
        }
        // If no candidate worked, we simply return: this path is a dead end.
    }
};

// ---------------------------------------------------------------------------
// PuzzleRunner: loads one puzzle file, solves it, and reports the results
// ---------------------------------------------------------------------------
class PuzzleRunner {
public:
    explicit PuzzleRunner(const string& file) : filename(file) {}

    void run() {
        cout << "==============================\n";
        cout << "Puzzle file: " << filename << "\n";
        cout << "==============================\n";

        Board puzzle;
        if (!puzzle.load(filename)) {
            cout << "Error: could not read a complete 9x9 puzzle from " << filename << "\n\n";
            return;
        }

        cout << "Puzzle:\n";
        puzzle.print(cout);
        cout << "\n";

        SudokuSolver solver(puzzle);
        const vector<Board>& solutions = solver.solveAll();

        if (solutions.empty()) {
            cout << "No solution found\n\n";
            return;
        }

        if (solutions.size() == 1) {
            cout << "Solution:\n";
            solutions[0].print(cout);
            cout << "\n";
        } else {
            cout << solutions.size() << " solutions found:\n\n";
            for (size_t i = 0; i < solutions.size(); i++) {
                cout << "Solution " << (i + 1) << ":\n";
                solutions[i].print(cout);
                cout << "\n";
            }
        }
    }

private:
    string filename;
};

int main() {
    for (int i = 1; i <= 5; i++) {
        PuzzleRunner runner("puzzle" + to_string(i) + ".txt");
        runner.run();
    }
    return 0;
}
