#include "LuaSprite.h"
#include "LuaUntrackedUserData.h"
#include "LuaClassTracker.h"
#include "LuaBaseSprite.h"
#include "LuaUtils.h"
#include "Sprite.h"

namespace ncine {

namespace LuaNames {
namespace Sprite {
	static const char *Sprite = "sprite";
}}

///////////////////////////////////////////////////////////
// PUBLIC FUNCTIONS
///////////////////////////////////////////////////////////

void LuaSprite::expose(LuaStateManager *stateManager)
{
	lua_State *L = stateManager->state();
	lua_newtable(L);

	if (stateManager->apiType() == LuaStateManager::ApiType::FULL)
	{
		LuaClassTracker<Sprite>::exposeDelete(L);
		LuaUtils::addFunction(L, LuaNames::newObject, newObject);
		LuaUtils::addFunction(L, LuaNames::cloneNode, cloneNode);
	}

	exposeFunctions(L);

	lua_setfield(L, -2, LuaNames::Sprite::Sprite);
}

void LuaSprite::release(void *object)
{
	Sprite *sprite = reinterpret_cast<Sprite *>(object);
	delete sprite;
}

///////////////////////////////////////////////////////////
// PRIVATE FUNCTIONS
///////////////////////////////////////////////////////////

void LuaSprite::exposeFunctions(lua_State *L)
{
	LuaBaseSprite::exposeFunctions(L);
}

int LuaSprite::newObject(lua_State *L)
{
	SceneNode *parent = LuaUntrackedUserData<SceneNode>::retrieveOrNil(L, -4);
	Texture *texture = LuaUntrackedUserData<Texture>::retrieveOrNil(L, -3);
	const float x = LuaUtils::retrieve<float>(L, -2);
	const float y = LuaUtils::retrieve<float>(L, -1);

	LuaClassTracker<Sprite>::newObject(L, parent, texture, x, y);

	return 1;
}

int LuaSprite::cloneNode(lua_State *L)
{
	const Sprite *sprite = LuaUntrackedUserData<Sprite>::retrieve(L, -1);

	if (sprite)
		LuaClassTracker<Sprite>::cloneNode(L, *sprite);
	else
		LuaUtils::pushNil(L);

	return 1;
}

}
