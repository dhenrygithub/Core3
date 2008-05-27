/*
Copyright (C) 2007 <SWGEmu>
 
This File is part of Core3.
 
This program is free software; you can redistribute 
it and/or modify it under the terms of the GNU Lesser 
General Public License as published by the Free Software
Foundation; either version 2 of the License, 
or (at your option) any later version.
 
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of 
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
See the GNU Lesser General Public License for
more details.
 
You should have received a copy of the GNU Lesser General 
Public License along with this program; if not, write to
the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
 
Linking Engine3 statically or dynamically with other modules 
is making a combined work based on Engine3. 
Thus, the terms and conditions of the GNU Lesser General Public License 
cover the whole combination.
 
In addition, as a special exception, the copyright holders of Engine3 
give you permission to combine Engine3 program with free software 
programs or libraries that are released under the GNU LGPL and with 
code included in the standard release of Core3 under the GNU LGPL 
license (or modified versions of such code, with unchanged license). 
You may copy and distribute such a system following the terms of the 
GNU LGPL for Engine3 and the licenses of the other code concerned, 
provided that you include the source code of that other code when 
and as the GNU LGPL requires distribution of source code.
 
Note that people who make modified versions of Engine3 are not obligated 
to grant this special exception for their modified versions; 
it is their choice whether to do so. The GNU Lesser General Public License 
gives permission to release a modified version without this exception; 
this exception also makes it possible to release a modified version 
which carries forward this exception.
*/

#include "../../ZoneClient.h"
#include "../../ZoneServer.h"

#include "../../objects.h"
#include "../../packets.h"

#include "../../managers/player/PlayerManager.h"
#include "../../managers/player/ProfessionManager.h"
#include "../../managers/crafting/CraftingManager.h"
#include "../../managers/item/ItemManager.h"
#include "../../managers/combat/CombatManager.h"
#include "../../managers/guild/GuildManager.h"
#include "../../managers/group/GroupManager.h"
#include "../../managers/planet/PlanetManager.h"
#include "../../managers/resource/ResourceManager.h"
#include "../../managers/loot/LootManager.h"
#include "../../managers/sui/SuiManager.h"

#include "../../../chat/ChatManager.h"
#include "../../../ServerCore.h"

#include "Races.h"

#include "../terrain/Terrain.h"
#include "../terrain/RegionNames.h"

#include "../tangible/weapons/WeaponImplementation.h"

#include "Player.h"
#include "PlayerImplementation.h"

#include "PlayerHAM.h"

#include "events/PlayerLogoutEvent.h"
#include "events/PlayerDisconnectEvent.h"
#include "events/PlayerRecoveryEvent.h"
#include "events/CommandQueueActionEvent.h"
#include "events/ChangeFactionEvent.h"
#include "events/CenterOfBeingEvent.h"
#include "events/SurveyEvent.h"
#include "events/EntertainerEvent.h"
#include "events/SampleEvent.h"

#include "../creature/events/DizzyFallDownEvent.h"

#include "professions/Certification.h"

#include "../../managers/combat/CommandQueueAction.h"
#include "../../managers/skills/SkillManager.h"

PlayerImplementation::PlayerImplementation() : PlayerServant(0) {
	init();

	zoneID = 1;
	//zoneID = 8;
		
	setHeight(1.0f);
}

PlayerImplementation::PlayerImplementation(uint64 cid) : PlayerServant(baseID = cid << 32) {
	init();

	characterID = cid;
}

PlayerImplementation::~PlayerImplementation() {
	clearBuffs(false);
	
	for (int i = 0; i < suiBoxes.size(); ++i) {
		SuiBox* box =  suiBoxes.get(i);
		box->finalize();
	}

	if (playerObject != NULL) {
		playerObject->finalize();
		playerObject = NULL;
	}
	
	if (inventory != NULL) {
		info("undeploying player inventory");

		inventory->finalize();
		inventory = NULL;
	}
	
	if (datapad != NULL) {
		info("undeploying player datapad");

		datapad->finalize();
		datapad = NULL;
	}
	
	if (hairObj != NULL) {
		hairObj->finalize();
		hairObj = NULL;
	}

	if (centerOfBeingEvent != NULL) {
		server->removeEvent(centerOfBeingEvent);
		
		delete centerOfBeingEvent;
		centerOfBeingEvent = NULL;
	}
	
	server->getZoneServer()->increaseTotalDeletedPlayers();
	
	info("undeploying player");
}

void PlayerImplementation::init() {
	objectType = PLAYER;
	
	owner = NULL;
	zone = NULL;
	
	onlineStatus  = LOGGINGIN;

	// objects
	playerObject = NULL;
	
	disconnectEvent = NULL;
	logoutEvent = NULL;
	
	recoveryEvent = new PlayerRecoveryEvent(this);
	changeFactionEvent = NULL;
	
	datapad = NULL;

	stfName = "species";
	
	// modifiers
	weaponSpeedModifier = 1;

	// constants
	itemShift = 100;
	
	// pvp stuff
	deathCount = 0;
	pvpRating = 0;
	duelList.setInsertPlan(SortedVector<Player*>::NO_DUPLICATE);
	
	// profession
	skillPoints = 0;
	skillBoxesToSave.setInsertPlan(SortedVector<SkillBox*>::NO_DUPLICATE);
	certificationList.setInsertPlan(SortedVector<Certification*>::NO_DUPLICATE);
	
	// Draft Schematics
	draftSchematicList.setInsertPlan(SortedVector<DraftSchematic*>::NO_DUPLICATE);
	draftSchematicList.setNullValue(NULL);
	draftSchematicUpdateCount = 0;
	
	//Crafting
	currentCraftingTool = NULL;
	
	//temp
	factionRank = "Sexy Tester";
	rebelPoints = 0; 
 	imperialPoints = 0;
 	
 	regionId = 31; //Ancorhead I think lols.

 	conversatingCreature = NULL;
 	
 	chatRooms.setInsertPlan(SortedVector<ChatRoom*>::NO_DUPLICATE);
 	
 	centered = false;
 	centerOfBeingEvent = new CenterOfBeingEvent(this);
 	
	lastTestPositionX = 0.f;
	lastTestPositionY = 0.f;
	
	tradeRequestedPlayer = 0;
	moneyToTrade = 0;
	acceptedTrade = false;
	verifiedTrade = false;

	surveyEvent = NULL;
	entertainerEvent = NULL;
	sampleEvent = NULL;
	firstSampleEvent = NULL;
	surveyWaypoint = NULL;
	surveyTool = NULL;
	cancelSample = false;
	surveyErrorMessage = false;
	sampleErrorMessage = false;
	
	suiBoxes.setInsertPlan(SortedVector<SuiBox*>::NO_DUPLICATE);
	suiBoxes.setNullValue(NULL);
	
	suiBoxNextID = 0;
	
	setLogging(false);
	setGlobalLogging(true);
}

void PlayerImplementation::create(ZoneClient* client) {
	playerObject = new PlayerObject(_this);

	setClient(client);
	client->setPlayer(_this);

	string logName = "Player = " + firstName;

	setLockName(logName);
	//client->setLockName("ZoneClient = " + firstName);

	setLoggingName(logName);

	info("created player");
}

void PlayerImplementation::refuseCreate(ZoneClient* client) {
	info("name refused for character creation");

	BaseMessage* msg = new ClientCreateCharacterFailed("name_declined_in_use");
	client->sendMessage(msg);
		
	client->disconnect();
}

void PlayerImplementation::load(ZoneClient* client) {
	try {
		wlock();
	
		setLoggingIn();
	
		owner = client;
		client->setPlayer(_this);

		stringstream logName;
		logName << "Player = " << firstName << " (0x" << hex << objectID << dec << ")";
	
		setLockName(logName.str());
		setLoggingName(logName.str());

		info("loading player");

		loadItems();

		setLoggingIn();
		
		Zone* zone = server->getZoneServer()->getZone(zoneID);
		insertToZone(zone);
		
		unlock();
	} catch (Exception& e) {
		unlock();

		error("logging in character");
		error(e.getMessage());

		client->disconnect();
	}
}

void PlayerImplementation::reload(ZoneClient* client) {
	try {
		wlock();

		if (isLoggingOut()) {
			if (disconnectEvent != NULL) {
				server->removeEvent(disconnectEvent);
				
				delete disconnectEvent;
				disconnectEvent = NULL;
			}
			
			info("reloading player");
		} else if (logoutEvent != NULL) {
			server->removeEvent(logoutEvent);
				
			delete logoutEvent;
			logoutEvent = NULL;
		} else if (isLoggingIn()) {
			info("kicking player over network failure");
			
			unlock();
			
			owner->disconnect();
			
			wlock();
			
			info("loading player");
		} else if (isOnline()) {
			info("already loaded");
			
			unlock();

			client->disconnect();
			return;
		} else if (isOffline()) {
			info("reloading player from Cache");
		}
	
		owner = client;
		client->setPlayer(_this);

		setLoggingIn();
	
		Zone* zone = server->getZoneServer()->getZone(zoneID);
		
		if (isInQuadTree())
			reinsertToZone(zone);
		else
			insertToZone(zone);

		clearBuffs(true);
		
		resetArmorEncumbrance();
		
		activateRecovery();
		
		unlock();
		
	} catch (Exception& e) {
		error("reconnecting in character");
		error(e.getMessage());

		unlock();

		client->disconnect();
	}
}

void PlayerImplementation::unload() {
	info("unloading player");

	clearCombatState(); // remove the defenders
	
	tradeItems.removeAll();
	
	clearBuffs(false);
	
	if (firstSampleEvent != NULL) {
		if (firstSampleEvent->isQueued())
			server->removeEvent(firstSampleEvent);
			
		delete firstSampleEvent;
		firstSampleEvent = NULL;
	}
	
	if (sampleEvent != NULL) {
		if (sampleEvent->isQueued())
			server->removeEvent(sampleEvent);
			
		delete sampleEvent;
		sampleEvent = NULL;
	}
	
	if (entertainerEvent != NULL) {
		if (entertainerEvent->isQueued())
			server->removeEvent(entertainerEvent);
			
		delete entertainerEvent;
		entertainerEvent = NULL;
	}
	
	// remove from group
	if (group != NULL && zone != NULL) {
		GroupManager* groupManager = server->getGroupManager();
		groupManager->leaveGroup(group, _this);
	}
	
	// remove from chat rooms
	while (!chatRooms.isEmpty()) {
		ChatRoom* room = chatRooms.get(0);
		room->removePlayer(_this, false);
	}

	// unload mount from world
	if (mount != NULL) {
		MountCreature* mnt = mount;
		mnt->wlock();
		
		mnt->store(false);
		
		mnt->unlock();
	}

	if (zone != NULL) {
		ZoneServer* zserver = zone->getZoneServer();
		
		ItemManager* itemManager = zserver->getItemManager();
		itemManager->unloadPlayerItems(_this);	

		PlayerManager* playerManager = zserver->getPlayerManager();
		playerManager->unload(_this);
		
		if(isInQuadTree()) {
	
			clearDuelList();
	
			if (isDancing())
				stopDancing();
			else if (isPlayingMusic())
				stopPlayingMusic();
				
			if (isWatching())
				stopWatch(watchID);
				
			if (isListening())
				stopListen(listenID);
	
			if (dizzyFallDownEvent != NULL && dizzyFallDownEvent->isQueued()) {
				server->removeEvent(dizzyFallDownEvent);
				
				delete dizzyFallDownEvent;
				dizzyFallDownEvent = NULL;
			}
		
			if (changeFactionEvent != NULL) {
				server->removeEvent(changeFactionEvent);
				delete changeFactionEvent;
				
				changeFactionEvent = NULL;
			}
			
			clearTarget();
			
			removeFromZone(true);
			//zone = NULL;
		}
	}
}

