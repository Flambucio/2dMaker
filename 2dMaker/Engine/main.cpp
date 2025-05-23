// 2dMaker.cpp : Questo file contiene la funzione 'main', in cui inizia e termina l'esecuzione del programma.
//

#include "Application/Window.h"
#include "../Editor/Boot/Boot.h"

int main()
{
	D2Maker::GUI::BootWindow bWin;
	bWin.RunBoot();
	if (bWin.prjFlag != D2Maker::GUI::ProjectSelectedFlag::SELECTED) return 0;
	D2Maker::Window::Initialize();
	D2Maker::Window::RunWindow();

}