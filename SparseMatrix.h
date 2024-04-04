//
// Created by Bekir Sadık Altunkaya on 13.03.2024.
//

#ifndef SPARSEMATRIXANDVECTOR_SPARSEMATRIX_H
#define SPARSEMATRIXANDVECTOR_SPARSEMATRIX_H
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
// structure for single element in row
struct Element {
    int index;
    double data;
};

// structure for single row
struct Row {
    int rowNumber;              // Row number
    std::vector<Element> elements; // Elements in the row
};
class SparseMatrix
{
private:
    std::vector<Row> rows;//all data itself
public:
    SparseMatrix();
    SparseMatrix(const std::string& fileName);
    bool operator==(const SparseMatrix& other) const;
    bool operator!=(const SparseMatrix& other) const { return !(*this == other); }//simple not operator implemented as inline
    SparseMatrix operator+(const SparseMatrix& other) const;
    SparseMatrix operator-(const SparseMatrix& other) const;

    SparseMatrix operator-() const;
    SparseMatrix& operator=(const SparseMatrix& other);
    SparseMatrix operator*(const SparseMatrix& other) const;
    friend std::ostream& operator<<(std::ostream& os, const SparseMatrix& spMat);
    SparseMatrix transpose() const;

};


#endif //SPARSEMATRIXANDVECTOR_SPARSEMATRIX_H