void PlayerImplementation::logout(bool doLock) {
	wlock(doLock);
	
	if (disconnectEvent == NULL) {
		info("creating disconnect event");		
		disconnectEvent = new PlayerDisconnectEvent(this);
		
		if (isLoggingOut()) {
			server->addEvent(disconnectEvent, 10);
		} else {
			server->addEvent(disconnectEvent, 1000);
			setLoggingOut();
		}
	}
	
	unlock(doLock);
}

void PlayerImplementation::userLogout(int msgCounter) {
	if (msgCounter < 0 || msgCounter > 3)
		msgCounter = 3;
		
	if (!isSitting()) {
		changePosture(CreatureObjectImplementation::SITTING_POSTURE);
	}
	
	if (!isInCombat() && isSitting()) {
		logoutEvent = new PlayerLogoutEvent(_this, msgCounter);
		
		switch (msgCounter) {
		case 3:
			sendSystemMessage("You can safely log out in 30 seconds...");
			server->addEvent(logoutEvent, 15000);
			break;
		case 2:
			sendSystemMessage("You can safely log out in 15 seconds...");
			server->addEvent(logoutEvent, 10000);
			break;
		case 1:
			sendSystemMessage("You can safely log out in 5 seconds...");
			server->addEvent(logoutEvent, 5000);
			break;
		case 0:  // Disconnect!!!
			info("Safe Logout");
			
			ClientLogout* packet = new ClientLogout();
			sendMessage(packet);
			break;
		}
	} else {
		if (logoutEvent != NULL){
			server->removeEvent(logoutEvent);
			delete logoutEvent;
			
			logoutEvent = NULL;
		}
		
		if (isInCombat())
			sendSystemMessage("Can not log out while in combat.");
		else if (!isSitting())
			sendSystemMessage("You must be sitting to log out.");
	}
}

void PlayerImplementation::disconnect(bool closeClient, bool doLock) {
	try {
		wlock(doLock);
		
		// User is disconnecting in combat.  Will remain LD.
		if (isInCombat() && !isLinkDead()) {
			info("link dead");
			
			setLinkDead();
		} else {
			info("disconnecting player");
			
			unload();
			
			setOffline();
		}
		
		if (disconnectEvent != NULL)
			disconnectEvent = NULL;
			
		if (logoutEvent != NULL) {
			server->removeEvent(logoutEvent);
			delete logoutEvent;
			
			logoutEvent = NULL;
		}
	
		if (closeClient && owner != NULL) {
			owner->closeConnection();
		}
		
		owner = NULL;
			
		unlock(doLock);
	} catch (Exception& e) {
		error("Exception on Player::disconnect()");
		e.printStackTrace();

		clearDisconnectEvent();
		unlock(doLock);
	} catch (...) {
		error("unreported exception on Player::disconnect()");
		
		clearDisconnectEvent();
		unlock(doLock);
	}
}

void PlayerImplementation::createItems() {
	inventory = new Inventory(_this);
		
	datapad = new Datapad(_this);

	ItemManager* itemManager = zone->getZoneServer()->getItemManager();
	itemManager->loadDefaultPlayerItems(_this);
	itemManager->loadDefaultPlayerDatapadItems(_this);
	
	if (!hairObject.empty()) {
		hairObj = new HairObject(_this, String::hashCode(hairObject), unicode("hair"), "hair");
		hairObj->setCustomizationString(hairData);
	}
}

void PlayerImplementation::loadItems() {
	inventory = new Inventory(_this);
		
	datapad = new Datapad(_this);

	ItemManager* itemManager = zone->getZoneServer()->getItemManager();
	itemManager->loadPlayerItems(_this);

	if (!hairObject.empty()) {
		hairObj = new HairObject(_this, String::hashCode(hairObject), unicode("hair"), "hair");
		hairObj->setCustomizationString(hairData);
	}
}

void PlayerImplementation::createBaseStats() {
	//TODO: bit hackish, find more clean solution
	
	int hamValues[9];
	
	if (startingProfession == "crafting_artisan")
		memcpy(hamValues, professionHams[0], sizeof(hamValues));
	else if (startingProfession == "combat_brawler")
		memcpy(hamValues, professionHams[1], sizeof(hamValues));
	else if (startingProfession == "social_entertainer")
		memcpy(hamValues, professionHams[2], sizeof(hamValues));
	else if (startingProfession == "combat_marksman")
		memcpy(hamValues, professionHams[3], sizeof(hamValues));
	else if (startingProfession == "science_medic")
		memcpy(hamValues, professionHams[4], sizeof(hamValues));
	else if (startingProfession == "outdoors_scout")
		memcpy(hamValues, professionHams[5], sizeof(hamValues));
	else
		memcpy(hamValues, professionHams[6], sizeof(hamValues));

	int race = Races::getRaceID(raceFile);

	// Add the race mods
	int hamMods[9];
	memcpy(hamMods, raceHamMods[race % 10], sizeof(hamMods));
		
	for (int i = 0; i < 9; i++)
		hamValues[i] += hamMods[i];
	
	baseHealth = hamValues[0];
	baseStrength = hamValues[1];
	baseConstitution = hamValues[2];
	baseAction = hamValues[3];
	baseQuickness = hamValues[4];
	baseStamina = hamValues[5];
	baseMind = hamValues[6];
	baseFocus = hamValues[7];
	baseWillpower = hamValues[8];
}

void PlayerImplementation::trainStartingProfession() {
	trainSkillBox(startingProfession + "_novice");
	
	ProfessionManager* professionManager = server->getProfessionManager();
	professionManager->loadDefaultSkills(this);
}

void PlayerImplementation::decayInventory() {
	if (inventory != NULL)
		for (int i = 0; i < inventory->objectsSize(); i++) {
			TangibleObject* item = ((TangibleObject*) inventory->getObject(i));
			
			if (item->isWeapon() && item->isEquipped())
				((Weapon*)item)->decayWeapon(5);
			else if (item->isArmor() && item->isEquipped())
				((Armor*)item)->decayArmor(5);
			
			item->sendTo(_this);
		}
}

void PlayerImplementation::resetArmorEncumbrance() {
	
	healthEncumbrance = 0;
	actionEncumbrance = 0;
	mindEncumbrance = 0;
	
	for (int i=0; i < inventory->objectsSize(); i++) {
		TangibleObject* item = ((TangibleObject*) inventory->getObject(i));
		
		if (item->isEquipped() && item->isArmor()) {
			item->setEquipped(false);
			unsetArmorSkillMods((Armor*)item);
			changeArmor(item->getObjectID(), true);
		
		}
	}
}

void PlayerImplementation::sendToOwner() {
	if (faction != 0)
		pvpStatusBitmask |= OVERT_FLAG;

	CreatureObjectImplementation::sendToOwner(_this, false);

	playerObject->sendToOwner();

	sendItemsTo(_this);
	sendPersonalContainers();
	sendGuildList();
	
	CreatureObjectImplementation::close(owner);
	
	if (parent != NULL)
		parent->sendTo(_this);
}

void PlayerImplementation::sendTo(Player* player, bool doClose) {
	CreatureObjectImplementation::sendTo(player, false);

	playerObject->sendTo(player);

	if (doClose)
		CreatureObjectImplementation::close(player->getClient());
}

void PlayerImplementation::sendPersonalContainers() {
	//datapad
	datapad->sendTo(_this, false);
		
	for (int i = 0; i < datapad->objectsSize(); ++i) {
		SceneObject* item = datapad->getObject(i);
		item->sendTo(_this);
	}
	
	datapad->close(_this);
}

void PlayerImplementation::insertToZone(Zone* zone) {
	PlayerImplementation::zone = zone;
	
	if (owner == NULL)
		return;

	try {
		zone->lock();

		info("inserting to zone");
		
		zone->registerObject(_this);

		owner->balancePacketCheckupTime();
		
		sendToOwner();

		if (parent != NULL) {
			BuildingObject* building = (BuildingObject*) parent->getParent();
			insertToBuilding(building);
			building->notifyInsertToZone(_this);
		} else {
			zone->insert(this);
			zone->inRange(this, 128);
		}
		
		owner->resetPacketCheckupTime();

		zone->unlock();
	} catch (...) {
		error("exception Player::insertToZone(Zone* zone)");

		zone->unlock();
	}
}

void PlayerImplementation::insertToBuilding(BuildingObject* building, bool doLock) {
	if (owner == NULL || isInQuadTree() || !parent->isCell())
		return;
	
	try {
		//building->lock(doLock);

		info("inserting to building");
		
		((CellObject*)parent)->addChild(_this);
		
		building->insert(this);
		building->inRange(this, 128);
		
		//building->unlock(doLock);
		
		linkType = 0xFFFFFFFF;
		broadcastMessage(link(parent), 128, false);
	
	} catch (...) {
		error("exception PlayerImplementation::insertToBuilding(BuildingObject* building)");

		//building->unlock(doLock);
	}
}

void PlayerImplementation::reinsertToZone(Zone* zone) {
	try {
		zone->lock();

		info("reinserting to zone");

		owner->balancePacketCheckupTime();

		sendToOwner();

    	for (int i = 0; i < inRangeObjectCount(); ++i) {
			QuadTreeEntry* obj = getInRangeObject(i);

			notifyInsert(obj);			
		}

		owner->resetPacketCheckupTime();

		zone->unlock();
	} catch (...) {
		error("exception Player::reinsertToZone(Zone* zone)");

		zone->unlock();
	}
	
}

void PlayerImplementation::updateZone(bool lightUpdate) {
	if (zone == NULL || isIncapacitated() || isDead())
		return;
	
	bool insert = false;
	
	if (isMounted())
		updateMountPosition();
	
	try {
		//info("updating player in Zone");
		
		zone->lock();
		
		if (parent != NULL && parent->isCell()) {
			CellObject* cell = (CellObject*)parent;

			removeFromBuilding((BuildingObject*)cell->getParent());

			parent = NULL;
			insert = true;
		}

		if (insert)
			zone->insert(this);
		else
			zone->update(this);
		
		zone->inRange(this, 128);
		
		if (!isMounted())
			updatePlayerPosition(lightUpdate);

		zone->unlock();
		
		//info("finished player update in Zone");
	} catch (...) {
		error("exception Player::updateZone()");

		zone->unlock();
	}
}

