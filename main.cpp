#include "Magnification.h"
#include "Windows.h"
#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;

static string windowName = "Age of Empires II: HD Edition";
static unsigned int xOffset = 17;
static unsigned int yOffset = 72;
static unsigned int yResolution = 1080;
static unsigned int xResolution = 1920;
static unsigned int xPositioning = 50;
static unsigned int yPositioning = 50;
static float scaleUp = 2.5;
static bool multipleMonitors = false;



void ClientResize(HWND hWnd, int nWidth, int nHeight) 
{
	RECT rcClient, rcWind;
	POINT ptDiff;
	GetClientRect(hWnd, &rcClient);
	GetWindowRect(hWnd, &rcWind);
	ptDiff.x = (rcWind.right - rcWind.left) - rcClient.right;
	ptDiff.y = (rcWind.bottom - rcWind.top) - rcClient.bottom;
	MoveWindow(hWnd, xPositioning, yPositioning, nWidth + ptDiff.x, nHeight + ptDiff.y, TRUE);//move half distance to top left corner
}

/*BOOL SetZoom(float magnificationFactor)
{
	// A magnification factor less than 1.0 is not valid.
	if (magnificationFactor < 1.0)
	{
		return FALSE;
	}

	// Calculate offsets such that the center of the magnified screen content 
	// is at the center of the screen. The offsets are relative to the 
	// unmagnified screen content.
	int W = GetSystemMetrics(SM_CXSCREEN);
	int H = GetSystemMetrics(SM_CYSCREEN);

	int xDlg = (int)((float) W * (1.0 - (1.0 / magnificationFactor)) / 2.0);
	int yDlg = (int)((float) H * (1.0 - (1.0 / magnificationFactor)) / 2.0);
	
	HWND window = FindWindow(NULL, "Age of Empires II: HD Edition");

	//MoveWindow(window, xDlg, yDlg, 1920/4, 1080/4, true);
	int xAdjustment = 868/2.5;
	int yAdjustment = 640/2.5;

	SetWindowPos(window, HWND_TOP, xDlg, yDlg, 1960/2.5, 1170/2.5, SWP_SHOWWINDOW);
//	SetWindowPos(window, HWND_TOP, xDlg, yDlg, W / magnificationFactor/2.5, H / magnificationFactor/2.5, SWP_SHOWWINDOW);

	RECT r;
	GetWindowRect(window, &r);

	int xOffset = r.left *2.5;//+xAdjustment;
	int yOffset = r.top *2.5;//+ yAdjustment;

	return MagSetFullscreenTransform(magnificationFactor, xOffset, yOffset);
}*//*
BOOL SetFullscreenMagnification(BOOL fPositionRelativeToVirtualScreen)
{
	BOOL fResult = FALSE;

	// Get the window whose upper-left corner is to appear at the upper-left 
	// corner of the magnified view.
	HWND hWndTarget = FindWindow(NULL, "Age of Empires II: HD Edition");
	if (hWndTarget != NULL)
	{
		ClientResize(hWndTarget, 1920 / 2.5, 1080 / 2.5);


		RECT rcTarget, rcClient;
		GetWindowRect(hWndTarget, &rcTarget);
		GetClientRect(hWndTarget, &rcClient);

		// Set the magnification to be 200%.
		float magVal = 2.0;

		// Position the point of interest to appear at the upper-left corner 
		// of the primary monitor.
	//	int xOffset = (rcTarget.left + ((rcTarget.right - rcTarget.left) - rcClient.right))*2.5;//-15;
	//	int yOffset = (rcTarget.top + ((rcTarget.bottom - rcTarget.top) - rcClient.bottom))*2.5;// -16;

		int xOffset = (rcTarget.left + 2 * GetSystemMetrics(SM_CXFIXEDFRAME))*2.5 +2;
		int yOffset = ( rcTarget.top + 2 * GetSystemMetrics(SM_CYFIXEDFRAME) + GetSystemMetrics(SM_CYCAPTION))*2.5;

		if (fPositionRelativeToVirtualScreen)
		{
			// Adjust the point of interest to appear at the upper-left corner of 
			// the virtual screen; that is, the left-most monitor.

			RECT rcVirtualScreen;

			rcVirtualScreen.left = GetSystemMetrics(SM_XVIRTUALSCREEN)/2.5;
			rcVirtualScreen.top = GetSystemMetrics(SM_YVIRTUALSCREEN)/2.5;


			xOffset -= (int)(rcVirtualScreen.left / magVal);
			yOffset -= (int)(rcVirtualScreen.top / magVal);
		}

		std::cout << "Client Left: " << rcClient.left << "\tClient Right: " << rcClient.right << "\tClient Top: " << rcClient.top << "\tClient bottom: " << rcClient.bottom << std::endl;
		std::cout << "Window Left: " << rcTarget.left << "\tWindow Right: " << rcTarget.right << "\tWindow Top: " << rcTarget.top << "\tWindow bottom: " << rcTarget.bottom << std::endl;

		fResult = MagSetFullscreenTransform(magVal, xOffset, yOffset);

	}

	return fResult;
}
*/

