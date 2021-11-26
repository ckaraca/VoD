<?php
/* $Id: czech-utf-8.inc.php,v 1.160 2003/09/29 12:06:17 nijel Exp $ */

/**
 * Czech language file by
 *   Michal Čihař <nijel at users.sourceforge.net>
 */

$charset = 'utf-8';
$allow_recoding = TRUE;
$text_dir = 'ltr';
$left_font_family = 'verdana, arial, helvetica, geneva, sans-serif';
$right_font_family = 'tahoma, arial, helvetica, geneva, sans-serif';
$number_thousands_separator = ' ';
$number_decimal_separator = '.';
// shortcuts for Byte, Kilo, Mega, Giga, Tera, Peta, Exa
$byteUnits = array('bajtů', 'kB', 'MB', 'GB', 'TB', 'PB', 'EB');

$day_of_week = array('Neděle', 'Pondělí', 'Úterý', 'Středa', 'Čtvrtek', 'Pátek', 'Sobota');
$month = array('ledna', 'února', 'března', 'dubna', 'května', 'června', 'července', 'srpna', 'září', 'října', 'listopadu', 'prosince');
// See http://www.php.net/manual/en/function.strftime.php to define the
// variable below
$datefmt = '%a %d. %b %Y, %H:%M';

$timespanfmt = '%s dnů, %s hodin, %s minut a %s sekund';

$strAPrimaryKey = 'V&nbsp;tabulce %s byl vytvořen primární klíč';
$strAbortedClients = 'Přerušené';
$strAbsolutePathToDocSqlDir = 'Zadejte absolutní cestu docSQL adresáře na serveru';
$strAccessDenied = 'Přístup odepřen';
$strAccessDeniedExplanation = 'phpMyAdmin se pokusil připojit k&nbsp;MySQL serveru, a ten odmítl připojení. Zkontrolujte jméno serveru, uživatelské jméno a heslo v&nbsp;souboru config.inc.php a ujistěte se, že jsou totožné s&nbsp;těmi co máte od administrátora MySQL serveru.';
$strAction = 'Akce';
$strAddAutoIncrement = 'Přidat hodnotu AUTO_INCREMENT';
$strAddDeleteColumn = 'Přidat/Smazat sloupec';
$strAddDeleteRow = 'Přidat/Smazat řádek s&nbsp;podmínkou';
$strAddDropDatabase = 'Přidat DROP DATABASE';
$strAddIntoComments = 'Přidat do komentářů';
$strAddNewField = 'Přidat nový sloupec';
$strAddPriv = 'Přidat nové privilegium';
$strAddPrivMessage = 'Oprávnění bylo přidáno.';
$strAddPrivilegesOnDb = 'Přidat oprávnění pro databázi';
$strAddPrivilegesOnTbl = 'Přidat oprávnění pro tabulku';
$strAddSearchConditions = 'Přidat vyhledávací parametry (obsah dotazu po příkazu "WHERE"):';
$strAddToIndex = 'Přidat do indexu  &nbsp;%s&nbsp;sloupců';
$strAddUser = 'Přidat nového uživatele';
$strAddUserMessage = 'Uživatel byl přidán.';
$strAddedColumnComment = 'Přidán komentář ke sloupci';
$strAddedColumnRelation = 'Přidána relace pro sloupec';
$strAdministration = 'Správa';
$strAffectedRows = 'Ovlivněné řádky:';
$strAfter = 'Po %s';
$strAfterInsertBack = 'Zpět';
$strAfterInsertNewInsert = 'Vložit další řádek';
$strAll = 'Všechno';
$strAllTableSameWidth = 'Zobrazit všechny tabulky stejnou šířkou';
$strAlterOrderBy = 'Změnit pořadí tabulky podle';
$strAnIndex = 'K&nbsp;tabulce %s byl přidán index';
$strAnalyzeTable = 'Analyzovat tabulku';
$strAnd = 'a';
$strAny = 'Jakýkoliv';
$strAnyColumn = 'Jakýkoliv sloupec';
$strAnyDatabase = 'Jakákoliv databáze';
$strAnyHost = 'Jakýkoliv počítač';
$strAnyTable = 'Jakákoliv tabulka';
$strAnyUser = 'Jakýkoliv uživatel';
$strArabic = 'Arabština';
$strArmenian = 'Arménština';
$strAscending = 'Vzestupně';
$strAtBeginningOfTable = 'Na začátku tabulky';
$strAtEndOfTable = 'Na konci tabulky';
$strAttr = 'Vlastnosti';
$strAutodetect = 'Automaticky zjistit';
$strAutomaticLayout = 'automatické rozvržení';

$strBack = 'Zpět';
$strBaltic = 'Baltické';
$strBeginCut = 'ZAČÁTEK VÝPISU';
$strBeginRaw = 'ZAČÁTEK VÝPISU';
$strBinary = ' Binární ';
$strBinaryDoNotEdit = ' Binární - neupravujte ';
$strBookmarkAllUsers = 'Umožnit všem uživatelům používat tuto položku';
$strBookmarkDeleted = 'Položka byla smazána z&nbsp;oblíbených.';
$strBookmarkLabel = 'Název';
$strBookmarkOptions = 'Nastavení oblíbeného dotazu';
$strBookmarkQuery = 'Oblíbený SQL dotaz';
$strBookmarkThis = 'Přidat tento SQL dotaz do oblíbených';
$strBookmarkView = 'Jen zobrazit';
$strBrowse = 'Projít';
$strBrowseForeignValues = 'Projít hodnoty cizích klíčů';
$strBulgarian = 'Bulharsky';
$strBzError = 'phpMyAdminovi se nepodařilo zkomprimovat výpis, protože rozšíření pro kompresi Bz2 je v&nbsp;této verzi php chybné. Doporučujeme nastavit <code>$cfg[\'BZipDump\']</code> v&nbsp;nastaveních phpMyAdmina na <code>FALSE</code>. Pokud chcete používat kompresi Bz2, měli byste nainstalovat novější verzi php. Více informací o&nbsp;tomto probnélu je u popisu chyby %s.';
$strBzip = '"zabzipováno"';