void PlayerImplementation::updateZoneWithParent(uint64 Parent, bool lightUpdate) {
	if (isMounted())
		dismount(true, true);

	if (zone == NULL)
		return;
	
	SceneObject* newParent = parent;
	
	if (parent == NULL || (parent != NULL && parent->getObjectID() != Parent))
		newParent = zone->lookupObject(Parent);

	if (newParent == NULL)
		return;

	bool insert = false;

	try {
		zone->lock();

		if (newParent != parent) {
			if (parent == NULL) {
				zone->remove(this);
				insert = true;
			} else {
				BuildingObject* building = (BuildingObject*) parent->getParent();
				SceneObject* newObj = newParent->getParent();

				if (newObj->isBuilding()) {
					BuildingObject* newBuilding = (BuildingObject*) newObj;

					if (building != newBuilding) {
						removeFromBuilding(building);

						insert = true;
					}
				}
	
				((CellObject*) parent)->removeChild(_this);
			}
			parent = newParent;
			((CellObject*) parent)->addChild(_this);
		}

		BuildingObject* building = (BuildingObject*) parent->getParent();

		if (insert) {
			insertToBuilding(building, false);
		} else {
			building->update(this);
			building->inRange(this, 128);
		}

		updatePlayerPosition(lightUpdate);
		
		zone->unlock();
	} catch (...) {
		zone->unlock();
		error("Exception in PlayerImplementation::updateZoneWithParent");
	}
}

void PlayerImplementation::updatePlayerPosition(bool doLightUpdate) {
	for (int i = 0; i < inRangeObjectCount(); ++i) {
		SceneObject* obj = (SceneObject*) (((SceneObjectImplementation*) getInRangeObject(i))->_getStub());
		
		if (obj != _this && obj->isPlayer()) {
			Player* player = (Player*) obj;
			
			if (doLightUpdate) {
				if (parent != NULL && parent->isCell()) {
					LightUpdateTransformWithParentMessage* umsg = new LightUpdateTransformWithParentMessage(_this);
					player->sendMessage(umsg);
				} else {
					LightUpdateTransformMessage* umsg = new LightUpdateTransformMessage(_this);
					player->sendMessage(umsg);
				}
			} else {
				if (parent != NULL && parent->isCell()) {
					UpdateTransformWithParentMessage* umsg = new UpdateTransformWithParentMessage(_this);
					player->sendMessage(umsg);
				} else {
					UpdateTransformMessage* umsg = new UpdateTransformMessage(_this);
					player->sendMessage(umsg);
				}
			}
		}
	}
}

void PlayerImplementation::updateMountPosition() {
	if (parent == NULL || parent != mount)
		return;
	
	try {
		mount->wlock();
		
		mount->setDirection(directionX, directionZ, directionY, directionW);
		mount->setPosition(positionX, positionZ, positionY);
		
		mount->updateZone();
		
		mount->unlock();
	} catch (...) {
		cout << "Unreported exception in PlayerImplementation::updateMount()\n";
		mount->unlock();
	}
}

void PlayerImplementation::removeFromZone(bool doLock) {
	if (zone == NULL)
		return;

	try {
		zone->lock(doLock);

		info("removing from zone");
		
		if (parent != NULL && parent->isCell()) {
			CellObject* cell = (CellObject*) parent;
			BuildingObject* building = (BuildingObject*)parent->getParent();

			removeFromBuilding(building);
		} else
			zone->remove(this);
		
		for (int i = 0; i < inRangeObjectCount(); ++i) {
			QuadTreeEntry* obj = getInRangeObject(i);

			if (obj != this)
				obj->removeInRangeObject(this);
		}

		removeInRangeObjects();
		
		zone->deleteObject(objectID);
		
		zone->unlock(doLock);
	} catch (...) {
		error("exception Player::removeFromZone(bool doLock)");

		zone->unlock(doLock);
	}
}

void PlayerImplementation::removeFromBuilding(BuildingObject* building, bool doLock) {
	if (building == NULL || !isInQuadTree() || !parent->isCell())
		return;

	try {
		//building->lock(doLock);

		info("removing from building");
		
		broadcastMessage(link(0, 0xFFFFFFFF), 128, false);
		
		((CellObject*)parent)->removeChild(_this);

		building->remove(this);
		
		//building->unlock(doLock);
	} catch (...) {
		error("exception PlayerImplementation::removeFromBuilding(BuildingObject* building, bool doLock)");

		//building->unlock(doLock);
	}
}

void PlayerImplementation::notifyInsert(QuadTreeEntry* obj) {
	SceneObject* scno = (SceneObject*) (((SceneObjectImplementation*) obj)->_getStub());
	
	Player* player;
	Creature* creature;
	TangibleObject* tangible;
	StaticObject* statico;
	AttackableObject* attacko;
	
	if (parent == scno)
		return;
	
	SceneObject* scnoParent = scno->getParent();
	
	if (scnoParent != NULL && scnoParent->isNonPlayerCreature())
		return;
	
	switch (scno->getObjectType()) {
	case SceneObjectImplementation::PLAYER:
		if (objectID == obj->getObjectID())
			break;
		
		player = (Player*) scno;
				
		player->sendTo(_this);
		player->sendItemsTo(_this);

		break;
	case SceneObjectImplementation::NONPLAYERCREATURE:
		creature = (Creature*) scno;
		
		creature->sendTo(_this);
		creature->sendItemsTo(_this);

		break;
	case SceneObjectImplementation::TANGIBLE:
		tangible = (TangibleObject*) scno;
		tangible->sendTo(_this);
		break;
	
	case SceneObjectImplementation::STATIC:
		statico = (StaticObject*) scno;
		statico->sendTo(_this);
		break;
		
	case SceneObjectImplementation::BUILDING:
		BuildingObject* buio = (BuildingObject*) scno;
		buio->sendTo(_this);
		break;
		
	case SceneObjectImplementation::ATTACKABLE:
		attacko = (AttackableObject*) scno;
		attacko->sendTo(_this);
		break;
	}
}

void PlayerImplementation::notifyDissapear(QuadTreeEntry* obj) {
	SceneObject* scno = (SceneObject*) (((SceneObjectImplementation*) obj)->_getStub());
	
	Player* player;
	Creature* creature;
	
	TangibleObject* tano;
	BuildingObject* buio;
	
	switch (scno->getObjectType()) {
	case SceneObjectImplementation::PLAYER:
		player = (Player*) scno;
		player->sendDestroyTo(_this);

		break;
	case SceneObjectImplementation::NONPLAYERCREATURE:
		creature = (Creature*) scno;
		creature->sendDestroyTo(_this);
		
		break;
		
	case SceneObjectImplementation::BUILDING:
		buio = (BuildingObject*) scno;
		buio->sendDestroyTo(_this);
		break;
		
	case SceneObjectImplementation::TANGIBLE:
		tano = (TangibleObject*) scno;
		tano->sendDestroyTo(_this);
		
		break;
	}
}

void PlayerImplementation::switchMap(int planetid) {
	if (zone == NULL)
		return;
	
	if (mount != NULL) {
		MountCreature* mnt = mount;
		mnt->wlock();

		mnt->store(false);

		mnt->unlock();
	}
	
	removeFromZone();
	
	parent = NULL;
	
	setIgnoreMovementTests(5);
	
	zoneID = planetid;
	ZoneServer* server = zone->getZoneServer();
	Zone* zone = server->getZone(zoneID);

	terrainName = Terrain::getTerrainName(zoneID);
	
	insertToZone(zone);
}	

void PlayerImplementation::doWarp(float x, float y, float z, float randomizeDistance, uint64 parentID) {
	if (zone == NULL)
		return;

	removeFromZone();
	
	parent = NULL;
	
	positionX = x;
	positionY = y;
	positionZ = z;
	
	if (parentID != 0)
		parent = zone->lookupObject(parentID);
	
	setIgnoreMovementTests(10);
	
	if (randomizeDistance != 0)
		randomizePosition(randomizeDistance);
	
	insertToZone(zone);
}	

void PlayerImplementation::bounceBack() {
	if (parent != NULL && parent->isCell()) {
		DataTransformWithParent* trans = new DataTransformWithParent(_this);
		sendMessage(trans);
	} else {
		DataTransform* trans = new DataTransform(_this);
		sendMessage(trans);
	}
}

void PlayerImplementation::notifySceneReady() {
	//if (isLoggingIn()) {
		setOnline();
		
		unicode msg = unicode("Welcome to the Official Core3 Test Center!");
		sendSystemMessage(msg);

		unicode msg2 = unicode("please help us sorting some problems out by being as active as you can. we need to stress the server for these bugs to arise. thank you");
		sendSystemMessage(msg2);
		
		unicode msg3 = unicode("This server is owned, operated, and developed by Team SWGEmu at SWGEmu.com and is in no way affiliated with any other server communities.");
		sendSystemMessage(msg3);
		
		ChatManager* chatManager = server->getChatManager();
		chatManager->listMail(_this);
		
		info("scene ready");
	//}	
}

void PlayerImplementation::sendSystemMessage(const string& message) {
	unicode msg(message);
	sendSystemMessage(msg);
}

void PlayerImplementation::sendSystemMessage(const string& file, const string& str, uint64 targetid) {
	ChatSystemMessage* msg = new ChatSystemMessage(file, str, targetid);
	sendMessage(msg);
}

void PlayerImplementation::sendSystemMessage(unicode& message) {
	ChatSystemMessage* smsg = new ChatSystemMessage(message);
	sendMessage(smsg);
}



void PlayerImplementation::queueFlourish(const string& modifier, uint64 target, uint32 actionCntr)
{	
	//TODO: Refactor this part later somehow?
	if (!isPlayer())
		return;

	//PlayerImplementation* player = (PlayerImplementation*) this;


	string skillBox = "social_entertainer_novice";
	
	if (!getSkillBoxesSize() || !hasSkillBox(skillBox)) {
		// TODO: sendSystemMessage("cmd_err", "ability_prose", creature);
		sendSystemMessage("You do not have sufficient abilities to Flourish");
		return;
	}		

	int fid = atoi(modifier.c_str());

    if (modifier == "") {
    	sendSystemMessage("performance", "flourish_format");
    	return;
    }
    	    
    if (fid < 1 || fid > 8) {
    	sendSystemMessage("performance", "flourish_not_valid");
    	sendSystemMessage("performance", "flourish_format");
    	return;
    }
    
    uint32 actionCRC = String::hashCode("flourish+" + modifier); // get the CRC for flourish+1, etc
    
    PlayerObject* po = getPlayerObject();    
    queueAction(po->getPlayer(), target, actionCRC, actionCntr, modifier);
	
}


void PlayerImplementation::queueAction(Player* player, uint64 target, uint32 actionCRC, uint32 actionCntr, const string& amod) {
	/*stringstream ident;
	ident << "0x" << hex << actionCRC << " (" << actionCntr << ")"; 
	
	sendSystemMessage("queing action " + ident.str());*/
	
	// Try to queue some music skills
	Skill* skill = creatureSkills.get(actionCRC);
	
	if ((isDancing() || isPlayingMusic()) && (skill != NULL) && !(skill->isEntertainSkill() || skill->isDanceSkill() || skill->isMusicSkill())) {
		player->sendSystemMessage("You cant use skills while dancing/playing music!");
		clearQueueAction(actionCntr);
	} else if (commandQueue.size() < 15) {
		CommandQueueAction* action = new CommandQueueAction(player, target, actionCRC, actionCntr, amod);

		if (!doAction(action))
			delete action;
	} else
		clearQueueAction(actionCntr);

	/*sendSystemMessage("queing action " + ident.str() + " finished");*/

	return;
}

