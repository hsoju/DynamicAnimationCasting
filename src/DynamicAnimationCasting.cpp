#include "DynamicAnimationCasting.h"

void Loki::HUD::FlashHUDMeter(RE::ActorValue a_av) {
    static REL::Relocation<decltype(FlashHUDMeter)> FlashHUDMenuMeter{RELOCATION_ID(51907, 52845)};
    return FlashHUDMenuMeter(a_av);
}

bool Loki::AnimationCasting::Cast::CastSpell(RE::SpellItem* a_spell, RE::Actor* a_actor) {
	if (a_spell) {
		float totalCost = 0.0f;
		if (_properties.effectiveCost) {
			totalCost = a_spell->CalculateMagickaCost(a_actor);
		}
		if (!_properties.effectiveCost || totalCost <= a_actor->AsActorValueOwner()->GetActorValue(RE::ActorValue::kMagicka)) {
			logger::info("Passed all conditional checks, subtracting costs and casting spells now...");
			if (_properties.effectiveCost) {
				a_actor->AsActorValueOwner()->RestoreActorValue(
					RE::ACTOR_VALUE_MODIFIER::kDamage,
					RE::ActorValue::kMagicka, totalCost * -1.00f);
			}

			logger::info("Casting Spell ' {} ' now", a_spell->GetFullName());
			a_actor->GetMagicCaster(RE::MagicSystem::CastingSource::kInstant)
				->CastSpellImmediate(
					a_spell,                                      // spell
					false,                                       // noHitEffectArt
					_properties.targetCaster ? a_actor : nullptr,// target
					1.00f,                                       // effectiveness
					false,                                       // hostileEffectivenessOnly
					0.0f,                                        // magnitude override
					_properties.targetCaster ? nullptr : a_actor // cause
				);
			return true;
		} else {
			logger::info("Failed effective magicka cost check");
			HUD::FlashHUDMeter(RE::ActorValue::kMagicka);
			return false;
		}
	} else {
		logger::info("Failed casting, spell not found");
		return false;
	}
}

