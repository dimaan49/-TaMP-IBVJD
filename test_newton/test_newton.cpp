#include "newton.hpp"
#include <QtTest>
#include <cmath>

class NewtonTest : public QObject
{
    Q_OBJECT

private slots:
    void testPositiveNumbers();
    void testZero();
    void testNegativeNumber();
    void testPrecision();
    void testCompareWithStdSqrt();
};

void NewtonTest::testPositiveNumbers()
{
    QVERIFY(std::abs(Task1::newton(4) - 2) < 1e-10);
    QVERIFY(std::abs(Task1::newton(9) - 3) < 1e-10);
    QVERIFY(std::abs(Task1::newton(16) - 4) < 1e-10);
}

void NewtonTest::testZero()
{
    QCOMPARE(Task1::newton(0), 0.0);
}

void NewtonTest::testNegativeNumber()
{
    QVERIFY_EXCEPTION_THROWN(Task1::newton(-1), std::invalid_argument);
}

void NewtonTest::testPrecision()
{
    double value = 2.0;
    double result = Task1::newton(value);
    QVERIFY(std::abs(result * result - value) < 1e-10); // Проверяет точность вычислений (что K^2 близко к иходному числу)
}

void NewtonTest::testCompareWithStdSqrt()
{
    double values[] = {0.5, 1.0, 2.0, 10.0, 100.0, 1000.0};
    for (double v : values) {
        double newtonResult = Task1::newton(v);
        double stdResult = std::sqrt(v);
        QVERIFY(std::abs(newtonResult - stdResult) < 1e-10);
    }
}

QTEST_APPLESS_MAIN(NewtonTest)
#include "test_newton.moc"
