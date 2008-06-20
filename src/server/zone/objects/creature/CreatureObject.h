/*
 *	server/zone/objects/creature/CreatureObject.h generated by engine3 IDL compiler 0.55
 */

#ifndef CREATUREOBJECT_H_
#define CREATUREOBJECT_H_

#include "engine/orb/DistributedObjectBroker.h"

#include "engine/service/proto/BaseMessage.h"

#include "engine/service/proto/StandaloneBaseMessage.h"

class SceneObject;

class Inventory;

class Container;

class TangibleObject;

class Weapon;

class Armor;

class GroupObject;

class Player;

class BuildingObject;

class MountCreature;

class Guild;

class Skill;

class BuffObject;

#include "../scene/SceneObject.h"

class CreatureObject : public SceneObject {
public:
	CreatureObject(unsigned long long oid);

	void info(const string& message, bool forcedLog = false);

	void sendItemsTo(Player* player);

	void sendTo(Player* player, bool doClose = true);

	void sendDestroyTo(Player* player);

	void doCombatAnimation(CreatureObject* defender, unsigned int animcrc, bool hit = false);

	void doAnimation(const string& anim);

	void playEffect(const string& file, const string& aux);

	void showFlyText(const string& file, const string& aux, unsigned char red, unsigned char green, unsigned char blue);

	void sendCombatSpam(CreatureObject* defender, TangibleObject* item, unsigned int damage, const string& skill, bool areaSpam = true);

	void addDamage(CreatureObject* creature, unsigned int damage);

	unsigned int getDamage(CreatureObject* creature);

	bool isLootOwner(CreatureObject* creature);

	unsigned int getMitigation(const string& mit);

	bool isIncapacitated();

	bool isDead();

	bool isKnockedDown();

	bool isKneeled();

	bool isProne();

	bool isInCombat();

	bool isDizzied();

	bool isStunned();

	bool isBlinded();

	bool isIntimidated();

	bool isDiseased();

	bool isPoisoned();

	bool isBleeding();

	bool isOnFire();

	bool isMounted();

	bool isRidingCreature();

	bool isMeditating();

	void updateKnockdownRecovery();

	void updateMood(const string& md);

	bool checkKnockdownRecovery();

	void updatePostureDownRecovery();

	void updatePostureUpRecovery();

	bool checkPostureDownRecovery();

	bool checkPostureUpRecovery();

	bool setState(unsigned long long state);

	bool clearState(unsigned long long state);

	void updateStates();

	void clearStates();

	float calculateBFRatio();

	void resetHAMBars();

	bool changeHAMBars(int hp, int ap, int mp, bool forcedChange = false);

	bool changeHAMWounds(int hpwnd, int apwnd, int mpwnd, bool forcedChange = false);

	void changeMaxHAMBars(int hp, int ap, int mp);

	bool takeHealthDamage(int damage);

	bool takeActionDamage(int damage);

	bool takeMindDamage(int damage);

	bool changeHealthWoundsBar(int wounds, bool forcedChange = false);

	bool changeActionWoundsBar(int wounds, bool forcedChange = false);

	bool changeMindWoundsBar(int wounds, bool forcedChange = false);

	bool changeHealthBar(int hp, bool forcedChange = false);

	bool changeStrengthBar(int st, bool forcedChange = false);

	bool changeConstitutionBar(int cs, bool forcedChange = false);

	void changeMaxHealthBar(int hp);

	void changeMaxStrengthBar(int hp);

	void changeMaxConstitutionBar(int hp);

	void changeShockWounds(int bf);

	bool changeActionBar(int ap, bool forcedChange = false);

	bool changeQuicknessBar(int qc, bool forcedChange = false);

	bool changeStaminaBar(int st, bool forcedChange = false);

	void changeMaxActionBar(int hp);

	void changeMaxQuicknessBar(int hp);

	void changeMaxStaminaBar(int hp);

	bool changeMindBar(int mp, bool forcedChange = false);

	bool changeFocusBar(int fc, bool forcedChange = false);

	bool changeWillpowerBar(int wl, bool forcedChange = false);

	void changeMaxMindBar(int hp);

	void changeMaxFocusBar(int hp);

	void changeMaxWillpowerBar(int hp);

	void setHealthBar(unsigned int hp);

	void setStrengthBar(unsigned int st);

	void setConstitutionBar(unsigned int cst);

	void setMaxHealthBar(unsigned int hp, bool updateClient = true);

