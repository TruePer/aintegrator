////////////////////////////////////////////////////////////////////////////////////////////////////
/// A-Integrator
/// (c) 2016 - Denis Fatkulin - denis.fatkulin@gmail.com
////////////////////////////////////////////////////////////////////////////////////////////////////
#include "MainWindow.h"
#include "TcpClient.h"
#include "ui_MainWindow.h"

#include <QSettings>
#include <QTimer>



MainWindow::MainWindow(const QString& moduleId, QWidget* parent)
    : QMainWindow(parent)
    , _ui(new Ui::MainWindow())
    , _client(new TcpClient(moduleId, this))
    , _statusTimer(new QTimer(this))
{
	_ui->setupUi(this);

	setWindowTitle(tr("Module - %1").arg(moduleId));

	readSettings();

	connect(_client, SIGNAL(messageRecieved(QString)), SLOT(onMessageRecieved(QString)));
	connect(_statusTimer, SIGNAL(timeout()), SLOT(onSendStatus()));

	_ui->sendStatus->click();
}

MainWindow::~MainWindow()
{
	delete _ui;
}

void MainWindow::changeEvent(QEvent* e)
{
	QMainWindow::changeEvent(e);
	switch (e->type()) {
	case QEvent::LanguageChange:
		_ui->retranslateUi(this);
		break;
	default:
		break;
	}
}

void MainWindow::onMessageRecieved(const QString& message)
{
	_ui->text->insertPlainText(message);
}

void MainWindow::on_sendStatus_clicked()
{
	if(_ui->sendStatus->isChecked()) {
		_client->sendStatus();
		_statusTimer->start(3000);
	}
	else {
		_statusTimer->stop();
	}
}

void MainWindow::onSendStatus()
{
	_client->sendStatus();
	_client->sendParameters();
}

void MainWindow::closeEvent(QCloseEvent* event)
{
	QSettings settings("nppntt", "TcpClient");
	settings.setValue("geometry", saveGeometry());
	settings.setValue("windowState", saveState());
	QMainWindow::closeEvent(event);
}

void MainWindow::readSettings()
{
	QSettings settings("nppntt", "TcpClient");
	restoreGeometry(settings.value("geometry", QSize(1024, 768)).toByteArray());
	restoreState(settings.value("windowState").toByteArray());
}
