// Библиотеки стандартной C++
#include <iostream>
#include <vector>
#include <stdexcept>
#include <iomanip>
#include <locale.h>

using namespace std;

// Определение типа для матрицы
using Matrix = vector<vector<int>>;

// Функция для ввода матрицы
Matrix inputMatrix(int rows, int cols) {
    // Инициализация пустой матрицы заданного размера
    Matrix matrix(rows, vector<int>(cols));
    // Просим пользователя ввести элементы матрицы
    cout << "Введите элементы матрицы (построчно):" << endl;
    // Заполняем матрицу введенными значениями
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            cin >> matrix[i][j];
        }
    }
    return matrix;
}

// Функция для вывода матрицы на экран
void printMatrix(const Matrix& matrix) {
    // Проходим по всем строкам матрицы
    for (const auto& row : matrix) {
        // Проходим по всем элементам строки и выводим их
        for (int elem : row) {
            cout << setw(5) << elem << " ";
        }
        cout << endl;
    }
}

// Функция для вычисления ранга матрицы
int rankOfMatrix(Matrix matrix) {
    // Инициализация ранга как количества столбцов
    int rank = matrix[0].size();
    // Проходим по всем строкам
    for (int row = 0; row < rank; row++) {
        // Если элемент на диагонали не равен нулю
        if (matrix[row][row]) {
            // Приводим все строки ниже к нулю в текущем столбце
            for (int col = 0; col < matrix.size(); col++) {
                if (col != row) {
                    double mult = (double)matrix[col][row] / matrix[row][row];
                    for (int i = 0; i < rank; i++)
                        matrix[col][i] -= mult * matrix[row][i];
                }
            }
        }
        else {
            // Если элемент на диагонали равен нулю
            bool reduce = true;
            for (int i = row + 1; i < matrix.size(); i++) {
                if (matrix[i][row]) {
                    swap(matrix[row], matrix[i]);
                    reduce = false;
                    break;
                }
            }
            if (reduce) {
                rank--;
                for (int i = 0; i < matrix.size(); i++)
                    matrix[i][row] = matrix[i][rank];
            }
            row--;
        }
    }
    return rank;
}

// Функция для транспонирования матрицы
Matrix transposeMatrix(const Matrix& matrix) {
    // Определяем размеры исходной матрицы
    int rows = matrix.size();
    int cols = matrix[0].size();
    // Создаем матрицу для результата
    Matrix transposed(cols, vector<int>(rows));
    // Заполняем транспонированную матрицу
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            transposed[j][i] = matrix[i][j];
        }
    }
    return transposed;
}

// Функция для вычисления определителя матрицы
int determinant(const Matrix& matrix, int n) {
    // Инициализация определителя
    int det = 0;
    // Базовый случай для матрицы 1x1
    if (n == 1)
        return matrix[0][0];

    // Временная матрица для хранения подматрицы
    Matrix submatrix(n, vector<int>(n));
    // Инициализация знака
    int sign = 1;

    // Проходим по всем элементам первой строки
    for (int x = 0; x < n; x++) {
        int subi = 0;
        for (int i = 1; i < n; i++) {
            int subj = 0;
            for (int j = 0; j < n; j++) {
                if (j == x)
                    continue;
                submatrix[subi][subj] = matrix[i][j];
                subj++;
            }
            subi++;
        }
        det = det + (sign * matrix[0][x] * determinant(submatrix, n - 1));
        sign = -sign;
    }
    return det;
}

// Функция для вычисления следа матрицы
int trace(const Matrix& matrix) {
    // Инициализация следа
    int trace = 0;
    // Суммируем элементы на главной диагонали
    for (int i = 0; i < matrix.size(); ++i) {
        trace += matrix[i][i];
    }
    return trace;
}

// Функция для вычисления обратной матрицы
Matrix inverseMatrix(const Matrix& matrix) {
    // Определяем размер матрицы
    int n = matrix.size();
    // Создаем расширенную матрицу
    Matrix augmentedMatrix(n, vector<int>(2 * n));
    // Заполняем расширенную матрицу
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            augmentedMatrix[i][j] = matrix[i][j];
        }
        augmentedMatrix[i][n + i] = 1;
    }
    // Приводим матрицу к ступенчатому виду
    for (int i = 0; i < n; ++i) {
        if (augmentedMatrix[i][i] == 0) {
            bool swapped = false;
            for (int j = i + 1; j < n; ++j) {
                if (augmentedMatrix[j][i] != 0) {
                    swap(augmentedMatrix[i], augmentedMatrix[j]);
                    swapped = true;
                    break;
                }
            }
            if (!swapped) {
                throw runtime_error("Матрица не является обратимой");
            }
        }
        int pivot = augmentedMatrix[i][i];
        for (int j = 0; j < 2 * n; ++j) {
            augmentedMatrix[i][j] /= pivot;
        }
        for (int j = 0; j < n; ++j) {
            if (i != j) {
                int factor = augmentedMatrix[j][i];
                for (int k = 0; k < 2 * n; ++k) {
                    augmentedMatrix[j][k] -= factor * augmentedMatrix[i][k];
                }
            }
        }
    }
    // Извлекаем обратную матрицу из расширенной
    Matrix inverse(n, vector<int>(n));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            inverse[i][j] = augmentedMatrix[i][j + n];
        }
    }
    return inverse;
}

