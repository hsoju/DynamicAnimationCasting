#pragma once

namespace Loki
{
	class Serialization {

	public:

		static void SaveAdditions(SKSE::SerializationInterface* a_intfc);
		static bool SetUUID(SKSE::SerializationInterface* a_intfc, const std::string& a_uuid, std::size_t c_size);
		static bool SetSpells(SKSE::SerializationInterface* a_intfc, std::vector<RE::SpellItem*>* a_spells);

		static void LoadAdditions(SKSE::SerializationInterface* a_intfc);
		static std::string GetUUID(SKSE::SerializationInterface* a_intfc, std::size_t c_size);
		static std::vector<RE::SpellItem*> GetSpells(SKSE::SerializationInterface* a_intfc);
	};
}