bool PlayerImplementation::doAction(CommandQueueAction* action) {
	CombatManager* combatManager = server->getCombatManager();

	uint32 actionCRC = action->getActionCRC();
	Skill* skill = creatureSkills.get(actionCRC);

	if (skill == NULL) {
		action->clearError(2);
		return false;
	}
	updateTarget(action->getTargetID());

	action->setTarget((Player*) targetObject.get());
	action->setSkill(skill);
	
	if (!action->check())
		return false;

	if (commandQueue.size() != 0 || !nextAction.isPast()) {
		if (commandQueue.size() == 0) {
			CommandQueueActionEvent* e = new CommandQueueActionEvent(this);
			server->addEvent(e, nextAction);
		}

		commandQueue.add(action);
	} else {
		nextAction.update();

		activateQueueAction(action);
		
	}

	return true;
}

void PlayerImplementation::activateQueueAction(CommandQueueAction* action) {
	if (!isOnline()) {
		return;
	}

	if (nextAction.isFuture()) {
		Event* e = new CommandQueueActionEvent(this);
		server->addEvent(e, nextAction);
		
		return;
	}

	if (action == NULL) {
		if (commandQueue.size() == 0)
			return;
			
		action = commandQueue.remove(0);
	}
	
	/*stringstream msg;
	msg << "activating action " << action->getSkill()->getSkillName() << " " << hex << "0x" << action->getActionCRC() << " (" 
		<< action->getActionCounter() << ")";
	sendSystemMessage(msg.str());*/
	
	CombatManager* combatManager = server->getCombatManager();

	if (!isIncapacitated() && !isDead()) {
		if (action->validate()) {
			float time = combatManager->handleAction(action);
			action->clear(time);
			uint64 ctime = nextAction.getMiliTime();
			
			nextAction.update();
			nextAction.addMiliTime((uint32) (time * 1000));
		} else {
			action->clearError(1, 19);
		}
	} else {
		action->clearError(1, 19);
	}

	delete action;

	activateRecovery();
	
	if (commandQueue.size() != 0) {
		Event* e = new CommandQueueActionEvent(this);

		if (!nextAction.isFuture()) {
			nextAction.update();
			nextAction.addMiliTime(100);
		}
		
		server->addEvent(e, nextAction);
	}
}

void PlayerImplementation::clearQueueAction(uint32 actioncntr, float timer, uint32 tab1, uint32 tab2) {
	BaseMessage* queuemsg = new CommandQueueRemove(_this, actioncntr, timer, tab1, tab2);
	sendMessage(queuemsg);
}

void PlayerImplementation::deleteQueueAction(uint32 actioncntr) {
	for (int i = 0; i < commandQueue.size(); ++i) {
		CommandQueueAction* action = commandQueue.get(i);

		if (action->getActionCounter() == actioncntr) {
			commandQueue.remove(i);
			delete action;
			break;
		}
	}
}

void PlayerImplementation::doIncapacitate() {
	clearCombatState();
	
	if (isMounted())
		dismount(true, true);

	if (deathCount == 0) {
		firstIncapacitationTime.update();
		firstIncapacitationTime.addMiliTime(900000);
	} else if (deathCount != 0 && firstIncapacitationTime.isPast()) {
		deathCount = 0;
		firstIncapacitationTime.update();
		firstIncapacitationTime.addMiliTime(900000);
	}
	
	if (++deathCount < 3) {
		// send incapacitation timer
		CreatureObjectDeltaMessage3* incap = new CreatureObjectDeltaMessage3(_this);
		incap->updateIncapacitationRecoveryTime(8);
		incap->close();
	
		sendMessage(incap);

		clearStates();
		setPosture(INCAPACITATED_POSTURE);

		rescheduleRecovery(8000);
	} else
		kill();
}

void PlayerImplementation::kill() {
	clearStates();
	setPosture(DEAD_POSTURE);

	deathCount = 0;

	rescheduleRecovery(5000);
}

void PlayerImplementation::changePosture(int post) {
	if (logoutEvent != NULL) {
		if(post == SITTING_POSTURE) {
			clearQueueAction(actionCounter);
			return;
		}
		
		sendSystemMessage("Logout canceled.");
		server->removeEvent(logoutEvent);
		delete logoutEvent;
		
		logoutEvent = NULL;
	}

	if (!getCanSample() && !getCancelSample()) {
		sendSystemMessage("You stop taking resource samples.");
		
		if (firstSampleEvent != NULL && firstSampleEvent->isQueued()) {
			server->removeEvent(firstSampleEvent);
			
			delete firstSampleEvent;
			firstSampleEvent = NULL;
		}
		
		if (sampleEvent != NULL) {
			uint64 time = -(sampleEvent->getTimeStamp().miliDifference());
			if (sampleEvent->isQueued())
				server->removeEvent(sampleEvent);
				
			delete sampleEvent;			
			sampleEvent = NULL;
		
			string str = "";
			sampleEvent = new SampleEvent(_this, str, true);
			server->addEvent(sampleEvent, time);
			setCancelSample(true);
		}
	}
	
	if (isMounted())
		return;

	if (hasAttackDelay()) {
		clearQueueAction(actionCounter);
		return;
	}

	if (meditating) {
		updateMood(Races::getMood(moodid));
		meditating = false;
	}
	
	if (isInCombat() && post == SITTING_POSTURE) {
		clearQueueAction(actionCounter);
		return;
	}
	
	if (isDizzied() && post == CreatureObjectImplementation::UPRIGHT_POSTURE) {
		if ((getSkillMod("combat_equillibrium") >> 1) > System::random(100)) {
			if (!dizzyFallDownEvent->isQueued())
				server->addEvent(dizzyFallDownEvent, 100);
			setPosture(post, true);
		} else {
			setPosture(CreatureObjectImplementation::KNOCKEDDOWN_POSTURE, true);
			sendSystemMessage("cbt_spam", "dizzy_fall_down_single");
		}
	} else
		setPosture(post);
	
	clearQueueAction(actionCounter);
}

void PlayerImplementation::activateRecovery() {
	if (!recoveryEvent->isQueued())
		server->addEvent(recoveryEvent, 3000);
}

void PlayerImplementation::rescheduleRecovery(int time) {
	if (recoveryEvent->isQueued())
		server->removeEvent(recoveryEvent);
	
	server->addEvent(recoveryEvent, time);
}

void PlayerImplementation::doRecovery() {
	if (isLinkDead()) {
		if (logoutTimeStamp.isPast()) {
			info("unloading dead linked player");
			
			unload();
		
			setOffline();
			
			return;
		} else {
			info("keeping dead linked player in game");
			
			activateRecovery();
		}
	}
	
	if (isIncapacitated()) {
		speed = 5.376;
		
		setPosture(UPRIGHT_POSTURE);
	} else if (isDead()) { 		
		doClone();
		
		return;
	}
	
	if (!isInCombat() && isOnFullHealth() && !hasStates() && !hasWounds() && !hasShockWounds()) {
		return;
	} else if (lastCombatAction.miliDifference() > 15000) {
		clearCombatState();
	} else if (isInCombat() && targetObject != NULL && !hasState(PEACE_STATE)
			&& (commandQueue.size() == 0)) {
		queueAction(_this, getTargetID(), 0xA8FEF90A, ++actionCounter, "");
	}

	if (!isOnFullHealth() || hasWounds() || hasShockWounds())
		calculateHAMregen();

	if (hasStates())
		doStateRecovery();

	if (isJedi())
		calculateForceRegen();
	
	activateRecovery();
}

void PlayerImplementation::doStateRecovery() {
	if (isDizzied() && dizzyRecoveryTime.isPast())
		clearState(DIZZY_STATE);

	if (isBlinded() && blindRecoveryTime.isPast())
		clearState(BLINDED_STATE);

	if (isStunned() && stunRecoveryTime.isPast())
		clearState(STUNNED_STATE);
			
	if (isIntimidated() && intimidateRecoveryTime.isPast())
		clearState(INTIMIDATED_STATE);
			
	if (isPoisoned()) {
		if (poisonRecoveryTime.isPast())
			clearState(POISONED_STATE);
		else doPoisonTick();
	}
	
	if (isDiseased()) {
		if (diseasedRecoveryTime.isPast())
			clearState(DISEASED_STATE);			
		else doDiseaseTick();
	}
	
	if (isOnFire()) {
		if (fireRecoveryTime.isPast())
			clearState(ONFIRE_STATE);			
		else doFireTick();
	}
	
	if (isBleeding()) {
		if (bleedingRecoveryTime.isPast())
			clearState(BLEEDING_STATE);
		else doBleedingTick();
	}

	updateStates();
}

void PlayerImplementation::doClone() {
	info("cloning player");

	switch (zoneID) {
	case 0:	// corellia
		if (faction == String::hashCode("rebel"))
			doWarp(-326.0f, -4640.0f, 0, true);				// shuttle 1
		else
			doWarp(-28.0f, -4438.0f);						// shuttle 2
		
		break;
	case 2: // dathomir
		if (faction == String::hashCode("rebel"))			// science outpost
			doWarp(-76.0f, -1627.0f, 0, true);
		else
			doWarp(618.0f, 3054.0f);						// trade outpost
		
		break;
	default:
		if (faction == String::hashCode("rebel"))
			doWarp(-130.0f, -5300.0f, 0, true);
		else if (faction == String::hashCode("imperial"))
			//doWarp(10.0f, -5480.0f, 0, true);
			doWarp(-2.8f, 0.1f, -4.8f, 0, 3565798);
		else
			doWarp(0.5f, 1.5f, 0.3f, 0, 1590892); // ah cloning facility
		
		break;
	}
	
	clearStates();
	clearBuffs();
	
	decayInventory();

	changeForcePowerBar(0);
	
	resetArmorEncumbrance();
	
	setNeutral();
	setCovert();

	clearDuelList();		
	
	setPosture(UPRIGHT_POSTURE);

	rescheduleRecovery();
}

void PlayerImplementation::doCenterOfBeing() {
	if (centered) {
		sendSystemMessage("combat_effects", "already_centered");
		return;
	}
	
	int duration = 0;
	int efficacy = 0;
	
	if (weaponObject == NULL) {
		duration = getSkillMod("center_of_being_duration_unarmed");
		efficacy = getSkillMod("unarmed_center_of_being_efficacy");
	} else {
		switch (weaponObject->getType()) {
		case WeaponImplementation::UNARMED:
			duration = getSkillMod("center_of_being_duration_unarmed");
			efficacy = getSkillMod("unarmed_center_of_being_efficacy");
			break;

		case WeaponImplementation::ONEHANDED:
			duration = getSkillMod("center_of_being_duration_onehandmelee");
			efficacy = getSkillMod("onehandmelee_center_of_being_efficacy");
			break;

		case WeaponImplementation::TWOHANDED:
			duration = getSkillMod("center_of_being_duration_twohandmelee");
			efficacy = getSkillMod("twohandmelee_center_of_being_efficacy");
			break;

		case WeaponImplementation::POLEARM:
			duration = getSkillMod("center_of_being_duration_polearm");
			efficacy = getSkillMod("polearm_center_of_being_efficacy");
			break;
		default:
			break;
		}
	}
	
	if (duration == 0 || efficacy == 0)
		return;
	
	//defenseBonus += efficacy;
	centeredBonus = efficacy;
	
	showFlyText("combat_effects", "center_start_fly", 0, 255, 0);
	
	server->addEvent(centerOfBeingEvent, duration * 1000);
	
	centered = true;
}

