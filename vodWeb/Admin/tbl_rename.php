<?php
/* $Id: tbl_rename.php,v 1.27 2003/08/25 15:22:03 rabus Exp $ */
// vim: expandtab sw=4 ts=4 sts=4:


/**
 * Gets some core libraries
 */
require('./libraries/grab_globals.lib.php');
$js_to_run = 'functions.js';
require('./libraries/common.lib.php');

PMA_checkParameters(array('db','table'));

/**
 * Defines the url to return to in case of error in a sql statement
 */
$err_url = 'tbl_properties.php?' . PMA_generate_common_url($db, $table);


/**
 * A new name has been submitted -> do the work
 */
if (isset($new_name) && trim($new_name) != '') {
    $old_name     = $table;
    $table        = $new_name;

    // Ensure the target is valid
    if (count($dblist) > 0 && PMA_isInto($db, $dblist) == -1) {
        exit();
    }
    if (PMA_MYSQL_INT_VERSION < 32306) {
        PMA_checkReservedWords($new_name, $err_url);
    }

    include('./header.inc.php');
    PMA_mysql_select_db($db);
    $sql_query = 'ALTER TABLE ' . PMA_backquote($old_name) . ' RENAME ' . PMA_backquote($new_name);
    $result    = PMA_mysql_query($sql_query) or PMA_mysqlDie('', '', '', $err_url);
    $message   = sprintf($strRenameTableOK, htmlspecialchars($old_name), htmlspecialchars($table));
    $reload    = 1;

    // garvin: Move old entries from comments to new table
    include('./libraries/relation.lib.php');
    $cfgRelation = PMA_getRelationsParam();
    if ($cfgRelation['commwork']) {
        $remove_query = 'UPDATE ' . PMA_backquote($cfgRelation['column_info'])
                      . ' SET     table_name = \'' . PMA_sqlAddslashes($table) . '\''
                      . ' WHERE db_name  = \'' . PMA_sqlAddslashes($db) . '\''
                      . ' AND table_name = \'' . PMA_sqlAddslashes($old_name) . '\'';
        $rmv_rs    = PMA_query_as_cu($remove_query);
        unset($rmv_query);
    }

    if ($cfgRelation['displaywork']) {
        $table_query = 'UPDATE ' . PMA_backquote($cfgRelation['table_info'])
                        . ' SET     table_name = \'' . PMA_sqlAddslashes($table) . '\''
                        . ' WHERE db_name  = \'' . PMA_sqlAddslashes($db) . '\''
                        . ' AND table_name = \'' . PMA_sqlAddslashes($old_name) . '\'';
        $tb_rs    = PMA_query_as_cu($table_query);
        unset($table_query);
        unset($tb_rs);
    }

    if ($cfgRelation['relwork']) {
        $table_query = 'UPDATE ' . PMA_backquote($cfgRelation['relation'])
                        . ' SET     foreign_table = \'' . PMA_sqlAddslashes($table) . '\''
                        . ' WHERE foreign_db  = \'' . PMA_sqlAddslashes($db) . '\''
                        . ' AND foreign_table = \'' . PMA_sqlAddslashes($old_name) . '\'';
        $tb_rs    = PMA_query_as_cu($table_query);
        unset($table_query);
        unset($tb_rs);

        $table_query = 'UPDATE ' . PMA_backquote($cfgRelation['relation'])
                        . ' SET     master_table = \'' . PMA_sqlAddslashes($table) . '\''
                        . ' WHERE master_db  = \'' . PMA_sqlAddslashes($db) . '\''
                        . ' AND master_table = \'' . PMA_sqlAddslashes($old_name) . '\'';
        $tb_rs    = PMA_query_as_cu($table_query);
        unset($table_query);
        unset($tb_rs);
    }

    if ($cfgRelation['pdfwork']) {
        $table_query = 'UPDATE ' . PMA_backquote($cfgRelation['table_coords'])
                        . ' SET     table_name = \'' . PMA_sqlAddslashes($table) . '\''
                        . ' WHERE db_name  = \'' . PMA_sqlAddslashes($db) . '\''
                        . ' AND table_name = \'' . PMA_sqlAddslashes($old_name) . '\'';
        $tb_rs    = PMA_query_as_cu($table_query);
        unset($table_query);
        unset($tb_rs);
    }

}


/**
 * No new name for the table!
 */
else {
    include('./header.inc.php');
    PMA_mysqlDie($strTableEmpty, '', '', $err_url);
}


/**
 * Back to the calling script
 */
require('./tbl_properties_operations.php');
?>
