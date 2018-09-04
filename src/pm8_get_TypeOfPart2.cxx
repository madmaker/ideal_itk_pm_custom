#include <iostream>
#include <string>
#include <vector>
#include <stdarg.h>
#include <stdlib.h>

#include <tc/tc.h>
#include <tccore/aom.h>
#include <tccore/aom_prop.h>
#include <ug_va_copy.h>

#include <base_utils/ResultCheck.hxx>
#include "misc.hxx"
#include "process_error.hxx"
#include "pm8_get_TypeOfPart2.hxx"

int pm8_get_TypeOfPart2(METHOD_message_t *mess, va_list args){
	try{
		printf("%s\n", "Into pm8_get_TypeOfPart2");
		tag_t prop_tag;
		//char **rvalue;

		va_list vargs;
		va_copy(vargs, args);
		prop_tag = va_arg(vargs, tag_t);
		char **rvalue = va_arg(vargs, char**);
		char* result;
		va_end(vargs);
		printf("%s\n", "End of vargs");

		//tag_t object_t = mess->object_tag;
		//char* typeOfPart = NULL;
		//char* result;
		//ResultCheck erc;

		/*printf("%s\n", "Asking type of part");
		erc = AOM_ask_value_string(object_t, "oc9_TypeOfPart", &typeOfPart);
		printf("Found [%s]\n", typeOfPart);
		if(typeOfPart!=NULL){*/
		printf("%s\n", "Allocating...");
		result = (char*) MEM_alloc(strlen("TEST") + 1);
		printf("%s\n", "Copying...");
		strcpy(result, "TEST");
		printf("%s\n", "Allocating...");
		*rvalue = (char*) MEM_alloc(strlen(result) + 1);
		printf("%s\n", "Copying...");
		strcpy(*rvalue, result);
			/*if(strcmp(typeOfPart, "Стандартное изделие") == 0){
				*rvalue = (char*) MEM_alloc(strlen("Standart") + 1);
				strcpy(*rvalue, "Standart");
			} else if(strcmp(typeOfPart, "Прочее изделие") == 0) {
				*rvalue = (char*) MEM_alloc(strlen("Other") + 1);
				strcpy(*rvalue, "Other");
			} else if(strcmp(typeOfPart, "Деталь") == 0){
				*rvalue = (char*) MEM_alloc(strlen("Part") + 1);
				strcpy(*rvalue, "Part");
			} else if(strcmp(typeOfPart, "Сборочная единица") == 0){
				*rvalue = (char*) MEM_alloc(strlen("Assembly") + 1);
				strcpy(*rvalue, "Assembly");
			} else if(strcmp(typeOfPart, "Комплект") == 0){
				*rvalue = (char*) MEM_alloc(strlen("Set") + 1);
				strcpy(*rvalue, "Set");
			} else if(strcmp(typeOfPart, "Комплекс") == 0){
				*rvalue = (char*) MEM_alloc(strlen("Complex") + 1);
				strcpy(*rvalue, "Complex");
			} else if(strcmp(typeOfPart, "Геометрия материала") == 0){
				*rvalue = (char*) MEM_alloc(strlen("GeomOfMat") + 1);
				strcpy(*rvalue, "GeomOfMat");
			} else if(strcmp(typeOfPart, "Материал") == 0){
				*rvalue = (char*) MEM_alloc(strlen("Material") + 1);
				strcpy(*rvalue, "Material");
			}*/
		/*	printf("%s\n", "Freeing memory");
			MEM_free(typeOfPart);
			printf("%s\n", "Done...");
		}
		return ITK_ok;*/
		MEM_free(result);
		printf("%s\n", "Done...");

	} catch (...) {
		return sisw::process_error(true, true, false);
	}
	return ITK_ok;
}
