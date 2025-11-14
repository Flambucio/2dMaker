#include "ComponentMenus.h"
using namespace D2Maker::GUI;

AnimationCreator::AnimationCreator(Entity& selectedEntity, std::function<void()> closeCreation) : updateComponents(updateComponents),
popup("Animation"),
selectedEntity(selectedEntity),
textBox_Timing("Timing of every sprite"),
textBox_OIL("Order in Layer"),
textBox_Names("Texture Names"),
addBtn(100, 30, "Add/Mod", [this](void)
	{
		AddAnimationComponent();
	}
),
closeBtn(100, 30, "Close", [this](void)
	{
		popup.Close();
	}
)
{
}

void AnimationCreator::Update()
{
	if (popup.Begin())
	{
		textBox_Names.Update();
		textBox_OIL.Update();
		textBox_Timing.Update();
		ImGui::Text("names shoul be inserted separaed by comma");
		ImGui::Text("Example:");
		ImGui::Text("texture_1,texture_2");
		ImGui::Text("names cant contain spaces (yet)");
		if (componentExists)
		{
			ImGui::Text("");
			ImGui::Text("WARNING");
			ImGui::Text("This component is not really intended to be modified since is a");
			ImGui::Text("component that relies on external files and not normal values");
		}
		closeBtn.Update();
		ImGui::SameLine();
		addBtn.Update();
		popup.End();
	}
}

void AnimationCreator::Activate(bool componentExists)
{
	this->componentExists = componentExists;
	if (componentExists)
	{
		Animation* anim = SceneManager::GetScene(SceneManager::currentScene)->em.getComponent<Animation>(selectedEntity);
		std::string namesStr = "";
		for (int i = 0;i < anim->texNames.size();i++)
		{
			namesStr += anim->texNames[i];
			if (i != anim->texNames.size() - 1)
			{
				namesStr += ',';
			}
		}
		textBox_Names.SetText(namesStr);
		textBox_OIL.SetText(std::to_string(anim->orderInLayer));
		textBox_Timing.SetText(std::to_string(anim->timing));

	}
	else
	{
		textBox_OIL.SetText(std::to_string(0));
		textBox_Timing.SetText(std::to_string(0));
	}
	popup.Open();
}

void AnimationCreator::AddAnimationComponent()
{
	int oil = 0;
	if (!ConvertStringToNum<int>(textBox_OIL.GetText(), oil)) return;

	float timing = 0;
	if (!ConvertStringToNum<float>(textBox_Timing.GetText(), timing)) return;


	std::string unparsedNames = textBox_Names.GetText();
	std::unordered_set<std::string> names;
	std::string tStr = "";
	for (int i = 0;i < unparsedNames.size();i++)
	{
		char tChar = unparsedNames[i];
		if (tChar == ',')
		{
			if (tStr == "") return;
			names.insert(tStr);
			tStr = "";
		}
		else
		{
			tStr += tChar;
		}

	}
	names.insert(tStr);
	std::vector<std::string> namesVec;
	SetToVector(names, namesVec);
	bool canClose = false;
	Scene* currentScene = SceneManager::GetScene(SceneManager::currentScene);

	if (!componentExists)
	{
		if (currentScene->em.addComponent<Animation>(this->selectedEntity,
			namesVec, timing, oil))
		{
			CONSOLELOGCOMPONENTSUCCESS(Animation, this->selectedEntity);
			canClose = true;
		}
		else { CONSOLELOGCOMPONENTUNSUCCESFUL(Animation, this->selectedEntity) }
	}
	else
	{
		std::vector<std::string> oldNamesVec = {};
		float oldTiming = 0;
		int oldOil = 0;
		{
			Animation* anim = currentScene->em.getComponent<Animation>(selectedEntity);
			oldNamesVec = anim->texNames;
			oldTiming = anim->timing;
			oldOil = anim->orderInLayer;
		}
		if (currentScene->em.RemoveComponent<Animation>(selectedEntity))
		{
			if (currentScene->em.addComponent<Animation>(selectedEntity, namesVec, timing, oil))
			{
				canClose = true;
				CONSOLELOGCOMPONENTSUCCESSMODIFY(Animation, this->selectedEntity);
			}
			else
			{
				currentScene->em.addComponent<Animation>(selectedEntity, oldNamesVec, oldTiming, oldOil);
			}
		}

	}

	if (!canClose) return;

	popup.Close();
	if (this->updateComponents)
	{
		this->updateComponents();
	}

}