void PlayerImplementation::removeCenterOfBeing() {
	if (!centered)
		return;
	
	server->removeEvent(centerOfBeingEvent);
	
	//defenseBonus -= centeredBonus;
	centeredBonus = 0;
	
	showFlyText("combat_effects", "center_stop_fly", 255, 0, 0);
	centered = false;
}

void PlayerImplementation::doPeace() {
	//info("trying Peace action");
	
	for (int i = 0; i < defenderList.size(); ++i) {
		ManagedReference<SceneObject> defender = defenderList.get(i);
				
		try {
			defender->wlock(_this);
			
			if (defender->hasDefender(_this)) {
				if (defender->isPeaced()) {
					removeDefender(defender);
					defender->removeDefender(_this);
					
					i--;
				}
			} else {
				removeDefender(defender);
				i--;
			}

			defender->unlock();
			
		} catch (...) {
			error("unknown exception in PlayerImplementation::doPeace()\n");
			defender->unlock();
		}
		
	}
	
	if (defenderList.size() != 0) {
		//info("defenderList not empty, trying to set Peace State");
		
		if (setState(PEACE_STATE))
			updateStates();
	}
	
	//info("finished doPeace");
}

void PlayerImplementation::lootCorpse(bool lootAll) {
	if (targetObject == NULL || !targetObject->isNonPlayerCreature())
		return;
	
	Creature* target = (Creature*) targetObject.get();
	
	if (!isIncapacitated() && !isDead() && isInRange(target, 20)) {
		LootManager* lootManager = server->getLootManager();
		
		if (lootAll)
			lootManager->lootCorpse(_this, target);
		else
			lootManager->showLoot(_this, target);
	}
}

void PlayerImplementation::lootObject(Creature* creature, SceneObject* object) {
	LootManager* lootManager = server->getLootManager();
	
	lootManager->lootObject(_this, creature, object->getObjectID());
}

void PlayerImplementation::calculateForceRegen() {
	if (isJedi() && !playerObject->isOnFullForce()) {
		if (getPosture() == SITTING_POSTURE)
			changeForcePowerBar(playerObject->getForceRegen());
		else
			changeForcePowerBar(playerObject->getForceRegen() / 3);
	}
}

bool PlayerImplementation::changeForcePowerBar(int32 fp) {
	int32 newForce = playerObject->getForcePower() + fp;
	
	if (newForce <= 0)
		return false; 
	
	setForcePowerBar(MIN(newForce, playerObject->getForcePowerMax()));

	return true;
}

void PlayerImplementation::addBuff(uint32 buffcrc, float time) {
	Buffs* bf = new Buffs(_this, buffcrc, time);
	sendMessage(bf);
}

// TODO: clearBuffs
void PlayerImplementation::clearBuffs(bool doUpdatePlayer) {
	// Clear buff icons
	if (doUpdatePlayer) {
		if (healthBuff)
			addBuff(0x98321369, 0.0f);
	
		if (strengthBuff)
			addBuff(0x815D85C5, 0.0f);
	
		if (constitutionBuff)
			addBuff(0x7F86D2C6, 0.0f);
	
		if (actionBuff)
			addBuff(0x4BF616E2, 0.0f);
	
		if (quicknessBuff)
			addBuff(0x71B5C842, 0.0f);
	
		if (staminaBuff)
			addBuff(0xED0040D9, 0.0f);
	
		if (mindBuff)
			addBuff(0x11C1772E, 0.0f);
	
		if (focusBuff)
			addBuff(0x2E77F586, 0.0f);
	
		if (willpowerBuff)
			addBuff(0x3EC6FCB6, 0.0f);
		
	}

	healthBuff = false;
	strengthBuff = false;
	constitutionBuff = false;
	actionBuff = false;
	quicknessBuff = false;
	staminaBuff = false;
	mindBuff = false;
	focusBuff = false;
	willpowerBuff = false;
	
	removeBuffs(doUpdatePlayer);
}



/*
 *	Item manipulation methods 
 */

void PlayerImplementation::addInventoryItem(TangibleObject* item) {
	CreatureObjectImplementation::addInventoryItem(item);
	
	if(item->isEquipped())
		equipPlayerItem(item);

}

void PlayerImplementation::equipPlayerItem(TangibleObject* item) {
	if (item->isEquipped())
		item->setEquipped(false);
	
	if (item->isWeapon()) {
		changeWeapon(item->getObjectID());
	} else if (item->isArmor()) {
		changeArmor(item->getObjectID(), true);
	} else if (item->isClothing()) {
		changeCloth(item->getObjectID());
	} else if (item->isInstrument()) {
		changeWeapon(item->getObjectID());
	}
}
 
bool PlayerImplementation::isAllowedBySpecies(TangibleObject * item) {
	int type = item->getObjectSubType();
	bool ithoonly = ((type == TangibleObjectImplementation::ITHOGARB) ||
			(item->getTemplateName().find("ith_armor") != string::npos));
	bool wookonly = ((type == TangibleObjectImplementation::WOOKIEGARB) ||
			(item->getTemplateName().find("armor_kashyyykian") != string::npos));
	bool footwear = ((type == TangibleObjectImplementation::FOOTWEAR) ||
			(type == TangibleObjectImplementation::FOOTARMOR));
	
	string species = this->getSpeciesName();
	
	if (species.compare("ithorian") == 0) {
		return ithoonly;
	} else if (species.compare("wookiee") == 0) {
		return wookonly;
	} else if (species.compare("trandoshan") == 0) {
		return !ithoonly && !wookonly && !footwear;
	} else {
		return !ithoonly && !wookonly;
	}			
}
void PlayerImplementation::changeCloth(uint64 itemid) {

	SceneObject* obj = inventory->getObject(itemid);
	if (obj == NULL || !obj->isTangible())
		return;
	
	TangibleObject* cloth = (TangibleObject*) obj;	
	
	if (cloth->isWeapon()) {
		if (cloth->isEquipped())
			changeWeapon(itemid);
		return;
	}
	if (cloth->isArmor()) {
		if (cloth->isEquipped())
			changeArmor(itemid, false);
		return;
	}
	
	if(!isAllowedBySpecies(cloth)) {
		cloth->setEquipped(false);
		sendSystemMessage("Your species can not wear this item.");
		return;
	}
	
	if (cloth->isEquipped()) {
		unequipItem(cloth);
	} else {
		equipItem(cloth);
	}
}

void PlayerImplementation::changeWeapon(uint64 itemid) {

	SceneObject* obj = inventory->getObject(itemid);
	
	if (obj == NULL || !obj->isTangible())
		return;
	
	if(this->isPlayingMusic())
		this->stopPlayingMusic();

	if (((TangibleObject*)obj)->isWeapon()) {
	
		Weapon* weapon = (Weapon*) obj;
		
		if (weapon == NULL) 
			return;
		
		if (centered)
			removeCenterOfBeing();
		
		if (weapon->isEquipped()) {
			unequipItem(weapon);
			unsetWeaponSkillMods(weapon);
			setWeapon(NULL);
			
			accuracy = getSkillMod("unarmed_accuracy");
		} else {
			if (weaponObject != NULL) {
				unequipItem(weaponObject);
				unsetWeaponSkillMods(weaponObject);
			}
			
			setWeapon(weapon);
			equipItem(weapon);
			
			setWeaponSkillMods(weapon);		
		}	
	} else if (((TangibleObject*)obj)->isInstrument()){
		
		Instrument* device = (Instrument*) obj;
		int instrument = device->getInstrumentType();
		
		string skillBox;
		// Needs to be refactored
		switch(instrument)
		{
		case InstrumentImplementation::SLITHERHORN: //SLITHERHORN
			skillBox = "social_entertainer_novice";
			if (!getSkillBoxesSize() || !hasSkillBox(skillBox)) {
				sendSystemMessage("You do not have sufficient abilities to equip " + device->getName().c_str() + ".");
				return;
			}
			break;
		case InstrumentImplementation::FIZZ: // FIZZ
			skillBox = "social_entertainer_music_01";
			if (!getSkillBoxesSize() || !hasSkillBox(skillBox)) {
				sendSystemMessage("You do not have sufficient abilities to equip " + device->getName().c_str() + ".");
				return;
			}
			break;
		case InstrumentImplementation::FANFAR: // FANFAR
			skillBox = "social_entertainer_music_03";
			if (!getSkillBoxesSize() || !hasSkillBox(skillBox)) {
				sendSystemMessage("You do not have sufficient abilities to equip " + device->getName().c_str() + ".");
				return;
			}
			break;
		case InstrumentImplementation::KLOOHORN: // KLOOHORN
			skillBox = "social_entertainer_music_04";
			if (!getSkillBoxesSize() || !hasSkillBox(skillBox)) {
				sendSystemMessage("You do not have sufficient abilities to equip " + device->getName().c_str() + ".");
				return;
			}
			break;
		case InstrumentImplementation::MANDOVIOL: // MANDOVIOL
			skillBox = "social_entertainer_master";
			if (!getSkillBoxesSize() || !hasSkillBox(skillBox)) {
				sendSystemMessage("You do not have sufficient abilities to equip " + device->getName().c_str() + ".");
				return;
			}
			break;
		case InstrumentImplementation::TRAZ: // TRAZ
			skillBox = "social_musician_novice";
			if (!getSkillBoxesSize() || !hasSkillBox(skillBox)) {
				sendSystemMessage("You do not have sufficient abilities to equip " + device->getName().c_str() + ".");
				return;
			}
			break;
		case InstrumentImplementation::BANDFILL: // BANDFILL
			skillBox = "social_musician_knowledge_02";
			if (!getSkillBoxesSize() || !hasSkillBox(skillBox)) {
				sendSystemMessage("You do not have sufficient abilities to equip " + device->getName().c_str() + ".");
				return;
			}
			break;
		case InstrumentImplementation::FLUTEDROOPY: // FLUTEDROOPY
			skillBox = "social_musician_knowledge_03";
			if (!getSkillBoxesSize() || !hasSkillBox(skillBox)) {
				sendSystemMessage("You do not have sufficient abilities to equip " + device->getName().c_str() + ".");
				return;
			}
			break;
		case InstrumentImplementation::OMNIBOX: // OMNIBOX
			skillBox = "social_musician_knowledge_04";
			if (!getSkillBoxesSize() || !hasSkillBox(skillBox)) {
				sendSystemMessage("You do not have sufficient abilities to equip " + device->getName().c_str() + ".");
				return;
			}
			break;
		case InstrumentImplementation::NALARGON: // NALARGON
			skillBox = "social_musician_master";
			if (!getSkillBoxesSize() || !hasSkillBox(skillBox)) {
				sendSystemMessage("You do not have sufficient abilities to equip " + device->getName().c_str() + ".");
				return;
			}
			break;
		default :
			sendSystemMessage("You do not have sufficient abilities to equip " + device->getName().c_str() + ".");
			return;
		}
		
		TangibleObject* item = (TangibleObject*) obj;
		
		if (isPlayingMusic())
			stopPlayingMusic();
		
		if (item->isEquipped())
		{
			unequipItem(item);
		}
		else
			equipItem(item);
	} else {
		TangibleObject* item = (TangibleObject*) obj;
		
		sendSystemMessage("triggered here.");
		if (item->isEquipped())
			unequipItem(item);
		else
			equipItem(item);
	}
}

