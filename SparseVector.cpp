//
// Created by Bekir Sadık Altunkaya on 19.03.2024.
//

#include "SparseVector.h"
SparseVector::SparseVector()//default constructor
{
//not implemented because not needed
}

SparseVector::SparseVector(std::string fileName)//constructor which will be used
{
  std::ifstream file(fileName);
  if(!file.is_open())//basic error check
  {
    std::cout<<"Could not open file: "<<fileName<<"\n";
    return;
  }
  else
  {//begin parsing txt
    std::string line;
    while(std::getline(file,line))
    {
      size_t startPos=0,spacePos=0;//size_t used for npos
      //i am aware of the fact that we haven't seen this in the class yet but
      //all of my research proved this to be the most feasible way for a student with
      //no string knowledge.
      while((spacePos=line.find(' ',startPos))!=std::string::npos)//token init'ing until space
      {
        auto token=line.substr(startPos,spacePos-startPos);
        auto colonPos=token.find(':');
        if(colonPos!=std::string::npos)
        {
          auto indexStr=token.substr(0,colonPos);//auto used because it wascovered in first lecture
          auto valueStr=token.substr(colonPos+1);
          int index = std::stoi(indexStr);//breaking down tokens
          double value=std::stod(valueStr);
          allData.push_back({index,value});
        }
        startPos=spacePos+1;//ready to get more tokens
      }
      auto token=line.substr(startPos);
      auto colonPos=token.find(':');
      if(colonPos!=std::string::npos)//process any other remaining tokens after last space
      {
        auto indexStr=token.substr(0,colonPos);
        auto valueStr=token.substr(colonPos+1);
        int index=std::stoi(indexStr);
        double value=std::stod(valueStr);
        allData.push_back({index,value});
      }
    }
  }
}

void SparseVector::addValue(int index, double value) {
  // check if vector is empty or the index is larger than the last index
  if (allData.empty() || index > allData.back().m_index) {
    allData.push_back(Data(index, value));
    return;
  }

  for (auto it = allData.begin(); it != allData.end(); ++it) {
    // if index already exists, update the value
    if (it->m_index == index) {
      it->m_value += value;
      return;
    }
      // if current position's index is greater than the new index, insert before this position
    else if (it->m_index > index) {
      allData.insert(it, Data(index, value));
      return;
    }
  }
}


void SparseVector::adjustValue(int index, double value) {
  // find the position to insert or adjust the value
  for (auto it = allData.begin(); it != allData.end(); ++it) {
    if (it->m_index == index) {
      it->m_value += value; // Adjust the value (add or subtract)
      // if adjusted value is zero, remove the element
      if (it->m_value == 0.0) {
        allData.erase(it);
      }
      return;
    } else if (it->m_index > index) {
      // insert only if the value is not zero
      if (value != 0.0) {
        allData.insert(it, Data(index, value));
      }
      return;
    }
  }
  // if we didn't return early, the new index is larger than any existing index, and the value is not zero
  if (value != 0.0) {
    allData.push_back(Data(index, value));
  }
}

bool SparseVector::operator==(SparseVector& other) const {
  // vector size equality check
  if (this->allData.size() != other.allData.size()) {
    return false;
  }

  // element equality check
  for (size_t i = 0; i < this->allData.size(); ++i) {
    // check for the index and value equality at the same time
    if ((this->allData[i].m_index != other.allData[i].m_index) ||
        (this->allData[i].m_value != other.allData[i].m_value)) {
      return false;
    }
  }

  // if no return statements are triggered, then vectors must be equal.
  return true;
}

int largestIndex(SparseVector& spVec1,SparseVector& spVec2)
{
  //research provided .back() operator to be the most useful
  if(spVec1.allData.back().m_index>spVec2.allData.back().m_index)
  {
    return spVec1.allData.back().m_index;
  }
  else
  {
    return spVec2.allData.back().m_index;
  }
}

double SparseVector::getValueAt(int targetInd)
{
  for(int i=0;i<allData.size();i++)
  {
    if(allData[i].m_index==targetInd)
    {
      return allData[i].m_value;
    }
    else
    {
      return 0;
    }
  }
}


SparseVector SparseVector::operator+(const SparseVector& other) const {
  SparseVector result;

  // Iterate through all elements of *this
  for (const auto& data : this->allData) {
    result.addValue(data.m_index, data.m_value);
  }

  // Iterate through all elements of other
  for (const auto& data : other.allData) {
    result.addValue(data.m_index, data.m_value);
  }

  return result;
}
SparseVector SparseVector::operator-(const SparseVector& other) const {
  SparseVector result = *this; // Start with a copy of the current vector

  // Iterate through all elements of other, subtracting from the result
  for (const auto& data : other.allData) {
    result.adjustValue(data.m_index, -data.m_value);
  }

  return result;
}


SparseVector SparseVector::operator-()
{
  for(int i=0;i<allData.size();i++) {
    this->allData[i].m_value = -this->allData[i].m_value;
  }
  return *this;
}

SparseVector& SparseVector::operator=(SparseVector const& other)
{
  this->allData=other.allData;
  return *this;
}

std::ostream& operator<<(std::ostream& os, const SparseVector& spVec) {
  for (const auto& data : spVec.allData) {
    os << data.m_index << ":" << data.m_value << " ";
  }
  return os;
}

double dot(SparseVector& sp1, SparseVector& sp2)
{
  double result=0;
  int max=largestIndex(sp1,sp2);
  for(int i=0;i<max;i++)
  {
    result+=sp1.getValueAt(i)*sp2.getValueAt(i);
  }
  return result;
}
SparseVector& largerVector(SparseVector& spVec1,SparseVector& spVec2)
{
  if(spVec1.allData.size()>spVec2.allData.size())
  {
    return spVec1;
  }
  else
  {
    return spVec2;
  }
}
