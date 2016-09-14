/*
 * pm8_get_source_value.cxx
 *
 *  Created on: Apr 29, 2015
 *      Author: infodba
 */
#include <iostream>
#include <string>
#include <stdarg.h>

#include <base_utils/ResultCheck.hxx>
#include <tc/tc.h>
#include <tccore/grm.h>
#include <tccore/method.h>
#include <tccore/tctype.h>
#include <tccore/aom_prop.h>
#include <sa/sa.h>
#include <tccore/aom.h>
#include <ug_va_copy.h>

#include <tc/preferences.h>

#include "misc.hxx"
#include "process_error.hxx"
#include "pm8_get_source_value.hxx"

using namespace std;

int pm8_get_source_value(METHOD_message_t *mess, va_list args) {
	try {
		va_list vargs;
		va_copy(vargs, args);
		tag_t prop_tag = va_arg(vargs, tag_t);
		char **rvalue = va_arg(vargs, char**);
		va_end(vargs);
		char* result;
		ResultCheck erc;

		// Получаем таг_т для всех используемых в программе отношений
		tag_t Pm8_DesignationRel_rel_tag = NULL_TAG;
		erc = GRM_find_relation_type("Pm8_DesignationRel",
				&Pm8_DesignationRel_rel_tag);
		tag_t Pm8_SourceRel_rel_tag = NULL_TAG;
		erc = GRM_find_relation_type("Pm8_SourceRel",
		&Pm8_SourceRel_rel_tag);

		// Получаем таг_т для всех используемых в программе типов объектов
		tag_t Pm8_CompanyPart_type_tag = NULL_TAG;
		erc = TCTYPE_find_type("Pm8_CompanyPartRevision", NULL,
				&Pm8_CompanyPart_type_tag);
		tag_t CommercialPart_type_tag = NULL_TAG;
		erc = TCTYPE_find_type("CommercialPart Revision", NULL,
				&CommercialPart_type_tag);
		tag_t Pm8_Material_type_tag = NULL_TAG;
		erc = TCTYPE_find_type("Pm8_MaterialRevision", NULL, &Pm8_Material_type_tag);
		tag_t Pm8_DesignForm_type_tag = NULL_TAG;
		erc = TCTYPE_find_type("Pm8_DesignForm", NULL, &Pm8_DesignForm_type_tag);

		// Получаем связанный отношением Pm8_SourceRel объект
		tag_t ref_source = NULLTAG;
		int num_rels = 0;
		auto_itk_mem_free<tag_t> source_rels;
		erc = GRM_list_secondary_objects_only(mess->object_tag,
				Pm8_SourceRel_rel_tag, &num_rels,
				source_rels.operator tag_t **());
		if (num_rels > 0) {
			ref_source = source_rels.operator tag_t *()[0];
		}

		if (ref_source != NULLTAG) {
			tag_t child_type_tag = NULL_TAG;
			erc = TCTYPE_ask_object_type(ref_source, &child_type_tag);
			//check if we get BO Item or it's descendant type
			logical is_item_type_child = false;
			erc = TCTYPE_is_type_of(child_type_tag, Pm8_CompanyPart_type_tag,
					&is_item_type_child);
			if (is_item_type_child) {
				// Pm8_CompanyPart
				tag_t designForm;
				int num_forms = 0;
				auto_itk_mem_free<tag_t> form_rels;
				erc = GRM_list_secondary_objects_only(ref_source,
						Pm8_DesignationRel_rel_tag, &num_forms,
						form_rels.operator tag_t **());
				if (num_forms > 0) {
					tag_t child_form_tag = form_rels.operator tag_t *()[0];
					tag_t child_form_type_tag = NULL_TAG;
					erc = TCTYPE_ask_object_type(child_form_tag,
							&child_form_type_tag);
					//check if we get BO Item or it's descendant type
					logical is_form_type_child = false;
					erc = TCTYPE_is_type_of(child_form_type_tag,
							Pm8_DesignForm_type_tag, &is_form_type_child);
					if (is_form_type_child) {
						erc = AOM_ask_value_string(child_form_tag,
								"pm8_Designation", &result);
						*rvalue = (char*) MEM_alloc(strlen(result) + 1);
						strcpy(*rvalue, result);
						return ITK_ok;
					}
				}
			}
			erc = TCTYPE_is_type_of(child_type_tag, CommercialPart_type_tag,
					&is_item_type_child);
			if (is_item_type_child) {
				// CommercialPart
				erc = AOM_ask_value_string(ref_source, "object_name", &result);
				*rvalue = (char*) MEM_alloc(strlen(result) + 1);
				strcpy(*rvalue, result);
				return ITK_ok;
			}
			erc = TCTYPE_is_type_of(child_type_tag, Pm8_Material_type_tag,
					&is_item_type_child);
			if (is_item_type_child) {
				// Pm8_Material
				erc = AOM_ask_value_string(ref_source, "object_name", &result);
				*rvalue = (char*) MEM_alloc(strlen(result) + 1);
				strcpy(*rvalue, result);
				return ITK_ok;
			}
		} else {
			erc = AOM_ask_value_string(mess->object_tag, "pm8_CADMaterial", &result);
			*rvalue = (char*) MEM_alloc(strlen(result) + 1);
			strcpy(*rvalue, result);
			return ITK_ok;
		}
	} catch (...) {
		return sisw::process_error(true, true, false);
	}
	return ITK_ok;
}
