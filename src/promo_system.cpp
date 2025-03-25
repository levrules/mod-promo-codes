#include "promo_system.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "Chat.h"
#include "DatabaseEnv.h"
#include "CommandScript.h"
#include "StringFormat.h"

using namespace Acore::ChatCommands;

class promo_system_command : public CommandScript
{
public:
    promo_system_command() : CommandScript("promo_system_command") {}

    ChatCommandTable GetCommands() const override
    {
        static ChatCommandTable commandTable =
        {
            { "promo", HandlePromoCodeCommand,  SEC_PLAYER, Console::No  },
        };
        return commandTable;
    }

    static bool HandlePromoCodeCommand(ChatHandler* handler, Optional<Variant<ObjectGuid::LowType, QuotedString>> const& promoCodeOpt)
    {
        Player* player = handler->GetPlayer();
        if (!player)
            return false;

        if (!promoCodeOpt)
        {
            handler->PSendSysMessage(MSG_PROMO_COMMAND);
            return false;
        }

        std::string promoCode = std::string(std::get<QuotedString>(*promoCodeOpt));

        QueryResult result = CharacterDatabase.Query(
            "SELECT `id`, `type`, `value`, `amount`, `count`, `uniqueCharacters`, `uniqueAccount`, `start_time`, `end_time` "
            "FROM `promo` WHERE `code` = '{}' AND `enable` = 1 AND `count` > 0 "
            "AND NOW() BETWEEN `start_time` AND `end_time`", promoCode);

        if (!result)
        {
            handler->PSendSysMessage(MSG_PROMO_FAILD, promoCode);
            return false;
        }

        Field* fields = result->Fetch();
        uint32 promoId = fields[0].Get<uint32>();
        uint32 promoType = fields[1].Get<uint32>();
        uint32 promoValue = fields[2].Get<uint32>();
        uint32 promoAmount = fields[3].Get<uint32>();
        uint32 uniqueCharacters = fields[5].Get<uint32>();
        uint32 uniqueAccount = fields[6].Get<uint32>();
        std::string startTime = fields[7].Get<std::string>();
        std::string endTime = fields[8].Get<std::string>();

        if (uniqueCharacters > 0)
        {
            QueryResult checkCharacter = CharacterDatabase.Query(
                "SELECT COUNT(*) FROM promo_reward WHERE guid = {} AND code = '{}'", player->GetGUID().GetCounter(), promoCode);
            if (checkCharacter && checkCharacter->Fetch()[0].Get<uint32>() >= uniqueCharacters)
            {
                handler->PSendSysMessage(MSG_PROMO_ALREADY_USED);
                return false;
            }
        }

        if (uniqueAccount > 0)
        {
            QueryResult checkAcc = CharacterDatabase.Query(
                "SELECT COUNT(*) FROM promo_reward WHERE account = {} AND code = '{}'", player->GetSession()->GetAccountId(), promoCode);
            if (checkAcc && checkAcc->Fetch()[0].Get<uint32>() >= uniqueAccount)
            {
                handler->PSendSysMessage(MSG_PROMO_ACCOUNT_USED);
                return false;
            }
        }

        switch (promoType)
        {
        case 0:
            player->AddItem(promoValue, promoAmount);
            break;
        case 1:
            player->learnSpell(promoValue, false);
            break;
        case 2:
            player->ModifyMoney(promoValue * 10000);
            break;
        case 3:
            player->ModifyHonorPoints(promoValue);
            break;
        case 4:
            player->ModifyArenaPoints(promoValue);
            break;
        case 5:
            player->SetAtLoginFlag(AT_LOGIN_CHANGE_FACTION);
            break;
        case 6:
            player->SetAtLoginFlag(AT_LOGIN_RENAME);
            break;
        case 7:
            player->SetAtLoginFlag(AT_LOGIN_CHANGE_RACE);
            break;
        default:
            handler->PSendSysMessage(MSG_PROMO_FAILD2, promoType);
            return false;
        }

        CharacterDatabase.Execute("INSERT INTO promo_reward (guid, account, code, type, value, amount, count, reward_time) VALUES ({}, {}, '{}', {}, {}, {}, 1, NOW())", 
            player->GetGUID().GetCounter(), player->GetSession()->GetAccountId(), promoCode, promoType, promoValue, promoAmount);

        CharacterDatabase.Execute("UPDATE promo SET count = count - 1 WHERE id = {}", promoId);

        handler->PSendSysMessage(MSG_PROMO_SUCCESS);
        return true;
    }
};

void AddPromoCodesScripts()
{
    new promo_system_command();
}
