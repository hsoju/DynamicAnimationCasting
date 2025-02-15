#include <toml++/toml.h>
#include "Framework.h"

void Loki::DynamicAnimationCasting::ReadToml(std::filesystem::path path) {
	std::string strPath = path.string();
	logger::info("Reading {}...", strPath);
    try {
        const auto tbl = toml::parse_file(path.c_str());
        auto& arr = *tbl.get_as<toml::array>("event");

		//size_t idx = 0;
        for (auto&& elem : arr) {
            auto& eventTable = *elem.as_table();

            logger::info("- - - - - - - - - - - - - - - - - - - - -");

            auto event = eventTable["AnimationEvent"].value<std::string>();
            logger::info("Animation Event -> {}", *event);

            auto raceFormID = eventTable["HasRaceFormID"].value<std::int32_t>();
            logger::info("Race Form ID -> {0:#x}", *raceFormID);

            auto raceEspName = eventTable["RaceEspName"].value<std::string>();
            logger::info("Race ESP name -> {}", *raceEspName);
            std::pair racePair = {*raceFormID, *raceEspName};

            auto actorFormID = eventTable["HasActorFormID"].value<std::int32_t>();
            logger::info("Actor Form ID -> {0:#x}", *actorFormID);
            
            auto actorEspName = eventTable["ActorEspName"].value<std::string>();
            logger::info("Actor ESP name -> {}", *actorEspName);
            std::pair<RE::FormID, std::string> actorPair = {*actorFormID, *actorEspName};

            auto weapFormID = eventTable["IsEquippedRightFormID"].value<std::int32_t>();
            logger::info("Right Form ID -> {0:#x}", *weapFormID);
            
            auto weapFormID2 = eventTable["IsEquippedLeftFormID"].value<std::int32_t>();
            logger::info("Left Form ID -> {0:#x}", *weapFormID2);
            
            auto weapEspName = eventTable["WeaponEspName"].value<std::string>();
            logger::info("Weapon ESP name -> {}", *weapEspName);
            std::pair<RE::FormID, RE::FormID> pair = {*weapFormID, *weapFormID2};
            std::pair<std::string, std::pair<RE::FormID, RE::FormID>> weapPair = {*weapEspName, pair};

            auto weapontype = eventTable["HasWeaponType"].value<std::int32_t>();
            logger::info("Weapon Type -> {}", *weapontype);
            auto weapType = *weapontype;

            auto effectFormID = eventTable["HasEffectFormID"].value<std::int32_t>();
            logger::info("Effect Form ID -> {0:#x}", *effectFormID);
            
            auto effectEspName = eventTable["EffectEspName"].value<std::string>();
            logger::info("Effect ESP name -> {}", *effectEspName);
            std::pair<RE::FormID, std::string> effectPair = {*effectFormID, *effectEspName};

            auto keywordFormID = eventTable["HasKeywordFormID"].value<std::int32_t>();
            logger::info("Keyword Form ID -> {0:#x}", *keywordFormID);
            
            auto keywordEspName = eventTable["KeywordEspName"].value<std::string>();
            logger::info("Keyword ESP name -> {}", *keywordEspName);
            std::pair<std::int32_t, std::string> keywordPair = {*keywordFormID, *keywordEspName};

			std::unordered_map<std::string, std::vector<std::int32_t>> map = {};
			bool spellsHaveUUID = eventTable["SpellFormIDs"].is_string();
			std::string spellUUID;
			if (!spellsHaveUUID) {
				auto spells = eventTable["SpellFormIDs"].as_array();
				if (spells) {
					std::vector<std::string> espVector;
					bool multipleEsps = eventTable["SpellEspName"].is_array();
					if (!multipleEsps) {
						auto spellEspName = eventTable["SpellEspName"].value<std::string>();
						espVector.push_back(*spellEspName);
						logger::info("Spell ESP name -> {}", *spellEspName);
					} else {
						auto spellEspNames = eventTable["SpellEspName"].as_array();
						for (auto& spellEspName : *spellEspNames) {
							espVector.push_back(*spellEspName.value<std::string>());
							logger::info("Spell ESP name -> {}", *spellEspName.value<std::string>());
						}
					}
					size_t numSpellEsps = espVector.size();
					size_t spellIdx = 0;
					for (auto& spell : *spells) {
						logger::info("Spell Form ID -> {0:#x}", *spell.value<std::int32_t>());
						const std::string& spellEspName = espVector[spellIdx];
						if (!map.contains(spellEspName)) {
							map[spellEspName] = std::vector<std::int32_t>();
						}
						map[spellEspName].push_back(*spell.value<std::int32_t>());
						if (spellIdx < (numSpellEsps - 1)) {
							spellIdx += 1;
						}
					}
				}
			} else {
				auto spell = eventTable["SpellFormIDs"].value<std::string>();
				logger::info("Spell uuid -> {}", *spell);
				spellUUID = std::move(*spell);
			}

			auto targetPlayer = eventTable["TargetCaster"].value<bool>();
            logger::info("Target Caster -> {}", *targetPlayer);

			std::optional<float> dupeTimer;
			if (eventTable.contains("DuplicateTimer")) {
				dupeTimer = eventTable["DuplicateTimer"].value<float>();
			} else {
				dupeTimer = 0.00f;
			}
            logger::info("Duplicate Timer -> {}", *dupeTimer);

            auto healthCost = eventTable["HealthCost"].value<float>();
            logger::info("Health Cost -> {}", *healthCost);
            
            auto staminaCost = eventTable["StaminaCost"].value<float>();
            logger::info("Stamina Cost -> {}", *staminaCost);
            
            auto magickaCost = eventTable["MagickaCost"].value<float>();
            logger::info("Magicka Cost -> {}", *magickaCost);

			std::optional<bool> effectiveCost;
			if (eventTable.contains("UseEffectiveMagickaCost")) {
				effectiveCost = eventTable["UseEffectiveMagickaCost"].value<bool>();
			} else {
				effectiveCost = false;
			}
			logger::info("Use Effective Magicka Cost ->{}", *effectiveCost);

            //AnimationCasting::Cast* cast =
            //    new AnimationCasting::Cast(map, racePair, actorPair, weapPair, weapType, effectPair, keywordPair,
            //                               *targetPlayer, *healthCost, *staminaCost, *magickaCost);

            //std::pair<std::string, AnimationCasting::Cast> pair2 = {*event, cast};

            _eventVector.emplace_back(std::piecewise_construct, std::tuple{*event},
                                      std::tuple{std::move(map), racePair, actorPair, weapPair, weapType, effectPair, keywordPair,
                                                 *targetPlayer, *dupeTimer, *healthCost, *staminaCost, *magickaCost, *effectiveCost});
			if (spellsHaveUUID) {
				if (!_additions.contains(spellUUID)) {
					_additions[spellUUID] = std::vector<Loki::AnimationCasting::Cast*>();
				}
				_additions[spellUUID].push_back(&_eventVector.back().second);
			}
			//Loki::AnimationCasting::Cast* caster = &_eventVector.back().second;
        }
        logger::info("Successfully read {}...", path.string());

    } catch (const toml::parse_error& e) {
        logger::error("Error parsing file '{}':\t{}\n\t\t({}:{})", *e.source().path, e.description(),
                      e.source().begin.line, e.source().begin.column);
    } catch (const std::exception& e) {
        logger::error("{}", e.what());
    } catch (...) {
        logger::error("Unknown failure");
    }
}

