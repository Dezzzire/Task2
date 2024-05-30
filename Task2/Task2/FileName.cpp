// ��������� ������������ ������
#include <iostream>
#include <vector>
#include <stdexcept>
#include <iomanip>
#include <locale.h>

using namespace std;

// ����������� ���� ������� ��� ������� �������� ����� �����
using Matrix = vector<vector<int>>;

// ������� ��� ����� �������
Matrix inputMatrix(int rows, int cols) {
    // �������� ������� ��������� �������
    Matrix matrix(rows, vector<int>(cols));
    // ����� ��������� ��� ����� ��������� �������
    cout << "������� �������� ������� (���������):" << endl;
    // ������ ��������� ������� � ����������
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            cin >> matrix[i][j];
        }
    }
    return matrix;
}

// ������� ��� ������ �������
void printMatrix(const Matrix& matrix) {
    // ������� ���� ����� �������
    for (const auto& row : matrix) {
        // ������ ��������� ������ � ��������
        for (int elem : row) {
            cout << setw(5) << elem << " ";
        }
        cout << endl;
    }
}

// ������� ��� ���������� ����� �������
int rankOfMatrix(Matrix matrix) {
    // ������������� ����� ��� ����� ��������
    int rank = matrix[0].size();
    // ������ �� ��������� �������
    for (int row = 0; row < rank; row++) {
        // ���� ������� ������� �� ��������� ���������
        if (matrix[row][row]) {
            // ���������� ��������� ��� ������� ���������� � ����
            for (int col = 0; col < static_cast<int>(matrix.size()); col++) {
                if (col != row) {
                    int mult = static_cast<int>(static_cast<double>(matrix[col][row]) / matrix[row][row]);
                    for (int i = 0; i < rank; i++)
                        matrix[col][i] -= mult * matrix[row][i];
                }
            }
        }
        // ���� ������� ������� �� ��������� �������
        else {
            // ����� ���������� �������� ���� ������� ������
            bool reduce = true;
            for (int i = row + 1; i < static_cast<int>(matrix.size()); i++) {
                if (matrix[i][row]) {
                    swap(matrix[row], matrix[i]);
                    reduce = false;
                    break;
                }
            }
            // ���� ��������� ������� �� ������
            if (reduce) {
                // ���������� ����� � �������� �������� �������
                rank--;
                for (int i = 0; i < static_cast<int>(matrix.size()); i++)
                    matrix[i][row] = matrix[i][rank];
            }
            row--;
        }
    }
    return rank;
}

// ������� ��� ���������������� �������
Matrix transposeMatrix(const Matrix& matrix) {
    // ��������� ���������� ����� � ��������
    int rows = matrix.size();
    int cols = matrix[0].size();
    // �������� ����������������� �������
    Matrix transposed(cols, vector<int>(rows));
    // ���������� ����������������� �������
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            transposed[j][i] = matrix[i][j];
        }
    }
    return transposed;
}

// ������� ��� ���������� ������������ �������
int determinant(const Matrix& matrix, int n) {
    // ������������� ������������
    int det = 0;
    // ���� ������� ����� ������ 1x1
    if (n == 1)
        return matrix[0][0];

    // �������� ����������
    Matrix submatrix(n, vector<int>(n));
    // ������������� �����
    int sign = 1;

    // ���������� ������������ �� ���������� �� ������ ������
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

// ������� ��� ���������� ����� �������
int trace(const Matrix& matrix) {
    // ������������� �����
    int trace = 0;
    // ������������ ������������ ���������
    for (int i = 0; i < static_cast<int>(matrix.size()); ++i) {
        trace += matrix[i][i];
    }
    return trace;
}

// ������ ������� ��� ���������� �������� �������
Matrix inverseMatrix(const Matrix& matrix) {
    // ��������� ����������� �������
    int n = static_cast<int>(matrix.size());
    // �������� ����������� �������
    Matrix augmentedMatrix(n, vector<int>(2 * n));
    // ���������� ����������� �������
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            augmentedMatrix[i][j] = matrix[i][j];
        }
        // ���������� ��������� ������� ������ �� ��������
        augmentedMatrix[i][n + i] = 1;
    }
    // ���������� � ������������� ���� ������� ������
    for (int i = 0; i < n; ++i) {
        // �������� �� ���� �� ���������
        if (augmentedMatrix[i][i] == 0) {
            bool swapped = false;
            // ����� ������ � ��������� ��������� ��� ������
            for (int j = i + 1; j < n; ++j) {
                if (augmentedMatrix[j][i] != 0) {
                    swap(augmentedMatrix[i], augmentedMatrix[j]);
                    swapped = true;
                    break;
                }
            }
            // ���� ����� ������ �� �������, ������� ����������
            if (!swapped) {
                throw runtime_error("������� �� �������� ���������");
            }
        }
        // ���������� ������� ������ � ���������� ������������� ��������
        int pivot = augmentedMatrix[i][i];
        for (int j = 0; j < 2 * n; ++j) {
            augmentedMatrix[i][j] /= pivot;
        }
        // ��������� ��������� � ��������, ����� ���������
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
// �������� � ���������� ������� �������� � ��������
Matrix inverse(n, vector<int>(n));
for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
        inverse[i][j] = augmentedMatrix[i][j + n];
    }
}
return inverse;
}

