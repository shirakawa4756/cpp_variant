//===------------------ Instruction class definition ------------*- C++ -*-===//
//
// bool, int, double, string を許容する多用型
//
// 本ファイルは，2条項BSDライセンスに従って配布されます．
// 詳細は、LICENSE.TXTを参照してください．
//
//===----------------------------------------------------------------------===//
///
/// \file
/// このクラスには，多用型クラス(variant)のクラス宣言が含まれます．
///
/// \author shirakawa4756@github.com
///
//===----------------------------------------------------------------------===//
#pragma once
#include <cstdint>
#include <string>
#include <memory>

#include "defines.hpp"

namespace variant {
/// boo, int, double, string の4種の型を柔軟に代入できる多用型です.
///
/// 本クラスは，bool. int, double ,string に渡る4種類の基本型を
/// 柔軟に代入できる多用型です．例えば，以下のコードを許します．
/// \code
///     Variant v1 = true;     // ok
///     Variant v2 = 100;      // ok
///     Variant v3 = 100.5;    // ok
///     Variant v4 = "string";  // ok
/// \endcode
///
/// それぞれの型を自然な形で保持できます．
/// これは，他の言語で提供される object や Variant, var型の挙動と
/// 似ていますが，以下の機能は提供していません．
///    - 算術演算(+, -, *, /)
///    - ブール演算(|, &, ||, &&)
///    - シフト演算(<<. >>)
/// これら演算を行うためには，一度元の型に変換し直す必要があります．
///
/// その理由は，int, double, string などの値を柔軟に保持することが
/// 目的のためです．また，算術演算などに対応するためには，複雑な
/// コードにならざるを得ないのも理由の1つです．
///
/// 本クラスは，型変換時，失敗すると例外(bat_variant_cast)を送出します．
/// 以下のコードは，数値に変換できない文字列を数値に変換するメソッドを
/// 実行したときの例です．varinat::bat_variant_cast を送出します．
/// \code
///     Variant v = "Don't cast to int32_t";
///     try {
///         int32_t i = v.toInt();  // throw variant::variant_bat_cast();
///     } catch (const variant::variant_bat_cast &e) {
///         std::cout << e.what() << std::endl;
///     }
/// \endcode
///
/// 例外の送出を避けたいときは，tryCast を用いてキャスト可能か判断します．
/// \code
///    Variant v  = "Don't cast to int32_t";
///    int result = 0;
///    if (v.tryCast(result)) {
///        std:cout << "Successful cast to int. " << result << std::endl;
///    } else {
///        std:cout << "Bat cast to int. " << result << std::endl;
///    }
/// \endcode
class VARIANT_API Variant
{
public:
    /// デフォルトコンストラクタ.
    ///
    /// 引数無指定でオブジェクトを作成すると
    /// 整数 0 として扱われます．
    Variant();
    Variant(const Variant &rhs) = default;
    Variant& operator=(const Variant &rhs) = default;

    /// null pointer のコンストラクタ.
    ///
    /// 本クラスは null pointer からの変換を許容しません．
    /// nullptr を代入しようとするとコンパイルエラーとなります．
    /// また，NULL を代入したときは，64bit 符号付き整数の 0 と
    /// 見なされます．
    ///
    /// \code
    ///     Variant v(nullptr);   // NG, C2280
    ///     Variant v = nullptr;  // NG, C2280
    ///     Variant v(NULL);      // OK, Interpreted as an integer of 0.
    ///     Variant v = NULL;     // OK, Interpreted as an integer of 0.
    /// \endcode
    Variant(std::nullptr_t null) = delete;

    /// bool型のコンストラクタ.
    ///
    /// bool型からの変換を許容するために
    /// コンストラクタを定義しています．
    /// explicit 宣言がないのは意図されたものです．
    ///
    /// 具体例:
    /// \code
    ///     Variant v = true;             // ok
    ///     std::cout << v << std::endl;  // print "1";
    ///     std::cout << std::boolalpha << v << std::endl;  // print "true";
    /// \endcode
    ///
    /// \param value false から true の真偽値(bool型)
    Variant(bool value);

    /// int32_t型のコンストラクタ.
    ///
    /// int32_t型からの変換を許容するために
    /// コンストラクタを定義しています．
    /// explicit 宣言がないのは意図されたものです．
    ///
    /// 具体例:
    /// \code
    ///     Variant v = 100;              // ok
    ///     std::cout << v << std::endl;  // print "100";
    /// \endcode
    ///
    /// \param value 32bit 符号付き整数
    Variant(int32_t value);