AudioComponentCreator::AudioComponentCreator(Entity& selectedEntity, std::function<void()> closeCreation) : updateComponents(updateComponents),
popup("Audio"),
selectedEntity(selectedEntity),
textBox("Audio Name"),
addBtn(100, 30, "Add/Mod", [this](void)
	{
		AddAudioComponent();
	}
),
closeBtn(100, 30, "Close", [this](void)
	{
		popup.Close();
	}
)
{
}

void AudioComponentCreator::Update()
{
	if (popup.Begin())
	{
		textBox.Update();
		if (componentExists)
		{
			ImGui::Text("");
			ImGui::Text("WARNING");
			ImGui::Text("This component is not really intended to be modified since is a");
			ImGui::Text("component that relies on external files and not normal values");
		}
		closeBtn.Update();
		ImGui::SameLine();
		addBtn.Update();
		popup.End();
	}
}

void AudioComponentCreator::Activate(bool componentExists)
{
	this->componentExists = componentExists;
	if (componentExists)
	{
		AudioComponent* ac = SceneManager::GetScene(SceneManager::currentScene)->em.getComponent<AudioComponent>(selectedEntity);
		textBox.SetText(ac->name);
	}
	popup.Open();
}

void AudioComponentCreator::AddAudioComponent()
{
	if (textBox.GetText() == "") return;
	bool canClose = false;
	Scene* currentScene = SceneManager::GetScene(SceneManager::currentScene);

	if (!componentExists)
	{
		if (currentScene->em.addComponent<AudioComponent>(this->selectedEntity, textBox.GetText()))
		{
			CONSOLELOGCOMPONENTSUCCESS(AudioComponent, this->selectedEntity);
			canClose = true;
		}
		else { CONSOLELOGCOMPONENTUNSUCCESFUL(AudioComponent, this->selectedEntity); }

	}
	else
	{
		std::string oldName = "";

		{
			AudioComponent* ac = currentScene->em.getComponent<AudioComponent>(selectedEntity);
			oldName = ac->name;
		}

		if (currentScene->em.RemoveComponent<AudioComponent>(selectedEntity))
		{
			if (currentScene->em.addComponent<AudioComponent>(this->selectedEntity, textBox.GetText()))
			{
				canClose = true;
				CONSOLELOGCOMPONENTSUCCESSMODIFY(AudioComponent, this->selectedEntity);
			}
			else
			{
				currentScene->em.addComponent<AudioComponent>(this->selectedEntity, oldName);
			}
		}


	}

	if (!canClose) return;
	popup.Close();
	if (this->updateComponents)
	{
		this->updateComponents();
	}
}

CameraCreator::CameraCreator(Entity& selectedEntity, std::function<void()> closeCreation) : closeCreation(closeCreation),
popup("Camera"),
textBox_x("Focus x"),
textBox_y("Focus y"),
checkBox_enableX("Enable X"),
checkBox_enableY("Enable Y"),
selectedEntity(selectedEntity),
addBtn(100, 30, "Add/Mod", [this](void)
	{
		AddCameraComponent();
	}
),
closeBtn(100, 30, "Close", [this](void)
	{
		popup.Close();
	}
)
{
}

void CameraCreator::Update()
{
	if (popup.Begin())
	{
		textBox_x.Update();
		textBox_y.Update();
		checkBox_enableX.Update();
		checkBox_enableY.Update();
		closeBtn.Update();
		ImGui::SameLine();
		addBtn.Update();
		popup.End();
	}
}

void CameraCreator::Activate(bool componentExists)
{
	this->componentExists = componentExists;
	if (componentExists)
	{
		Camera* cam = SceneManager::GetScene(SceneManager::currentScene)->em.getComponent<Camera>(selectedEntity);
		textBox_x.SetText(std::to_string(cam->x));
		textBox_y.SetText(std::to_string(cam->y));
		checkBox_enableX.SetValue(cam->enableX);
		checkBox_enableY.SetValue(cam->enableY);
	}
	else
	{
		textBox_x.SetText(std::to_string(0));
		textBox_y.SetText(std::to_string(0));
		checkBox_enableX.SetValue(false);
		checkBox_enableY.SetValue(false);
	}
	popup.Open();
}