void Loki::DynamicAnimationCasting::LoadTomls() {
    constexpr auto path = L"Data/SKSE/Plugins/_DynamicAnimationCasting";
    constexpr auto ext = L".toml";

    if (_TomlLoaded) 
        return;

    _TomlLoaded = true;

    auto dataHandle = RE::TESDataHandler::GetSingleton();

    logger::info("Reading all .tomls in file...");

    if (std::filesystem::is_directory(path)) {
        for (const auto& file : std::filesystem::directory_iterator(path)) {
            if (std::filesystem::is_regular_file(file) && file.path().extension() == ext) {
                ReadToml(file.path());
            }
        }
    }

    logger::info("Successfully read all .tomls in file.");
}

void Loki::DynamicAnimationCasting::ReplaceSpells(const std::string& a_uuid, const std::vector<RE::SpellItem*>& a_newSpells) {
	for (auto& caster : _additions[a_uuid]) {
		caster->replacements = std::move(a_newSpells);
		_additionsCache[a_uuid] = &(caster->replacements);
	}
}
//
//void Loki::DynamicAnimationCasting::SwapSpell(const std::string &a_filePath, std::uint32_t a_eventIdx, RE::SpellItem* a_originalSpell, RE::SpellItem* a_newSpell)
//{
//	auto pairKey = std::pair<std::string, size_t>(a_filePath, a_eventIdx);
//	auto& caster = _casters[pairKey];
//	bool alreadyAssigned = false;
//	if (caster->reassignments == nullptr) {
//		std::vector<std::pair<RE::SpellItem*, RE::SpellItem*>> spellVector;
//		caster->reassignments = &spellVector;
//	} else {
//		for (auto& elem : *(caster->reassignments)) {
//			if (elem.second == a_originalSpell) {
//				elem.second = a_newSpell;
//				alreadyAssigned = true;
//				break;
//			}
//		}
//	}
//	if (!alreadyAssigned) {
//		caster->reassignments->push_back(std::pair<RE::SpellItem*, RE::SpellItem*>(a_originalSpell, a_newSpell));
//	}
//}

