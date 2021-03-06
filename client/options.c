#include "../common/ini_parser.h"
#include "../common/messages.h"
#include "../common/utils.h"
#include "options.h"

#include <stdio.h>
#include <string.h>


#define SERVER_HOST_SIZE 256
char server_host[SERVER_HOST_SIZE] = "192.168.0.101";
int server_port = 9312;

ExcCode load_server_host(const char *key, const char *value) {
	strncpy(server_host, value, SERVER_HOST_SIZE - 1);
	server_host[SERVER_HOST_SIZE - 1] = '\0';
	return 0;
}

ExcCode save_server_host(const char *key, char *buffer, int buffer_size) {
	strncpy(buffer, server_host, buffer_size - 1);
	buffer[buffer_size - 1] = '\0';
	return 0;
}

ExcCode load_server_port(const char *key, const char *value) {
	TRY(parse_int(key, value, PORT_MIN, PORT_MAX, &server_port));
	return 0;
}

ExcCode save_server_port(const char *key, char *buffer, int buffer_size) {
	snprintf(buffer, buffer_size, "%d", server_port);
	return 0;
}


int orientation = 0;

const char *orientation_captions[] = {"Portrait", "Landscape"};

#define ERR_INCORRECT_ORIENTATION "Incorrect orientation type"

ExcCode load_orientation(const char *key, const char *value) {
	for (int i = 0; i < 2; i++)
		if (!strcmp(value, orientation_captions[i])) {
			orientation = i;
			return 0;
		}
	PANIC(ERR_INCORRECT_ORIENTATION);
}

ExcCode save_orientation(const char *key, char *buffer, int buffer_size) {
	strncpy(buffer, orientation_captions[orientation], buffer_size - 1);
	buffer[buffer_size - 1] = '\0';
	return 0;
}


#define PASSWORD_DEFAULT "demo"
char password[PASSWORD_SIZE] = PASSWORD_DEFAULT;

ExcCode load_password(const char *key, const char *value) {
	strncpy(password, value, PASSWORD_SIZE - 1);
	password[PASSWORD_SIZE - 1] = '\0';
	return 0;
}

ExcCode save_password(const char *key, char *buffer, int buffer_size) {
	strncpy(buffer, password, buffer_size - 1);
	buffer[buffer_size - 1] = '\0';
	return 0;
}


int agreement_accepted = 0;

ExcCode load_agreement_accepted(const char *key, const char *value) {
	TRY(parse_bool(key, value, &agreement_accepted));
	return 0;
}

ExcCode save_agreement_accepted(
		const char *key, char *buffer, int buffer_size) {
	strncpy(buffer, agreement_accepted ? INI_VALUE_TRUE : INI_VALUE_FALSE,
			buffer_size - 1);
	buffer[buffer_size - 1] = 0;
	return 0;
}


const struct IniSection sections[] = {
	{"Server", (struct IniParam []) {
		{"Host", load_server_host, save_server_host, 0},
		{"Port", load_server_port, save_server_port, 0},
		{NULL}
	}},
	{"Security", (struct IniParam []) {
		{"Password", load_password, save_password, 0},
		{NULL}
	}},
	{"Client", (struct IniParam []) {
		{"Orientation", load_orientation, save_orientation, 0},
		{NULL}
	}},
	{"Agreement", (struct IniParam []) {
		{"Accepted", load_agreement_accepted, save_agreement_accepted, 0},
		{NULL}
	}},
	{NULL}
};


ExcCode options_config_load(const char *filename) {
	TRY(ini_load(filename, sections));
	return 0;
}

ExcCode options_config_save(const char *filename) {
	TRY(ini_save(filename, sections));
	return 0;
}