void PlayerImplementation::changeArmor(uint64 itemid, bool forced) {

	SceneObject* obj = inventory->getObject(itemid);
	
	if (obj == NULL || !obj->isTangible())
		return;
	
	if (((TangibleObject*)obj)->isArmor()) {
		Armor* armoritem = (Armor*) obj;
		
		if (armoritem == NULL) 
			return;
		
		if(!isAllowedBySpecies(armoritem)) {
			armoritem->setEquipped(false);
			sendSystemMessage("Your species can not wear this item.");
			return;
		}
		
		if (armoritem->isEquipped()) {
			unequipItem((TangibleObject*) obj);
			unsetArmorSkillMods(armoritem);
			unsetArmorEncumbrance(armoritem);
		} else {
			Armor* olditem = getArmor(armoritem->getType());
			
			if (olditem != NULL) {
				unsetArmorSkillMods(olditem);
				unsetArmorEncumbrance(olditem);
				unequipItem((TangibleObject*) olditem);
			}
			
			if (setArmorEncumbrance(armoritem, forced)) {
				equipItem((TangibleObject*) obj);
				setArmorSkillMods(armoritem);
			} else
				sendSystemMessage("You don't have enough pool points to do that!");
		}
	} else
	{
		TangibleObject* item = (TangibleObject*) obj;
		
		if (item->isEquipped())
			unequipItem(item);
		else
			equipItem(item);
	}
	
	BaseMessage* creo6 = new CreatureObjectMessage6(_this);
	BaseMessage* creo4 = new CreatureObjectMessage4(this);
	
	sendMessage(creo6);
	sendMessage(creo4);
}

void PlayerImplementation::setItemSkillMod(int type, int value) {
	switch (type) {
	case 1:
		addSkillModBonus("melee_defense", value, true);
		break;
	case 2:
		addSkillModBonus("ranged_defense", value, true);
		break;
	case 3:
		addSkillModBonus("stun_defense", value, true);
		break;
	case 4:
		addSkillModBonus("dizzy_defense", value, true);
		break;
	case 5:
		addSkillModBonus("blind_defense", value, true);
		break;
	case 6:
		addSkillModBonus("knockdown_defense", value, true);
		break;
	case 7:
		addSkillModBonus("intimidate_defense", value, true);
		break;
	case 8:
		addSkillModBonus("pistol_speed", value, true);
		break;
	case 9:
		addSkillModBonus("carbine_speed", value, true);
		break;
	case 10:
		addSkillModBonus("rifle_speed", value, true);
		break;
	case 11:
		addSkillModBonus("unarmed_speed", value, true);
		break;
	case 12:
		addSkillModBonus("onehandmelee_speed", value, true);
		break;
	case 13:
		addSkillModBonus("twohandmelee_speed", value, true);
		break;
	case 14:
		addSkillModBonus("polearm_speed", value, true);
		break;
	case 15:
		addSkillModBonus("pistol_accuracy", value, true);
		break;
	case 16:
		addSkillModBonus("carbine_accuracy", value, true);
		break;
	case 17:
		addSkillModBonus("rifle_accuracy", value, true);
		break;
	case 18:
		addSkillModBonus("unarmed_accuracy", value, true);
		break;
	case 19:
		addSkillModBonus("onehandmelee_accuracy", value, true);
		break;
	case 20:
		addSkillModBonus("twohandmelee_accuracy", value, true);
		break;
	case 21:
		addSkillModBonus("polearm_accuracy", value, true);
		break;
	case 22:
		addSkillModBonus("dodge", value, true);
		break;
	case 23:
		addSkillModBonus("block", value, true);
		break;
	case 24:
		addSkillModBonus("counterattack", value, true);
		break;
	case 25:
		addSkillModBonus("resistance_bleeding", value, true);
		break;
	case 26:
		addSkillModBonus("resistance_disease", value, true);
		break;
	case 27:
		addSkillModBonus("resistance_fire", value, true);
		break;
	case 28:
		addSkillModBonus("resistance_poison", value, true);
		break;
	case 29:
		addSkillModBonus("slope_move", value, true);
		break;
	case 30:
		addSkillModBonus("heavyweapon_speed", value, true);
		break;
	case 31:
		addSkillModBonus("heavyweapon_accuracy", value, true);
		break;	
	}
}

void PlayerImplementation::setWeaponSkillMods(Weapon* weapon) {
	switch (weapon->getType()) {
		case WeaponImplementation::UNARMED:
			accuracy = getSkillMod("unarmed_accuracy");
			break;
				
		case WeaponImplementation::ONEHANDED:
			accuracy = getSkillMod("onehandmelee_accuracy");
			break;
					
		case WeaponImplementation::TWOHANDED:
			accuracy = getSkillMod("twohandmelee_accuracy");
			break;
					
		case WeaponImplementation::POLEARM:
			accuracy = getSkillMod("polearm_accuracy");
			break;
				
		case WeaponImplementation::PISTOL:
			accuracy = getSkillMod("pistol_accuracy");
			break;
					
		case WeaponImplementation::CARBINE:
			accuracy = getSkillMod("carbine_accuracy");
			break;
					
		case WeaponImplementation::RIFLE:
			accuracy = getSkillMod("rifle_accuracy");
			break;
			
		case WeaponImplementation::HEAVYWEAPON:
			accuracy = getSkillMod("heavyweapon_accuracy");
			break;

		case WeaponImplementation::SPECIALHEAVYWEAPON:
			if (weapon->getType() == WeaponImplementation::RIFLEFLAMETHROWER)
				accuracy = getSkillMod("heavy_flame_thrower_accuracy");
			
			else if (weapon->getType() == WeaponImplementation::RIFLELIGHTNING)
				accuracy = getSkillMod("heavy_rifle_lightning_accuracy");
			
			accuracy += getSkillMod("heavyweapon_accuracy");
			break;
			
		/*case Weapon::ONEHANDSABER:
			accuracy = SkillMods.get("");
			break;
			
		case Weapon::TWOHANDSABER:
			accuracy = SkillMods.get("");
			break;
			
		case Weapon::POLEARMSABER:
			accuracy = SkillMods.get("");
			break;*/
	}
	setItemSkillMod(weapon->getSkillMod0Type(), weapon->getSkillMod0Value());
	setItemSkillMod(weapon->getSkillMod1Type(), weapon->getSkillMod1Value());
	setItemSkillMod(weapon->getSkillMod2Type(), weapon->getSkillMod2Value());

	if (checkCertification(weapon->getCert())) {
		weapon->setCertified(true);
	} else {
		sendSystemMessage("You are not certified to use this weapon. Damage will be reduced.");
		weapon->setCertified(false);
	}
}

void PlayerImplementation::setArmorSkillMods(Armor* armoritem) {
	setItemSkillMod(armoritem->getSkillMod0Type(), armoritem->getSkillMod0Value());
	setItemSkillMod(armoritem->getSkillMod1Type(), armoritem->getSkillMod1Value());
	setItemSkillMod(armoritem->getSkillMod2Type(), armoritem->getSkillMod2Value());

	setItemSkillMod(armoritem->getSocket0Type(), armoritem->getSocket0Value());
	setItemSkillMod(armoritem->getSocket1Type(), armoritem->getSocket1Value());
	setItemSkillMod(armoritem->getSocket2Type(), armoritem->getSocket2Value());
	setItemSkillMod(armoritem->getSocket3Type(), armoritem->getSocket3Value());	
	
}

void PlayerImplementation::unsetArmorSkillMods(Armor* armoritem) {
	setItemSkillMod(armoritem->getSkillMod0Type(), -armoritem->getSkillMod0Value());
	setItemSkillMod(armoritem->getSkillMod1Type(), -armoritem->getSkillMod1Value());
	setItemSkillMod(armoritem->getSkillMod2Type(), -armoritem->getSkillMod2Value());

	setItemSkillMod(armoritem->getSocket0Type(), -armoritem->getSocket0Value());
	setItemSkillMod(armoritem->getSocket1Type(), -armoritem->getSocket1Value());
	setItemSkillMod(armoritem->getSocket2Type(), -armoritem->getSocket2Value());
	setItemSkillMod(armoritem->getSocket3Type(), -armoritem->getSocket3Value());
	
}

void PlayerImplementation::unsetWeaponSkillMods(Weapon* weapon) {
	setItemSkillMod(weapon->getSkillMod0Type(), -weapon->getSkillMod0Value());
	setItemSkillMod(weapon->getSkillMod1Type(), -weapon->getSkillMod1Value());
	setItemSkillMod(weapon->getSkillMod2Type(), -weapon->getSkillMod2Value());
	
	accuracy = getSkillMod("unarmed_accuracy"); 
}

bool PlayerImplementation::setArmorEncumbrance(Armor* armor, bool forced) {
	int healthEncumb = armor->getHealthEncumbrance();
	int actionEncumb = armor->getActionEncumbrance();
	int mindEncumb = armor->getMindEncumbrance();
	
	if ((healthEncumb >= strength || healthEncumb >= constitution ||
		actionEncumb >= quickness || actionEncumb >= stamina ||
		mindEncumb >= focus || mindEncumb >= willpower) && !forced)
		return false;
	
	if ((strength > 100000 || constitution > 100000 ||
		quickness > 100000 || stamina > 100000 ||
		focus > 100000 || willpower > 100000) && !forced)
		return false;	
	
	healthEncumbrance += healthEncumb;
	actionEncumbrance += actionEncumb;
	mindEncumbrance += mindEncumb;
	
	strengthMax -= healthEncumb;
	constitutionMax -= healthEncumb;
	quicknessMax -= actionEncumb;
	staminaMax -= actionEncumb;
	focusMax -= mindEncumb;
	willpowerMax -= mindEncumb;
	
	strength -= healthEncumb;
	constitution -= healthEncumb;
	quickness -= actionEncumb;
	stamina -= actionEncumb;
	focus -= mindEncumb;
	willpower -= mindEncumb;
	
	return true;
	
}

void PlayerImplementation::unsetArmorEncumbrance(Armor* armor) {	
	int healthEncumb = armor->getHealthEncumbrance();
	int actionEncumb = armor->getActionEncumbrance();
	int mindEncumb = armor->getMindEncumbrance();
	
	healthEncumbrance -= healthEncumb;
	actionEncumbrance -= actionEncumb;
	mindEncumbrance -= mindEncumb;
	
	strengthMax += healthEncumb;
	constitutionMax += healthEncumb;
	quicknessMax += actionEncumb;
	staminaMax += actionEncumb;
	focusMax += mindEncumb;
	willpowerMax += mindEncumb;
	
	strength += healthEncumb;
	constitution += healthEncumb;
	quickness += actionEncumb;
	stamina += actionEncumb;
	focus += mindEncumb;
	willpower += mindEncumb;

}

