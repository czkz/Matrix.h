#pragma once
#include <iomanip>

template <size_t _rows, size_t _cols, typename T = float>
class Matrix {
    using real_t = typename std::conditional<
        std::is_floating_point<T>::value && (sizeof(T) >= sizeof(float)),
        T, float>::type;
    static_assert(_rows != 0, "Can't create matrix with 0 rows");
    static_assert(_cols != 0, "Can't create matrix with 0 columns");
public:
    static constexpr size_t rows = _rows;
    static constexpr size_t cols = _cols;
    static constexpr size_t n = rows * cols;
    std::array<T, rows* cols> data;
public:

    constexpr Matrix() noexcept : data() {}
    // std::array is not movable, so pass by const reference
    constexpr Matrix(const std::array<T, n>& data) noexcept : data(data) {}
    constexpr Matrix(const Matrix<rows, cols, T>& other) noexcept : data(other.data) {}
    template <typename _T>
    explicit constexpr Matrix(const Matrix<rows, cols, _T>& other) noexcept {
        for(size_t i = 0; i < n; i++) {
            data[i] = other[i];
        }
    }
    constexpr Matrix(Matrix<rows, cols, T>&& other) noexcept : data(std::move(other.data)) {}
    constexpr Matrix<rows, cols, T>& operator=(const Matrix<rows, cols, T>& other)& noexcept { data = other.data; return *this; }
    constexpr Matrix<rows, cols, T>& operator=(Matrix<rows, cols, T>&& other)& noexcept { data = std::move(other.data); return *this; }

    [[nodiscard]] static constexpr Matrix<rows, cols, T> FromColumns(const std::array<Matrix<rows, 1, T>, cols>& columns) noexcept {
        Matrix<rows, cols, T> ret;
        for(size_t j = 0; j < cols; j++) {
            for(size_t i = 0; i < rows; i++) {
                ret(i, j) = columns[j][i];
            }
        }
        return ret;
    }
    [[nodiscard]] static constexpr Matrix<rows, cols, T> Zero() noexcept {
        return Matrix({0});
    }
    [[nodiscard]] static constexpr Matrix<rows, cols, T> Identity() noexcept {
        static_assert(rows == cols, "Identity matrix must be square");
        auto ret = Matrix<rows, cols, T>::Zero();
        for (size_t i = 0; i < rows; i++) {
            ret.data[(rows + 1)*i] = 1;
        }
        return ret;
    }

    [[nodiscard]] constexpr T& operator[](int i) noexcept { return data[i]; }
    [[nodiscard]] constexpr T operator[](int i) const noexcept { return data[i]; }
    [[nodiscard]] constexpr T& operator()(int row, int col) noexcept { return data[row * cols + col]; }
    [[nodiscard]] constexpr T operator()(int row, int col) const noexcept { return data[row * cols + col]; }

    [[nodiscard]] constexpr auto begin() noexcept { return data.begin(); }
    [[nodiscard]] constexpr auto end() noexcept { return data.end(); }
    [[nodiscard]] constexpr auto begin() const noexcept { return data.begin(); }
    [[nodiscard]] constexpr auto end() const noexcept { return data.end(); }
    [[nodiscard]] constexpr auto cbegin() const noexcept { return data.cbegin(); }
    [[nodiscard]] constexpr auto cend() const noexcept { return data.cend(); }

    constexpr void fill(T v) noexcept { data.fill(v); }

    [[nodiscard]] inline constexpr size_t rOf(size_t i) const noexcept { return i / cols; }
    [[nodiscard]] inline constexpr size_t cOf(size_t i) const noexcept { return i % cols; }

    template <typename _T>
    constexpr Matrix<rows, cols, T>& operator+=(const Matrix<rows, cols, _T>& other)& noexcept {
        for (size_t i = 0; i < n; i++) {
            data[i] += other.data[i];
        }
        return *this;
    }
    template <typename _T>
    constexpr Matrix<rows, cols, T>& operator-=(const Matrix<rows, cols, _T>& other)& noexcept {
        for (size_t i = 0; i < n; i++) {
            data[i] -= other.data[i];
        }
        return *this;
    }
    constexpr Matrix<rows, cols, T>& operator*=(const T other)& noexcept {
        for (size_t i = 0; i < n; i++) {
            data[i] *= other;
        }
        return *this;
    }
    constexpr Matrix<rows, cols, T>& operator/=(const T other)& noexcept {
        for (size_t i = 0; i < n; i++) {
            data[i] /= other;
        }
        return *this;
    }

