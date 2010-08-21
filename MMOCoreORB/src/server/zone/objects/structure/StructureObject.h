/*
 *	server/zone/objects/structure/StructureObject.h generated by engine3 IDL compiler 0.60
 */

#ifndef STRUCTUREOBJECT_H_
#define STRUCTUREOBJECT_H_

#include "engine/orb/DistributedObjectBroker.h"

#include "engine/core/ManagedReference.h"

#include "engine/core/ManagedWeakReference.h"

namespace server {
namespace zone {
namespace objects {
namespace structure {
namespace events {

class StructureMaintenanceTask;

} // namespace events
} // namespace structure
} // namespace objects
} // namespace zone
} // namespace server

using namespace server::zone::objects::structure::events;

namespace server {
namespace zone {
namespace objects {
namespace scene {

class SceneObject;

} // namespace scene
} // namespace objects
} // namespace zone
} // namespace server

using namespace server::zone::objects::scene;

namespace server {
namespace zone {
namespace objects {
namespace creature {

class CreatureObject;

} // namespace creature
} // namespace objects
} // namespace zone
} // namespace server

using namespace server::zone::objects::creature;

namespace server {
namespace zone {
namespace objects {
namespace player {

class PlayerCreature;

} // namespace player
} // namespace objects
} // namespace zone
} // namespace server

using namespace server::zone::objects::player;

namespace server {
namespace zone {
namespace templates {

class SharedObjectTemplate;

} // namespace templates
} // namespace zone
} // namespace server

using namespace server::zone::templates;

#include "server/zone/objects/structure/StructurePermissionList.h"

#include "server/zone/objects/tangible/TangibleObject.h"

namespace server {
namespace zone {
namespace objects {
namespace structure {

class StructureObject : public TangibleObject {
public:
	StructureObject();

	void initializeTransientMembers();

	void loadTemplateData(SharedObjectTemplate* templateData);

	void createChildObjects();

	int notifyStructurePlaced(PlayerCreature* player);

	bool checkRequisitesForPlacement(PlayerCreature* player);

	unsigned int generateDestroyCode();

	bool isValidDestroyCode(unsigned int code);

	void scheduleMaintenanceExpirationEvent();

	bool isOnAdminList(SceneObject* obj);

	bool isOnAdminList(unsigned long long oid);

	bool isOnEntryList(SceneObject* obj);

	bool isOnEntryList(unsigned long long oid);

	bool isOnBanList(SceneObject* obj);

	bool isOnBanList(unsigned long long oid);

	bool isOwnerOf(SceneObject* obj);

	bool isOwnerOf(unsigned long long oid);

	bool isOnAccessList(SceneObject* obj);

	bool isOnAccessList(unsigned long long oid);

	void sendPermissionListTo(PlayerCreature* player, const String& listName);

	bool addPermission(PlayerCreature* player, PlayerCreature* targetPlayer, const String& listName);

	bool addPermission(PlayerCreature* player, const String& targetPlayerName, const String& listName);

	bool removePermission(PlayerCreature* player, PlayerCreature* targetPlayer, const String& listName);

	bool removePermission(PlayerCreature* player, const String& targetPlayerName, const String& listName);

	int getRedeedCost();

	unsigned long long getOwnerObjectID();

	unsigned long long getDeedObjectID();

	int getLotSize();

	unsigned int getDestroyCode();

	int getBaseMaintenanceRate();

	int getBasePowerRate();

	int getSurplusMaintenance();

	int getSurplusPower();

	bool isPublicStructure();

	void setOwnerObjectID(unsigned long long objectID);

	void setDeedObjectID(unsigned long long deedID);

	void setLotSize(int lots);

	void setDestroyCode(unsigned int code);

	void setBaseMaintenanceRate(int rate);

	void setBasePowerRate(int rate);

	void setSurplusMaintenance(int surplus);

	void addMaintenance(int add);

