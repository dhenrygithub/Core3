/*
 *	server/zone/objects/installation/shuttle/ShuttleInstallation.cpp generated by engine3 IDL compiler 0.60
 */

#include "ShuttleInstallation.h"

#include "server/zone/objects/creature/CreatureObject.h"

#include "server/zone/objects/player/PlayerCreature.h"

#include "server/zone/packets/object/ObjectMenuResponse.h"

#include "server/zone/packets/scene/AttributeListMessage.h"

#include "server/zone/packets/object/ObjectMenuResponse.h"

#include "server/zone/Zone.h"

#include "server/zone/templates/SharedObjectTemplate.h"

#include "server/zone/objects/creature/shuttle/ShuttleCreature.h"

#include "server/zone/objects/tangible/terminal/ticketcollector/TicketCollector.h"

#include "server/zone/objects/tangible/terminal/travel/TravelTerminal.h"

/*
 *	ShuttleInstallationStub
 */

ShuttleInstallation::ShuttleInstallation() : InstallationObject(DummyConstructorParameter::instance()) {
	ShuttleInstallationImplementation* _implementation = new ShuttleInstallationImplementation();
	_impl = _implementation;
	_impl->_setStub(this);
}

ShuttleInstallation::ShuttleInstallation(DummyConstructorParameter* param) : InstallationObject(param) {
}

ShuttleInstallation::~ShuttleInstallation() {
}


void ShuttleInstallation::insertToZone(Zone* zone) {
	ShuttleInstallationImplementation* _implementation = (ShuttleInstallationImplementation*) _getImplementation();
	if (_implementation == NULL) {
		if (!deployed)
			throw ObjectNotDeployedException(this);

		DistributedMethod method(this, 6);
		method.addObjectParameter(zone);

		method.executeWithVoidReturn();
	} else
		_implementation->insertToZone(zone);
}

void ShuttleInstallation::removeFromZone() {
	ShuttleInstallationImplementation* _implementation = (ShuttleInstallationImplementation*) _getImplementation();
	if (_implementation == NULL) {
		if (!deployed)
			throw ObjectNotDeployedException(this);

		DistributedMethod method(this, 7);

		method.executeWithVoidReturn();
	} else
		_implementation->removeFromZone();
}

void ShuttleInstallation::spawnShuttleObjects() {
	ShuttleInstallationImplementation* _implementation = (ShuttleInstallationImplementation*) _getImplementation();
	if (_implementation == NULL) {
		if (!deployed)
			throw ObjectNotDeployedException(this);

		DistributedMethod method(this, 8);

		method.executeWithVoidReturn();
	} else
		_implementation->spawnShuttleObjects();
}

void ShuttleInstallation::despawnShuttleObjects() {
	ShuttleInstallationImplementation* _implementation = (ShuttleInstallationImplementation*) _getImplementation();
	if (_implementation == NULL) {
		if (!deployed)
			throw ObjectNotDeployedException(this);

		DistributedMethod method(this, 9);

		method.executeWithVoidReturn();
	} else
		_implementation->despawnShuttleObjects();
}

bool ShuttleInstallation::checkRequisitesForPlacement(PlayerCreature* player) {
	ShuttleInstallationImplementation* _implementation = (ShuttleInstallationImplementation*) _getImplementation();
	if (_implementation == NULL) {
		if (!deployed)
			throw ObjectNotDeployedException(this);

		DistributedMethod method(this, 10);
		method.addObjectParameter(player);

		return method.executeWithBooleanReturn();
	} else
		return _implementation->checkRequisitesForPlacement(player);
}

DistributedObjectServant* ShuttleInstallation::_getImplementation() {

	_updated = true;
	return _impl;
}

void ShuttleInstallation::_setImplementation(DistributedObjectServant* servant) {
	_impl = servant;
}

/*
 *	ShuttleInstallationImplementation
 */

ShuttleInstallationImplementation::ShuttleInstallationImplementation(DummyConstructorParameter* param) : InstallationObjectImplementation(param) {
	_initializeImplementation();
}


ShuttleInstallationImplementation::~ShuttleInstallationImplementation() {
}


void ShuttleInstallationImplementation::finalize() {
}

void ShuttleInstallationImplementation::_initializeImplementation() {
	_setClassHelper(ShuttleInstallationHelper::instance());

	_serializationHelperMethod();
	_serializationHelperMethod();
}