bool startMagnification(){
	bool result = FALSE;

	HWND hWndTarget = FindWindow(NULL, windowName.c_str());
	if (hWndTarget != NULL)
	{
		ClientResize(hWndTarget, xResolution / scaleUp, yResolution / scaleUp);

		RECT rcTarget, rcClient;
		GetWindowRect(hWndTarget, &rcTarget);
		GetClientRect(hWndTarget, &rcClient);

		// Set the magnification to correct value
		int horizontal = scaleUp * GetSystemMetrics(SM_CXFULLSCREEN);
		//int vertical = scaleUp * GetSystemMetrics(SM_CYFULLSCREEN);
		//std::cout << horizontal << " " << vertical << endl;
		//float magVal = min((float)horizontal / (float)xResolution, (float)vertical / (float)yResolution);

		float magVal = (float)horizontal / (float)xResolution;

		// Position the point of interest to appear at the upper-left corner 
		// of the primary monitor.

		int x = rcTarget.left*scaleUp + xOffset;
		int y = rcTarget.top*scaleUp + yOffset;

		//cout << x << " " << y << " " << magVal << endl;

		if (multipleMonitors)
		{
			// Adjust the point of interest to appear at the upper-left corner of 
			// the virtual screen; that is, the left-most monitor.

			RECT rcVirtualScreen;

			rcVirtualScreen.left = GetSystemMetrics(SM_XVIRTUALSCREEN) / scaleUp;
			rcVirtualScreen.top = GetSystemMetrics(SM_YVIRTUALSCREEN) / scaleUp;


			x -= (int)(rcVirtualScreen.left / magVal);
			y -= (int)(rcVirtualScreen.top / magVal);
		}
		
		result = MagSetFullscreenTransform(magVal, x, y);
		SetForegroundWindow(hWndTarget);
	}
	else {
		std::cout << "Window was not found. Please open the program and check the spelling of the window name: " << windowName << endl;
		return true;
	}

	return result;
}

bool stopMagnification() {
	return MagSetFullscreenTransform(1.0, 0, 0);
}

void changeResolution() {
	std::cout << "Please enter the desired width in pixels as a plain number. Current value: " << xResolution << std::endl;
	std::cin >> xResolution;
	std::cout << "The width was set to " << xResolution << "px" << std::endl;
	std::cout << "Now the height please. Current value: " << yResolution << std::endl;
	std::cin >> yResolution;
	std::cout << "The height was set to " << yResolution << "px" << std::endl;
}

void changePositioning() {
	std::cout << "Please enter the desired x position of the target window in pixels. Current value: " << xPositioning << std::endl;
	std::cin >> xPositioning;
	std::cout << "The width was set to " << xPositioning << "px" << std::endl;
	std::cout << "Now the y value please. Current value: " << yPositioning << std::endl;
	std::cin >> yPositioning;
	std::cout << "The height was set to " << yPositioning << "px" << std::endl;
}

