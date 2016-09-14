/*
 * pm8_get_source_value.cxx
 *
 *  Created on: Apr 29, 2015
 *      Author: infodba
 */
#include <iostream>
#include <string>
#include <vector>
#include <stdarg.h>
#include <stdlib.h>

#include <base_utils/ResultCheck.hxx>
#include <tc/tc.h>
#include <tc/preferences.h>
#include <tccore/grm.h>
#include <tccore/method.h>
#include <tccore/tctype.h>
#include <tccore/aom_prop.h>
#include <sa/sa.h>
#include <tccore/aom.h>
#include <ug_va_copy.h>

#include "misc.hxx"
#include "process_error.hxx"
#include "pm8_get_document_type2_value.hxx"

#define NUM_ENTRIES 1

using namespace std;

int getDocumentType(vector<char*>, string);
char* getSymbolPart(string);

int pm8_get_document_type2_value(METHOD_message_t *mess, va_list args) {
	try {
		va_list vargs;
		va_copy(vargs, args);
		tag_t prop_tag = va_arg(vargs, tag_t);
		char **rvalue = va_arg(vargs, char**);
		va_end(vargs);

		ResultCheck erc;
		char* result = NULL;

		char* item_id;
		string item_id_short;
		std::vector<char*> shortTypes;
		std::vector<char*> longTypes;

		erc = AOM_ask_value_string(mess->object_tag, "item_id", &item_id);

		string item_id_string(item_id);
		size_t posOfSpace = item_id_string.find_last_of(" ");
		if (posOfSpace != string::npos) {
			int n_values = 0;
			char **values = NULL;
			PREF_initialize();
			erc = PREF_ask_char_values("Pm8_DocumentTypes", &n_values, &values);
			printf("\t n_values: %d \n", n_values);

			if (n_values > 0) {
				char* splitedString;
				for (int ii = 0; ii < n_values; ii++) {
					splitedString = strtok(values[ii], "=");
					shortTypes.push_back(splitedString);
					splitedString = strtok(NULL, "=");
					longTypes.push_back(splitedString);
				}

				MEM_free(values);
				values = NULL;

				item_id_short = item_id_string.substr(posOfSpace + 1,
						item_id_string.length());
				int pos = getDocumentType(shortTypes, item_id_short);
				if (pos != -1) {
					result = (char*) MEM_alloc(strlen(longTypes.at(pos)) + 1);
					strcpy(result, longTypes.at(pos));
				} else {
					// Тип не определен
					result = (char*) MEM_alloc(strlen("Тип не определен") + 1);
					strcpy(result, "Тип не определен");
				}
			} else {
				// Тип не определен
				result = (char*) MEM_alloc(strlen("Тип не определен") + 1);
				strcpy(result, "Тип не определен");
				MEM_free(values);
				values = NULL;
				return ITK_ok;
			}

		} else {
			int entryCount;
			char** entryNames;
			char** entryValues;
			tag_t query_t = NULL_TAG;
			int resultCount;
			char entryNamesArray[NUM_ENTRIES][QRY_uid_name_size_c + 1] = {
					"Обозначение" };

			entryNames = (char**) MEM_alloc( NUM_ENTRIES * sizeof *entryNames);
			for (int index = 0; index < NUM_ENTRIES; index++) {
				entryNames[index] = (char*) MEM_alloc(
						strlen(entryNamesArray[index] + 1));
				if (entryNames[index])
					strcpy(entryNames[index], entryNamesArray[index]);
			}

			entryValues = (char**) MEM_alloc(NUM_ENTRIES * sizeof(char *));
			entryValues[0] = (char *) MEM_alloc(strlen(item_id) + 1);
			strcpy(entryValues[0], item_id);
			//MEM_free(item_id);

			auto_itk_mem_free<tag_t> results_found;
			erc = QRY_find("idealplm_comppart", &query_t);
			if (query_t != NULL_TAG) {
				std::cout << "Query \"idealplm_comppart\" not found!" << std::endl;
				erc = QRY_execute(query_t, NUM_ENTRIES, entryNames, entryValues,
						&resultCount, results_found.operator tag_t **());

				if (resultCount != 0) {
					// Берём первый попавшийся объект
					tag_t found_kit = results_found.operator tag_t *()[0];

					char* typeOfPart;
					if (found_kit != NULL_TAG) {
						erc = AOM_ask_value_string(found_kit, "pm8_TypeOfPart",
								&typeOfPart);
						if (strcmp("", typeOfPart) == 0 || strcmp("GeomOfMaterial", typeOfPart) == 0) {
							result = (char*) MEM_alloc(
									strlen("Тип не определен") + 1);
							strcpy(result, "Тип не определен");
						} else if (strcmp("Part", typeOfPart) == 0) {
							result = (char*) MEM_alloc(
									strlen("") + 1);
							strcpy(result, "");
						} else {
							result = (char*) MEM_alloc(
									strlen("Спецификация") + 1);
							strcpy(result, "Спецификация");
						}
					}
				}

			}
			MEM_free(entryNames[0]);
			MEM_free(entryNames);
			MEM_free(entryValues[0]);
			MEM_free(entryValues);
		}

		//free(item_id);
		if (result) {
			*rvalue = (char*) MEM_alloc(strlen(result) + 1);
			strcpy(*rvalue, result);
			//free(result);
		} else {
			*rvalue = (char*) MEM_alloc(strlen("") + 1);
			strcpy(*rvalue, "");
		}

		return ITK_ok;

	} catch (...) {
		return sisw::process_error(true, true, false);
	}
	return ITK_ok;
}

int getDocumentType(vector<char*> typesShort, string idShort) {
	int result = -1;
	int pos = 0;
	char* symbolPart = getSymbolPart(idShort);
	cout << "Определяем тип документа" << endl;
	for (std::vector<char*>::iterator it = typesShort.begin();
			it != typesShort.end(); ++it) {
		if (strcmp((*it), idShort.c_str()) == 0
				&& (strlen(*it) == strlen(idShort.c_str()))) {
			result = pos;
			break;
		} else if (strcmp((*it), symbolPart) == 0
				&& (strlen(*it) != strlen(idShort.c_str()))) {
			result = pos;
		} else if (strcmp((*it), symbolPart) == 0
				&& (strlen(*it) == strlen(idShort.c_str()))) {
			result = pos;
			break;
		}
		pos++;
	}
	MEM_free(symbolPart);
	cout << "Возвращаем значение" << endl;
	return result;
}

char* getSymbolPart(string type) {
	char* stringg = (char*) MEM_alloc(3);
	int character;
	for (int i = 0; i < type.length() && i < 3; i++) {
		character = type[i];
		if ((character >= 'А' && character <= 'Я')
				|| (character >= 'а' && character <= 'я')) {
			stringg[i] = character;
		} else {
			stringg[i] = '\0';
			break;
		}
	}
	return stringg;
}
