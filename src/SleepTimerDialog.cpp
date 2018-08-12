#include "SleepTimerDialog.h"
#include "ui_SleepTimerDialog.h"

SleepTimerDialog::SleepTimerDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::SleepTimerDialog)
{
	ui->setupUi(this);
	Qt::WindowFlags flags = windowFlags();
	flags &= ~Qt::WindowContextHelpButtonHint;
	setWindowFlags(flags);

	ui->comboBox->addItem("None");
	ui->comboBox->addItem("15");
	ui->comboBox->addItem("30");
	ui->comboBox->addItem("45");
	ui->comboBox->addItem("60");
	ui->comboBox->addItem("90");
	ui->comboBox->addItem("120");
	ui->comboBox->addItem("180");
	ui->comboBox->addItem("240");
	ui->comboBox->addItem("360");
	ui->comboBox->addItem("480");
	ui->comboBox->addItem("600");
}

SleepTimerDialog::~SleepTimerDialog()
{
	delete ui;
}

void SleepTimerDialog::setMinutes(int min)
{
	if (min > 0) {
		ui->comboBox->setCurrentText(QString::number(min));
	} else {
		ui->comboBox->setCurrentIndex(0);
	}
}

int SleepTimerDialog::minutes() const
{
	return ui->comboBox->currentText().toInt();
}