// Функция для проверки равенства двух матриц
bool areMatricesEqual(const Matrix& matrix1, const Matrix& matrix2) {
    // Проверяем размеры матриц
    if (matrix1.size() != matrix2.size() || matrix1[0].size() != matrix2[0].size()) {
        return false;
    }
    // Проверяем элементы матриц на равенство
    for (int i = 0; i < matrix1.size(); ++i) {
        for (int j = 0; j < matrix1[0].size(); ++j) {
            if (matrix1[i][j] != matrix2[i][j]) {
                return false;
            }
        }
    }
    return true;
}

// Функция для сложения двух матриц
Matrix addMatrices(const Matrix& matrix1, const Matrix& matrix2) {
    // Определяем размеры матриц
    int rows = matrix1.size();
    int cols = matrix1[0].size();
    // Создаем матрицу для результата
    Matrix result(rows, vector<int>(cols));
    // Складываем соответствующие элементы матриц
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            result[i][j] = matrix1[i][j] + matrix2[i][j];
        }
    }
    return result;
}

// Функция для умножения двух матриц
Matrix multiplyMatrices(const Matrix& matrix1, const Matrix& matrix2) {
    // Определяем размеры матриц
    int rows1 = matrix1.size();
    int cols1 = matrix1[0].size();
    int cols2 = matrix2[0].size();
    // Создаем матрицу для результата
    Matrix result(rows1, vector<int>(cols2));
    // Умножаем матрицы
    for (int i = 0; i < rows1; ++i) {
        for (int j = 0; j < cols2; ++j) {
            for (int k = 0; k < cols1; ++k) {
                result[i][j] += matrix1[i][k] * matrix2[k][j];
            }
        }
    }
    return result;
}

// Главная функция
int main() {
    // Устанавливаем локаль для поддержки русского языка
    setlocale(LC_ALL, "ru_RU.UTF-8");
    // Ввод размеров матриц
    int rows1, cols1, rows2, cols2;

    cout << "Введите количество строк и столбцов для матрицы 1: ";
    cin >> rows1 >> cols1;
    Matrix matrix1 = inputMatrix(rows1, cols1);

    cout << "Введите количество строк и столбцов для матрицы 2: ";
    cin >> rows2 >> cols2;
    Matrix matrix2 = inputMatrix(rows2, cols2);

    // Вывод матриц
    cout << "Матрица 1:" << endl;
    printMatrix(matrix1);
    cout << "Матрица 2:" << endl;
    printMatrix(matrix2);

    // Вывод ранга матриц
    cout << "Ранг матрицы 1: " << rankOfMatrix(matrix1) << endl;
    cout << "Ранг матрицы 2: " << rankOfMatrix(matrix2) << endl;

    // Вывод транспонированных матриц
    cout << "Транспонированная матрица 1:" << endl;
    printMatrix(transposeMatrix(matrix1));
    cout << "Транспонированная матрица 2:" << endl;
    printMatrix(transposeMatrix(matrix2));

    // Вычисление и вывод характеристик первой матрицы
    if (rows1 == cols1) {
        cout << "Матрица 1 является квадратной." << endl;
        cout << "Определитель матрицы 1: " << determinant(matrix1, rows1) << endl;
        cout << "След матрицы 1: " << trace(matrix1) << endl;
        try {
            Matrix inv1 = inverseMatrix(matrix1);
            cout << "Обратная матрица 1:" << endl;
            printMatrix(inv1);
        }
        catch (runtime_error& e) {
            cout << e.what() << endl;
        }
    }
    else {
        cout << "Матрица 1 не является квадратной." << endl;
    }

    // Вычисление и вывод характеристик второй матрицы
    if (rows2 == cols2) {
        cout << "Матрица 2 является квадратной." << endl;
        cout << "Определитель матрицы 2: " << determinant(matrix2, rows2) << endl;
        cout << "След матрицы 2: " << trace(matrix2) << endl;
        try {
            Matrix inv2 = inverseMatrix(matrix2);
            cout << "Обратная матрица 2:" << endl;
            printMatrix(inv2);
        }
        catch (runtime_error& e) {
            cout << e.what() << endl;
        }
    }
    else {
        cout << "Матрица 2 не является квадратной." << endl;
    }

    // Проверяем, равны ли матрицы
    if (areMatricesEqual(matrix1, matrix2)) {
        cout << "Матрицы равны." << endl;
    }
    else {
        cout << "Матрицы не равны." << endl;
    }

    // Проверяем, можно ли сложить матрицы
    if (rows1 == rows2 && cols1 == cols2) {
        cout << "Сложение матриц:" << endl;
        printMatrix(addMatrices(matrix1, matrix2));
    }
    else {
        cout << "Матрицы не могут быть сложены из-за разных размеров." << endl;
    }

    // Проверяем, можно ли умножить матрицы
    if (cols1 == rows2) {
        cout << "Умножение матриц:" << endl;
        printMatrix(multiplyMatrices(matrix1, matrix2));
    }
    else {
        cout << "Матрицы не могут быть умножены из-за несовместимых размеров." << endl;
    }

    // Завершаем программу
    return 0;
}
