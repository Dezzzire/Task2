// Включение заголовочных файлов
#include <iostream>
#include <vector>
#include <stdexcept>
#include <iomanip>
#include <locale.h>

using namespace std;

// Определение типа матрицы как вектора векторов целых чисел
using Matrix = vector<vector<int>>;

// Функция для ввода матрицы
Matrix inputMatrix(int rows, int cols) {
    // Создание матрицы заданного размера
    Matrix matrix(rows, vector<int>(cols));
    // Вывод сообщения для ввода элементов матрицы
    cout << "Введите элементы матрицы (построчно):" << endl;
    // Чтение элементов матрицы с клавиатуры
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            cin >> matrix[i][j];
        }
    }
    return matrix;
}

// Функция для вывода матрицы
void printMatrix(const Matrix& matrix) {
    // Перебор всех строк матрицы
    for (const auto& row : matrix) {
        // Печать элементов строки с отступом
        for (int elem : row) {
            cout << setw(5) << elem << " ";
        }
        cout << endl;
    }
}

// Функция для вычисления ранга матрицы
int rankOfMatrix(Matrix matrix) {
    // Инициализация ранга как числа столбцов
    int rank = matrix[0].size();
    // Проход по диагонали матрицы
    for (int row = 0; row < rank; row++) {
        // Если текущий элемент на диагонали ненулевой
        if (matrix[row][row]) {
            // Приведение элементов под текущей диагональю к нулю
            for (int col = 0; col < static_cast<int>(matrix.size()); col++) {
                if (col != row) {
                    int mult = static_cast<int>(static_cast<double>(matrix[col][row]) / matrix[row][row]);
                    for (int i = 0; i < rank; i++)
                        matrix[col][i] -= mult * matrix[row][i];
                }
            }
        }
        // Если текущий элемент на диагонали нулевой
        else {
            // Поиск ненулевого элемента ниже текущей строки
            bool reduce = true;
            for (int i = row + 1; i < static_cast<int>(matrix.size()); i++) {
                if (matrix[i][row]) {
                    swap(matrix[row], matrix[i]);
                    reduce = false;
                    break;
                }
            }
            // Если ненулевой элемент не найден
            if (reduce) {
                // Уменьшение ранга и удаление текущего столбца
                rank--;
                for (int i = 0; i < static_cast<int>(matrix.size()); i++)
                    matrix[i][row] = matrix[i][rank];
            }
            row--;
        }
    }
    return rank;
}

// Функция для транспонирования матрицы
Matrix transposeMatrix(const Matrix& matrix) {
    // Получение количества строк и столбцов
    int rows = matrix.size();
    int cols = matrix[0].size();
    // Создание транспонированной матрицы
    Matrix transposed(cols, vector<int>(rows));
    // Заполнение транспонированной матрицы
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
    // Если матрица имеет размер 1x1
    if (n == 1)
        return matrix[0][0];

    // Создание подматрицы
    Matrix submatrix(n, vector<int>(n));
    // Инициализация знака
    int sign = 1;

    // Вычисление определителя по разложению по первой строке
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
    // Суммирование диагональных элементов
    for (int i = 0; i < static_cast<int>(matrix.size()); ++i) {
        trace += matrix[i][i];
    }
    return trace;
}

// Начало функции для вычисления обратной матрицы
Matrix inverseMatrix(const Matrix& matrix) {
    // Получение размерности матрицы
    int n = static_cast<int>(matrix.size());
    // Создание расширенной матрицы
    Matrix augmentedMatrix(n, vector<int>(2 * n));
    // Заполнение расширенной матрицы
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            augmentedMatrix[i][j] = matrix[i][j];
        }
        // Добавление единичной матрицы справа от исходной
        augmentedMatrix[i][n + i] = 1;
    }
    // Приведение к диагональному виду методом Гаусса
    for (int i = 0; i < n; ++i) {
        // Проверка на ноль на диагонали
        if (augmentedMatrix[i][i] == 0) {
            bool swapped = false;
            // Поиск строки с ненулевым элементом для обмена
            for (int j = i + 1; j < n; ++j) {
                if (augmentedMatrix[j][i] != 0) {
                    swap(augmentedMatrix[i], augmentedMatrix[j]);
                    swapped = true;
                    break;
                }
            }
            // Если такой строки не найдено, матрица необратима
            if (!swapped) {
                throw runtime_error("Матрица не является обратимой");
            }
        }
        // Приведение текущей строки к единичному диагональному элементу
        int pivot = augmentedMatrix[i][i];
        for (int j = 0; j < 2 * n; ++j) {
            augmentedMatrix[i][j] /= pivot;
        }
        // Обнуление элементов в столбцах, кроме диагонали
        for (int j = 0; j < n; ++j) {
            if (i != j) {
                int factor = augmentedMatrix[j][i];
                for (int k = 0; k < 2 * n; ++k) {
                    augmentedMatrix[j][k] -= factor * augmentedMatrix[i][k];
                }
            }
        }
    }
}
// Создание и заполнение матрицы обратной к исходной
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
    // Проверка размеров матриц
    if (matrix1.size() != matrix2.size() || matrix1[0].size() != matrix2[0].size()) {
        return false;
    }
    // Проверка каждого элемента матриц на равенство
    for (int i = 0; i < static_cast<int>(matrix1.size()); ++i) {
        for (int j = 0; j < static_cast<int>(matrix1[0].size()); ++j) {
            if (matrix1[i][j] != matrix2[i][j]) {
                return false;
            }
        }
    }
    return true;
}

