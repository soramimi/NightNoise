#include "ApplicationGlobal.h"
#include "MainWindow.h"
#include "SleepTimerDialog.h"
#include "ui_MainWindow.h"
#include <QDebug>
#include <QThread>
#include <QTime>
#include <memory>

class MyAudioOutput : public QThread {
public:
	volatile int volume = 100;
protected:
	void run()
	{
		QAudioFormat format;
		format.setByteOrder(QAudioFormat::LittleEndian);
		format.setChannelCount(2);
		format.setCodec("audio/pcm");
		format.setSampleRate(16000);
		format.setSampleSize(16);
		format.setSampleType(QAudioFormat::SignedInt);
		QAudioOutput player(format);
		QIODevice *device = player.start();

		while (1) {
			if (isInterruptionRequested()) break;

			if (volume > 0) {
				int amplitude;

				auto Rand = [&](){
					long long r = (long long)rand() + rand() + rand() + rand();
					r = r * amplitude * 2 / 4 / RAND_MAX - amplitude;
					return (int16_t)r;
				};

				const int N = 800;

				while (1) {
					int16_t buf[N];

					int n = player.bytesFree();
					n /= N * 2;
					if (n < 1) break;
					for (int i = 0; i < n; i++) {
						amplitude = volume * 327;
						for (int j = 0; j < N; j++) {
							buf[j] = Rand();
						}
						device->write((char const *)buf, N * 2);
					}
				}
			}

			msleep(1);
		}

	}
};

struct MainWindow::Private {
	MyAudioOutput audio_out;
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

	startTimer(100);
}

MainWindow::~MainWindow()
{
	m->audio_out.requestInterruption();
	m->audio_out.wait();
	delete m;
	delete ui;
}

bool MainWindow::isPlaying() const
{
	return m->playing;
}

void MainWindow::setSleepTimer()
{
	if (global->sleep_timeout_minutes > 0) {
		m->sleep_time = QDateTime::currentDateTime().addSecs(global->sleep_timeout_minutes * 60);
	} else {
		m->sleep_time = QDateTime();
	}
	ui->label_status->setText(QString());
}

void MainWindow::on_pushButton_start_clicked()
{
	m->playing = true;
	m->fadeout = false;

	setSleepTimer();

	m->audio_out.volume = 100;
	if (!m->audio_out.isRunning()) {
		m->audio_out.start();
	}
}

void MainWindow::on_pushButton_stop_clicked()
{
	m->playing = false;
	m->fadeout = false;
	m->audio_out.volume = 0;
}

void MainWindow::timerEvent(QTimerEvent *event)
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
		if (m->audio_out.volume > 0) {
			m->audio_out.volume--;
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