void ShuttleInstallationImplementation::_setStub(DistributedObjectStub* stub) {
	_this = (ShuttleInstallation*) stub;
	InstallationObjectImplementation::_setStub(stub);
}

DistributedObjectStub* ShuttleInstallationImplementation::_getStub() {
	return _this;
}

ShuttleInstallationImplementation::operator const ShuttleInstallation*() {
	return _this;
}

void ShuttleInstallationImplementation::lock(bool doLock) {
	_this->lock(doLock);
}

void ShuttleInstallationImplementation::lock(ManagedObject* obj) {
	_this->lock(obj);
}

void ShuttleInstallationImplementation::rlock(bool doLock) {
	_this->rlock(doLock);
}

void ShuttleInstallationImplementation::wlock(bool doLock) {
	_this->wlock(doLock);
}

void ShuttleInstallationImplementation::wlock(ManagedObject* obj) {
	_this->wlock(obj);
}

void ShuttleInstallationImplementation::unlock(bool doLock) {
	_this->unlock(doLock);
}

void ShuttleInstallationImplementation::runlock(bool doLock) {
	_this->runlock(doLock);
}

void ShuttleInstallationImplementation::_serializationHelperMethod() {
	InstallationObjectImplementation::_serializationHelperMethod();

	_setClassName("ShuttleInstallation");

}

void ShuttleInstallationImplementation::readObject(ObjectInputStream* stream) {
	uint16 _varCount = stream->readShort();
	for (int i = 0; i < _varCount; ++i) {
		String _name;
		_name.parseFromBinaryStream(stream);

		uint16 _varSize = stream->readShort();

		int _currentOffset = stream->getOffset();

		if(ShuttleInstallationImplementation::readObjectMember(stream, _name)) {
		}

		stream->setOffset(_currentOffset + _varSize);
	}

	initializeTransientMembers();
}

bool ShuttleInstallationImplementation::readObjectMember(ObjectInputStream* stream, const String& _name) {
	if (InstallationObjectImplementation::readObjectMember(stream, _name))
		return true;

	if (_name == "shuttle") {
		TypeInfo<ManagedReference<ShuttleCreature* > >::parseFromBinaryStream(&shuttle, stream);
		return true;
	}

	if (_name == "ticketCollector") {
		TypeInfo<ManagedReference<TicketCollector* > >::parseFromBinaryStream(&ticketCollector, stream);
		return true;
	}

	if (_name == "travelTerminal") {
		TypeInfo<ManagedReference<TravelTerminal* > >::parseFromBinaryStream(&travelTerminal, stream);
		return true;
	}


	return false;
}

void ShuttleInstallationImplementation::writeObject(ObjectOutputStream* stream) {
	int _currentOffset = stream->getOffset();
	stream->writeShort(0);
	int _varCount = ShuttleInstallationImplementation::writeObjectMembers(stream);
	stream->writeShort(_currentOffset, _varCount);
}

int ShuttleInstallationImplementation::writeObjectMembers(ObjectOutputStream* stream) {
	String _name;
	int _offset;
	uint16 _totalSize;
	_name = "shuttle";
	_name.toBinaryStream(stream);
	_offset = stream->getOffset();
	stream->writeShort(0);
	TypeInfo<ManagedReference<ShuttleCreature* > >::toBinaryStream(&shuttle, stream);
	_totalSize = (uint16) (stream->getOffset() - (_offset + 2));
	stream->writeShort(_offset, _totalSize);

	_name = "ticketCollector";
	_name.toBinaryStream(stream);
	_offset = stream->getOffset();
	stream->writeShort(0);
	TypeInfo<ManagedReference<TicketCollector* > >::toBinaryStream(&ticketCollector, stream);
	_totalSize = (uint16) (stream->getOffset() - (_offset + 2));
	stream->writeShort(_offset, _totalSize);

	_name = "travelTerminal";
	_name.toBinaryStream(stream);
	_offset = stream->getOffset();
	stream->writeShort(0);
	TypeInfo<ManagedReference<TravelTerminal* > >::toBinaryStream(&travelTerminal, stream);
	_totalSize = (uint16) (stream->getOffset() - (_offset + 2));
	stream->writeShort(_offset, _totalSize);


	return 3 + InstallationObjectImplementation::writeObjectMembers(stream);
}

ShuttleInstallationImplementation::ShuttleInstallationImplementation() {
	_initializeImplementation();
	// server/zone/objects/installation/shuttle/ShuttleInstallation.idl():  		setLoggingName("ShuttleInstallation");
	setLoggingName("ShuttleInstallation");
}

