#include <sstream>
#include <cstddef>
#include <math.h>
#include <iostream>
#include <type_traits>
#include <cstring>
#include <initializer_list>
#include <iostream>
#include <vector>


template <class T>
class Matris {
public:

	Matris(std::initializer_list<T> l);
	Matris(const size_t rows, const size_t cols);
	static_assert(std::is_move_constructible<T>::value && std::is_move_assignable<T>::value,
		"requires moveConstructible and moveAssignable");

	using iterator = T*;
	explicit Matris(const size_t size);
	Matris();
	Matris(const Matris<T>& m);
	Matris(Matris<T>&& m);
	~Matris();
	size_t rows();
	size_t cols();
	Matris& operator=(const Matris<T>& mx);
	Matris& operator=(Matris<T>&& mx);

	T& operator()(size_t row, size_t col);
	void reset();
	void insert_row(int row);
	void append_row(int row);
	void remove_row(int row);

	void insert_column(int col);
	void append_column(int col);
	void remove_column(int col);
	void transpose();
	void operator+=(const Matris<T>& m);
	void operator+=(size_t s);
	void operator-=(const Matris<T>& m);
	void operator-=(size_t s);
	void operator*=(Matris<T>& m);
	void operator*=(size_t s);
	iterator begin();
	iterator end();

	//Matris<T>& operator*(Matris<T>&m, size_t s);
	//void remove_row();
	//T * m_vec;
private:
	size_t m_rows;
   size_t m_cols;
   size_t m_capacity;
   T * m_vec;
};
template<class T>
std::ostream& operator<<(std::ostream& os, Matris<T>& m);
template<class T>
std::istream& operator>>(std::istream& is, Matris<T>& m);

template<class T>
Matris<T> identity(unsigned int s);

template<class T>
Matris<T>& operator*(Matris<T>& m, Matris<T>& mx);

template<class T>
Matris<T>& operator*(Matris<T>& m, size_t s);

template<class T>
Matris<T>& operator*(size_t s, Matris<T>& m);

template<class T>
Matris<T>& operator-(Matris<T>& m, Matris<T>& mx);

template<class T>
Matris<T>& operator-(Matris<T>& m, size_t s);

template<class T>
Matris<T>& operator+(Matris<T>& m, Matris<T>& mx);

template<class T>
Matris<T>& operator+(Matris<T>& m, size_t s);














//c++ code
// constructors
// l value = bundna variabler, ex: int a = b. R value: ett temporät värde vi inte "binder" till bågot, ex: funktion return.
// && = r value reference. && matchar ex bar(foo) -> bar(int &&)

//constructors
template<class T>
Matris<T>::Matris(std::initializer_list<T> l){
    if(fmod(sqrt(l.size()),1) != 0){
      throw std::out_of_range("The matrix needs to be a square matrix");
  }
  m_vec = new T[l.size()];
  std::copy(l.begin(),l.end(),m_vec);
  m_capacity= l.size();
  m_rows = sqrt(l.size());
  m_cols = sqrt(l.size());
}


template<class T>
Matris<T>::Matris(const size_t rows, const size_t cols){
    m_rows = rows;
    m_cols = cols;
    m_capacity = rows * cols;
    m_vec = new T[m_capacity];
    for(int i = 0; i < m_capacity;i++){
        m_vec[i] = T();
    }
}

template<class T>
Matris<T>::Matris(const size_t size){
    m_rows = size;
    m_cols = size;
    m_capacity = size * size;
    m_vec = new T[m_capacity]; 
    for(int i = 0; i < m_capacity; i++){
        m_vec[i] = T();
    }
}

template<class T>
Matris<T>::Matris(){
    m_rows = 0;
    m_cols = 0;
    m_capacity = 0;
    m_vec = new T[0];
}


template<class T>
Matris<T>::Matris(const Matris<T>& m){
    m_rows = m.m_rows;
    m_cols = m.m_cols;
    m_capacity = m.m_capacity;
    m_vec = new T[m_capacity];
    std::copy(m.m_vec, m.m_vec + m_capacity, m_vec);
}


