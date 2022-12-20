#include "Serialization.h"
#include "Framework.h"

bool Loki::Serialization::SetUUID(SKSE::SerializationInterface* a_intfc, const std::string& a_uuid, std::size_t c_size)
{
	const char* u_data = a_uuid.c_str();
	std::size_t u_dataLength = strlen(u_data);
	if (!a_intfc->WriteRecordData(u_dataLength)) {
		logger::error("Failed to write length of uuid.");
	} else {
		if (!a_intfc->WriteRecordData(u_data, u_dataLength * c_size)) {
			logger::error("Failed to write uuid.");
		} else {
			//logger::info("Successfully wrote {}", *u_data);
			return true;
		}
	}
	return false;
}

bool Loki::Serialization::SetSpells(SKSE::SerializationInterface* a_intfc, std::vector<RE::SpellItem*>* a_spells)
{
	bool   success = true;
	size_t a_numSpells = a_spells->size();
	if (!a_intfc->WriteRecordData(a_numSpells)) {
		logger::error("Failed to write number of spells.");
		success = false;
	} else {
		for (RE::SpellItem* spell : *a_spells) {
			std::int32_t formId = spell->GetFormID();
			if (!a_intfc->WriteRecordData(formId)) {
				logger::error("Failed to write spell form id.");
				success = false;
				break;
			}
		}
	}
	return success;
}

void Loki::Serialization::SaveAdditions(SKSE::SerializationInterface* a_intfc) {
	if (!a_intfc->OpenRecord('ADD_', 1)) {
		logger::error("Failed to open record for uuid's.");
	} else {
		std::size_t a_numUUIDs = Loki::DynamicAnimationCasting::_additions.size();
		if (!a_intfc->WriteRecordData(a_numUUIDs)) {
			logger::error("Failed to write number of uuid's.");
		} else {
			const std::size_t c_size = sizeof(char);
			for (auto&& elem : Loki::DynamicAnimationCasting::_additionsCache) {
				if (!SetUUID(a_intfc, elem.first, c_size)) {
					break;
				} else {
					if (!SetSpells(a_intfc, elem.second)) {
						break;
					}
				}
			}
		}
	}
}

std::string Loki::Serialization::GetUUID(SKSE::SerializationInterface* a_intfc, std::size_t c_size)
{
	std::size_t a_UUIDLength;
	if (!a_intfc->ReadRecordData(a_UUIDLength)) {
		logger::error("Failed to load length of uuid.");
	} else {
		char* a_UUID = new char[a_UUIDLength];
		if (!a_intfc->ReadRecordData(a_UUID, a_UUIDLength * c_size)) {
			logger::error("Failed to load uuid.");
		} else {
			a_UUID[a_UUIDLength] = '\0';
			std::string str_UUID = std::string(a_UUID);
			delete[] a_UUID;
			return str_UUID;
		}
	}
	return std::string();
}

std::vector<RE::SpellItem*> Loki::Serialization::GetSpells(SKSE::SerializationInterface* a_intfc)
{
	std::vector<RE::SpellItem*> spells;
	std::size_t                 a_numSpells;
	if (!a_intfc->ReadRecordData(a_numSpells)) {
		logger::error("Failed to load length of spells.");
	} else {
		std::int32_t oldFormId;
		RE::FormID   newFormId;
		for (size_t i = 0; i < a_numSpells; i++) {
			if (!a_intfc->ReadRecordData(oldFormId)) {
				logger::error("Failed to load spell form id.");
			} else {
				if (!a_intfc->ResolveFormID(oldFormId, newFormId)) {
					logger::error("Unable to resolve old form id.");
				} else {
					spells.push_back(static_cast<RE::SpellItem*>(RE::TESForm::LookupByID(newFormId)));
				}
			}
		}
	}
	return spells;
}

void Loki::Serialization::LoadAdditions(SKSE::SerializationInterface* a_intfc)
{
	std::uint32_t type;
	std::uint32_t version;
	std::uint32_t length;
	while (a_intfc->GetNextRecordInfo(type, version, length)) {
		switch (type) {
		case 'ADD_':
			std::size_t a_numUUIDs;
			if (!a_intfc->ReadRecordData(a_numUUIDs)) {
				logger::error("Failed to load number of uuid's.");
			} else {
				const std::size_t c_size = sizeof(char);
				for (std::uint32_t ix = 0; ix < a_numUUIDs; ix++) {
					const std::string&                 a_uuid = GetUUID(a_intfc, c_size);
					const std::vector<RE::SpellItem*>& a_spells = GetSpells(a_intfc);
					if (a_uuid.size() > 0 && a_spells.size() > 0) {
						Loki::DynamicAnimationCasting::ReplaceSpells(a_uuid, a_spells);
					}
				}
			}
			break;
		default:
			logger::error("Unrecognized signature type!");
			break;
		}
	}
}