void ShuttleInstallationImplementation::insertToZone(Zone* zone) {
	// server/zone/objects/installation/shuttle/ShuttleInstallation.idl():  		super.insertToZone(zone);
	InstallationObjectImplementation::insertToZone(zone);
	// server/zone/objects/installation/shuttle/ShuttleInstallation.idl():  		spawnShuttleObjects();
	spawnShuttleObjects();
}

void ShuttleInstallationImplementation::removeFromZone() {
	// server/zone/objects/installation/shuttle/ShuttleInstallation.idl():  		despawnShuttleObjects();
	despawnShuttleObjects();
	// server/zone/objects/installation/shuttle/ShuttleInstallation.idl():  		super.removeFromZone();
	InstallationObjectImplementation::removeFromZone();
	// server/zone/objects/installation/shuttle/ShuttleInstallation.idl():  		updateToDatabaseWithoutChildren();
	updateToDatabaseWithoutChildren();
}

/*
 *	ShuttleInstallationAdapter
 */

ShuttleInstallationAdapter::ShuttleInstallationAdapter(ShuttleInstallationImplementation* obj) : InstallationObjectAdapter(obj) {
}

enum {RPC_INSERTTOZONE__ZONE_ = 6,RPC_REMOVEFROMZONE__,RPC_SPAWNSHUTTLEOBJECTS__,RPC_DESPAWNSHUTTLEOBJECTS__,RPC_CHECKREQUISITESFORPLACEMENT__PLAYERCREATURE_};

Packet* ShuttleInstallationAdapter::invokeMethod(uint32 methid, DistributedMethod* inv) {
	Packet* resp = new MethodReturnMessage(0);

	switch (methid) {
	case RPC_INSERTTOZONE__ZONE_:
		insertToZone((Zone*) inv->getObjectParameter());
		break;
	case RPC_REMOVEFROMZONE__:
		removeFromZone();
		break;
	case RPC_SPAWNSHUTTLEOBJECTS__:
		spawnShuttleObjects();
		break;
	case RPC_DESPAWNSHUTTLEOBJECTS__:
		despawnShuttleObjects();
		break;
	case RPC_CHECKREQUISITESFORPLACEMENT__PLAYERCREATURE_:
		resp->insertBoolean(checkRequisitesForPlacement((PlayerCreature*) inv->getObjectParameter()));
		break;
	default:
		return NULL;
	}

	return resp;
}

void ShuttleInstallationAdapter::insertToZone(Zone* zone) {
	((ShuttleInstallationImplementation*) impl)->insertToZone(zone);
}

void ShuttleInstallationAdapter::removeFromZone() {
	((ShuttleInstallationImplementation*) impl)->removeFromZone();
}

void ShuttleInstallationAdapter::spawnShuttleObjects() {
	((ShuttleInstallationImplementation*) impl)->spawnShuttleObjects();
}

void ShuttleInstallationAdapter::despawnShuttleObjects() {
	((ShuttleInstallationImplementation*) impl)->despawnShuttleObjects();
}

bool ShuttleInstallationAdapter::checkRequisitesForPlacement(PlayerCreature* player) {
	return ((ShuttleInstallationImplementation*) impl)->checkRequisitesForPlacement(player);
}

/*
 *	ShuttleInstallationHelper
 */

ShuttleInstallationHelper* ShuttleInstallationHelper::staticInitializer = ShuttleInstallationHelper::instance();

ShuttleInstallationHelper::ShuttleInstallationHelper() {
	className = "ShuttleInstallation";

	Core::getObjectBroker()->registerClass(className, this);
}

void ShuttleInstallationHelper::finalizeHelper() {
	ShuttleInstallationHelper::finalize();
}

DistributedObject* ShuttleInstallationHelper::instantiateObject() {
	return new ShuttleInstallation(DummyConstructorParameter::instance());
}

DistributedObjectServant* ShuttleInstallationHelper::instantiateServant() {
	return new ShuttleInstallationImplementation(DummyConstructorParameter::instance());
}

DistributedObjectAdapter* ShuttleInstallationHelper::createAdapter(DistributedObjectStub* obj) {
	DistributedObjectAdapter* adapter = new ShuttleInstallationAdapter((ShuttleInstallationImplementation*) obj->_getImplementation());

	obj->_setClassName(className);
	obj->_setClassHelper(this);

	adapter->setStub(obj);

	return adapter;
}

