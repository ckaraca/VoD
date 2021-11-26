<?php
/* $Id: db_details_db_info.php,v 1.7 2003/07/19 11:01:51 lem9 Exp $ */
// vim: expandtab sw=4 ts=4 sts=4:


// Check parameters

if (!defined('PMA_COMMON_LIB_INCLUDED')) {
    include('./libraries/common.lib.php');
}

PMA_checkParameters(array('db'));


/**
 * Gets the list of the table in the current db and informations about these
 * tables if possible
 */
// staybyte: speedup view on locked tables - 11 June 2001
$tables = array();
if (PMA_MYSQL_INT_VERSION >= 32303) {
    // Special speedup for newer MySQL Versions (in 4.0 format changed)
    if ($cfg['SkipLockedTables'] == TRUE && PMA_MYSQL_INT_VERSION >= 32330) {
        $local_query    = 'SHOW OPEN TABLES FROM ' . PMA_backquote($db);
        $db_info_result = PMA_mysql_query($local_query) or PMA_mysqlDie('', $local_query, '', $err_url_0);
        // Blending out tables in use
        if ($db_info_result != FALSE && mysql_num_rows($db_info_result) > 0) {
            while ($tmp = PMA_mysql_fetch_row($db_info_result)) {
                // if in use memorize tablename
                if (eregi('in_use=[1-9]+', $tmp[1])) {
                    $sot_cache[$tmp[0]] = TRUE;
                }
            }
            mysql_free_result($db_info_result);

            if (isset($sot_cache)) {
                $local_query    = 'SHOW TABLES FROM ' . PMA_backquote($db);
                $db_info_result = PMA_mysql_query($local_query) or PMA_mysqlDie('', $local_query, '', $err_url_0);
                if ($db_info_result != FALSE && mysql_num_rows($db_info_result) > 0) {
                    while ($tmp = PMA_mysql_fetch_row($db_info_result)) {
                        if (!isset($sot_cache[$tmp[0]])) {
                            $local_query = 'SHOW TABLE STATUS FROM ' . PMA_backquote($db) . ' LIKE \'' . addslashes($tmp[0]) . '\'';
                            $sts_result  = PMA_mysql_query($local_query) or PMA_mysqlDie('', $local_query, '', $err_url_0);
                            $sts_tmp     = PMA_mysql_fetch_array($sts_result);
                            $tables[]    = $sts_tmp;
                        } else { // table in use
                            $tables[]    = array('Name' => $tmp[0]);
                        }
                    }
                    mysql_free_result($db_info_result);
                    $sot_ready = TRUE;
                }
            }
        }
    }
    if (!isset($sot_ready)) {
        $local_query    = 'SHOW TABLE STATUS FROM ' . PMA_backquote($db);
        $db_info_result = PMA_mysql_query($local_query) or PMA_mysqlDie('', $local_query, '', $err_url_0);
        if ($db_info_result != FALSE && mysql_num_rows($db_info_result) > 0) {
            while ($sts_tmp = PMA_mysql_fetch_array($db_info_result)) {
                $tables[] = $sts_tmp;
            }
            mysql_free_result($db_info_result);
        }
    }
    $num_tables = (isset($tables) ? count($tables) : 0);
} // end if (PMA_MYSQL_INT_VERSION >= 32303)
else {
    $db_info_result = PMA_mysql_list_tables($db);
    $num_tables     = ($db_info_result) ? @mysql_numrows($db_info_result) : 0;
    for ($i = 0; $i < $num_tables; $i++) {
        $tables[] = PMA_mysql_tablename($db_info_result, $i);
    }
    mysql_free_result($db_info_result);
}


/**
 * Displays top menu links
 */
echo '<!-- Top menu links -->' . "\n";
require('./db_details_links.php');

?>