	void setMaxStrengthBar(unsigned int st, bool updateClient = true);

	void setMaxConstitutionBar(unsigned int cst, bool updateClient = true);

	void setActionBar(unsigned int ap);

	void setQuicknessBar(unsigned int qck);

	void setStaminaBar(unsigned int sta);

	void setMaxActionBar(unsigned int ap, bool updateClient = true);

	void setMaxQuicknessBar(unsigned int qck, bool updateClient = true);

	void setMaxStaminaBar(unsigned int sta, bool updateClient = true);

	void setMindBar(unsigned int mp);

	void setFocusBar(unsigned int fc);

	void setWillpowerBar(unsigned int will);

	void setMaxMindBar(unsigned int mp, bool updateClient = true);

	void setMaxFocusBar(unsigned int fc, bool updateClient = true);

	void setMaxWillpowerBar(unsigned int will, bool updateClient = true);

	bool hasAttackDelay();

	void clearAttackDelay();

	void activateBurstRun();

	bool hasQueuedState(unsigned int skillCRC);

	bool removeQueuedState(unsigned int skillCRC);

	bool addQueuedState(unsigned int skillCRC);

	void setMeditateState();

	bool isGuilded();

	void removeGroup();

	bool isInAGroup();

	void setGroupID(unsigned long long gid);

	void updateGroupId(unsigned long long id);

	void updateGroupInviterId(unsigned long long id);

	void setDefenderID(long long did);

	void updateTarget(unsigned long long targ);

	void updateTarget(SceneObject* targ);

	void setActionCounter(unsigned int actioncntr);

	void setWeapon(Weapon* wep);

	void setCreatureLinkID(unsigned long long creatureID);

	void setAccuracy(int acc);

	void setDamageBonus(int bonus);

	void setDefenseBonus(int bonus);

	void setLastMovementUpdateStamp(unsigned int tme);

	void setIgnoreMovementTests(int times);

	void setLevel(unsigned int lvl);

	void updateServerMovementStamp();

	SceneObject* getInventoryItem(unsigned long long oid);

	void removeInventoryItem(unsigned long long oid);

	SceneObject* getLootItem(unsigned long long oid);

	void removeLootItem(unsigned long long oid);

	void broadcastMessage(BaseMessage* msg, int range = 128, bool doLock = true);

	void broadcastMessage(StandaloneBaseMessage* msg, int range = 128, bool doLock = true);

	Inventory* getInventory();

	Container* getLootContainer();

	unsigned long long getNewItemID();

	Weapon* getWeapon();

	Armor* getArmor(int type);

	void addSkill(Skill* skill);

	int getSkillMod(const string& name);

	bool hasSkillMod(const string& name);

	int getSkillModBonus(const string& name);

	bool hasSkillModBonus(const string& name);

	int getAccuracy();

	int getDefenseBonus();

	int getDamageBonus();

	int getConditionDamage();

	int getMaxCondition();

	int getCondition();

	MountCreature* getMount();

	unsigned long long getLastServerMovementStamp();

	unsigned long long getCreatureLinkID();

	float getDistanceTo(CreatureObject* targetCreature);

	float getHeight();

	unsigned int getCreatureSkillsCount();

	unsigned int getNewCreatureSkillsCount(int cnt);

	unsigned int getLastMovementUpdateStamp();

	int getIgnoreMovementTests();

	unsigned int getLevel();

	int getCenteredBonus();

	void sendSystemMessage(const string& message);

	void sendSystemMessage(const string& file, const string& str, unsigned long long targetid = 0);

	void addInventoryItem(TangibleObject* item);

	void addLootItem(TangibleObject* item);

	void startDancing(const string& anim, bool changeDance = false);

	void startPlayingMusic(const string& anim, bool changeDance = false);

	void startWatch(unsigned long long entid);

	void startListen(unsigned long long entid);

	void stopWatch(unsigned long long entid, bool doSendPackets = true, bool forced = false, bool doLock = true);

	void stopListen(unsigned long long entid, bool doSendPackets = true, bool forced = false, bool doLock = true);

	bool isPlayingMusic();

	bool isDancing();

	void stopDancing();

	void stopPlayingMusic();

	void addListener(CreatureObject* creature);

	void addWatcher(CreatureObject* creature);

	void removeListener(CreatureObject* creature);

	void removeWatcher(CreatureObject* creature);

	void doFlourish(const string& modifier);

	void doPerformanceAction();

	void doEntertainerPatronEffects(bool healShock = false, bool healWounds = false, bool addBuff = false);