void PlayerImplementation::applyPowerup(uint64 powerupID, uint64 targetID) {
	
	Powerup* powerup = (Powerup*) getInventoryItem(powerupID);
	Weapon* weapon = (Weapon*) getInventoryItem(targetID);
	
	if (weapon == NULL || powerup == NULL)
		return;
	
	weapon->wlock();
	powerup->wlock();
	
	if (weapon->getPowerupUses() == 0) {
		stringstream msg;
		msg << "You powerup your " << weapon->getName().c_str() << " with " << powerup->getName().c_str();
		sendSystemMessage(msg.str());
		powerup->apply(weapon);
		powerup->remove(_this);
		
		weapon->unlock();
		powerup->unlock();
		powerup->finalize();
		
		return;
	}
	else
		sendSystemMessage("This weapon is already powered up!");
	
	weapon->unlock();
	powerup->unlock();
}


void PlayerImplementation::applyAttachment(uint64 attachmentID, uint64 targetID) {
	
	Attachment* attachment = (Attachment*) getInventoryItem(attachmentID);
	Armor* armor = (Armor*) getInventoryItem(targetID);
		
	if (armor == NULL || attachment == NULL)
		return;
	
	armor->wlock();
	attachment->wlock();
	
	int skillModType;
	int skillModValue;
	
	int armorIndex;
	int attachmentIndex;
	
	bool done = false;
	bool setMods = false;

	while (!done) {
		done = true;
		attachmentIndex = attachment->getBestSkillMod();
		skillModType = attachment->getSkillModType(attachmentIndex);
		skillModValue = attachment->getSkillModValue(attachmentIndex);
		
		if (armor->isEquipped()) {
			unsetArmorSkillMods(armor);
			setMods = true;
		}
		
		int armorIndex = armor->addSkillMod(skillModType, skillModValue);
		
		if (setMods) {
			setArmorSkillMods(armor);
			setMods = false;
		}
		
		switch (armorIndex) {
		case (-1): // add failed
			break;
		case (-2): // equal or lesser value, remove skill mod from attachment and try again
			attachment->setSkillModValue(attachmentIndex, 0);
			attachment->setUpdated(true);
			done = false;
			break;
		case (-3): // we overwrote the skill mod with a higher one.  delete the attachment.
			armor->unlock();
			attachment->unlock();
			attachment->remove(_this);
			
			attachment->finalize();
			return;
		default: // skill mod was added successfully
			armor->unlock();
			attachment->unlock();
			attachment->remove(_this);
			
			attachment->finalize();
			return;
		}
	}
	// this is for the case when we pulled off the skill mod but the attachment only had one mod)
	if (attachment->isUpdated()) {
		attachment->remove(_this);
		attachment->setUpdated(false);
		
		armor->unlock();
		attachment->unlock();
		
		attachment->finalize();
		return; 
	}
	
	armor->unlock();
	attachment->unlock();
}

void PlayerImplementation::setOvert() {
	if (!(pvpStatusBitmask & OVERT_FLAG))
		pvpStatusBitmask |= OVERT_FLAG;
	
	uint32 pvpBitmask = pvpStatusBitmask;
		
	try {
		zone->lock();
	
		for (int i = 0; i < inRangeObjectCount(); ++i) {
			SceneObject* object = (SceneObject*) (((SceneObjectImplementation*) getInRangeObject(i))->_getStub());
			
			if (object->isPlayer()) {
				Player* player = (Player*) object;
				sendFactionStatusTo(player, true);
			}
		}
		
		zone->unlock();
	} catch (...) {
		error("exception Player::setOvert()");
		zone->unlock();
	}
}

void PlayerImplementation::setCovert() {
	if (pvpStatusBitmask & OVERT_FLAG)
		pvpStatusBitmask -= OVERT_FLAG;
	
	try {
		zone->lock();
	
		for (int i = 0; i < inRangeObjectCount(); ++i) {
			SceneObject* object = (SceneObject*) (((SceneObjectImplementation*) getInRangeObject(i))->_getStub());
			
			if (object->isPlayer()) {
				Player* player = (Player*) object;
				sendFactionStatusTo(player, true);
			}
		}
			
		zone->unlock();
	} catch (...) {
		error("exception PlayerImplementation::setOvert()");
		zone->unlock();
	}
}

void PlayerImplementation::setLinkDead() {
	onlineStatus = LINKDEAD;
	
	if (playerObject != NULL)
		playerObject->setCharacterBit(PlayerObjectImplementation::LD, true);
	
	logoutTimeStamp.update();
	logoutTimeStamp.addMiliTime(30000);

	activateRecovery();	
}

void PlayerImplementation::setOnline() {
	if (isLinkDead()) {
		if (playerObject != NULL)
			playerObject->clearCharacterBit(PlayerObjectImplementation::LD, true);
	}
	
	onlineStatus = ONLINE;
}

bool PlayerImplementation::isInDuelWith(Player* targetPlayer, bool doLock) {
	if (_this == targetPlayer)
		return false;

	try {
		if (doLock)
			targetPlayer->wlock(_this);

		bool res; 
		if (requestedDuelTo(targetPlayer) && targetPlayer->requestedDuelTo(_this))
			res = true;
		else
			res = false;

		if (doLock)
			targetPlayer->unlock();
			
		return res;
	} catch (...) {
		if (doLock)
			targetPlayer->unlock();
		
		return false;
	}
}

void PlayerImplementation::addToDuelList(Player* targetPlayer) {
	if (duelList.put(targetPlayer) != -1)
		info("player [" + targetPlayer->getLoggingName() + "] added to duel list");
	else
		error("player [" + targetPlayer->getLoggingName() + "] was already in duel list");
		
}

void PlayerImplementation::removeFromDuelList(Player* targetPlayer) {
	if (duelList.drop(targetPlayer))
		info("player [" + targetPlayer->getLoggingName() + "] removed from duel list");
	else
		error("player [" + targetPlayer->getLoggingName() + "] was not found in duel list for removal");
}

void PlayerImplementation::clearDuelList() {
	if (zone != NULL) {
		CombatManager* combatManager = server->getCombatManager();
		
		combatManager->freeDuelList(_this);
	}
}

// Crafting
CraftingTool* PlayerImplementation::getCurrentCraftingTool() {
	return currentCraftingTool;
}
void PlayerImplementation::setCurrentCraftingTool(CraftingTool* ct) {
	currentCraftingTool = ct;
}
void PlayerImplementation::clearCurrentCraftingTool() {
	currentCraftingTool = NULL;
}

void PlayerImplementation::prepareCraftingSession(CraftingTool * ct, DraftSchematic* ds) {
	CraftingManager* craftingManager = server->getCraftingManager();
	craftingManager->prepareCraftingSession(_this, ct, ds);
}

void PlayerImplementation::addResourceToCraft(ResourceContainer * rnco, int slot, int counter) {
	CraftingManager* craftingManager = server->getCraftingManager();
	craftingManager->addResourceToCraft(_this, rnco, slot, counter);
}

void PlayerImplementation::removeResourceFromCraft(uint64 resID, int slot, int counter) {
	CraftingManager* craftingManager = server->getCraftingManager();
	craftingManager->removeResourceFromCraft(_this, slot, counter);
}

void PlayerImplementation::nextCraftingStage(string test) {
	CraftingManager* craftingManager = server->getCraftingManager();
	craftingManager->nextCraftingStage(_this, test);
}

void PlayerImplementation::craftingCustomization(string name, int condition) {
	CraftingManager* craftingManager = server->getCraftingManager();
	craftingManager->craftingCustomization(_this, name, condition);
}

void PlayerImplementation::createPrototype(string count) {
	CraftingManager* craftingManager = server->getCraftingManager();
	craftingManager->createPrototype(_this, count);
}

void PlayerImplementation::createSchematic(string count) {
	CraftingManager* craftingManager = server->getCraftingManager();
	craftingManager->createSchematic(_this, count);
}

void PlayerImplementation::handleExperimenting(int count, int numRowsAttempted, string expstring) {
	CraftingManager* craftingManager = server->getCraftingManager();
	craftingManager->handleExperimenting(_this, count, numRowsAttempted, expstring);
}
// Draft Schematics

void PlayerImplementation::addDraftSchematicsFromGroupName(const string& schematicGroupName) {
	CraftingManager* craftingManager = server->getCraftingManager();
	craftingManager->addDraftSchematicsFromGroupName(_this, schematicGroupName);
}

void PlayerImplementation::subtractDraftSchematicsFromGroupName(const string& schematicGroupName) {
	CraftingManager* craftingManager = server->getCraftingManager();
	craftingManager->subtractDraftSchematicsFromGroupName(_this, schematicGroupName);
}

void PlayerImplementation::addDraftSchematic(DraftSchematic* ds) {
	draftSchematicList.put(ds->getSchematicID(), ds);
}
void PlayerImplementation::subtractDraftSchematic(DraftSchematic* ds) {
	draftSchematicList.drop(ds->getSchematicID());
}

void PlayerImplementation::sendDraftSchematics() {
	PlayerObjectDeltaMessage9* dplay9;

	dplay9 = new PlayerObjectDeltaMessage9(playerObject);

	dplay9->updateDraftSchematics();

	dplay9->close();
	sendMessage(dplay9);
	
	// Sending all the ingredients and experimental properties when draft schematics are sent
	// is the only way I can think of at the moment to prevent the bug if the client
	// leaves their datapad open and they surrender a skill that has draft schematics.
	// The draft schematics that are suppose to be deleted when surrendering only disappear
	// when the datapad is refreshed (close datapad, open datapad to refresh), if the client
	// clicks on a draft schematic he doesn't have, it screws up their retreiveing the information
	// of the draft schematic because they don't really have that schematic
	for (int i = 0; i < draftSchematicList.size(); i++) {
		DraftSchematic* schematic = draftSchematicList.get(i); 
		schematic->sendIngredientsToPlayer(_this);
		schematic->sendExperimentalPropertiesToPlayer(_this);
	}
}

// Get by key
DraftSchematic* PlayerImplementation::getDraftSchematic(uint32 schematicID) {
	if(draftSchematicList.contains(schematicID)) {
		return draftSchematicList.get(schematicID);
	} else {
		return NULL;
	}
}

// Get by index
DraftSchematic* PlayerImplementation::getDraftSchematic(int index) {
	if(index >= 0 && index < draftSchematicList.size()) {
		return draftSchematicList.get(index);
	} else {
		return NULL;
	}
}

void PlayerImplementation::sendMessage(BaseMessage* msg) {
	if (owner != NULL)
		owner->sendMessage(msg);
	else
		delete msg;
}

void PlayerImplementation::sendMessage(StandaloneBaseMessage* msg) {
	if (owner != NULL)
		owner->sendMessage(msg);
	else
		delete msg;
}

