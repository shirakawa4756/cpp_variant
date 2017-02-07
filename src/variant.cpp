//===------------------ Instruction class definition ------------*- C++ -*-===//
//
// bool, int, double, string を許容する多用型
//
// 本ファイルは，2条項BSDライセンスに従って配布されます．
// 詳細は、LICENSE.TXTを参照してください。
//
//===----------------------------------------------------------------------===//
///
/// \file
/// このクラスには，多用型クラス(variant)のクラス実装が含まれます．
///
//===----------------------------------------------------------------------===//
#include "variant.hpp"

#include <algorithm>
#include <locale>

namespace variant {
///////////////////////////////////////////////////////////////////////////////
//
// Variant::Holder
//
///////////////////////////////////////////////////////////////////////////////
class Variant::Holder
{
public:
    virtual ~Holder() = default;
    virtual bool to_bool() const = 0;
    virtual int32_t to_int() const = 0;
    virtual double to_double() const = 0;
    virtual std::string to_string() const = 0;
    virtual std::ostream& writeOstream(std::ostream& os) const = 0;
}; // Variant::Holder

///////////////////////////////////////////////////////////////////////////////
//
// Variant::BoolHolder
//
///////////////////////////////////////////////////////////////////////////////
class Variant::BoolHolder : public Variant::Holder
{
public:
    BoolHolder() = default;
    BoolHolder(const BoolHolder &rhs) = default;
    BoolHolder& operator=(const BoolHolder &rhs) = default;
    BoolHolder(bool value) : value_(value) {}

    virtual bool to_bool() const
    {
        return value_;
    }

    virtual int32_t to_int() const
    {
        return value_;
    }

    virtual double to_double() const
    {
        return value_;
    }

    virtual std::string to_string() const
    {
        return std::to_string(value_);
    }

    virtual std::ostream& writeOstream(std::ostream& os) const
    {
        return os << value_;
    }

private:
    bool value_;
}; // class Variant::BoolHolder

///////////////////////////////////////////////////////////////////////////////
//
// Variant::IntHolder
//
///////////////////////////////////////////////////////////////////////////////
class Variant::IntHolder : public Variant::Holder
{
public:
    IntHolder() = default;
    IntHolder(const IntHolder &rhs) = default;
    IntHolder& operator=(const IntHolder &rhs) = default;

    IntHolder(int32_t value) : value_(value) {}

    virtual bool to_bool() const
    {
        return static_cast<bool>(value_ != 0);
    }

    virtual int32_t to_int() const
    {
        return value_;
    }
    virtual double to_double() const
    {
        return static_cast<double>(value_);
    }

    virtual std::string to_string() const
    {
        return std::to_string(value_);
    }

    virtual std::ostream& writeOstream(std::ostream& os) const
    {
        return os << value_;
    }
private:
    int32_t value_;
}; // class Variant::IntHolder

///////////////////////////////////////////////////////////////////////////////
//
// Variant::DoubleHolder
//
///////////////////////////////////////////////////////////////////////////////
class Variant::DoubleHolder : public Variant::Holder
{
public:
    DoubleHolder() = default;
    DoubleHolder(const DoubleHolder &rhs) = default;
    DoubleHolder& operator=(const DoubleHolder &rhs) = default;

    DoubleHolder(double value) : value_(value) {}

    virtual bool to_bool() const
    {
        return static_cast<bool>(value_ != 0);
    }

    virtual int32_t to_int() const
    {
        return static_cast<int32_t>(value_);
    }

    virtual double to_double() const
    {
        return value_;
    }

    virtual std::string to_string() const
    {
        return std::to_string(value_);
    }

    virtual std::ostream& writeOstream(std::ostream& os) const
    {
        return os << value_;
    }
private:
    double value_;
}; // class Variant::DoubleHolder

///////////////////////////////////////////////////////////////////////////////
//
// Variant::StringHolder
//
///////////////////////////////////////////////////////////////////////////////
class Variant::StringHolder : public Variant::Holder
{
public:
    StringHolder() = default;

    StringHolder(const StringHolder &rhs) = default;

    StringHolder& operator=(const StringHolder &rhs) = default;

    StringHolder(const std::string &s) : str_(s) {}

    StringHolder(const char *s) : str_(s) {}

    virtual bool to_bool() const
    {
        const std::string lower = toLower(str_);
        if (lower == "false") return false;
        if (lower == "true")  return true;
        return (to_int() != 0);

    }

    virtual int32_t to_int() const
    {
        return std::stoi(str_);
    }

    virtual double to_double() const
    {
        return std::stod(str_);
    }

    virtual std::string to_string() const
    {
        return str_;
    }

    virtual std::ostream& writeOstream(std::ostream& os) const
    {
        return os << str_;
    }
private:
    std::string toLower(const std::string &s) const
    {
        std::string d = s;
        std::transform(d.begin(), d.end(), d.begin(), [](char c) {
            return std::tolower(c, std::locale());
        });
        return d;
    }

    std::string str_;
}; // class Variant::DoubleHolder

///////////////////////////////////////////////////////////////////////////////
//
// Variant::HolderFactory
//
///////////////////////////////////////////////////////////////////////////////
class Variant::HolderFactory
{
public:
    static std::shared_ptr<Holder> make_holder(bool value)
    {
        return std::make_shared<BoolHolder>(value);
    }

    static std::shared_ptr<Holder> make_holder(int32_t value)
    {
        return std::make_shared<IntHolder>(value);
    }

    static std::shared_ptr<Holder> make_holder(double value)
    {
        return std::make_shared<DoubleHolder>(value);
    }

    static std::shared_ptr<Holder> make_holder(const char *s)
    {
        return std::make_shared<StringHolder>(s);
    }

    static std::shared_ptr<Holder> make_holder(const std::string &s)
    {
        return std::make_shared<StringHolder>(s);
    }
}; // class Variant::HolderFactory

///////////////////////////////////////////////////////////////////////////////
//
// Variant Class
//
///////////////////////////////////////////////////////////////////////////////
Variant::Variant(bool value)
  : holder_(HolderFactory::make_holder(value))
{
}

Variant::Variant(int32_t value)
  : holder_(HolderFactory::make_holder(value))
{
}

Variant::Variant(double value)
  : holder_(HolderFactory::make_holder(value))
{
}

Variant::Variant(char *s)
  : holder_(HolderFactory::make_holder(s))
{
}

Variant::Variant(const char *s)
  : holder_(HolderFactory::make_holder(s))
{
}

Variant::Variant(const std::string &s)
  : holder_(HolderFactory::make_holder(s))
{
}

bool Variant::toBool() const
{
    return holder_->to_bool();
}

int32_t Variant::toInt() const
{
    return holder_->to_int();
}

double Variant::toDouble() const
{
    return holder_->to_double();
}

std::string Variant::toString() const
{
    return holder_->to_string();
}

///////////////////////////////////////////////////////////////////////////////
//
// operator<< function( of variant::Variant)
//
///////////////////////////////////////////////////////////////////////////////
std::ostream& operator<<(std::ostream& os, const Variant &var)
{
    return var.holder_->writeOstream(os);
}
} // namespace variant