/*
 * xx99_properties_helper.cxx
 *
 *  Created on: Apr 1, 2015
 *      Author: Administrator
 */

#include <iostream>
#include <string>
#include <base_utils/ResultCheck.hxx>
#include <tc/tc.h>
#include <tccore/custom.h>
#include "process_error.hxx"
#include "pm8_properties.hxx"
#include "pm8_action_handlers.hxx"

//cppcheck-suppress unusedFunction
extern "C" __declspec (dllexport) int pm8_properties_helper_register_callbacks() {
	try {
		ResultCheck erc;
		erc = CUSTOM_register_exit("pm8_properties_helper",
				"USER_register_properties",
				pm8_properties_helper_register_properties);

		erc = CUSTOM_register_exit("pm8_properties_helper", "USER_gs_shell_init_module",
				pm8_idealplm_custom_register_action_handlers);

	} catch (...) {
		return sisw::process_error(true, true, false);
	}
	return ITK_ok;
}
