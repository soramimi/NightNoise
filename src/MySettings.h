#ifndef MYSETTINGS_H
#define MYSETTINGS_H

#include <QSettings>

class MySettings : public QSettings {
	Q_OBJECT
public:
	explicit MySettings(QObject *parent = 0);
};

#endif // MYSETTINGS_H
