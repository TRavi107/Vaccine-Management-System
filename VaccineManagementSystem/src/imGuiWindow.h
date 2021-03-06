#pragma once
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h> // Will drag system OpenGL headers
#include "sqlConnection.h"

enum class DisplayWindow {
	loginWin,
	tableWin,
	browseTableWin,
	Droptable,
	AddRow,
	errorWind,
	droprow,
	comingSoon,
};


struct data {
	char _data[64] = "";
	char datatype[64] = "";
};

class imGuiWindow
{
public:
	imGuiWindow();
	~imGuiWindow();
	void Run();
	static void glfw_error_callback(int error, const char* description);
	void LoginMenu();
	void TableWindow();
	void BrowseTableWindow();
	void MenuBar();
	void DropTableWin();
	void DropTable();
	void DropRow();
	void AddRow();
	void ErrorWindow();
	void DropRowFunc();
	void ComingSoon();
	inline void SwitchState(DisplayWindow targetWindow) { activeWin = targetWindow; };
	inline void SwitchDBtable(std::string targetDBtable) { activeDBtable = targetDBtable; };
	inline std::shared_ptr<sqlConnection> GetSqlPtr() { return sqlConnectionInstance; };


private:
	DisplayWindow activeWin = DisplayWindow::loginWin;
	DisplayWindow previousWin = DisplayWindow::tableWin;

	std::string activeDBtable;
	std::string selected_columnName, selected_columnData;
	std::shared_ptr<sqlConnection> sqlConnectionInstance;
	int Win_width = 1280;
	int Win_height = 720;
	ImVec2 loginWinSize = ImVec2(200, 200);
	ImVec2 loginWinPos = ImVec2(Win_width / 2 - loginWinSize.x / 2, Win_height / 2 - loginWinSize.y / 2 - 100);
	ImVec2 tableWinSize = ImVec2(400, 400);
	ImVec2 tableWinPos = ImVec2(Win_width / 2 - tableWinSize.x / 2, Win_height / 2 - tableWinSize.y / 2 - 100);
	bool show_demo_window = false;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	GLFWwindow* window;
	ImGuiTableFlags flags = ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg |
							ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_Resizable | 
							ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable;
};

