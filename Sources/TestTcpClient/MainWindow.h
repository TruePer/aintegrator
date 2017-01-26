////////////////////////////////////////////////////////////////////////////////////////////////////
/// A-Integrator
/// (c) 2016 - Denis Fatkulin - denis.fatkulin@gmail.com
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>



namespace Ui {
class MainWindow;
}

class TcpClient;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(const QString& moduleId, QWidget* parent = nullptr);
	~MainWindow();

protected:
	void changeEvent(QEvent* e);

private slots:
	void onMessageRecieved(const QString& message);
	void on_sendStatus_clicked();
	void onSendStatus();

private:
	void closeEvent(QCloseEvent* event);
	void readSettings();

private:
	Ui::MainWindow* _ui;
	TcpClient*      _client;
	QTimer*         _statusTimer;
};

#endif // MAINWINDOW_H