	void addEntertainerFlourishBuff();

	void addEntertainerBuffDuration(int performanceType, float duration);

	void setEntertainerBuffDuration(int performanceType, float duration);

	float getEntertainerBuffDuration(int performanceType);

	void activateRecovery();

	int getCreatureSkillsSize();

	string& getSkill(int idx);

	bool hasSkill(unsigned int skillCRC);

	void mountCreature(MountCreature* mnt, bool lockMount = true);

	void dismount(bool lockMount = true, bool ignoreCooldown = true);

	void addCashCredits(unsigned int credits);

	void addBankCredits(unsigned int credits);

	void updateCashCredits(unsigned int credits);

	void updateBankCredits(unsigned int credits);

	void subtractCashCredits(unsigned int credits);

	void subtractBankCredits(unsigned int credits);

	void setCashCredits(int credits);

	void setBankCredits(int credits);

	void addBuff(int buffCRC, float duration);

	void applyBuff(BuffObject* buff);

	void removeBuff(const unsigned int buffCRC, bool remove = true);

	bool hasSpice();

	bool verifyCashCredits(unsigned int creditsToRemove);

	bool verifyBankCredits(unsigned int creditsToRemove);

	bool isOnFullHealth();

	bool hasStates();

	bool hasState(unsigned long long state);

	bool hasWounds();

	bool hasShockWounds();

	bool isAttackable();

	bool isAttackableBy(CreatureObject* attacker);

	bool isListening();

	bool isWatching();

	unicode& getCharacterName();

	string& getStfName();

	string& getSpeciesName();

	string& getRaceName();

	string& getGender();

	string& getTerrainName();

	void getCharacterAppearance(string& appearance);

	bool isOvert();

	unsigned long long getStateBitmask();

	unsigned int getCreatureBitmask();

	unsigned char getPosture();

	unsigned int getFaction();

	bool isRebel();

	bool isNeutral();

	bool isImperial();

	unsigned int getPvpStatusBitmask();

	unsigned int getBankCredits();

	unsigned int getCashCredits();

	unsigned int getWoundsUpdateCounter();

	unsigned int getNewWoundsUpdateCounter(int upd);

	unsigned int getBaseHealth();

	unsigned int getBaseStrength();

	unsigned int getBaseConstitution();

	unsigned int getBaseAction();

	unsigned int getBaseQuickness();

	unsigned int getBaseStamina();

	unsigned int getBaseMind();

	unsigned int getBaseFocus();

	unsigned int getBaseWillpower();

	unsigned int getHealth();

	unsigned int getStrength();

	unsigned int getConstitution();

	unsigned int getAction();

	unsigned int getQuickness();

	unsigned int getStamina();

	unsigned int getMind();

	unsigned int getFocus();

	unsigned int getWillpower();

	unsigned int getHealthMax();

	unsigned int getStrengthMax();

	unsigned int getConstitutionMax();

	unsigned int getActionMax();

	unsigned int getQuicknessMax();

	unsigned int getStaminaMax();

	unsigned int getMindMax();

	unsigned int getFocusMax();

	unsigned int getWillpowerMax();

	unsigned int getArmor();

	float getArmorResist(int resistType);

	unsigned int getHealthWounds();

	unsigned int getActionWounds();

	unsigned int getMindWounds();

	unsigned int getShockWounds();

	unsigned int getHAMUpdateCounter();

	unsigned int getNewHAMUpdateCounter(int cnt);

	unsigned int getHAMMaxUpdateCounter();

	unsigned int getNewHAMMaxUpdateCounter(int cnt);

	unsigned int getHealthEncumbrance();

	unsigned int getActionEncumbrance();

	unsigned int getMindEncumbrance();

	unsigned long long getTargetID();

	SceneObject* getTarget();

	unsigned long long getWeaponID();

	unsigned long long getGroupID();

	unsigned long long getGuildID();

	Guild* getGuild();

	float getSpeed();

	float getAcceleration();

	unsigned long long getDefenderID();

	unsigned int getDefenderUpdateCounter();

	unsigned int getNewDefenderUpdateCounter(int cnt);

	string& getPerformanceAnimation();

	unsigned int getPerformanceCounter();

	unsigned int getInstrumentID();

	string& getPerformanceName();

	string& getMood();

	unsigned char getMoodID();

	unsigned long long getGroupInviterID();

	unsigned long long getGroupInviteCounter();

