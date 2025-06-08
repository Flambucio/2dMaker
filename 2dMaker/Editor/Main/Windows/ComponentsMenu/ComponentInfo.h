#pragma once
#include "../../../API/APIExport.h"

#define DEF_GET_INFO_FOR_TYPE(TYPE) template<>\
	void Get<TYPE>(Entity selectedEntity)


namespace D2Maker
{
	namespace GUI
	{
		class ComponentInfo
		{
		private:
			GUIAPI::ButtonWithCallback<> closeBtn;
			GUIAPI::PopUp popup;
			std::string infoStr;

			template<typename T>
			void Get(Entity selectedEntity)
			{
				infoStr = "Nothing to display for this";
			}

			DEF_GET_INFO_FOR_TYPE(Transform)
			{
				
				Transform* transform = CURRENT_SCENE->em.getComponent<Transform>(selectedEntity);
				infoStr = "default x: " + std::to_string(transform->defaultX) + "\n" +
					"default y: " + std::to_string(transform->defaultY) + "\n" +
					"x: " + std::to_string(transform->x) + "\n" +
					"y: " + std::to_string(transform->y) + "\n" +
					"width: " + std::to_string(transform->width) + "\n" +
					"height: " + std::to_string(transform->height) + "\n" +
					"rotation: " + std::to_string(transform->rotationDegrees) + "\n";
			}

			DEF_GET_INFO_FOR_TYPE(Velocity)
			{
				Velocity* vel = CURRENT_SCENE->em.getComponent<Velocity>(selectedEntity);
				infoStr = "dx: " + std::to_string(vel->dx) + "\n" +
					"dy: " + std::to_string(vel->dy) + "\n" +
					"d-theta: " + std::to_string(vel->dtheta) + "\n" +
					"default dx: " + std::to_string(vel->defaultDx) + "\n" +
					"default dy: " + std::to_string(vel->defaultDy) + "\n" +
					"default d-theta: " + std::to_string(vel->defaultDtheta) + "\n";
			}

			DEF_GET_INFO_FOR_TYPE(AudioComponent)
			{
				AudioComponent* ac = CURRENT_SCENE->em.getComponent<AudioComponent>(selectedEntity);
				infoStr = "name: " + ac->name;
			}

			DEF_GET_INFO_FOR_TYPE(Script)
			{
				Script* script = CURRENT_SCENE->em.getComponent<Script>(selectedEntity);
				infoStr = "path: " + script->filepath;
			}

			DEF_GET_INFO_FOR_TYPE(TextureComponent)
			{
				TextureComponent* text = CURRENT_SCENE->em.getComponent<TextureComponent>(selectedEntity);
				infoStr = "name: " + text->name + "\n" +
					"order in layer: " + std::to_string(text->orderInLayer);
			}

			DEF_GET_INFO_FOR_TYPE(Name)
			{
				Name* name = CURRENT_SCENE->em.getComponent<Name>(selectedEntity);
				infoStr = "name: " + name->name;
			}

			DEF_GET_INFO_FOR_TYPE(Animation)
			{
				Animation* anim = CURRENT_SCENE->em.getComponent<Animation>(selectedEntity);
				std::string componentStr = "";
				for (auto& texture : anim->texNames)
				{
					componentStr += texture + "\n";
				}

				infoStr = "textures :" + componentStr +
					"timing: " + std::to_string(anim->timing) + "\n" +
					"order in layer: " + std::to_string(anim->orderInLayer);
			}

			DEF_GET_INFO_FOR_TYPE(RigidBody)
			{
				RigidBody* rb = CURRENT_SCENE->em.getComponent<RigidBody>(selectedEntity);
				infoStr = "mass: " + std::to_string(rb->mass) + "\n" +
					"bounciness: " + std::to_string(rb->bounciness) + "\n" +
					"max dy: " + std::to_string(rb->maxDy);
			}

			DEF_GET_INFO_FOR_TYPE(Follow)
			{
				Follow* follow = CURRENT_SCENE->em.getComponent<Follow>(selectedEntity);
				infoStr = "name to follow: " + follow->entityToFollow + "\n" +
					"velocity: " + std::to_string(follow->velocity);
			}

			DEF_GET_INFO_FOR_TYPE(Camera)
			{
				Camera* cam = CURRENT_SCENE->em.getComponent<Camera>(selectedEntity);
				std::string enableXStr = "";
				std::string enableYStr = "";
				cam->enableX ? enableXStr = "true" : enableXStr = "false";
				cam->enableY ? enableYStr = "true" : enableYStr = "false";
				infoStr = "enable x: " + enableXStr + "\n" +
					"enable y: " + enableYStr + "\n" +
					"x: " + std::to_string(cam->x) + "\n" +
					"y: " + std::to_string(cam->y) + "\n";
			}

			const std::array<std::string, 10> components = { "Transform" ,"Velocity","Audio","Script",
			"Texture","Name","Animation","RigidBody","Camera","Follow"};
		public:
			ComponentInfo() : popup("Info"), closeBtn(200, 30, "Close", [this](void)
				{
					popup.Close();
				}
			){ }


			void GetInfo(std::string component, Entity selectedEntity)
			{

				if (component == "Transform")
				{
					Get<Transform>(selectedEntity);
				}
				else if (component == "Velocity")
				{
					Get<Velocity>(selectedEntity);
				}

				else if (component == "Audio")
				{
					Get<AudioComponent>(selectedEntity);
				}

				else if (component == "Script")
				{
					Get<Script>(selectedEntity);
				}

				else if (component == "Texture")
				{
					Get<TextureComponent>(selectedEntity);
				}

				else if (component == "Name")
				{
					Get<Name>(selectedEntity);
				}

				else if (component == "Animation")
				{
					Get<Animation>(selectedEntity);
				}

				else if (component == "RigidBody")
				{
					Get<RigidBody>(selectedEntity);
				}

				else if (component == "Follow")
				{
					Get<Follow>(selectedEntity);
				}

				else if (component == "Camera")
				{
					Get<Camera>(selectedEntity);
				}
				else
				{
					Get<void>(selectedEntity);
				}
			}

			void Activate(std::string component,Entity selectedEntity)
			{
				GetInfo(component, selectedEntity);
				popup.Open();
			}

			void Update()
			{
				if (popup.Begin())
				{
					ImGui::Text(infoStr.c_str());
					closeBtn.Update();
					popup.End();
				}
			}

		};
	}
}