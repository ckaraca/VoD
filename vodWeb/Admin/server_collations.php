<?php
/* $Id: server_collations.php,v 1.2 2003/07/23 19:22:59 rabus Exp $ */
// vim: expandtab sw=4 ts=4 sts=4:


/**
 * Does the common work
 */
require('./server_common.inc.php');


/**
 * Displays the links
 */
require('./server_links.inc.php');


/**
 * Displays the sub-page heading
 */
echo '<h2>' . "\n"
   . '    ' . $strCharsetsAndCollations . "\n"
   . '</h2>' . "\n";


/**
 * Checks the MySQL version
 */
if (PMA_MYSQL_INT_VERSION < 40100) {
    // TODO: Some nice Message :-)
    include('./footer.inc.php');
    exit;
}


/**
 * Includes the required charset library
 */
require('./libraries/mysql_charsets.lib.php');


/**
 * Outputs the result
 */
echo '<table border="0">' . "\n"
   . '    <tr>' . "\n"
   . '        <td valign="top">' . "\n"
   . '            <table border="0">' . "\n"
   . '                <tr>' . "\n"
   . '                <th>' . "\n"
   . '                    ' . $strCollation . "\n"
   . '                </th>' . "\n"
   . '                <th>' . "\n"
   . '                    ' . $strDescription . "\n"
   . '                </th>' . "\n"
   . '            </tr>' . "\n";

reset($mysql_charsets);
reset($mysql_collations);
$i = 0;
$table_row_count = count($mysql_charsets) + $mysql_collations_count;

while (list(, $current_charset) = each($mysql_charsets)) {
    if ($i > $table_row_count / 2) {
        $i = 0;
        echo '            </table>' . "\n"
           . '        </td>' . "\n"
           . '        <td valign="top">' . "\n"
           . '            <table border="0">' . "\n"
           . '                <tr>' . "\n"
           . '                <th>' . "\n"
           . '                    ' . $strCollation . "\n"
           . '                </th>' . "\n"
           . '                <th>' . "\n"
           . '                    ' . $strDescription . "\n"
           . '                </th>' . "\n"
           . '            </tr>' . "\n";
    }
    $i++;
    echo '            <tr>' . "\n"
       . '                <td colspan="2" bgcolor="' . $cfg['ThBgcolor'] . '" align="right">' . "\n"
       . '                    &nbsp;<b>' . htmlspecialchars($current_charset) . '</b>' . "\n"
       . '                    (<i>' . htmlspecialchars($mysql_charsets_descriptions[$current_charset]) . '</i>)&nbsp;' . "\n"
       . '                </td>' . "\n"
       . '            </tr>' . "\n";
    $useBgcolorOne = TRUE;
    reset($mysql_collations[$current_charset]);
    while (list(, $current_collation) = each($mysql_collations[$current_charset])) {
        $i++;
        echo '            <tr>' . "\n"
           . '                <td bgcolor="' . ($mysql_default_collations[$current_charset] == $current_collation ? $cfg['BrowseMarkerColor'] : ($useBgcolorOne ? $cfg['BgcolorOne'] : $cfg['BgcolorTwo'])) . '">' . "\n"
           . '                    &nbsp;' . htmlspecialchars($current_collation) . '&nbsp;' . "\n"
           . '                </td>' . "\n"
           . '                <td bgcolor="' . ($useBgcolorOne ? $cfg['BgcolorOne'] : $cfg['BgcolorTwo']) . '">' . "\n"
           . '                    &nbsp;' . PMA_getCollationDescr($current_collation) . '&nbsp;' . "\n"
           . '                </td>' . "\n"
           . '            </tr>' . "\n";
        $useBgcolorOne = !$useBgcolorOne;
    }
}
unset($table_row_count);
echo '            </table>' . "\n"
   . '        </td>' . "\n"
   . '    </tr>' . "\n"
   . '</table>' . "\n";

require('./footer.inc.php');

?>
