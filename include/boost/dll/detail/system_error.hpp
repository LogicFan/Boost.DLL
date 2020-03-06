// Copyright 2014 Renato Tegon Forti, Antony Polukhin.
// Copyright 2015-2019 Antony Polukhin.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <boost/dll/config.hpp>
#include <boost/predef/os.h>
#include <boost/throw_exception.hpp>

#if !defined(_WIN32)
#   include <dlfcn.h>
#endif

#include <filesystem>
#include <system_error>

namespace boost { namespace dll { namespace detail {

    inline void reset_dlerror() noexcept {
#if !defined(_WIN32)
        const char* const error_txt = dlerror();
        (void)error_txt;
#endif
    }

    inline void report_error(const std::error_code& ec, const char* message) {
#if !defined(_WIN32)
        const char* const error_txt = dlerror();
        if (error_txt) {
            boost::throw_exception(
                std::system_error(
                    ec,
                    message + std::string(" (dlerror system message: ") + error_txt + std::string(")")
                )
            );
        }
#endif

        boost::throw_exception(
            std::system_error(
                ec, message
            )
        );
    }

}}} // dll::detail

