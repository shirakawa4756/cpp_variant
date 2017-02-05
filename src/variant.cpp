#include "variant.hpp"

namespace variant {
///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Variant::Holder
//
///////////////////////////////////////////////////////////////////////////////////////////////////
class Variant::Holder
{
public:
    virtual ~Holder() = default;
    virtual bool to_bool() const = 0;
    virtual int32_t to_int() const = 0;
    virtual double to_double() const = 0;
    virtual std::string to_string() const = 0;
}; // Variant::Holder

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Variant::BoolHolder
//
///////////////////////////////////////////////////////////////////////////////////////////////////
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

private:
    bool value_;
}; // class Variant::BoolHolder

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Variant::IntHolder
//
///////////////////////////////////////////////////////////////////////////////////////////////////
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

private:
    int32_t value_;
}; // class Variant::IntHolder

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Variant::DoubleHolder
//
///////////////////////////////////////////////////////////////////////////////////////////////////
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

private:
    double value_;
}; // class Variant::DoubleHolder


///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Variant::HolderFactory
//
///////////////////////////////////////////////////////////////////////////////////////////////////
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
}; // class Variant::HolderFactory

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Variant Class
//
///////////////////////////////////////////////////////////////////////////////////////////////////
Variant::Variant(std::nullptr_t null) : holder_(null)
{
}

Variant::Variant(bool value) : holder_(HolderFactory::make_holder(value))
{
}

Variant::Variant(int32_t value) : holder_(HolderFactory::make_holder(value))
{
}

Variant::Variant(double value) : holder_(HolderFactory::make_holder(value))
{
}

bool Variant::to_bool() const
{
    return holder_->to_bool();
}

int32_t Variant::to_int() const
{
    return holder_->to_int();
}

double Variant::to_double() const
{
    return holder_->to_double();
}

std::string Variant::to_string() const
{
    return holder_->to_string();
}
} // namespace variant