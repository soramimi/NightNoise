#ifndef APPLICATIONGLOBAL_H
#define APPLICATIONGLOBAL_H

#include <QString>

class ApplicationGlobal {
public:
	QString organization_name;
	QString application_name;
	QString generic_config_dir;
	QString app_config_dir;
	QString config_file_path;

	int sleep_timeout_minutes = 60;

	ApplicationGlobal();
	static void loadSettings();
	static void saveSettings();
};

extern ApplicationGlobal *global;

#endif // APPLICATIONGLOBAL_H