// ������� ��� �������� ��������� ���� ������
bool areMatricesEqual(const Matrix& matrix1, const Matrix& matrix2) {
    // �������� �������� ������
    if (matrix1.size() != matrix2.size() || matrix1[0].size() != matrix2[0].size()) {
        return false;
    }
    // �������� ������� �������� ������ �� ���������
    for (int i = 0; i < static_cast<int>(matrix1.size()); ++i) {
        for (int j = 0; j < static_cast<int>(matrix1[0].size()); ++j) {
            if (matrix1[i][j] != matrix2[i][j]) {
                return false;
            }
        }
    }
    return true;
}

// ������� ��� �������� ���� ������
Matrix addMatrices(const Matrix& matrix1, const Matrix& matrix2) {
    // ��������� �������� ������
    int rows = static_cast<int>(matrix1.size());
    int cols = static_cast<int>(matrix1[0].size());
    // �������� �������������� �������
    Matrix result(rows, vector<int>(cols));
    // �������� ��������������� ��������� ���� ������
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            result[i][j] = matrix1[i][j] + matrix2[i][j];
        }
    }
    return result;
}

// ������� ��� ��������� ���� ������
Matrix multiplyMatrices(const Matrix& matrix1, const Matrix& matrix2) {
    // ��������� �������� ������
    int rows1 = static_cast<int>(matrix1.size());
    int cols1 = static_cast<int>(matrix1[0].size());
    int cols2 = static_cast<int>(matrix2[0].size());
    // �������� �������������� �������
    Matrix result(rows1, vector<int>(cols2));
    // ��������� ������
    for (int i = 0; i < rows1; ++i) {
        for (int j = 0; j < cols2; ++j) {
            for (int k = 0; k < cols1; ++k) {
                result[i][j] += matrix1[i][k] * matrix2[k][j];
            }
        }
    }
    return result;
}

// �������� ������� ���������
int main() {
    // ��������� ������ ��� ������ �� ������� �����
    setlocale(LC_ALL, "Russian");
    // ���������� ���������� ��� �������� ������
    int rows1, cols1, rows2, cols2;
    // ���� �������� � ��������� ������
    cout << "������� ���������� ����� � �������� ��� ������� 1: ";
    cin >> rows1 >> cols1;
    Matrix matrix1 = inputMatrix(rows1, cols1);

    cout << "������� ���������� ����� � �������� ��� ������� 2: ";
    cin >> rows2 >> cols2;
    Matrix matrix2 = inputMatrix(rows2, cols2);

    // ����� ������ �� �����
    cout << "������� 1:" << endl;
    printMatrix(matrix1);
    cout << "������� 2:" << endl;
    printMatrix(matrix2);

    // ����� ������ ������
    cout << "���� ������� 1: " << rankOfMatrix(matrix1) << endl;
    cout << "���� ������� 2: " << rankOfMatrix(matrix2) << endl;

    // ����� ����������������� ������
    cout << "����������������� ������� 1:" << endl;
    printMatrix(transposeMatrix(matrix1));
    // ����� ����������������� ������� 2
    cout << "����������������� ������� 2:" << endl;
    printMatrix(transposeMatrix(matrix2));

    // �������� ������� ���������� ����� ������� 1
    if (rows1 == cols1) {
        // ����� ���������� � ������� 1
        cout << "������� 1 �������� ����������." << endl;
        cout << "������������ ������� 1: " << determinant(matrix1, rows1) << endl;
        cout << "���� ������� 1: " << trace(matrix1) << endl;
        try {
            // ������� ��������� �������� ������� ��� ������� 1
            Matrix inverse1 = inverseMatrix(matrix1);
            // ����� �������� �������
            cout << "�������� ������� 1:" << endl;
            printMatrix(inverse1);
        }
        catch (const runtime_error& e) {
            // ����� ��������� �� ������
            cerr << "������: " << e.what() << endl;
        }
    }
    else {
        cout << "������� 1 �� �������� ����������, ������� ������������ � �������� ������� �� ����� ���� ���������." << endl;
    }

    // �������� ������� ���������� ����� ������� 2
    if (rows2 == cols2) {
        // ����� ���������� � ������� 2
        cout << "������� 2 �������� ����������." << endl;
        cout << "������������ ������� 2: " << determinant(matrix2, rows2) << endl;
        cout << "���� ������� 2: " << trace(matrix2) << endl;
        try {
            // ������� ��������� �������� ������� ��� ������� 2
            Matrix inverse2 = inverseMatrix(matrix2);
            // ����� �������� �������
            cout << "�������� ������� 2:" << endl;
            printMatrix(inverse2);
        }
        catch (const runtime_error& e) {
            // ����� ��������� �� ������
            cerr << "������: " << e.what() << endl;
        }
    }
    else {
        cout << "������� 2 �� �������� ����������, ������� ������������ � �������� ������� �� ����� ���� ���������." << endl;
    }

    // �������� ����������� �������� ������
    if (rows1 == rows2 && cols1 == cols2) {
        // ���������� ����� ������
        Matrix sum = addMatrices(matrix1, matrix2);
        // ����� ���������� ��������
        cout << "����� ������:" << endl;
        printMatrix(sum);
    }
    else {
        cout << "������� �� ����� ���� ������� ��-�� ������ ������������." << endl;
    }

    // �������� ����������� ��������� ������
    if (cols1 == rows2) {
        // ���������� ������������ ������
        Matrix product = multiplyMatrices(matrix1, matrix2);
        // ����� ���������� ���������
        cout << "������������ ������:" << endl;
        printMatrix(product);
    }
    else {
        cout << "������� �� ����� ���� ����������� ��-�� ������������� ������������." << endl;
    }

    return 0;
}