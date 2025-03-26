local lastPromoMessage = "" -- Для предотвращения спама

-- Функция для обработки ввода промокода
function CustomPromoFrame_OnActivate()
    local inputText = CustomPromoFrameInputBox:GetText()
    if inputText and inputText ~= "" then
        SendChatMessage(".promo " .. inputText, "SAY")
        CustomPromoFrameInputBox:SetText("") -- Очищаем поле
    end
end

-- Обработчик чата для фильтрации ответов
local function PromoChatFilter(self, event, msg, author, ...)
    if string.find(msg, "PROMO") then -- Фильтр по ключевому слову
        if msg ~= lastPromoMessage then
            CustomPromoFrame.Responsse:SetText(msg)
            CustomPromoFrame.Responsse:Show() -- Показываем при новом сообщении
            lastPromoMessage = msg
        end
        return true -- Блокируем вывод в чат
    end
end

-- Подключаем фильтр сообщений
ChatFrame_AddMessageEventFilter("CHAT_MSG_SYSTEM", PromoChatFilter)

-- Команда для открытия фрейма
SLASH_PROMO1 = "/promo"
SlashCmdList["PROMO"] = function()
    if CustomPromoFrame:IsShown() then
        CustomPromoFrame:Hide()
    else
        CustomPromoFrame:Show()
        CustomPromoFrame.Responsse:Hide() -- Скрываем Response при открытии фрейма
    end
end
