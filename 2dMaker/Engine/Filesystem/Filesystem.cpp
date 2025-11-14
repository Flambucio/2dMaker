#include "Filesystem.h"
using namespace D2Maker;


bool FileSys::CreateProject(const std::string& name)
{
	if (fs::exists("Projects/" + name))
	{
		WARN("This project already exists");
		return false;
	}

	fs::create_directory("Projects/" + name);
	fs::create_directory("Projects/" + name + "/Scripts");
	fs::create_directory("Projects/" + name + "/Scenes");
	fs::create_directories("Projects/" + name + "/Resources");
	fs::create_directories("Projects/" + name + "/Resources/Textures");
	fs::create_directories("Projects/" + name + "/Resources/Audios");
	return true;




}
void FileSys::GetProjects()
{
	projectNames.clear();
	for (const auto& entry : fs::directory_iterator("Projects"))
	{
		if (entry.is_directory())
		{
			projectNames.insert(entry.path().filename().string());
		}
	}
}
void FileSys::DeleteProject(const std::string& name)
{
	if (!fs::exists("Projects/" + name) || !fs::is_directory("Projects/" + name))
	{
		return;
	}

	fs::remove_all("Projects/" + name);

	auto it = projectNames.find(name);
	if (it != projectNames.end())
	{
		projectNames.erase(it);
	}



}
void FileSys::SelectProject(const std::string& name)
{
	if (fs::exists("Projects/" + name) && fs::is_directory("Projects/" + name) && currentProject == "")
	{
		currentProject = name;
	}
}
void FileSys::LoadScenes()
{

	TRACE("LOAD PROJECT " + currentProject);
	if (currentProject == "")
	{
		return;
	}

	if (!(fs::exists("Projects/" + currentProject) || !fs::is_directory("Projects/" + currentProject)))
	{
		TRACE("early return:" + currentProject);
		return;
	}

	for (const auto& entry : fs::directory_iterator("Projects/" + currentProject + "/Scenes"))
	{
		if (entry.is_directory())
		{
			std::string sceneToCreateName = entry.path().filename().string();
			std::string sceneToCreatePath = entry.path().string();
			SceneManager::AddScene(sceneToCreateName);
			TRACE(sceneToCreateName);
			for (const auto& entityFile : fs::directory_iterator(entry.path()))
			{
				LoadEntity(sceneToCreateName, entityFile.path().string());
			}
		}
	}

	std::string defScenePath = "Projects/" + currentProject + "/defaultscene.txt";
	if (!fs::exists(defScenePath) || !fs::is_regular_file(defScenePath))
	{
		return;
	}

	std::vector<std::vector<std::string>> parsedDefSceneStr;
	Parser::ParseString(defScenePath, parsedDefSceneStr);
	if (parsedDefSceneStr.size() != 1)
	{
		return;
	}
	if (parsedDefSceneStr[0].size() != 1)
	{
		return;
	}

	SceneManager::defaultScene = parsedDefSceneStr[0][0];



}
void FileSys::LoadEntity(const std::string& sceneToCreateName, const std::string& EntityPath)
{
	//TRACE("IN LOAD ENTITY");
	Scene* scene = SceneManager::GetScene(sceneToCreateName);
	//TRACE("MAYBE NULL");
	if (!scene) return;
	//TRACE("NOT NULL");
	std::vector<std::vector<std::string>> parsedStr;
	Parser::ParseString(EntityPath, parsedStr);
	if (parsedStr.empty() || parsedStr[0].empty()) {
		ERROR("Parsing error");
		return;
	}
	//TRACE("DOPO IL CHECK");
	Entity entity = scene->em.createEntity(parsedStr[0][0]);
	//TRACE("CREATO IL NOME");
	PRINT_2D_ARRAY_STR(parsedStr);
	for (int i = 1;i < parsedStr.size() - 1;i++)
	{
		if (parsedStr[i][0] == "TRANSFORM")
		{
			scene->em.addComponent<Transform>(entity,
				stof(parsedStr[i][1]),
				stof(parsedStr[i][2]),
				stof(parsedStr[i][3]),
				stof(parsedStr[i][4]),
				stof(parsedStr[i][5])
			);
		}
		else if (parsedStr[i][0] == "VELOCITY")
		{
			scene->em.addComponent<Velocity>(entity,
				stof(parsedStr[i][1]),
				stof(parsedStr[i][2]),
				stof(parsedStr[i][3])
			);
		}
		else if (parsedStr[i][0] == "COLLIDER")
		{
			scene->em.addComponent<Collider>(entity);
		}
		else if (parsedStr[i][0] == "TIMER")
		{
			scene->em.addComponent<Timer>(entity);
		}
		else if (parsedStr[i][0] == "NAME")
		{
			scene->em.addComponent<Name>(entity, parsedStr[i][1]);
		}
		else if (parsedStr[i][0] == "AUDIO")
		{
			scene->em.addComponent<AudioComponent>(entity, parsedStr[i][1]);
		}
		else if (parsedStr[i][0] == "SCRIPT")
		{
			scene->em.addComponent<Script>(entity, "Projects/" + currentProject + "/Scripts/" + parsedStr[i][1]);
		}
		else if (parsedStr[i][0] == "TEXTURE")
		{
			//TRACE("TRYING TO LOAD TEXTURE");
			//TRACE("parsedSTR[i][1]" + parsedStr[i][1]);
			//TRACE("parsedSTR[i][2]" + parsedStr[i][2]);
			scene->em.addComponent<TextureComponent>(entity,
				parsedStr[i][1],
				stoi(parsedStr[i][2])
			);
			//TRACE("LOADED TEXTURE");
		}
		else if (parsedStr[i][0] == "ANIMATION")
		{
			//TRACE("INTO ANIMATION");
			std::vector<std::string> names;
			for (int j = 1;j < parsedStr[i].size() - 2;j++)
			{
				names.push_back(parsedStr[i][j]);
			}
			scene->em.addComponent<Animation>(entity,
				names,
				stof(parsedStr[i][parsedStr[i].size() - 2]),
				stoi(parsedStr[i][parsedStr[i].size() - 1])
			);
		}
		else if (parsedStr[i][0] == "RIGIDBODY")
		{
			scene->em.addComponent<RigidBody>(entity,
				stof(parsedStr[i][1]),
				stof(parsedStr[i][2]),
				stof(parsedStr[i][3])
			);
		}
		else if (parsedStr[i][0] == "CAMERA")
		{
			bool enableX;
			bool enableY;
			parsedStr[i][1] == "T" ? enableX = true : enableX = false;
			parsedStr[i][2] == "T" ? enableY = true : enableY = false;
			scene->em.addComponent<Camera>(entity,
				enableX,
				enableY,
				stoi(parsedStr[i][3]),
				stoi(parsedStr[i][4])
			);
		}
		else if (parsedStr[i][0] == "FOLLOW")
		{
			scene->em.addComponent<Follow>(entity,
				stof(parsedStr[i][1]),
				parsedStr[i][2]
			);
		}
		//TRACE("FINISHED FUNCTION");
	}
}
void FileSys::Save()
{
	if (currentProject == "")
	{
		return;
	}
	std::string currentProjectPath = "Projects/" + currentProject;
	fs::remove_all(currentProjectPath + "/Scenes");
	fs::create_directory(currentProjectPath + "/Scenes");
	for (auto& scene : SceneManager::scenes)
	{
		std::string currentSceneDirectory = currentProjectPath + "/Scenes/" + scene.first;
		fs::create_directories(currentSceneDirectory);
		EntityManager& currentEM = scene.second->em;

		for (std::pair<std::string, Entity> entity : currentEM.entityNames)
		{
			SaveEntity(currentEM, entity, currentSceneDirectory);
		}


	}

	std::ofstream out(currentProjectPath + "/defaultscene.txt");
	out << SceneManager::defaultScene;
	out.close();
	SaveAssets();
	SaveConfigs();






}

