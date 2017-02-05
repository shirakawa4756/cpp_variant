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
    Variant() = default;
    Variant(const Variant &rhs) = default;
    Variant& operator=(const Variant &rhs) = default;

    Variant(std::nullptr_t null);
    Variant(bool b);
    Variant(int32_t i);
    Variant(double d);
    Variant(char *str);
    Variant(const std::string &str);

    bool to_bool() const;
    int32_t to_int() const;
    double to_double() const;
    std::string to_string() const;
    std::wstring to_wstring() const;

    operator bool() const;
    operator int32_t() const;
    operator double() const;
    operator std::string() const;

private:
    class Holder;
    class BoolHolder;
    class IntHolder;
    class DoubleHolder;
    class StringHolder;
    class HolderFactory;
    std::shared_ptr<Holder> holder_;
}; // class class VARIANT_API Variant_
} // namespace variant