template<class T>
Matris<T>::Matris(Matris<T>&& m):m_vec(m.m_vec),m_capacity(m.m_capacity),m_rows(m.m_rows),m_cols(m.m_cols){
    std::cout << "u made it" << std::endl;
    m.m_vec = nullptr;
    m.m_capacity = 0;
    m.m_cols = 0;
    m.m_rows = 0;
}

template<class T>
Matris<T>::~Matris(){
    delete []m_vec;
}
  //accessors
template<class T>
size_t Matris<T>::rows(){
    return m_rows;
}
template<class T>
size_t Matris<T>::cols(){
    return m_cols;
}

template<class T>
typename Matris<T>::iterator
Matris<T>::begin(){
    return m_vec;
}
template<class T>
typename Matris<T>::iterator
Matris<T>::end(){
    return m_vec + m_capacity;
}
  // operators
template<class T>
Matris<T>& Matris<T>::operator=(const Matris<T>& mx){
    if(&mx == this){
        return *this;
    }
    if(m_rows != mx.m_rows || m_cols != mx.m_cols){
        m_rows = mx.m_rows;
        m_cols = mx.m_cols;
        m_capacity = mx.m_capacity;
    }

    delete []m_vec;
    m_vec = new T[m_capacity];
    std::copy(mx.m_vec, mx.m_vec + m_capacity, m_vec);
    return *this;
}



template<class T>
Matris<T>& Matris<T>::operator=(Matris<T>&& mx){
    if(m_vec != nullptr){
        delete []m_vec;
    }
    m_vec = std::move(mx.m_vec);
    m_capacity = mx.m_capacity;
    m_cols = mx.m_cols;
    m_rows = mx.m_rows;
    return *this;
}

template<class T>
T& Matris<T>::operator()(size_t row, size_t col){
    if(m_rows <= row || m_cols <= col){
        throw std::out_of_range("out of index");
    }
    return m_vec[row * m_cols + col];
}

template<class T>
void Matris<T>::operator+=(const Matris<T>& m){
    if(m.m_rows != m_rows || m.m_cols != m_cols){
        throw std::out_of_range("The number of columns and rows has to be the same");
    }
    for(size_t i = 0; i < m_capacity; i++){
        m_vec[i] += m.m_vec[i];
    }
}

template<class T>
void Matris<T>::operator+=(const size_t s){
    for(size_t i = 0; i < m_capacity; i++){
        m_vec[i] += s;
    }
}

template<class T>
void Matris<T>::operator-=(const Matris<T>& m){
    if(m.m_rows != m_rows || m.m_cols != m_cols){
        throw std::out_of_range("The number of columns and rows has to be the same");
    }
    for(size_t i = 0; i < m_capacity; i++){
        m_vec[i] -= m.m_vec[i];
    }
}

template<class T>
void Matris<T>::operator-=(size_t s){
    for(size_t i = 0; i < m_capacity; i++){
        m_vec[i] -= s;
    }
}


template<class T>
void Matris<T>::operator*=(Matris<T>& m){
    if(m_cols != m.m_rows){
        throw std::out_of_range("The number of columns and rows has to be the same");
    }
    Matris<T> result(m_rows, m.m_cols);

    for(int i = 0; i < result.m_capacity;i++){
        result.m_vec[i] = T();
    }
    for(size_t i = 0; i < m_rows; i++){
        for(size_t j = 0; j < m.m_cols;j++){
            for(size_t k = 0; k < m_cols; k++){
                result(i, j) += m_vec[i*m_cols + k] * m(k,j);
            }
        }
    }
    *this = result;
}

template<class T>
void Matris<T>::operator*=(const size_t s){
    for(size_t i = 0; i < m_capacity; i++){
        m_vec[i] *= s;
    }
}

template<class T>
void Matris<T>::reset(){
    for(int i = 0; i < m_capacity;i++){
        m_vec[i] = T();
    }
}

