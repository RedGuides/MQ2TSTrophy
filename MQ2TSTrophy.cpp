/**
	MQ2TSTrophy by Sic & CWTN; a RedGuides Exclusive
	Purpose: Equipping your Tradeskill Trophies when opening an environmental crafting container.
	Actions: This plugin will see if you have a trophy that matches the environmental container you have open,
		if it is not equipped, it will equip it. It does not swap back to anything.

**/
#include <mq/Plugin.h>

PreSetup("MQ2TSTrophy");
PLUGIN_VERSION(3.1);

const std::string pluginname = "\ar[\atMQ2TSTrophy\ar]\aw:: ";

bool WorldContainerCheck();
bool InGame();
void EquipTrophy(const char* pTrophy, const char* pSlot = "Ammo"); // Defaulted to ammo slot

std::string TrophyCheckByType(const std::string& trophytype);
bool IsTrophyEquipped(const std::string& trophy);
ItemClient* Cursor();

bool containerfound = false;
bool bMatches = false;
char szContainerName[32] = { 0 };
int iStep = 1;
int iPulse = 0;

const std::vector<int> vSlot = {
	eInventorySlot::InvSlot_Ammo,
	eInventorySlot::InvSlot_Range,
	eInventorySlot::InvSlot_Primary,
	eInventorySlot::InvSlot_Secondary
};

class MQ2TrophyType : public MQ2Type
{
public:
	enum TrophyMembers
	{
		Version,
		Matches,
		Alchemy,
		Baking,
		Brewing,
		Fletching,
		Jewelry,
		Tinkering,
		Pottery,
		Research,
		Blacksmithing,
		Tailoring,
		Poison,
		Fishing,
		Container,
	};

	MQ2TrophyType() : MQ2Type("Trophy") {
		TypeMember(Version);
		TypeMember(Matches);
		TypeMember(Alchemy);
		TypeMember(Baking);
		TypeMember(Brewing);
		TypeMember(Fletching);
		TypeMember(Jewelry);
		TypeMember(Tinkering);
		TypeMember(Pottery);
		TypeMember(Research);
		TypeMember(Blacksmithing);
		TypeMember(Tailoring);
		TypeMember(Poison);
		TypeMember(Fishing);
		TypeMember(Container);
	};

	~MQ2TrophyType()
	{
	}