$strCSVOptions = 'Nastavení CSV exportu';
$strCannotLogin = 'Nepodařilo se přihlášení k MySQL serveru';
$strCantLoad = 'nelze nahrát rozšíření %s,<br />prosím zkontolujte nastavení PHP';
$strCantLoadMySQL = 'nelze nahrát rozšíření pro MySQL,<br />prosím zkontrolujte nastavení PHP.';
$strCantLoadRecodeIconv = 'Nelze nahrát rozšíření iconv ani recode potřebná pro převod znakových sad. Upravte nastavení php tak aby umožňovalo použít tyto rozšíření nebo vypněte převod znakových sad v&nbsp;phpMyAdminu.';
$strCantRenameIdxToPrimary = 'Index nemůžete přejmenovat na "PRIMARY"!';
$strCantUseRecodeIconv = 'Nelze použít funkce iconv ani libiconv ani recode_string, přestože rozšíření jsou nahrána. Zkontrolujte nastavení php.';
$strCardinality = 'Mohutnost';
$strCarriage = 'Návrat vozíku (CR): \\r';
$strCaseInsensitive = 'nerozlišovat velká a malá písmena';
$strCaseSensitive = 'rozlišovat velká a malá písmena';
$strCentralEuropean = 'Střední Evropa';
$strChange = 'Změnit';
$strChangeCopyMode = 'Vytvořit nového uživatele se stejnými oprávněními a ...';
$strChangeCopyModeCopy = '... zachovat původního uživatele.';
$strChangeCopyModeDeleteAndReload = ' ... smazat uživatele a poté znovu načíst oprávnění.';
$strChangeCopyModeJustDelete = ' ... smazat původního uživatele ze všech tabulek.';
$strChangeCopyModeRevoke = ' ... odebrat všechna oprávnění půodnímu uživateli a poté ho smazat.';
$strChangeCopyUser = 'Změnit informace o&nbsp;uživateli / Kopírovat uživatele';
$strChangeDisplay = 'Zvolte které sloupce zobrazit';
$strChangePassword = 'Změnit heslo';
$strCharset = 'Znaková sada';
$strCharsetOfFile = 'Znaková sada souboru:';
$strCharsets = 'Znakové sady';
$strCharsetsAndCollations = 'Znakové sady a porovnávání';
$strCheckAll = 'Zaškrtnout vše';
$strCheckDbPriv = 'Zkontrolovat oprávnění pro databázi';
$strCheckPrivs = 'Zkontrolovat oprávnění';
$strCheckPrivsLong = 'Zkontrolovat oprávnění pro databázi &quot;%s&quot;.';
$strCheckTable = 'Zkontrolovat tabulku';
$strChoosePage = 'Zvolte stránku, kterou chcete změnit';
$strColComFeat = 'Zobrazuji komentáře sloupců';
$strCollation = 'Porovnávání';
$strColumn = 'Sloupec';
$strColumnNames = 'Názvy sloupců';
$strColumnPrivileges = 'Oprávnění pro jednotlivé sloupce';
$strCommand = 'Příkaz';
$strComments = 'Komentáře';
$strCompleteInserts = 'Úplné inserty';
$strCompression = 'Komprese';
$strConfigFileError = 'phpMyAdmin nemohl načíst konfigurační soubor!<br />Tato chyba může nastat pokud v&nbsp;něm php najde chybu nebo nemůže tento soubor najít.<br />Po kliknutí na následující odkaz se konfigurace spustí a budou zobrazeny informace o&nbsp;chybě, ke které došlo. Pak opravte tuto chybu (nejčastěji se jedná o&nbsp;chybějící středník).<br />Pokud získáte prázdnou stránku, všechno je v&nbsp;pořádku.';
$strConfigureTableCoord = 'Prosím, nastavte souřadnice pro tabulku %s';
$strConfirm = 'Opravdu chcete toto provést?';
$strConnections = 'Připojení';
$strConstraintsForDumped = 'Omezení pro exportované tabulky';
$strConstraintsForTable = 'Omezení pro tabulku';
$strCookiesRequired = 'Během tohoto kroku musíte mít povoleny cookies.';
$strCopyTable = 'Kopírovat tabulku do (databáze<b>.</b>tabulka):';
$strCopyTableOK = 'Tabulka %s byla zkopírována do %s.';
$strCopyTableSameNames = 'Nelze kopírovat tabulku na sebe samu!';
$strCouldNotKill = 'phpMyAdminovi se nepodařilo zabít vlákno %s. Pravděpodobne jeho běh již skončil.';
$strCreate = 'Vytvořit';
$strCreateIndex = 'Vytvořit index na&nbsp;%s&nbsp;sloupcích';
$strCreateIndexTopic = 'Vytvořit nový index';
$strCreateNewDatabase = 'Vytvořit novou databázi';
$strCreateNewTable = 'Vytvořit novou tabulku v&nbsp;databázi %s';
$strCreatePage = 'Vytvořit novou stránku';
$strCreatePdfFeat = 'Vytváření PDF';
$strCriteria = 'Podmínka';
$strCroatian = 'Chorvatsky';
$strCyrillic = 'Cyrilika';
$strCzech = 'Česky';