    /// int64_t型のコンストラクタ.
    ///
    /// int64_t型からの変換を許容するために
    /// コンストラクタを定義しています．
    /// explicit 宣言がないのは意図されたものです．
    ///
    /// 具体例:
    /// \code
    ///     Variant v = 100;              // ok
    ///     std::cout << v << std::endl;  // print "100";
    /// \endcode
    ///
    /// \param value 64bit 符号付き整数
    Variant(int64_t value);

    /// double型のコンストラクタ.
    ///
    /// double型からの変換を許容するために
    /// コンストラクタを定義しています．
    /// explicit 宣言がないのは意図されたものです．
    ///
    /// 具体例:
    /// \code
    ///     Variant v = 0.001;            // ok
    ///     std::cout << v << std::endl;  // print "0.001";
    /// \endcode
    ///
    /// \param value float以上，long double 以下の浮動小数点
    ///              (ほとんどのシステムでは 64bit)
    Variant(double value);

    /// 文字列型のコンストラクタ.
    ///
    /// 文字列型からの変換を許容するために
    /// コンストラクタを定義しています．
    /// explicit 宣言がないのは意図されたものです．
    ///
    /// 具体例:
    /// \code
    ///     Variant v = "foo";       // ok
    ///     std::cout << v << std::endl;  // print "foo";
    ///     v = "bar";
    ///     std::cout << v << std::endl;  // print "bar";
    /// \endcode
    ///
    /// \param str Null文字で終了するマルチバイト文字
    ///
    /// @{
    Variant(const char *str);
    Variant(const std::string &str);
    /// @}

    /// bool型への変換を行います.
    ///
    /// 保持されている数値，文字列を bool型へ変換します．
    ///   - int, double
    ///       - 0    : false
    ///       - 0以外: true 
    ///   - string
    ///       - "0": false
    ///       - "0" 以外の数値変換可能な文字列: true
    ///       - "false": false (False, fAlse などの大文字小文字の違いは吸収)
    ///       - "true" : true  (True, truE などの大文字小文字の違いは吸収)
    ///
    /// このメソッドは，上記のように bool型へ変換できないとき，
    /// variant::bat_cast を送出します．
    /// \code
    ///     Variant v = "false";
    ///     bool i    = v.toBool();  // to false;
    /// \endcode
    bool toBool() const;

    /// int32_t 型への変換を行います.
    ///
    /// 保持されている数値，文字列を int32_t 型へ変換します．
    /// 本クラスの整数型は int64_t で保持されますが，利便性のため
    /// int32_t への変換メソッドが用意されています．
    ///   - bool
    ///       - false: 0
    ///       - true : 1
    ///   - double
    ///       - static_cast<int32_t> で変換される数値
    ///   - string
    ///       - atoi, stoi で変換される数値
    int32_t toInt() const;

    /// int64_t 型への変換を行います.
    ///
    /// 保持されている数値，文字列を int64_t 型へ変換します．
    ///   - bool
    ///       - false: 0
    ///       - true : 1
    ///   - double
    ///       - static_cast<int64_t> で変換される数値
    ///   - string
    ///       - atol, stol で変換される数値
    int64_t toInt64() const;

    /// double 型への変換を行います.
    ///
    /// 保持されている数値，文字列を double 型へ変換します．
    ///   - bool
    ///       - false: 0.0
    ///       - true : 1.0
    ///   - int
    ///       - static_cast<double> で変換される数値
    ///   - string
    ///       - atod, stod で変換される数値
    double toDouble() const;

    /// 文字列型(std::string)へ変換します．
    ///
    /// 本メソッドからは例外が送出されることはありません(不送出保証)．
    std::string toString() const;

    /// 文字列型(std::wstring)へ変換します．
    ///
    /// \param locale ロケール指定
    ///
    /// 本メソッドからは例外が送出されることはありません(不送出保証)．
    std::wstring toWString() const;

    /// 保持されている元の型が bool 型のとき true を返します
    ///
    /// @return true のとき，元の型が bool
    ///
    /// 例えば以下のとき，true となります．
    /// \code
    ///     Variant v = true;
    ///     if (v.isBool()) {
    ///         std::cout << "v is bool." << std::endl;
    ///     }
    /// \endcode
    bool isBool() const;

    /// 保持されている元の型が int 型のとき true を返します
    ///
    /// @return true のとき，元の型が int
    ///
    /// 例えば以下のとき，true となります．
    /// \code
    ///     Variant v = 100;
    ///     if (v.isInt()) {
    ///         std::cout << "v is int." << std::endl;
    ///     }
    /// \endcode
    bool isInt() const;

    /// 保持されている元の型が double 型のとき true を返します
    ///
    /// @return true のとき，元の型が double
    ///
    /// 例えば以下のとき，true となります．
    /// \code
    ///     Variant v = 100;
    ///     if (v.isInt()) {
    ///         std::cout << "v is double." << std::endl;
    ///     }
    /// \endcode
    bool isDouble() const;