template<class T>
void Matris<T>::insert_row(int row){
    if(m_rows < row){
        throw std::out_of_range("Can't add row here");
    }
    T* temp = new T[m_capacity + m_cols];
    std::copy(m_vec, m_vec + row*m_cols, temp);
    std::copy(m_vec + row*m_cols, m_vec + m_capacity, temp + (row+1)*m_cols);
    for(size_t i = 0; i < m_cols; i++){
        temp[i + row*m_cols] = T();
    }
    delete []m_vec;
    m_vec = temp;
    m_capacity += m_cols;
    m_rows += 1;
}

template<class T>
void Matris<T>::append_row(int row){
    insert_row(row + 1);
}

template<class T>
void Matris<T>::remove_row(int row){
    T* temp = new T[m_cols*(m_rows - 1)];
    std::copy(m_vec, m_vec + m_cols*row, temp);
    std::copy(m_vec + m_cols*(row+1), m_vec + m_capacity, temp + m_cols*row);
    delete []m_vec;
    m_vec = temp;
    m_rows = m_rows - 1;
    m_capacity = m_capacity - m_cols;
}

template<class T>
void Matris<T>::insert_column(int col){
    this->transpose();
    this->insert_row(col);
    this->transpose();
}

template<class T>
void Matris<T>::append_column(int col){
    this->transpose();
    this->append_row(col);
    this->transpose();
}

template<class T>
void Matris<T>::remove_column(int col){
    this->transpose();
    this->remove_row(col);
    this->transpose();
}



template<class T>
void Matris<T>::transpose(){
    Matris<T> temp(m_cols, m_rows);
    Matris<T> temp2 = *this;
    for(int i = 0; i < m_cols; i++){
        for(int j = 0; j < m_rows; j++){
            temp(i, j) = temp2(j,i);
        }
    }
    //delete []m_vec;
    *this = temp;
}

template<class T>
std::ostream& operator<<(std::ostream& os, Matris<T>& m){
    for(int i = 0; i < m.rows(); i++){
        for(int j = 0; j < m.cols(); j++){
            os << m(i, j) << " ";
        }
        os << std::endl;
    }
    return os;
}

template<class T>
std::istream& operator>>(std::istream& is, Matris<T>& m){
    T c;
    size_t rows = 1;
    size_t columns = 0;
    std::vector<T> row;
    std::vector<size_t> cols;
    while(is >> c){
        columns++;
        row.push_back(c);

        if(is.peek() == '\n'){
            rows ++;
            cols.push_back(columns);
            columns = 0;
        }
    
        else if(is.peek() == ';'){
            break;
        }
    }
    cols.push_back(columns);

    for(int i = 0; i < int(cols.size())-1;i++){
        if(cols[i] != cols[i+1]){
            throw std::out_of_range("All the rows must be the same length");
        }
    }

    Matris<T> ma(rows, columns);
    int index = 0;
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < columns; j++){
            ma(i,j) = row[index];
            index++;
        }
    }
    m = ma;
    return is;
}

template<class T>
Matris<T> identity(unsigned int s){
    Matris<T> matris(s);
    for(int i = 0; i < matris.rows();i++){
        for(int j = 0; j < matris.cols(); j++){
            if(j == i){
                matris(i,j) = T() + 1;
            }else{
                matris(i,j) = T();
            }
        }
    }
    return matris;
}


template<class T>
Matris<T>& operator*(Matris<T>& m, Matris<T>& mx){
    m *= mx;
    return m;
}

template<class T>
Matris<T>& operator*(Matris<T>& m, size_t s){
    m *= s;
    return m;
}

template<class T>
Matris<T>& operator*(size_t s, Matris<T>& m){
    m *= s;
    return m;
}

template<class T>
Matris<T>& operator-(Matris<T>& m, Matris<T>& mx){
    m -= mx;
    return m;
}

template<class T>
Matris<T>& operator-(Matris<T>& m, size_t s){
    m -= s;
    return m;
}

template<class T>
Matris<T>& operator+(Matris<T>& m, Matris<T>& mx){
    m += mx;
    return m;
}

template<class T>
Matris<T>& operator+(Matris<T>& m, size_t s){
    m += s;
    return m;
}

int main(){
    Matris<int> a{1,2,3,4};
    Matris<int> b{4,3,2,1};
    std::cout << a*b;
}