	unsigned long long getNewGroupInviteCounter();

	unsigned long long getWatchID();

	unsigned long long getListenID();

	bool isInBuilding();

	int getBuildingType();

	string& getGuildName();

	int getWeaponSpeedModifier();

	unsigned int getSkillBoxesUpdateCounter();

	unsigned int getSkillModsCounter();

	unsigned int getNewSkillModsCounter(int cnt);

	void setBaseHealth(unsigned int health);

	void setBaseStrength(unsigned int strength);

	void setBaseConstitution(unsigned int constituition);

	void setBaseAction(unsigned int action);

	void setBaseQuickness(unsigned int quickness);

	void setBaseStamina(unsigned int stamina);

	void setBaseMind(unsigned int mind);

	void setBaseFocus(unsigned int focus);

	void setBaseWillpower(unsigned int willpower);

	void setHealth(unsigned int health);

	void setStrength(unsigned int strength);

	void setConstitution(unsigned int constituition);

	void setAction(unsigned int action);

	void setQuickness(unsigned int quickness);

	void setStamina(unsigned int stamina);

	void setMind(unsigned int mind);

	void setFocus(unsigned int focus);

	void setWillpower(unsigned int willpower);

	void setHealthMax(unsigned int health);

	void setStrengthMax(unsigned int strength);

	void setConstitutionMax(unsigned int constituition);

	void setActionMax(unsigned int action);

	void setQuicknessMax(unsigned int quickness);

	void setStaminaMax(unsigned int stamina);

	void setMindMax(unsigned int mind);

	void setFocusMax(unsigned int focus);

	void setWillpowerMax(unsigned int willpower);

	void setArmor(unsigned int ar);

	void setHealthWounds(unsigned int wounds);

	void setActionWounds(unsigned int wounds);

	void setMindWounds(unsigned int wounds);

	void setShockWounds(unsigned int wounds);

	void setWoundsUpdateCounter(unsigned int count);

	void setSpeed(float spd);

	void setAcceleration(float acc);

	void setKinetic(float kin);

	void setEnergy(float ene);

	void setElectricity(float ele);

	void setStun(float stu);

	void setBlast(float bla);

	void setHeat(float hea);

	void setCold(float col);

	void setAcid(float aci);

	void setLightSaber(float lig);

	void changeConditionDamage(int amount);

	void setPosture(unsigned char state, bool overrideDizzy = false, bool objectInteraction = false, float objX = 0, float objY = 0, float objZ = 0);

	void setPvpStatusBitmask(unsigned int mask);

	void setDizziedState();

	void setStunnedState();

	void setBlindedState();

	void setIntimidatedState();

	void setPoisonedState(int str, int type, int duration);

	void setBleedingState(int str, int type, int duration);

	void setDiseasedState(int str, int type, int duration);

	void setOnFireState(int str, int type, int duration);

	bool setNextAttackDelay(int del);

	void setCharacterName(const string& name);

	void setCharacterName(const unicode& name);

	void setTerrainName(const string& name);

	void setCharacterAppearance(string& cust);

	void setAppearanceAttribute(unsigned char type, unsigned char value);

	void setAppearanceAttribute(string& type, unsigned char value);

	void updateCharacterAppearance();

	void setRaceName(const string& name);

	void setSpeciesName(const string& name);

	void setGender(const string& name);

	void setFaction(unsigned int fac);

	void setHeight(float h);

	void setMood(unsigned char mdid);

	void setMood(const string& m);

	void setGroup(GroupObject* Group);

	void setMount(MountCreature* mount);

protected:
	CreatureObject(DummyConstructorParameter* param);

	virtual ~CreatureObject();

	string _return_getGender;
	string _return_getGuildName;
	string _return_getMood;
	string _return_getPerformanceAnimation;
	string _return_getPerformanceName;
	string _return_getRaceName;
	string _return_getSkill;
	string _return_getSpeciesName;
	string _return_getStfName;
	string _return_getTerrainName;

	unicode _return_getCharacterName;

	friend class CreatureObjectHelper;
};

class CreatureObjectImplementation;

class CreatureObjectAdapter : public SceneObjectAdapter {
public:
	CreatureObjectAdapter(CreatureObjectImplementation* impl);

	Packet* invokeMethod(sys::uint32 methid, DistributedMethod* method);

	void info(const string& message, bool forcedLog);

	void sendItemsTo(Player* player);

	void sendTo(Player* player, bool doClose);

	void sendDestroyTo(Player* player);

