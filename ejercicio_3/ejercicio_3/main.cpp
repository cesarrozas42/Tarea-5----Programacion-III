#include <iostream>
#include <memory>
#include <string>
#include <cmath>
#include <stdexcept>
#include <optional>
#include <iomanip>

class Expression {
public:
    virtual ~Expression() = default;
    virtual double evaluate(double x) const = 0;
    virtual std::shared_ptr<Expression> derivative() const = 0;
    virtual std::string toString() const = 0;

    virtual std::optional<double> constantValue() const {
        return std::nullopt;
    }
};

using Expr = std::shared_ptr<Expression>;

class Constant : public Expression {
    double value;
public:
    explicit Constant(double v) : value(v) {}

    double evaluate(double) const override {
        return value;
    }

    Expr derivative() const override {
        return std::make_shared<Constant>(0);
    }

    std::string toString() const override {
        if (std::abs(value - std::round(value)) < 1e-10)
            return std::to_string(static_cast<long long>(std::llround(value)));
        return std::to_string(value);
    }

    std::optional<double> constantValue() const override {
        return value;
    }
};

class Variable : public Expression {
public:
    double evaluate(double x) const override {
        return x;
    }

    Expr derivative() const override {
        return std::make_shared<Constant>(1);
    }

    std::string toString() const override {
        return "x";
    }
};

class BinaryOperation : public Expression {
protected:
    Expr left;
    Expr right;

    BinaryOperation(Expr l, Expr r) : left(std::move(l)), right(std::move(r)) {}
};

class Add : public BinaryOperation {
public:
    Add(Expr l, Expr r) : BinaryOperation(std::move(l), std::move(r)) {}

    double evaluate(double x) const override {
        return left->evaluate(x) + right->evaluate(x);
    }

    Expr derivative() const override {
        return std::make_shared<Add>(left->derivative(), right->derivative());
    }

    std::string toString() const override {
        return "(" + left->toString() + " + " + right->toString() + ")";
    }
};

class Multiply : public BinaryOperation {
public:
    Multiply(Expr l, Expr r) : BinaryOperation(std::move(l), std::move(r)) {}

    double evaluate(double x) const override {
        return left->evaluate(x) * right->evaluate(x);
    }

    Expr derivative() const override {

        return std::make_shared<Add>(
            std::make_shared<Multiply>(left->derivative(), right),
            std::make_shared<Multiply>(left, right->derivative())
        );
    }

    std::string toString() const override {
        return "(" + left->toString() + " * " + right->toString() + ")";
    }
};

class Divide : public BinaryOperation {
public:
    Divide(Expr l, Expr r) : BinaryOperation(std::move(l), std::move(r)) {}

    double evaluate(double x) const override {
        double denominator = right->evaluate(x);
        if (std::abs(denominator) < 1e-12)
            throw std::domain_error("Division entre cero al evaluar la expresion.");
        return left->evaluate(x) / denominator;
    }

    Expr derivative() const override;

    std::string toString() const override {
        return "(" + left->toString() + " / " + right->toString() + ")";
    }
};

class UnaryFunction : public Expression {
protected:
    Expr operand;
    explicit UnaryFunction(Expr e) : operand(std::move(e)) {}
};

class Sin : public UnaryFunction {
public:
    explicit Sin(Expr e) : UnaryFunction(std::move(e)) {}

    double evaluate(double x) const override {
        return std::sin(operand->evaluate(x));
    }

    Expr derivative() const override;

    std::string toString() const override {
        return "sin(" + operand->toString() + ")";
    }
};

class Cos : public UnaryFunction {
public:
    explicit Cos(Expr e) : UnaryFunction(std::move(e)) {}

    double evaluate(double x) const override {
        return std::cos(operand->evaluate(x));
    }

    Expr derivative() const override {

        return std::make_shared<Multiply>(
            std::make_shared<Constant>(-1),
            std::make_shared<Multiply>(
                std::make_shared<Sin>(operand),
                operand->derivative()
            )
        );
    }

    std::string toString() const override {
        return "cos(" + operand->toString() + ")";
    }
};

class Log : public UnaryFunction {
public:
    explicit Log(Expr e) : UnaryFunction(std::move(e)) {}

    double evaluate(double x) const override {
        double v = operand->evaluate(x);
        if (v <= 0)
            throw std::domain_error("Logaritmo real definido solo para valores positivos.");
        return std::log(v);
    }

    Expr derivative() const override {
        return std::make_shared<Divide>(operand->derivative(), operand);
    }

    std::string toString() const override {
        return "ln(" + operand->toString() + ")";
    }
};

class Power : public BinaryOperation {
public:
    Power(Expr base, Expr exponent)
        : BinaryOperation(std::move(base), std::move(exponent)) {}

    double evaluate(double x) const override {
        return std::pow(left->evaluate(x), right->evaluate(x));
    }

    Expr derivative() const override {

        const auto n = right->constantValue();
        if (n.has_value()) {
            return std::make_shared<Multiply>(
                std::make_shared<Multiply>(
                    std::make_shared<Constant>(*n),
                    std::make_shared<Power>(
                        left,
                        std::make_shared<Constant>(*n - 1)
                    )
                ),
                left->derivative()
            );
        }

        return std::make_shared<Multiply>(
            std::make_shared<Power>(left, right),
            std::make_shared<Add>(
                std::make_shared<Multiply>(right->derivative(), std::make_shared<Log>(left)),
                std::make_shared<Divide>(
                    std::make_shared<Multiply>(right, left->derivative()),
                    left
                )
            )
        );
    }

    std::string toString() const override {
        return "(" + left->toString() + "^" + right->toString() + ")";
    }
};

Expr Divide::derivative() const {

    return std::make_shared<Divide>(
        std::make_shared<Add>(
            std::make_shared<Multiply>(left->derivative(), right),
            std::make_shared<Multiply>(
                std::make_shared<Constant>(-1),
                std::make_shared<Multiply>(left, right->derivative())
            )
        ),
        std::make_shared<Power>(right, std::make_shared<Constant>(2))
    );
}

Expr Sin::derivative() const {

    return std::make_shared<Multiply>(
        std::make_shared<Cos>(operand),
        operand->derivative()
    );
}

int main() {
    std::cout << "========== EJERCICIO 3: COMPOSITE ==========\n";
    auto x = std::make_shared<Variable>();
    Expr expr = std::make_shared<Multiply>(
        std::make_shared<Add>(
            std::make_shared<Power>(x, std::make_shared<Constant>(2)),
            std::make_shared<Multiply>(std::make_shared<Constant>(3), x)
        ),
        std::make_shared<Sin>(x)
    );
    Expr deriv = expr->derivative();
    std::cout << std::setprecision(10);
    std::cout << "f(x)  = " << expr->toString() << '\n';
    std::cout << "f'(x) = " << deriv->toString() << '\n';
    std::cout << "f(2)  = " << expr->evaluate(2) << '\n';
    std::cout << "f'(2) = " << deriv->evaluate(2) << '\n';
    return 0;
}
