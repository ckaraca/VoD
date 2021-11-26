<?php
/* $Id: mult_submits.inc.php,v 1.32 2003/09/27 19:30:07 lem9 Exp $ */
// vim: expandtab sw=4 ts=4 sts=4:


/**
 * Prepares the work and runs some other scripts if required
 */
if (!empty($submit_mult)
    && ($submit_mult != $strWithChecked)
    && (  !empty($selected_db) 
       || !empty($selected_tbl)
       || !empty($selected_fld)
       || !empty($rows_to_delete)
         )) {

    if (!empty($selected_db)) {
        $selected     = $selected_db;
        $what         = 'drop_db';
    } else if (!empty($selected_tbl)) {
        if ($submit_mult == $strPrintView) {
            include('./tbl_printview.php');
            exit();
        } else {
           $selected = $selected_tbl;
           switch ($submit_mult) {
               case 'drop_db':
                   $what = 'drop_db';
                   break;
               case $strDrop:
                   $what = 'drop_tbl';
                   break;
               case $strEmpty:
                   $what = 'empty_tbl';
                   break;
               case $strCheckTable:
                   unset($submit_mult);
                   $query_type = 'check_tbl';
                   $mult_btn   = $strYes;
                   break;
               case $strOptimizeTable:
                   unset($submit_mult);
                   $query_type = 'optimize_tbl';
                   $mult_btn   = $strYes;
                   break;
               case $strRepairTable:
                   unset($submit_mult);
                   $query_type = 'repair_tbl';
                   $mult_btn   = $strYes;
                   break;
               case $strAnalyzeTable:
                   unset($submit_mult);
                   $query_type = 'analyze_tbl';
                   $mult_btn   = $strYes;
                   break;
           } // end switch
        }
    } else if (!empty($selected_fld)) {
        $selected     = $selected_fld;
        if ($submit_mult == $strDrop) {
            $what     = 'drop_fld';
        } else {
            include('./tbl_alter.php');
            exit();
        }
    } else {
        $what = 'row_delete';
        $selected = $rows_to_delete;
    }
} // end if


/**
 * Displays the confirmation form if required
 */
if (!empty($submit_mult) && !empty($what)) {
    $js_to_run = 'functions.js';
    unset($message);
    if (!empty($table)) {
        include('./tbl_properties_common.php');
        $url_query .= '&amp;goto=tbl_properties.php&amp;back=tbl_properties.php';
        include('./tbl_properties_table_info.php');
    }
    elseif (!empty($db)) {
        include('./db_details_common.php');
        include('./db_details_db_info.php');
    }
    // Builds the query
    $full_query     = '';
    $selected_cnt   = count($selected);
    for ($i = 0; $i < $selected_cnt; $i++) {
        switch ($what) {
            case 'row_delete':
                $full_query .= htmlspecialchars(urldecode($selected[$i]))
                            . ';<br />';
                break;
            case 'drop_db':
                $full_query .= 'DROP DATABASE '
                            . PMA_backquote(htmlspecialchars(urldecode($selected[$i])))
                            . ';<br />';
                break;

            case 'drop_tbl':
                $full_query .= (empty($full_query) ? 'DROP TABLE ' : ', ')
                            . PMA_backquote(htmlspecialchars(urldecode($selected[$i])))
                            . (($i == $selected_cnt - 1) ? ';<br />' : '');
                break;

            case 'empty_tbl':
                if (PMA_MYSQL_INT_VERSION >= 40000) {
                    $full_query .= 'TRUNCATE ';
                } else {
                    $full_query .= 'DELETE FROM ';
                }
                $full_query .= PMA_backquote(htmlspecialchars(urldecode($selected[$i])))
                            . ';<br />';
                break;

            case 'drop_fld':
                if ($full_query == '') {
                    $full_query .= 'ALTER TABLE '
                                . PMA_backquote(htmlspecialchars($table))
                                . '<br />&nbsp;&nbsp;DROP '
                                . PMA_backquote(htmlspecialchars(urldecode($selected[$i])))
                                . ',';
                } else {
                    $full_query .= '<br />&nbsp;&nbsp;DROP '
                                . PMA_backquote(htmlspecialchars(urldecode($selected[$i])))
                                . ',';
                }
                if ($i == $selected_cnt-1) {
                    $full_query = ereg_replace(',$', ';<br />', $full_query);
                }
                break;
        } // end switch
    }

    // Displays the form
    echo $strDoYouReally . '&nbsp;:<br />' . "\n";
    echo '<tt>' . $full_query . '</tt>&nbsp;?<br/>' . "\n";
    ?>
<form action="<?php echo $action; ?>" method="post">
    <?php
    echo "\n";
    if (strpos(' ' . $action, 'db_details') == 1) {
        echo PMA_generate_common_hidden_inputs($db);
    } else if (strpos(' ' . $action, 'tbl_properties') == 1 
              || $what == 'row_delete') {
        echo PMA_generate_common_hidden_inputs($db,$table);
    } else  {
        echo PMA_generate_common_hidden_inputs();
    }
    for ($i = 0; $i < $selected_cnt; $i++) {
        echo '    <input type="hidden" name="selected[]" value="' . htmlspecialchars($selected[$i]) . '" />' . "\n";
    }
    ?>
    <input type="hidden" name="query_type" value="<?php echo $what; ?>" />
    <?php
    if ($what == 'row_delete') {
        echo '<input type="hidden" name="original_sql_query" value="' . $original_sql_query . '" />' . "\n";
        echo '<input type="hidden" name="original_pos" value="' . $original_pos . '" />' . "\n";
        echo '<input type="hidden" name="original_url_query" value="' . $original_url_query . '" />' . "\n";
    }
    ?>
    <input type="submit" name="mult_btn" value="<?php echo $strYes; ?>" />
    <input type="submit" name="mult_btn" value="<?php echo $strNo; ?>" />
</form>
    <?php
    echo"\n";

    include('./footer.inc.php');
    exit();
} // end if