void CameraCreator::AddCameraComponent()
{
	bool enableX = checkBox_enableX.GetValue();
	bool enableY = checkBox_enableY.GetValue();
	float x = 0;
	float y = 0;
	if (!ConvertStringToNum<float>(textBox_x.GetText(), x)) return;
	if (!ConvertStringToNum<float>(textBox_y.GetText(), y)) return;
	bool canClose = false;
	Scene* currentScene = SceneManager::GetScene(SceneManager::currentScene);

	if (!componentExists)
	{
		if (currentScene->em.addComponent<Camera>(this->selectedEntity,
			enableX, enableY, x, y))
		{
			CONSOLELOGCOMPONENTSUCCESS(Camera, this->selectedEntity);
			canClose = true;

		}
		else { CONSOLELOGCOMPONENTUNSUCCESFUL(Camera, this->selectedEntity); }

	}
	else
	{
		Camera* cam = currentScene->em.getComponent<Camera>(selectedEntity);
		cam->enableX = enableX;
		cam->enableY = enableY;
		cam->x = x;
		cam->y = y;
		canClose = true;
		CONSOLELOGCOMPONENTSUCCESSMODIFY(Camera, this->selectedEntity);
	}

	if (!canClose) return;
	popup.Close();
	if (this->closeCreation)
	{
		this->closeCreation();
	}

}

ColliderCreator::ColliderCreator(Entity& selectedEntity, std::function<void()> updateComponents) : updateComponents(updateComponents),
popup("Collider"),
selectedEntity(selectedEntity),
addBtn(100, 30, "Add", [this](void)
	{
		if (SceneManager::GetScene(SceneManager::currentScene)->em.addComponent<Collider>(this->selectedEntity))
		{
			CONSOLELOGCOMPONENTSUCCESS(Collider, this->selectedEntity);
			popup.Close();
			if (this->updateComponents)
			{
				this->updateComponents();
			}
		}
		else { CONSOLELOGCOMPONENTUNSUCCESFUL(Collider, this->selectedEntity); }
	}
),
closeBtn(100, 30, "Close", [this](void)
	{
		popup.Close();
	}
)
{
}

void ColliderCreator::Update()
{
	//TRACE("update popup");
	if (popup.Begin())
	{
		TRACE("true");
		closeBtn.Update();
		ImGui::SameLine();
		addBtn.Update();
		popup.End();
	}
}

FollowCreator::FollowCreator(Entity& selectedEntity, std::function<void()> closeCreation) : closeCreation(closeCreation),
popup("Follow"),
selectedEntity(selectedEntity),
textBox_NameToFollow("name to follow"),
textBox_Vel("Velocity"),
addBtn(100, 30, "Add/Mod", [this](void)
	{

	}
),
closeBtn(100, 30, "Close", [this](void)
	{
		popup.Close();
	}
)
{
}

void FollowCreator::Update()
{
	if (popup.Begin())
	{
		textBox_NameToFollow.Update();
		textBox_Vel.Update();
		closeBtn.Update();
		ImGui::SameLine();
		addBtn.Update();
		popup.End();
	}
}

void FollowCreator::Activate(bool componentExists)
{
	this->componentExists = componentExists;
	if (componentExists)
	{
		Follow* follow = SceneManager::GetScene(SceneManager::currentScene)->em.getComponent<Follow>(selectedEntity);
		textBox_NameToFollow.SetText(follow->entityToFollow);
		textBox_Vel.SetText(std::to_string(follow->velocity));
	}
	else
	{
		textBox_Vel.SetText(std::to_string(0));
	}
	popup.Open();
}

