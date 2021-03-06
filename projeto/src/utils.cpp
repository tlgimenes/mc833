////////////////////////////////////////////////////////////////////////////////
/**
 * @file utils.cpp
 * @date 2017-06-11
 * @author Tiago Lobato Gimenes    (tlgimenes@gmail.com)
 *
 * @copyright Tiago Lobato Gimenes 2017. All rights reserved.
 *
 * @brief
 *
 * This file contains implementation of the correspoding header file, i.e. .hpp,
 * .hh or .h
 */
////////////////////////////////////////////////////////////////////////////////

#include "utils.hpp"

////////////////////////////////////////////////////////////////////////////////

log_level_t log::log_level = INFO;

////////////////////////////////////////////////////////////////////////////////

std::random_device random::rd;
std::mt19937 random::gen(rd());

////////////////////////////////////////////////////////////////////////////////
