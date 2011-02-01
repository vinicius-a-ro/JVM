/*
 * classViewer.c
 *
 *  Created on: 05/01/2011
 *      Author: joaofreitas
 */

#include "classViewer.h"

void inspectClassFile(classFileFormat *classFile) {

	printf("-------------- CLASS FILE -----------------\n");
	printGeneralInformation(classFile);
	printConstantPool(classFile);
	printInterfaces(classFile);
	printFields(classFile);
	printMethods(classFile);
	printAttributes(classFile);
}

void printGeneralInformation(classFileFormat *classFile) {
	printf("Magic Number: %X\n", classFile->magic);
	printf("Minor Version: %d\n", classFile->minor_version);
	printf("Major Version: %d\n", classFile->major_version);
	printf("Constant Pool Count: %d\n", classFile->constant_pool_count);
	printf("Access Flags: 0x%.6X\n", classFile->access_flags);
	printf("This class: %d\n", classFile->this_class);
	printf("Super class: %d\n", classFile->super_class);
	printf("Interface Count: %d\n", classFile->interfaces_count);
	printf("Fields Count: %d\n", classFile->fields_count);
	printf("Methods Count: %d\n", classFile->methods_count);
	printf("Attributes count: %d\n", classFile->attributes_count);
}

void printConstantPool(classFileFormat *classFile) {
	int cp_size, i=1, tag, string_length;
	cp_info *constant_pool;
	cp_size = classFile->constant_pool_count;

	printf("----------- CONSTANT POOL -------------\n");
	for (constant_pool = classFile->constant_pool; constant_pool < classFile->constant_pool + cp_size - 1; constant_pool++){
		tag = constant_pool->tag;
		printf("\t[%d] ", i);
		i++;
		switch(tag) {
			case 1:
				string_length = constant_pool->constant_union.c_utf8.length;
				printf("Length %d. String: ", string_length);
				printf("%s\n", constant_pool->constant_union.c_utf8.bytes);
				break;
			case 3:
				printf("Value: %d\n", constant_pool->constant_union.c_integer.bytes);
				break;
			case 4:
				printf("Value: %d\n", constant_pool->constant_union.c_float.bytes );
				break;
			case 5:
				printf("High Bytes: 0x%.5X. Low Bytes 0x%.5X\n", constant_pool->constant_union.c_long.high_bytes, constant_pool->constant_union.c_long.low_bytes );
				break;
			case 6:
				printf("High Bytes: 0z%X. Low Bytes 0x%X\n", constant_pool->constant_union.c_double.high_bytes, constant_pool->constant_union.c_double.low_bytes);
				break;
			case 7:
				printf("Class index: #%d\n", constant_pool->constant_union.c_class.name_index);
				break;
			case 8:
				printf("String index: #%d\n", constant_pool->constant_union.c_string.string_index);
				break;
			case 9:
				printf("FieldRef class index: %d. Fieldref name and type index: %d\n", constant_pool->constant_union.c_fieldref.class_index, constant_pool->constant_union.c_fieldref.name_and_type_index);
				break;
			case 10:
				printf("Metodref class index: %d. Metodref name and type index: %d\n", constant_pool->constant_union.c_methodref.class_index, constant_pool->constant_union.c_methodref.name_and_type_index);
				break;
			case 11:
				printf("Interface class index: %d. Interface name and type index: %d\n", constant_pool->constant_union.c_interface_methodref.class_index, constant_pool->constant_union.c_interface_methodref.name_and_type_index);
				break;
			case 12:
				printf("Nametype class index: %d. Nametype name and type index: %d\n", constant_pool->constant_union.c_nametype.name_index, constant_pool->constant_union.c_nametype.descriptor_index);
				break;
			case 13:
				printf("(large numeric continued)\n");
				break;
			default:
				break;
		}
		fflush(stdout);
	}
}

void printInterfaces(classFileFormat *classFile) {
/*TODO fazer método para imprimir interfaces*/
}