// Функция для сложения двух матриц
Matrix addMatrices(const Matrix& matrix1, const Matrix& matrix2) {
    // Получение размеров матриц
    int rows = static_cast<int>(matrix1.size());
    int cols = static_cast<int>(matrix1[0].size());
    // Создание результирующей матрицы
    Matrix result(rows, vector<int>(cols));
    // Сложение соответствующих элементов двух матриц
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            result[i][j] = matrix1[i][j] + matrix2[i][j];
        }
    }
    return result;
}

// Функция для умножения двух матриц
Matrix multiplyMatrices(const Matrix& matrix1, const Matrix& matrix2) {
    // Получение размеров матриц
    int rows1 = static_cast<int>(matrix1.size());
    int cols1 = static_cast<int>(matrix1[0].size());
    int cols2 = static_cast<int>(matrix2[0].size());
    // Создание результирующей матрицы
    Matrix result(rows1, vector<int>(cols2));
    // Умножение матриц
    for (int i = 0; i < rows1; ++i) {
        for (int j = 0; j < cols2; ++j) {
            for (int k = 0; k < cols1; ++k) {
                result[i][j] += matrix1[i][k] * matrix2[k][j];
            }
        }
    }
    return result;
}

// Основная функция программы
int main() {
    // Установка локали для вывода на русском языке
    setlocale(LC_ALL, "Russian");
    // Объявление переменных для размеров матриц
    int rows1, cols1, rows2, cols2;
    // Ввод размеров и элементов матриц
    cout << "Введите количество строк и столбцов для матрицы 1: ";
    cin >> rows1 >> cols1;
    Matrix matrix1 = inputMatrix(rows1, cols1);

    cout << "Введите количество строк и столбцов для матрицы 2: ";
    cin >> rows2 >> cols2;
    Matrix matrix2 = inputMatrix(rows2, cols2);

    // Вывод матриц на экран
    cout << "Матрица 1:" << endl;
    printMatrix(matrix1);
    cout << "Матрица 2:" << endl;
    printMatrix(matrix2);

    // Вывод рангов матриц
    cout << "Ранг матрицы 1: " << rankOfMatrix(matrix1) << endl;
    cout << "Ранг матрицы 2: " << rankOfMatrix(matrix2) << endl;

    // Вывод транспонированных матриц
    cout << "Транспонированная матрица 1:" << endl;
    printMatrix(transposeMatrix(matrix1));
    // Вывод транспонированной матрицы 2
    cout << "Транспонированная матрица 2:" << endl;
    printMatrix(transposeMatrix(matrix2));

    // Проверка наличия квадратной формы матрицы 1
    if (rows1 == cols1) {
        // Вывод информации о матрице 1
        cout << "Матрица 1 является квадратной." << endl;
        cout << "Определитель матрицы 1: " << determinant(matrix1, rows1) << endl;
        cout << "След матрицы 1: " << trace(matrix1) << endl;
        try {
            // Попытка вычислить обратную матрицу для матрицы 1
            Matrix inverse1 = inverseMatrix(matrix1);
            // Вывод обратной матрицы
            cout << "Обратная матрица 1:" << endl;
            printMatrix(inverse1);
        }
        catch (const runtime_error& e) {
            // Вывод сообщения об ошибке
            cerr << "Ошибка: " << e.what() << endl;
        }
    }
    else {
        cout << "Матрица 1 не является квадратной, поэтому определитель и обратная матрица не могут быть вычислены." << endl;
    }

    // Проверка наличия квадратной формы матрицы 2
    if (rows2 == cols2) {
        // Вывод информации о матрице 2
        cout << "Матрица 2 является квадратной." << endl;
        cout << "Определитель матрицы 2: " << determinant(matrix2, rows2) << endl;
        cout << "След матрицы 2: " << trace(matrix2) << endl;
        try {
            // Попытка вычислить обратную матрицу для матрицы 2
            Matrix inverse2 = inverseMatrix(matrix2);
            // Вывод обратной матрицы
            cout << "Обратная матрица 2:" << endl;
            printMatrix(inverse2);
        }
        catch (const runtime_error& e) {
            // Вывод сообщения об ошибке
            cerr << "Ошибка: " << e.what() << endl;
        }
    }
    else {
        cout << "Матрица 2 не является квадратной, поэтому определитель и обратная матрица не могут быть вычислены." << endl;
    }

    // Проверка возможности сложения матриц
    if (rows1 == rows2 && cols1 == cols2) {
        // Вычисление суммы матриц
        Matrix sum = addMatrices(matrix1, matrix2);
        // Вывод результата сложения
        cout << "Сумма матриц:" << endl;
        printMatrix(sum);
    }
    else {
        cout << "Матрицы не могут быть сложены из-за разных размерностей." << endl;
    }

    // Проверка возможности умножения матриц
    if (cols1 == rows2) {
        // Вычисление произведения матриц
        Matrix product = multiplyMatrices(matrix1, matrix2);
        // Вывод результата умножения
        cout << "Произведение матриц:" << endl;
        printMatrix(product);
    }
    else {
        cout << "Матрицы не могут быть перемножены из-за несовместимых размерностей." << endl;
    }

    return 0;
}