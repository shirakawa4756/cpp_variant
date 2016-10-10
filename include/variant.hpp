//===-- llvm/Instruction.h - Instruction class definition -------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// This file contains the declaration of the Instruction class, which is the
/// base class for all of the VM instructions.
///
//===----------------------------------------------------------------------===//
#pragma once
#include <cstdint>
#include <string>
#include <memory>

#include "defines.hpp"

namespace variant {

/**
 *
 */
class VARIANT_API Variant
{
public:
    using String         = std::string;
    using char_type      = String::traits_type::char_type;
    using traits_type    = String::traits_type;
    using allocator_type = String::allocator_type;

    Variant() = default;
    Variant(const Variant &rhs) = default;
    Variant& operator=(const Variant &rhs) = default;

    Variant(std::nullptr_t null);
    Variant(bool value);
    Variant(int32_t value);
    Variant(int64_t value);
    Variant(uint32_t value);
    Variant(uint64_t value);
    Variant(double value);
    Variant(char_type *value);
    Variant(const String &value);

    bool to_bool() const;
    int64_t to_int() const;
    uint64_t to_uint() const;
    double to_double() const;
    String to_string() const;

    operator bool() const;
    operator int64_t() const;
    operator uint64_t() const;
    operator double() const;
    operator String() const;

private:
    class Holder;
    class BoolHolder;
    class IntHolder;
    class UIntHolder;
    class DoubleHolder;
    class StringHolder;
    class HolderFactory;
    std::shared_ptr<Holder> holder_;
}; // class class VARIANT_API Variant_
} // namespace variant