    [[nodiscard]] constexpr Matrix<rows, cols, T> operator-() const noexcept {
        Matrix<rows, cols, T> ret;
        for (size_t i = 0; i < n; i++) {
            ret.data[i] = -this->data[i];
        }
        return ret;
    }

    // Explicitly delete multiplication of matricies with incompatible dimensions
    template <size_t rows2, size_t cols2, typename _T>
    [[nodiscard]] constexpr Matrix<rows, cols2, T> operator*(const Matrix<rows2, cols2, _T>& other) const noexcept = delete;

    template <size_t cols2, typename _T>
    [[nodiscard]] constexpr Matrix<rows, cols2, T> operator*(const Matrix<cols, cols2, _T>& other) const noexcept {
        Matrix<rows, cols2, T> ret;
        for (size_t i = 0; i < ret.n; i++) {
            const int cRow = ret.rOf(i);
            const int cCol = ret.cOf(i);
            T sum = 0;
            for (size_t j = 0; j < cols; j++) {
                sum += (*this)(cRow, j) * other(j, cCol);
            }
            ret[i] = sum;
        }
        return ret;
    }

    friend constexpr Matrix<rows, cols, T> operator+(Matrix<rows, cols, T> a, const Matrix<rows, cols, T>& b) noexcept { return a += b; }
    friend constexpr Matrix<rows, cols, T> operator-(Matrix<rows, cols, T> a, const Matrix<rows, cols, T>& b) noexcept { return a -= b; }
    friend constexpr Matrix<rows, cols, T> operator*(Matrix<rows, cols, T> a, const T& b) noexcept { return a *= b; }
    friend constexpr Matrix<rows, cols, T> operator*(const T& b, Matrix<rows, cols, T> a) noexcept { return a *= b; }
    friend constexpr Matrix<rows, cols, T> operator/(Matrix<rows, cols, T> a, const T& b) noexcept { return a /= b; }


    // operator== is often meaningless for floats (0.1 + 0.2 != 0.3),
    // that's why it's disabled.
    // Using `a.data == b.data` instead signifies that one checks
    // for equal data and not for equivalent matrices.
    template <typename _T> constexpr bool operator==(const Matrix<rows, cols, _T>& other) const noexcept = delete;

    // template <typename _T>
    // [[nodiscard]] constexpr bool operator==(const Matrix<rows, cols, _T>& other) const noexcept {
    //     for (size_t i = 0; i < n; i++) {
    //         if (this->data[i] != other.data[i]) {
    //             return false;
    //         }
    //     }
    //     return true;
    // }

    template <size_t rows2, size_t cols2>
    [[nodiscard]] constexpr Matrix<rows2, cols2, T> Submatrix() const noexcept {
        static_assert(rows2 <= rows, "Submatrix must be smaller than the original matrix");
        static_assert(cols2 <= cols, "Submatrix must be smaller than the original matrix");
        Matrix<rows2, cols2, T> ret;
        for (size_t i = 0; i < rows2; i++) {
            for (size_t j = 0; j < cols2; j++) {
                ret(i, j) = (*this)(i, j);
            }
        }
        return ret;
    }

    template <size_t rows2, size_t cols2>
    [[nodiscard]] constexpr Matrix<rows2, cols2, T> Resized() const noexcept {
        auto ret = Matrix<rows2, cols2, T>::Zero();
        const auto r = std::min(rows, rows2);
        const auto c = std::min(cols, cols2);
        for (size_t i = 0; i < r; i++) {
            for (size_t j = 0; j < c; j++) {
                ret(i, j) = (*this)(i, j);
            }
        }
        return ret;
    }

    [[nodiscard]] constexpr Matrix<1, cols, T> Row(size_t row) const noexcept {
        Matrix<1, cols, T> ret;
        for (size_t i = 0; i < cols; i++) {
            ret[i] = (*this)(row, i);
        }
        return ret;
    }

    [[nodiscard]] constexpr Matrix<rows, 1, T> Column(size_t col) const noexcept {
        Matrix<rows, 1, T> ret;
        for (size_t i = 0; i < rows; i++) {
            ret[i] = (*this)(i, col);
        }
        return ret;
    }

    [[nodiscard]] constexpr Matrix<rows, cols, T> Transposed() const noexcept {
        Matrix<cols, rows, T> ret;
        for (size_t i = 0; i < rows; i++) {
            for (size_t j = 0; j < cols; j++) {
                ret(j, i) = (*this)(i, j);
            }
        }
        return ret;
    }

