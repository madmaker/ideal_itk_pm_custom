/*
 * misc.cxx
 *
 *  Created on: Apr 1, 2015
 *      Author: Administrator
 */

#include "misc.hxx"
/**
 *
 */
//cppcheck-suppress unusedFunction
bool starts_with(const std::string & param, const std::string & what) {
	if (param.size() < what.size())
		return false;
	return !param.compare(0, what.size(), what);
}