    /// 保持されている元の型が std::string 型のとき true を返します
    ///
    /// @return true のとき，元の型が std::string
    ///
    /// 例えば以下のとき，true となります．
    /// \code
    ///     Variant v = "anything";
    ///     if (v.isString()) {
    ///         std::cout << "v is std::string." << std::endl;
    ///     }
    /// \endcode
    bool isString() const;

    /// bool型 に変換します．返し値は，演算が成功したとき true を返します.
    ///
    /// @param[out] result 変換された bool 型
    /// @return true のとき，成功したことを示します
    ///
    /// 具体例:
    /// \code
    ///     variant::Variant values[] = { 0, 1, 0.0, 1.0, "true", "Not Boolean" };
    ///
    ///     for (const auto &v : values) {
    ///         bool result = false;
    ///         if (v.tryCast(result)) {
    ///             std::cout << "Converted: " << v << "-->"
    ///                       << std::boolalpha << result << std::endl;
    ///         }
    ///         else {
    ///             std::cout << "Attempted: " << v << std::endl;
    ///         }
    //     }
    /// \endcode
    bool tryCast(bool &result) const;

    /// bool型 に変換します．返し値は，演算が成功したとき true を返します.
    ///
    /// @param[out] result 変換された bool 型
    /// @return true のとき，成功したことを示します
    ///
    /// 具体例:
    /// \code
    ///     Variant values[] = {
    ///             false, true, 0, 1, -10, 10, 0.0, 1.0, -10.1, 10.1,
    ///             "false", "true", "0", "1", "-10", "10", "0.0", "1.0",
    ///             "-10.2", "10.2" };
    ///
    ///     for (const auto &v : values) {
    ///         int32_t result = -1.0;
    ///         if (v.tryCast(result)) {
    ///             std::cout << "Converted: " << v << "-->"
    ///                       << result << std::endl;
    ///         }
    ///         else {
    ///             std::cout << "Attempted: " << v << std::endl;
    ///         }
    //     }
    /// \endcode
    bool tryCast(int32_t &result) const;

    /// bool型 に変換します．返し値は，演算が成功したとき true を返します.
    ///
    /// @param[out] result 変換された bool 型
    /// @return true のとき，成功したことを示します
    ///
    /// 具体例:
    /// \code
    ///     Variant values[] = {
    ///             false, true, 0, 1, -10, 10, 0.0, 1.0, -10.1, 10.1,
    ///             "false", "true", "0", "1", "-10", "10", "0.0", "1.0",
    ///             "-10.2", "10.2" };
    ///
    ///     for (const auto &v : values) {
    ///         int64_t result = -1.0;
    ///         if (v.tryCast(result)) {
    ///             std::cout << "Converted: " << v << "-->"
    ///                       << result << std::endl;
    ///         }
    ///         else {
    ///             std::cout << "Attempted: " << v << std::endl;
    ///         }
    //     }
    /// \endcode
    bool tryCast(int64_t &result) const;

    /// double 型に変換します．変換が成功したとき true を返します.
    ///
    /// @param[out] result 変換された double 型
    /// @return true のとき，成功したことを示します
    ///
    /// 具体例:
    /// \code
    ///     Variant values[] = {
    ///             false, true, 0, 1, -10, 10, 0.0, 1.0, -10.1, 10.1,
    ///             "false", "true", "0", "1", "-10", "10", "0.0", "1.0",
    ///             "-10.2", "10.2" };
    ///
    ///     for (const auto &v : values) {
    ///         double result = -1.0;
    ///         if (v.tryCast(result)) {
    ///             std::cout << "Converted: " << v << "-->"
    ///                       << result << std::endl;
    ///         }
    ///         else {
    ///             std::cout << "Attempted: " << v << std::endl;
    ///         }
    //     }
    /// \endcode
    bool tryCast(double &result) const;

    operator bool() const;
    operator int32_t() const;
    operator int64_t() const;
    operator double() const;
    operator std::string() const;
    operator std::wstring() const;

    /// 出力ストリームをサポートするために friend 宣言されています
    /// @{
    friend VARIANT_API
        std::ostream& operator<<(std::ostream& os, const Variant &var);
    friend VARIANT_API
        std::wostream& operator<<(std::wostream& wos, const Variant &var);
    /// @}
private:
    class Holder;
    class BoolHolder;
    class IntHolder;
    class DoubleHolder;
    class StringHolder;
    class HolderFactory;
    std::shared_ptr<const Holder> holder_;
}; // class class VARIANT_API Variant_

VARIANT_API std::ostream& operator<<(std::ostream& os, const Variant &var);
VARIANT_API std::wostream& operator<<(std::wostream& wos, const Variant &var);
} // namespace variant