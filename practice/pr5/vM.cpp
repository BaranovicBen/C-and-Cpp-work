#include <iostream>
#include <vector>
#include <memory>
#include <stdexcept>
#include <cassert>

template <typename T_>
struct CMatrixView;

template <typename T_>
struct CMatrix{
    size_t m_H;
    size_t m_W;
    std::vector<std::vector<T_>> m_Data;
    std::shared_ptr<bool> m_Token = std::make_shared<bool>(true);

    CMatrix(const size_t h, const size_t w)
         : m_H(h), m_W(w){
        for (size_t i = 0; i < h; i++)
            m_Data.emplace_back(m_W, T_());
    }
  
    T_ & operator()(size_t row, size_t col){
        if (row >= m_H || col >= m_W)
            throw std::out_of_range("out of range");
        return m_Data[row][col];
    }
 
    CMatrixView<T_> createView (ssize_t rowOffset, ssize_t colOffset){
             std::cerr<<"Tvorim view\n"; 
        return CMatrixView<T_>(this,rowOffset,colOffset,m_Token);
    }
};


template <typename T_>
struct CMatrixView{
    CMatrix<T_>* m_OrigMatrix;
    ssize_t m_RowOffset;
    ssize_t m_ColOffset;
    std::shared_ptr<bool>  m_token;

    CMatrixView(CMatrix<T_>* origMatrix, ssize_t rowOffset, ssize_t colOffset, std::shared_ptr<bool> token)
        : m_OrigMatrix(origMatrix), m_RowOffset(rowOffset), m_ColOffset(colOffset), m_token(token)
    {
             std::cerr<<"Konstruktujem view\n"; 
    };

    T_ & operator()(ssize_t rowOffset, ssize_t colOffset){
        std::cerr<<"Hej ty exsitujes?\n"; 
        if(!m_token || !*m_token)
            throw std::logic_error("Matrix is destroyed or reassigned");
        ssize_t realRow = rowOffset + m_RowOffset;
        ssize_t realCol = colOffset + m_ColOffset;
                     
        std::cerr<<"Hej ty si valid?\n"; 
        if(realRow < 0 || realCol < 0 || static_cast<size_t>(realRow) >= m_OrigMatrix->m_H || static_cast<size_t>(realCol) >= m_OrigMatrix->m_W)
            throw std::out_of_range("Access out of range");
        std::cerr<<"Passed vibe check\n";         
        return(*m_OrigMatrix)(realRow,realCol);
    };
};

int main(){
    bool exceptionThrown = false;
 
    CMatrix<int> m1(3, 3);
 
    m1(0, 0) = 100;
    m1(0, 1) = 200;
    m1(2, 2) = 400;
    m1(1, 1) = 0;
 
    assert(m1(0, 0) == 100);
    assert(m1(1, 1) == 0);
    assert(m1(2, 2) == 400);
 
    CMatrixView<int> v1 = m1.createView(1, 1);

    assert(v1(-1, -1) == 100);
    v1(-1, -1) = 50;
    assert(v1(-1, -1) == 50);

    exceptionThrown = false;
    try {
        v1(2, 2) = 0;
    } catch (const std::out_of_range &) {
        exceptionThrown = true;
    }
    assert(exceptionThrown);
      std::cerr<<"Test 1\n"; 

    CMatrix<int> m2(4, 5);
 
    m2(1, 3) = 100;
    m2(0, 2) = 200;
    m2(3, 4) = 400;
    m2(0, 1) = 600;

    assert(m2(1, 3) == 100);
    assert(m2(0, 1) == 600);
    assert(m2(3, 4) == 400);
 
    CMatrixView<int> v2 = m2.createView(-2, 5);
      std::cerr<<"Test 2\n"; 

    assert(v2(2, -3) == 200);
    assert(v2(5, -1) == 400);
 
    CMatrixView<int> v3(v2);
      std::cerr<<"Test 3\n"; 

    assert(v3(2, -3) == 200);
    assert(v3(5, -1) == 400);
 
    v3 = v1;
 
    assert(v3(-1, -1) == 50);
       std::cerr<<"Test 4\n"; 
    m2 = m1;
 
    exceptionThrown = false;
    try {
        v2(2, 2) = 0;
    } catch (const std::logic_error &) {
        exceptionThrown = true;
    }
    assert(exceptionThrown);
       std::cerr<<"Test 5\n"; 

    return EXIT_SUCCESS;
}