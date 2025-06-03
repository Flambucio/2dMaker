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
			std::function<void()> CloseCallbackCreate;
			Entity& selectedEntity;
		public:


			ComponentMenus(Entity& selectedEntity) : selectedEntity(selectedEntity),
				traCreator(selectedEntity, [this](void) {this->CloseCallbackCreate();}),
				collCreator(selectedEntity, [this](void) {this->CloseCallbackCreate();}),
				velCreator(selectedEntity, [this](void) { this->CloseCallbackCreate(); }),
				timCreator(selectedEntity, [this](void) { this->CloseCallbackCreate(); }),
				nameCreator(selectedEntity, [this](void) { this->CloseCallbackCreate(); }),
				audCreator(selectedEntity, [this](void) { this->CloseCallbackCreate(); }),
				scrCreator(selectedEntity, [this](void) {this->CloseCallbackCreate();}),
				texCreator(selectedEntity, [this](void) {this->CloseCallbackCreate();}),
				aniCreator(selectedEntity, [this](void) {this->CloseCallbackCreate();}),
				rbCreator(selectedEntity, [this](void) {this->CloseCallbackCreate();}),
				folCreator(selectedEntity, [this](void) {this->CloseCallbackCreate();}),
				camCreator(selectedEntity, [this](void) {this->CloseCallbackCreate();})

			{ }

			void ActivateComponentWindow(std::string componentName, bool componentExists=false)
			{
				TRACE(componentName);
				TRACE(" c name");
				TRACE((componentName == "Collider"));
				if(componentName=="Transform") traCreator.Activate();
				if (componentName == "Collider") collCreator.Activate();
				if (componentName == "Velocity") velCreator.Activate();
				if (componentName == "Timer") timCreator.Activate();
				if (componentName == "Name") nameCreator.Activate();
				if (componentName == "Audio") audCreator.Activate();
				if (componentName == "Script") scrCreator.Activate();
				if (componentName == "Texture") texCreator.Activate();
				if (componentName == "Animation") aniCreator.Activate();
				if (componentName == "RigidBody") rbCreator.Activate();
				if (componentName == "Follow") folCreator.Activate();
				if (componentName == "Camera") camCreator.Activate();

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

			void SetCloseCallback(std::function<void()> f)
			{
				CloseCallbackCreate = f;
			}

			
		};
	}
}