#include <stddef.h>
#include "Framework.h"

using namespace RE::BSScript;
using namespace SKSE;
using namespace SKSE::log;
using namespace SKSE::stl;

namespace {
    void InitializeLogging(const SKSE::PluginDeclaration* plugin) {
#ifndef NDEBUG
        auto sink = std::make_shared<spdlog::sinks::msvc_sink_mt>();
#else
        auto path = logger::log_directory();
        if (!path) {
            report_and_fail("Unable to lookup SKSE logs directory.");
        }

        *path /= fmt::format(FMT_STRING("{}.log"), plugin->GetName());
        auto sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(path->string(), true);
#endif

#ifndef NDEBUG
        const auto level = spdlog::level::trace;
#else
        const auto level = spdlog::level::info;
#endif

		auto log = std::make_shared<spdlog::logger>("global log"s, std::move(sink));
        log->set_level(level);
        log->flush_on(level);

        spdlog::set_default_logger(std::move(log));
        spdlog::set_pattern("[%l] %v"s);
		/*spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%n] [%l] [%t] [%s:%#] %v");*/
    }

	//void SaveAdditions(SKSE::SerializationInterface* a_intfc) {
	//	if (!a_intfc->OpenRecord('ADD_', 1)) {
	//		logger::error("Failed to open record for spell additions.");
	//	} else {
	//		auto&       additions = Loki::DynamicAnimationCasting::_additions;
	//		std::size_t size = additions.size();
	//		if (!a_intfc->WriteRecordData(size)) {
	//			logger::error("Failed to write size for spell additions.");
	//		} else {
	//			for (const auto& elem : additions) {
	//				if (!a_intfc->WriteRecordData(elem.first.first)) {
	//					logger::error("Failed to write toml string for spell addition.");
	//					break;
	//				}
	//				if (!a_intfc->WriteRecordData(elem.first.second)) {
	//					logger::error("Failed to write event index for spell addition.");
	//					break;
	//				}
	//				if (!a_intfc->WriteRecordData(elem.second->size())) {
	//					logger::error("Failed to write size of spells.");
	//					break;
	//				}
	//				for (const auto& spell : *(elem.second)) {
	//					std::int32_t formId = spell->formID;
	//					if (!a_intfc->WriteRecordData(formId)) {
	//						logger::error("Failed to write form id for spell.");
	//						return;
	//					}
	//				}
	//			}
	//		}
	//	}
	//}

	//void LoadAdditions(SKSE::SerializationInterface* a_intfc) {
	//	std::vector<float> _data;

	//	std::uint32_t type;
	//	std::uint32_t version;
	//	std::uint32_t length;

	//	while (a_intfc->GetNextRecordInfo(type, version, length)) {
	//		switch (type) {
	//		case 'ADD_':
	//			{
	//				std::size_t size;
	//				if (!a_intfc->ReadRecordData(size)) {
	//					logger::error("Failed to load size of spell additions.");
	//				}
	//				for (std::uint32_t i = 0; i < size; i++) {
	//					std::string filePath;
	//					std::size_t eventIdx;
	//					std::size_t numSpells;
	//					std::vector<RE::SpellItem*> spells;
	//					if (!a_intfc->ReadRecordData(filePath)) {
	//						logger::error("Failed to load file path.");
	//					} else {
	//						if (!a_intfc->ReadRecordData(eventIdx)) {
	//							logger::error("Failed to load event index.");
	//						} else {
	//							if (!a_intfc->ReadRecordData(numSpells)) {
	//								logger::error("Failed to load size of spells.");
	//							} else {
	//								auto pairKey = std::pair<std::string, std::size_t>(filePath, eventIdx);
	//								for (std::uint32_t ix = 0; ix < numSpells; ix++) {
	//									std::int32_t oldFormId = 0;
	//									RE::FormID   newFormId;
	//									if (!a_intfc->ReadRecordData(oldFormId)) {
	//										logger::error("Failed to load spell form id.");
	//									} else {
	//										if (!a_intfc->ResolveFormID(oldFormId, newFormId)) {
	//											logger::error("Failed to resolve form id.");
	//										} else {
	//											auto spell = static_cast<RE::SpellItem*>(RE::TESForm::LookupByID(newFormId));
	//											spells.push_back(spell);
	//										}
	//									}
	//								}
	//								Loki::DynamicAnimationCasting::_additions[pairKey]->clear();
	//								Loki::DynamicAnimationCasting::_additions[pairKey] = &spells;
	//							}
	//						}
	//					}
	//				}
	//			}
	//		}
	//	}
	//	Loki::DynamicAnimationCasting::LoadAdditions();
	//}

	void SaveSwaps(SKSE::SerializationInterface* a_intfc) {

	}

	void SaveSpellData(SKSE::SerializationInterface* a_intfc)
	{
		//SaveAdditions(a_intfc);
	}

