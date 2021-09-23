#include <iostream>
#include "inih/INIReader.h"

#define INI_SECTION "wms"

struct wmsyncConfig {
	std:: string username, password;
	std::string gwId, wlan, mac;
};

void startSync(wmsyncConfig *config);

int main(int argc, char *argv[])
{
	if (argc != 2) {
		std::cout << "Usage: " << argv[0]
			<< " <path_to_config.ini>" << std::endl;
		return 1;
	}
	
	INIReader reader(argv[1]);

	if (reader.ParseError() != 0) {
		std::cout << "Can't load " << argv[1] << std::endl;
	}

	wmsyncConfig config;
	config.username = reader.Get(INI_SECTION, "username", "UNKNOWN");
	config.password = reader.Get(INI_SECTION, "password", "UNKNOWN");
	config.gwId = reader.Get(INI_SECTION, "gw_id", "UNKNOWN");
	config.wlan = reader.Get(INI_SECTION, "wlan", "UNKNOWN");
	config.mac = reader.Get(INI_SECTION, "mac", "UNKNOWN");

	startSync(&config);
	return 0;
}

void startSync(wmsyncConfig *config) {
	std::cout << config->username << std::endl;
}
