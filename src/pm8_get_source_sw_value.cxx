/*
 * pm8_get_source_sw_value.cxx
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

#include "misc.hxx"
#include "process_error.hxx"
#include "pm8_get_source_sw_value.hxx"

int pm8_get_source_sw_value(METHOD_message_t *mess, va_list args) {
	try {
		tag_t prop_tag;
		va_list vargs;
		va_copy(vargs, args);
		char **rvalue;
		char* result;
		prop_tag = va_arg(vargs, tag_t);
		rvalue = va_arg(vargs, char**);
		va_end(vargs);
		result = "";

		// Получаем таг_т для всех используемых в программе отношений
		//std::cout << "Getting tag_t's for relations" << std::endl;
		/*tag_t Pm8_DocRel_rel_tag = NULL_TAG;
		 ResultCheck erc = GRM_find_relation_type("Pm8_DocRel",
		 &Pm8_DocRel_rel_tag);
		 std::cout << "Done for docrel" << std::endl;*/
		tag_t Pm8_DesignationRel_rel_tag = NULL_TAG;
		ResultCheck erc = GRM_find_relation_type("Pm8_DesignationRel",
				&Pm8_DesignationRel_rel_tag);
		//std::cout << "Done for designationrel" << std::endl;
		tag_t Pm8_SourceRel_rel_tag = NULL_TAG;
		 erc = GRM_find_relation_type("Pm8_SourceRel",
		 &Pm8_SourceRel_rel_tag);
		 //std::cout << "Done for sourcerel" << std::endl;
		// Получаем таг_т для всех используемых в программе типов объектов
		//std::cout << "Getting tagt's for items" << std::endl;
		tag_t Pm8_CompanyPart_type_tag = NULL_TAG;
		erc = TCTYPE_find_type("Pm8_CompanyPartRevision", NULL,
				&Pm8_CompanyPart_type_tag);
		//std::cout << "Done for companypart" << std::endl;
		tag_t CommercialPart_type_tag = NULL_TAG;
		erc = TCTYPE_find_type("CommercialPart Revision", NULL,
				&CommercialPart_type_tag);
		//std::cout << "Done for commercial" << std::endl;
		tag_t Pm8_Material_type_tag = NULL_TAG;
		erc = TCTYPE_find_type("Pm8_MaterialRevision", NULL, &Pm8_Material_type_tag);
		tag_t Pm8_DesignForm_type_tag = NULL_TAG;
		erc = TCTYPE_find_type("Pm8_DesignForm", NULL, &Pm8_DesignForm_type_tag);
		//std::cout << "Done for material" << std::endl;
		//TODO
		date_s creation_date;
		tag_t creation_date_tag;
		puts("SHIT");
		erc = AOM_ask_value_date(mess->object_tag, "creation_date", &creation_date);
		printf("%d.%d.%d _ %d:%d:%d", creation_date.year, creation_date.month, creation_date.day, creation_date.hour, creation_date.minute, creation_date.second);
		// Получаем связанный отношением Pm8_SourceRel объект
		tag_t ref_source = NULLTAG;
		//std::cout << "Getting related value" << std::endl;
		int num_rels = 0;
		auto_itk_mem_free<tag_t> source_rels;
		erc = GRM_list_secondary_objects_only(mess->object_tag,
				Pm8_SourceRel_rel_tag, &num_rels,
				source_rels.operator tag_t **());
		if (num_rels > 0) {
			ref_source = source_rels.operator tag_t *()[0];
		}
		//erc = AOM_ask_value_tag(mess->object_tag, "pm8_Source", &ref_source);
		//std::cout << "Got related?" << std::endl;
		//
		if (ref_source != NULLTAG) {
			//std::cout << "Got " << "FOUND YA!" << " related things"
			//		<< std::endl;
			tag_t child_type_tag = NULL_TAG;
			erc = TCTYPE_ask_object_type(ref_source, &child_type_tag);
			//check if we get BO Item or it's descendant type
			logical is_item_type_child = false;
			erc = TCTYPE_is_type_of(child_type_tag, Pm8_CompanyPart_type_tag,
					&is_item_type_child);
			if (is_item_type_child) {
				// Pm8_CompanyPart
				//std::cout << "Got company part" << std::endl;
				tag_t designForm;
				int num_forms = 0;
				auto_itk_mem_free<tag_t> form_rels;
				erc = GRM_list_secondary_objects_only(ref_source,
						Pm8_DesignationRel_rel_tag, &num_forms,
						form_rels.operator tag_t **());
				if (num_forms > 0) {
					//std::cout << "Found by Pm8_DesignationRel" << std::endl;
					tag_t child_form_tag = form_rels.operator tag_t *()[0];
					tag_t child_form_type_tag = NULL_TAG;
					erc = TCTYPE_ask_object_type(child_form_tag,
							&child_form_type_tag);
					//check if we get BO Item or it's descendant type
					logical is_form_type_child = false;
					erc = TCTYPE_is_type_of(child_form_type_tag,
							Pm8_DesignForm_type_tag, &is_form_type_child);
					if (is_form_type_child) {
						//std::cout << "Found DesignForm" << std::endl;
						erc = AOM_ask_value_string(child_form_tag,
								"pm8_Designation", &result);
						//result = "Изделие-заготовка<stack>\n" + result + "</stack>";
						char trueresult[194];
						//<STACK size=1 divider=no>Изделие-заготовка<OVER>
						strcpy(trueresult,"<STACK size=1 divider=no>Изделие-заготовка<OVER>");
						strcat(trueresult, result);
						strcat(trueresult, "</STACK>");
						*rvalue = (char*) MEM_alloc(strlen(trueresult) + 1);
						strcpy(*rvalue, trueresult);
						return ITK_ok;
					}
				}
				///*rvalue = "Pm8_CompanyPart";
			}
			erc = TCTYPE_is_type_of(child_type_tag, CommercialPart_type_tag,
					&is_item_type_child);
			if (is_item_type_child) {
				// CommercialPart
				//std::cout << "Got commercial part" << std::endl;
				erc = AOM_ask_value_string(ref_source, "object_name", &result);
				char trueresult[194];
				strcpy(trueresult,"<STACK size=1 divider=no>Изделие-заготовка<OVER>");
				strcat(trueresult, result);
				strcat(trueresult, "</STACK>");
				*rvalue = (char*) MEM_alloc(strlen(trueresult) + 1);
				strcpy(*rvalue, trueresult);
				return ITK_ok;
			}
			erc = TCTYPE_is_type_of(child_type_tag, Pm8_Material_type_tag,
					&is_item_type_child);
			if (is_item_type_child) {
				// Pm8_Material
				//std::cout << "Got material" << std::endl;
				erc = AOM_ask_value_string(ref_source, "object_name", &result);
				*rvalue = (char*) MEM_alloc(strlen(result) + 1);
				strcpy(*rvalue, result);
				return ITK_ok;
			}
		} else {
			//std::cout << "Got zero related things" << std::endl;
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