    //void InitializeSerialization() {
    //    log::trace("Initializing cosave serialization...");
    //    auto* serde = GetSerializationInterface();
    //    serde->SetUniqueID(_byteswap_ulong('SMPL'));
    //    serde->SetSaveCallback(Sample::HitCounterManager::OnGameSaved);
    //    serde->SetRevertCallback(Sample::HitCounterManager::OnRevert);
    //    serde->SetLoadCallback(Sample::HitCounterManager::OnGameLoaded);
    //    log::trace("Cosave serialization initialized.");
    //}

	//bool ReplaceSpells(RE::StaticFunctionTag*, RE::BSFixedString a_filePath, std::uint32_t a_eventIdx, std::vector<RE::SpellItem*> a_newSpells) {
	//	Loki::DynamicAnimationCasting::ReplaceSpells(std::string(a_filePath.c_str()), a_eventIdx, &a_newSpells);
	//}

	//bool SwapSpell(RE::StaticFunctionTag*, RE::BSFixedString a_filePath, std::uint32_t a_eventIdx, RE::SpellItem* a_originalSpell, RE::SpellItem* a_newSpell) {
	//	Loki::DynamicAnimationCasting::SwapSpell(std::string(a_filePath.c_str()), a_eventIdx, a_originalSpell, a_newSpell);
	//}

 //   bool InitializePapyrus(RE::BSScript::IVirtualMachine* a_vm) {
 //       log::trace("Initializing Papyrus binding...");
	//	a_vm->RegisterFunction("ReplaceSpells", "DynamicAnimationCasting", ReplaceSpells);
	//	a_vm->RegisterFunction("SwapSpell", "DynamicAnimationCasting", SwapSpell);
	//	log::trace("Papyrus binding finished.");
	//	return true;
 //   }

    void InitializeHooking() {
        //log::trace("Initializing trampoline...");
        //auto& trampoline = GetTrampoline();
        //trampoline.create(64);
        //log::trace("Trampoline initialized.");

        Loki::DynamicAnimationCasting::LoadTomls();

        Loki::DynamicAnimationCasting::InstallGraphEventSink();
    }

    void InitializeMessaging() {
        if (!GetMessagingInterface()->RegisterListener([](MessagingInterface::Message* message) {
                switch (message->type) {
                    // Skyrim lifecycle events.
                    case MessagingInterface::kPostLoad:  // Called after all plugins have finished running
                                                         // SKSEPlugin_Load. It is now safe to do multithreaded
                                                         // operations, or operations against other plugins.
                    case MessagingInterface::kPostPostLoad:  // Called after all kPostLoad message handlers have run.
                    case MessagingInterface::kInputLoaded:   // Called when all game data has been found.
                        break;
                    case MessagingInterface::kDataLoaded:  // All ESM/ESL/ESP plugins have loaded, main menu is now
                                                           // active.
                        // It is now safe to access form data.
                        InitializeHooking();
                        break;

                    // Skyrim game events.
                    case MessagingInterface::kNewGame:      // Player starts a new game from main menu.
                    case MessagingInterface::kPreLoadGame:  // Player selected a game to load, but it hasn't loaded yet.
                                                            // Data will be the name of the loaded save.
                    case MessagingInterface::kPostLoadGame:  // Player's selected save game has finished loading.
                                                             // Data will be a boolean indicating whether the load was
                                                             // successful.
                    case MessagingInterface::kSaveGame:  // The player has saved a game.
                                                         // Data will be the save name.
                    case MessagingInterface::kDeleteGame:  // The player deleted a saved game from within the load menu.
                        break;
                }
            })) {
            stl::report_and_fail("Unable to register message listener.");
        }
    }
}

extern "C" [[maybe_unused]] DLLEXPORT constinit auto SKSEPlugin_Version = []() {
    SKSE::PluginVersionData v;
    v.PluginVersion(Plugin::VERSION);
    v.PluginName(Plugin::NAME);

	v.UsesAddressLibrary(true);
    //v.CompatibleVersions({ SKSE::RUNTIME_SSE_LATEST_AE, SKSE::RUNTIME_LATEST_VR});
    v.HasNoStructUse();

    return v;
}();

extern "C" [[maybe_unused]] DLLEXPORT bool SKSEAPI SKSEPlugin_Query(const SKSE::QueryInterface*, SKSE::PluginInfo * pluginInfo) {
    pluginInfo->name = SKSEPlugin_Version.pluginName;
    pluginInfo->infoVersion = SKSE::PluginInfo::kVersion;
    pluginInfo->version = SKSEPlugin_Version.pluginVersion;
    return true;
}

extern "C" [[maybe_unused]] DLLEXPORT bool SKSEAPI SKSEPlugin_Load(const SKSE::LoadInterface* skse) {
    auto* plugin = PluginDeclaration::GetSingleton();
    auto version = plugin->GetVersion();

	InitializeLogging(plugin);
    log::info("{} {} is loading...", plugin->GetName(), version);

    SKSE::Init(skse);

    InitializeMessaging();
    // InitializeSerialization();
	//const auto papyrus = SKSE::GetPapyrusInterface();
	//papyrus->Register(InitializePapyrus);

    log::info("{} has finished loading.", plugin->GetName());
    return true;
}
