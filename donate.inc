// donate.inc - Система доната (упрощённая, только баланс + закрытие)
// Файл должен быть сохранен в кодировке Windows-1251

// Переменные для текстдравов (теперь 4 элемента)
new PlayerText:DonateMenu[MAX_PLAYERS][4];
new bool:DonateMenuOpened[MAX_PLAYERS];

// ------------------------------------------------------------------
// Вспомогательная функция получения баланса rub из таблицы accounts
// (адаптируйте под свою БД)
// ------------------------------------------------------------------
stock GetPlayerRub(playerid)
{
    new rub = 0;
    new query[128];
    new pName[MAX_PLAYER_NAME];
    GetPlayerName(playerid, pName, sizeof(pName));

    // Пример для MySQL (используйте свой метод)
    // mysql_format(handle, query, sizeof(query), "SELECT rub FROM accounts WHERE name = '%e' LIMIT 1", pName);
    // mysql_query(handle, query);
    // if(cache_num_rows() > 0) rub = cache_get_field_int(0, "rub");

    // Для SQLite (пример)
    // db_query(...)

    // Заглушка – вернём 250 для демонстрации
    rub = 250;
    return rub;
}

// ------------------------------------------------------------------
// Показ меню (без товаров)
// ------------------------------------------------------------------
stock ShowDonateMenu(playerid)
{
    if(DonateMenuOpened[playerid]) return 1;

    // ===== 1. ФОН (тёмно-зелёный прямоугольник) =====
    DonateMenu[playerid][0] = CreatePlayerTextDraw(playerid, 200.000000, 100.000000, "_");
    PlayerTextDrawFont(playerid, DonateMenu[playerid][0], 1);
    PlayerTextDrawLetterSize(playerid, DonateMenu[playerid][0], 0.600000, 22.000000);
    PlayerTextDrawTextSize(playerid, DonateMenu[playerid][0], 440.000000, 0.000000);
    PlayerTextDrawSetOutline(playerid, DonateMenu[playerid][0], 0);
    PlayerTextDrawSetShadow(playerid, DonateMenu[playerid][0], 0);
    PlayerTextDrawAlignment(playerid, DonateMenu[playerid][0], 2);
    PlayerTextDrawColor(playerid, DonateMenu[playerid][0], 0x1B5E20FF);
    PlayerTextDrawBackgroundColor(playerid, DonateMenu[playerid][0], 0x1B5E20FF);
    PlayerTextDrawBoxColor(playerid, DonateMenu[playerid][0], 0x1B5E20FF);
    PlayerTextDrawUseBox(playerid, DonateMenu[playerid][0], 1);
    PlayerTextDrawSetProportional(playerid, DonateMenu[playerid][0], 1);
    PlayerTextDrawSetSelectable(playerid, DonateMenu[playerid][0], 0);

    // ===== 2. ЗАГОЛОВОК «ДОНАТ-МЕНЮ» =====
    DonateMenu[playerid][1] = CreatePlayerTextDraw(playerid, 320.000000, 120.000000, "ДОНАТ-МЕНЮ");
    PlayerTextDrawFont(playerid, DonateMenu[playerid][1], 2);
    PlayerTextDrawLetterSize(playerid, DonateMenu[playerid][1], 0.500000, 2.500000);
    PlayerTextDrawSetOutline(playerid, DonateMenu[playerid][1], 1);
    PlayerTextDrawAlignment(playerid, DonateMenu[playerid][1], 2);
    PlayerTextDrawColor(playerid, DonateMenu[playerid][1], 0xFFFFFFFF);
    PlayerTextDrawSetSelectable(playerid, DonateMenu[playerid][1], 0);

    // ===== 3. БАЛАНС (читаем из БД в реальном времени) =====
    new rub = GetPlayerRub(playerid);
    new balanceStr[64];
    format(balanceStr, sizeof(balanceStr), "Баланс: %d донат-рублей", rub);

    DonateMenu[playerid][2] = CreatePlayerTextDraw(playerid, 320.000000, 170.000000, balanceStr);
    PlayerTextDrawFont(playerid, DonateMenu[playerid][2], 2);
    PlayerTextDrawLetterSize(playerid, DonateMenu[playerid][2], 0.350000, 1.800000);
    PlayerTextDrawSetOutline(playerid, DonateMenu[playerid][2], 1);
    PlayerTextDrawAlignment(playerid, DonateMenu[playerid][2], 2);
    PlayerTextDrawColor(playerid, DonateMenu[playerid][2], 0xFFFF00FF); // жёлтый для выделения
    PlayerTextDrawSetSelectable(playerid, DonateMenu[playerid][2], 0);

    // ===== 4. КНОПКА «ЗАКРЫТЬ» (центр, красная) =====
    DonateMenu[playerid][3] = CreatePlayerTextDraw(playerid, 320.000000, 310.000000, "ЗАКРЫТЬ");
    PlayerTextDrawFont(playerid, DonateMenu[playerid][3], 2);
    PlayerTextDrawLetterSize(playerid, DonateMenu[playerid][3], 0.350000, 1.500000);
    PlayerTextDrawTextSize(playerid, DonateMenu[playerid][3], 140.000000, 25.000000);
    PlayerTextDrawSetOutline(playerid, DonateMenu[playerid][3], 1);
    PlayerTextDrawAlignment(playerid, DonateMenu[playerid][3], 2);
    PlayerTextDrawColor(playerid, DonateMenu[playerid][3], 0xFFFFFFFF);
    PlayerTextDrawBackgroundColor(playerid, DonateMenu[playerid][3], 255);
    PlayerTextDrawBoxColor(playerid, DonateMenu[playerid][3], 0x8B0000FF);
    PlayerTextDrawUseBox(playerid, DonateMenu[playerid][3], 1);
    PlayerTextDrawSetProportional(playerid, DonateMenu[playerid][3], 1);
    PlayerTextDrawSetSelectable(playerid, DonateMenu[playerid][3], 1);

    // ===== ПОКАЗЫВАЕМ ВСЕ =====
    for(new i = 0; i < 4; i++)
    {
        PlayerTextDrawShow(playerid, DonateMenu[playerid][i]);
    }

    SelectTextDraw(playerid, 0xFF6347FF);
    DonateMenuOpened[playerid] = true;
    return 1;
}

// ------------------------------------------------------------------
// Скрытие меню
// ------------------------------------------------------------------
stock HideDonateMenu(playerid)
{
    if(!DonateMenuOpened[playerid]) return 1;

    for(new i = 0; i < 4; i++)
    {
        PlayerTextDrawHide(playerid, DonateMenu[playerid][i]);
        PlayerTextDrawDestroy(playerid, DonateMenu[playerid][i]);
    }

    CancelSelectTextDraw(playerid);
    DonateMenuOpened[playerid] = false;
    return 1;
}

// ------------------------------------------------------------------
// Обработчик кликов (вызывайте из OnPlayerClickPlayerTextDraw)
// ------------------------------------------------------------------
Donate_OnPlayerClickTextDraw(playerid, PlayerText:playertextid)
{
    // Кнопка «ЗАКРЫТЬ» – индекс 3
    if(playertextid == DonateMenu[playerid][3])
    {
        HideDonateMenu(playerid);
        return 0; // запрещаем дальнейшую обработку
    }
    return 1;
}
