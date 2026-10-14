#include <iostream>
#include <fstream>
#include <string>
#include <vector>

// Class representing a 9x9 Sudoku board and its solving logic
class SudokuBoard {
private:
    char grid[9][9];

    // Helper to check if placing 'num' at (row, col) is valid
    bool isValid(int row, int col, char num) const {
        // Check row
        for (int j = 0; j < 9; ++j) {
            if (grid[row][j] == num) {
                return false;
            }
        }
        // Check column
        for (int i = 0; i < 9; ++i) {
            if (grid[i][col] == num) {
                return false;
            }
        }
        // Check 3x3 subgrid (block)
        int startRow = row - (row % 3);
        int startCol = col - (col % 3);
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                if (grid[startRow + i][startCol + j] == num) {
                    return false;
                }
            }
        }
        return true;
    }

    // Find the next empty cell marked with '_'
    bool findEmptyCell(int& row, int& col) const {
        for (row = 0; row < 9; ++row) {
            for (col = 0; col < 9; ++col) {
                if (grid[row][col] == '_') {
                    return true;
                }
            }
        }
        return false;
    }

public:
    SudokuBoard() {
        clear();
    }

    // Reset the board to blanks
    void clear() {
        for (int i = 0; i < 9; ++i) {
            for (int j = 0; j < 9; ++j) {
                grid[i][j] = '_';
            }
        }
    }

    // Load puzzle from a text file
    bool loadFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            return false;
        }
        char ch;
        for (int i = 0; i < 9; ++i) {
            for (int j = 0; j < 9; ++j) {
                if (!(file >> ch)) {
                    return false;
                }
                if ((ch >= '1' && ch <= '9') || ch == '_') {
                    grid[i][j] = ch;
                } else {
                    --j; // Retry if unexpected character encountered
                }
            }
        }
        return true;
    }

    // Print the puzzle grid cleanly
    void print() const {
        for (int i = 0; i < 9; ++i) {
            if (i > 0 && i % 3 == 0) {
                std::cout << "------+-------+------\n";
            }
            for (int j = 0; j < 9; ++j) {
                if (j > 0 && j % 3 == 0) {
                    std::cout << "| ";
                }
                std::cout << grid[i][j] << ' ';
            }
            std::cout << '\n';
        }
    }

    // Recursive brute-force method to find and print all solutions
    void solveAll(int& solutionCount) {
        int row, col;
        // If no empty cells remain, a solution is found
        if (!findEmptyCell(row, col)) {
            solutionCount++;
            std::cout << "Solution " << solutionCount << ":\n";
            print();
            std::cout << "\n";
            return; // Return to continue searching for alternative solutions (backtracking)
        }

        // Try candidates '1' through '9'
        for (char num = '1'; num <= '9'; ++num) {
            if (isValid(row, col, num)) {
                grid[row][col] = num;       // Place candidate
                solveAll(solutionCount);    // Recursive call
                grid[row][col] = '_';       // Backtrack
            }
        }
    }
};

// Application controller class managing puzzle files
class SudokuApp {
public:
    void run() {
        std::string puzzleFiles[5] = {
            "puzzle1.txt", 
            "puzzle2.txt", 
            "puzzle3.txt", 
            "puzzle4.txt", 
            "puzzle5.txt"
        };

        for (int i = 0; i < 5; ++i) {
            std::cout << "========================================\n";
            std::cout << "Puzzle File: " << puzzleFiles[i] << "\n";
            std::cout << "========================================\n";

            SudokuBoard board;
            if (!board.loadFromFile(puzzleFiles[i])) {
                std::cout << "Error: Could not open or read " << puzzleFiles[i] << "\n\n";
                continue;
            }

            std::cout << "Initial Puzzle:\n";
            board.print();
            std::cout << "\n--- Solutions ---\n\n";

            int solutionCount = 0;
            board.solveAll(solutionCount);

            if (solutionCount == 0) {
                std::cout << "No solution found.\n\n";
            } else {
                std::cout << "Total solutions found for " << puzzleFiles[i] << ": " << solutionCount << "\n\n";
            }
        }
    }
};

int main() {
    SudokuApp app;
    app.run();
    return 0;
}