	void setSurplusPower(int surplus);

	void addPower(int add);

	void setPublicStructure(bool privacy);

	bool isStructureObject();

	bool isRedeedable();

	int getLength();

	void setLength(int len);

	int getWidth();

	void setWidth(int wid);

protected:
	StructureObject(DummyConstructorParameter* param);

	virtual ~StructureObject();

	friend class StructureObjectHelper;
};

} // namespace structure
} // namespace objects
} // namespace zone
} // namespace server

using namespace server::zone::objects::structure;

namespace server {
namespace zone {
namespace objects {
namespace structure {

class StructureObjectImplementation : public TangibleObjectImplementation {
protected:
	StructureMaintenanceTask* structureMaintenanceTask;

	StructurePermissionList structurePermissionList;

	Time maintenanceExpires;

	Time powerExpires;

	unsigned long long ownerObjectID;

	unsigned long long deedObjectID;

	int lotSize;

	unsigned int destroyCode;

	int baseMaintenanceRate;

	int basePowerRate;

	float surplusMaintenance;

	float surplusPower;

	int length;

	int width;

	bool publicStructure;

public:
	StructureObjectImplementation();

	StructureObjectImplementation(DummyConstructorParameter* param);

	void initializeTransientMembers();

	void loadTemplateData(SharedObjectTemplate* templateData);

	void createChildObjects();

	virtual int notifyStructurePlaced(PlayerCreature* player);

	virtual bool checkRequisitesForPlacement(PlayerCreature* player);

	virtual unsigned int generateDestroyCode();

	bool isValidDestroyCode(unsigned int code);

	void scheduleMaintenanceExpirationEvent();

	bool isOnAdminList(SceneObject* obj);

	bool isOnAdminList(unsigned long long oid);

	bool isOnEntryList(SceneObject* obj);

	bool isOnEntryList(unsigned long long oid);

	bool isOnBanList(SceneObject* obj);

	bool isOnBanList(unsigned long long oid);

	bool isOwnerOf(SceneObject* obj);

	bool isOwnerOf(unsigned long long oid);

	bool isOnAccessList(SceneObject* obj);

	bool isOnAccessList(unsigned long long oid);

	void sendPermissionListTo(PlayerCreature* player, const String& listName);

	bool addPermission(PlayerCreature* player, PlayerCreature* targetPlayer, const String& listName);

	bool addPermission(PlayerCreature* player, const String& targetPlayerName, const String& listName);

	bool removePermission(PlayerCreature* player, PlayerCreature* targetPlayer, const String& listName);

	bool removePermission(PlayerCreature* player, const String& targetPlayerName, const String& listName);

	int getRedeedCost();

	unsigned long long getOwnerObjectID();

	unsigned long long getDeedObjectID();

	int getLotSize();

	unsigned int getDestroyCode();

	int getBaseMaintenanceRate();

	int getBasePowerRate();

	int getSurplusMaintenance();

	int getSurplusPower();

	bool isPublicStructure();

	void setOwnerObjectID(unsigned long long objectID);

	void setDeedObjectID(unsigned long long deedID);

	void setLotSize(int lots);

	void setDestroyCode(unsigned int code);

	void setBaseMaintenanceRate(int rate);

	void setBasePowerRate(int rate);

	void setSurplusMaintenance(int surplus);

	void addMaintenance(int add);

	void setSurplusPower(int surplus);

	void addPower(int add);

	void setPublicStructure(bool privacy);

	bool isStructureObject();

	bool isRedeedable();

	int getLength();

	void setLength(int len);

	int getWidth();

	void setWidth(int wid);

	StructureObject* _this;

	operator const StructureObject*();

	DistributedObjectStub* _getStub();
protected:
	virtual ~StructureObjectImplementation();

	void finalize();

	void _initializeImplementation();

	void _setStub(DistributedObjectStub* stub);

	void lock(bool doLock = true);

	void lock(ManagedObject* obj);

