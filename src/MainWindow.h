#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAudioOutput>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT
private:
	struct Private;
	Private *m;
public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

	bool isPlaying() const;
private slots:
	void on_pushButton_start_clicked();
	void on_pushButton_stop_clicked();
	void on_pushButton_sleep_timer_clicked();

	void on_toolButton_minimize_clicked();

	void on_toolButton_close_clicked();

private:
	Ui::MainWindow *ui;

	// QObject interface
	void setSleepTimer();
protected:
	void timerEvent(QTimerEvent *event);
};

#endif // MAINWINDOW_H
