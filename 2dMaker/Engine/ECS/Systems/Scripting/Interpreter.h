#include "../Sysbase.h"
#include "../../../Events/EventManager.h"
#include "Tokens.h"
#include "../Physics/ColliderFunctions.h"


namespace D2Maker
{

	enum class Action
	{
		NONE = -1,
		MOVE = 0,
		MOVE_RELATIVE = 1,
		SET = 2,
		SET_RELATIVE = 3
	};

	enum class Coord
	{
		NONE = -1,
		X = 0,
		Y = 1,
		THETA = 2,
	};

	enum class Conditional
	{
		NONE=-1,
		KEYPRESS=0,
		COLLIDE=1
	};

	enum class InstructionType
	{
		NONE = -1,
		PHYSICS=0,
		AUDIO=1,
	};


	static class Interpreter
	{
	public:
		static void InterpretTokens(std::vector<std::vector<std::string>> tokens,Entity entity,EntityManager&em)
		{
			//TRACE("outside for");
			//PRINT_2D_ARRAY_STR(tokens);
			

			for (int i = 0;i < tokens.size();i++)
			{
				//TRACE("Processing line: " + std::to_string(i + 1));
				//PRINT_ARRAY_STR(tokens[i]);
				if (tokens[i].size() > 0)
				{
					
					//TRACE("executing line: " + std::to_string(i + 1));
					if (tokens[i][0] == "KEYPRESS" || tokens[i][0] == "COLLIDE")
					{
						//TRACE("executing line: " + std::to_string(i + 1));
						ExecuteConditional(tokens[i],entity,em);
						
					}
					else if (tokens[i][0] == "TIMER")
					{
						ExecuteTimedInstruction(tokens[i], entity, em);
					}
					else
					{
						//TRACE("executing line: " + std::to_string(i + 1));
						InterpretInstruction(tokens[i], 0,entity,em);
					}
					
				}
			}
		}
	private:
		static void InterpretInstruction(std::vector<std::string> instruction, int startIndex,Entity entity,EntityManager&em)
		{
			const std::unordered_set<std::string>  tokens = { "X","Y","MOVE","SET","RELATIVE","THETA","PLAY","TIMER"};
			if (instruction.size()-startIndex < 2)
			{
				return;
			}
			if (instruction[0 + startIndex] == "SET" || instruction[0 + startIndex] == "MOVE")
			{
				Action action = Action::NONE;
				Coord coord = Coord::NONE;
				float value = 0.0f;
				for (int i = startIndex;i < instruction.size();i++)
				{
					if (instruction[i] == "MOVE" || instruction[i] == "SET")
					{
						if (action == Action::NONE)
						{
							if (instruction[i] == "SET")
							{
								action = Action::SET;
								//TRACE("set");
							}
							else
							{
								action = Action::MOVE;
							}

						}
						else
						{
							return;
						}
					}
					else if (i - 1 >= startIndex && ((instruction[i] == "X") || (instruction[i] == "Y") || (instruction[i] == "THETA")))
					{
						if (instruction[i - 1] == "MOVE" || instruction[i - 1] == "SET")
						{
							if (instruction[i] == "X")
							{
								coord = Coord::X;
							}
							else if (instruction[i] == "THETA")
							{
								coord = Coord::THETA;
							}
							else
							{
								coord = Coord::Y;
							}
						}
						else
						{
							return;
						}
					}
					else if (instruction[i] == "RELATIVE")
					{
						if (i - 3 >= startIndex)
						{
							if (tokens.find(instruction[i - 1]) == tokens.end() && instruction[i - 3] == "SET")
							{
								if (action == Action::SET)
								{
									action = Action::SET_RELATIVE;
								}
								else
								{
									return;
								}
							}
							else if (tokens.find(instruction[i - 1]) == tokens.end() && instruction[i - 3] == "MOVE")
							{
								if (action == Action::MOVE)
								{
									action = Action::MOVE_RELATIVE;
								}
								else
								{
									return;
								}
							}
							else
							{
								return;
							}
						}
						else
						{
							return;
						}
					}
					else
					{
						if (!ConvertStringToNumber(instruction[i], value))
						{
							return;
						}
					}


				}
				//TRACE("value=" + std::to_string(value));
				if (action != Action::NONE && coord != Coord::NONE && value != 0.0f ||
					(action == Action::SET && coord != Coord::NONE))
				{

					ExecutePhysicsInstruction(action, coord, value, entity, em);
				}
			}
			else if (instruction[0 + startIndex] == "PLAY")
			{
				if (instruction.size() - startIndex != 2)
				{
					return;
				}
				ExecuteAudioInstruction(instruction[1 + startIndex]);
				
			}
			
			


		}