void Loki::DynamicAnimationCasting::InstallGraphEventSink() {
    logger::info("Injecting Graph Event Sink Hooks");
    // Install hook on Character/PlayerCharacter's BSTEventSink<RE::BSAnimationGraphEvent> base class
    REL::Relocation<uintptr_t> PCProcessAnimGraphEventVtbl{RE::VTABLE_PlayerCharacter[2]};
    REL::Relocation<uintptr_t> NPCProcessAnimGraphEventVtbl{RE::VTABLE_Character[2]};
    _PCProcessEvent = PCProcessAnimGraphEventVtbl.write_vfunc(0x1, &PCProcessEvent);
    _NPCProcessEvent = NPCProcessAnimGraphEventVtbl.write_vfunc(0x1, &NPCProcessEvent);
}

RE::BSEventNotifyControl Loki::DynamicAnimationCasting::ProcessEvent(RE::BSTEventSink<RE::BSAnimationGraphEvent>* a_this, RE::BSAnimationGraphEvent& a_event, RE::BSTEventSource<RE::BSAnimationGraphEvent>* a_src) {

    if (a_event.tag != NULL && a_event.holder != NULL) {
        auto actorPtr = a_event.holder->As<RE::Actor>();

        for (auto& [tag, caster] : DynamicAnimationCasting::_eventVector) {
            if (a_event.tag == (RE::BSFixedString)tag) {
                logger::info("Event Found: {}", a_event.tag);
                caster.CastSpells(actorPtr);
            }
        }
    }
    return RE::BSEventNotifyControl::kContinue;
}

RE::BSEventNotifyControl Loki::DynamicAnimationCasting::PCProcessEvent(
    RE::BSTEventSink<RE::BSAnimationGraphEvent>* a_this, RE::BSAnimationGraphEvent& a_event,
    RE::BSTEventSource<RE::BSAnimationGraphEvent>* a_src) {
    ProcessEvent(a_this, a_event, a_src);
    return _PCProcessEvent(a_this, a_event, a_src);
}

RE::BSEventNotifyControl Loki::DynamicAnimationCasting::NPCProcessEvent(
    RE::BSTEventSink<RE::BSAnimationGraphEvent>* a_this, RE::BSAnimationGraphEvent& a_event,
    RE::BSTEventSource<RE::BSAnimationGraphEvent>* a_src) {
    ProcessEvent(a_this, a_event, a_src);
    return _NPCProcessEvent(a_this, a_event, a_src);
}