	void doCombatAnimation(CreatureObject* defender, unsigned int animcrc, bool hit);

	void doAnimation(const string& anim);

	void playEffect(const string& file, const string& aux);

	void showFlyText(const string& file, const string& aux, unsigned char red, unsigned char green, unsigned char blue);

	void sendCombatSpam(CreatureObject* defender, TangibleObject* item, unsigned int damage, const string& skill, bool areaSpam);

	void addDamage(CreatureObject* creature, unsigned int damage);

	unsigned int getDamage(CreatureObject* creature);

	bool isLootOwner(CreatureObject* creature);

	unsigned int getMitigation(const string& mit);

	bool isIncapacitated();

	bool isDead();

	bool isKnockedDown();

	bool isKneeled();

	bool isProne();

	bool isInCombat();

	bool isDizzied();

	bool isStunned();

	bool isBlinded();

	bool isIntimidated();

	bool isDiseased();

	bool isPoisoned();

	bool isBleeding();

	bool isOnFire();

	bool isMounted();

	bool isRidingCreature();

	bool isMeditating();

	void updateKnockdownRecovery();

	void updateMood(const string& md);

	bool checkKnockdownRecovery();

	void updatePostureDownRecovery();

	void updatePostureUpRecovery();

	bool checkPostureDownRecovery();

	bool checkPostureUpRecovery();

	bool setState(unsigned long long state);

	bool clearState(unsigned long long state);

	void updateStates();

	void clearStates();

	float calculateBFRatio();

	void resetHAMBars();

	bool changeHAMBars(int hp, int ap, int mp, bool forcedChange);

	bool changeHAMWounds(int hpwnd, int apwnd, int mpwnd, bool forcedChange);

	void changeMaxHAMBars(int hp, int ap, int mp);

	bool takeHealthDamage(int damage);

	bool takeActionDamage(int damage);

	bool takeMindDamage(int damage);

	bool changeHealthWoundsBar(int wounds, bool forcedChange);

	bool changeActionWoundsBar(int wounds, bool forcedChange);

	bool changeMindWoundsBar(int wounds, bool forcedChange);

	bool changeHealthBar(int hp, bool forcedChange);

	bool changeStrengthBar(int st, bool forcedChange);

	bool changeConstitutionBar(int cs, bool forcedChange);

	void changeMaxHealthBar(int hp);

	void changeMaxStrengthBar(int hp);

	void changeMaxConstitutionBar(int hp);

	void changeShockWounds(int bf);

	bool changeActionBar(int ap, bool forcedChange);

	bool changeQuicknessBar(int qc, bool forcedChange);

	bool changeStaminaBar(int st, bool forcedChange);

	void changeMaxActionBar(int hp);

	void changeMaxQuicknessBar(int hp);

	void changeMaxStaminaBar(int hp);

	bool changeMindBar(int mp, bool forcedChange);

	bool changeFocusBar(int fc, bool forcedChange);

	bool changeWillpowerBar(int wl, bool forcedChange);

	void changeMaxMindBar(int hp);

	void changeMaxFocusBar(int hp);

	void changeMaxWillpowerBar(int hp);

	void setHealthBar(unsigned int hp);

	void setStrengthBar(unsigned int st);

	void setConstitutionBar(unsigned int cst);

	void setMaxHealthBar(unsigned int hp, bool updateClient);

	void setMaxStrengthBar(unsigned int st, bool updateClient);

	void setMaxConstitutionBar(unsigned int cst, bool updateClient);

	void setActionBar(unsigned int ap);

	void setQuicknessBar(unsigned int qck);

	void setStaminaBar(unsigned int sta);

	void setMaxActionBar(unsigned int ap, bool updateClient);

	void setMaxQuicknessBar(unsigned int qck, bool updateClient);

	void setMaxStaminaBar(unsigned int sta, bool updateClient);

	void setMindBar(unsigned int mp);

	void setFocusBar(unsigned int fc);

	void setWillpowerBar(unsigned int will);

	void setMaxMindBar(unsigned int mp, bool updateClient);

	void setMaxFocusBar(unsigned int fc, bool updateClient);

	void setMaxWillpowerBar(unsigned int will, bool updateClient);

	bool hasAttackDelay();

	void clearAttackDelay();

	void activateBurstRun();

	bool hasQueuedState(unsigned int skillCRC);

	bool removeQueuedState(unsigned int skillCRC);

	bool addQueuedState(unsigned int skillCRC);