$strDBComment = 'Komentář k databázi: ';
$strDBGContext = 'Kontext';
$strDBGContextID = 'Kontext ID';
$strDBGHits = 'Zásahů';
$strDBGLine = 'Řádka';
$strDBGMaxTimeMs = 'Min. čas, ms';
$strDBGMinTimeMs = 'Max. čas, ms';
$strDBGModule = 'Modul';
$strDBGTimePerHitMs = 'Čas/Zásah, ms';
$strDBGTotalTimeMs = 'Celkový čas, ms';
$strDanish = 'Dánsky';
$strData = 'Data';
$strDataDict = 'Datový slovník';
$strDataOnly = ' Jen data';
$strDatabase = 'Databáze ';
$strDatabaseExportOptions = 'Nastavení exportu databází';
$strDatabaseHasBeenDropped = 'Databáze %s byla zrušena.';
$strDatabaseNoTable = 'Tato databáze neobsahuje žádné tabulky!';
$strDatabaseWildcard = 'Databáze (zástupné znaky povoleny):';
$strDatabases = 'Databáze';
$strDatabasesDropped = '%s databáze byla úspěšně zrušena.';
$strDatabasesStats = 'Statistiky databází';
$strDatabasesStatsDisable = 'Skrýt podrobnosti';
$strDatabasesStatsEnable = 'Zobrazit podrobnosti';
$strDatabasesStatsHeavyTraffic = 'Poznámka: Zobrazení podrobností o&nbsp;databáchích může způsobit značné zvýšení provozu mezi webserverem a MySQL serverem.';
$strDbPrivileges = 'Oprávnění pro jednotlivé databáze';
$strDbSpecific = 'závislé na databázi';
$strDefault = 'Výchozí';
$strDefaultValueHelp = 'Výchozí hodnotu zadejte jen jednu hodnotu bez uvozovek a escapování znaků, například: a';
$strDelOld = 'Aktuální stránka se odkazuje na tabulky, které již neexistují. Chcete odstranit tyto odkazy?';
$strDelete = 'Smazat';
$strDeleteAndFlush = 'Odstranit uživatele a znovunačíst oprávnění.';
$strDeleteAndFlushDescr = 'Toto je nejčistčí řešení, ale načítání oprávnění může trvat dlouho.';
$strDeleteFailed = 'Smazání selhalo!';
$strDeleteUserMessage = 'Byl smazán uživatel %s.';
$strDeleted = 'Řádek byl smazán';
$strDeletedRows = 'Smazané řádky:';
$strDeleting = 'Odstraňuji %s';
$strDescending = 'Sestupně';
$strDescription = 'Popis';
$strDictionary = 'slovník';
$strDisabled = 'Vypnuto';
$strDisplay = 'Zobrazit';
$strDisplayFeat = 'Zobrazení funkcí';
$strDisplayOrder = 'Seřadit podle:';
$strDisplayPDF = 'Zobrazit jako schéma v&nbsp;PDF';
$strDoAQuery = 'Provést "dotaz podle příkladu" (zástupný znak: "%")';
$strDoYouReally = 'Opravdu si přejete vykonat příkaz';
$strDocu = 'Dokumentace';
$strDrop = 'Odstranit';
$strDropDB = 'Odstranit databázi %s';
$strDropSelectedDatabases = 'Zrušit vybranou databázi';
$strDropTable = 'Smazat tabulku';
$strDropUsersDb = 'Odstranit databáze se stejnými jmény jako uživatelé.';
$strDumpComments = 'Vložit komentáře ke sloupcům jako SQL komentáře';
$strDumpSaved = 'Výpis byl uložen do souboru %s.';
$strDumpXRows = 'Vypsat %s řádků od %s.';
$strDumpingData = 'Vypisuji data pro tabulku';
$strDynamic = 'dynamický';

$strEdit = 'Upravit';
$strEditPDFPages = 'Upravit PDF stránky';
$strEditPrivileges = 'Upravit oprávnění';
$strEffective = 'Efektivní';
$strEmpty = 'Vyprázdnit';
$strEmptyResultSet = 'MySQL vrátil prázdný výsledek (tj. nulový počet řádků).';
$strEnabled = 'Zapnuto';
$strEnd = 'Konec';
$strEndCut = 'KONEC VÝPISU';
$strEndRaw = 'KONEC VÝPISU';
$strEnglish = 'Anglicky';
$strEnglishPrivileges = 'Poznámka: názvy oprávnění v&nbsp;MySQL jsou uváděny anglicky';
$strError = 'Chyba';
$strEstonian = 'Estonsky';
$strExcelOptions = 'Nastavení exportu do Excelu';
$strExecuteBookmarked = 'Spustit oblíbený dotaz';
$strExplain = 'Vysvětlit (EXPLAIN) SQL';
$strExport = 'Export';
$strExportToXML = 'Export do XML';
$strExtendedInserts = 'Rozšířené inserty';
$strExtra = 'Extra';

$strFailedAttempts = 'Nepovedených pokusů';
$strField = 'Sloupec';
$strFieldHasBeenDropped = 'Sloupec %s byl odstraněn';
$strFields = 'Sloupce';
$strFieldsEmpty = ' Nebyl zadán počet sloupců! ';
$strFieldsEnclosedBy = 'Názvy sloupců uzavřené do';
$strFieldsEscapedBy = 'Názvy sloupců escapovány';
$strFieldsTerminatedBy = 'Sloupce oddělené';
$strFileAlreadyExists = 'Soubor %s již na serveru existuje, změntě jméno souboru, nebo zvolte přepsání souboru.';
$strFileCouldNotBeRead = 'Soubor nelze přečíst';
$strFileNameTemplate = 'Vzor pro jméno souboru';
$strFileNameTemplateHelp = 'Použijte __DB__ pro jméno databáze, __TABLE__ pro jméno tabulky a jakékoliv parametry pro %sfunkci strftime%s pro vložení data. Přípona souboru bude automaticky přidána podle typu. Jakýkoliv jiný text bude zachován.';
$strFileNameTemplateRemember = 'zapamatovat si hodnotu';
$strFixed = 'pevný';
$strFlushPrivilegesNote = 'Poznámka: phpMyAdmin získává oprávnění přímo z&nbsp;tabulek MySQL. Obsah těchto tabulek se může lišit od oprávnění, která server právě používá, pokud byly tyto tabulky upravovány. V&nbsp;tomto případě je vhodné provést %sznovunačtení oprávnění%s před pokračováním.';
$strFlushTable = 'Vyprázdnit vyrovnávací paměť pro tabulku ("FLUSH")';
$strFormEmpty = 'Chybějící hodnota ve formuláři!';
$strFormat = 'Formát';
$strFullText = 'Celé texty';
$strFunction = 'Funkce';

$strGenBy = 'Vygeneroval';
$strGenTime = 'Vygenerováno';
$strGeneralRelationFeat = 'Obecné funkce relací';
$strGerman = 'Německy';
$strGlobal = 'globální';
$strGlobalPrivileges = 'Globální oprávnění';
$strGlobalValue = 'Globální hodnota';
$strGo = 'Proveď';
$strGrantOption = 'Přidělování';
$strGrants = 'Přidělování';
$strGreek = 'Řečtina';
$strGzip = '"zagzipováno"';

$strHasBeenAltered = 'byla změněna.';
$strHasBeenCreated = 'byla vytvořena.';
$strHaveToShow = 'Musíte volit alespoň jeden sloupec, který chcete zobrazit.';
$strHebrew = 'Hebrejsky';
$strHome = 'Hlavní strana';
$strHomepageOfficial = 'Oficiální stránka phpMyAdmina';
$strHomepageSourceforge = 'Nová stránka phpMyAdmina';
$strHost = 'Počítač';
$strHostEmpty = 'Jméno počítače je prázdné!';
$strHungarian = 'Maďarsky';