void printFields(classFileFormat *classFile) {
	field_info *field;
	cp_info element;
	char *formatacao = "\t\t";
	int field_size, index = 0, i;

	printf("----------- FIELDS -------------\n");
	field_size = classFile->fields_count;
	for (field = classFile->fields; field < classFile->fields+ field_size; field++) {
		element = getConstantPoolElementByIndex(classFile, field->name_index);
		printf("\t-----------> [%d] %s -----------\n", index, element.constant_union.c_utf8.bytes);

		printf("\tName index: %d\n", field->name_index);
		printf("\tDescriptor index: %d\n", field->descriptor_index);
		printf("\tAccess Flags: 0x%x\n", field->access_flags);
		printf("\tAttributes count: %d\n", field->attributes_count);
		for (i = 0; i < field->attributes_count; i++) {
			printAttribute(classFile, field->attributes[i], formatacao, i);
		}
		index++;
	}
}

void printMethods(classFileFormat *classFile) {
	method_info *method;
	cp_info element;
	char *formatacao = "\t\t";
	int method_size, attribute_size, i, index = 0;

	method_size = classFile->methods_count;
	printf("----------- METHODS -------------\n");
	for (method = classFile->methods; method < classFile->methods + method_size; method++) {
		attribute_size = method->attributes_count;
		element = getConstantPoolElementByIndex(classFile, method->name_index);

		printf("\t-----------> [%d] %s -----------\n", index, element.constant_union.c_utf8.bytes);
		printf("\tName index: %d\n",  method->name_index);
		printf("\tDescriptor index: %d\n", method->descriptor_index);
		printf("\tAccess Flag: %d\n", method->access_flags);
		printf("\tAttributes count: %d\n", attribute_size);
		printf("\tAttributes:\n");
		for (i = 0; i < attribute_size; i++) {
			printAttribute(classFile, method->attributes[i], formatacao, i);
			fflush(stdout);
		}
		printf("\n");
		index++;
	}
}

void printAttributes(classFileFormat *classFile) {
	int attribute_size, i;
	char *formatacao = "\t";

	printf("----------- ATTRIBUTES -------------\n");
	attribute_size = classFile->attributes_count;
	for (i = 0; i < attribute_size; i++) {
		printAttribute(classFile, classFile->attributes[i], formatacao, i);
		fflush(stdout);
	}
}

int printLookUpSwitch(attribute_info attribute, int index, opcode_info *op_info) {
	u1 code;
	int default_byte, number_pairs, match, offset, i, j;

	code = attribute.attribute_union.code.code[index];
	index++;

	while (index %4 !=0){
		index++;
	}

	default_byte = 0;
	for(i = 0; i < 3; i++) {
		default_byte |=  attribute.attribute_union.code.code[index];
		default_byte = default_byte << 8;
		index++;
	}

	default_byte |= attribute.attribute_union.code.code[index];
	index++;

	number_pairs = 0;
	for(i = 0; i < 3; i++)
	{
		number_pairs |= attribute.attribute_union.code.code[index];
		number_pairs = number_pairs << 8;
		index++;
	}

	number_pairs |= attribute.attribute_union.code.code[index];
	index++;
	printf("%s %d\n", op_info[code].mnemonic, number_pairs);
	fflush(stdout);

	for(i = 0; i < number_pairs; i++) {
		match = 0;

		for(j = 0; j < 3; j++) {
			match |= attribute.attribute_union.code.code[index];
			match = match << 8;
			index++;
		}

		match |= attribute.attribute_union.code.code[index];
		index++;
		printf("%d: ", match);
		fflush(stdout);
		offset = 0;

		for(j = 0; j < 3; j++) {
			offset |= attribute.attribute_union.code.code[index];
			offset = offset << 8;
			index++;
		}

		offset |= attribute.attribute_union.code.code[index];
		index++;
		printf("%d (+%d)\n", offset+3, offset);
		fflush(stdout);
	}
	printf("default: %d (+%d)\n", default_byte+3, default_byte);
	fflush(stdout);
	return index-1;

}