void FollowCreator::AddFollowComponent()
{
	float vel = 0;
	std::string name = textBox_NameToFollow.GetText();
	bool canClose = false;
	if (name == "") return;
	if (!ConvertStringToNum<float>(textBox_Vel.GetText(), vel)) return;
	Scene* currentScene = SceneManager::GetScene(SceneManager::currentScene);

	if (!componentExists)
	{
		if (currentScene->em.addComponent<Follow>(this->selectedEntity, vel, name))
		{
			CONSOLELOGCOMPONENTSUCCESS(Follow, this->selectedEntity);
			canClose = true;
		}
		else { CONSOLELOGCOMPONENTUNSUCCESFUL(Follow, this->selectedEntity); }
	}
	else
	{
		float oldVel = 0;
		std::string oldName = "";
		{
			Follow* follow = currentScene->em.getComponent<Follow>(selectedEntity);
			oldVel = follow->velocity;
			oldName = follow->entityToFollow;

		}
		if (currentScene->em.addComponent<Follow>(selectedEntity, vel, name))
		{
			CONSOLELOGCOMPONENTSUCCESSMODIFY(Follow, this->selectedEntity);
			canClose = true;
		}
		else
		{
			currentScene->em.addComponent<Follow>(selectedEntity, oldVel, oldName);
		}
	}

	if (!canClose) return;
	popup.Close();
	if (this->closeCreation)
	{
		this->closeCreation();
	}

}

RigidbodyCreator::RigidbodyCreator(Entity& selectedEntity, std::function<void()> closeCreation) : updateComponents(updateComponents),
popup("Rigidbody"),
selectedEntity(selectedEntity),
textBox_Mass("Mass"),
textBox_BounceFactor("Bounciness (factor)"),
textBox_MaxDy("Max gravity speed (dy)"),
addBtn(100, 30, "Add/Mod", [this](void)
	{
		AddRigidBodyComponent();
	}
),
closeBtn(100, 30, "Close", [this](void)
	{
		popup.Close();
	}
)
{
}

void RigidbodyCreator::Update()
{
	if (popup.Begin())
	{
		textBox_Mass.Update();
		textBox_BounceFactor.Update();
		textBox_MaxDy.Update();
		closeBtn.Update();
		ImGui::SameLine();
		addBtn.Update();
		popup.End();
	}
}

void RigidbodyCreator::Activate(bool componentExists)
{
	this->componentExists = componentExists;
	if (componentExists)
	{
		RigidBody* rb = SceneManager::GetScene(SceneManager::currentScene)->em.getComponent<RigidBody>(selectedEntity);
		textBox_Mass.SetText(std::to_string(rb->mass));
		textBox_BounceFactor.SetText(std::to_string(rb->bounciness));
		textBox_MaxDy.SetText(std::to_string(rb->maxDy));
	}
	else
	{
		textBox_Mass.SetText(std::to_string(0));
		textBox_BounceFactor.SetText(std::to_string(0));
		textBox_MaxDy.SetText(std::to_string(0));
	}
	popup.Open();
}

void RigidbodyCreator::AddRigidBodyComponent()
{
	float mass = 0;
	float bounciness = 0;
	float maxDy;
	bool canClose = false;
	if (!ConvertStringToNum<float>(textBox_Mass.GetText(), mass)) return;
	if (!ConvertStringToNum<float>(textBox_BounceFactor.GetText(), bounciness)) return;
	if (!ConvertStringToNum<float>(textBox_MaxDy.GetText(), maxDy)) return;
	Scene* currentScene = SceneManager::GetScene(SceneManager::currentScene);
	if (!componentExists)
	{
		if (currentScene->em.addComponent<RigidBody>(this->selectedEntity,
			mass, bounciness, maxDy))
		{
			CONSOLELOGCOMPONENTSUCCESS(RigidBody, this->selectedEntity);
			canClose = true;
		}
		else { CONSOLELOGCOMPONENTUNSUCCESFUL(RigidBody, this->selectedEntity); }
	}
	else
	{

		RigidBody* rb = currentScene->em.getComponent<RigidBody>(selectedEntity);
		rb->bounciness = bounciness;
		rb->mass = mass;
		rb->maxDy = maxDy;
		canClose = true;
		CONSOLELOGCOMPONENTSUCCESSMODIFY(RigidBody, this->selectedEntity);
	}


	if (!canClose) return;
	popup.Close();
	if (this->updateComponents)
	{
		this->updateComponents();
	}




}

ScriptCreator::ScriptCreator(Entity& selectedEntity, std::function<void()> updateComponents) : updateComponents(updateComponents),
popup("Script"),
selectedEntity(selectedEntity),
textBox("Filepath"),
addBtn(180, 30, "Add/Mod", [this](void)
	{
		AddScriptComponent();
	}
),
closeBtn(180, 30, "Close", [this](void)
	{
		popup.Close();
	}
)
{
}