		static void ExecuteAudioInstruction(const std::string& name)
		{
			if (AudioLoader::Exists(name))
			{
				AudioLoader::PlayAudio(name);
			}
		}


		static void ExecutePhysicsInstruction(Action action,Coord coord,float value,Entity entity,EntityManager&em)
		{
			if (action == Action::MOVE || action == Action::MOVE_RELATIVE)
			{
				if (!em.hasComponent<Velocity>(entity))
				{
					return;
				}
				Velocity* vel=em.getComponent<Velocity>(entity);
				if (coord == Coord::X)
				{
					if (action == Action::MOVE)
					{
						vel->dx = value;
					}
					else
					{
						vel->dx += value;
					}
					
				}
				else if(coord == Coord::Y)
				{
					if (action == Action::MOVE)
					{
						vel->dy = value;
					}
					else
					{
						vel->dy += value;
					}
					
				}
				else if (coord == Coord::THETA)
				{
					if (action == Action::MOVE)
					{
						vel->dtheta = value;
						
					}
					else
					{
						vel->dtheta += value;
					}
				}
			}
			else if(action==Action::SET || action == Action::SET_RELATIVE)
			{
				//TRACE("setting");
				if (!em.hasComponent<Transform>(entity))
				{
					return;
				}
				Transform* transform = em.getComponent<Transform>(entity);
				if (coord == Coord::X)
				{
					if (action == Action::SET)
					{
						transform->x = value;
					}
					else
					{
						transform->x += value;
					}
				}
				if (coord == Coord::Y)
				{
					if (action == Action::SET)
					{
						transform->y = value;
					}
					else
					{
						transform->y += value;
					}
				}
				if (coord == Coord::THETA)
				{
					if (action == Action::SET)
					{
						transform->rotationDegrees = value;
					}
					else
					{
						transform->rotationDegrees += value;
					}
				}
			}
		}

		static void ExecuteConditional(std::vector<std::string> instruction,Entity entity, EntityManager&em)
		{
			//TRACE("executing conditional");
			//PRINT_ARRAY_STR(instruction);
			if (instruction.size() < 2)
			{
				return;
			}
			if (instruction[0] == "KEYPRESS")
			{
				auto it = Tokens::keyMap.find(instruction[1]);
				if (it != Tokens::keyMap.end()) 
				{
					Keys key = it->second;
					if (EventManager::IsKeyPressed(key))
					{
						//TRACE("KEY PRESSED:" + instruction[1]);
						InterpretInstruction(instruction, 2, entity, em);
					}
					
				}
				else
				{
					return;
				}
			}
			else if (instruction[0]=="COLLIDE")
			{
				auto it = Tokens::keyMap.find(instruction[1]);
				if (it != Tokens::keyMap.end())
				{
					if (ColliderFunctions::CheckCollisionWithName(entity, em, instruction[1]))
					{
						InterpretInstruction(instruction, 2, entity, em);
					}

				}
				else
				{
					return;
				}
			}
		}
		static void ExecuteTimedInstruction(std::vector<std::string> instruction, Entity entity, EntityManager& em)
		{
			float value;
			if (instruction.size() < 2)
			{
				return;
			}
			if (!em.hasComponent<Timer>(entity))
			{
				return;
			}
			if (!ConvertStringToNumber(instruction[1], value))
			{
				return;
			}
			Timer* timer = em.getComponent<Timer>(entity);
			timer->accumulator += DeltaTime::Get();
			if (timer->accumulator >= value)
			{
				timer->accumulator -= value;
				InterpretInstruction(instruction, 2, entity, em);
			}


			
		}

		static bool ConvertStringToNumber(const std::string& string, float& value)
		{
			std::istringstream iss(string);
			iss >> std::noskipws >> value;

			if (!(iss.eof() && !iss.fail()))
			{
				return false;
			}

			return true;
		}
	};
}