//
// Created by 藤田優太 on 2026/07/17.
//

#pragma once

#ifndef RINALANG_COMPONENTS_EXCEPTIONS_H
#define RINALANG_COMPONENTS_EXCEPTIONS_H
#include <stdexcept>

#endif //RINALANG_COMPONENTS_EXCEPTIONS_H

namespace rinalang::exception {

    struct runtime_exception : std::runtime_error {
        using std::runtime_error::runtime_error;
    };

    struct component_notfound_exception : runtime_exception {
        using runtime_exception::runtime_exception;
    };

    struct component_same_type_exception : runtime_exception {
        using runtime_exception::runtime_exception;
    };

    struct required_not_attached_exception : runtime_exception {
        using runtime_exception::runtime_exception;
    };

}