#include "Matrix.h"
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

TEST_CASE("[Matrix] ctors") {
    Matrix<2, 2> m ({
        1, 2,
        3, 4,
    });
    REQUIRE( m.data == std::array<float, 4>({1.0f, 2.0f, 3.0f, 4.0f}) );

    Matrix<2, 2> m2 (m);
    Matrix<2, 2, int> m3 (m);

    CHECK(m2.data == m.data);
    CHECK( m3.data == std::array<int, 4>({1, 2, 3, 4}) );

    Matrix<2, 2> m4 (std::move(m2));
    CHECK(m4.data == m.data);
}

TEST_CASE("[Matrix] columns ctor") {
    Matrix<3, 1> a ({1, 2, 3});
    Matrix<3, 1> b ({4, 5, 6});
    Matrix<3, 1> c ({7, 8, 9});
    Matrix<3, 3> expected ({
        1, 4, 7,
        2, 5, 8,
        3, 6, 9,
    });
    CHECK(Matrix<3, 3>::FromColumns({a, b, c}).data == expected.data);
}

TEST_CASE("[Matrix] assignment") {
    const Matrix<2, 2> m ({
        1, 2,
        3, 4,
    });
    Matrix<2, 2> m2;
    m2 = m;
    CHECK( m2.data == m.data );
    Matrix<2, 2> m3;
    m3 = std::move(m2);
    CHECK( m3.data == m.data );
}

TEST_CASE("[Matrix] indexing") {
    const Matrix<3, 2> m ({
        1, 2,
        3, 4,
        5, 6,
    });
    CHECK(m[0] == 1);
    CHECK(m[1] == 2);
    CHECK(m[2] == 3);
    CHECK(m(0, 0) == 1);
    CHECK(m(0, 1) == 2);
    CHECK(m(1, 0) == 3);
    CHECK(m(1, 1) == 4);
    CHECK(m(2, 0) == 5);
    CHECK(m(2, 1) == 6);
}

TEST_CASE("[Matrix] iterators") {
    Matrix<4, 4> m;
    const Matrix<4, 4> c;
    CHECK(m.begin()  == m.data.begin());
    CHECK(m.end()    == m.data.end());
    CHECK(m.cbegin() == m.data.cbegin());
    CHECK(m.cend()   == m.data.cend());
    CHECK(c.begin()  == c.data.begin());
    CHECK(c.end()    == c.data.end());
}

TEST_CASE("[Matrix] fill") {
    Matrix<4, 4> m;
    m.fill(17);
    CHECK(m[10] == 17);
}

TEST_CASE("[Matrix] rOf, cOf") {
    const Matrix<3, 2> m ({
        1, 2,
        3, 4,
        5, 6,
    });
    const int i = 5;
    REQUIRE(m[i] == 6);
    CHECK(m.rOf(i) == 2);
    CHECK(m.cOf(i) == 1);
}

TEST_CASE("[Matrix] operators") {
    const Matrix<3, 2> m ({
        1, 2,
        3, 4,
        5, 6,
    });
    const Matrix<3, 2> expected ({
        2,  4,
        6,  8,
        10, 12,
    });
    CHECK( (m + m).data     == expected.data );
    CHECK( (m - m).data     == Matrix<3, 2>({0}).data );
    CHECK( (2 * m).data     == expected.data );
    CHECK( (m * 2).data     == expected.data );
    CHECK( (m * 2 / 2).data == m.data );
    CHECK( (m * -1).data    == (-m).data );
}

TEST_CASE("[Matrix] multiplication") {
    const Matrix<3, 2> a ({
        1, 2,
        3, 4,
        5, 6,
    });
    const Matrix<2, 5> b ({
        10, 11, 12, 13, 14,
        15, 16, 17, 18, 19,
    });
    const Matrix<3, 5> c ({
        40, 43, 46, 49, 52,
        90, 97, 104, 111, 118,
        140, 151, 162, 173, 184,
    });
    CHECK( (a * b).data == c.data );
}

// TEST_CASE("[Matrix] comparison") {
//     Matrix<3, 2> m ({
//         1, 2,
//         3, 4,
//         5, 6,
//     });
//     CHECK(m == m);
//     CHECK(m != -m);
// }

TEST_CASE("[Matrix] submatrix") {
    const Matrix<4, 4> m ({
        1,  2,  3,  4,
        5,  6,  7,  8,
        9,  10, 11, 12,
        13, 14, 15, 16,
    });
    const Matrix<3, 2> expected ({
        1,  2,
        5,  6,
        9,  10,
    });
    CHECK(m.Submatrix<4, 4>().data == m.data);
    CHECK(m.Submatrix<3, 2>().data == expected.data);
}

