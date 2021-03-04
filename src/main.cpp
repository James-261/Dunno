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

MAKE_HOOK_OFFSETLESS(Player_Start, void, Il2CppObject* self)
{
    INFO("Player Start Hook Called!");

    Il2CppString* csName = CRASH_UNLESS(il2cpp_utils::GetFieldValue<Il2CppString*>(self, "savedName"));
    std::string name = to_utf8(csstrtostr(csName));

    getLogger().info("Name was: %s", name);
    Player_Start(self);
}

MAKE_HOOK_OFFSETLESS(Player_Update, void, Il2CppObject* self)
{
    CRASH_UNLESS(il2cpp_utils::SetFieldValue<Il2CppString*>(self, "savedName", il2cpp_utils::createcsstr("newName")));    } 
    Player_Update(self);
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

    INSTALL_HOOK_OFFSETLESS(logger, Player_Start, il2cpp_utils::FindMethodUnsafe("GorillaComputer", "Player", "Awake", 0));
    INSTALL_HOOK_OFFSETLESS(logger, Player_Update, il2cpp_utils::FindMethodUnsafe("GorillaComputer", "Player", "Update", 0));

    INFO("Installed hooks!");
}