	virtual bool GetMember(MQVarPtr VarPtr, const char* Member, char* Index, MQTypeVar& Dest) override
	{
		MQTypeMember* pMember = MQ2TrophyType::FindMember(Member);
		if (!pMember)
			return false;
		if (!pLocalPlayer)
			return false;
		switch ((TrophyMembers)pMember->ID)
		{
			case Version:
				Dest.Float = MQ2Version;
				Dest.Type = mq::datatypes::pFloatType;
				return true;
			case Matches: // this will return true/false if our equipped tradeskill trophy matches the world/enviro container we are in
				Dest.DWord = bMatches;
				Dest.Type = mq::datatypes::pBoolType;
				return true;
			case Alchemy:
				strcpy_s(DataTypeTemp, TrophyCheckByType("Alchemy").c_str());
				Dest.Ptr = DataTypeTemp;
				Dest.Type = mq::datatypes::pStringType;
				return true;
			case Baking:
				strcpy_s(DataTypeTemp, TrophyCheckByType("Baking").c_str());
				Dest.Ptr = DataTypeTemp;
				Dest.Type = mq::datatypes::pStringType;
				return true;
			case Brewing:
				strcpy_s(DataTypeTemp, TrophyCheckByType("Brewing").c_str());
				Dest.Ptr = DataTypeTemp;
				Dest.Type = mq::datatypes::pStringType;
				return true;
			case Fletching:
				strcpy_s(DataTypeTemp, TrophyCheckByType("Fletching").c_str());
				Dest.Ptr = DataTypeTemp;
				Dest.Type = mq::datatypes::pStringType;
				return true;
			case Jewelry:
				strcpy_s(DataTypeTemp, TrophyCheckByType("Jewelry").c_str());
				Dest.Ptr = DataTypeTemp;
				Dest.Type = mq::datatypes::pStringType;
				return true;
			case Tinkering:
				strcpy_s(DataTypeTemp, TrophyCheckByType("Tinkering").c_str());
				Dest.Ptr = DataTypeTemp;
				Dest.Type = mq::datatypes::pStringType;
				return true;
			case Pottery:
				strcpy_s(DataTypeTemp, TrophyCheckByType("Pottery").c_str());
				Dest.Ptr = DataTypeTemp;
				Dest.Type = mq::datatypes::pStringType;
				return true;
			case Research:
				strcpy_s(DataTypeTemp, TrophyCheckByType("Research").c_str());
				Dest.Ptr = DataTypeTemp;
				Dest.Type = mq::datatypes::pStringType;
				return true;
			case Blacksmithing:
				strcpy_s(DataTypeTemp, TrophyCheckByType("Blacksmithing").c_str());
				Dest.Ptr = DataTypeTemp;
				Dest.Type = mq::datatypes::pStringType;
				return true;
			case Tailoring:
				strcpy_s(DataTypeTemp, TrophyCheckByType("Tailoring").c_str());
				Dest.Ptr = DataTypeTemp;
				Dest.Type = mq::datatypes::pStringType;
				return true;
			case Poison:
				strcpy_s(DataTypeTemp, TrophyCheckByType("Poison").c_str());
				Dest.Ptr = DataTypeTemp;
				Dest.Type = mq::datatypes::pStringType;
				return true;
			case Fishing:
				strcpy_s(DataTypeTemp, TrophyCheckByType("Fishing").c_str());
				Dest.Ptr = DataTypeTemp;
				Dest.Type = mq::datatypes::pStringType;
				return true;
			case Container: // this returns true/false if we see a world/enviro tradeskill container
				Dest.DWord = containerfound;
				Dest.Type = mq::datatypes::pBoolType;
				return true;
			default:
				break;
		}
		return false;
	}

	bool ToString(MQVarPtr VarPtr, const char* Destination)
	{
		return true;
	}
};

MQ2TrophyType* pTrophyType = nullptr;

bool TrophyData(const char* szIndex, MQTypeVar& Dest)
{
	Dest.DWord = 1;
	Dest.Type = pTrophyType;
	return true;
}

PLUGIN_API void InitializePlugin()
{
	DebugSpewAlways("Initializing MQ2TSTrophy");
	AddMQ2Data("Trophy", TrophyData);
	pTrophyType = new MQ2TrophyType;
}

PLUGIN_API void ShutdownPlugin()
{
	DebugSpewAlways("Shutting down MQ2TSTrophy");
	RemoveMQ2Data("Trophy");
	delete pTrophyType;
}

PLUGIN_API int OnIncomingChat(const char* pLine, unsigned long Color)
{
	return 0;
}

