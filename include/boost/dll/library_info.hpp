// Copyright 2014 Renato Tegon Forti, Antony Polukhin.
// Copyright 2015-2019 Antony Polukhin.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "config.hpp"
#include "detail/pe_info.hpp"
#include "detail/elf_info.hpp"
#include "detail/macho_info.hpp"

#include <fstream>
#include <filesystem>
#include <system_error>
#include <assert.h>

/// \file boost/dll/library_info.hpp
/// \brief Contains only the boost::dll::library_info class that is capable of
/// extracting different information from binaries.

namespace boost { namespace dll {

/*!
* \brief Class that is capable of extracting different information from a library or binary file.
* Currently understands ELF, MACH-O and PE formats on all the platforms.
*/
class library_info {
private:
    std::ifstream f_;

    enum {
        fmt_elf_info32,
        fmt_elf_info64,
        fmt_pe_info32,
        fmt_pe_info64,
        fmt_macho_info32,
        fmt_macho_info64
    } fmt_;

    /// @cond
    inline static void throw_if_in_32bit_impl(std::true_type /* is_32bit_platform */) {
        throw std::runtime_error("Not native format: 64bit binary");
    }

    inline static void throw_if_in_32bit_impl(std::false_type /* is_32bit_platform */) noexcept {}


    inline static void throw_if_in_32bit() {
        throw_if_in_32bit_impl( std::integral_constant<bool, (sizeof(void*) == 4)>() );
    }

    static void throw_if_in_windows() {
#if defined(_WIN32)
        throw std::runtime_error("Not native format: not a PE binary");
#endif
    }

    static void throw_if_in_linux() {
#if !defined(_WIN32) && !defined(__APPLE__)
        throw std::runtime_error("Not native format: not an ELF binary");
#endif
    }

    static void throw_if_in_macos() {
#if defined(__APPLE__)
        throw std::runtime_error("Not native format: not an Mach-O binary");
#endif
    }

    void init(bool throw_if_not_native) {
        if (boost::dll::detail::elf_info32::parsing_supported(f_)) {
            if (throw_if_not_native) { throw_if_in_windows(); throw_if_in_macos(); }

            fmt_ = fmt_elf_info32;
        } else if (boost::dll::detail::elf_info64::parsing_supported(f_)) {
            if (throw_if_not_native) { throw_if_in_windows(); throw_if_in_macos(); throw_if_in_32bit(); }

            fmt_ = fmt_elf_info64;
        } else if (boost::dll::detail::pe_info32::parsing_supported(f_)) {
            if (throw_if_not_native) { throw_if_in_linux(); throw_if_in_macos(); }

            fmt_ = fmt_pe_info32;
        } else if (boost::dll::detail::pe_info64::parsing_supported(f_)) {
            if (throw_if_not_native) { throw_if_in_linux(); throw_if_in_macos(); throw_if_in_32bit(); }

            fmt_ = fmt_pe_info64;
        } else if (boost::dll::detail::macho_info32::parsing_supported(f_)) {
            if (throw_if_not_native) { throw_if_in_linux(); throw_if_in_windows(); }

            fmt_ = fmt_macho_info32;
        } else if (boost::dll::detail::macho_info64::parsing_supported(f_)) {
            if (throw_if_not_native) { throw_if_in_linux(); throw_if_in_windows(); throw_if_in_32bit(); }

            fmt_ = fmt_macho_info64;
        } else {
            throw std::runtime_error("Unsupported binary format");
        }
    }
    /// @endcond

public:
    library_info(library_info const &) = delete;
    /*!
    * Opens file with specified path and prepares for information extraction.
    * \param library_path Path to the binary file from which the info must be extracted.
    * \param throw_if_not_native_format Throw an exception if this file format is not
    * supported by OS.
    */
    explicit library_info(const std::filesystem::path& library_path, bool throw_if_not_native_format = true)
        : f_(
            library_path,
            std::ios_base::in | std::ios_base::binary
        )
    {
        f_.exceptions(
            std::ios_base::failbit
            | std::ifstream::badbit
            | std::ifstream::eofbit
        );

        init(throw_if_not_native_format);
    }

    /*!
    * \return List of sections that exist in binary file.
    */
    std::vector<std::string> sections() {
        switch (fmt_) {
        case fmt_elf_info32:   return boost::dll::detail::elf_info32::sections(f_);
        case fmt_elf_info64:   return boost::dll::detail::elf_info64::sections(f_);
        case fmt_pe_info32:    return boost::dll::detail::pe_info32::sections(f_);
        case fmt_pe_info64:    return boost::dll::detail::pe_info64::sections(f_);
        case fmt_macho_info32: return boost::dll::detail::macho_info32::sections(f_);
        case fmt_macho_info64: return boost::dll::detail::macho_info64::sections(f_);
        };
        assert(false);
        return {};
    }

    /*!
    * \return List of all the exportable symbols from all the sections that exist in binary file.
    */
    std::vector<std::string> symbols() {
        switch (fmt_) {
        case fmt_elf_info32:   return boost::dll::detail::elf_info32::symbols(f_);
        case fmt_elf_info64:   return boost::dll::detail::elf_info64::symbols(f_);
        case fmt_pe_info32:    return boost::dll::detail::pe_info32::symbols(f_);
        case fmt_pe_info64:    return boost::dll::detail::pe_info64::symbols(f_);
        case fmt_macho_info32: return boost::dll::detail::macho_info32::symbols(f_);
        case fmt_macho_info64: return boost::dll::detail::macho_info64::symbols(f_);
        };
        assert(false);
        return {};
    }

    /*!
    * \param section_name Name of the section from which symbol names must be returned.
    * \return List of symbols from the specified section.
    */
    std::vector<std::string> symbols(const char* section_name) {
        switch (fmt_) {
        case fmt_elf_info32:   return boost::dll::detail::elf_info32::symbols(f_, section_name);
        case fmt_elf_info64:   return boost::dll::detail::elf_info64::symbols(f_, section_name);
        case fmt_pe_info32:    return boost::dll::detail::pe_info32::symbols(f_, section_name);
        case fmt_pe_info64:    return boost::dll::detail::pe_info64::symbols(f_, section_name);
        case fmt_macho_info32: return boost::dll::detail::macho_info32::symbols(f_, section_name);
        case fmt_macho_info64: return boost::dll::detail::macho_info64::symbols(f_, section_name);
        };
        assert(false);
        return {};
    }


    //! \overload std::vector<std::string> symbols(const char* section_name)
    std::vector<std::string> symbols(const std::string& section_name) {
        switch (fmt_) {
        case fmt_elf_info32:   return boost::dll::detail::elf_info32::symbols(f_, section_name.c_str());
        case fmt_elf_info64:   return boost::dll::detail::elf_info64::symbols(f_, section_name.c_str());
        case fmt_pe_info32:    return boost::dll::detail::pe_info32::symbols(f_, section_name.c_str());
        case fmt_pe_info64:    return boost::dll::detail::pe_info64::symbols(f_, section_name.c_str());
        case fmt_macho_info32: return boost::dll::detail::macho_info32::symbols(f_, section_name.c_str());
        case fmt_macho_info64: return boost::dll::detail::macho_info64::symbols(f_, section_name.c_str());
        };
        assert(false);
        return {};
    }
};

}} // namespace boost::dll