void ScriptCreator::Update()
{
	if (popup.Begin())
	{
		textBox.Update();
		ImGui::Text("Insert only the name of the file (no extension)");
		ImGui::Text("Custom subdirectories are not supported (yet)");
		ImGui::Text("file format: txt");
		ImGui::Text("The file should be into Projects/{currentProject}/Scripts/");
		if (componentExists)
		{
			ImGui::Text("");
			ImGui::Text("WARNING");
			ImGui::Text("This component is not really intended to be modified since is a");
			ImGui::Text("component that relies on external files and not normal values");
		}


		closeBtn.Update();
		ImGui::SameLine();
		addBtn.Update();
		popup.End();
	}
}

void ScriptCreator::Activate(bool componentExists)
{
	this->componentExists = componentExists;
	if (componentExists)
	{
		Script* script = SceneManager::GetScene(SceneManager::currentScene)->em.getComponent<Script>(selectedEntity);
		std::string Ttex = ((fs::path)script->filepath).filename().string();
		SliceStringReference(Ttex, 0, (Ttex.size() - 5));
		textBox.SetText(Ttex);
	}
	popup.Open();
}

void ScriptCreator::AddScriptComponent()
{
	bool canClose = false;
	if (textBox.GetText() == "") return;
	Scene* currentScene = SceneManager::GetScene(SceneManager::currentScene);
	std::string path = "Projects/" + FileSys::currentProject + "/Scripts/" + textBox.GetText() + ".txt";

	if (!componentExists)
	{
		if (currentScene->em.addComponent<Script>(this->selectedEntity, path))
		{
			canClose = true;
			CONSOLELOGCOMPONENTSUCCESS(Script, this->selectedEntity);
		}
		else { CONSOLELOGCOMPONENTUNSUCCESFUL(Script, this->selectedEntity); }
	}
	else
	{
		std::string oldPath = "";
		{
			Script* script = currentScene->em.getComponent<Script>(selectedEntity);
			oldPath = script->filepath;
		}

		if (currentScene->em.RemoveComponent<Script>(selectedEntity))
		{
			if (currentScene->em.addComponent<Script>(selectedEntity, path))
			{
				canClose = true;
				CONSOLELOGCOMPONENTSUCCESSMODIFY(Script, this->selectedEntity);
			}
			else
			{
				currentScene->em.addComponent<Script>(selectedEntity, oldPath);
			}
		}


	}


	if (!canClose) return;
	popup.Close();
	if (this->updateComponents)
	{
		this->updateComponents();
	}
}

TextureComponentCreator::TextureComponentCreator(Entity& selectedEntity, std::function<void()> updateComponents) : updateComponents(updateComponents),
popup("Texture"),
selectedEntity(selectedEntity),
textBox_Name("Texture Name"),
textBox_OIL("Order in Layer"),
addBtn(100, 30, "Add/Mod", [this](void)
	{
		AddTextureComponent();
	}
),
closeBtn(100, 30, "Close", [this](void)
	{
		popup.Close();
	}
)
{
}

void TextureComponentCreator::Update()
{
	if (popup.Begin())
	{
		textBox_Name.GetText();
		if (componentExists)
		{
			ImGui::Text("");
			ImGui::Text("WARNING");
			ImGui::Text("This component is not really intended to be modified since is a");
			ImGui::Text("component that relies on external files and not normal values");
		}
		closeBtn.Update();
		ImGui::SameLine();
		addBtn.Update();
		popup.End();
	}
}

void TextureComponentCreator::Activate(bool componentExists)
{
	this->componentExists = componentExists;
	if (componentExists)
	{
		TextureComponent* tc = SceneManager::GetScene(SceneManager::currentScene)->em.getComponent<TextureComponent>(selectedEntity);
		textBox_Name.SetText(tc->name);
		textBox_OIL.SetText(std::to_string(tc->orderInLayer));
	}
	else
	{
		textBox_OIL.SetText(std::to_string(0));
	}
	popup.Open();
}

