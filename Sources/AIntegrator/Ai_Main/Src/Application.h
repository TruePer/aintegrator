////////////////////////////////////////////////////////////////////////////////////////////////////
/// A-Integrator
/// (c) 2016 - Denis Fatkulin - denis.fatkulin@gmail.com
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef AI_MAIN_APPLICATION_H
#define AI_MAIN_APPLICATION_H

#include <QApplication>



class Translator;

namespace ai {

namespace common {
class Core;
}

namespace main {

/**
 * Класс приложения
 */
class Application : public QApplication
{
public:
	Application(int& argc, char** argv);

public:
	void initialize(ai::common::Core& core);

public:
	static void registerTypes();

private:
	void setupLog4Qt();

private:
	Translator* _translator;
};

} // namespace main
} // namespace ai

#endif // AI_MAIN_APPLICATION_H