	void setMeditateState();

	bool isGuilded();

	void removeGroup();

	bool isInAGroup();

	void setGroupID(unsigned long long gid);

	void updateGroupId(unsigned long long id);

	void updateGroupInviterId(unsigned long long id);

	void setDefenderID(long long did);

	void updateTarget(unsigned long long targ);

	void updateTarget(SceneObject* targ);

	void setActionCounter(unsigned int actioncntr);

	void setWeapon(Weapon* wep);

	void setCreatureLinkID(unsigned long long creatureID);

	void setAccuracy(int acc);

	void setDamageBonus(int bonus);

	void setDefenseBonus(int bonus);

	void setLastMovementUpdateStamp(unsigned int tme);

	void setIgnoreMovementTests(int times);

	void setLevel(unsigned int lvl);

	void updateServerMovementStamp();

	SceneObject* getInventoryItem(unsigned long long oid);

	void removeInventoryItem(unsigned long long oid);

	SceneObject* getLootItem(unsigned long long oid);

	void removeLootItem(unsigned long long oid);

	void broadcastMessage(BaseMessage* msg, int range, bool doLock);

	void broadcastMessage(StandaloneBaseMessage* msg, int range, bool doLock);

	Inventory* getInventory();

	Container* getLootContainer();

	unsigned long long getNewItemID();

	Weapon* getWeapon();

	Armor* getArmor(int type);

	int getSkillMod(const string& name);

	bool hasSkillMod(const string& name);

	int getSkillModBonus(const string& name);

	bool hasSkillModBonus(const string& name);

	int getAccuracy();

	int getDefenseBonus();

	int getDamageBonus();

	int getConditionDamage();

	int getMaxCondition();

	int getCondition();

	MountCreature* getMount();

	unsigned long long getLastServerMovementStamp();

	unsigned long long getCreatureLinkID();

	float getDistanceTo(CreatureObject* targetCreature);

	float getHeight();

	unsigned int getCreatureSkillsCount();

	unsigned int getNewCreatureSkillsCount(int cnt);

	unsigned int getLastMovementUpdateStamp();

	int getIgnoreMovementTests();

	unsigned int getLevel();

	int getCenteredBonus();

	void sendSystemMessage(const string& message);

	void sendSystemMessage(const string& file, const string& str, unsigned long long targetid);

	void addInventoryItem(TangibleObject* item);

	void addLootItem(TangibleObject* item);

	void startDancing(const string& anim, bool changeDance);

	void startPlayingMusic(const string& anim, bool changeDance);

	void startWatch(unsigned long long entid);

	void startListen(unsigned long long entid);

	void stopWatch(unsigned long long entid, bool doSendPackets, bool forced, bool doLock);

	void stopListen(unsigned long long entid, bool doSendPackets, bool forced, bool doLock);

	bool isPlayingMusic();

	bool isDancing();

	void stopDancing();

	void stopPlayingMusic();

	void addListener(CreatureObject* creature);

	void addWatcher(CreatureObject* creature);

	void removeListener(CreatureObject* creature);

	void removeWatcher(CreatureObject* creature);

	void doFlourish(const string& modifier);

	void doPerformanceAction();

	void doEntertainerPatronEffects(bool healShock, bool healWounds, bool addBuff);

	void addEntertainerFlourishBuff();

	void addEntertainerBuffDuration(int performanceType, float duration);

	void setEntertainerBuffDuration(int performanceType, float duration);

	float getEntertainerBuffDuration(int performanceType);

	void activateRecovery();

	int getCreatureSkillsSize();

	string& getSkill(int idx);

	bool hasSkill(unsigned int skillCRC);

	void mountCreature(MountCreature* mnt, bool lockMount);

	void dismount(bool lockMount, bool ignoreCooldown);

	void addCashCredits(unsigned int credits);

	void addBankCredits(unsigned int credits);

	void updateCashCredits(unsigned int credits);

	void updateBankCredits(unsigned int credits);

	void subtractCashCredits(unsigned int credits);

	void subtractBankCredits(unsigned int credits);

	void setCashCredits(int credits);

	void setBankCredits(int credits);

	void addBuff(int buffCRC, float duration);

	void applyBuff(BuffObject* buff);

	void removeBuff(const unsigned int buffCRC, bool remove);

	bool hasSpice();

	bool verifyCashCredits(unsigned int creditsToRemove);

	bool verifyBankCredits(unsigned int creditsToRemove);

	bool isOnFullHealth();

