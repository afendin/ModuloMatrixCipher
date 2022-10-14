#include<iostream>
#include<math.h>
#include <vector>
#include<string.h>
using namespace std;

#define BASE 127
#define SPACE  32

// a hack function to always get a positive modulo
int positiveModulo(int i, int n)
{
    return (i % n + n) % n;
}

int main()
{
    int n;
    cout << "Enter number of rows:\n";
    cin >> n;
    int matrix[n][n];
    int oMatrix[n][n]; // original matrix
    int iMatrix[n][n]; // inverted matrix
    string message = "Well let's try to encode this one.";
    std::vector<int> messageVector;

    // For modular base I chose number 127, because it is prime, and we won't
    // have to care about the fact that determinant of the key matrix must not
    // have any common factors with the modular base. And from the other side
    // modular base 127 perfectry matches with ASCII and we can directry
    // convert every symbol of our message to int without using special mappings
    for (char letter : message)
        messageVector.push_back(int(letter));

    // We encode message by parts that equal n(number of rows in matrix) so we have
    // to append spaces at the end of the message vector to complete incomplete part
    messageVector.insert(messageVector.end(), n - messageVector.size() % n, SPACE);

    std::vector<int> encodedVector(messageVector.size(), 0);
    std::vector<int> decodedVector(messageVector.size(), 0);


    // I recommend you generate matrices with online tool below
    // https://www.intmath.com/matrices-determinants/inverse-matrix-gauss-jordan-elimination.php
    // because it gives you invertible matrices that are required for message to be decoded
    cout << "\nFill the matrix:\n";
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cin >> matrix[i][j];
            oMatrix[i][j] = matrix[i][j];
        }
    }

    // initialize inverted matrix vector as identity matrix first
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i == j) {
                iMatrix[i][j] = 1;
                continue;
            }

            iMatrix[i][j] = 0;
        }
    }



    // Matrix inversion logic. For better understanding what is actually happening here
    // I recommend you to follow link below, generate some matrix, and get clear vision
    // https://www.intmath.com/matrices-determinants/inverse-matrix-gauss-jordan-elimination.php

    for (int column = 0; column < n; ++column) {  
        int diagonalElement = matrix[column][column];
        int factor = 0;

        // find factor that makes diagonal element 1
        while (positiveModulo(diagonalElement * factor, BASE) != 1)
            ++factor;

        // multiply whole row of both martix to that factor
        for (int row = column; row < n; ++row)
            matrix[column][row] = positiveModulo(matrix[column][row] * factor, BASE);
        for (int row = 0; row <n; ++row)
            iMatrix[column][row] = positiveModulo(iMatrix[column][row] * factor, BASE);

        // Zero out upper right part of the matrix
        for (int inColumn = 0; inColumn < column; ++inColumn) {
            int factor = matrix[inColumn][column];

            for (int row = column; row < n; ++row) {
                matrix[inColumn][row] = matrix[inColumn][row] - factor * matrix[column][row];
                matrix[inColumn][row] = positiveModulo(matrix[inColumn][row], BASE);
            }

            for (int row = 0; row < n; ++row) {
                iMatrix[inColumn][row] = iMatrix[inColumn][row] - factor * iMatrix[column][row];
                iMatrix[inColumn][row] = positiveModulo(iMatrix[inColumn][row], BASE);
            }
        }

        // Zero out bottom left part of the matrix
        int lRow = column; // leading row
        for (int row = lRow + 1; row < n; ++ row) {
            int factor = matrix[row][column];

            for (int term = lRow; term < n; ++term) {
                matrix[row][term] = matrix[lRow][term] * factor - matrix[row][term];
                matrix[row][term] = positiveModulo(matrix[row][term], BASE);
            }
            for (int term = 0; term < n; ++term) {
                iMatrix[row][term] = iMatrix[lRow][term] * factor - iMatrix[row][term];
                iMatrix[row][term] = positiveModulo(iMatrix[row][term], BASE);
            }
        }
    }


    cout << "\nOriginal matrix:\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << oMatrix[i][j] << "      ";
        }
        cout << endl;
    }

    cout << "\nInverted matrix:\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << iMatrix[i][j] << "      ";
        }
        cout << endl;
    }


    // Encode our message with original matrix
    for (int step = 0; step < messageVector.size(); step += n) {
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                encodedVector[step + i] += oMatrix[i][j] * messageVector[step + j];
                encodedVector[step + i] = positiveModulo(encodedVector[step + i], BASE);
            }
        }
    }

    // Decode our message with inverse matrix
    for (int step = 0; step < messageVector.size(); step += n) {
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                decodedVector[step + i] += iMatrix[i][j] * encodedVector[step + j] ;
                decodedVector[step + i] = positiveModulo(decodedVector[step + i], BASE);
            }
        }
    }


    cout << "\nOriginal message:\n";
    for (int i : messageVector)
        cout << char(i);
    cout << endl;

    cout << "\nEncoded message:\n";
    for (int i : encodedVector)
        cout << char(i);
    cout << endl;

    cout << "\nDecoded message:\n";
    for (int i : decodedVector)
        cout << char(i);
    cout << endl;



    return 0;
}