    [[nodiscard]] constexpr real_t Trace() const noexcept {
        static_assert(rows == cols, "Trace of a non-square matrix is undefined");
        real_t sum = 0;
        for (size_t i = 0; i < rows; i++) {
            sum += data[(rows + 1) * i];
        }
        return sum;
    }

    [[nodiscard]] constexpr Matrix Inverse() const noexcept {
        static_assert(rows == cols, "Can't calculate inverse of a non-square matrix");
        Matrix<rows, cols * 2, T> m ({0});
        for (size_t i = 0; i < rows; i++) {
            for (size_t j = 0; j < cols; j++) {
                m(i, j) = (*this)(i, j);
            }
            m(i, i + cols) = 1;
        }
        m.Gauss();
        Matrix<rows, cols, T> ret;
        for (size_t i = 0; i < rows; i++) {
            for (size_t j = 0; j < cols; j++) {
                ret(i, j) = m(i, j + cols);
            }
        }
        return ret;
    }

    constexpr void Gauss() noexcept {
        Matrix<rows, cols, T>& m = *this;
        // For each row, subtract it from all other rows
        for (size_t current_row = 0; current_row < m.rows; current_row++) {

            // Find the first non-zero element in current row
            size_t cur_lead = m.cols;
            for (size_t j = 0; j < m.cols; j++) {
                if (m(current_row, j) != 0) {
                    cur_lead = j;
                    break;
                }
            }

            // Make sure we're always working with the smallest lead.
            // Lead can't be smaller then row index on row 0
            // Lead can't be smaller then row index after row 0,
            // because row 0 pass removes lead 0 from all other rows,
            // so if lead == row than there's no smaller lead
            if (cur_lead != current_row) {
                size_t min_lead_row = current_row;
                size_t min_lead = cur_lead;
                // Find a smaller lead after current row
                for (size_t ii = current_row + 1; ii < rows; ii++) {
                    size_t lead = m.rows;
                    // Find this row's lead
                    // We don't care about it if it's same or bigger,
                    // only if it's smaller
                    for (size_t j = 0; j < cur_lead; j++) {
                        if (m(ii, j) != 0) {
                            lead = j;
                            break;
                        }
                    }
                    if (lead < min_lead) {
                        min_lead_row = ii;
                        min_lead = lead;
                    }
                }

                // Swap rows
                if (min_lead_row != current_row) {
                    for (size_t j = min_lead; j < cols; j++) {
                        // Allow non-std overloads of swap() for T
                        using namespace std;
                        swap(m(current_row, j), m(min_lead_row, j));
                    }
                    cur_lead = min_lead;
                }
            }

            // Skip the row if it's empty
            if (cur_lead == size_t(m.cols)) { break; }


            // Divide current_row by lead for it to become 1
            // Optimization: manually set lead to 1, ignore cells before lead
            {
                T lead_val = m(current_row, cur_lead);
                if (lead_val != 1) {
                    m(current_row, cur_lead) = 1;
                    for (size_t j = cur_lead + 1; j < m.cols; j++) {
                        m(current_row, j) /= lead_val;
                    }
                }
            }

            // Zero out current lead's column on all rows except current
            // by subtracting `current_row * f` from it
            for (size_t other_row = 0; other_row < m.rows; other_row++) {
                if (other_row == current_row) { continue; }
                // Skip if other lead is already zero
                if (m(other_row, cur_lead) == 0) { continue; }
                // Calculate f so that lead becomes zero
                real_t f = 1.0 / m(current_row, cur_lead) * m(other_row, cur_lead);
                // Subtract current_row from other_row
                for (size_t j = 0; j < m.cols; j++) {
                    m(other_row, j) -= m(current_row, j) * f;
                }
            }

        }
    }

    friend std::ostream& operator<<(std::ostream& os, const Matrix<rows, cols, T>& m) {
        static const auto len = [](const T a) {
            std::stringstream ss;
            ss << a;
            return ss.str().size();
        };

        if (rows * cols > 0) {

            auto mlen = len(m.data[0]);
            for (const auto& v : m.data) {
                mlen = std::max(mlen, len(v));
            }
            for (size_t row = 0; row < rows; row++) {
                os << '|';
                for (size_t col = 0; col < cols - 1; col++) {
                    os << std::setw(mlen) << m(row, col) << ' ';
                }
                os << std::setw(mlen) << m(row, cols - 1) << '|' << std::endl;
            }
        }
        return os;
    }
};