PLUGIN_API void OnPulse()
{
	int iPulseDelay = 20;
	if (!InGame()) return;

	if (++iPulse < iPulseDelay) return;

	iPulse = 0;

	WorldContainerCheck();

	if (iStep == 3) {
		if (ItemOnCursor()) {
			EzCommand("/autoinv");
		}
		iStep = 4;
	}

	if (containerfound) {
		std::string type;

		if (strstr(szContainerName, "Alchemy Table")) {
			type = "Alchemy";
		}
		else if (strstr(szContainerName, "Mixing Bowl") || strstr(szContainerName, "Oven") || strstr(szContainerName, "Ice Cream")) {
			type = "Baking";
		}
		else if (strstr(szContainerName, "Brewing Barrel")) {
			type = "Brewing";
		}
		else if (strstr(szContainerName, "Fletching Table")) {
			type = "Fletching";
		}
		else if (strstr(szContainerName, "Jewelry Making Table")) {
			type = "Jewelry";
		}
		else if (strstr(szContainerName, "Fly Making Bench")) {
			type = "Fishing";
		}
		else if (strstr(szContainerName, "Kiln") || strstr(szContainerName, "Pottery Wheel")) {
			type = "Pottery";
		}
		else if (strstr(szContainerName, "Poisoncrafting Table")) {
			type = "Poison";
		}
		else if (strstr(szContainerName, "Forge")) {
			type = "Blacksmithing";
		}
		else if (strstr(szContainerName, "Spell Research Table")) {
			type = "Research";
		}
		else if (strstr(szContainerName, "Loom")) {
			type = "Tailoring";
		}
		else if (strstr(szContainerName, "Tinkering")) {
			type = "Tinkering";
		}
		else {
			containerfound = false;
		}

		if (!type.empty()) {
			auto needtrophy = TrophyCheckByType(type);
			if (!needtrophy.empty() && !IsTrophyEquipped(needtrophy)) {
				bMatches = false;
				if (type == "Fishing" || (type == "Tailoring" && string_equals(needtrophy, "Blessed Akhevan Shadow Shears"))) {
					// Fishing is the only item that requires mainhand
					// Blessed Ahkevan Shadow Shears requires pri or secondary
					EquipTrophy(needtrophy.c_str(), "Mainhand");
				}
				else
					EquipTrophy(needtrophy.c_str());
			}
			else {
				bMatches = true;
			}
		}

		if (iStep == 4) {
			iStep = 1;
		}
	}
	else {
		if (iStep > 1)
			iStep = 1;
	}
}

PLUGIN_API void OnZoned()
{
	if (!InGame()) return;
	iStep = 1;
}

// This checks any "world containers" like a ground spawn crafting bench/table
bool WorldContainerCheck()
{
	if (CContainerMgr* pWnd = pContainerMgr) {
		ItemClient* thiscontaineritem = pWnd->pWorldContainer;
		if (thiscontaineritem && thiscontaineritem->Open == 1) {
			if (ItemDefinition* worldContainer = GetItemFromContents(thiscontaineritem)) {
				strcpy_s(szContainerName, worldContainer->Name);
				containerfound = true;
			}
		}
		else {
			containerfound = false;
		}
	}
	return containerfound;
}

void EquipTrophy(const char* pTrophy, const char* pSlot)
{
	for (const auto& i : vSlot) {
		if (pTrophy) {
			char szBuffer[128] = { 0 };
			if (iStep == 1) {
				WriteChatf("%s\ayPicking up: \ap%s", pluginname.c_str(), pTrophy);
				sprintf_s(szBuffer, "/squelch /nomodkey /shiftkey /itemnotify \"%s\" leftmouseup", pTrophy);
				EzCommand(szBuffer);
				iStep = 2;
			}
			else if (iStep == 2) {
				if (ItemClient* pCursor = Cursor()) {
					if (!_stricmp(pCursor->GetName(), pTrophy)) {
						WriteChatf("%s\aySwapping: \ap%s\aw into slot: \ay%s", pluginname.c_str(), pTrophy, pSlot);
						sprintf_s(szBuffer, "/squelch /nomodkey /shiftkey /itemnotify %s leftmouseup", pSlot);
						EzCommand(szBuffer);
						iStep = 3;
					}
				}
			}
		}
	}
}

bool IsTrophyEquipped(const std::string& trophy)
{
	bool equipped = false;
	for (const auto& i : vSlot) {
		if (ItemClient* pItem = FindItemBySlot(i)) {
			if (pItem->GetItemDefinition()->Name == trophy) {
				equipped = true;
			}
		}
	}
	return equipped;
}

ItemClient* Cursor()
{
	return GetPcProfile()->GetInventorySlot(InvSlot_Cursor);
}

