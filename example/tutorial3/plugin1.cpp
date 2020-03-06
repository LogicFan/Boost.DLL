// Copyright 2014 Renato Tegon Forti, Antony Polukhin.
// Copyright 2015-2019 Antony Polukhin.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>


//[plugcpp_my_plugin_sum
#include <boost/config.hpp> // for BOOST_SYMBOL_EXPORT
#include "../tutorial_common/plugin.hpp"

namespace my_namespace {

class my_plugin_sum : public plugin_base {
public:
    my_plugin_sum() {
        std::cout << "Constructing plugin" << std::endl;
    }

    std::string name() const {
        return "sum";
    }

    float calculate(float x, float y) {
        return x + y;
    }
   
    ~my_plugin_sum() {
        std::cout << "Destructing plugin" << std::endl;
    }
};

// Exporting `my_namespace::plugin` variable with alias name `plugin`
// (Has the same effect as `BOOST_DLL_ALIAS(my_namespace::plugin, plugin)`)
extern "C" BOOST_SYMBOL_EXPORT my_plugin_sum plugin;
my_plugin_sum plugin;

} // namespace my_namespace

//]
