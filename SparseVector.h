//
// Created by Bekir Sadık Altunkaya on 13.03.2024.
//

#ifndef SPARSEMATRIXANDVECTOR_SPARSEVECTOR_H
#define SPARSEMATRIXANDVECTOR_SPARSEVECTOR_H
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
//structure to hold vector elements
struct Data
{
    int m_index;
    double m_value;
    // constructor for easy insertion
    Data(int index, double value) : m_index(index), m_value(value) {}
};

class SparseVector
{
private:
    std::vector<Data> allData;//vector to hold data
public:
    SparseVector();
    SparseVector(std::string fileName);
    friend SparseVector& largerVector(SparseVector& spVec1,SparseVector& spVec2);
    friend int largestIndex(SparseVector& spVec1,SparseVector& spVec2);
    double getValueAt(int targetInd);
    void addValue(int index, double value);
    void adjustValue(int index, double value);
    bool operator==(SparseVector& other) const;
    bool operator!=(SparseVector& other) const { return !(*this == other); }//simple not operator implemented as inline

    SparseVector operator+(const SparseVector& other) const;
    SparseVector operator-(const SparseVector& other) const;
    SparseVector operator-();
    SparseVector& operator=(SparseVector const& other);
    friend std::ostream& operator<<(std::ostream& os, const SparseVector& spVec);

    friend double dot(SparseVector& sp1, SparseVector& sp2);

};

#endif //SPARSEMATRIXANDVECTOR_SPARSEVECTOR_H