void changeWindowName() {
	std::cout << "Please enter the name of the window you want to resize. Current window name is: " << windowName << std::endl;
	std::getline(cin, windowName);
	std::cout << "The window name was set to " << windowName  << std::endl;
}

void showCommands() {
	std::cout << std::setw(20) << left<<"magnify, m" << std::setw(30) << "Start magnification." << std::endl;
	std::cout << std::setw(20) << "stop, s" << std::setw(30) << "Stop magnification." << std::endl;
	std::cout << std::setw(20) << "quit, q, exit" << std::setw(30) << "Exit application." << std::endl;
	std::cout << std::setw(20) << "info, i, settings" << std::setw(30) << "Show all currently used vallues for the magnification." << std::endl;
	std::cout << std::setw(20) << "setResolution" << std::setw(30) << "Change the resoultion you want to transform your window to." << std::endl;
	std::cout << std::setw(20) << "setPosition" << std::setw(30) << "Change the position of the target window." << std::endl;
	std::cout << std::setw(20) << "setXOffset" << std::setw(30) << "Change the offset if the window is not exactly at the correct position." << std::endl;
	std::cout << std::setw(20) << "setYOffset" << std::setw(30) << "Change the offset if the window is not exactly at the correct position." << std::endl;
	std::cout << std::setw(20) << "help, commands" << std::setw(30) << "Show all commands." << std::endl;
	std::cout << std::setw(20) << "setScaleUp" << std::setw(30) << "Adjust the scale up. This is be defined in your Windows settings for better readabilty of any user interface. (Standard in this application is 2.5)" << std::endl;
	std::cout << std::setw(20) << "setWindow" << std::setw(30) << "Change the name of the window to Target. (Standard is <Age of Empires II: HD Edition> )" << std::endl;
	std::cout << std::setw(20) << "useLeftMostMonitor" << std::setw(30) << "Enable this if you want to use your left most monitor instead of your main monitor. Does nothing if you are on a single monitor." << std::endl;
	std::cout << std::setw(20) << "useMainMonitor" << std::setw(30) << "Resets the application to use the main monitor instead of the left most." << std::endl;
	std::cout << std::setw(20) << "refresh" << std::setw(30) << "Updates to the values set in config.txt." << std::endl;
}

void showSettings() {
	std::cout << std::setw(35) << left << "Window:" << windowName << std::endl;
	std::cout << std::setw(35) << left << "Resolution:"  << xResolution << " x " << yResolution << std::endl;
	std::cout << std::setw(35) << left << "Position:" << xPositioning << " x " << yPositioning << std::endl;
	std::cout << std::setw(35) << left << "Window offset:"  << xOffset << " x " << yOffset << std::endl;
	std::cout << std::setw(35) << left << "Scaling factor set in MS Windows:" << scaleUp << std::endl;
	std::cout << std::setw(35) << left << "Use multiple monitors:"  << (multipleMonitors ? "YES" : "NO") << std::endl;
}

void read_in_config_file() {
	string line;
	ifstream config("config.txt");
	if (config.is_open())
	{
		// name 
		if (getline(config, line)) {
			string tail = line.substr(line.find("[") + 1);
			string svalue = tail.substr(0, tail.find("]"));
			windowName = svalue;
		}
		// resolution
		if (getline(config, line)) {
			string tailx = line.substr(line.find("[") + 1);
			string svaluex = tailx.substr(0, tailx.find("]"));
			xResolution = std::stoi(svaluex.c_str());
			string taily = tailx.substr(tailx.find("[") + 1);
			string svaluey = taily.substr(0, taily.find("]"));
			yResolution = std::stoi(svaluey.c_str());
		}
		// position
		if (getline(config, line)) {
			string tailx = line.substr(line.find("[") + 1);
			string svaluex = tailx.substr(0, tailx.find("]"));
			xPositioning = std::stoi(svaluex.c_str());
			string taily = tailx.substr(tailx.find("[") + 1);
			string svaluey = taily.substr(0, taily.find("]"));
			yPositioning = std::stoi(svaluey.c_str());
		}
		// offset
		if (getline(config, line)) {
			string tailx = line.substr(line.find("[") + 1);
			string svaluex = tailx.substr(0, tailx.find("]"));
			xOffset = std::stoi(svaluex.c_str());
			string taily = tailx.substr(tailx.find("[") + 1);
			string svaluey = taily.substr(0, taily.find("]"));
			yOffset = std::stoi(svaluey.c_str());
		}
		// scaling
		if (getline(config, line)) {
			string tail = line.substr(line.find("[") + 1);
			string svalue = tail.substr(0, tail.find("]"));
			scaleUp = std::stof(svalue.c_str());
		}
		// multiple monitirs
		if (getline(config, line)) {
			string tail = line.substr(line.find("[") + 1);
			string svalue = tail.substr(0, tail.find("]"));
			multipleMonitors = svalue =="1" || svalue == "true" || svalue == "yes" || svalue == "TRUE" || svalue == "Yes" || svalue == "YES";
		}
		config.close();
	}
	else cout << "Unable to open config.txt";
}

