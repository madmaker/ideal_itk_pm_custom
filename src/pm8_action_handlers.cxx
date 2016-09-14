#include <iostream>
#include <string>
#include <base_utils/ResultCheck.hxx>
#include <tc/tc.h>
#include <property/prop_msg.h>
#include <tccore/custom.h>
#include <tccore/method.h>
#include "process_error.hxx"
#include "pm8_action_handlers.hxx"
#include "pm8_rename_namedrefs.hxx"

/**
 *
 */
int pm8_idealplm_custom_register_action_handlers(int * decision, va_list args) {
	try {
		printf("\n");
		printf("REGISTRERING ACTION HANDLERS...");
		printf("\n");

		int ifail= ITK_ok;
		ResultCheck erc;
		*decision = ALL_CUSTOMIZATIONS;

		ifail = EPM_register_action_handler("Pm8-rename-namedrefs", "", pm8_rename_namedrefs);

	} catch (...) {
		return sisw::process_error(true, true, false);
	}
	return ITK_ok;
}