void FileSys::SaveEntity(EntityManager& em, std::pair<std::string, Entity> entityPair, std::string currentSceneDirectory)
{
	std::string writeStr = "";
	writeStr += entityPair.first + ";\n";
	Entity entity = entityPair.second;

	if (em.hasComponent<Transform>(entity))
	{
		Transform* transform = em.getComponent<Transform>(entity);
		transform->ResetPos();
		writeStr += "TRANSFORM " + std::to_string(transform->x) +
			" " + std::to_string(transform->y) +
			" " + std::to_string(transform->width) +
			" " + std::to_string(transform->height) +
			" " + std::to_string(transform->rotationDegrees) + ";\n";

	}
	if (em.hasComponent<Velocity>(entity))
	{
		Velocity* velocity = em.getComponent<Velocity>(entity);
		writeStr += "VELOCITY " + std::to_string(velocity->defaultDx) +
			" " + std::to_string(velocity->defaultDy) +
			" " + std::to_string(velocity->defaultDtheta) + ";\n";
	}
	if (em.hasComponent<Collider>(entity))
	{
		writeStr += "COLLIDER;\n";
	}
	if (em.hasComponent<Timer>(entity))
	{
		writeStr += "TIMER;\n";
	}
	if (em.hasComponent<Name>(entity))
	{
		Name* name = em.getComponent<Name>(entity);
		writeStr += "NAME " + name->name + ";\n";
	}
	if (em.hasComponent<AudioComponent>(entity))
	{
		AudioComponent* ac = em.getComponent<AudioComponent>(entity);
		writeStr += "AUDIO " + ac->name + ";\n";
	}
	if (em.hasComponent<Script>(entity))
	{
		Script* script = em.getComponent<Script>(entity);
		writeStr += "SCRIPT " + fs::path(script->filepath).filename().string() + ";\n";
	}
	if (em.hasComponent<TextureComponent>(entity) && !em.hasComponent<Animation>(entity))
	{

		TextureComponent* texture = em.getComponent<TextureComponent>(entity);
		writeStr += "TEXTURE " + texture->name + " " + std::to_string(texture->orderInLayer) + ";\n";
	}
	if (em.hasComponent<Animation>(entity))
	{
		Animation* animation = em.getComponent<Animation>(entity);
		std::string vectorStr;
		for (int i = 0;i < animation->texNames.size();i++)
		{
			vectorStr += animation->texNames[i] + " ";
		}
		writeStr += "ANIMATION " + vectorStr + std::to_string(animation->timing) + " " + std::to_string(animation->orderInLayer) + ";\n";
	}
	if (em.hasComponent<RigidBody>(entity))
	{
		RigidBody* rigidbody = em.getComponent<RigidBody>(entity);
		writeStr += "RIGIDBODY " + std::to_string(rigidbody->mass) +
			" " + std::to_string(rigidbody->bounciness) +
			" " + std::to_string(rigidbody->maxDy) + ";\n";
	}
	if (em.hasComponent<Camera>(entity))
	{
		Camera* camera = em.getComponent<Camera>(entity);
		std::string boolX;
		std::string boolY;
		camera->enableX ? boolX = "T" : boolX = "F";
		camera->enableY ? boolY = "T" : boolY = "F";
		writeStr += "CAMERA " + boolX + " " + boolY +
			" " + std::to_string(camera->x) +
			" " + std::to_string(camera->y) + ";\n";
	}
	if (em.hasComponent<Follow>(entity))
	{
		Follow* follow = em.getComponent<Follow>(entity);
		writeStr += "FOLLOW " + std::to_string(follow->velocity) + " " + follow->entityToFollow + ";\n";
	}

	std::ofstream out(currentSceneDirectory + "/" + entityPair.first + ".txt");
	out << writeStr;
	out.close();


}

