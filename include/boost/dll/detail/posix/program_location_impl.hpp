// Copyright 2014 Renato Tegon Forti, Antony Polukhin.
// Copyright 2015-2019 Antony Polukhin.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <boost/dll/config.hpp>
#include <boost/dll/detail/system_error.hpp>
#include <boost/predef/os.h>

#include <filesystem>
#include <system_error>

#if BOOST_OS_MACOS || BOOST_OS_IOS

#include <mach-o/dyld.h>

namespace boost { namespace dll { namespace detail {
    inline std::filesystem::path program_location_impl(std::error_code &ec) {
        ec.clear();

        char path[1024];
        uint32_t size = sizeof(path);
        if (_NSGetExecutablePath(path, &size) == 0)
            return std::filesystem::path(path);

        char *p = new char[size];
        if (_NSGetExecutablePath(p, &size) != 0) {
            ec = std::make_error_code(
                std::errc::bad_file_descriptor
            );
        }

        std::filesystem::path ret(p);
        delete[] p;
        return ret;
    }
}}} // namespace dll::detail

#elif BOOST_OS_SOLARIS

#include <stdlib.h>
namespace boost { namespace dll { namespace detail {
    inline std::filesystem::path program_location_impl(std::error_code& ec) {
        ec.clear();

        return std::filesystem::path(getexecname());
    }
}}} // namespace dll::detail

#elif BOOST_OS_BSD_FREE

#include <sys/types.h>
#include <sys/sysctl.h>
#include <stdlib.h>

namespace boost { namespace dll { namespace detail {
    inline std::filesystem::path program_location_impl(std::error_code& ec) {
        ec.clear();

        int mib[4];
        mib[0] = CTL_KERN;
        mib[1] = KERN_PROC;
        mib[2] = KERN_PROC_PATHNAME;
        mib[3] = -1;
        char buf[10240];
        size_t cb = sizeof(buf);
        sysctl(mib, 4, buf, &cb, NULL, 0);

        return std::filesystem::path(buf);
    }
}}} // namespace dll::detail



#elif BOOST_OS_BSD_NET

namespace boost { namespace dll { namespace detail {
    inline std::filesystem::path program_location_impl(std::error_code &ec) {
        return std::filesystem::read_symlink("/proc/curproc/exe", ec);
    }
}}} // namespace dll::detail

#elif BOOST_OS_BSD_DRAGONFLY


namespace boost { namespace dll { namespace detail {
    inline std::filesystem::path program_location_impl(std::error_code &ec) {
        return std::filesystem::read_symlink("/proc/curproc/file", ec);
    }
}}} // namespace dll::detail

#elif BOOST_OS_QNX

#include <fstream>
#include <string> // for std::getline
namespace boost { namespace dll { namespace detail {
    inline std::filesystem::path program_location_impl(std::error_code &ec) {
        ec.clear();

        std::string s;
        std::ifstream ifs("/proc/self/exefile");
        std::getline(ifs, s);

        if (ifs.fail() || s.empty()) {
            ec = std::make_error_code(
                std::errc::bad_file_descriptor
            );
        }

        return std::filesystem::path(s);
    }
}}} // namespace dll::detail

#else  // BOOST_OS_LINUX || BOOST_OS_UNIX || BOOST_OS_HPUX || BOOST_OS_ANDROID

namespace boost { namespace dll { namespace detail {
    inline std::filesystem::path program_location_impl(std::error_code &ec) {
        // We can not use
        // dll::detail::path_from_handle(dlopen(NULL, RTLD_LAZY | RTLD_LOCAL), ignore);
        // because such code returns empty path.

        return std::filesystem::read_symlink("/proc/self/exe", ec);   // Linux specific
    }
}}} // namespace dll::detail

#endif

