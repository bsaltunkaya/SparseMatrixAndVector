 //
// Created by Bekir Sadık Altunkaya on 19.03.2024.
//
#include "SparseMatrix.h"


SparseMatrix::SparseMatrix()//default constructor
{
//not implemented because not needed at all
}

SparseMatrix::SparseMatrix(const std::string& fileName) {//constructor with file name
  std::ifstream file(fileName);
  std::string line;
  while (getline(file, line)) {
    std::istringstream lineStream(line);
    int rowNumber;
    lineStream >> rowNumber;
    Row row{rowNumber};

    std::string element;
    while (lineStream >> element) {
      std::istringstream elementStream(element);
      int index;
      double data;
      char colon;
      elementStream >> index >> colon >> data;
      row.elements.push_back({index, data});
    }
    rows.push_back(row);
  }
}



bool SparseMatrix::operator==(const SparseMatrix& other) const {//comparison operator
  // != operator implemented inline
  if (rows.size() != other.rows.size()) return false;
  for (size_t i = 0; i < rows.size(); ++i) {
    const Row& row1 = rows[i];
    const Row& row2 = other.rows[i];
    if (row1.rowNumber != row2.rowNumber || row1.elements.size() != row2.elements.size()) return false;
    for (size_t j = 0; j < row1.elements.size(); ++j) {
      if (row1.elements[j].index != row2.elements[j].index || row1.elements[j].data != row2.elements[j].data) {
        return false;
      }
    }
  }
  return true;
}

SparseMatrix SparseMatrix::operator+(const SparseMatrix& other) const {
  SparseMatrix result;

  for (const auto& thisRow : this->rows) {
    Row newRow{thisRow.rowNumber};

    auto it = std::find_if(other.rows.begin(), other.rows.end(),
                           [&](const Row& otherRow) { return otherRow.rowNumber == thisRow.rowNumber; });

    if (it != other.rows.end()) {
      const auto& otherRow = *it;

      auto thisElem = thisRow.elements.begin();
      auto otherElem = otherRow.elements.begin();
      while (thisElem != thisRow.elements.end() || otherElem != otherRow.elements.end()) {
        if (thisElem != thisRow.elements.end() && (otherElem == otherRow.elements.end() || thisElem->index < otherElem->index)) {
          newRow.elements.push_back(*thisElem);
          ++thisElem;
        } else if (otherElem != otherRow.elements.end() && (thisElem == thisRow.elements.end() || otherElem->index < thisElem->index)) {
          newRow.elements.push_back(*otherElem);
          ++otherElem;
        } else if (thisElem != thisRow.elements.end() && otherElem != otherRow.elements.end()) {
          double sum = thisElem->data + otherElem->data;
          if (sum != 0) { // If the addition results in zero, do not add the element
            newRow.elements.push_back({thisElem->index, sum});
          }
          ++thisElem;
          ++otherElem;
        }
      }
    } else {
      newRow = thisRow;
    }
    if (!newRow.elements.empty()) {
      result.rows.push_back(newRow);
    }
  }

  for (const auto& otherRow : other.rows) {
    if (std::none_of(result.rows.begin(), result.rows.end(),
                     [&](const Row& resRow) { return resRow.rowNumber == otherRow.rowNumber; })) {
      result.rows.push_back(otherRow);
    }
  }

  std::sort(result.rows.begin(), result.rows.end(), [](const Row& a, const Row& b) {
      return a.rowNumber < b.rowNumber;
  });

  return result;
}
SparseMatrix SparseMatrix::operator-(const SparseMatrix& other) const {
  SparseMatrix result;

  for (const auto &thisRow: this->rows) {
    bool rowFound = false;
    for (const auto &otherRow: other.rows) {
      if (thisRow.rowNumber == otherRow.rowNumber) {
        Row newRow;
        newRow.rowNumber = thisRow.rowNumber;

        auto thisElem = thisRow.elements.begin();
        auto otherElem = otherRow.elements.begin();
        while (thisElem != thisRow.elements.end() || otherElem != otherRow.elements.end()) {
          if (thisElem != thisRow.elements.end() &&
              (otherElem == otherRow.elements.end() || thisElem->index < otherElem->index)) {
            newRow.elements.push_back(*thisElem);
            ++thisElem;
          } else if (otherElem != otherRow.elements.end() &&
                     (thisElem == thisRow.elements.end() || otherElem->index < thisElem->index)) {
            newRow.elements.push_back(Element{otherElem->index, -otherElem->data});
            ++otherElem;
          } else if (thisElem->index == otherElem->index) {
            double diff = thisElem->data - otherElem->data;
            if (diff != 0) { // If the subtraction results in zero, do not add the element
              newRow.elements.push_back(Element{thisElem->index, diff});
            }
            ++thisElem;
            ++otherElem;
          }
        }
        if (!newRow.elements.empty()) {
          result.rows.push_back(newRow);
        }
        rowFound = true;
        break;
      }
    }
    if (!rowFound) {
      result.rows.push_back(thisRow);
    }
  }

  for (const auto &otherRow: other.rows) {
    bool rowExists = false;
    for (const auto &thisRow: this->rows) {
      if (thisRow.rowNumber == otherRow.rowNumber) {
        rowExists = true;
        break;
      }
    }
    if (!rowExists) {
      Row newRow;
      newRow.rowNumber = otherRow.rowNumber;
      for (const auto &elem: otherRow.elements) {
        newRow.elements.push_back(Element{elem.index, -elem.data});
      }
      if (!newRow.elements.empty()) {
        result.rows.push_back(newRow);
      }
    }
  }

  std::sort(result.rows.begin(), result.rows.end(), [](const Row &a, const Row &b) {
      return a.rowNumber < b.rowNumber;
  });

  return result;
}