void PlayerImplementation::addSkillBox(SkillBox* skillBox, bool updateClient) {
	skillBoxes.put(skillBox->getName(), skillBox);
	
	if (updateClient) {
		CreatureObjectDeltaMessage1* dcreo1;
	
		dcreo1 = new CreatureObjectDeltaMessage1(this);
		dcreo1->startSkillBoxListUpdate(1);
		dcreo1->addSkillBox(skillBox->getName());
	
		dcreo1->close();
		sendMessage(dcreo1);
	}
}

void PlayerImplementation::removeSkillBox(SkillBox* skillBox, bool updateClient) {
	skillBoxes.remove(skillBox->getName());
	
	if (updateClient) {
		CreatureObjectDeltaMessage1* dcreo1;
	
		dcreo1 = new CreatureObjectDeltaMessage1(this);
		dcreo1->startSkillBoxListUpdate(1);
		dcreo1->removeSkillBox(skillBox->getName());
	
		dcreo1->close();
		sendMessage(dcreo1);
	}
}

void PlayerImplementation::addCertifications(Vector<Certification*>& certs, bool updateClient) {		
	PlayerObjectDeltaMessage9* dplay9;
	
	if (updateClient) {
		dplay9 = new PlayerObjectDeltaMessage9(playerObject);
		dplay9->startSkillListUpdate(certs.size());
	}
	
	for (int i = 0; i < certs.size(); i++) {
		Certification* cert = certs.get(i);
		certificationList.put(cert->getName(), cert);
		
		if (updateClient)
			dplay9->addSkill(cert->getName());
	}
	
	if (updateClient) {
		dplay9->close();
		sendMessage(dplay9);
	}
}

void PlayerImplementation::removeCertifications(Vector<Certification*>& certs, bool updateClient) {
	for (int i = 0; i < certs.size(); i++) {
		Certification* cert = certs.get(i);
		certificationList.drop(cert->getName());
	}
	
	if (updateClient) {
		PlayerObjectDeltaMessage9* dplay9 = new PlayerObjectDeltaMessage9(playerObject);
		dplay9->updateSkilsAndCertifications();
		dplay9->close();
		sendMessage(dplay9);
	}
}

void PlayerImplementation::increasePvpRating(int value) {
	pvpRating = pvpRating + value;	
}

void PlayerImplementation::decreasePvpRating(int value) {
	pvpRating = pvpRating - value;	
}

void PlayerImplementation::toggleCharacterBit(uint32 bit) {
	if (playerObject != NULL) {
		if (!playerObject->setCharacterBit(bit, true))
			playerObject->clearCharacterBit(bit, true);
	}
}

bool PlayerImplementation::awardBadge(uint32 badgeindex) {
  	if (badgeindex > 139)
  		return false; 

	badges.setBadge(badgeindex);
	
	return true;
}

void PlayerImplementation::getPlayersNearYou() {
	if (zone == NULL || objectID <= 0x15 || isIncapacitated() || isDead())
		return;

	try {
		zone->lock();

		PlayersNearYouMessage* pny = new PlayersNearYouMessage(_this);
	
		uint32 counter = 0;
		
		for (int i = 0; i < inRangeObjectCount(); ++i) {
			SceneObject* obj = (SceneObject*) (((SceneObjectImplementation*) getInRangeObject(i))->_getStub());
			
			if (obj->isPlayer()) {
				++counter;
				Player* player = (Player*) obj;
				pny->addFoundPlayer(player);
			}
		}
		
		pny->insertPlayerCounter(counter);
		sendMessage(pny);
	
		zone->unlock();
	} catch (...) {
		error("exception PlayerImplementation::getPlayersNearYou()");
		zone->unlock();
	}
}

void PlayerImplementation::addDatapadItem(SceneObject* item) {
	datapad->addObject(item);
}

SceneObject* PlayerImplementation::getDatapadItem(uint64 oid) {
	return datapad->getObject(oid);
}

void PlayerImplementation::removeDatapadItem(uint64 oid) {
	datapad->removeObject(oid);
}

SceneObject* PlayerImplementation::getPlayerItem(uint64 oid) {
	SceneObject* object = NULL;
	
	object = getInventoryItem(oid);

	if (object == NULL) {
		object = getDatapadItem(oid);

		if (object == NULL)
			object = getWaypoint(oid);
	}

	return object;
}

bool PlayerImplementation::setGuild(uint32 gid) {
	PlayerManager* playerManager = zone->getZoneServer()->getPlayerManager();
	GuildManager* guildManager = playerManager->getGuildManager();
	
	guild = guildManager->getGuild(gid);
	
	return guild != NULL;
}

bool PlayerImplementation::updateGuild(uint32 gid) {
	PlayerManager* playerManager = zone->getZoneServer()->getPlayerManager();
	GuildManager* guildManager = playerManager->getGuildManager();
	
	Guild* gld = guildManager->getGuild(gid);
	if (gld == NULL)
		return false;
	
	guild = gld;
	
	sendGuildTo();
	
	return true;
}

void PlayerImplementation::updateGuild(Guild* gld) {
	guild = gld;
	
	sendGuildTo();
}

void PlayerImplementation::sendGuildList() {
	if (guild != NULL)
		guild->sendGuildListTo(_this);
}

void PlayerImplementation::setAdminLevel(int level) {
	adminLevel = level;
	
	PlayerObjectDeltaMessage6* dplay6 = new PlayerObjectDeltaMessage6(playerObject);
	dplay6->setAdminLevel(level);
	dplay6->close();
	broadcastMessage(dplay6);
}

void PlayerImplementation::saveProfessions() {
	ProfessionManager* professionManager = server->getProfessionManager();
	professionManager->saveProfessions(this);
}

void PlayerImplementation::loadProfessions() {
	ProfessionManager* professionManager = server->getProfessionManager();
	professionManager->loadProfessions(this);
	
	accuracy = getSkillMod("unarmed_accuracy"); 
}

bool PlayerImplementation::trainSkillBox(const string& name) {
	ProfessionManager* professionManager = server->getProfessionManager();
	
	return professionManager->trainSkillBox(name, this);
}

void PlayerImplementation::surrenderSkillBox(const string& name) {
	ProfessionManager* professionManager = server->getProfessionManager();
	
	return professionManager->surrenderSkillBox(name, this);
}

void PlayerImplementation::newChangeFactionEvent(uint32 faction) {
	changeFactionEvent = new ChangeFactionEvent(this, faction);
	server->addEvent(changeFactionEvent);
}

void PlayerImplementation::setEntertainerEvent() {
	entertainerEvent = new EntertainerEvent(_this);
	
	SkillManager* skillManager = server->getSkillManager();
	Performance* performance = NULL;
	
	if(isDancing())
		performance = skillManager->getDance(getPerformanceName());
	else if(isPlayingMusic() && getInstrument() != NULL)
		performance = skillManager->getSong(getPerformanceName(), getInstrument()->getInstrumentType());
	else
		return;
	
	if(!performance) { // shouldn't happen
		stringstream msg;
		msg << "Performance was null in setEntertainerEvent.  Please report to McMahon! Name: " << getPerformanceName() << " and Type: " << dec << getInstrument()->getInstrumentType();

		sendSystemMessage(msg.str());
		return;
	}
	// I think the getLoopDuration is wrong now...thinking it should just be flat 10 seconds
	//server->addEvent(entertainerEvent, (uint64) performance->getLoopDuration() * 1000);
	server->addEvent(entertainerEvent, 10000);
}

void PlayerImplementation::setSurveyEvent(string& resourceName) {
	surveyEvent = new SurveyEvent(_this, resourceName);
	server->addEvent(surveyEvent, 5000);
}

void PlayerImplementation::setSampleEvent(string& resourceName, bool firstTime) {
	if (surveyTool == NULL) {
		sendSystemMessage("Please contact Ritter ASAP and log the exact actions you just took for a bug report. Thank you.");
		return;
	}
	
	if (getParent() != NULL && getParent()->isCell()) {
		sendSystemMessage("You cannot perform survey-related actions inside a structure.");
		return;
	} else if (isInCombat()) {
		ChatSystemMessage* sysMessage = new ChatSystemMessage("survey","sample_cancel_attack");
		sendMessage(sysMessage);
		return;
	} else if (isMounted()) {
		sendSystemMessage("You can not sample while riding a mount.");
		return;
	} else if (getInventoryItem(surveyTool->getObjectID()) == NULL) {
		ChatSystemMessage* sysMessage = new ChatSystemMessage("survey","sample_gone");
		sendMessage(sysMessage);
		return;
	} else if (getPosture() != CreatureObjectImplementation::CROUCHED_POSTURE) {
		return;
	}
		
	if (firstTime) {
		firstSampleEvent = new SampleEvent(_this, resourceName);
		server->addEvent(firstSampleEvent, 2000);
		
		sampleEvent = new SampleEvent(_this, resourceName, false, true);
		server->addEvent(sampleEvent, 14000);
	} else {
		firstSampleEvent = NULL;
		
		if (changeActionBar(-200, false) ) {
			activateRecovery();
			
			sampleEvent = new SampleEvent(_this, resourceName);

			getZone()->getZoneServer()->getResourceManager()->sendSampleMessage(_this, resourceName);
			
			server->addEvent(sampleEvent, 12000);
		} else {
			sendSystemMessage("You do not have enough action to do that.");
			changePosture(CreatureObjectImplementation::UPRIGHT_POSTURE);
		}
	}
}

void PlayerImplementation::sendSampleTimeRemaining() {
	// Precondition: sampleEvent != NULL
	int time = -(sampleEvent->getTimeStamp().miliDifference()) / 1000;
	
	unicode ustr = "";
	ChatSystemMessage* sysMessage = new ChatSystemMessage("survey","tool_recharge_time",ustr,time,false);
	sendMessage(sysMessage);
}

void PlayerImplementation::launchFirework() {
	//Create the firework in the world.
	FireworkWorld* firework = new FireworkWorld(_this);
	firework->setZoneProcessServer(server);
	firework->setDirection(0, 0, -0.64, 0.76);

	setPosture(CROUCHED_POSTURE);

	try {
		zone->lock();

		for (int i = 0; i < inRangeObjectCount(); ++i) {
			SceneObject* obj = (SceneObject*) (((SceneObjectImplementation*) getInRangeObject(i))->_getStub());
		
			if (obj->isPlayer()) {
				Player* player = (Player*) obj;

				firework->sendTo(player);
			
				Animation* anim = new Animation(_this, "manipulate_low");
				player->sendMessage(anim);
			}
		}

		zone->unlock();
	} catch (...) {
		zone->unlock();

		cout << "unreported Exception on Player::launchFirework()\n";
	}

	firework->finalize();		
}

int PlayerImplementation::getSlicingAbility() {
	string txt0 = "combat_smuggler_novice";
	string txt1 = "combat_smuggler_slicing_01";
	string txt2 = "combat_smuggler_slicing_02";
	string txt3 = "combat_smuggler_slicing_03";
	string txt4 = "combat_smuggler_slicing_04";
	string txt5 = "combat_smuggler_master";

	if (hasSkillBox(txt5))
		return 5;
	else if (hasSkillBox(txt4))
		return 4;
	else if (hasSkillBox(txt3))
		return 3;
	else if (hasSkillBox(txt2))
		return 2;
	else if (hasSkillBox(txt1))
		return 1;
	else if (hasSkillBox(txt0))
		return 0;
	
	return -1;
}