	void rlock(bool doLock = true);

	void wlock(bool doLock = true);

	void wlock(ManagedObject* obj);

	void unlock(bool doLock = true);

	void runlock(bool doLock = true);

	void _serializationHelperMethod();

	friend class StructureObject;
};

class StructureObjectAdapter : public TangibleObjectAdapter {
public:
	StructureObjectAdapter(StructureObjectImplementation* impl);

	Packet* invokeMethod(sys::uint32 methid, DistributedMethod* method);

	void initializeTransientMembers();

	void createChildObjects();

	int notifyStructurePlaced(PlayerCreature* player);

	bool checkRequisitesForPlacement(PlayerCreature* player);

	unsigned int generateDestroyCode();

	bool isValidDestroyCode(unsigned int code);

	void scheduleMaintenanceExpirationEvent();

	bool isOnAdminList(SceneObject* obj);

	bool isOnAdminList(unsigned long long oid);

	bool isOnEntryList(SceneObject* obj);

	bool isOnEntryList(unsigned long long oid);

	bool isOnBanList(SceneObject* obj);

	bool isOnBanList(unsigned long long oid);

	bool isOwnerOf(SceneObject* obj);

	bool isOwnerOf(unsigned long long oid);

	bool isOnAccessList(SceneObject* obj);

	bool isOnAccessList(unsigned long long oid);

	void sendPermissionListTo(PlayerCreature* player, const String& listName);

	bool addPermission(PlayerCreature* player, PlayerCreature* targetPlayer, const String& listName);

	bool addPermission(PlayerCreature* player, const String& targetPlayerName, const String& listName);

	bool removePermission(PlayerCreature* player, PlayerCreature* targetPlayer, const String& listName);

	bool removePermission(PlayerCreature* player, const String& targetPlayerName, const String& listName);

	int getRedeedCost();

	unsigned long long getOwnerObjectID();

	unsigned long long getDeedObjectID();

	int getLotSize();

	unsigned int getDestroyCode();

	int getBaseMaintenanceRate();

	int getBasePowerRate();

	int getSurplusMaintenance();

	int getSurplusPower();

	bool isPublicStructure();

	void setOwnerObjectID(unsigned long long objectID);

	void setDeedObjectID(unsigned long long deedID);

	void setLotSize(int lots);

	void setDestroyCode(unsigned int code);

	void setBaseMaintenanceRate(int rate);

	void setBasePowerRate(int rate);

	void setSurplusMaintenance(int surplus);

	void addMaintenance(int add);

	void setSurplusPower(int surplus);

	void addPower(int add);

	void setPublicStructure(bool privacy);

	bool isStructureObject();

	bool isRedeedable();

	int getLength();

	void setLength(int len);

	int getWidth();

	void setWidth(int wid);

protected:
	String _param1_sendPermissionListTo__PlayerCreature_String_;
	String _param2_addPermission__PlayerCreature_PlayerCreature_String_;
	String _param1_addPermission__PlayerCreature_String_String_;
	String _param2_addPermission__PlayerCreature_String_String_;
	String _param2_removePermission__PlayerCreature_PlayerCreature_String_;
	String _param1_removePermission__PlayerCreature_String_String_;
	String _param2_removePermission__PlayerCreature_String_String_;
};

class StructureObjectHelper : public DistributedObjectClassHelper, public Singleton<StructureObjectHelper> {
	static StructureObjectHelper* staticInitializer;

public:
	StructureObjectHelper();

	void finalizeHelper();

	DistributedObject* instantiateObject();

	DistributedObjectServant* instantiateServant();

	DistributedObjectAdapter* createAdapter(DistributedObjectStub* obj);

	friend class Singleton<StructureObjectHelper>;
};

} // namespace structure
} // namespace objects
} // namespace zone
} // namespace server

using namespace server::zone::objects::structure;

#endif /*STRUCTUREOBJECT_H_*/
