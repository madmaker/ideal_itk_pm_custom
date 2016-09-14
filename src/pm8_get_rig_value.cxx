/*
 * pm8_get_source_value.cxx
 *
 *  Created on: Apr 29, 2015
 *      Author: infodba
 */
#include <iostream>
#include <string>
#include <stdarg.h>
#include <iterator>

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
#include "pm8_get_rig_value.hxx"

using namespace std;

int pm8_get_rig_value(METHOD_message_t *mess, va_list args) {
	try {
		cout << "INTO GET_RIG_VALUE" << endl;
		va_list vargs;
		va_copy(vargs, args);
		tag_t	tProp=va_arg (vargs, tag_t);
		int *count = va_arg (vargs, int * );
		tag_t ** rvalue = va_arg (vargs, tag_t **);
		va_end(vargs);

		//initial zeroing
		*count = 0;
		*rvalue = NULL;

		// Получаем таг_т для всех используемых в программе отношений
		tag_t Pm8_RigForRel_rel_tag = NULL_TAG;
		ResultCheck erc = GRM_find_relation_type("Pm8_RigForRel",
				&Pm8_RigForRel_rel_tag);
		// Получаем таг_т для всех используемых в программе типов объектов
		tag_t Pm8_PartRev_type_tag = NULL_TAG;
		erc = TCTYPE_find_type("Pm8_PartRevision", NULL,
				&Pm8_PartRev_type_tag);
		// Получаем связанный отношением Pm8_SourceRel объект
		tag_t ref_rig = NULLTAG;
		int num_rels = 0;
		auto_itk_mem_free<tag_t> rig_rels;
		erc = GRM_list_primary_objects_only(mess->object_tag,
				Pm8_RigForRel_rel_tag, &num_rels,
				rig_rels.operator tag_t **());
		if (num_rels > 0) {
			*count = num_rels;
			*rvalue = static_cast <tag_t*> ( MEM_alloc(num_rels * sizeof(tag_t)) );
			for (int ii = 0; ii < num_rels; ii++)
			{
				(*rvalue)[ii] = rig_rels.operator tag_t *()[ii];
			}
		}
		cout << "LEFT GET_RIG_VALUE" << endl;
	} catch (...) {
		return sisw::process_error(true, true, false);
	}
	return ITK_ok;
}