bool command(std::string input) {
	if (input == "setResolution") { changeResolution(); }
	else if (input == "setPosition") { changePositioning(); }
	else if (input == "setWindow") { changeWindowName(); }
	else if (input == "setScaleUp") { cout << "Current scale: " << scaleUp << " Enter new scale" << endl; cin >> scaleUp; cout << "Scale set to " << scaleUp << endl; }
	else if (input == "setXOffset") { cout << "Current X offset: " << xOffset << " Enter new X offset" << endl; cin >> xOffset; cout << "X offset changed to " << xOffset << endl; }
	else if (input == "setYOffset") { cout << "Current Y offset: " << yOffset << " Enter new Y offset" << endl; cin >> yOffset; cout << "Y offset changed to " << yOffset << endl; }
	else if (input == "help" || input == "commands") { showCommands(); }
	else if (input == "settings" || input == "show" || input == "info" || input=="i") { showSettings(); }
	else if (input == "m" || input == "magnify" || input == "start") { cout << "Starting magnification now, write stop to quit magnification without escaping the program. " << endl; if (!startMagnification()) { cout << "Sorry, magnification failed. Contact me (Jakob) on Steam." << endl; } }
	else if (input == "s" || input == "stop") { stopMagnification(); std::cout << "Stopped magnificaton" << endl; }
	else if (input == "useLeftMostMonitor") { multipleMonitors = true; }
	else if (input == "useMainMonitor" || input == "dontUseLetfMostMonitor") { multipleMonitors = false; }
	else if (input == "11") { std::cout << "lol" << std::endl; }
	else if (input == "MARCO" || input == "POLO" || input == "ROBIN HOOD" || input == "I LOVE THE MONKEY HEAD" || input == "HOW DO YOU TURN THIS ON") { std::cout << "Nice try :)" << std::endl; }
	else if (input == "Fuck you" || input == "fuck you" || input == "fuck you!" || input == "Fuck you!") { std::cout << "Why do you write such things?!"; return false; }
	else if (input == "exit" || input == "q" || input == "quit") { return false; }
	else if (input == "refresh" || input == "update") { read_in_config_file(); }
	return true;
}

int main() {
	SetConsoleTitle(("AoE resizer by Jakob"));

	MagInitialize();
	read_in_config_file();
	//std::cout << GetSystemMetrics(SM_CXFIXEDFRAME) << std::endl << GetSystemMetrics(SM_CYMENU) << std::endl << GetSystemMetrics(SM_CYCAPTION) << std::endl;
	//HWND hWndTarget = FindWindow(NULL, "Age of Empires II: HD Edition");
	//ClientResize(hWndTarget, 1920/2.5, 1080/2.5);
	//SetZoom(2.0);
	//SetFullscreenMagnification(false);
	std::string input;

	std::cout << "Write help for a list of commands." << std::endl;

	do { std::getline(std::cin, input); }
	while (command(input));
	
	MagUninitialize();
	return 0;
}