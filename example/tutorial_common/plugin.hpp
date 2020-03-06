// Copyright 2016-2019 Antony Polukhin.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DLL_MY_PLUGIN_API_HPP
#define BOOST_DLL_MY_PLUGIN_API_HPP

#define visible __attribute__((__visibility__("default")))

//[plugapi
#include <string>

class visible plugin_base {
public:
   virtual std::string name() const = 0;
   virtual float calculate(float x, float y) = 0;

   virtual ~plugin_base() {}
};
//]
   
#endif // BOOST_DLL_MY_PLUGIN_API_HPP