	bool hasStates();

	bool hasState(unsigned long long state);

	bool hasWounds();

	bool hasShockWounds();

	bool isAttackable();

	bool isAttackableBy(CreatureObject* attacker);

	bool isListening();

	bool isWatching();

	unicode& getCharacterName();

	string& getStfName();

	string& getSpeciesName();

	string& getRaceName();

	string& getGender();

	string& getTerrainName();

	void getCharacterAppearance(string& appearance);

	bool isOvert();

	unsigned long long getStateBitmask();

	unsigned int getCreatureBitmask();

	unsigned char getPosture();

	unsigned int getFaction();

	bool isRebel();

	bool isNeutral();

	bool isImperial();

	unsigned int getPvpStatusBitmask();

	unsigned int getBankCredits();

	unsigned int getCashCredits();

	unsigned int getWoundsUpdateCounter();

	unsigned int getNewWoundsUpdateCounter(int upd);

	unsigned int getBaseHealth();

	unsigned int getBaseStrength();

	unsigned int getBaseConstitution();

	unsigned int getBaseAction();

	unsigned int getBaseQuickness();

	unsigned int getBaseStamina();

	unsigned int getBaseMind();

	unsigned int getBaseFocus();

	unsigned int getBaseWillpower();

	unsigned int getHealth();

	unsigned int getStrength();

	unsigned int getConstitution();

	unsigned int getAction();

	unsigned int getQuickness();

	unsigned int getStamina();

	unsigned int getMind();

	unsigned int getFocus();

	unsigned int getWillpower();

	unsigned int getHealthMax();

	unsigned int getStrengthMax();

	unsigned int getConstitutionMax();

	unsigned int getActionMax();

	unsigned int getQuicknessMax();

	unsigned int getStaminaMax();

	unsigned int getMindMax();

	unsigned int getFocusMax();

	unsigned int getWillpowerMax();

	unsigned int getArmor();

	float getArmorResist(int resistType);

	unsigned int getHealthWounds();

	unsigned int getActionWounds();

	unsigned int getMindWounds();

	unsigned int getShockWounds();

	unsigned int getHAMUpdateCounter();

	unsigned int getNewHAMUpdateCounter(int cnt);

	unsigned int getHAMMaxUpdateCounter();

	unsigned int getNewHAMMaxUpdateCounter(int cnt);

	unsigned int getHealthEncumbrance();

	unsigned int getActionEncumbrance();

	unsigned int getMindEncumbrance();

	unsigned long long getTargetID();

	SceneObject* getTarget();

	unsigned long long getWeaponID();

	unsigned long long getGroupID();

	unsigned long long getGuildID();

	Guild* getGuild();

	float getSpeed();

	float getAcceleration();

	unsigned long long getDefenderID();

	unsigned int getDefenderUpdateCounter();

	unsigned int getNewDefenderUpdateCounter(int cnt);

	string& getPerformanceAnimation();

	unsigned int getPerformanceCounter();

	unsigned int getInstrumentID();

	string& getPerformanceName();

	string& getMood();

	unsigned char getMoodID();

	unsigned long long getGroupInviterID();

	unsigned long long getGroupInviteCounter();

	unsigned long long getNewGroupInviteCounter();

	unsigned long long getWatchID();

	unsigned long long getListenID();

	bool isInBuilding();

	int getBuildingType();

	string& getGuildName();

	int getWeaponSpeedModifier();

	unsigned int getSkillBoxesUpdateCounter();

	unsigned int getSkillModsCounter();

	unsigned int getNewSkillModsCounter(int cnt);

	void setBaseHealth(unsigned int health);

	void setBaseStrength(unsigned int strength);

	void setBaseConstitution(unsigned int constituition);

	void setBaseAction(unsigned int action);

	void setBaseQuickness(unsigned int quickness);

	void setBaseStamina(unsigned int stamina);

	void setBaseMind(unsigned int mind);

	void setBaseFocus(unsigned int focus);

	void setBaseWillpower(unsigned int willpower);

	void setHealth(unsigned int health);

	void setStrength(unsigned int strength);

	void setConstitution(unsigned int constituition);

	void setAction(unsigned int action);

	void setQuickness(unsigned int quickness);

	void setStamina(unsigned int stamina);

	void setMind(unsigned int mind);

	void setFocus(unsigned int focus);

	void setWillpower(unsigned int willpower);

	void setHealthMax(unsigned int health);

	void setStrengthMax(unsigned int strength);

