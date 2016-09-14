/*
 * pm8_get_source_value.cxx
 *
 *  Created on: Apr 29, 2015
 *      Author: infodba
 */
#include <iostream>
#include <string>
#include <stdio.h>
#include <stdarg.h>
#include <iterator>

#include <base_utils/ResultCheck.hxx>
#include <tc/tc.h>
#include <tccore/grm.h>
#include <tccore/method.h>
#include <tccore/tctype.h>
#include <tccore/aom_prop.h>
#include <tc/preferences.h>
#include <sa/sa.h>
#include <tccore/aom.h>
#include <ug_va_copy.h>

#include "misc.hxx"
#include "process_error.hxx"
#include "pm8_get_mat_string_nx_value.hxx"

void removeMultipleSpaces(char**);
char* parse(char*);

int pm8_get_mat_string_nx_value(METHOD_message_t *mess, va_list args) {
	try {
		va_list vargs;
		va_copy(vargs, args);
		ResultCheck erc;
		char **rvalue;
		char* object_name;
		tag_t tProp = va_arg(vargs, tag_t);
		rvalue = va_arg(vargs, char**);
		va_end(vargs);

		tag_t Pm8_SourceRel_rel_t = NULL_TAG;
		tag_t Pm8_DesignationRel_rel_tag = NULL_TAG;
		tag_t Pm8_CompanyPartRev_type_t = NULL_TAG;
		tag_t CommercialPartRev_type_t = NULL_TAG;
		tag_t Pm8_MaterialRev_type_t = NULL_TAG;
		tag_t Pm8_DesignForm_type_tag = NULL_TAG;

		// Получаем таг_т для всех используемых в программе отношений
		erc = GRM_find_relation_type("Pm8_SourceRel", &Pm8_SourceRel_rel_t);
		erc = GRM_find_relation_type("Pm8_DesignationRel",
				&Pm8_DesignationRel_rel_tag);

		// Получаем таг_т для всех используемых в программе типов объектов
		erc = TCTYPE_find_type("Pm8_CompanyPartRevision", NULL,
				&Pm8_CompanyPartRev_type_t);
		erc = TCTYPE_find_type("CommercialPart Revision", NULL,
				&CommercialPartRev_type_t);
		erc = TCTYPE_find_type("Pm8_MaterialRevision", NULL,
				&Pm8_MaterialRev_type_t);
		erc = TCTYPE_find_type("Pm8_DesignForm", NULL,
				&Pm8_DesignForm_type_tag);

		// Получаем связанный отношением Pm8_SourceRel объект
		tag_t ref_source = NULLTAG;
		int num_rels = 0;
		auto_itk_mem_free<tag_t> source_rels;
		erc = GRM_list_secondary_objects_only(mess->object_tag,
				Pm8_SourceRel_rel_t, &num_rels,
				source_rels.operator tag_t **());

		if (num_rels > 0) {
			ref_source = source_rels.operator tag_t *()[0];
			if (ref_source != NULLTAG) {
				tag_t child_type_tag = NULL_TAG;
				erc = TCTYPE_ask_object_type(ref_source, &child_type_tag);
				//check if we get BO Item or it's descendant type

				logical is_company_part_type = false;
				logical is_commercial_part_type = false;
				logical is_material_type = false;
				erc = TCTYPE_is_type_of(child_type_tag,
						Pm8_CompanyPartRev_type_t, &is_company_part_type);
				erc = TCTYPE_is_type_of(child_type_tag,
						CommercialPartRev_type_t, &is_commercial_part_type);
				erc = TCTYPE_is_type_of(child_type_tag, Pm8_MaterialRev_type_t,
						&is_material_type);

				if (is_commercial_part_type) {
					erc = AOM_ask_value_string(ref_source, "object_name",
							&object_name);
					//<TИзделие-заготовка!CommercialPart Revision.object_name>
					*rvalue = (char*) MEM_alloc(
							strlen(object_name)
									+ strlen("<TИзделие-заготовка!>") + 1);
					strcpy(*rvalue, "<TИзделие-заготовка!");
					strcat(*rvalue, object_name);
					strcat(*rvalue, ">");
				} else if (is_company_part_type) {
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
						logical is_form_type_child = false;
						erc = TCTYPE_is_type_of(child_form_type_tag,
								Pm8_DesignForm_type_tag, &is_form_type_child);
						if (is_form_type_child) {
							erc = AOM_ask_value_string(child_form_tag,
									"pm8_Designation", &object_name);
						}
					}
					*rvalue = (char*) MEM_alloc(
							strlen(object_name)
									+ strlen("<TИзделие-заготовка!>") + 1);
					strcpy(*rvalue, "<TИзделие-заготовка!");
					strcat(*rvalue, object_name);
					strcat(*rvalue, ">");
				} else if (is_material_type) {
					erc = AOM_ask_value_string(ref_source, "object_name",
												&object_name);
					removeMultipleSpaces(&object_name);
					char* result = parse(object_name);
					MEM_free(object_name);
					*rvalue = (char*) MEM_alloc(strlen(result) + 1);
					strcpy(*rvalue, result);
					MEM_free(result);
				}
			}
		}
	} catch (...) {
		return sisw::process_error(true, true, false);
	}
	return ITK_ok;
}

void removeMultipleSpaces(char** input) {
	int i, j;
	int count = 1;
	j = 0;
	char* new_s = (char*) MEM_alloc(strlen(*input) + 1);
	for (i = 0; i < strlen(*input); i++) {
		if ((*input)[i] == ' ') {
			new_s[j] = (*input)[i];
			j++;
			while ((*input)[i + count] == ' ') {
				count++;
			}
			i += count - 1;
			count = 1;
		} else {
			new_s[j] = (*input)[i];
			j++;
		}
	}
	new_s[j] = '\0';
	*input = (char*) MEM_alloc(strlen(new_s));
	strcpy(*input, new_s);
}

char* parse(char* input) {
	int i;
	char* result;

	size_t currentMaxLength = 0;
	int preferencePos = -1;
	int currentLength = 0;
	int firstSpacePos = 0;
	int similarity = 0;

	int n_values;
	char **preference = NULL;
	PREF_initialize();
	PREF_ask_char_values("Pm8_SortamentTypes", &n_values, &preference);

	for (i = 0; i < n_values; i++) {
		int prefLength = strlen(preference[i]);
		int contains = !strncmp(input, preference[i], prefLength);
		if (currentMaxLength > prefLength) {
			break;
		}
		if ((currentMaxLength < prefLength) && contains) {
			currentMaxLength = prefLength;
			preferencePos = i;
		}
	}

	int hasDelimiter = (strchr(input, '/')!=NULL)?1:0;
	if (preferencePos != -1 && hasDelimiter) {
		char* temp = (char*) MEM_alloc(strlen(input)+1);
		strcpy(temp, input);
		result = (char*) MEM_alloc(strlen(input)+4+1); //reserved 4 chars for "<V", ">", " "

		strcpy(result, preference[preferencePos]);
		strcat(result, " <V");

		char* secondPart = strtok((temp+strlen(preference[preferencePos])+1),"/");
		strcat(result, secondPart);
		strcat(result, "!");
		strcat(result, (temp + strlen(preference[preferencePos]) + strlen(secondPart) + 2)); //incremented by 2 to skip a space and a slash

		strcat(result, ">");
		MEM_free(temp);
	} else {
		result = (char*) MEM_alloc(strlen(input)+1);
		strcpy(result, input);
	}
	MEM_free(preference);
	preference = NULL;
	return result;
}
