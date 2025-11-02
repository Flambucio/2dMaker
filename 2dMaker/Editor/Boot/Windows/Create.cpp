#include "Create.h"

using namespace D2Maker::GUI;

BootPopUpCreate::BootPopUpCreate()
	: popup("Create Project Menu"),
	name("Project name"),
	createBtn(150, 30, "Create Project", [this]()
		{
			if (FileSys::CreateProject(name.GetText()))
			{
				FileSys::SelectProject(name.GetText());
			}
		}),
	closeBtn(150, 30, "Close", [this]()
		{
			ImGui::CloseCurrentPopup();
		})
{
}

void BootPopUpCreate::Update()
{
	ImGui::SetNextWindowSize(ImVec2(350, 120), ImGuiCond_Always);
	if (popup.Begin())
	{
		name.Update();
		createBtn.Update();
		ImGui::SameLine();
		closeBtn.Update();
		popup.End();
	}
}

void BootPopUpCreate::Activate()
{
	popup.Open();
}