#pragma once
#include "DynamicAnimationCasting.h"

namespace Loki {

    class DynamicAnimationCasting {
    
    public:

        static void ReadToml(std::filesystem::path path);
		static void LoadTomls();

        static void InstallGraphEventSink();

        static RE::BSEventNotifyControl ProcessEvent(RE::BSTEventSink<RE::BSAnimationGraphEvent>* a_this,
                                                     RE::BSAnimationGraphEvent& a_event,
                                                     RE::BSTEventSource<RE::BSAnimationGraphEvent>* a_src);

        static inline REL::Relocation<decltype(ProcessEvent)> _PCProcessEvent;
        static inline REL::Relocation<decltype(ProcessEvent)> _NPCProcessEvent;

        static RE::BSEventNotifyControl PCProcessEvent(RE::BSTEventSink<RE::BSAnimationGraphEvent>* a_this,
                                                       RE::BSAnimationGraphEvent& a_event,
                                                       RE::BSTEventSource<RE::BSAnimationGraphEvent>* a_src);

        static RE::BSEventNotifyControl NPCProcessEvent(RE::BSTEventSink<RE::BSAnimationGraphEvent>* a_this,
                                                        RE::BSAnimationGraphEvent& a_event,
                                                        RE::BSTEventSource<RE::BSAnimationGraphEvent>* a_src);

        static inline bool _TomlLoaded = false;
        static inline std::vector<std::pair<RE::BSFixedString, Loki::AnimationCasting::Cast>> _eventVector = {};

		static inline std::unordered_map<std::pair<std::string, size_t>, Loki::AnimationCasting::Cast*> _casters = {};
		static void ReplaceSpells(const std::string& a_filePath, std::uint32_t a_eventIdx, std::vector<RE::SpellItem*>* a_newSpells);
		static void SwapSpell(const std::string& a_filepath, std::uint32_t a_eventIdx, RE::SpellItem* a_originalSpell, RE::SpellItem* a_newSpell);
		//static inline std::unordered_map<std::pair<std::string_view, std::uint32_t&>&, std::vector<RE::SpellItem*>&> papyrusMapping = {};
		//static void AddSpells(std::string_view a_filepath, std::uint32_t &a_spellIdx, std::vector<RE::SpellItem*> &a_spells);
    };

};
