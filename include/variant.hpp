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
/// このクラスには，多用型クラス(variant)のクラス宣言が含まれます．
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
///     Variant v4 = "string;  // ok
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
/// 目的のためです．また，演算系も柔軟に対応できないのも理由の1つです．
///
/// 本クラスは，型変換時，失敗すると例外(bat_cast)を送出します．
/// 以下のコードは，数値に変換できない文字列を数値に変換したときの例です．
/// varinat::bat_cast を送出します．
/// \code
///     Variant v = "Don't cast to int32_t";
///     try {
///         int32_t i = v.toInt();  // throw variant::bat_cast();
///     } catch (const variant::bat_cast &e) {
///         std::cout << e.what() << std::endl;
///     }
/// \endcode
///
/// 例外の送出を避けたいときは，tryCast を用いてキャスト可能か判断します．
/// \code
///    Variant v = "Don't cast to int32_t";
///
///    int result;
///    if (v.tryCast(result)) {
///        std:cout << "Successful cast to int. " << result << std::endl;
///    } else {
///        std:cout << "Bat cast to int. " << result << std::endl;
///    }
/// \endcode
class VARIANT_API Variant
{
public:
    Variant() = default;
    Variant(const Variant &rhs) = default;
    Variant& operator=(const Variant &rhs) = default;

    /// null pointer のコンストラクタ.
    ///
    /// 本クラスは null pointer からの変換を許容しません．
    /// nullptr を代入しようとするとコンパイルエラーとなります．
    /// また，NULL を代入したときは，32bit 符号付き整数の 0 と
    /// 見なされます．
    ///
    /// \code
    ///     Variant v(nullptr);   // NG, C2280
    ///     Variant v = nullptr;  // NG, C2440
    ///     Variant v(NULL);      // OK, Interpreted as an integer of 0.
    ///     Variant v = NULL;     // OK, Interpreted as an integer of 0.
    /// \endcode
    explicit  Variant(std::nullptr_t null) = delete;

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
    Variant(char *str);
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
    int32_t toInt() const;
    double toDouble() const;
    std::string toString() const;

    bool isBool() const;
    bool isInt() const;
    bool isDouble() const;
    bool isString() const;

    bool tryCast(bool &result) const;
    bool tryCast(int32_t &result) const;
    bool tryCast(double &result) const;
    bool tryCast(std::string &result) const;

    friend std::ostream& operator<<(std::ostream& os, const Variant &var);
private:
    class Holder;
    class BoolHolder;
    class IntHolder;
    class DoubleHolder;
    class StringHolder;
    class HolderFactory;
    std::shared_ptr<Holder> holder_;
}; // class class VARIANT_API Variant_

std::ostream& operator<<(std::ostream& os, const Variant &var);
} // namespace variant