TEST_CASE("[Matrix] resized") {
    const Matrix<3, 2> m ({
        1, 2,
        3, 4,
        5, 6,
    });
    const Matrix<4, 4> expected ({
        1, 2, 0, 0,
        3, 4, 0, 0,
        5, 6, 0, 0,
        0, 0, 0, 0,
    });
    CHECK(m.Resized<4, 4>().data == expected.data);
    CHECK(m.Resized<3, 2>().data == m.data);
}

TEST_CASE("[Matrix] row, column") {
    const Matrix<4, 3> m ({
        1,  2,  3,
        4,  5,  6,
        7,  8,  9,
        10, 11, 12,
    });
    CHECK( m.Row(2).data == std::array<float, 3>({7, 8, 9}) );
    CHECK( m.Column(1).data == std::array<float, 4>({2, 5, 8, 11}) );
}

TEST_CASE("[Matrix] transpose") {
    const Matrix<4, 4> m ({
        1,  2,  3,  4,
        5,  6,  7,  8,
        9,  10, 11, 12,
        13, 14, 15, 16,
    });
    const Matrix<4, 4> expected ({
        1, 5, 9,  13,
        2, 6, 10, 14,
        3, 7, 11, 15,
        4, 8, 12, 16,
    });
    CHECK(m.Transposed().data == expected.data);
    CHECK(m.Transposed().Transposed().data == m.data);
}

TEST_CASE("[Matrix] trace") {
    const Matrix<4, 4> m ({
        1,  2,  3,  4,
        5,  6,  7,  8,
        9,  10, 11, 12,
        13, 14, 15, 16,
    });
    CHECK(m.Trace() == 1 + 6 + 11 + 16);
}

TEST_CASE("[Matrix] inverse") {
    const auto abs_cmp = [](auto a, auto b) {
        return std::abs(a) < std::abs(b);
    };
    {
        const Matrix<3, 3> m ({
             7.0,  2.0,  1.0,
             0.0,  4.0, -1.0,
            -3.0,  4.0, -2.0,
        });
        const Matrix<3, 3> expected ({
             0.4, -0.8,  0.6,
            -0.3,  1.1, -0.7,
            -1.2,  3.4, -2.8,
        });
        const auto res = m.Inverse() - expected;
        CHECK( *std::max_element(res.begin(), res.end(), abs_cmp) < 0.000001f );
    }
    {
        const Matrix<3, 3> m ({
             0.0,  4.0, -1.0,
             7.0,  2.0,  1.0,
            -3.0,  4.0, -2.0,
        });
        const Matrix<3, 3> expected ({
            -0.8,  0.4,  0.6,
             1.1, -0.3, -0.7,
             3.4, -1.2, -2.8,
        });
        const auto res = m.Inverse() - expected;
        CHECK( *std::max_element(res.begin(), res.end(), abs_cmp) < 0.000001f );
    }
    {
        Matrix<2, 3> m ({
             0.0,  0.0,  0.0,
             7.0,  2.0,  1.0,
        });
        const Matrix<2, 3> expected ({
             0.0,  0.0,  0.0,
             7.0,  2.0,  1.0,
        });
        m.Gauss();
        const auto res = m - expected;
        CHECK( *std::max_element(res.begin(), res.end(), abs_cmp) < 0.000001f );
    }
    {
        Matrix<3, 4> m ({
            1,  3,  1,  9,
            1,  1, -1,  1,
            3, 11,  5, 35,
        });
        const Matrix<3, 4> expected ({
            1,  0, -2, -3,
            0,  1,  1,  4,
            0,  0,  0,  0,
        });
        m.Gauss();
        const auto res = m - expected;
        CHECK( *std::max_element(res.begin(), res.end(), abs_cmp) < 0.000001f );
    }
    {
        Matrix<3, 4> m ({
            0,  3,  1,  9,
            0,  1, -1,  1,
            0, 11,  5, 35,
        });
        const Matrix<3, 4> expected ({
            0,  1,  0, 2.5,
            0,  0,  1, 1.5,
            0,  0,  0,   0,
        });
        m.Gauss();
        const auto res = m - expected;
        CHECK( *std::max_element(res.begin(), res.end(), abs_cmp) < 0.000001f );
    }
}

TEST_CASE("[Matrix] static zero") {
    const auto m = Matrix<5, 6>::Zero();
    const auto isZero = [](auto e){ return e == 0; };
    CHECK( std::all_of(m.begin(), m.end(), isZero) );
}

TEST_CASE("[Matrix] identity") {
    Matrix<3, 3> m;
    m = Matrix<3, 3>::Identity();
    Matrix<3, 3> expected ({
        1, 0, 0,
        0, 1, 0,
        0, 0, 1,
    });
    CHECK(m[0] == 1);
    m[0] = 1;
    CHECK(m.data == expected.data);
}
