#include "ApplicationGlobal.h"
#include "MainWindow.h"
#include "MySettings.h"
#include "joinpath.h"
#include <QApplication>
#include <QDir>
#include <QFileInfo>
#include <QStandardPaths>

ApplicationGlobal *global;

void ApplicationGlobal::loadSettings()
{
	MySettings s;
	s.beginGroup("Global");
	global->sleep_timeout_minutes = s.value("SleepTimeoutMinutes", "60").toInt();
	s.endGroup();
}

void ApplicationGlobal::saveSettings()
{
	MySettings s;
	s.beginGroup("Global");
	s.setValue("SleepTimeoutMinutes", QString::number(global->sleep_timeout_minutes));
	s.endGroup();
}

int main(int argc, char *argv[])
{
	ApplicationGlobal g;
	global = &g;
	global->organization_name = "soramimi.jp";
	global->application_name = "NightNoise";
	global->generic_config_dir = QStandardPaths::writableLocation(QStandardPaths::GenericConfigLocation);
	global->app_config_dir = global->generic_config_dir / global->organization_name / global->application_name;
	global->config_file_path = joinpath(global->app_config_dir, global->application_name + ".ini");
	if (!QFileInfo(global->app_config_dir).isDir()) {
		QDir().mkpath(global->app_config_dir);
	}

	QApplication a(argc, argv);
	a.setOrganizationName(global->organization_name);
	a.setApplicationName(global->application_name);

	MainWindow w;
	w.show();

	return a.exec();
}
