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
#include "variant/variant.hpp"

#if defined(_WIN32) || defined(_WIN64) || defined(_MSC_VER)
    #include <Windows.h>
#endif

#include <cassert>
#include <cmath>
#include <algorithm>
#include <codecvt>
#include <locale>
#include <sstream>
#include <vector>

namespace variant {
#if defined(_WIN32) || defined(_WIN64) || defined(_MSC_VER)
std::wstring to_wstring(const std::string &s)
{
    std::vector<wchar_t> wcs(s.size() + 1);
    size_t               len = 0;
    mbstowcs_s(&len, &wcs[0], wcs.size(), s.c_str(), _TRUNCATE);
    return std::wstring(wcs.cbegin(), wcs.cend());
}
#else
std::wstring to_wstring(const std::string &s)
{
    std::mbstate_t state = std::mbstate_t();
    const char     *mbs  = s.c_str();
    const int      len   = 1 + std::mbsrtowcs(nullptr, &mbs, 0, &state);
    std::vector<wchar_t> wcs(len);
    std::mbsrtowcs(&wcs[0], &mbs, len, &state);
    return std::wstring(wcs.cbegin(), wcs.cend());
}
#endif

/// 文字列を int64_t へ変換できるか試します.
///
/// \param s 文字列
/// \param[out] result 変換された数値
/// \return true なら変換に成功した
///
/// 文字列(s) を strtoll を用いて int64_t に変換します．
/// 文字列全体が int64_t を許容する以外は不成功と見なし
/// false を返します．
///
/// 具体例
/// \code
///     std::string list[] = {"0", "1", "+10", "-10",
///                           "100$", "-100$", "+1000$",
///                           "$1000$"};
///
///     for (const auto &s : list) {
///         int64_t result = -1;
///         if (tryStringToInt64(s, result)) {
///             std::cout << "Accept: " << s << " -> " << result << std::endl;
///         } else {
///             std::cout << "Deny: " << s << "->" result << std::endl;
///         }
///     }
/// \endcode
bool tryStringToInt64(const std::string &s, int64_t &result)
{
    // 文字列から整数へ変換するときの10進指定
    //
    // 10進数で変換することを示します.
    static const int32_t CONVERTED_BASE_DECIMAL = 10;

    // エラー番号の初期化
    //
    // errno は大域変数のため，初期化します．
    // 0 は成功を示すのでそれ以外で初期化します．
    errno = 1;

    char *convetedEnd = NULL;
    const int64_t dst = std::strtoll(s.c_str(), &convetedEnd,
                                     CONVERTED_BASE_DECIMAL);

    if (errno == ERANGE || *convetedEnd != '\0') {
        return false;
    }

    result = dst;
    return true;
}

/// 文字列を double へ変換できるか試します.
///
/// \param s 文字列
/// \param[out] result 変換された数値
/// \return true なら変換に成功した
///
/// 文字列(s) を strtoll を用いて double に変換します．
/// 文字列全体が double を許容する以外は不成功と見なし
/// false を返します．
///
/// 具体例
/// \code
///     std::string list[] = { "0", "1", "+10", "-10", "0.0", "1.3", 
///                            "-1.1", "+10.5", "-10.25", "-1E10",
///                            "100$", "-100$", "+1000$", "$1000$" };
///
///     for (const auto &s : list) {
///         int64_t result = -1;
///         if (tryStringToInt64(s, result)) {
///             std::cout << "Accept: " << s << " -> " << result << std::endl;
///         } else {
///             std::cout << "Deny: " << s << "->" result << std::endl;
///         }
///     }
/// \endcode
bool tryStringToDouble(const std::string &s, double &result)
{
    // エラー番号の初期化
    //
    // errno は大域変数のため，初期化します．
    // 0 は成功を示すのでそれ以外で初期化します．
    errno = 0;

    char *convetedEnd = NULL;
    const double dst = std::strtod(s.c_str(), &convetedEnd);

    if (errno == ERANGE || *convetedEnd != '\0') {
        return false;
    }

    result = dst;
    return true;
}

/// Variant::Types を分かりやすく文字列で示します.
///
/// @param t 型情報(Varaint::Types)
/// @return 型情報のメッセージ
///
/// Types の整数値を文字列メッセージへ変換します.
std::string variantTypesMessage(const Variant::Types &t)
{
    switch (t)
    {
    case Variant::Types::TYPE_BOOL:
        return "bool";

    case Variant::Types::TYPE_INT:
        return "int64_t";

    case Variant::Types::TYPE_DOUBLE:
        return "double";

    case Variant::Types::TYPE_STRING:
        return "std::string";

    default:
        std::stringstream ss;
        ss << "Invaild Argument(Variant::type). "
           << "Function: " << __FUNCTION__
           << "(" << __FILE__ << ":" << __LINE__ << ")";
        throw std::runtime_error(ss.str());
    }
}

/// bat_variant_cast と等価なメッセージを送出するアサート.
///
/// \param var       変数値(文字列，整数，実数，真理値が期待されます)
/// \param from_type 変換元の型情報を示します
/// \param to_type   変換先の型情報を示します
///
/// 関数，ファイル名，行数を展開して例外を送出します.
#define ASSERT_BAT_VARIANT_CAST(var, from_type, to_type)                    \
    {                                                                       \
        assert(false && "Bat Variant Cast.");                               \
    \
}

/// bat_variant_cast を送出するマクロ.
///
/// \param var       変数値(文字列，整数，実数，真理値が期待されます)
/// \param from_type 変換元の型情報を示します
/// \param to_type   変換先の型情報を示します
///
/// 関数，ファイル名，行数を展開して例外を送出します.
#define THROW_BAT_VARIANT_CAST(var, from_type, to_type)                     \
    {                                                                       \
        std::stringstream ss;                                               \
        ss << var;                                                          \
        throw variant::bat_variant_cast(ss.str(), from_type, to_type,       \
                                        __FUNCTION__, __FILE__, __LINE__);  \
    \
}

#ifndef NDEBUG
#define BAT_VARIANT_CAST_ERROR ASSERT_BAT_VARIANT_CAST
#else
#define BAT_VARIANT_CAST_ERROR THROW_BAT_VARIANT_CAST
#endif

///////////////////////////////////////////////////////////////////////////////
//
// Variant::Holder
//
///////////////////////////////////////////////////////////////////////////////
class Variant::Holder
{
public:
    virtual ~Holder() = default;
    virtual bool toBool() const = 0;
    virtual int64_t toInt() const = 0;
    virtual double toDouble() const = 0;
    virtual std::string toString() const = 0;

