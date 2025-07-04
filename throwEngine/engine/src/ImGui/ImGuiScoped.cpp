#include "ImGui/ImGuiScoped.h"

#include "imgui.h"

#include "imgui_impl_glfw.h"

#include "imgui_impl_opengl3.h"

#include "core/Logger.h"
#include "core/Debug.h"
#define DEBUG_PTR(ptr) DEBUG::DebugForEngineObjectPointers(ptr)

namespace ENGINE::UI
{

	/* *********************************** SCOPED WINDOW ************************************ */

	ImGuiScopedWindow::ImGuiScopedWindow(const char* name, bool* p_open, ImGuiWindowFlags flags)
		: m_open(ImGui::Begin(name, p_open, flags)) {}

	ImGuiScopedWindow::~ImGuiScopedWindow()
	{
		if (m_open)
			ImGui::End();
	}

	/* *********************************** SCOPED TABLE ************************************ */

	ImGuiScopedTable::ImGuiScopedTable(const char* tableName, uint16_t columns, ImGuiWindowFlags flags)
		: m_open(ImGui::BeginTable(tableName, columns, flags)) {}

	ImGuiScopedTable::~ImGuiScopedTable()
	{
		if (m_open)
			ImGui::EndTable();
	}

	/* *********************************** SCOPED MENU ************************************ */

	ImGuiScopedMainMenuBar::ImGuiScopedMainMenuBar()
		: m_open(ImGui::BeginMainMenuBar()) {
	}

	ImGuiScopedMainMenuBar::~ImGuiScopedMainMenuBar()
	{
		if (m_open)
			ImGui::EndMainMenuBar();
	}

	/* *********************************** SCOPED MAIN MENUBAR ************************************ */


	ImGuiScopedMenu::ImGuiScopedMenu(const char* label, bool enabled)
		: m_open(ImGui::BeginMenu(label, enabled)) {
	}

	ImGuiScopedMenu::~ImGuiScopedMenu()
	{
		if (m_open)
			ImGui::EndMenu();
	}
}