$strId = 'ID';
$strIdxFulltext = 'Fulltext';
$strIfYouWish = 'Pokud si přejete natáhnout jen vybrané sloupce z&nbsp;tabulky, napište je jako seznam sloupců oddělených čárkou.';
$strIgnore = 'Ignorovat';
$strIgnoringFile = 'Ignoruji soubor %s';
$strImportDocSQL = 'Importovat soubory docSQL';
$strImportFiles = 'Importovat soubory';
$strImportFinished = 'Import ukončen';
$strInUse = 'právě se používá';
$strIndex = 'Index';
$strIndexHasBeenDropped = 'Index %s byl odstraněn';
$strIndexName = 'Jméno indexu&nbsp;:';
$strIndexType = 'Typ indexu&nbsp;:';
$strIndexes = 'Indexy';
$strInnodbStat = 'Stav InnoDB';
$strInsecureMySQL = 'Váš konfigurační soubor obsahuje nastavení (uživatel root bez hesla), které je výchozí pro MySQL. Váš MySQL server s&nbsp;tímto výchozím nastavením je snadno napadnutelný, a proto byste měli změnit toto nastavení a tím podstatně zvýšit bezpečnost Vašeho serveru.';
$strInsert = 'Vložit';
$strInsertAsNewRow = 'Vložit jako nový řádek';
$strInsertNewRow = 'Vložit nový řádek';
$strInsertTextfiles = 'Vložit textové soubory do tabulky';
$strInsertedRowId = 'Id vloženého řádku:';
$strInsertedRows = 'Vloženo řádků:';
$strInstructions = 'Instrukce';
$strInternalNotNecessary = '* Interní relace není nutná, pokud již relace existuje v InnoDB.';
$strInternalRelations = 'Interní relace';
$strInvalidName = '"%s" je rezervované slovo a proto ho nemůžete požít jako jméno databáze/tabulky/sloupce.';

$strJapanese = 'Japonština';
$strJumpToDB = 'Na dattabázi &quot;%s&quot;.';
$strJustDelete = 'Jen odstranit uživatele z tabulek s oprávněními.';
$strJustDeleteDescr = 'Odstranění uživatelé stále budou mít přistup na server dokud nebudou znovunačtena oprávnění.';

$strKeepPass = 'Neměnit heslo';
$strKeyname = 'Klíčový název';
$strKill = 'Zabít';
$strKorean = 'Korejština';

$strLaTeX = 'LaTeX';
$strLaTeXOptions = 'Nastavení exportu do LaTeXu';
$strLandscape = 'Na šířku';
$strLatexCaption = 'Titulek tabulky';
$strLatexContent = 'Obsah tabulky __TABLE__';
$strLatexContinued = '(pokračování)';
$strLatexContinuedCaption = 'Titulek pokračování tabulky';
$strLatexIncludeCaption = 'Použít titulek tabulky';
$strLatexLabel = 'Návěstí';
$strLatexStructure = 'Struktura tabulky __TABLE__';
$strLength = 'Délka';
$strLengthSet = 'Délka/Množina*';
$strLimitNumRows = 'záznamů na stránku';
$strLineFeed = 'Ukončení řádku (Linefeed): \\n';
$strLines = 'Řádek';
$strLinesTerminatedBy = 'Řádky ukončené';
$strLinkNotFound = 'Odkaz nenalezen';
$strLinksTo = 'Odkazuje na';
$strLithuanian = 'Litevsky';
$strLoadExplanation = 'Automaticky jsou zvoleny nejvhodnější parametry, pokud toto nastavení selže, můžete zkusit druhou možnost.';
$strLoadMethod = 'Parametry pro příkaz LOAD';
$strLocalhost = 'Lokální';
$strLocationTextfile = 'textový soubor';
$strLogPassword = 'Heslo:';
$strLogServer = 'Server';
$strLogUsername = 'Jméno:';
$strLogin = 'Přihlášení';
$strLoginInformation = 'Přihlašování';
$strLogout = 'Odhlásit se';

$strMIME_MIMEtype = 'MIME typ';
$strMIME_available_mime = 'Dostupné MIME typy';
$strMIME_available_transform = 'Dostupné tranformace';
$strMIME_description = 'Popis';
$strMIME_file = 'Jméno souboru';
$strMIME_nodescription = 'Pro tuto transformaci není dostupný žádný popis.<br />Zeptejte se autora co %s dělá.';
$strMIME_transformation = 'Transformace při prohlížení';
$strMIME_transformation_note = 'Pro seznam dostupných parametrů transformací a jejich MIME typů klikněte na %spopisy transformací%s';
$strMIME_transformation_options = 'Parametry transformace';
$strMIME_transformation_options_note = 'Zadejte parametry transformací v&nbsp;následujícím tvaru: \'a\',\'b\',\'c\'...<br />Pokud potřebujete použít zpětné lomítko ("\") nebo jednoduché uvozovky ("\'") mezi těmito hodnotami, vložte před ně zpětné lomítko (například \'\\\\xyz\' or \'a\\\'b\').';
$strMIME_without = 'MIME typy zobrazené kurzívou nemají vlastní transformační funkci';
$strMissingBracket = 'Chybí závorka';
$strModifications = 'Změny byly uloženy';
$strModify = 'Úpravy';
$strModifyIndexTopic = 'Upravit index';
$strMoreStatusVars = 'Další informace o&nbsp;stavu';
$strMoveTable = 'Přesunout tabulku do (databáze<b>.</b>tabulka):';
$strMoveTableOK = 'Tabulka %s byla přesunuta do %s.';
$strMoveTableSameNames = 'Nelze přeunout tabulku na sebe samu!';
$strMultilingual = 'mnohojazyčný';
$strMustSelectFile = 'Zvolte soubor, který chcete vložit.';
$strMySQLCharset = 'Znaková sada v&nbsp;MySQL';
$strMySQLReloaded = 'MySQL znovu načteno.';
$strMySQLSaid = 'MySQL hlásí: ';
$strMySQLServerProcess = 'MySQL %pma_s1% spuštěné na %pma_s2%, přihlášen %pma_s3%';
$strMySQLShowProcess = 'Zobrazit procesy';
$strMySQLShowStatus = 'Ukázat MySQL informace o&nbsp;běhu';
$strMySQLShowVars = 'Ukázat MySQL systémové proměnné';

