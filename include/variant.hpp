//===------------------ Instruction class definition ------------*- C++ -*-===//
//
// bool, int, double, string �����e���鑽�p�^
//
// �{�t�@�C���́C2����BSD���C�Z���X�ɏ]���Ĕz�z����܂��D
// �ڍׂ́ALICENSE.TXT���Q�Ƃ��Ă��������B
//
//===----------------------------------------------------------------------===//
///
/// \file
/// ���̃N���X�ɂ́C���p�^�N���X(variant)�̃N���X�錾���܂܂�܂��D
///
//===----------------------------------------------------------------------===//
#pragma once
#include <cstdint>
#include <string>
#include <memory>

#include "defines.hpp"

namespace variant {
/// boo, int, double, string ��4��̌^���_��ɑ���ł��鑽�p�^�ł�.
///
/// �{�N���X�́Cbool. int, double ,string �ɓn��4��ނ̊�{�^��
/// �_��ɑ���ł��鑽�p�^�ł��D�Ⴆ�΁C�ȉ��̃R�[�h�������܂��D
/// \code
///     Variant v1 = true;     // ok
///     Variant v2 = 100;      // ok
///     Variant v3 = 100.5;    // ok
///     Variant v4 = "string;  // ok
/// \endcode
///
/// ���ꂼ��̌^�����R�Ȍ`�ŕێ��ł��܂��D
/// ����́C���̌���Œ񋟂���� object �� Variant, var�^�̋�����
/// ���Ă��܂����C�ȉ��̋@�\�͒񋟂��Ă��܂���D
///    - �Z�p���Z(+, -, *, /)
///    - �u�[�����Z(|, &, ||, &&)
///    - �V�t�g���Z(<<. >>)
/// ����牉�Z���s�����߂ɂ́C��x���̌^�ɕϊ��������K�v������܂��D
///
/// ���̗��R�́Cint, double, string �Ȃǂ̒l���_��ɕێ����邱�Ƃ�
/// �ړI�̂��߂ł��D�܂��C���Z�n���_��ɑΉ��ł��Ȃ��̂����R��1�ł��D
///
/// �{�N���X�́C�^�ϊ����C���s����Ɨ�O(bat_cast)�𑗏o���܂��D
/// �ȉ��̃R�[�h�́C���l�ɕϊ��ł��Ȃ�������𐔒l�ɕϊ������Ƃ��̗�ł��D
/// varinat::bat_cast �𑗏o���܂��D
/// \code
///     Variant v = "Don't cast to int32_t";
///     try {
///         int32_t i = v.toInt();  // throw variant::bat_cast();
///     } catch (const variant::bat_cast &e) {
///         std::cout << e.what() << std::endl;
///     }
/// \endcode
///
/// ��O�̑��o����������Ƃ��́CtryCast ��p���ăL���X�g�\�����f���܂��D
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

    /// null pointer �̃R���X�g���N�^.
    ///
    /// �{�N���X�� null pointer ����̕ϊ������e���܂���D
    /// nullptr �������悤�Ƃ���ƃR���p�C���G���[�ƂȂ�܂��D
    /// �܂��CNULL ���������Ƃ��́C32bit �����t�������� 0 ��
    /// ���Ȃ���܂��D
    ///
    /// \code
    ///     Variant v(nullptr);   // NG, C2280
    ///     Variant v = nullptr;  // NG, C2440
    ///     Variant v(NULL);      // OK, Interpreted as an integer of 0.
    ///     Variant v = NULL;     // OK, Interpreted as an integer of 0.
    /// \endcode
    explicit  Variant(std::nullptr_t null) = delete;

    /// bool�^�̃R���X�g���N�^.
    ///
    /// bool�^����̕ϊ������e���邽�߂�
    /// �R���X�g���N�^���`���Ă��܂��D
    /// explicit �錾���Ȃ��͈̂Ӑ}���ꂽ���̂ł��D
    ///
    /// ��̗�:
    /// \code
    ///     Variant v = true;             // ok
    ///     std::cout << v << std::endl;  // print "1";
    ///     std::cout << std::boolalpha << v << std::endl;  // print "true";
    /// \endcode
    ///
    /// \param value false ���� true �̐^�U�l(bool�^)
    Variant(bool value);

    /// int32_t�^�̃R���X�g���N�^.
    ///
    /// int32_t�^����̕ϊ������e���邽�߂�
    /// �R���X�g���N�^���`���Ă��܂��D
    /// explicit �錾���Ȃ��͈̂Ӑ}���ꂽ���̂ł��D
    ///
    /// ��̗�:
    /// \code
    ///     Variant v = 100;              // ok
    ///     std::cout << v << std::endl;  // print "100";
    /// \endcode
    ///
    /// \param value 32bit �����t������
    Variant(int32_t value);

    /// double�^�̃R���X�g���N�^.
    ///
    /// double�^����̕ϊ������e���邽�߂�
    /// �R���X�g���N�^���`���Ă��܂��D
    /// explicit �錾���Ȃ��͈̂Ӑ}���ꂽ���̂ł��D
    ///
    /// ��̗�:
    /// \code
    ///     Variant v = 0.001;            // ok
    ///     std::cout << v << std::endl;  // print "0.001";
    /// \endcode
    ///
    /// \param value float�ȏ�Clong double �ȉ��̕��������_
    ///              (�قƂ�ǂ̃V�X�e���ł� 64bit)
    Variant(double value);

    /// ������^�̃R���X�g���N�^.
    ///
    /// ������^����̕ϊ������e���邽�߂�
    /// �R���X�g���N�^���`���Ă��܂��D
    /// explicit �錾���Ȃ��͈̂Ӑ}���ꂽ���̂ł��D
    ///
    /// ��̗�:
    /// \code
    ///     Variant v = "foo";       // ok
    ///     std::cout << v << std::endl;  // print "foo";
    ///     v = "bar";
    ///     std::cout << v << std::endl;  // print "bar";
    /// \endcode
    ///
    /// \param str Null�����ŏI������}���`�o�C�g����
    ///
    /// @{
    Variant(char *str);
    Variant(const char *str);
    Variant(const std::string &str);
    /// @}

    /// bool�^�ւ̕ϊ����s���܂�.
    ///
    /// �ێ�����Ă��鐔�l�C������� bool�^�֕ϊ����܂��D
    ///   - int, double
    ///       - 0    : false
    ///       - 0�ȊO: true 
    ///   - string
    ///       - "0": false
    ///       - "0" �ȊO�̐��l�ϊ��\�ȕ�����: true
    ///       - "false": false (False, fAlse �Ȃǂ̑啶���������̈Ⴂ�͋z��)
    ///       - "true" : true  (True, truE �Ȃǂ̑啶���������̈Ⴂ�͋z��)
    ///
    /// ���̃��\�b�h�́C��L�̂悤�� bool�^�֕ϊ��ł��Ȃ��Ƃ��C
    /// variant::bat_cast �𑗏o���܂��D
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