void FileSys::LoadAssets()
{
	LoadTextures();
	LoadAudios();
}
void FileSys::LoadAudios()
{

	std::string path = "Projects/" + currentProject + "/audioinfo.txt";
	std::string audioFolder = "Projects/" + currentProject + "/Resources/Audios";
	if (!fs::exists(audioFolder))
	{
		fs::create_directory(audioFolder);
	}
	audioFolder += "/";
	if (!fs::exists(path) || fs::is_directory(path))
	{
		return;
	}
	std::vector<std::vector<std::string>> audios;
	Parser::ParseString(path, audios);
	for (std::vector<std::string> audio : audios)
	{
		if (audio.size() < 2) { continue; }
		AudioLoader::LoadAudio(audio[0], audioFolder + audio[1]);
	}
}
void FileSys::LoadTextures()
{
	std::string path = "Projects/" + currentProject + "/textureinfo.txt";
	std::string textureFolder = "Projects/" + currentProject + "/Resources/Textures";
	if (!fs::exists(textureFolder))
	{
		fs::create_directory(textureFolder);
	}
	textureFolder += "/";
	if (!fs::exists(path) || fs::is_directory(path))
	{
		return;
	}
	std::vector<std::vector<std::string>> textures;
	Parser::ParseString(path, textures);
	PRINT_2D_ARRAY_STR(textures);
	for (std::vector<std::string> texture : textures)
	{
		if (texture.size() < 2) { continue; }
		TextureLoader::LoadTexture(texture[0], textureFolder + texture[1]);
	}

}