$strName = 'Název';
$strNext = 'Další';
$strNo = 'Ne';
$strNoDatabases = 'Žádné databáze';
$strNoDatabasesSelected = 'Nebyla vybrána žádná databáze.';
$strNoDescription = 'žádný popisek';
$strNoDropDatabases = 'Příkaz "DROP DATABASE" je vypnutý.';
$strNoExplain = 'Bez vysvětlení (EXPLAIN) SQL';
$strNoFrames = 'phpMyAdmin se lépe používá v&nbsp;prohlížeči podporujícím rámy ("FRAME").';
$strNoIndex = 'Žádný index nebyl definován!';
$strNoIndexPartsDefined = 'Žádná část indexu nebyla definována!';
$strNoModification = 'Žádná změna';
$strNoOptions = 'Tento formát nemá žádná nastavení';
$strNoPassword = 'Žádné heslo';
$strNoPermission = 'Web server nemá oprávnění uložit výpis do souboru %s.';
$strNoPhp = 'Bez PHP kódu';
$strNoPrivileges = 'Bez oprávnění';
$strNoQuery = 'Žádný SQL dotaz!';
$strNoRights = 'Nemáte dostatečná práva na provedení této akce!';
$strNoSpace = 'Nedostatek místa pro uložení souboru %s.';
$strNoTablesFound = 'V&nbsp;databázi nebyla nalezena ani jedna tabulka.';
$strNoUsersFound = 'Žádný uživatel nenalezen.';
$strNoUsersSelected = 'Nebyl vybrán žádný uživatel.';
$strNoValidateSQL = 'Bez kontroly SQL';
$strNone = 'Žádná';
$strNotNumber = 'Toto není číslo!';
$strNotOK = 'není OK';
$strNotSet = '<b>%s</b> tabulka nenalezena nebo není nastavena v&nbsp;%s';
$strNotValidNumber = ' není platné číslo řádku!';
$strNull = 'Nulový';
$strNumSearchResultsInTable = '%s odpovídající(ch) záznam(ů) v&nbsp;tabulce <i>%s</i>';
$strNumSearchResultsTotal = '<b>Celkem:</b> <i>%s</i> odpovídající(ch) záznam(ů)';
$strNumTables = 'Tabulek';

$strOK = 'OK';
$strOftenQuotation = 'Často uvozující znaky. Volitelně znamená, že pouze položky u kterých je to nutné (obvykle typu CHAR a VARCHAR) jsou uzavřeny do uzavíracích znaků.';
$strOperations = 'Úpravy';
$strOptimizeTable = 'Optimalizovat tabulku';
$strOptionalControls = 'Volitelné. Určuje jak zapisovat nebo číst speciální znaky.';
$strOptionally = 'Volitelně';
$strOptions = 'Vlastnosti';
$strOr = 'nebo';
$strOverhead = 'Navíc';
$strOverwriteExisting = 'Přepsat existující soubor(y)';

$strPHP40203 = 'Používáte PHP 4.2.3, které má závažnou chybu při práci s&nbsp;vícebajtovými znaky (mbsting), jedná se o&nbsp;chybu PHP číslo 19404. Nedoporučujeme používat tuto verzi PHP s&nbsp;phpMyAdminem.';
$strPHPVersion = 'Verze PHP';
$strPageNumber = 'Strana číslo:';
$strPaperSize = 'Velikost stránky';
$strPartialText = 'Zkrácené texty';
$strPassword = 'Heslo';
$strPasswordChanged = 'Heslo pro %s bylo úspěšně změněno.';
$strPasswordEmpty = 'Heslo je prázdné!';
$strPasswordNotSame = 'Hesla nejsou stejná!';
$strPdfDbSchema = 'Schéma databáze "%s" - Strana %s';
$strPdfInvalidPageNum = 'Nedefinované číslo stránky v&nbsp;PDF!';
$strPdfInvalidTblName = 'Tabulka "%s" neexistuje!';
$strPdfNoTables = 'žádné tabulky';
$strPerHour = 'za hodinu';
$strPerMinute = 'za minutu';
$strPerSecond = 'za sekundu';
$strPhoneBook = 'adresář';
$strPhp = 'Zobrazit PHP kód';
$strPmaDocumentation = 'Dokumentace phpMyAdmina';
$strPmaUriError = 'Parametr <tt>$cfg[\'PmaAbsoluteUri\']</tt> MUSÍ být nastaven v&nbsp;konfiguračním souboru!';
$strPortrait = 'Na výšku';
$strPos1 = 'Začátek';
$strPrevious = 'Předchozí';
$strPrimary = 'Primární';
$strPrimaryKey = 'Primární klíč';
$strPrimaryKeyHasBeenDropped = 'Primární klíč byl odstraněn';
$strPrimaryKeyName = 'Jméno primárního klíče musí být "PRIMARY"!';
$strPrimaryKeyWarning = '("PRIMARY" <b>musí</b> být jméno <b>pouze</b> primárního klíče!)';
$strPrint = 'Vytisknout';
$strPrintView = 'Náhled k vytištění';
$strPrintViewFull = 'Náhled k vytištění (s&nbsp;kompletními texty)';
$strPrivDescAllPrivileges = 'Všechna oprávnění kromě GRANT.';
$strPrivDescAlter = 'Umožňuje měnit strukturu existujících tabulek.';
$strPrivDescCreateDb = 'Umožňuje vytvářet nové databáze a tabulky.';
$strPrivDescCreateTbl = 'Umožňuje vytvářet nové tabulky.';
$strPrivDescCreateTmpTable = 'Umožňuje vytvářet dočasné tabulky.';
$strPrivDescDelete = 'Umožňuje mazat data.';
$strPrivDescDropDb = 'Umožňuje odstranit databáze a tabulky.';
$strPrivDescDropTbl = 'Umožňuje odstranit tabulky.';
$strPrivDescExecute = 'Umožnuje spouštět uložené procedury. V&nbsp;této verzi MySQL se nepoužívá.';
$strPrivDescFile = 'Umožňuje importovat a exportovat data z/do souborů na serveru.';
$strPrivDescGrant = 'Umožňuje přidávat uživatele a oprávnění bez znovunačítání tabulek s&nbsp;oprávněními.';
$strPrivDescIndex = 'Umožňuje vytvářet a rušit indexy.';
$strPrivDescInsert = 'Umožňuje vkládata a přepisovat data.';
$strPrivDescLockTables = 'Umožňuje zamknout tabulku pro aktuální thread.';
$strPrivDescMaxConnections = 'Omezí počet nových připojení, která může uživatel vytvořit za hodinu.';
$strPrivDescMaxQuestions = 'Omezí kolik dotazů může uživatel odeslat serveru za hodinu.';
$strPrivDescMaxUpdates = 'Omezí kolik dotazů, které mění nejakou tabulku nebo databázi, může uživatel spustit za hodinu.';
$strPrivDescProcess3 = 'Umožňuje zabíjet procesy jiným uživatelům.';
$strPrivDescProcess4 = 'Umožňuje vidět kompletní dotazy v&nbsp;seznamu procesů.';
$strPrivDescReferences = 'Nemá žádný vliv v&nbsp;této verzi MySQL.';
$strPrivDescReload = 'Umožňuje znovunačítání nastavení serveru a vyprázdnění vyrovnávacích pamětí serveru.';
$strPrivDescReplClient = 'Umožní uživateli zjistit kde je hlavní / pomocný server.';
$strPrivDescReplSlave = 'Potřebné pro replikaci pomocných serverů.';
$strPrivDescSelect = 'Umožňuje číst data.';
$strPrivDescShowDb = 'Dává přístup k&nbsp;úplnému seznamu databází.';
$strPrivDescShutdown = 'Umožňuje vypnout server.';
$strPrivDescSuper = 'Umožňuje připojení i když je dosažen maximální počet připojení. Potřebné pro většinu operací pro správu serveru jako nastavování globálních proměnných a zabíjení threadů jiných uživatelů.';
$strPrivDescUpdate = 'Umožňuje měnit data.';
$strPrivDescUsage = 'Žádná oprávnění.';
$strPrivileges = 'Oprávnění';
$strPrivilegesReloaded = 'Oprávnění byla znovunačtena úspěšně.';
$strProcesslist = 'Seznam procesů';
$strProperties = 'Vlastnosti';
$strPutColNames = 'Přidat jména sloupců na první řádek';

