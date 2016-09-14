#include <stdlib.h>
#include <grm.h>
#include <epm.h>
#include <dataset.h>
#include <tccore/aom.h>
#include <tccore/aom_prop.h>
#include <epm_toolkit_tc_utils.h>
#include <base_utils/ResultCheck.hxx>
#include "process_error.hxx"
#include "misc.hxx"
#include "pm8_rename_namedrefs.hxx"

/*
 *
 */

int pm8_rename_namedrefs(EPM_action_message_t msg) {
	try {
		printf("%s\n", "+into oc9_setInvNo");
		tag_t *attachments, root_task, temp_type_t, parent_temp_type_t, dataset_type_t = NULLTAG, *namedrefs;
		int *attachments_types,
			attachments_count = 0,
			namedrefs_count,
			i = 0,
			j = 0;
		char* name;
		char* type_name;
		bool isDatasetType;
		ResultCheck erc;
		auto_itk_mem_free<tag_t> results_found;

		erc = TCTYPE_find_type("Dataset", NULL, &dataset_type_t);
		if(dataset_type_t==NULLTAG) {
			printf("%s\n", "Can't find Dataset type!");
			return ITK_ok;
		}

		erc = EPM_ask_root_task(msg.task, &root_task);
		erc = EPM_ask_all_attachments(root_task, &attachments_count, &attachments, &attachments_types);
		printf("attachments count=%d\n", attachments_count);
		for (i = 0; i < attachments_count; i++) {
			if (attachments_types[i] == EPM_target_attachment) {
				erc = TCTYPE_ask_object_type(attachments[i], &temp_type_t);
				erc = TCTYPE_ask_parent_type(temp_type_t, &parent_temp_type_t);
				erc = TCTYPE_is_type_of(dataset_type_t, parent_temp_type_t, &isDatasetType);
				if(isDatasetType){
					printf("%s\n", "Working with a dataset...");
					erc = AE_ask_dataset_named_refs(attachments[i], &namedrefs_count, &namedrefs);
					for(j = 0; j < namedrefs_count; j++){
						erc = AOM_ask_value_string(namedrefs[j], "original_file_name", &name);
						printf("Found name = %s\n", name);
						erc = AOM_refresh(namedrefs[j], TRUE);
						erc = IMF_set_original_file_name(namedrefs[j], "shit.txt");
						erc = AOM_save(namedrefs[j]);
						erc = AOM_refresh(namedrefs[j], FALSE);
						printf("Set name = %s\n", "shit");
					}
				}
			}
		}

		if(attachments) MEM_free(attachments);
		if(attachments_types) MEM_free(attachments_types);
		if(namedrefs) MEM_free(namedrefs);
		if(name) MEM_free(name);
		if(type_name) MEM_free(type_name);
	} catch (...) {
		return sisw::process_error(true, true, false);
	}
	return ITK_ok;
}