void TextureComponentCreator::AddTextureComponent()
{
	int oil = 0; //order in layer
	if (textBox_Name.GetText() == "") return;
	if (!ConvertStringToNum<int>(textBox_OIL.GetText(), oil)) return;
	bool canClose = false;
	Scene* currentScene = SceneManager::GetScene(SceneManager::currentScene);

	if (!componentExists)
	{
		if (currentScene->em.addComponent<TextureComponent>(this->selectedEntity,
			textBox_Name.GetText(), oil))
		{
			canClose = true;
			CONSOLELOGCOMPONENTSUCCESS(TextureComponent, this->selectedEntity);
		}
		else { CONSOLELOGCOMPONENTUNSUCCESFUL(TextureComponent, this->selectedEntity); }
	}
	else
	{
		int oldOil = 0;
		std::string oldTextName = "";
		{
			TextureComponent* textureComponent = currentScene->em.getComponent<TextureComponent>(selectedEntity);
			oldOil = textureComponent->orderInLayer;
			oldTextName = textureComponent->name;
		}
		if (currentScene->em.RemoveComponent<TextureComponent>(selectedEntity))
		{
			if (currentScene->em.addComponent<TextureComponent>(selectedEntity, textBox_Name.GetText(), oil))
			{
				canClose = true;
				CONSOLELOGCOMPONENTSUCCESSMODIFY(TextureComponent, this->selectedEntity);
			}
			else
			{
				currentScene->em.addComponent<TextureComponent>(selectedEntity, oldTextName, oldOil);
			}
		}
	}

	if (!canClose) return;
	popup.Close();
	if (this->updateComponents)
	{
		this->updateComponents();
	}
}

TimerCreator::TimerCreator(Entity& selectedEntity, std::function<void()> closeCreation) : updateComponents(updateComponents),
popup("Timer"),
selectedEntity(selectedEntity),
addBtn(100, 30, "Add", [this](void)
	{
		if (SceneManager::GetScene(SceneManager::currentScene)->em.addComponent<Timer>(this->selectedEntity))
		{
			CONSOLELOGCOMPONENTSUCCESS(Timer, this->selectedEntity);
			popup.Close();
			if (this->updateComponents)
			{
				this->updateComponents();
			}
		}
		else { CONSOLELOGCOMPONENTUNSUCCESFUL(Timer, this->selectedEntity); }
	}
),
closeBtn(100, 30, "Close", [this](void)
	{
		popup.Close();
	}
)
{
}

void TimerCreator::Update()
{
	if (popup.Begin())
	{
		closeBtn.Update();
		ImGui::SameLine();
		addBtn.Update();
		popup.End();
	}
}

TransformCreator::TransformCreator(Entity& selectedEntity, std::function<void()> updateComponents) : textBox_x("X coord"), textBox_y("Y coord"),
textBox_width("Width"), textBox_height("Height"), textBox_rotation("Rotation (degrees)"),
selectedEntity(selectedEntity), popup("Transform"),
closeBtn(100, 30, "Close", [this](void)
	{
		popup.Close();
	}
),
addBtn(100, 30, "Add/Mod", [this](void)
	{
		AddTransformComponent();
	}
),
updateComponents(updateComponents)
{

}

void TransformCreator::Update()
{
	if (popup.Begin())
	{
		textBox_x.Update();
		textBox_y.Update();
		textBox_width.Update();
		textBox_height.Update();
		textBox_rotation.Update();
		closeBtn.Update();
		ImGui::SameLine();
		addBtn.Update();
		popup.End();
	}
}

void TransformCreator::Activate(bool componentExists)
{
	this->componentExists = componentExists;

	if (componentExists)
	{
		Scene* currentScene = SceneManager::GetScene(SceneManager::currentScene);
		Transform* transform = currentScene->em.getComponent<Transform>(selectedEntity);
		textBox_x.SetText(std::to_string(transform->defaultX));
		textBox_y.SetText(std::to_string(transform->defaultY));
		textBox_width.SetText(std::to_string(transform->width));
		textBox_height.SetText(std::to_string(transform->height));
		textBox_rotation.SetText(std::to_string(transform->rotationDegrees));

	}
	else
	{
		textBox_x.SetText(std::to_string(0));
		textBox_y.SetText(std::to_string(0));
		textBox_width.SetText(std::to_string(0));
		textBox_height.SetText(std::to_string(0));
		textBox_rotation.SetText(std::to_string(0));
	}
	popup.Open();
}