$strQBE = 'Dotaz';
$strQBEDel = 'smazat';
$strQBEIns = 'přidat';
$strQueryFrame = 'SQL okno';
$strQueryFrameDebug = 'Ladicí informace';
$strQueryFrameDebugBox = 'Aktivní proměnné pro formulář dotazu:\nDB: %s\nTabulka: %s\nServer: %s\n\nSou4asn0 hodnoty prom2nn7ch pro formul85 dotazu:\nDB: %s\nTabulke: %s\nServer: %s\n\nOtevřeno z: %s\nFrameset: %s.';
$strQueryOnDb = 'SQL dotaz na databázi <b>%s</b>:';
$strQuerySQLHistory = 'SQL historie';
$strQueryStatistics = '<b>Statistika dotazů</b>: Od spuštění bylo serveru posláno %s dotazů.';
$strQueryTime = 'Dotaz zabral %01.4f sekund';
$strQueryType = 'Typ dotazu';
$strQueryWindowLock = 'Nepřepisovat tento dotaz z&nbsp;hlavního okna';

$strReType = 'Heslo znovu';
$strReceived = 'Přijato';
$strRecords = 'Záznamů';
$strReferentialIntegrity = 'Zkontrolovat integritu odkazů:';
$strRelationNotWorking = 'Některé funkce pro práci s&nbsp;propojenými tabulkami byly vypnuty. %sZde%s zjistíte proč.';
$strRelationView = 'Zobrazit relace';
$strRelationalSchema = 'Relační schéma';
$strRelations = 'Relace';
$strReloadFailed = 'Znovunačtení MySQL selhalo.';
$strReloadMySQL = 'Znovunačtení MySQL';
$strReloadingThePrivileges = 'Znovunačítám oprávnění';
$strRememberReload = 'Nezapomeňte znovu načíst server.';
$strRemoveSelectedUsers = 'Odstranit vybrané uživatele';
$strRenameTable = 'Přejmenovat tabulku na';
$strRenameTableOK = 'Tabulka %s byla přejmenována na %s';
$strRepairTable = 'Opravit tabulku';
$strReplace = 'Přepsat';
$strReplaceNULLBy = 'Nahradit NULL hodnoty';
$strReplaceTable = 'Přepsat data tabulky souborem';
$strReset = 'Původní';
$strResourceLimits = 'Omezení zdrojů';
$strRevoke = 'Zrušit';
$strRevokeAndDelete = 'Odebrat uživatelům veškerá oprávnění a poté je odstranit z tabulek.';
$strRevokeAndDeleteDescr = 'Uživatelé budou mít oprávnění "USAGE" (používání) dokud nebudou znovunačtena oprávnění.';
$strRevokeGrant = 'Zrušit oprávnění přidělovat práva';
$strRevokeGrantMessage = 'Bylo zrušeno oprávnění přidělovat práva pro %s';
$strRevokeMessage = 'Byla zrušena práva pro %s';
$strRevokePriv = 'Zrušit práva';
$strRowLength = 'Délka řádku';
$strRowSize = ' Velikost řádku ';
$strRows = 'Řádků';
$strRowsFrom = 'řádků začínající od';
$strRowsModeFlippedHorizontal = 'vodorovném (otočené hlavičky)';
$strRowsModeHorizontal = 'vodorovném';
$strRowsModeOptions = 've %s režimu a opakovat hlavičky po %s řádcích.';
$strRowsModeVertical = 'svislém';
$strRowsStatistic = 'Statistika řádků';
$strRunQuery = 'Provést dotaz';
$strRunSQLQuery = 'Spustit SQL dotaz(y) na databázi %s';
$strRunning = 'na %s';
$strRussian = 'Ruština';