inline bool InGame()
{
	return (GetGameState() == GAMESTATE_INGAME && GetCharInfo() && GetCharInfo()->pSpawn && GetPcProfile());
}

std::map <const std::string, std::vector<std::string> > mTrophies = {
	{ "Alchemy", {
		"Mundunugu Medicine Stick", "Master Alchemist Trophy", "Expert Alchemist Trophy", "Journeyman Alchemist Trophy",
		"Freshman Alchemist Trophy", "Apprentice Alchemist Trophy", "Beginner Alchemist Trophy"
		}
	},

	{ "Baking", {
		"Denmother's Rolling Pin", "Master Baker Trophy", "Expert Baker Trophy", "Journeyman Baker Trophy",
		"Freshman Baker Trophy", "Apprentice Baker Trophy", "Beginner Baker Trophy"
		}
	},

	{ "Brewing", {
		"Brewmaster's Mug", "Master Brewer Trophy", "Expert Brewer Trophy", "Journeyman Brewer Trophy",
		"Freshman Brewer Trophy", "Apprentice Brewer Trophy", "Beginner Brewer Trophy"
		}
	},

	{ "Fletching", {
		"Fletcher's Arrow", "Master Fletcher Trophy", "Expert Fletcher Trophy", "Journeyman Fletcher Trophy",
		"Freshman Fletcher Trophy", "Apprentice Fletcher Trophy", "Beginner Fletcher Trophy"
		}
	},

	{ "Jewelry", {
		"Intricate Jewelers Glass", "Master Jeweler Trophy", "Expert Jeweler Trophy", "Journeyman Jeweler Trophy",
		"Freshman Jeweler Trophy", "Apprentice Jeweler Trophy", "Beginner Jeweler Trophy"
		}
	},

	{ "Tinkering", {
		"Hovering Contraption", "Master Mechanist Trophy", "Expert Mechanist Trophy", "Journeyman Mechanist Trophy",
		"Freshman Mechanist Trophy", "Apprentice Mechanist Trophy", "Beginner Mechanist Trophy"
		}
	},

	{ "Pottery", {
		"Clay Flinger's Loop", "Master Potter Trophy", "Expert Potter Trophy", "Journeyman Potter Trophy", "Freshman Potter Trophy",
		"Apprentice Potter Trophy", "Beginner Potter Trophy"
		}
	},

	{ "Research", {
		"Ethereal Quill", "Master Researcher Trophy", "Expert Researcher Trophy", "Journeyman Researcher Trophy",
		"Freshman Researcher Trophy", "Apprentice Researcher Trophy", "Beginner Researcher Trophy"
		}
	},

	{ "Blacksmithing", {
		"Blacksmith's Adamantine Hammer", "Master Smith Trophy", "Expert Smith Trophy", "Journeyman Smith Trophy",
		"Freshman Smith Trophy", "Apprentice Smith Trophy", "Beginner Smith Trophy"
		}
	},

	{ "Poison", {
		"Peerless Pestle", "Master Toxicologist Trophy", "Expert Toxicologist Trophy", "Journeyman Toxicologist Trophy",
		"Freshman Toxicologist Trophy", "Apprentice Toxicologist Trophy", "Beginner Toxicologist Trophy"
		}
	},

	{ "Tailoring", {
		"Blessed Akhevan Shadow Shears", "Mystical Bolt", "Master Tailor Trophy", "Expert Tailor Trophy", "Journeyman Tailor Trophy",
		"Freshman Tailor Trophy", "Apprentice Tailor Trophy", "Beginner Tailor Trophy"
		}
	},

	{ "Fishing", {
		"The Bone Rod", "Collapsible Fishing Pole"
		}
	}
};

std::string TrophyCheckByType(const std::string& trophytype) {
	for (auto& trophyname : mTrophies.at(trophytype)) {
		if (FindItemByName(&trophyname[0], 1)) {
			return trophyname;
		}
	}
	return {};
}