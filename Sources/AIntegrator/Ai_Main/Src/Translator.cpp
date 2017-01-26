////////////////////////////////////////////////////////////////////////////////////////////////////
/// A-Integrator
/// (c) 2016 - Denis Fatkulin - denis.fatkulin@gmail.com
////////////////////////////////////////////////////////////////////////////////////////////////////
#include "Logger.h"
#include "Translator.h"

#include <QApplication>
#include <QSettings>
#include <QTranslator>



using namespace ai::main;

Translator::Translator(QObject* parent)
    : QObject(parent)
    , _translatorRu(new QTranslator(this))
{
	_translatorRu->load("AIntegrator_ru", ":/Translation/");
	loadSettings();
}

QString Translator::getEmptyString()
{
	return QString();
}

void Translator::selectLanguage(quint32 languageId)
{
	switch(languageId) {
	case 0: // English
		 qApp->removeTranslator(_translatorRu);
		 saveSettings("en");
		break;
	case 1: // Russian
		qApp->installTranslator(_translatorRu);
		saveSettings("ru");
		break;
	}

	emit languageChanged();
}

void Translator::loadSettings()
{
	QSettings settings("nppntt", "AIntegrator");
	auto lang = settings.value("language").toString();

	if(lang.isEmpty() || lang == "ru") {
		selectLanguage(1);
	}
	else {
		selectLanguage(0);
	}
}

void Translator::saveSettings(const QString& lang)
{
	QSettings settings("nppntt", "AIntegrator");
	settings.setValue("language", lang);

	ai::log() << tr("Language: %1").arg(lang);
}

