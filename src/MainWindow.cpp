#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "ApplicationGlobal.h"
#include "SleepTimerDialog.h"
#include <QDebug>
#include <QTime>
#include <QTimer>
#include <memory>

struct MainWindow::Private {
	QTimer interval_10ms_timer;
	QTimer interval_100ms_timer;
	std::shared_ptr<QAudioOutput> player;
	QIODevice *output = nullptr;
	int volume = 0;
	QDateTime sleep_time;
	bool playing = false;
	bool fadeout = false;
};

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
	, m(new Private)
{
	ui->setupUi(this);

	ApplicationGlobal::loadSettings();

	setWindowState(Qt::WindowFullScreen);

	ui->label_status->setText(QString());

	connect(&m->interval_10ms_timer, &QTimer::timeout, [&](){
		on10msInterval();
	});
	m->interval_10ms_timer.start(10);

	connect(&m->interval_100ms_timer, &QTimer::timeout, [&](){
		on100msInterval();
	});
	m->interval_100ms_timer.start(100);

	QAudioFormat format;
	format.setByteOrder(QAudioFormat::LittleEndian);
	format.setChannelCount(2);
	format.setCodec("audio/pcm");
	format.setSampleRate(16000);
	format.setSampleSize(16);
	format.setSampleType(QAudioFormat::SignedInt);
	m->player = std::make_shared<QAudioOutput>(format);
	m->output = m->player->start();
}

MainWindow::~MainWindow()
{
	delete m;
	delete ui;
}

bool MainWindow::isPlaying() const
{
	return m->playing;
}

void MainWindow::setSleepTimer()
{
	m->sleep_time = QDateTime::currentDateTime().addSecs(global->sleep_timeout_minutes * 60);
	ui->label_status->setText(QString());
}

void MainWindow::stopSleepTimer()
{
	m->sleep_time = QDateTime();
	ui->label_status->setText(QString());
}

void MainWindow::on_pushButton_start_clicked()
{
	m->playing = true;
	m->fadeout = false;
	m->volume = 100;
	setSleepTimer();
}

void MainWindow::on_pushButton_stop_clicked()
{
	m->playing = false;
	m->fadeout = false;
	m->volume = 0;
	stopSleepTimer();
}

void MainWindow::on10msInterval()
{
	if (m->volume > 0) {
		int amplitude;

		auto Rand = [&](){
			long long r = (long long)rand() + rand() + rand() + rand();
			r = r * amplitude * 2 / 4 / RAND_MAX - amplitude;
			return (int16_t)r;
		};

		const int N = 800;

		while (1) {
			int16_t buf[N];

			int n = m->player->bytesFree();
			n /= N * 2;
			if (n < 1) break;
			for (int i = 0; i < n; i++) {
				amplitude = m->volume * 327;
				for (int j = 0; j < N; j++) {
					buf[j] = Rand();
				}
				m->output->write((char const *)buf, N * 2);
			}
		}
	}
}

void MainWindow::on100msInterval()
{
	QDateTime now = QDateTime::currentDateTime();

	{
		QDateTime local = now.toLocalTime();
		QString s = QString().sprintf("%u-%02u-%02u, %02u:%02u:%02u"
									  , local.date().year()
									  , local.date().month()
									  , local.date().day()
									  , local.time().hour()
									  , local.time().minute()
									  , local.time().second()
									  );
		ui->label_current_time->setText(s);
	}


	if (m->sleep_time.isValid()) {
		int secs = now.secsTo(m->sleep_time);
		if (secs > 0) {
			QString s = QString().sprintf("Stop in %u:%02u", secs / 60, secs % 60);
			ui->label_status->setText(s);
		} else {
			ui->label_status->setText(QString());
			m->sleep_time = QDateTime();
			m->fadeout = true;
		}
	}

	if (m->fadeout) {
		if (m->volume > 0) {
			m->volume--;
		} else {
			m->fadeout = false;
		}
	}
}

void MainWindow::on_pushButton_sleep_timer_clicked()
{
	SleepTimerDialog dlg(this);
	dlg.setMinutes(global->sleep_timeout_minutes);
	if (dlg.exec() == QDialog::Accepted) {
		global->sleep_timeout_minutes = dlg.minutes();
		if (isPlaying()) {
			setSleepTimer();
		}
		ApplicationGlobal::saveSettings();
	}
}

void MainWindow::on_toolButton_minimize_clicked()
{
	setWindowState(windowState() ^ Qt::WindowMinimized);
}

void MainWindow::on_toolButton_close_clicked()
{
	close();
}
