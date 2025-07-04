#pragma once
#include <iostream>

// forward declarations
namespace ImGui
{
	void End();
	void EndTable();
	void EndMenu();
	void EndMenuBar();
};
typedef int ImGuiWindowFlags;
struct GLFWwindow;

namespace ENGINE::UI
{

	struct ImGuiScopedWindow
	{
	private:
		bool m_open;
	
	public:
		ImGuiScopedWindow(const char* name, bool* p_open = nullptr, ImGuiWindowFlags flags = 0);
		~ImGuiScopedWindow();

		operator bool() const { return m_open; };
	};

	/* *********************************** SCOPED TABLE ************************************ */

	struct ImGuiScopedTable
	{
	private:
		bool m_open;

	public:
		ImGuiScopedTable(const char* id, uint16_t columns, ImGuiWindowFlags flags = 0);
		~ImGuiScopedTable();

		operator bool() const { return m_open; };
	};

	/* *********************************** SCOPED MENU ************************************ */

	struct ImGuiScopedMainMenuBar
	{
	private:
		bool m_open;

	public:
		ImGuiScopedMainMenuBar();
		~ImGuiScopedMainMenuBar();

		operator bool() const { return m_open; };
	};
	
	/* *********************************** SCOPED MENU ************************************ */

	struct ImGuiScopedMenu
	{
	private:
		bool m_open;

	public:
		ImGuiScopedMenu(const char* label, bool enabled = true);
		~ImGuiScopedMenu();

		operator bool() const { return m_open; };
	};
}