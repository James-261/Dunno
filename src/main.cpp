#include "modloader/shared/modloader.hpp"
#include "beatsaber-hook/shared/utils/logging.hpp"
#include <string>

#include "beatsaber-hook/shared/utils/typedefs.h"
#include "beatsaber-hook/shared/utils/il2cpp-utils.hpp"


ModInfo modInfo;

#define INFO(value...) getLogger().info(value)
#define ERROR(value...) getLogger().error(value)

Logger& getLogger()
{
    static Logger* logger = new Logger(modInfo, LoggerOptions(false, true));
    return *logger;
}

MAKE_HOOK_OFFSETLESS(Player_Awake, void, Il2CppObject* self)
{
    INFO("Player Awake Hook Called!");

    Il2CppString* csName = CRASH_UNLESS(il2cpp_utils::GetFieldValue<Il2CppString*>(self, "savedName"));
    std::string name = to_utf8(csstrtostr(csName));

    getLogger().info("Name was: %s", name);
    CRASH_UNLESS(il2cpp_utils::SetFieldValue(self, "savedName", il2cpp_utils::createcsstr("newName"))); 
    Player_Awake(self);
}

extern "C" void setup(ModInfo& info)
{
    info.id = ID;
    info.version = VERSION;
    modInfo = info;
    INFO("Hellowo World from setup!");
}

extern "C" void load()
{
    INFO("Hello World From Load!");
 
    Logger& logger = getLogger();
    il2cpp_functions::Init();
    INFO("Installing hooks...");

    INSTALL_HOOK_OFFSETLESS(logger, Player_Awake, il2cpp_utils::FindMethodUnsafe("GorillaComputer", "Player", "Awake", 0));

    INFO("Installed hooks!");
}
