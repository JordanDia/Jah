#include "jahpch.h"
#include "ScriptGlue.h"
#include "ScriptEngine.h"
#include "Core/Core.h"

#include "Core/UUID.h"

#include "mono/metadata/object.h"
#include <string>
#include "Scene/Entity.h"
#include "Core/Input.h"
#include <Core\KeyCodes.h>

namespace Jah {
	
#define JAH_ADD_INTERNAL_CALL(Name) mono_add_internal_call("Jah.InternalCalls::" #Name, Name)

#pragma region Native

	static void NativeLog(MonoString* string, int parameter)
	{
		char* cStr = mono_string_to_utf8(string);
		std::string str(cStr);

		mono_free(cStr);
		JAH_CORE_TRACE("NativeLog: {} | Parameter: {}", str, parameter);
	}

	static void NativeLog_Vector(glm::vec3* vec, glm::vec3* outResult)
	{
		JAH_CORE_TRACE("Value: {0}", glm::to_string(*vec));
		*outResult = glm::normalize(*vec);
	}

	static float NativeLog_VectorDot(glm::vec3* vec)
	{
		JAH_CORE_TRACE("Value: {0}", glm::to_string(*vec));
		return glm::dot(*vec, *vec);
	}

#pragma endregion 

#pragma region Entity

	static void Entity_GetTranslation(UUID uuid, glm::vec3* outTranslation)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		*outTranslation = entity.GetComponent<TransformComponent>().Translation;
	}

	static void Entity_SetTranslation(UUID uuid, glm::vec3* translation)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		entity.GetComponent<TransformComponent>().Translation = *translation;
	}

#pragma endregion

#pragma region Input

	static bool Input_IsKeyDown(KeyCode keycode)
	{
		return Input::IsKeyPressed(keycode);
	}

#pragma endregion

#pragma region ScriptGlue

	namespace ScriptGlue {

		void RegisterFunctions()
		{
			JAH_ADD_INTERNAL_CALL(NativeLog);
			JAH_ADD_INTERNAL_CALL(NativeLog_Vector);
			JAH_ADD_INTERNAL_CALL(NativeLog_VectorDot);


			JAH_ADD_INTERNAL_CALL(Entity_GetTranslation);
			JAH_ADD_INTERNAL_CALL(Entity_SetTranslation);

			JAH_ADD_INTERNAL_CALL(Input_IsKeyDown);
		}

	}

#pragma endregion

}