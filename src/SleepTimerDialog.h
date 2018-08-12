#ifndef SLEEPTIMERDIALOG_H
#define SLEEPTIMERDIALOG_H

#include <QDialog>

namespace Ui {
class SleepTimerDialog;
}

class SleepTimerDialog : public QDialog
{
	Q_OBJECT

public:
	explicit SleepTimerDialog(QWidget *parent = 0);
	~SleepTimerDialog();

	void setMinutes(int min);
	int minutes() const;
private:
	Ui::SleepTimerDialog *ui;
};

#endif // SLEEPTIMERDIALOG_H
