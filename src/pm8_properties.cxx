/*
 * xx99_properties.cxx
 *
 *  Created on: Apr 1, 2015
 *      Author: Administrator
 */

#include <iostream>
#include <string>
#include <base_utils/ResultCheck.hxx>
#include <tc/tc.h>
#include <property/prop.h>
#include <property/prop_msg.h>
#include <tccore/custom.h>
#include <tccore/method.h>
#include "process_error.hxx"
#include "pm8_get_source_value.hxx"
#include "pm8_get_source_sw_value.hxx"
#include "pm8_get_rig_value.hxx"
#include "pm8_get_document_type2_value.hxx"
#include "pm8_get_mat_string_nx_value.hxx"
#include "pm8_properties.hxx"

/**
 *
 */
int pm8_properties_helper_register_properties(int *decision, va_list args) {
	try {
		printf("\n");
		printf("REGISTRERING PROPERTIES...");
		printf("\n");
		//
		*decision = ALL_CUSTOMIZATIONS;
		//
		METHOD_id_t meth0 = { 0 };
		METHOD_id_t meth1 = { 0 };

		ResultCheck erc;
		erc = METHOD_register_prop_method("Pm8_CompanyPartRevision",
				"pm8_Source", PROP_ask_value_string_msg,
				pm8_get_source_value /*pm8_get_document_type2_value*/, NULL,
				&meth0);

		erc = METHOD_register_prop_method("Pm8_CompanyPartRevision",
				"pm8_SourceSW", PROP_ask_value_string_msg,
				pm8_get_source_sw_value, NULL, &meth0);

		/*erc = METHOD_register_prop_method("Pm8_CompanyPartRevision",
		 "pm8_Rig", PROP_ask_value_tags_msg,
		 pm8_get_rig_value, NULL, &meth1);*/

		erc = METHOD_register_prop_method("CommercialPart Revision",
				"pm8_Source", PROP_ask_value_string_msg, pm8_get_source_value,
				NULL, &meth0);

		erc = METHOD_register_prop_method("CommercialPart Revision",
				"pm8_SourceSW", PROP_ask_value_string_msg,
				pm8_get_source_sw_value, NULL, &meth0);

		/*erc = METHOD_register_prop_method("CommercialPart Revision",
		 "pm8_Rig", PROP_ask_value_tags_msg,
		 pm8_get_rig_value, NULL, &meth1);*/

		erc = METHOD_register_prop_method("Pm8_KD", "pm8_DocumentType",
				PROP_ask_value_string_msg, pm8_get_document_type2_value, NULL,
				&meth0);

		erc = METHOD_register_prop_method("Pm8_CompanyPartRevision",
				"pm8_MatStringNX", PROP_ask_value_string_msg,
				pm8_get_mat_string_nx_value, NULL, &meth0);

	} catch (...) {
		return sisw::process_error(true, true, false);
	}
	return ITK_ok;
}