//output operator,fairly simple
std::ostream& operator<<(std::ostream& os, const SparseMatrix& spMat) {
  for (const auto& row : spMat.rows) {
    os << row.rowNumber << " ";
    for (const auto& elem : row.elements) {
      os << elem.index << ":" << elem.data << " ";
    }
    os << "\n";
  }
  return os;
}

SparseMatrix SparseMatrix::operator-() const {//negation operator, just *-1's them all
  SparseMatrix result;
  for (const auto& row : this->rows) {
    Row newRow;
    newRow.rowNumber = row.rowNumber;
    for (const auto& elem : row.elements) {
      newRow.elements.push_back(Element{elem.index, -elem.data});
    }
    result.rows.push_back(newRow);
  }
  return result;
}

SparseMatrix& SparseMatrix::operator=(const SparseMatrix& other) {
  // self assignment check
  if (this == &other) {
    return *this;
  }

  // copy data
  rows = other.rows;

  return *this;
}

SparseMatrix SparseMatrix::operator*(const SparseMatrix& other) const {
  SparseMatrix result;
  // iterate through each row of the first matrix
  for (const auto& thisRow : this->rows) {
    Row newRow;
    newRow.rowNumber = thisRow.rowNumber;
    // implement temporary vector to store multiplication results before sending out to newRow
    std::vector<double> tempResults(other.rows.size(), 0.0);

    // iterate through each element of the current row
    for (const auto& thisElem : thisRow.elements) {
      // find/multiply matching elements in the other matrix
      for (const auto& otherRow : other.rows) {
        for (const auto& otherElem : otherRow.elements) {
          if (thisElem.index == otherRow.rowNumber) { // Matching element found

            tempResults[otherElem.index] += thisElem.data * otherElem.data;
          }
        }
      }
    }

    // convert tempResults into sparse format for the newRow
    for (int i = 0; i < tempResults.size(); ++i) {
      if (tempResults[i] != 0) { // store only non zero values
        newRow.elements.push_back({i, tempResults[i]});
      }
    }

    if (!newRow.elements.empty()) {
      result.rows.push_back(newRow);
    }
  }
//sorts before returning object
  std::sort(result.rows.begin(), result.rows.end(), [](const Row& a, const Row& b) {
      return a.rowNumber < b.rowNumber;
  });

  return result;
}

SparseMatrix SparseMatrix::transpose() const {
  SparseMatrix transposedMatrix;
  // find the total number of columns in the original matrix to init rows in the transposed matrix
  int maxColumnIndex = 0;
  for (const auto& row : this->rows) {
    for (const auto& elem : row.elements) {
      if (elem.index > maxColumnIndex) {
        maxColumnIndex = elem.index;
      }
    }
  }

  // init rows for the transposed matrix
  transposedMatrix.rows.resize(maxColumnIndex + 1); // index starts from 0
  for (int i = 0; i <= maxColumnIndex; ++i) {
    transposedMatrix.rows[i].rowNumber = i;
  }

  // fill the transposed matrix
  for (const auto& row : this->rows) {
    for (const auto& elem : row.elements) {
      // row index of the original matrix becomes the column index in the transposed matrix etc
      Element transposedElem = {row.rowNumber, elem.data}; // swap index and rowNumber
      transposedMatrix.rows[elem.index].elements.push_back(transposedElem);
    }
  }
  std::sort(transposedMatrix.rows.begin(), transposedMatrix.rows.end(), [](const Row& a, const Row& b) {
      return a.rowNumber < b.rowNumber;
  });

  return transposedMatrix;
}