void Loki::AnimationCasting::Cast::CastSpells(const RE::Actor* a_actor) {

	RE::Actor* actor = (RE::Actor*)a_actor;

    bool failed = false;
    if (actor->AsActorValueOwner()->GetActorValue(RE::ActorValue::kStamina) < _properties.staminaCost) {
        logger::info("Failed kStamina check"); failed = true;
        HUD::FlashHUDMeter(RE::ActorValue::kStamina);
    }
    if (actor->AsActorValueOwner()->GetActorValue(RE::ActorValue::kMagicka) < _properties.magickaCost) {
        logger::info("Failed kMagicka check"); failed = true;
        HUD::FlashHUDMeter(RE::ActorValue::kMagicka);
    }
    if (failed) { return; }

    auto HasEffect = [actor](RE::FormID a_id) -> bool {

        if (auto activeEffect = actor->AsMagicTarget()->GetActiveEffectList()) {

            bool hasIt = false;
            if (auto dhandle = RE::TESDataHandler::GetSingleton(); dhandle) {
                for (auto& ae : *activeEffect) {

                    if (!ae) {
                        break;
                    }
                    if (!ae->effect) {
                        continue;
                    }
                    if (!ae->effect->baseEffect) {
                        continue;
                    }
                    if (ae->effect->baseEffect->formID == a_id) {
                        hasIt = true;
                    }

                }
                return hasIt;
            }

        }
        return false;
    };

    RE::FormID r_id = 0;
    RE::FormID l_id = 0;
    RE::WEAPON_TYPE r_type = {};
    RE::WEAPON_TYPE l_type = {};
    if (auto r_weapon = a_actor->GetEquippedObject(false)) {
        r_id = r_weapon->formID;
        if (auto wep = r_weapon->As<RE::TESObjectWEAP>()) {
            r_type = wep->weaponData.animationType.get();
        }
    }
    if (auto l_weapon = a_actor->GetEquippedObject(true)) {
        l_id = l_weapon->formID;
        if (auto wep = l_weapon->As<RE::TESObjectWEAP>()) {
            l_type = wep->weaponData.animationType.get();
        }
    }

    if (auto handle = RE::TESDataHandler::GetSingleton(); handle) {

        auto vrace = handle->LookupForm<RE::TESRace>(_properties.racePair.first, _properties.racePair.second);
        if (_properties.racePair.first == -1 || _properties.racePair.first == 0 ||
            (vrace && vrace->formID == actor->GetActorBase()->race->formID)) {
        
            auto vactor = handle->LookupForm<RE::Actor>(_properties.actorPair.first, _properties.actorPair.second);
            if (_properties.actorPair.first == -1 || _properties.actorPair.first == 0 || (vactor && vactor->formID == actor->formID)) {

                auto vRWeapon = handle->LookupForm<RE::TESObjectWEAP>(_properties.weapPair.second.first, _properties.weapPair.first);
                if (_properties.weapPair.second.first == -1 || _properties.weapPair.second.first == 0 || (vRWeapon && vRWeapon->formID == r_id)) {
                
                    auto vLWeapon = handle->LookupForm<RE::TESObjectWEAP>(_properties.weapPair.second.second, _properties.weapPair.first);
                    if (_properties.weapPair.second.second == -1 || _properties.weapPair.second.second == -1 || (vLWeapon && vLWeapon->formID == l_id)) {

                        if (_properties.weapType == -1 || _properties.weapType == 0 || 
                            (RE::WEAPON_TYPE)_properties.weapType == r_type || (RE::WEAPON_TYPE)_properties.weapType == l_type) {
                        
                            auto veffect = handle->LookupForm<RE::EffectSetting>(_properties.effectPair.first, _properties.effectPair.second);
                            if (_properties.effectPair.first == -1 || _properties.effectPair.first == 0 || (veffect && HasEffect(veffect->formID))) {

                                auto vkeyword = handle->LookupForm<RE::BGSKeyword>(_properties.keywordPair.first, _properties.keywordPair.second);
                                if (_properties.keywordPair.first == -1 || _properties.keywordPair.first == 0 || (vkeyword && actor->HasKeyword(vkeyword))) {
                                        
                                    auto currentTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

                                    if (_properties.dupeTimer <= 0.00f || previousTime == 0 || ((_properties.dupeTimer * 1000) < (currentTime - previousTime))) {
										
                                        previousTime = currentTime;

										actor->AsActorValueOwner()->RestoreActorValue(RE::ACTOR_VALUE_MODIFIER::kDamage,
											RE::ActorValue::kHealth,
											_properties.healthCost * -1.00f);
										actor->AsActorValueOwner()->RestoreActorValue(RE::ACTOR_VALUE_MODIFIER::kDamage,
											RE::ActorValue::kStamina,
											_properties.staminaCost * -1.00f);
										actor->AsActorValueOwner()->RestoreActorValue(RE::ACTOR_VALUE_MODIFIER::kDamage,
											RE::ActorValue::kMagicka,
											_properties.magickaCost * -1.00f);

										if (replacements.size() < 1) {
											for (auto spell : _properties.spells) {
												for (auto it = spell.second.begin(); it < spell.second.end(); ++it) {
													auto single = handle->LookupForm<RE::SpellItem>((RE::FormID)*it, spell.first.c_str());
													bool success = CastSpell(single, actor);
													if (!success) {
														return;
													}
												}
											}
										} else {
											for (auto spell : replacements) {
												bool success = CastSpell(spell, actor);
												if (!success) {
													return;
												}
											}
										}
                                        logger::info("... Finished casting spells.");
                                    }
                           
                                } else { logger::info("keyword check failed"); 
									}

                            } else { logger::info("effect check failed"); 
								}
                        
                        } else { logger::info("weapon type check failed"); 
							}
                    
                    } else { logger::info("Left weapon check failed"); 
						}
                
                } else { logger::info("Right weapon check failed"); 
					}
            
            } else { logger::info("actor check failed"); 
				}

        } else { logger::info("race check failed"); 
			}
    }
    else { logger::info("Error: invalid data handler"); 
		};

}
