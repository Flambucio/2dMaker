#pragma once
#include "TransformCreation.h"
#include "ColliderCreation.h"
#include "VelocityCreator.h"
#include "TimerCreator.h"
#include "NameCreator.h"
#include "AudioComponentCreator.h"
#include "ScriptCreator.h"
#include "TextureComponentCreator.h"
#include "AnimationCreator.h"
#include "RigidbodyCreator.h"
#include "CameraCreator.h"
#include "FollowCreator.h"
namespace D2Maker
{
	namespace GUI
	{
		enum class CWindowActionFlag
		{
			NONE = -1,
			MODIFYING = 0,
			CREATING=1
		};



		class ComponentMenus
		{
		private:
			TransformCreator traCreator;
			ColliderCreator collCreator;
			VelocityCreator velCreator;
			TimerCreator timCreator;
			NameCreator nameCreator;
			AudioComponentCreator audCreator;
			ScriptCreator scrCreator;
			TextureComponentCreator texCreator;
			AnimationCreator aniCreator;
			RigidbodyCreator rbCreator;
			CameraCreator camCreator;
			FollowCreator folCreator;
			std::function<void()> updateComponents;
			Entity& selectedEntity;
		public:


			ComponentMenus(Entity& selectedEntity,std::function<void()> updateComponents) : selectedEntity(selectedEntity),
				updateComponents(updateComponents),
				traCreator(selectedEntity, [this](void) {if (this->updateComponents) { this->updateComponents(); }}),
				collCreator(selectedEntity, [this](void) {if (this->updateComponents) { this->updateComponents(); }}),
				velCreator (selectedEntity, [this](void) {}),
				timCreator (selectedEntity, [this](void) {}),
				nameCreator(selectedEntity, [this](void) {}),
				audCreator (selectedEntity, [this](void) {}),
				scrCreator (selectedEntity, [this](void) {}),
				texCreator (selectedEntity, [this](void) {}),
				aniCreator (selectedEntity, [this](void) {}),
				rbCreator  (selectedEntity, [this](void) {}),
				folCreator (selectedEntity, [this](void) {}),
				camCreator (selectedEntity, [this](void) {})

			{ }

			void ActivateComponentWindow(std::string componentName, bool componentExists)
			{
				TRACE(componentName);
				TRACE(" c name");
				TRACE((componentName == "Collider"));
				if(componentName=="Transform") traCreator.Activate(componentExists);
				if (componentName == "Collider") collCreator.Activate(componentExists);
				if (componentName == "Velocity") velCreator.Activate(componentExists);
				if (componentName == "Timer") timCreator.Activate(componentExists);
				if (componentName == "Name") nameCreator.Activate(componentExists);
				if (componentName == "Audio") audCreator.Activate(componentExists);
				if (componentName == "Script") scrCreator.Activate(componentExists);
				if (componentName == "Texture") texCreator.Activate(componentExists);
				if (componentName == "Animation") aniCreator.Activate(componentExists);
				if (componentName == "RigidBody") rbCreator.Activate(componentExists);
				if (componentName == "Follow") folCreator.Activate(componentExists);
				if (componentName == "Camera") camCreator.Activate(componentExists);

			}

			void Update()
			{
				traCreator.Update();
				collCreator.Update();
				velCreator.Update();
				timCreator.Update();
				nameCreator.Update();
				audCreator.Update();
				scrCreator.Update();
				texCreator.Update();
				aniCreator.Update();
				rbCreator.Update();
				folCreator.Update();
				camCreator.Update();
			}

			
		};
	}
}