int printTableSwitch(attribute_info attribute, int index, opcode_info *op_info) {
	u1 code;
	int default_value, low_value, high_value, offsets_count, value, i, j;

	code = attribute.attribute_union.code.code[index];
	printf("%s", op_info[code].mnemonic);
	index++;

	while (index %4 !=0){
		index++;
	}

	default_value = 0;
	for(i = 0; i < 3; i++) {
		default_value |=  attribute.attribute_union.code.code[index];
		default_value = default_value << 8;
		index++;
	}

	default_value |= attribute.attribute_union.code.code[index];
	index++;

	low_value = 0;
	for(i = 0; i < 3; i++) {
		low_value |=  attribute.attribute_union.code.code[index];
		low_value = low_value << 8;
		index++;
	}

	low_value |= attribute.attribute_union.code.code[index];
	index++;

	high_value = 0;
	for(i = 0; i < 3; i++) {
		high_value |=  attribute.attribute_union.code.code[index];
		high_value = high_value << 8;
		index++;
	}

	high_value |= attribute.attribute_union.code.code[index];
	index++;

	offsets_count = (high_value-low_value+1);


	printf(" %d to %d\n", low_value, high_value);
	fflush(stdout);
	for(i = low_value; i <= high_value; i++) {
		value = 0;
		for (j = 0; j < 3; j++) {
			value |=  attribute.attribute_union.code.code[index];
			value = value << 8;
			index++;
		}
		value |= attribute.attribute_union.code.code[index];
		index++;
		printf("%d: %d (+%d)\n", i, value+4, value);
		fflush(stdout);
	}

	printf("default: %d (+%d)\n", default_value+3, default_value);
	fflush(stdout);
	return index-1;

}

void printAttribute(classFileFormat *classFile, attribute_info attribute, char *format,int index) {
	cp_info cp_element;
	opcode_info *op_info;
	class_member *cm;
	int tag, i;


	cp_element = getConstantPoolElementByIndex(classFile, attribute.attribute_name_index);

	printf("%s[%d] %s\n", format, index, cp_element.constant_union.c_utf8.bytes);
	printf("%sAttribute name index %d\n", format, attribute.attribute_name_index);
	printf("%sAttribute length %d\n", format, attribute.attribute_length);
	fflush(stdout);
	tag = attribute.tag;

	switch(tag) {
		case 1:
			printf("%sConstant Value index: %d\n", format, attribute.attribute_union.constant_value.constant_value_index);
			break;
		case 2:
			printf("%sMaximum stack depth: %d\n", format, attribute.attribute_union.code.max_stack);
			printf("%sMaximum local variables: %d\n", format, attribute.attribute_union.code.max_locals);
			printf("%sCode Length: %d\n", format, attribute.attribute_union.code.code_length);
			fflush(stdout);
			op_info = get_opcode_info();
			for (i=0 ; i< attribute.attribute_union.code.code_length; i++) {
				fflush(stdout);

				u1 code = attribute.attribute_union.code.code[i];
				if (code == 0xaa) {
					i = printTableSwitch(attribute, i, op_info);
				} else if (code == 0xab) {
					i = printLookUpSwitch(attribute, i, op_info);
				} else {
					printf("%s\n", op_info[code].mnemonic);
				}
				/*fflush(stdout);*/
			}
			break;
		case 3:
			printf("%sNumber of exceptions: %d\n", format, attribute.attribute_union.exceptions.number_of_exceptions);
			for (i=0; i<attribute.attribute_union.exceptions.number_of_exceptions; i++) {
				printf("Exception index table: %d\n", attribute.attribute_union.exceptions.exception_index_table[i]);
			}
			fflush(stdout);
			break;
		case 4:
			printf("%sException index table: %d\n", format, attribute.attribute_union.inner_classes.number_of_classes);
			for (cm = attribute.attribute_union.inner_classes.classes; cm < attribute.attribute_union.inner_classes.classes + attribute.attribute_union.inner_classes.number_of_classes; cm++) {
				printf("%s\tInner class access flags: %d\n", format, cm->inner_class_access_flags);
				printf("%s\tInner class info index: %d\n", format, cm->inner_class_info_index);
				printf("%s\tInner name index: %d\n", format, cm->inner_name_index);
				printf("%s\tOuter class info index: %d\n", format, cm->outer_class_info_index);
				fflush(stdout);
			}
			break;
		case 5:
			printf("%sSynthetic attribute\n", format);
			fflush(stdout);
			break;
		case 6:
			printf("%sAtributo da jvm ignorado por ser opcional\n", format);
			fflush(stdout);
			break;
		default:
			break;
	}

}