	void setConstitutionMax(unsigned int constituition);

	void setActionMax(unsigned int action);

	void setQuicknessMax(unsigned int quickness);

	void setStaminaMax(unsigned int stamina);

	void setMindMax(unsigned int mind);

	void setFocusMax(unsigned int focus);

	void setWillpowerMax(unsigned int willpower);

	void setArmor(unsigned int ar);

	void setHealthWounds(unsigned int wounds);

	void setActionWounds(unsigned int wounds);

	void setMindWounds(unsigned int wounds);

	void setShockWounds(unsigned int wounds);

	void setWoundsUpdateCounter(unsigned int count);

	void setSpeed(float spd);

	void setAcceleration(float acc);

	void setKinetic(float kin);

	void setEnergy(float ene);

	void setElectricity(float ele);

	void setStun(float stu);

	void setBlast(float bla);

	void setHeat(float hea);

	void setCold(float col);

	void setAcid(float aci);

	void setLightSaber(float lig);

	void changeConditionDamage(int amount);

	void setPosture(unsigned char state, bool overrideDizzy, bool objectInteraction, float objX, float objY, float objZ);

	void setPvpStatusBitmask(unsigned int mask);

	void setDizziedState();

	void setStunnedState();

	void setBlindedState();

	void setIntimidatedState();

	void setPoisonedState(int str, int type, int duration);

	void setBleedingState(int str, int type, int duration);

	void setDiseasedState(int str, int type, int duration);

	void setOnFireState(int str, int type, int duration);

	bool setNextAttackDelay(int del);

	void setCharacterName(const string& name);

	void setCharacterName(const unicode& name);

	void setTerrainName(const string& name);

	void setCharacterAppearance(string& cust);

	void setAppearanceAttribute(unsigned char type, unsigned char value);

	void setAppearanceAttribute(string& type, unsigned char value);

	void updateCharacterAppearance();

	void setRaceName(const string& name);

	void setSpeciesName(const string& name);

	void setGender(const string& name);

	void setFaction(unsigned int fac);

	void setHeight(float h);

	void setMood(unsigned char mdid);

	void setMood(const string& m);

	void setGroup(GroupObject* Group);

	void setMount(MountCreature* mount);

protected:
	string _param0_info__string_bool_;
	string _param0_doAnimation__string_;
	string _param0_playEffect__string_string_;
	string _param1_playEffect__string_string_;
	string _param0_showFlyText__string_string_char_char_char_;
	string _param1_showFlyText__string_string_char_char_char_;
	string _param3_sendCombatSpam__CreatureObject_TangibleObject_int_string_bool_;
	string _param0_getMitigation__string_;
	string _param0_updateMood__string_;
	string _param0_getSkillMod__string_;
	string _param0_hasSkillMod__string_;
	string _param0_getSkillModBonus__string_;
	string _param0_hasSkillModBonus__string_;
	string _param0_sendSystemMessage__string_;
	string _param0_sendSystemMessage__string_string_long_;
	string _param1_sendSystemMessage__string_string_long_;
	string _param0_startDancing__string_bool_;
	string _param0_startPlayingMusic__string_bool_;
	string _param0_doFlourish__string_;
	string _param0_getCharacterAppearance__string_;
	string _param0_setCharacterName__string_;
	unicode _param0_setCharacterName__unicode_;
	string _param0_setTerrainName__string_;
	string _param0_setCharacterAppearance__string_;
	string _param0_setAppearanceAttribute__string_char_;
	string _param0_setRaceName__string_;
	string _param0_setSpeciesName__string_;
	string _param0_setGender__string_;
	string _param0_setMood__string_;
};

class CreatureObjectHelper : public DistributedObjectClassHelper, public Singleton<CreatureObjectHelper> {
	static CreatureObjectHelper* staticInitializer;

public:
	CreatureObjectHelper();

	void finalizeHelper();

	DistributedObject* instantiateObject();

	DistributedObjectAdapter* createAdapter(DistributedObjectStub* obj);

	friend class SingletonWrapper<CreatureObjectHelper>;
};

#include "../scene/SceneObjectImplementation.h"

class CreatureObjectServant : public SceneObjectImplementation {
public:
	CreatureObject* _this;

public:
	CreatureObjectServant(unsigned long long oid);
	virtual ~CreatureObjectServant();

	void _setStub(DistributedObjectStub* stub);
	DistributedObjectStub* _getStub();

};

#endif /*CREATUREOBJECT_H_*/