void FileSys::SaveAssets()
{
	SaveTextures();
	SaveAudios();
}
void FileSys::SaveTextures()
{
	std::string writeStr = "";
	for (auto& texture : TextureLoader::GetMap())
	{
		writeStr += texture.first + " " + fs::path(texture.second->GetPath()).filename().string() + ";\n";

	}

	std::ofstream out("Projects/" + currentProject + "/textureinfo.txt");
	out << writeStr;
	out.close();
}
void FileSys::SaveAudios()
{
	std::string writeStr = "";
	for (auto& audio : AudioLoader::GetMap())
	{
		writeStr += audio.first + " " + fs::path(audio.second->GetFilePath()).filename().string() + ";\n";
	}
	std::ofstream out("Projects/" + currentProject + "/audioinfo.txt");
	out << writeStr;
	out.close();
}

void FileSys::LoadConfigs()
{
	TRACE("LOADCONFIGS");
	if (currentProject == "")
	{
		return;
	}
	std::string path = "Projects/" + currentProject + "/configs.txt";
	if (!fs::exists(path) || !fs::is_regular_file(path))
	{
		return;
	}
	TRACE("CONFIG FILE EXISTS")
		std::vector<std::vector<std::string>> parsedConfigs;
	Parser::ParseString(path, parsedConfigs);
	TRACE("PARSED CONFIGS:");
	PRINT_2D_ARRAY_STR_ADVANCED(parsedConfigs);
	for (auto& line : parsedConfigs)
	{
		TRACE("LINE:");
		PRINT_ARRAY_STR(line);
		int valueInt = 0;
		std::string valueStr = "";
		bool valueBool = false;
		if (line[0] == "END") break;
		if (line.size() < 2 && !(line[0] == "FULLSCREEN")) continue;
		if (line[0] == "VIRTUALWIDTH")
		{
			if (ConvertStringToNum<int>(line[1], valueInt)) GameOptions::virtualWidth = valueInt;
		}
		else if (line[0] == "VIRTUALHEIGHT")
		{
			if (ConvertStringToNum<int>(line[1], valueInt)) GameOptions::virtualHeight = valueInt;
		}
		else if (line[0] == "DEFAULTWIDTH")
		{
			if (ConvertStringToNum<int>(line[1], valueInt)) GameOptions::defaultWidth = valueInt;
		}
		else if (line[0] == "DEFAULTHEIGHT")
		{
			if (ConvertStringToNum<int>(line[1], valueInt)) GameOptions::defaultHeight = valueInt;
		}
		else if (line[0] == "NAME")
		{
			GameOptions::gameName = line[1];
			TRACE("gamename Load: " + line[1]);
		}
		else if (line[0] == "LOGOFILE")
		{
			GameOptions::logoFile = line[1];
		}
		else if (line[0] == "DEFAULTSCENE")
		{
			GameOptions::defaultScene = line[1];
		}
		else if (line[0] == "FULLSCREEN")
		{
			GameOptions::fullScreen = true;
		}
	}

}

void FileSys::SaveConfigs()
{
	TRACE("Saving configs for project: " + currentProject);
	if (currentProject == "")
	{
		return;
	}
	std::string path = "Projects/" + currentProject + "/configs.txt";
	std::ofstream out(path);
	if (!out.is_open())
	{
		WARN("Could not open configs file for writing");
		return;
	}
	out << "VIRTUALWIDTH " << GameOptions::virtualWidth << ";\n";
	out << "VIRTUALHEIGHT " << GameOptions::virtualHeight << ";\n";
	out << "DEFAULTWIDTH " << GameOptions::defaultWidth << ";\n";
	out << "DEFAULTHEIGHT " << GameOptions::defaultHeight << ";\n";
	out << "NAME " << GameOptions::gameName << ";\n";
	out << "LOGOFILE " << GameOptions::logoFile << ";\n";
	out << "DEFAULTSCENE " << GameOptions::defaultScene << ";\n";
	if (GameOptions::fullScreen)
		out << "FULLSCREEN;\n";
	out << "END;";
	out.close();
}