/**
 * Executes the query
 */
else if ($mult_btn == $strYes) {

    if ($query_type == 'drop_db' || $query_type == 'drop_tbl' || $query_type == 'drop_fld') {
        include('./libraries/relation_cleanup.lib.php');
    }

    $sql_query      = '';
    $selected_cnt   = count($selected);
    for ($i = 0; $i < $selected_cnt; $i++) {
        switch ($query_type) {
            case 'row_delete':
                $a_query = urldecode($selected[$i]);
                break;

            case 'drop_db':
                PMA_relationsCleanupDatabase($selected[$i]);
                $a_query   = 'DROP DATABASE '
                           . PMA_backquote(urldecode($selected[$i]));
                $reload    = 1;
                break;

            case 'drop_tbl':
                PMA_relationsCleanupTable($db, $selected[$i]);
                $sql_query .= (empty($sql_query) ? 'DROP TABLE ' : ', ')
                           . PMA_backquote(urldecode($selected[$i]))
                           . (($i == $selected_cnt-1) ? ';' : '');
                $reload    = 1;
                break;

            case 'check_tbl':
                $sql_query .= (empty($sql_query) ? 'CHECK TABLE ' : ', ')
                           . PMA_backquote(urldecode($selected[$i]));
                break;

            case 'optimize_tbl':
                $sql_query .= (empty($sql_query) ? 'OPTIMIZE TABLE ' : ', ')
                           . PMA_backquote(urldecode($selected[$i]));
                break;

            case 'analyze_tbl':
                $sql_query .= (empty($sql_query) ? 'ANALYZE TABLE ' : ', ')
                           . PMA_backquote(urldecode($selected[$i]));
                break;

            case 'repair_tbl':
                $sql_query .= (empty($sql_query) ? 'REPAIR TABLE ' : ', ')
                           . PMA_backquote(urldecode($selected[$i]));
                break;

            case 'empty_tbl':
                if (PMA_MYSQL_INT_VERSION >= 40000) {
                    $a_query = 'TRUNCATE ';
                } else {
                    $a_query = 'DELETE FROM ';
                }
                $a_query .= PMA_backquote(htmlspecialchars(urldecode($selected[$i])));
                break;

            case 'drop_fld':
                PMA_relationsCleanupTable($db, $table, $selected[$i]);
                $sql_query .= (empty($sql_query) ? 'ALTER TABLE ' . PMA_backquote($table) : ',')
                           . ' DROP ' . PMA_backquote(urldecode($selected[$i]))
                           . (($i == $selected_cnt-1) ? ';' : '');
                break;
        } // end switch

        // All "DROP TABLE","DROP FIELD", "OPTIMIZE TABLE" and "REPAIR TABLE"
        // statements will be run at once below
        if ($query_type != 'drop_tbl'
            && $query_type != 'drop_fld'
            && $query_type != 'repair_tbl'
            && $query_type != 'analyze_tbl'
            && $query_type != 'optimize_tbl'
            && $query_type != 'check_tbl') {

            $sql_query .= $a_query . ';' . "\n";

            if ($query_type != 'drop_db') {
                PMA_mysql_select_db($db);
            }
            $result = @PMA_mysql_query($a_query) or PMA_mysqlDie('', $a_query, FALSE, $err_url);
        } // end if
    } // end for

    if ($query_type == 'drop_tbl'
        || $query_type == 'drop_fld') {
        PMA_mysql_select_db($db);
        $result = @PMA_mysql_query($sql_query) or PMA_mysqlDie('', '', FALSE, $err_url);
    } elseif ($query_type == 'repair_tbl'
        || $query_type == 'analyze_tbl'
        || $query_type == 'check_tbl'
        || $query_type == 'optimize_tbl') {
        include('./sql.php');
        exit();
    }

}

?>