$strSQL = 'SQL';
$strSQLOptions = 'Nastavení SQL exportu';
$strSQLParserBugMessage = 'Je možné, že jste našli chybu v&nbsp;SQL parseru. Prosím prozkoumejte podrobně SQL dotaz, především jestli jsou správně uvozovky a jestli nejsou proházené. Další možnost selhání je pokud nahráváte soubor s&nbsp;binárními daty nezapsanými v&nbsp;uvozovkách. Můžete také vyzkoušet příkazovou řádku MySQL. Níže uvedený výstup z&nbsp;MySQL serveru (pokud je nějaký) Vám také může pomoci při zkoumání problému. Pokud stále máte problémy nebo pokud SQL parser ohlásí chybu u dotazu, který na příkazové řádce funguje, prosím pokuste se zredukovat dotaz na co nejmenší, ve kterém se problém ještě vyskytne, a ohlašte chybu na stránkách phpMyAdmina spolu se sekcí VÝPIS uvedenou níže:';
$strSQLParserUserError = 'Pravděpodobně máte v&nbsp;SQL dotazu chybu. Níže uvedený výstup MySQL serveru (pokud je nějaký) Vám také může pomoci při zkoumání problému';
$strSQLQuery = 'SQL-dotaz';
$strSQLResult = 'Výsledek SQL dotazu';
$strSQPBugInvalidIdentifer = 'Chybný identifikátor';
$strSQPBugUnclosedQuote = 'Neuzavřené uvozovky';
$strSQPBugUnknownPunctuation = 'Neznámé interpunkční znaménko';
$strSave = 'Ulož';
$strSaveOnServer = 'Uložit na serveru v adresáři %s';
$strScaleFactorSmall = 'Měřítko je příliš malé, aby se schéma vešlo na jednu stránku';
$strSearch = 'Vyhledávání';
$strSearchFormTitle = 'Vyhledávání v&nbsp;databázi';
$strSearchInTables = 'V&nbsp;tabulkách:';
$strSearchNeedle = 'Slova nebo hodnoty, které chcete vyhledat (zástupný znak: "%"):';
$strSearchOption1 = 'alespoň jedno ze slov';
$strSearchOption2 = 'všechna slova';
$strSearchOption3 = 'přesnou frázi';
$strSearchOption4 = 'jako regulární výraz';
$strSearchResultsFor = 'Výsledny vyhledávání pro "<i>%s</i>" %s:';
$strSearchType = 'Najít:';
$strSecretRequired = 'Nastavte klíč pro šifrování cookies v&nbsp;kofiguračním souboru (blowfish_secret).';
$strSelect = 'Vybrat';
$strSelectADb = 'Prosím vyberte databázi';
$strSelectAll = 'Vybrat vše';
$strSelectFields = 'Zvolte sloupec (alespoň jeden):';
$strSelectNumRows = 'v&nbsp;dotazu';
$strSelectTables = 'Vybrat tabulky';
$strSend = 'Do souboru';
$strSent = 'Odesláno';
$strServer = 'Server %s';
$strServerChoice = 'Výběr serveru';
$strServerStatus = 'Stav serveru';
$strServerStatusUptime = 'Tento MySQL server beží %s. Čas spuštění: %s.';
$strServerTabProcesslist = 'Procesy';
$strServerTabVariables = 'Proměnné';
$strServerTrafficNotes = '<b>Provoz serveru</b>: Informace o&nbsp;síťovém provozu MySQL serveru od jeho spuštění.';
$strServerVars = 'Proměnné a nastavení serveru';
$strServerVersion = 'Verze MySQL';
$strSessionValue = 'Hodnota sezení';
$strSetEnumVal = 'Pokud je sloupec typu "enum" nebo "set", zadávejte hodnoty v&nbsp;následujícím formátu: \'a\',\'b\',\'c\'...<br />Pokud potřebujete zadat zpětné lomítko ("\") nebo jednoduché uvozovky ("\'") mezi těmito hodnotami, napište před ně zpětné lomítko (příklad: \'\\\\xyz\' nebo \'a\\\'b\').';
$strShow = 'Zobrazit';
$strShowAll = 'Zobrazit vše';
$strShowColor = 'Barevné šipky';
$strShowCols = 'Zobrazit sloupce';
$strShowDatadictAs = 'Formát datového slovníku';
$strShowFullQueries = 'Zobrazit celé dotazy';
$strShowGrid = 'Zobrazit mřížku';
$strShowPHPInfo = 'Zobrazit informace o&nbsp;PHP';
$strShowTableDimension = 'Rozměry tabulek';
$strShowTables = 'Zobrazit tabulky';
$strShowThisQuery = 'Zobrazit zde tento dotaz znovu';
$strShowingRecords = 'Zobrazeny záznamy';
$strSimplifiedChinese = 'Zjedodušená Čínština';
$strSingly = '(po jednom)';
$strSize = 'Velikost';
$strSort = 'Řadit';
$strSortByKey = 'Setřídit podle klíče';
$strSpaceUsage = 'Využití místa';
$strSplitWordsWithSpace = 'Slova jsou oddělena mezerou (" ").';
$strStatCheckTime = 'Poslední kontrola';
$strStatCreateTime = 'Vytvoření';
$strStatUpdateTime = 'Poslední změna';
$strStatement = 'Údaj';
$strStatus = 'Stav';
$strStrucCSV = 'CSV data';
$strStrucData = 'Strukturu a data';
$strStrucDrop = 'Přidej \'DROP TABLE\'';
$strStrucExcelCSV = 'CSV data pro MS Excel';
$strStrucOnly = 'Pouze strukturu';
$strStructPropose = 'Navrhnout strukturu tabulky';
$strStructure = 'Struktura';
$strSubmit = 'Odešli';
$strSuccess = 'Váš SQL-dotaz byl úspěšně vykonán';
$strSum = 'Celkem';
$strSwedish = 'Švédsky';
$strSwitchToTable = 'Přepnout na zkopírovanou tabulku';