    virtual Types type() const = 0;

    virtual bool canCastToBool() const = 0;
    virtual bool canCastToInt() const = 0;
    virtual bool canCastToDouble() const = 0;

    virtual std::ostream& writeOstream(std::ostream &os) const = 0;
    virtual std::wostream& writeWOstream(std::wostream &wos) const = 0;
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

    virtual bool toBool() const
    {
        return value_;
    }

    virtual int64_t toInt() const
    {
        return value_;
    }

    virtual double toDouble() const
    {
        return value_;
    }

    virtual Types type() const
    {
        return Types::TYPE_BOOL;
    }

    virtual std::string toString() const
    {
        return std::to_string(value_);
    }

    virtual bool canCastToBool() const
    {
        return true;
    }

    virtual bool canCastToInt() const
    {
        return true;
    }

    virtual bool canCastToDouble() const
    {
        return true;
    }

    virtual std::ostream& writeOstream(std::ostream& os) const
    {
        return os << value_;
    }

    virtual std::wostream& writeWOstream(std::wostream &wos) const
    {
        return wos << value_;
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
    IntHolder(int64_t value) : value_(value) {}

    virtual bool toBool() const
    {
        return static_cast<bool>(value_ != 0);
    }

    virtual int64_t toInt() const
    {
        return value_;
    }
    virtual double toDouble() const
    {
        return static_cast<double>(value_);
    }

    virtual std::string toString() const
    {
        return std::to_string(value_);
    }

    virtual Types type() const
    {
        return Types::TYPE_INT;
    }

    virtual bool canCastToBool() const
    {
        return true;
    }

    virtual bool canCastToInt() const
    {
        return true;
    }

    virtual bool canCastToDouble() const
    {
        return true;
    }

    virtual std::ostream& writeOstream(std::ostream& os) const
    {
        return os << value_;
    }

    virtual std::wostream& writeWOstream(std::wostream &wos) const
    {
        return wos << value_;
    }
private:
    int64_t value_;
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

    virtual bool toBool() const
    {
        return static_cast<bool>(fabs(value_) 
                <= std::numeric_limits<double>::epsilon());
    }

    virtual int64_t toInt() const
    {
        return static_cast<int32_t>(value_);
    }

    virtual double toDouble() const
    {
        return value_;
    }

    virtual Types type() const
    {
        return Types::TYPE_DOUBLE;
    }

    virtual bool canCastToBool() const
    {
        return true;
    }

    virtual bool canCastToInt() const
    {
        return true;
    }

    virtual bool canCastToDouble() const
    {
        return true;
    }

    virtual std::string toString() const
    {
        return std::to_string(value_);
    }

    virtual std::ostream& writeOstream(std::ostream& os) const
    {
        return os << value_;
    }

    virtual std::wostream& writeWOstream(std::wostream &wos) const
    {
        return wos << value_;
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

    virtual bool toBool() const
    {
        const std::string lower = toLower(str_);
        if (lower == "false") return false;
        if (lower == "true")  return true;
        return (toDouble() <= std::numeric_limits<double>::epsilon());
    }

    virtual int64_t toInt() const
    {
        int64_t result = 0;
        if (!tryStringToInt64(str_, result)) {
            BAT_VARIANT_CAST_ERROR(str_, type(), Types::TYPE_INT);
        }
        return result;
    }

    virtual double toDouble() const
    {
        double result = 0;
        if (!tryStringToDouble(str_, result)) {
            BAT_VARIANT_CAST_ERROR(str_, type(), Types::TYPE_DOUBLE);
        }
        return result;
    }

    virtual std::string toString() const
    {
        return str_;
    }

    virtual Types type() const
    {
        return Types::TYPE_STRING;
    }

    virtual bool canCastToBool() const
    {
        if (toLower(str_) == "true") return true;
        if (toLower(str_) == "false") return true;
        return canCastToDouble();
    }

    virtual bool canCastToInt() const
    {
        int64_t result = 0;
        return tryStringToInt64(str_, result);

    }

    virtual bool canCastToDouble() const
    {
        double result = 0.0;
        return tryStringToDouble(str_, result);
    }

    virtual std::ostream& writeOstream(std::ostream& os) const
    {
        return os << str_.c_str();
    }

    virtual std::wostream& writeWOstream(std::wostream &wos) const
    {
        const std::wstring &ws = to_wstring(str_);
        return wos << ws.c_str();
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

    static std::shared_ptr<Holder> make_holder(int64_t value)
    {
        return std::make_shared<IntHolder>(value);
    }

    static std::shared_ptr<Holder> make_holder(double value)
    {
        return std::make_shared<DoubleHolder>(value);
    }

    static std::shared_ptr<Holder> make_holder(const char *s)
    {
        if (s == nullptr) return std::make_shared<StringHolder>("");
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
Variant::Variant()
  : holder_(HolderFactory::make_holder(0))
{
}

Variant::Variant(bool value)
  : holder_(HolderFactory::make_holder(value))
{
}

Variant::Variant(int32_t value)
  : holder_(HolderFactory::make_holder(value))
{
}

Variant::Variant(int64_t value)
    : holder_(HolderFactory::make_holder(value))
{
}

Variant::Variant(double value)
  : holder_(HolderFactory::make_holder(value))
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
    return holder_->toBool();
}

int32_t Variant::toInt() const
{
    return static_cast<int32_t>(holder_->toInt());
}

int64_t Variant::toInt64() const
{
    return holder_->toInt();
}

double Variant::toDouble() const
{
    return holder_->toDouble();
}

std::string Variant::toString() const
{
    return holder_->toString();
}

std::wstring Variant::toWString() const
{
    return to_wstring(holder_->toString());
}

Variant::Types Variant::type() const
{
    return holder_->type();
}

bool Variant::isBool() const
{
    return (holder_->type() == Types::TYPE_BOOL);
}

bool Variant::isInt() const
{
    return (holder_->type() == Types::TYPE_INT);
}

bool Variant::isDouble() const
{
    return (holder_->type() == Types::TYPE_DOUBLE);
}

bool Variant::isString() const
{
    return (holder_->type() == Types::TYPE_STRING);
}

bool Variant::tryCast(bool &result) const
{
    if (!holder_->canCastToBool()) return false;
    result = holder_->toBool();
    return true;
}

bool Variant::tryCast(int32_t &result) const
{
    if (!holder_->canCastToInt()) return false;
    result = static_cast<int32_t>(holder_->toInt());
    return true;
}

bool Variant::tryCast(int64_t &result) const
{
    if (!holder_->canCastToInt()) return false;
    result = holder_->toInt();
    return true;
}

bool Variant::tryCast(double &result) const
{
    if (!holder_->canCastToDouble()) return false;
    result = holder_->toDouble();
    return true;
}

Variant::operator bool() const
{
    return toBool();
}

Variant::operator int32_t() const
{
    return toInt();
}

Variant::operator int64_t() const
{
    return toInt64();
}

Variant::operator double() const
{
    return toDouble();
}

Variant::operator std::string() const
{
    return toString();
}

Variant::operator std::wstring() const
{
    return toWString();
}

///////////////////////////////////////////////////////////////////////////////
//
// operator<< function( of variant::Variant)
//
///////////////////////////////////////////////////////////////////////////////
bat_variant_cast::bat_variant_cast(const std::string &value,
        Variant::Types from, Variant::Types to, std::string function,
        std::string filename, int32_t line)
  : value_(value), from_(from), to_(to),
    function_(function), filename_(filename), line_(line)
{
    whatMessage_ = makeMessage();
}

const char* bat_variant_cast::what() const
{
    return whatMessage_.c_str();
}

std::string bat_variant_cast::makeMessage() const
{
    std::stringstream ss;
    ss << "Bat Variant Cast: Value = [" << value_ << "]. "
        << "Value type "
        << "from " << variantTypesMessage(from_)
        << "(Types = " << static_cast<int>(from_) << ") "
        << "to " << variantTypesMessage(to_)
        << "(Types = "  << static_cast<int>(to_) << "). "
        << "Function: " << function_
        << "(" << filename_ << ":" << line_ << ").";
    return ss.str();
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

///////////////////////////////////////////////////////////////////////////////
//
// operator<< function( of variant::Variant)
//
///////////////////////////////////////////////////////////////////////////////
std::wostream& operator<<(std::wostream& wos, const Variant &var)
{
    return var.holder_->writeWOstream(wos);
}
} // namespace variant