void TransformCreator::AddTransformComponent()
{
	bool canClose = false;
	float x = 0;
	float y = 0;
	float width = 0;
	float height = 0;
	float rotationDeg = 0;
	if (!ConvertStringToNum<float>(textBox_x.GetText(), x))					 return;
	if (!ConvertStringToNum<float>(textBox_y.GetText(), y))				 	 return;
	if (!ConvertStringToNum<float>(textBox_width.GetText(), width))			 return;
	if (!ConvertStringToNum<float>(textBox_height.GetText(), height))		 return;
	if (!ConvertStringToNum<float>(textBox_rotation.GetText(), rotationDeg)) return;
	if (!componentExists)
	{


		if (SceneManager::GetScene(SceneManager::currentScene)->em.addComponent<Transform>(
			selectedEntity, x, y, width, height, rotationDeg))
		{
			CONSOLELOGCOMPONENTSUCCESS(Transform, this->selectedEntity);
			canClose = true;
		}
		else { CONSOLELOGCOMPONENTUNSUCCESFUL(Transform, this->selectedEntity); }

	}
	else
	{
		Transform* transform = SceneManager::GetScene(SceneManager::currentScene)->em.getComponent<Transform>(selectedEntity);
		transform->defaultX = x;
		transform->defaultY = y;
		transform->width = width;
		transform->height = height;
		transform->rotationDegrees = rotationDeg;
		canClose = true;
		CONSOLELOGCOMPONENTSUCCESSMODIFY(Transform, this->selectedEntity);

	}

	if (!canClose) return;
	popup.Close();
	if (updateComponents)
	{
		updateComponents();
	}


}

VelocityCreator::VelocityCreator(Entity& selectedEntity, std::function<void()> updateComponents) :
	selectedEntity(selectedEntity),
	updateComponents(updateComponents),
	textBox_dx("dx"),
	textBox_dy("dy"),
	textBox_dtheta("d-theta"),
	popup("Velocity"),
	addBtn(100, 30, "Add/Mod", [this](void)
		{
			AddVelocityComponent();
		}
	),
	closeBtn(100, 30, "Close", [this](void)
		{
			popup.Close();
		}
	)
{
}

void VelocityCreator::Update()
{
	if (popup.Begin())
	{
		textBox_dx.Update();
		textBox_dy.Update();
		textBox_dtheta.Update();
		closeBtn.Update();
		ImGui::SameLine();
		addBtn.Update();
		popup.End();
	}
}

void VelocityCreator::AddVelocityComponent()
{
	bool canClose = false;
	float dx = 0;
	float dy = 0;
	float dtheta = 0;
	if (!ConvertStringToNum<float>(textBox_dx.GetText(), dx))			return;
	if (!ConvertStringToNum<float>(textBox_dy.GetText(), dy))			return;
	if (!ConvertStringToNum<float>(textBox_dtheta.GetText(), dtheta))	return;


	if (!componentExists)
	{
		if (SceneManager::GetScene(SceneManager::currentScene)->em.addComponent<Velocity>(
			selectedEntity, dx, dy, dtheta))
		{
			CONSOLELOGCOMPONENTSUCCESS(Velocity, this->selectedEntity);
			canClose = true;
		}
		else { CONSOLELOGCOMPONENTUNSUCCESFUL(Velocity, this->selectedEntity); }
	}
	else
	{
		Velocity* vel = SceneManager::GetScene(SceneManager::currentScene)->em.getComponent<Velocity>(selectedEntity);
		vel->defaultDx = dx;
		vel->defaultDy = dy;
		vel->defaultDtheta = dtheta;
		canClose = true;
		CONSOLELOGCOMPONENTSUCCESSMODIFY(Velocity, this->selectedEntity);
	}

	if (!canClose) return;
	popup.Close();
	if (updateComponents)
	{
		updateComponents();
	}
}

void VelocityCreator::Activate(bool componentExists)
{
	this->componentExists = componentExists;
	if (componentExists)
	{
		Velocity* vel = SceneManager::GetScene(SceneManager::currentScene)->em.getComponent<Velocity>(selectedEntity);
		textBox_dx.SetText(std::to_string(vel->defaultDx));
		textBox_dy.SetText(std::to_string(vel->defaultDy));
		textBox_dtheta.SetText(std::to_string(vel->defaultDtheta));

	}
	else
	{
		textBox_dx.SetText(std::to_string(0));
		textBox_dy.SetText(std::to_string(0));
		textBox_dtheta.SetText(std::to_string(0));
	}
	popup.Open();
}