$strTable = 'Tabulka';
$strTableComments = 'Komentář k tabulce';
$strTableEmpty = 'Jméno tabulky je prázdné!';
$strTableHasBeenDropped = 'Tabulka %s byla odstraněna';
$strTableHasBeenEmptied = 'Tabulka %s byla vyprázdněna';
$strTableHasBeenFlushed = 'Vyrovnávací paměť pro tabulku %s byla vyprázdněna';
$strTableMaintenance = ' Údržba tabulky ';
$strTableOfContents = 'Obsah';
$strTableOptions = 'Parametry tabulky';
$strTableStructure = 'Struktura tabulky';
$strTableType = 'Typ tabulky';
$strTables = '%s tabulek';
$strTblPrivileges = 'Oprávnění pro jednotlivé tabulky';
$strTextAreaLength = 'Tento sloupec možná nepůjde <br />(kvůli délce) upravit ';
$strThai = 'Thajština';
$strTheContent = 'Obsah souboru byl vložen';
$strTheContents = 'Obsah souboru přepíše obsah zvolené tabulky v&nbsp;těch řádcích, kde je stejný primární nebo unikátní klíč.';
$strTheTerminator = 'Sloupce jsou odděleny tímto znakem.';
$strThisHost = 'Tento počítač';
$strThisNotDirectory = 'Nebyl zadán adresář';
$strThreadSuccessfullyKilled = 'Vlákno %s bylo úspěšne zabito.';
$strTime = 'Čas';
$strToggleScratchboard = 'Zobrazit grafický návrh';
$strTotal = 'celkem';
$strTotalUC = 'Celkem';
$strTraditionalChinese = 'Tradiční Čínština';
$strTraffic = 'Provoz';
$strTransformation_image_jpeg__inline = 'Zobrazí náhled obrázku s&nbsp;odkazem na obrázek; parametry šířka a výška v&nbsp;bodech (poměr stran obrázku zůstane zachován)';
$strTransformation_image_jpeg__link = 'Zobrazí odkaz na obrázek (například stáhnutí pole blob).';
$strTransformation_image_png__inline = 'Viz image/jpeg: inline';
$strTransformation_text_plain__dateformat = 'Zobrazí datum nebo čas (TIME, TIMESTAMP a DATETIME) podle místního nastavení. První parametr je posun (v&nbsp;hodinách), který bude přidán k&nbsp;času (výchozí je 0). Druhý parametr je formátovací řetězec pro funkci strftime().';
$strTransformation_text_plain__external = 'JEN PRO LINUX: Spustí externí program, na jeho standardní vstup pošle obsah pole a zobrazí výstup programu. Výchozí je program Tidy, který pěkně zformátuje HTML. Z&nbsp;bezpečnostních důvodů musíte jména povolených programů zapsat do souboru libraries/transformations/text_plain__external.inc.php. První parametr je číslo programu, který má být spuštěn a druhý parametr udává parametry tohoto programu. Třetí parametr určuje, zda mají být ve výstupu nahrazeny HTML entity (např. pro zobrazení zdrojového kódu HTML) (výchozí je 1, tedy převádět na entity), čtvrtý (při nastavení na 1) zajistí přidání parametru NOWRAP k&nbsp;vypisovanému textu, čímž se zachová formátování (výchozí je 1).';
$strTransformation_text_plain__formatted = 'Zachová původní formátování sloupce, tak jak je uložen v&nbsp;databázi.';
$strTransformation_text_plain__imagelink = 'Zobrazí obrázek a odkaz z&nbsp;pole obsahujícího odkaz na obrázek. První parametr je prefix URL (například "http://mojedomena.cz/", druhý a třetí šířku a výšku obrázku.';
$strTransformation_text_plain__link = 'Zobrazí odkaz z&nbsp;pole obsahující odkaz. První parametr je prefix URL (například "http://mojedomena.cz/", druhý text odkazu.';
$strTransformation_text_plain__substr = 'Zobrazí jen část textu. První parametr je posun od začátku (výchozí je 0) a druhý určuje délku textu, který se má zobrazit, pokud není uveden, bude zobrazen zbytek textu. Třetí parametr uřuje jaký text má být přidán za zkrácený text (výchozí je ...).';
$strTransformation_text_plain__unformatted = 'Zobrazí text pomocí HTML entit, případný HTML se zobrazí v&nbsp;původním tvaru.';
$strTruncateQueries = 'Zobrazit zkrácené dotazy';
$strTurkish = 'Turecky';
$strType = 'Typ';

$strUkrainian = 'Ukrajinsky';
$strUncheckAll = 'Odškrtnout vše';
$strUnicode = 'Unicode';
$strUnique = 'Unikátní';
$strUnknown = 'neznámý';
$strUnselectAll = 'Odznačit vše';
$strUpdComTab = 'Podívejte se prosím do dokumentace, jak aktualizovat tabulku s&nbsp;informacemi o&nbsp; sloupcích (Column_comments Table)';
$strUpdatePrivMessage = 'Byla aktualizovana oprávnění pro %s.';
$strUpdateProfile = 'Změny přístupu:';
$strUpdateProfileMessage = 'Přístup byl změněn.';
$strUpdateQuery = 'Aktualizovat dotaz';
$strUpgradeMySQL = 'Měli byste aktualizovat MySQL na verzi %s nebo vyšší.';
$strUsage = 'Používá';
$strUseBackquotes = 'Použít zpětné uvozovky u&nbsp;jmen tabulek a sloupců';
$strUseHostTable = 'Použít tabulku s&nbsp;počítači';
$strUseTables = 'Použít tabulky';
$strUseTextField = 'Použít textové pole';
$strUseThisValue = 'Použít tuto hodnotu';
$strUser = 'Uživatel';
$strUserAlreadyExists = 'Uživatel %s již existuje!';
$strUserEmpty = 'Jméno uživatele je prázdné!';
$strUserName = 'Jméno uživatele';
$strUserNotFound = 'Zvolený uživatel nebyl nalezen v&nbsp;tabulce oprávnění.';
$strUserOverview = 'Přehled uživatelů';
$strUsers = 'Uživatelé';
$strUsersDeleted = 'Vybraní uživatelé byli úspěšně odstraněni.';
$strUsersHavingAccessToDb = 'Uživatelé mající přístup k&nbsp;&quot;%s&quot;';

$strValidateSQL = 'Zkontrolovat SQL';
$strValidatorError = 'SQL validator nemohl být inicializován. Prosím zkontrolujte jestli máte nainstalované potřebné rozšíření php, jak je popsáno v&nbsp;%sdokumentaci%s.';
$strValue = 'Hodnota';
$strVar = 'Proměnná';
$strViewDump = 'Export tabulky';
$strViewDumpDB = 'Export databáze';
$strViewDumpDatabases = 'Export databází';

$strWebServerUploadDirectory = 'soubor z&nbsp;adresáře pro upload';
$strWebServerUploadDirectoryError = 'Adresář určený pro upload souborů nemohl být otevřen';
$strWelcome = 'Vítej v&nbsp;%s';
$strWestEuropean = 'Západní Evropa';
$strWildcard = 'maska';
$strWindowNotFound = 'Cílové onko prohlížeče nemohlo býy aktualizováno. Možná jste zavřel rodičovské okno, nebo prohlížeč blokuje operace mezi okny z důvodu bezpečnostních nastavení.';
$strWithChecked = 'Zaškrtnuté:';
$strWritingCommentNotPossible = 'Nelze zapsat komentář';
$strWritingRelationNotPossible = 'Nelze zapsat relaci';
$strWrongUser = 'Špatné uživatelské jméno nebo heslo. Přístup odepřen.';

$strXML = 'XML';

$strYes = 'Ano';

$strZeroRemovesTheLimit = 'Poznámka: Nastavení těchto parametrů na 0 (nulu) odstraní omezení.';
$strZip = '"zazipováno"';
// To translate

?>
