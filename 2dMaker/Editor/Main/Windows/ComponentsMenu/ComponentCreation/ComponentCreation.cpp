#include "ComponentCreation.h"
using namespace D2Maker::GUI;
ComponentCreation::ComponentCreation(Entity& selectedEntity, std::function<void()> updateComponents, std::unique_ptr<OpenComponentMenuEventUI>& EventPtr) : selectedEntity(selectedEntity),
popup("Add Component"),
EventPtr(EventPtr),
dropdown(componentsStrArr, 0, "Select Component"),
updateComponents(updateComponents),
closeBtn(100, 30, "Close", [this](void)
	{
		popup.Close();
	}
),
selectBtn(100, 30, "Select", [this](void)
	{
		this->EventPtr = std::make_unique<OpenComponentMenuEventUI>(dropdown.GetCurrVal(), false);
	}
)
{
}

void ComponentCreation::Update()
{

	if (popup.Begin())
	{
		dropdown.Update();
		closeBtn.Update();
		ImGui::SameLine();
		selectBtn.Update();
		popup.End();
	}
}