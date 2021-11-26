<?php
/* $Id: display_tbl.lib.php,v 1.169.2.1 2003/10/02 18:30:14 lem9 Exp $ */
// vim: expandtab sw=4 ts=4 sts=4:

/**
 * Set of functions used to display the records returned by a sql query
 */

if (!defined('PMA_DISPLAY_TBL_LIB_INCLUDED')) {
    define('PMA_DISPLAY_TBL_LIB_INCLUDED', 1);

    /**
     * Defines the display mode to use for the results of a sql query
     *
     * It uses a synthetic string that contains all the required informations.
     * In this string:
     *   - the first two characters stand for the action to do while
     *     clicking on the "edit" link (eg 'ur' for update a row, 'nn' for no
     *     edit link...);
     *   - the next two characters stand for the action to do while
     *     clicking on the "delete" link (eg 'kp' for kill a process, 'nn' for
     *     no delete link...);
     *   - the next characters are boolean values (1/0) and respectively stand
     *     for sorting links, navigation bar, "insert a new row" link, the
     *     bookmark feature, the expand/collapse text/blob fields button and
     *     the "display printable view" option.
     *     Of course '0'/'1' means the feature won't/will be enabled.
     *
     * @param   string   the synthetic value for display_mode (see �1 a few
     *                   lines above for explanations)
     * @param   integer  the total number of rows returned by the sql query
     *                   without any programmatically appended "LIMIT" clause
     *                   (just a copy of $unlim_num_rows if it exists, else
     *                   computed inside this function)
     *
     * @return  array    an array with explicit indexes for all the display
     *                   elements
     *
     * @global  string   the database name
     * @global  string   the table name
     * @global  integer  the total number of rows returned by the sql query
     *                   without any programmatically appended "LIMIT" clause
     * @global  array    the properties of the fields returned by the query
     * @global  string   the url to return to in case of error in a sql
     *                   statement
     *
     * @access  private
     *
     * @see     PMA_displayTable()
     */
    function PMA_setDisplayMode(&$the_disp_mode, &$the_total)
    {
        global $db, $table;
        global $unlim_num_rows, $fields_meta;
        global $err_url;

        // 1. Initializes the $do_display array
        $do_display              = array();
        $do_display['edit_lnk']  = $the_disp_mode[0] . $the_disp_mode[1];
        $do_display['del_lnk']   = $the_disp_mode[2] . $the_disp_mode[3];
        $do_display['sort_lnk']  = (string) $the_disp_mode[4];
        $do_display['nav_bar']   = (string) $the_disp_mode[5];
        $do_display['ins_row']   = (string) $the_disp_mode[6];
        $do_display['bkm_form']  = (string) $the_disp_mode[7];
        $do_display['text_btn']  = (string) $the_disp_mode[8];
        $do_display['pview_lnk'] = (string) $the_disp_mode[9];

        // 2. Display mode is not "false for all elements" -> updates the
        // display mode
        if ($the_disp_mode != 'nnnn000000') {
            // 2.0 Print view -> set all elements to FALSE!
            if (isset($GLOBALS['printview']) && $GLOBALS['printview'] == '1') {
                $do_display['edit_lnk']  = 'nn'; // no edit link
                $do_display['del_lnk']   = 'nn'; // no delete link
                $do_display['sort_lnk']  = (string) '0';
                $do_display['nav_bar']   = (string) '0';
                $do_display['ins_row']   = (string) '0';
                $do_display['bkm_form']  = (string) '0';
                $do_display['text_btn']  = (string) '0';
                $do_display['pview_lnk'] = (string) '0';
            }
            // 2.1 Statement is a "SELECT COUNT", a
            //     "CHECK/ANALYZE/REPAIR/OPTIMIZE", an "EXPLAIN" one or
            //     contains a "PROC ANALYSE" part
            else if ($GLOBALS['is_count'] || $GLOBALS['is_analyse'] || $GLOBALS['is_maint'] || $GLOBALS['is_explain']) {
                $do_display['edit_lnk']  = 'nn'; // no edit link
                $do_display['del_lnk']   = 'nn'; // no delete link
                $do_display['sort_lnk']  = (string) '0';
                $do_display['nav_bar']   = (string) '0';
                $do_display['ins_row']   = (string) '0';
                $do_display['bkm_form']  = (string) '1';
                $do_display['text_btn']  = (string) '0';
                $do_display['pview_lnk'] = (string) '1';
            }
            // 2.2 Statement is a "SHOW..."
            else if ($GLOBALS['is_show']) {
                // 2.2.1 TODO : defines edit/delete links depending on show statement
                $tmp = eregi('^SHOW[[:space:]]+(VARIABLES|(FULL[[:space:]]+)?PROCESSLIST|STATUS|TABLE|GRANTS|CREATE|LOGS)', $GLOBALS['sql_query'], $which);
                if (strpos(' ' . strtoupper($which[1]), 'PROCESSLIST') > 0) {
                    $do_display['edit_lnk'] = 'nn'; // no edit link
                    $do_display['del_lnk']  = 'kp'; // "kill process" type edit link
                }
                else {
                    // Default case -> no links
                    $do_display['edit_lnk'] = 'nn'; // no edit link
                    $do_display['del_lnk']  = 'nn'; // no delete link
                }
                // 2.2.2 Other settings
                $do_display['sort_lnk']  = (string) '0';
                $do_display['nav_bar']   = (string) '0';
                $do_display['ins_row']   = (string) '0';
                $do_display['bkm_form']  = (string) '1';
                $do_display['text_btn']  = (string) '1';
                $do_display['pview_lnk'] = (string) '1';
            }
            // 2.3 Other statements (ie "SELECT" ones) -> updates
            //     $do_display['edit_lnk'], $do_display['del_lnk'] and
            //     $do_display['text_btn'] (keeps other default values)
            else {
                $prev_table = $fields_meta[0]->table;
                $do_display['text_btn']  = (string) '1';
                for ($i = 0; $i < $GLOBALS['fields_cnt']; $i++) {
                    $is_link = ($do_display['edit_lnk'] != 'nn'
                                || $do_display['del_lnk'] != 'nn'
                                || $do_display['sort_lnk'] != '0'
                                || $do_display['ins_row'] != '0');
                    // 2.3.2 Displays edit/delete/sort/insert links?
                    if ($is_link
                        && ($fields_meta[$i]->table == '' || $fields_meta[$i]->table != $prev_table)) {
                        $do_display['edit_lnk'] = 'nn'; // don't display links
                        $do_display['del_lnk']  = 'nn';
                        // TODO: May be problematic with same fields names in
                        //       two joined table.
                        // $do_display['sort_lnk'] = (string) '0';
                        $do_display['ins_row']  = (string) '0';
                        if ($do_display['text_btn'] == '1') {
                            break;
                        }
                    } // end if (2.3.2)
                    // 2.3.3 Always display print view link
                    $do_display['pview_lnk']    = (string) '1';
                    $prev_table = $fields_meta[$i]->table;
                } // end for
            } // end if..elseif...else (2.1 -> 2.3)
        } // end if (2)

        // 3. Gets the total number of rows if it is unknown
        if (isset($unlim_num_rows) && $unlim_num_rows != '') {
            $the_total = $unlim_num_rows;
        }
        else if (($do_display['nav_bar'] == '1' || $do_display['sort_lnk'] == '1')
                 && (!empty($db) && !empty($table))) {
            $the_total   = PMA_countRecords($db, $table, TRUE);
        }

        // 4. If navigation bar or sorting fields names urls should be
        //    displayed but there is only one row, change these settings to
        //    false
        if ($do_display['nav_bar'] == '1' || $do_display['sort_lnk'] == '1') {

            if (isset($unlim_num_rows) && $unlim_num_rows < 2) {
                // garvin: force display of navbar for vertical/horizontal display-choice.
                // $do_display['nav_bar']  = (string) '0';
                $do_display['sort_lnk'] = (string) '0';
            }

        } // end if (3)

        // 5. Updates the synthetic var
        $the_disp_mode = join('', $do_display);

        return $do_display;
    } // end of the 'PMA_setDisplayMode()' function


    /**
     * Displays a navigation bar to browse among the results of a sql query
     *
     * @param   integer  the offset for the "next" page
     * @param   integer  the offset for the "previous" page
     * @param   string   the url-encoded query
     *
     * @global  string   the current language
     * @global  string   the currect charset for MySQL
     * @global  integer  the server to use (refers to the number in the
     *                   configuration file)
     * @global  string   the database name
     * @global  string   the table name
     * @global  string   the url to go back in case of errors
     * @global  integer  the total number of rows returned by the sql query
     * @global  integer  the total number of rows returned by the sql query
     *                   without any programmatically appended "LIMIT" clause
     * @global  integer  the current position in results
     * @global  mixed    the maximum number of rows per page ('all' = no limit)
     * @global  string   the display mode (horizontal/vertical/horizontalflipped)
     * @global  integer  the number of row to display between two table headers
     * @global  boolean  whether to limit the number of displayed characters of
     *                   text type fields or not
     *
     * @access  private
     *
     * @see     PMA_displayTable()
     */
    function PMA_displayTableNavigation($pos_next, $pos_prev, $encoded_query)
    {
        global $lang, $convcharset, $server, $db, $table;
        global $goto;
        global $num_rows, $unlim_num_rows, $pos, $session_max_rows;
        global $disp_direction, $repeat_cells;
        global $dontlimitchars;
        ?>

<!-- Navigation bar -->
<table border="0">
<tr>
        <?php
        // Move to the beginning or to the previous page
        if ($pos > 0 && $session_max_rows != 'all') {
            // loic1: patch #474210 from Gosha Sakovich - part 1
            if ($GLOBALS['cfg']['NavigationBarIconic']) {
                $caption1 = '&lt;&lt;';
                $caption2 = '&nbsp;&lt;&nbsp;';
                $title1   = ' title="' . $GLOBALS['strPos1'] . '"';
                $title2   = ' title="' . $GLOBALS['strPrevious'] . '"';
            } else {
                $caption1 = $GLOBALS['strPos1'] . ' &lt;&lt;';
                $caption2 = $GLOBALS['strPrevious'] . ' &lt;';
                $title1   = '';
                $title2   = '';
            } // end if... else...
            ?>
    <td>
        <form action="sql.php" method="post">
            <?php echo PMA_generate_common_hidden_inputs($db, $table); ?>
            <input type="hidden" name="sql_query" value="<?php echo $encoded_query; ?>" />
            <input type="hidden" name="pos" value="0" />
            <input type="hidden" name="session_max_rows" value="<?php echo $session_max_rows; ?>" />
            <input type="hidden" name="disp_direction" value="<?php echo $disp_direction; ?>" />
            <input type="hidden" name="repeat_cells" value="<?php echo $repeat_cells; ?>" />
            <input type="hidden" name="goto" value="<?php echo $goto; ?>" />
            <input type="hidden" name="dontlimitchars" value="<?php echo $dontlimitchars; ?>" />
            <input type="submit" name="navig" value="<?php echo $caption1; ?>"<?php echo $title1; ?> />
        </form>
    </td>
    <td>
        <form action="sql.php" method="post">
            <?php echo PMA_generate_common_hidden_inputs($db, $table); ?>
            <input type="hidden" name="sql_query" value="<?php echo $encoded_query; ?>" />
            <input type="hidden" name="pos" value="<?php echo $pos_prev; ?>" />
            <input type="hidden" name="session_max_rows" value="<?php echo $session_max_rows; ?>" />
            <input type="hidden" name="disp_direction" value="<?php echo $disp_direction; ?>" />
            <input type="hidden" name="repeat_cells" value="<?php echo $repeat_cells; ?>" />
            <input type="hidden" name="goto" value="<?php echo $goto; ?>" />
            <input type="hidden" name="dontlimitchars" value="<?php echo $dontlimitchars; ?>" />
            <input type="submit" name="navig" value="<?php echo $caption2; ?>"<?php echo $title2; ?> />
        </form>
    </td>
            <?php
        } // end move back
        echo "\n";
        ?>
    <td>
        &nbsp;&nbsp;&nbsp;
    </td>
    <td align="center">
        <form action="sql.php" method="post"
            onsubmit="return (checkFormElementInRange(this, 'session_max_rows', 1) && checkFormElementInRange(this, 'pos', 0, <?php echo $unlim_num_rows - 1; ?>))">
            <?php echo PMA_generate_common_hidden_inputs($db, $table); ?>
            <input type="hidden" name="sql_query" value="<?php echo $encoded_query; ?>" />
            <input type="hidden" name="goto" value="<?php echo $goto; ?>" />
            <input type="hidden" name="dontlimitchars" value="<?php echo $dontlimitchars; ?>" />
            <input type="submit" name="navig" value="<?php echo $GLOBALS['strShow']; ?> :" />
            <input type="text" name="session_max_rows" size="3" value="<?php echo (($session_max_rows != 'all') ? $session_max_rows : $GLOBALS['cfg']['MaxRows']); ?>" class="textfield" onfocus="this.select()" />
            <?php echo $GLOBALS['strRowsFrom'] . "\n"; ?>
            <input type="text" name="pos" size="6" value="<?php echo (($pos_next >= $unlim_num_rows) ? 0 : $pos_next); ?>" class="textfield" onfocus="this.select()" />
            <br />
        <?php
        // Display mode (horizontal/vertical and repeat headers)
        $param1 = '            <select name="disp_direction">' . "\n"
                . '                <option value="horizontal"' . (($disp_direction == 'horizontal') ? ' selected="selected"': '') . '>' . $GLOBALS['strRowsModeHorizontal'] . '</option>' . "\n"
                . '                <option value="horizontalflipped"' . (($disp_direction == 'horizontalflipped') ? ' selected="selected"': '') . '>' . $GLOBALS['strRowsModeFlippedHorizontal'] . '</option>' . "\n"
                . '                <option value="vertical"' . (($disp_direction == 'vertical') ? ' selected="selected"': '') . '>' . $GLOBALS['strRowsModeVertical'] . '</option>' . "\n"
                . '            </select>' . "\n"
                . '           ';
        $param2 = '            <input type="text" size="3" name="repeat_cells" value="' . $repeat_cells . '" class="textfield" />' . "\n"
                . '           ';
        echo '    ' . sprintf($GLOBALS['strRowsModeOptions'], "\n" . $param1, "\n" . $param2) . "\n";
        ?>
        </form>
    </td>
    <td>
        &nbsp;&nbsp;&nbsp;
    </td>
        <?php
        // Move to the next page or to the last one
        if (($pos + $session_max_rows < $unlim_num_rows) && $num_rows >= $session_max_rows
            && $session_max_rows != 'all') {
            // loic1: patch #474210 from Gosha Sakovich - part 2
            if ($GLOBALS['cfg']['NavigationBarIconic']) {
                $caption3 = '&nbsp;&gt;&nbsp;';
                $caption4 = '&gt;&gt;';
                $title3   = ' title="' . $GLOBALS['strNext'] . '"';
                $title4   = ' title="' . $GLOBALS['strEnd'] . '"';
            } else {
                $caption3 = '&gt; ' . $GLOBALS['strNext'];
                $caption4 = '&gt;&gt; ' . $GLOBALS['strEnd'];
                $title3   = '';
                $title4   = '';
            } // end if... else...
            echo "\n";
            ?>
    <td>
        <form action="sql.php" method="post">
            <?php echo PMA_generate_common_hidden_inputs($db, $table); ?>
            <input type="hidden" name="sql_query" value="<?php echo $encoded_query; ?>" />
            <input type="hidden" name="pos" value="<?php echo $pos_next; ?>" />
            <input type="hidden" name="session_max_rows" value="<?php echo $session_max_rows; ?>" />
            <input type="hidden" name="disp_direction" value="<?php echo $disp_direction; ?>" />
            <input type="hidden" name="repeat_cells" value="<?php echo $repeat_cells; ?>" />
            <input type="hidden" name="goto" value="<?php echo $goto; ?>" />
            <input type="hidden" name="dontlimitchars" value="<?php echo $dontlimitchars; ?>" />
            <input type="submit" name="navig" value="<?php echo $caption3; ?>"<?php echo $title3; ?> />
        </form>
    </td>
    <td>
        <form action="sql.php" method="post"
            onsubmit="return <?php echo (($pos + $session_max_rows < $unlim_num_rows && $num_rows >= $session_max_rows) ? 'true' : 'false'); ?>">
            <?php echo PMA_generate_common_hidden_inputs($db, $table); ?>
            <input type="hidden" name="sql_query" value="<?php echo $encoded_query; ?>" />
            <input type="hidden" name="pos" value="<?php echo $unlim_num_rows - $session_max_rows; ?>" />
            <input type="hidden" name="session_max_rows" value="<?php echo $session_max_rows; ?>" />
            <input type="hidden" name="disp_direction" value="<?php echo $disp_direction; ?>" />
            <input type="hidden" name="repeat_cells" value="<?php echo $repeat_cells; ?>" />
            <input type="hidden" name="goto" value="<?php echo $goto; ?>" />
            <input type="hidden" name="dontlimitchars" value="<?php echo $dontlimitchars; ?>" />
            <input type="submit" name="navig" value="<?php echo $caption4; ?>"<?php echo $title4; ?> />
        </form>
    </td>
            <?php
        } // end move toward


        //page redirection
        $pageNow = @floor($pos / $session_max_rows) + 1;
        $nbTotalPage = @ceil($unlim_num_rows / $session_max_rows);

        if ($nbTotalPage > 1){ //if1
           ?>
       <td>
           &nbsp;&nbsp;&nbsp;
       </td>
       <td>
            <?php //<form> for keep the form alignment of button < and << ?>
            <form>
            <?php echo $GLOBALS['strPageNumber']; ?>
                <select name="goToPage" onChange="goToUrl(this, '<?php echo "sql.php?sql_query=".$encoded_query."&amp;session_max_rows=".$session_max_rows."&amp;disp_direction=".$disp_direction."&amp;repeat_cells=".$repeat_cells."&amp;goto=".$goto."&amp;dontlimitchars=".$dontlimitchars."&amp;".PMA_generate_common_url($db, $table)."&amp;"; ?>')">

           <?php
            if ($nbTotalPage < 200) {
                $firstPage = 1;
                $lastPage  = $nbTotalPage;
            } else {
                $range = 20;
                $firstPage = ($pageNow - $range < 1 ? 1 : $pageNow - $range);
                $lastPage  = ($pageNow + $range > $nbTotalPage ? $nbTotalPage : $pageNow + $range);
            }
            for ($i=$firstPage; $i<=$lastPage; $i++){
                if ($i == $pageNow) {
                    $selected = 'selected="selected"';
                } else {
                    $selected = "";
                }
                echo "                <option ".$selected." value=\"".(($i - 1) * $session_max_rows)."\">".$i."</option>\n";
            }
           ?>

                </select>
            </form>
        </td>
            <?php
        } //_if1


        // Show all the records if allowed
        if ($GLOBALS['cfg']['ShowAll'] && ($num_rows < $unlim_num_rows)) {
            echo "\n";
            ?>
    <td>
        &nbsp;&nbsp;&nbsp;
    </td>
    <td>
        <form action="sql.php" method="post">
            <?php echo PMA_generate_common_hidden_inputs($db, $table); ?>
            <input type="hidden" name="sql_query" value="<?php echo $encoded_query; ?>" />
            <input type="hidden" name="pos" value="0" />
            <input type="hidden" name="session_max_rows" value="all" />
            <input type="hidden" name="disp_direction" value="<?php echo $disp_direction; ?>" />
            <input type="hidden" name="repeat_cells" value="<?php echo $repeat_cells; ?>" />
            <input type="hidden" name="goto" value="<?php echo $goto; ?>" />
            <input type="hidden" name="dontlimitchars" value="<?php echo $dontlimitchars; ?>" />
            <input type="submit" name="navig" value="<?php echo $GLOBALS['strShowAll']; ?>" />
        </form>
    </td>
            <?php
        } // end show all
        echo "\n";
        ?>
</tr>
</table>

        <?php
    } // end of the 'PMA_displayTableNavigation()' function


    /**
     * Displays the headers of the results table
     *
     * @param   array    which elements to display
     * @param   array    the list of fields properties
     * @param   integer  the total number of fields returned by the sql query
     * @param   array    the analyzed query
     *
     * @return  boolean  always true
     *
     * @global  string   the current language
     * @global  string   the current charset for MySQL
     * @global  integer  the server to use (refers to the number in the
     *                   configuration file)
     * @global  string   the database name
     * @global  string   the table name
     * @global  string   the sql query
     * @global  string   the url to go back in case of errors
     * @global  integer  the total number of rows returned by the sql query
     * @global  integer  the current position in results
     * @global  integer  the maximum number of rows per page
     * @global  array    informations used with vertical display mode
     * @global  string   the display mode (horizontal/vertical/horizontalflipped)
     * @global  integer  the number of row to display between two table headers
     * @global  boolean  whether to limit the number of displayed characters of
     *                   text type fields or not
     *
     * @access  private
     *
     * @see     PMA_displayTable()
     */
    function PMA_displayTableHeaders(&$is_display, &$fields_meta, $fields_cnt = 0, $analyzed_sql = '')
    {
        global $lang, $convcharset, $server, $db, $table;
        global $goto;
        global $sql_query, $num_rows, $pos, $session_max_rows;
        global $vertical_display, $disp_direction, $repeat_cells, $highlight_columns;
        global $dontlimitchars;

        if ($analyzed_sql == '') {
            $analyzed_sql = array();
        }

        // can be result sorted?
        if ($is_display['sort_lnk'] == '1') {
            // Defines the url used to append/modify a sorting order
            // Nijel: This was originally done inside loop below, but I see
            //        no reason to do this for each column.
            if (eregi('(.*)([[:space:]]ORDER[[:space:]]*BY[[:space:]](.*))', $sql_query, $regs1)) {
                if (eregi('((.*)([[:space:]]ASC|[[:space:]]DESC)([[:space:]]|$))(.*)', $regs1[2], $regs2)) {
                    $unsorted_sql_query = trim($regs1[1] . ' ' . $regs2[5]);
                    $sql_order          = trim($regs2[1]);
                    eregi('(ORDER[[:space:]]*BY[[:space:]]*)(.*)([[:space:]]*ASC|[[:space:]]*DESC)',$sql_order,$after_order);
                    $sort_expression = trim($after_order[2]);
                }
                else if (eregi('((.*))[[:space:]]+(LIMIT (.*)|PROCEDURE (.*)|FOR UPDATE|LOCK IN SHARE MODE)', $regs1[2], $regs3)) {
                    $unsorted_sql_query = trim($regs1[1] . ' ' . $regs3[3]);
                    $sql_order          = trim($regs3[1]) . ' ASC';
                    eregi('(ORDER[[:space:]]*BY[[:space:]]*)(.*)([[:space:]]*ASC|[[:space:]]*DESC)',$sql_order,$after_order);
                    $sort_expression = trim($after_order[2]);
                } else {
                    $unsorted_sql_query = trim($regs1[1]);
                    $sql_order          = trim($regs1[2]) . ' ASC';
                    eregi('(ORDER[[:space:]]*BY[[:space:]]*)(.*)([[:space:]]*ASC|[[:space:]]*DESC)',$sql_order,$after_order);
                    $sort_expression = trim($after_order[2]);
                }
            } else {
                $unsorted_sql_query     = $sql_query;
            }

            // sorting by indexes

            // grab indexes data:
            $local_query = 'SHOW KEYS FROM ' . PMA_backquote($table);
            $result      = PMA_mysql_query($local_query) or PMA_mysqlDie('', $local_query, '', $err_url_0);
            $idx_cnt     = mysql_num_rows($result);

            $prev_index = '';
            for ($i = 0; $i < $idx_cnt; $i++) {
                $row = (defined('PMA_IDX_INCLUDED') ? $ret_keys[$i] : PMA_mysql_fetch_array($result));

                if ($row['Key_name'] != $prev_index ){
                    $indexes[]  = $row['Key_name'];
                    $prev_index = $row['Key_name'];
                }
                $indexes_info[$row['Key_name']]['Sequences'][]     = $row['Seq_in_index'];
                $indexes_info[$row['Key_name']]['Non_unique']      = $row['Non_unique'];
                if (isset($row['Cardinality'])) {
                    $indexes_info[$row['Key_name']]['Cardinality'] = $row['Cardinality'];
                }
            //    I don't know what does following column mean....
            //    $indexes_info[$row['Key_name']]['Packed']          = $row['Packed'];
                $indexes_info[$row['Key_name']]['Comment']         = (isset($row['Comment']))
                                                                   ? $row['Comment']
                                                                   : '';
                $indexes_info[$row['Key_name']]['Index_type']      = (isset($row['Index_type']))
                                                                   ? $row['Index_type']
                                                                   : '';

                $indexes_data[$row['Key_name']][$row['Seq_in_index']]['Column_name']  = $row['Column_name'];
                if (isset($row['Sub_part'])) {
                    $indexes_data[$row['Key_name']][$row['Seq_in_index']]['Sub_part'] = $row['Sub_part'];
                }
            } // end while

            // do we have any index?
            if (isset($indexes_data)) {

                if ($disp_direction == 'horizontal' || $disp_direction == 'horizontalflipped') {
                    $span = $fields_cnt;
                } else {
                    $span = $num_rows + floor($num_rows/$repeat_cells) + 1;
                }
                if ($is_display['edit_lnk'] != 'nn') $span++;
                if ($is_display['del_lnk'] != 'nn') $span++;
                    
                ?>
<tr>
    <td colspan="<?php echo $span; ?>" align="center">
                <?php
            echo '<form action="sql.php" method="POST">' . "\n";
            echo PMA_generate_common_hidden_inputs($db, $table, 5);
            echo '<input type="hidden" name="pos" value="' . $pos .  '" />' . "\n";
            echo '<input type="hidden" name="session_max_rows" value="' . $session_max_rows . '" />' . "\n";
            echo '<input type="hidden" name="disp_direction" value="' . $disp_direction . '" />' . "\n";
            echo '<input type="hidden" name="repeat_cells" value="' . $repeat_cells . '" />' . "\n";
            echo '<input type="hidden" name="dontlimitchars" value="' . $dontlimitchars . '" />' . "\n";
            echo $GLOBALS['strSortByKey'] . ': <select name="sql_query">&nbsp;';
            $used_index = false;
            $local_order = str_replace('  ', ' ', $sql_order);
            while (list($key, $val) = each($indexes_data)) {
                $asc_sort = 'ORDER BY ';
                $desc_sort = 'ORDER BY ';
                while (list($key2, $val2) = each($val)) {
                    $asc_sort .= PMA_backquote($val2['Column_name']) . ' ASC , ';
                    $desc_sort .= PMA_backquote($val2['Column_name']) . ' DESC , ';
                }
                $asc_sort = substr($asc_sort, 0, -3);
                $desc_sort = substr($desc_sort, 0, -3);
                $used_index = $used_index || $local_order == $asc_sort || $local_order == $desc_sort;
                echo '<option value="' . htmlspecialchars($unsorted_sql_query . ' ' . $asc_sort) . '"' . ($local_order == $asc_sort ? ' selected="selected"' : '') . '>' . htmlspecialchars($key) . ' (' . $GLOBALS['strAscending'] . ')</option>';
                echo "\n";
                echo '<option value="' . htmlspecialchars($unsorted_sql_query . ' ' . $desc_sort) . '"' . ($local_order == $desc_sort ? ' selected="selected"' : '') . '>' . htmlspecialchars($key) . ' (' . $GLOBALS['strDescending'] . ')</option>';
                echo "\n";
            }
            echo '<option value="' . htmlspecialchars($unsorted_sql_query) . '"' . ($used_index ? '' : ' selected="selected"' ) . '>' . $GLOBALS['strNone'] . '</option>';
            echo "\n";
            echo '</select>&nbsp;';
            echo "\n";
            echo '<input type="submit" value="' . $GLOBALS['strGo'] . '" />';
            echo "\n";
            echo '</form>';
            echo "\n";
            ?>
    </td>
</tr>
            <?php
            }
        }

        
        if ($disp_direction == 'horizontal' || $disp_direction == 'horizontalflipped') {
            ?>
<!-- Results table headers -->
<tr>
            <?php
            echo "\n";
        }

        $vertical_display['emptypre']   = 0;
        $vertical_display['emptyafter'] = 0;
        $vertical_display['textbtn']    = '';


        // Start of form for multi-rows delete

        if ($is_display['del_lnk'] == 'dr') {
            echo '<form method="post" action="tbl_row_delete.php" name="rowsDeleteForm">' . "\n";
            echo PMA_generate_common_hidden_inputs($db, $table, 1); 
        }

        // 1. Displays the full/partial text button (part 1)...
        if ($disp_direction == 'horizontal' || $disp_direction == 'horizontalflipped') {
            $colspan  = ($is_display['edit_lnk'] != 'nn' && $is_display['del_lnk'] != 'nn')
                      ? ' colspan="3"'
                      : '';
        } else {
            $rowspan  = ($is_display['edit_lnk'] != 'nn' && $is_display['del_lnk'] != 'nn')
                      ? ' rowspan="2"'
                      : '';
        }
        $text_url = 'sql.php?'
                  . PMA_generate_common_url($db, $table)
                  . '&amp;sql_query=' . urlencode($sql_query)
                  . '&amp;pos=' . $pos
                  . '&amp;session_max_rows=' . $session_max_rows
                  . '&amp;pos=' . $pos
                  . '&amp;disp_direction=' . $disp_direction
                  . '&amp;repeat_cells=' . $repeat_cells
                  . '&amp;goto=' . $goto
                  . '&amp;dontlimitchars=' . (($dontlimitchars) ? 0 : 1);

        //     ... before the result table
        if (($is_display['edit_lnk'] == 'nn' && $is_display['del_lnk'] == 'nn')
            && $is_display['text_btn'] == '1') {
            $vertical_display['emptypre'] = ($is_display['edit_lnk'] != 'nn' && $is_display['del_lnk'] != 'nn') ? 2 : 0;
            if ($disp_direction == 'horizontal' || $disp_direction == 'horizontalflipped') {
                ?>
    <td colspan="<?php echo $fields_cnt; ?>" align="center">
        <a href="<?php echo $text_url; ?>">
            <img src="./images/<?php echo (($dontlimitchars) ? 'partialtext' : 'fulltext'); ?>.png" border="0" width="50" height="20" alt="<?php echo (($dontlimitchars) ? $GLOBALS['strPartialText'] : $GLOBALS['strFullText']); ?>" title="<?php echo (($dontlimitchars) ? $GLOBALS['strPartialText'] : $GLOBALS['strFullText']); ?>" /></a>
    </td>
</tr>

<tr>
                <?php
            } // end horizontal/horizontalflipped mode
            else {
                echo "\n";
                ?>
<tr>
    <td colspan="<?php echo $num_rows + floor($num_rows/$repeat_cells) + 1; ?>" align="center">
        <a href="<?php echo $text_url; ?>">
            <img src="./images/<?php echo (($dontlimitchars) ? 'partialtext' : 'fulltext'); ?>.png" border="0" width="50" height="20" alt="<?php echo (($dontlimitchars) ? $GLOBALS['strPartialText'] : $GLOBALS['strFullText']); ?>" title="<?php echo (($dontlimitchars) ? $GLOBALS['strPartialText'] : $GLOBALS['strFullText']); ?>" /></a>
    </td>
</tr>
                <?php
            } // end vertical mode
        }

        //     ... at the left column of the result table header if possible
        //     and required
        else if ($GLOBALS['cfg']['ModifyDeleteAtLeft'] && $is_display['text_btn'] == '1') {
            $vertical_display['emptypre'] = ($is_display['edit_lnk'] != 'nn' && $is_display['del_lnk'] != 'nn') ? 2 : 0;
            if ($disp_direction == 'horizontal' || $disp_direction == 'horizontalflipped') {
                echo "\n";
                ?>
    <td<?php echo $colspan; ?> align="center">
        <a href="<?php echo $text_url; ?>">
            <img src="./images/<?php echo (($dontlimitchars) ? 'partialtext' : 'fulltext'); ?>.png" border="0" width="50" height="20" alt="<?php echo (($dontlimitchars) ? $GLOBALS['strPartialText'] : $GLOBALS['strFullText']); ?>" title="<?php echo (($dontlimitchars) ? $GLOBALS['strPartialText'] : $GLOBALS['strFullText']); ?>" /></a>
    </td>
                <?php
            } // end horizontal/horizontalflipped mode
            else {
                $vertical_display['textbtn'] = '    <td' . $rowspan . ' align="center" valign="middle">' . "\n"
                                             . '        <a href="' . $text_url . '">' . "\n"
                                             . '            <img src="./images/' . (($dontlimitchars) ? 'partialtext' : 'fulltext') . '.png" border="0" width="50" height="20" alt="' . (($dontlimitchars) ? $GLOBALS['strPartialText'] : $GLOBALS['strFullText']) . '" title="' . (($dontlimitchars) ? $GLOBALS['strPartialText'] : $GLOBALS['strFullText']) . '" /></a>' . "\n"
                                             . '    </td>' . "\n";
            } // end vertical mode
        }

        //     ... else if no button, displays empty(ies) col(s) if required
        else if ($GLOBALS['cfg']['ModifyDeleteAtLeft']
                 && ($is_display['edit_lnk'] != 'nn' || $is_display['del_lnk'] != 'nn')) {
            $vertical_display['emptypre'] = ($is_display['edit_lnk'] != 'nn' && $is_display['del_lnk'] != 'nn') ? 2 : 0;
            if ($disp_direction == 'horizontal' || $disp_direction == 'horizontalflipped') {
                echo "\n";
                ?>
    <td<?php echo $colspan; ?>></td>
                <?php
                echo "\n";
            } // end horizontal/horizontalfipped mode
            else {
                $vertical_display['textbtn'] = '    <td' . $rowspan . '></td>' . "\n";
            } // end vertical mode
        }

        // 2. Displays the fields' name
        // 2.0 If sorting links should be used, checks if the query is a "JOIN"
        //     statement (see 2.1.3)

        // 2.0.1 Prepare Display column comments if enabled ($cfg['ShowBrowseComments']).
        //       Do not show comments, if using horizontalflipped mode, because of space usage
        if ($GLOBALS['cfg']['ShowBrowseComments'] && $GLOBALS['cfgRelation']['commwork'] && $disp_direction != 'horizontalflipped') {
            $comments_map = PMA_getComments($db, $table);
        } else {
            $comments_map = array();
        }

        if ($GLOBALS['cfgRelation']['commwork'] && $GLOBALS['cfgRelation']['mimework'] && $GLOBALS['cfg']['BrowseMIME']) {
            require('./libraries/transformations.lib.php');
            $GLOBALS['mime_map'] = PMA_getMIME($db, $table);
        }


        if ($is_display['sort_lnk'] == '1') {
            $is_join = eregi('(.*)[[:space:]]+FROM[[:space:]]+.*[[:space:]]+JOIN', $sql_query, $select_stt);
        } else {
            $is_join = FALSE;
        }

        // garvin: See if we have to highlight any header fields of a WHERE query.
        //  Uses SQL-Parser results.
        $highlight_columns = array();
        if (isset($analyzed_sql) && isset($analyzed_sql[0]) &&
            isset($analyzed_sql[0]['where_clause_identifiers'])) {
            
            $wi = 0;
            @reset($analyzed_sql[0]['where_clause_identifiers']);
            while(list($wci_nr, $wci) = each($analyzed_sql[0]['where_clause_identifiers'])) {
                $highlight_columns[$wci] = 'true';
            }
        }
        for ($i = 0; $i < $fields_cnt; $i++) {
            // garvin: See if this column should get highlight because it's used in the
            //  where-query.
            if (isset($highlight_columns[$fields_meta[$i]->name]) || isset($highlight_columns[PMA_backquote($fields_meta[$i]->name)])) {
                $column_style = 'style="border: 1px solid ' . $GLOBALS['cfg']['BrowseMarkerColor'] . '"';
            } else {
                $column_style = '';
            }

            // 2.0 Prepare comment-HTML-wrappers for each row, if defined/enabled.
            if (isset($comments_map[$fields_meta[$i]->name])) {
                $comments_table_wrap_pre = '<table border="0" cellpadding="0" cellspacing="0"><tr><th>';
                $comments_table_wrap_post = '</th></tr><tr><th style="font-size: 8pt; font-weight: normal">' . htmlspecialchars($comments_map[$fields_meta[$i]->name]) . '</td></tr></table>';
            } else {
                $comments_table_wrap_pre = '';
                $comments_table_wrap_post = '';
            }
    
            // 2.1 Results can be sorted
            if ($is_display['sort_lnk'] == '1') {

                // 2.1.1 Checks if the current column is used to sort the
                //       results
                if (empty($sql_order)) {
                    $is_in_sort = FALSE;
                } else {
//                    $pattern = str_replace('\\', '\\\\', $fields_meta[$i]->name);
//                    $pattern = str_replace('(','\(', $pattern);
//                    $pattern = str_replace(')','\)', $pattern);
//                    $pattern = str_replace('*','\*', $pattern);

                    // field name may be preceded by a space, or any number
                    // of characters followed by a dot (tablename.fieldname)
//                    $is_in_sort = eregi('([[:space:]]|(.*\.))(`?)' . $pattern . '(`?)[ ,$]', $sql_order);
                    // instead of using eregi(), now do a direct comparison
                    // for the sort expression (avoids problems with queries
                    // like "SELECT id, count(id)..." and clicking to sort
                    // on id or on count(id) )
                      $is_in_sort = (PMA_backquote($fields_meta[$i]->name) == $sort_expression ? TRUE : FALSE);
                }
                // 2.1.2 Checks if the table name is required (it's the case
                //       for a query with a "JOIN" statement and if the column
                //       isn't aliased)
                if ($is_join
                    && !eregi('([^[:space:],]|`[^`]`)[[:space:]]+(as[[:space:]]+)?' . $fields_meta[$i]->name, $select_stt[1], $parts)) {
                    $sort_tbl = PMA_backquote($fields_meta[$i]->table) . '.';
                } else {
                    $sort_tbl = '';
                }
                // 2.1.3 Check the field name for backquotes.
                //       If it contains some, it's probably a function column
                //       like 'COUNT(`field`)'
                if (strpos(' ' . $fields_meta[$i]->name, '`') > 0) {
                    $sort_order = ' ORDER BY \'' . $fields_meta[$i]->name . '\' ';
                } else {
                    $sort_order = ' ORDER BY ' . $sort_tbl . PMA_backquote($fields_meta[$i]->name) . ' ';
                }
                // 2.1.4 Do define the sorting url
                if (!$is_in_sort) {
                    // loic1: patch #455484 ("Smart" order)
                    $cfg['Order']  = strtoupper($GLOBALS['cfg']['Order']);
                    if ($cfg['Order'] == 'SMART') {
                        $cfg['Order'] = (eregi('time|date', $fields_meta[$i]->type)) ? 'DESC' : 'ASC';
                    }
                    $sort_order .= $cfg['Order'];
                    $order_img   = '';
                }
                else if (eregi('[[:space:]]ASC$', $sql_order)) {
                    $sort_order .= ' DESC';
                    $order_img   = '&nbsp;<img src="./images/asc_order.png" border="0" width="7" height="7" alt="'. $GLOBALS['strAscending'] . '" title="'. $GLOBALS['strAscending'] . '" />';
                }
                else if (eregi('[[:space:]]DESC$', $sql_order)) {
                    $sort_order .= ' ASC';
                    $order_img   = '&nbsp;<img src="./images/desc_order.png" border="0" width="7" height="7" alt="'. $GLOBALS['strDescending'] . '" title="'. $GLOBALS['strDescending'] . '" />';
                }
                if (eregi('(.*)([[:space:]](LIMIT (.*)|PROCEDURE (.*)|FOR UPDATE|LOCK IN SHARE MODE))', $unsorted_sql_query, $regs3)) {
                    $sorted_sql_query = $regs3[1] . $sort_order . $regs3[2];
                } else {
                    $sorted_sql_query = $unsorted_sql_query . $sort_order;
                }
                $url_query = PMA_generate_common_url($db, $table)
                           . '&amp;pos=' . $pos
                           . '&amp;session_max_rows=' . $session_max_rows
                           . '&amp;disp_direction=' . $disp_direction
                           . '&amp;repeat_cells=' . $repeat_cells
                           . '&amp;dontlimitchars=' . $dontlimitchars
                           . '&amp;sql_query=' . urlencode($sorted_sql_query);

                // 2.1.5 Displays the sorting url
                if ($disp_direction == 'horizontal' || $disp_direction == 'horizontalflipped') {
                    echo "\n";
                    ?>
    <th <?php echo $column_style; ?> <?php if ($disp_direction == 'horizontalflipped') echo 'valign="bottom"'; ?>>
        <?php echo $comments_table_wrap_pre; ?>
        <a href="sql.php?<?php echo $url_query; ?>" <?php echo (($disp_direction == 'horizontalflipped' AND $GLOBALS['cfg']['HeaderFlipType'] == 'css') ? 'style="direction: ltr; writing-mode: tb-rl;"' : '') . ' title="' . $GLOBALS['strSort'] . '"'; ?>>
            <?php echo ($disp_direction == 'horizontalflipped'  && $GLOBALS['cfg']['HeaderFlipType'] == 'fake' ? PMA_flipstring(htmlspecialchars($fields_meta[$i]->name), "<br />\n") : htmlspecialchars($fields_meta[$i]->name)); ?> </a><?php echo $order_img . "\n"; ?>
        <?php echo $comments_table_wrap_post; ?>
    </th>
                    <?php
                }
                $vertical_display['desc'][] = '    <th ' . $column_style . '>' . "\n"
                                            . $comments_table_wrap_pre
                                            . '        <a href="sql.php?' . $url_query . '">' . "\n"
                                            . '            ' . htmlspecialchars($fields_meta[$i]->name) . '</a>' . $order_img . "\n"
                                            . $comments_table_wrap_post
                                            . '    </th>' . "\n";
            } // end if (2.1)

            // 2.2 Results can't be sorted
            else {
                if ($disp_direction == 'horizontal' || $disp_direction == 'horizontalflipped') {
                    echo "\n";
                    ?>
    <th <?php echo $column_style; ?> <?php if ($disp_direction == 'horizontalflipped') echo 'valign="bottom"'; ?>  <?php echo ($disp_direction == 'horizontalflipped' && $GLOBALS['cfg']['HeaderFlipType'] == 'css' ? 'style="direction: ltr; writing-mode: tb-rl;"' : ''); ?>>
        <?php echo $comments_table_wrap_pre; ?>
        <?php echo ($disp_direction == 'horizontalflipped' && $GLOBALS['cfg']['HeaderFlipType'] == 'fake'? PMA_flipstring(htmlspecialchars($fields_meta[$i]->name), "<br />\n") : htmlspecialchars($fields_meta[$i]->name)) . "\n"; ?>
        <?php echo $comments_table_wrap_post; ?>
    </th>
                    <?php
                }
                $vertical_display['desc'][] = '    <th ' . $column_style . '>' . "\n"
                                            . $comments_table_wrap_pre
                                            . '        ' . htmlspecialchars($fields_meta[$i]->name) . "\n"
                                            . $comments_table_wrap_post
                                            . '    </th>';
            } // end else (2.2)
        } // end for

        // 3. Displays the full/partial text button (part 2) at the right
        //    column of the result table header if possible and required...
        if ($GLOBALS['cfg']['ModifyDeleteAtRight']
            && ($is_display['edit_lnk'] != 'nn' || $is_display['del_lnk'] != 'nn')
            && $is_display['text_btn'] == '1') {
            $vertical_display['emptyafter'] = ($is_display['edit_lnk'] != 'nn' && $is_display['del_lnk'] != 'nn') ? 2 : 1;
            if ($disp_direction == 'horizontal' || $disp_direction == 'horizontalflipped') {
                echo "\n";
                ?>
    <td<?php echo $colspan; ?> align="center">
        <a href="<?php echo $text_url; ?>">
            <img src="./images/<?php echo (($dontlimitchars) ? 'partialtext' : 'fulltext'); ?>.png" border="0" width="50" height="20" alt="<?php echo (($dontlimitchars) ? $GLOBALS['strPartialText'] : $GLOBALS['strFullText']); ?>" title="<?php echo (($dontlimitchars) ? $GLOBALS['strPartialText'] : $GLOBALS['strFullText']); ?>" /></a>
    </td>
                <?php
            } // end horizontal/horizontalflipped mode
            else {
                $vertical_display['textbtn'] = '    <td' . $rowspan . ' align="center" valign="middle">' . "\n"
                                             . '        <a href="' . $text_url . '">' . "\n"
                                             . '            <img src="./images/' . (($dontlimitchars) ? 'partialtext' : 'fulltext') . '.png" border="0" width="50" height="20" alt="' . (($dontlimitchars) ? $GLOBALS['strPartialText'] : $GLOBALS['strFullText']) . '" title="' . (($dontlimitchars) ? $GLOBALS['strPartialText'] : $GLOBALS['strFullText']) . '" /></a>' . "\n"
                                             . '    </td>' . "\n";
            } // end vertical mode
        }

        //     ... else if no button, displays empty cols if required
        // (unless coming from Browse mode print view)
        else if ($GLOBALS['cfg']['ModifyDeleteAtRight']
                 && ($is_display['edit_lnk'] == 'nn' && $is_display['del_lnk'] == 'nn')
                 && (!$GLOBALS['is_header_sent'])) {
            $vertical_display['emptyafter'] = ($is_display['edit_lnk'] != 'nn' && $is_display['del_lnk'] != 'nn') ? 2 : 1;
            if ($disp_direction == 'horizontal' || $disp_direction == 'horizontalflipped') {
                echo "\n";
                ?>
    <td<?php echo $colspan; ?>></td>
                <?php
            } // end horizontal/horizontalflipped mode
            else {
                $vertical_display['textbtn'] = '    <td' . $rowspan . '></td>' . "\n";
            } // end vertical mode
        }

        if ($disp_direction == 'horizontal' || $disp_direction == 'horizontalflipped') {
            echo "\n";
            ?>
</tr>
            <?php
        }
        echo "\n";

        return TRUE;
    } // end of the 'PMA_displayTableHeaders()' function



    /**
     * Displays the body of the results table
     *
     * @param   integer  the link id associated to the query which results have
     *                   to be displayed
     * @param   array    which elements to display
     * @param   array    the list of relations
     * @param   array    the analyzed query
     *
     * @return  boolean  always true
     *
     * @global  string   the current language
     * @global  string   the current charset for MySQL
     * @global  integer  the server to use (refers to the number in the
     *                   configuration file)
     * @global  string   the database name
     * @global  string   the table name
     * @global  string   the sql query
     * @global  string   the url to go back in case of errors
     * @global  integer  the current position in results
     * @global  integer  the maximum number of rows per page
     * @global  array    the list of fields properties
     * @global  integer  the total number of fields returned by the sql query
     * @global  array    informations used with vertical display mode
     * @global  string   the display mode (horizontal/vertical/horizontalflipped)
     * @global  integer  the number of row to display between two table headers
     * @global  boolean  whether to limit the number of displayed characters of
     *                   text type fields or not
     *
     * @access  private
     *
     * @see     PMA_displayTable()
     */
    function PMA_displayTableBody(&$dt_result, &$is_display, $map, $analyzed_sql)
    {
        global $lang, $convcharset, $server, $db, $table;
        global $goto;
        global $sql_query, $pos, $session_max_rows, $fields_meta, $fields_cnt;
        global $vertical_display, $disp_direction, $repeat_cells, $highlight_columns;
        global $dontlimitchars;

        if (!is_array($map)) {
            $map = array();
        }
        ?>
<!-- Results table body -->
        <?php
        echo "\n";

        $row_no                     = 0;
        $vertical_display['edit']   = array();
        $vertical_display['delete'] = array();
        $vertical_display['data']   = array();

        // Correction uva 19991216 in the while below
        // Previous code assumed that all tables have keys, specifically that
        // the phpMyAdmin GUI should support row delete/edit only for such
        // tables.
        // Although always using keys is arguably the prescribed way of
        // defining a relational table, it is not required. This will in
        // particular be violated by the novice.
        // We want to encourage phpMyAdmin usage by such novices. So the code
        // below has been changed to conditionally work as before when the
        // table being displayed has one or more keys; but to display
        // delete/edit options correctly for tables without keys.

        // loic1: use 'PMA_mysql_fetch_array' rather than 'PMA_mysql_fetch_row'
        //        to get the NULL values

        while ($row = PMA_mysql_fetch_array($dt_result)) {

            // lem9: "vertical display" mode stuff
            if (($row_no != 0) && ($repeat_cells != 0) && !($row_no % $repeat_cells) && ($disp_direction == 'horizontal' || $disp_direction == 'horizontalflipped')) {
                echo '<tr>' . "\n";

                for ($foo_i = 0; $foo_i < $vertical_display['emptypre']; $foo_i++) {
                    echo '    <td>&nbsp;</td>' . "\n";
                }

                reset($vertical_display['desc']);
                while (list($key, $val) = each($vertical_display['desc'])) {
                    echo $val;
                }

                for ($foo_i = 0; $foo_i < $vertical_display['emptyafter']; $foo_i++) {
                    echo '    <td>&nbsp;</td>' . "\n";
                }

                echo '</tr>' . "\n";
            } // end if

            if (isset($GLOBALS['printview']) && ($GLOBALS['printview'] == '1')) {
                $bgcolor = '#ffffff';
            } else {
                $bgcolor = ($row_no % 2) ? $GLOBALS['cfg']['BgcolorOne'] : $GLOBALS['cfg']['BgcolorTwo'];
            }

            if ($disp_direction == 'horizontal' || $disp_direction == 'horizontalflipped') {
                // loic1: pointer code part
                $on_mouse     = '';
                if (!isset($GLOBALS['printview']) || ($GLOBALS['printview'] != '1')) {
                    if ($GLOBALS['cfg']['BrowsePointerColor'] != '') {
                        $on_mouse = ' onmouseover="setPointer(this, ' . $row_no . ', \'over\', \'' . $bgcolor . '\', \'' . $GLOBALS['cfg']['BrowsePointerColor'] . '\', \'' . $GLOBALS['cfg']['BrowseMarkerColor'] . '\');"'
                                  . ' onmouseout="setPointer(this, ' . $row_no . ', \'out\', \'' . $bgcolor . '\', \'' . $GLOBALS['cfg']['BrowsePointerColor'] . '\', \'' . $GLOBALS['cfg']['BrowseMarkerColor'] . '\');"';
                    }
                    if ($GLOBALS['cfg']['BrowseMarkerColor'] != '') {
                        $on_mouse .= ' onmousedown="setPointer(this, ' . $row_no . ', \'click\', \'' . $bgcolor . '\', \'' . $GLOBALS['cfg']['BrowsePointerColor'] . '\', \'' . $GLOBALS['cfg']['BrowseMarkerColor'] . '\');"';
                    }
                } // end if
                ?>
<tr<?php echo $on_mouse; ?>>
                <?php
                echo "\n";
            }

            // 1. Prepares the row (gets primary keys to use)
            if ($is_display['edit_lnk'] != 'nn' || $is_display['del_lnk'] != 'nn') {
                $primary_key              = '';
                $unique_key               = '';
                $uva_nonprimary_condition = '';

                // 1.1 Results from a "SELECT" statement -> builds the
                //     "primary" key to use in links
                if ($is_display['edit_lnk'] == 'ur' /* || $is_display['edit_lnk'] == 'dr' */) {
                    for ($i = 0; $i < $fields_cnt; ++$i) {
                        $field_flags = PMA_mysql_field_flags($dt_result, $i);
                        $meta      = $fields_meta[$i];

                        // do not use an alias in a condition
                        $column_for_condition = $meta->name;
                        reset($analyzed_sql[0]['select_expr']);
                        while (list ($select_expr_position, $select_expr) = each ($analyzed_sql[0]['select_expr'])) {
                            $alias = $analyzed_sql[0]['select_expr'][$select_expr_position]['alias'];
                            if (!empty($alias)) {
                                $true_column = $analyzed_sql[0]['select_expr'][$select_expr_position]['column'];
                                if ($alias == $meta->name) {
                                    $column_for_condition = $true_column;
                                } // end if
                            } // end if
                        } // end while

                        // to fix the bug where float fields (primary or not)
                        // can't be matched because of the imprecision of
                        // floating comparison, use CONCAT
                        // (also, the syntax "CONCAT(field) IS NULL"
                        // that we need on the next "if" will work)
                        if ($meta->type == 'real') {
                            $condition = ' CONCAT(' . PMA_backquote($column_for_condition) . ') ';
                        } else {
                            $condition = ' ' . PMA_backquote($column_for_condition) . ' ';
                        } // end if... else...

                        // loic1: To fix bug #474943 under php4, the row
                        //        pointer will depend on whether the "is_null"
                        //        php4 function is available or not
                        $pointer = (function_exists('is_null') ? $i : $meta->name);
                        if (!isset($row[$meta->name])
                            || (function_exists('is_null') && is_null($row[$pointer]))) {
                            $condition .= 'IS NULL AND';
                        } else {
                            if ($meta->type == 'blob'
                                // hexify only if this is a true not empty BLOB
                                 && eregi('BINARY', $field_flags)
                                 && !empty($row[$pointer])) {
                                    $condition .= 'LIKE 0x' . bin2hex($row[$pointer]). ' AND';
                            } else {
                                $condition .= '= \'' . PMA_sqlAddslashes($row[$pointer], FALSE, TRUE) . '\' AND';
                            }
                        }
                        if ($meta->primary_key > 0) {
                            $primary_key .= $condition;
                        } else if ($meta->unique_key > 0) {
                            $unique_key  .= $condition;
                        }
                        $uva_nonprimary_condition .= $condition;
                    } // end for

                    // Correction uva 19991216: prefer primary or unique keys
                    // for condition, but use conjunction of all values if no
                    // primary key
                    if ($primary_key) {
                        $uva_condition = $primary_key;
                    } else if ($unique_key) {
                        $uva_condition = $unique_key;
                    } else {
                        $uva_condition = $uva_nonprimary_condition;
                    }
                    
                    $uva_condition     = urlencode(preg_replace('|\s?AND$|', '', $uva_condition));
                } // end if (1.1)

                // 1.2 Defines the urls for the modify/delete link(s)
                $url_query  = PMA_generate_common_url($db, $table)
                            . '&amp;pos=' . $pos
                            . '&amp;session_max_rows=' . $session_max_rows
                            . '&amp;disp_direction=' . $disp_direction
                            . '&amp;repeat_cells=' . $repeat_cells
                            . '&amp;dontlimitchars=' . $dontlimitchars;

                // We need to copy the value or else the == 'both' check will always return true
                $propicon = (string)$GLOBALS['cfg']['PropertiesIconic'];

                if ($propicon == 'both') {
                    $iconic_spacer = '<nobr>';
                } else {
                    $iconic_spacer = '';
                }

                // 1.2.1 Modify link(s)
                if ($is_display['edit_lnk'] == 'ur') { // update row case
//                    $lnk_goto = 'sql.php'
//                             . '?' . str_replace('&amp;', '&', $url_query)
//                              . '&sql_query=' . urlencode($sql_query)
//                              . '&goto=' . (empty($goto) ? 'tbl_properties.php' : $goto);
// to reduce the length of the URL, because of some browsers limitations:
                    $lnk_goto = 'sql.php';

                    $edit_url = 'tbl_change.php'
                              . '?' . $url_query
                              . '&amp;primary_key=' . $uva_condition
                              . '&amp;sql_query=' . urlencode($sql_query)
                              . '&amp;goto=' . urlencode($lnk_goto);
                    if ($GLOBALS['cfg']['PropertiesIconic'] == FALSE) {
                        $edit_str = $GLOBALS['strEdit'];
                    } else {
                        $edit_str = $iconic_spacer . '<img width="12" height="13" src="images/button_edit.png" alt="' . $GLOBALS['strEdit'] . '" title="' . $GLOBALS['strEdit'] . '" border="0" />';
                        if ($propicon == 'both') {
                            $edit_str .= '&nbsp;' . $GLOBALS['strEdit'] . '</nobr>';
                        }
                    }
                } // end if (1.2.1)

                if ($table == $GLOBALS['cfg']['Bookmark']['table'] && $db == $GLOBALS['cfg']['Bookmark']['db']) {
                    $bookmark_go = '<form method="post" action="read_dump.php">'
                                    . PMA_generate_common_hidden_inputs($row['dbase'], '')
                                    . '<input type="hidden" name="id_bookmark" value="' . $row['id'] . '" />'
                                    . '<input type="hidden" name="action_bookmark" value="0" />'
                                    . '<input type="submit" name="SQL" value="' . $GLOBALS['strExecuteBookmarked'] . '" />'
                                    . '</form>';
                } else { 
                    $bookmark_go = '';
                }

                // 1.2.2 Delete/Kill link(s)
                if ($is_display['del_lnk'] == 'dr') { // delete row case
                    $lnk_goto = 'sql.php'
                              . '?' . str_replace('&amp;', '&', $url_query)
                              . '&sql_query=' . urlencode($sql_query)
                              . '&zero_rows=' . urlencode(htmlspecialchars($GLOBALS['strDeleted']))
                              . '&goto=' . (empty($goto) ? 'tbl_properties.php' : $goto);
                    $del_query = urlencode('DELETE FROM ' . PMA_backquote($table) . ' WHERE') . $uva_condition . ((PMA_MYSQL_INT_VERSION >= 32207) ? urlencode(' LIMIT 1') : '');
                    $del_url  = 'sql.php'
                              . '?' . $url_query
                              . '&amp;sql_query=' . $del_query 
                              . '&amp;zero_rows=' . urlencode(htmlspecialchars($GLOBALS['strDeleted']))
                              . '&amp;goto=' . urlencode($lnk_goto);
                    $js_conf  = 'DELETE FROM ' . PMA_jsFormat($table)
                              . ' WHERE ' . trim(PMA_jsFormat(urldecode($uva_condition), FALSE))
                              . ((PMA_MYSQL_INT_VERSION >= 32207) ? ' LIMIT 1' : '');
                    if ($GLOBALS['cfg']['PropertiesIconic'] == FALSE) {
                        $del_str = $GLOBALS['strDelete'];
                    } else {
                        $del_str = $iconic_spacer . '<img width="12" height="13" src="images/button_drop.png" alt="' . $GLOBALS['strDelete'] . '" title="' . $GLOBALS['strDelete'] . '" border="0" />';
                        if ($propicon == 'both') {
                            $del_str .= '&nbsp;' . $GLOBALS['strDelete'] . '</nobr>';
                        }
                    }
                } else if ($is_display['del_lnk'] == 'kp') { // kill process case
                    $lnk_goto = 'sql.php'
                              . '?' . str_replace('&amp;', '&', $url_query)
                              . '&sql_query=' . urlencode($sql_query)
                              . '&goto=main.php';
                    $del_url  = 'sql.php?'
                              . PMA_generate_common_url('mysql')
                              . '&amp;sql_query=' . urlencode('KILL ' . $row['Id'])
                              . '&amp;goto=' . urlencode($lnk_goto);
                    $js_conf  = 'KILL ' . $row['Id'];
                    if ($GLOBALS['cfg']['PropertiesIconic'] == FALSE) {
                        $del_str = $GLOBALS['strKill'];
                    } else {
                        $del_str = $iconic_spacer . '<img width="12" height="13" src="images/button_drop.png" alt="' . $GLOBALS['strKill'] . '" title="' . $GLOBALS['strKill'] . '" border="0" />';
                        if ($propicon == 'both') {
                            $del_str .= '&nbsp;' . $GLOBALS['strKill'] . '</nobr>';
                        }
                    }
                } // end if (1.2.2)

                // 1.3 Displays the links at left if required
                if ($GLOBALS['cfg']['ModifyDeleteAtLeft']
                    && ($disp_direction == 'horizontal' || $disp_direction == 'horizontalflipped')) {
                    include('./libraries/display_tbl_links.lib.php');
                } // end if (1.3)
                echo (($disp_direction == 'horizontal' || $disp_direction == 'horizontalflipped') ? "\n" : '');
            } // end if (1)

            // 2. Displays the rows' values
            for ($i = 0; $i < $fields_cnt; ++$i) {
                $meta    = $fields_meta[$i];
                // loic1: To fix bug #474943 under php4, the row pointer will
                //        depend on whether the "is_null" php4 function is
                //        available or not
                $pointer = (function_exists('is_null') ? $i : $meta->name);
                
                // garvin: See if this column should get highlight because it's used in the
                //  where-query.
                if (isset($highlight_columns) && (isset($highlight_columns[$meta->name]) || isset($highlight_columns[PMA_backquote($meta->name)]))) {
                    $column_style = 'style="border: 1px solid ' . $GLOBALS['cfg']['BrowseMarkerColor'] . '"';
                } else {
                    $column_style = '';
                }

                // garvin: Wrap MIME-transformations. [MIME]
                $default_function = 'default_function'; // default_function
                $transform_function = $default_function;
                $transform_options = array();

                if ($GLOBALS['cfgRelation']['mimework'] && $GLOBALS['cfg']['BrowseMIME']) {

                    if (isset($GLOBALS['mime_map'][$meta->name]['mimetype']) && isset($GLOBALS['mime_map'][$meta->name]['transformation']) && !empty($GLOBALS['mime_map'][$meta->name]['transformation'])) {
                        // garvin: for security, never allow to break out from transformations directory
                        $include_file = eregi_replace('\.\.*', '.', $GLOBALS['mime_map'][$meta->name]['transformation']);

                        if (file_exists('./libraries/transformations/' . $include_file)) {
                            $transformfunction_name = str_replace('.inc.php', '', $GLOBALS['mime_map'][$meta->name]['transformation']);
    
                            include('./libraries/transformations/' . $include_file);
    
                            if (defined('PMA_TRANSFORMATION_' . strtoupper($transformfunction_name)) && function_exists('PMA_transformation_' . $transformfunction_name)) {
                                $transform_function = 'PMA_transformation_' . $transformfunction_name;
                                $transform_options = PMA_transformation_getOptions((isset($GLOBALS['mime_map'][$meta->name]['transformation_options']) ? $GLOBALS['mime_map'][$meta->name]['transformation_options'] : ''));
                            }
                        } // end if file_exists
                    } // end if transformation is set
                } // end if mime/transformation works.

                $transform_options['wrapper_link'] = '?'
                                                    . (isset($url_query) ? $url_query : '')
                                                    . '&amp;primary_key=' . (isset($uva_condition) ? $uva_condition : '')
                                                    . '&amp;sql_query=' . (isset($sql_query) ? urlencode($sql_query) : '')
                                                    . '&amp;goto=' . (isset($sql_goto) ? urlencode($lnk_goto) : '')
                                                    . '&amp;transform_key=' . urlencode($meta->name);


                // n u m e r i c
                if ($meta->numeric == 1) {
                    

                // lem9: if two fields have the same name (this is possible
                //       with self-join queries, for example), using $meta->name
                //       will show both fields NULL even if only one is NULL,
                //       so use the $pointer
                //      (works only if function_exists('is_null')
                // PS:   why not always work with the number ($i), since
                //       the default second parameter of
                //       mysql_fetch_array() is MYSQL_BOTH, so we always get
                //       associative and numeric indices?

                    //if (!isset($row[$meta->name])
                    if (!isset($row[$pointer])
                        || (function_exists('is_null') && is_null($row[$pointer]))) {
                        $vertical_display['data'][$row_no][$i]     = '    <td align="right" valign="top" ' . $column_style . ' bgcolor="' . $bgcolor . '"><i>NULL</i></td>' . "\n";
                    } else if ($row[$pointer] != '') {
                        $vertical_display['data'][$row_no][$i]     = '    <td align="right" valign="top" ' . $column_style . ' bgcolor="' . $bgcolor . '" nowrap="nowrap">';

                        reset($analyzed_sql[0]['select_expr']);
                        while (list ($select_expr_position, $select_expr) = each ($analyzed_sql[0]['select_expr'])) {
                            $alias = $analyzed_sql[0]['select_expr'][$select_expr_position]['alias'];
                            if (!empty($alias)) {
                                $true_column = $analyzed_sql[0]['select_expr'][$select_expr_position]['column'];
                                if ($alias == $meta->name) {
                                    $meta->name = $true_column;
                                } // end if
                            } // end if
                        } // end while

                        if (isset($map[$meta->name])) {
                            // Field to display from the foreign table?
                            if (!empty($map[$meta->name][2])) {
                                $dispsql     = 'SELECT ' . PMA_backquote($map[$meta->name][2])
                                             . ' FROM ' . PMA_backquote($map[$meta->name][3]) . '.' . PMA_backquote($map[$meta->name][0])
                                             . ' WHERE ' . PMA_backquote($map[$meta->name][1])
                                             . ' = ' . $row[$pointer];
                                $dispresult  = PMA_mysql_query($dispsql);
                                if ($dispresult && mysql_num_rows($dispresult) > 0) {
                                    $dispval = PMA_mysql_result($dispresult, 0);
                                }
                                else {
                                    $dispval = $GLOBALS['strLinkNotFound'];
                                }
                            }
                            else {
                                $dispval     = '';
                            } // end if... else...
                            $title = (!empty($dispval))? ' title="' . htmlspecialchars($dispval) . '"' : '';

                            $vertical_display['data'][$row_no][$i] .= '<a href="sql.php?'
                                                                   .  PMA_generate_common_url($map[$meta->name][3], $map[$meta->name][0])
                                                                   .  '&amp;pos=0&amp;session_max_rows=' . $session_max_rows . '&amp;dontlimitchars=' . $dontlimitchars
                                                                   .  '&amp;sql_query=' . urlencode('SELECT * FROM ' . PMA_backquote($map[$meta->name][0]) . ' WHERE ' . PMA_backquote($map[$meta->name][1]) . ' = ' . $row[$pointer]) . '"' . $title . '>'
                                                                   .  ($transform_function != $default_function ? $transform_function($row[$pointer], $transform_options) : $transform_function($row[$pointer])) . '</a>';
                        } else {
                            $vertical_display['data'][$row_no][$i] .= ($transform_function != $default_function ? $transform_function($row[$pointer], $transform_options) : $transform_function($row[$pointer]));
                        }
                        $vertical_display['data'][$row_no][$i]     .= '</td>' . "\n";
                    } else {
                        $vertical_display['data'][$row_no][$i]     = '    <td align="right" ' . $column_style . ' valign="top" bgcolor="' . $bgcolor . '" nowrap="nowrap">&nbsp;</td>' . "\n";
                    }

                //  b l o b

                } else if ($GLOBALS['cfg']['ShowBlob'] == FALSE && eregi('BLOB', $meta->type)) {
                    // loic1 : PMA_mysql_fetch_fields returns BLOB in place of
                    // TEXT fields type, however TEXT fields must be displayed
                    // even if $cfg['ShowBlob'] is false -> get the true type
                    // of the fields.
                    $field_flags = PMA_mysql_field_flags($dt_result, $i);
                    if (eregi('BINARY', $field_flags)) {
                        $blobtext = '[BLOB';
                        if (isset($row[$pointer])) {
                            $blob_size = PMA_formatByteDown(strlen($row[$pointer]), 3, 1);
                            $blobtext .= ' - '. $blob_size [0] . ' ' . $blob_size[1];
                            unset($blob_size);
                        }

                        $blobtext .= ']';
                        $blobtext = ($default_function != $transform_function ? $transform_function($blobtext, $transform_options) : $default_function($blobtext));

                        $vertical_display['data'][$row_no][$i]      = '    <td align="center" ' . $column_style . ' valign="top" bgcolor="' . $bgcolor . '">' . $blobtext . '</td>';
                    } else {
                        //if (!isset($row[$meta->name])
                        if (!isset($row[$pointer])
                            || (function_exists('is_null') && is_null($row[$pointer]))) {
                            $vertical_display['data'][$row_no][$i] = '    <td valign="top" ' . $column_style . ' bgcolor="' . $bgcolor . '"><i>NULL</i></td>' . "\n";
                        } else if ($row[$pointer] != '') {
                            // garvin: if a transform function for blob is set, none of these replacements will be made
                            if (strlen($row[$pointer]) > $GLOBALS['cfg']['LimitChars'] && ($dontlimitchars != 1)) {
                                $row[$pointer] = substr($row[$pointer], 0, $GLOBALS['cfg']['LimitChars']) . '...';
                            }
                            // loic1: displays all space characters, 4 space
                            // characters for tabulations and <cr>/<lf>
                            $row[$pointer]     = ($default_function != $transform_function ? $transform_function($row[$pointer], $transform_options) : $default_function($row[$pointer]));

                            $vertical_display['data'][$row_no][$i] = '    <td valign="top" ' . $column_style . ' bgcolor="' . $bgcolor . '">' . $row[$pointer] . '</td>' . "\n";
                        } else {
                            $vertical_display['data'][$row_no][$i] = '    <td valign="top" ' . $column_style . ' bgcolor="' . $bgcolor . '">&nbsp;</td>' . "\n";
                        }
                    }
                } else {
                    //if (!isset($row[$meta->name])
                    if (!isset($row[$pointer])
                        || (function_exists('is_null') && is_null($row[$pointer]))) {
                        $vertical_display['data'][$row_no][$i]     = '    <td valign="top" ' . $column_style . ' bgcolor="' . $bgcolor . '"><i>NULL</i></td>' . "\n";
                    } else if ($row[$pointer] != '') {
                        // loic1: support blanks in the key
                        $relation_id = $row[$pointer];

                        // nijel: Cut all fields to $cfg['LimitChars']
                        if (strlen($row[$pointer]) > $GLOBALS['cfg']['LimitChars'] && ($dontlimitchars != 1)) {
                            $row[$pointer] = substr($row[$pointer], 0, $GLOBALS['cfg']['LimitChars']) . '...';
                        }

                        // loic1: displays special characters from binaries
                        $field_flags = PMA_mysql_field_flags($dt_result, $i);
                        if (eregi('BINARY', $field_flags)) {
                            $row[$pointer]     = str_replace("\x00", '\0', $row[$pointer]);
                            $row[$pointer]     = str_replace("\x08", '\b', $row[$pointer]);
                            $row[$pointer]     = str_replace("\x0a", '\n', $row[$pointer]);
                            $row[$pointer]     = str_replace("\x0d", '\r', $row[$pointer]);
                            $row[$pointer]     = str_replace("\x1a", '\Z', $row[$pointer]);
                            $row[$pointer]     = ($default_function != $transform_function ? $transform_function('BLOB', $transform_options) : $default_function($row[$pointer]));
                        }
                        // loic1: displays all space characters, 4 space
                        // characters for tabulations and <cr>/<lf>
                        else {
                            $row[$pointer]     = ($default_function != $transform_function ? $transform_function($row[$pointer], $transform_options) : $default_function($row[$pointer]));
                        }

                        // garvin: transform functions may enable nowrapping:
                        $function_nowrap = $transform_function . '_nowrap';
                        $bool_nowrap = (($default_function != $transform_function && function_exists($function_nowrap)) ? $function_nowrap($transform_options) : false);
                        
                        // loic1: do not wrap if date field type
                        $nowrap = ((eregi('DATE|TIME', $meta->type) || $bool_nowrap) ? ' nowrap="nowrap"' : '');
                        $vertical_display['data'][$row_no][$i]     = '    <td valign="top" ' . $column_style . ' bgcolor="' . $bgcolor . '"' . $nowrap . '>';

                        reset($analyzed_sql[0]['select_expr']);
                        while (list ($select_expr_position, $select_expr) = each ($analyzed_sql[0]['select_expr'])) {
                            $alias = $analyzed_sql[0]['select_expr'][$select_expr_position]['alias'];
                            if (!empty($alias)) {
                                $true_column = $analyzed_sql[0]['select_expr'][$select_expr_position]['column'];
                                if ($alias == $meta->name) {
                                    $meta->name = $true_column;
                                } // end if
                            } // end if
                        } // end while

                        if (isset($map[$meta->name])) {
                            // Field to display from the foreign table?
                            if (!empty($map[$meta->name][2])) {
                                $dispsql     = 'SELECT ' . PMA_backquote($map[$meta->name][2])
                                             . ' FROM ' . PMA_backquote($map[$meta->name][3]) . '.' . PMA_backquote($map[$meta->name][0])
                                             . ' WHERE ' . PMA_backquote($map[$meta->name][1])
                                             . ' = \'' . PMA_sqlAddslashes($row[$pointer]) . '\'';
                                $dispresult  = @PMA_mysql_query($dispsql);
                                if ($dispresult && mysql_num_rows($dispresult) > 0) {
                                    $dispval = PMA_mysql_result($dispresult, 0);
                                }
                                else {
                                    $dispval = $GLOBALS['strLinkNotFound'];
                                }
                            }
                            else {
                                $dispval = '';
                            }
                            $title = (!empty($dispval))? ' title="' . htmlspecialchars($dispval) . '"' : '';

                            $vertical_display['data'][$row_no][$i] .= '<a href="sql.php?'
                                                                   .  PMA_generate_common_url($map[$meta->name][3], $map[$meta->name][0])
                                                                   .  '&amp;pos=0&amp;session_max_rows=' . $session_max_rows . '&amp;dontlimitchars=' . $dontlimitchars
                                                                   .  '&amp;sql_query=' . urlencode('SELECT * FROM ' . PMA_backquote($map[$meta->name][0]) . ' WHERE ' . PMA_backquote($map[$meta->name][1]) . ' = \'' . PMA_sqlAddslashes($relation_id) . '\'') . '"' . $title . '>'
                                                                   .  $row[$pointer] . '</a>';
                        } else {
                            $vertical_display['data'][$row_no][$i] .= $row[$pointer];
                        }
                        $vertical_display['data'][$row_no][$i]     .= '</td>' . "\n";
                    } else {
                        $vertical_display['data'][$row_no][$i]     = '    <td valign="top" ' . $column_style . ' bgcolor="' . $bgcolor . '">&nbsp;</td>' . "\n";
                    }
                }

                // lem9: output stored cell
                if ($disp_direction == 'horizontal' || $disp_direction == 'horizontalflipped') {
                    echo $vertical_display['data'][$row_no][$i];
                }

                if (isset($vertical_display['rowdata'][$i][$row_no])) {
                    $vertical_display['rowdata'][$i][$row_no] .= $vertical_display['data'][$row_no][$i];
                } else {
                    $vertical_display['rowdata'][$i][$row_no] = $vertical_display['data'][$row_no][$i];
                }
            } // end for (2)

            // 3. Displays the modify/delete links on the right if required
            if ($GLOBALS['cfg']['ModifyDeleteAtRight']
                && ($disp_direction == 'horizontal' || $disp_direction == 'horizontalflipped')) {
                    include('./libraries/display_tbl_links.lib.php');
            } // end if (3)

            if ($disp_direction == 'horizontal' || $disp_direction == 'horizontalflipped') {
                echo "\n";
                ?>
</tr>
                <?php
            } // end if

            // 4. Gather links of del_urls and edit_urls in an array for later
            //    output
            if (!isset($vertical_display['edit'][$row_no])) {
                $vertical_display['edit'][$row_no]   = '';
                $vertical_display['delete'][$row_no] = '';
            }

            if (isset($edit_url)) {
                $vertical_display['edit'][$row_no]   .= '    <td align="center" valign="' . ($bookmark_go != '' ? 'top' : 'middle') . '" bgcolor="' . $bgcolor . '">' . "\n"
                                                     . PMA_linkOrButton($edit_url, $edit_str, '')
                                                     . $bookmark_go
                                                     .  '    </td>' . "\n";
            }

            if (isset($del_url)) {
                $vertical_display['delete'][$row_no] .= '    <td align="center" valign="' . ($bookmark_go != '' ? 'top' : 'middle') . '" bgcolor="' . $bgcolor . '">' . "\n"
                                                     . PMA_linkOrButton($del_url, $del_str, (isset($js_conf) ? $js_conf : ''))
                                                     .  '    </td>' . "\n";
            }

            echo (($disp_direction == 'horizontal' || $disp_direction == 'horizontalflipped') ? "\n" : '');
            $row_no++;
        } // end while
        $GLOBALS['url_query'] = $url_query;

        return TRUE;
    } // end of the 'PMA_displayTableBody()' function


    /**
     * Do display the result table with the vertical direction mode.
     * Credits for this feature goes to Garvin Hicking <hicking@faktor-e.de>.
     *
     * @return  boolean  always true
     *
     * @global  array    the information to display
     * @global  integer  the number of row to display between two table headers
     *
     * @access  private
     *
     * @see     PMA_displayTable()
     */
    function PMA_displayVerticalTable()
    {
        global $vertical_display, $repeat_cells;

        reset($vertical_display);

        // Displays "edit" link at top if required
        if ($GLOBALS['cfg']['ModifyDeleteAtLeft'] && is_array($vertical_display['edit'])) {
            echo '<tr>' . "\n";
            echo $vertical_display['textbtn'];
            reset($vertical_display['edit']);
            $foo_counter = 0;
            while (list($key, $val) = each($vertical_display['edit'])) {
                if (($foo_counter != 0) && ($repeat_cells != 0) && !($foo_counter % $repeat_cells)) {
                    echo '    <td>&nbsp;</td>' . "\n";
                }

                echo $val;
                $foo_counter++;
            } // end while
            echo '</tr>' . "\n";
        } // end if

        // Displays "delete" link at top if required
        if ($GLOBALS['cfg']['ModifyDeleteAtLeft'] && is_array($vertical_display['delete'])) {
            echo '<tr>' . "\n";
            if (!is_array($vertical_display['edit'])) {
                echo $vertical_display['textbtn'];
            }
            reset($vertical_display['delete']);
            $foo_counter = 0;
            while (list($key, $val) = each($vertical_display['delete'])) {
                if (($foo_counter != 0) && ($repeat_cells != 0) && !($foo_counter % $repeat_cells)) {
                    echo '<td>&nbsp;</td>' . "\n";
                }

                echo $val;
                $foo_counter++;
            } // end while
            echo '</tr>' . "\n";
        } // end if

        // Displays data
        reset($vertical_display['desc']);
        $row_no = 0;
        while (list($key, $val) = each($vertical_display['desc'])) {
            $row_no++;

            if (isset($GLOBALS['printview']) && ($GLOBALS['printview'] == '1')) {
                $bgcolor = '#ffffff';
            } else {
                $bgcolor = ($row_no % 2) ? $GLOBALS['cfg']['BgcolorOne'] : $GLOBALS['cfg']['BgcolorTwo'];
            }

            $on_mouse     = '';
            if (!isset($GLOBALS['printview']) || ($GLOBALS['printview'] != '1')) {
                if ($GLOBALS['cfg']['BrowsePointerColor'] != '') {
                    $on_mouse = ' onmouseover="setVerticalPointer(this, ' . $row_no . ', \'over\', \'' . $GLOBALS['cfg']['BgcolorOne'] . '\', \'' . $GLOBALS['cfg']['BgcolorTwo'] . '\', \'' . $GLOBALS['cfg']['BrowsePointerColor'] . '\', \'' . $GLOBALS['cfg']['BrowseMarkerColor'] . '\');"'
                              . ' onmouseout="setVerticalPointer(this, ' . $row_no . ', \'out\', \'' . $GLOBALS['cfg']['BgcolorOne'] . '\', \'' . $GLOBALS['cfg']['BgcolorTwo'] . '\', \'' . $GLOBALS['cfg']['BrowsePointerColor'] . '\', \'' . $GLOBALS['cfg']['BrowseMarkerColor'] . '\');"';
                }
                if ($GLOBALS['cfg']['BrowseMarkerColor'] != '') {
                    $on_mouse .= ' onmousedown="setVerticalPointer(this, ' . $row_no . ', \'click\', \'' . $GLOBALS['cfg']['BgcolorOne'] . '\', \'' . $GLOBALS['cfg']['BgcolorTwo'] . '\', \'' . $GLOBALS['cfg']['BrowsePointerColor'] . '\', \'' . $GLOBALS['cfg']['BrowseMarkerColor'] . '\');"';
                }
            } // end if

            echo '<tr ' . $on_mouse . '>' . "\n";
            echo $val;

            $foo_counter = 0;
            while (list($subkey, $subval) = each($vertical_display['rowdata'][$key])) {
                if (($foo_counter != 0) && ($repeat_cells != 0) and !($foo_counter % $repeat_cells)) {
                    echo $val;
                }

                echo $subval;
                $foo_counter++;
            } // end while

            echo '</tr>' . "\n";
        } // end while

        // Displays "edit" link at bottom if required
        if ($GLOBALS['cfg']['ModifyDeleteAtRight'] && is_array($vertical_display['edit'])) {
            echo '<tr>' . "\n";
            echo $vertical_display['textbtn'];
            reset($vertical_display['edit']);
            $foo_counter = 0;
            while (list($key, $val) = each($vertical_display['edit'])) {
                if (($foo_counter != 0) && ($repeat_cells != 0) && !($foo_counter % $repeat_cells)) {
                    echo '<td>&nbsp;</td>' . "\n";
                }

                echo $val;
                $foo_counter++;
            } // end while
            echo '</tr>' . "\n";
        } // end if

        // Displays "delete" link at bottom if required
        if ($GLOBALS['cfg']['ModifyDeleteAtRight'] && is_array($vertical_display['delete'])) {
            echo '<tr>' . "\n";
            if (!is_array($vertical_display['edit'])) {
                echo $vertical_display['textbtn'];
            }
            reset($vertical_display['delete']);
            $foo_counter = 0;
            while (list($key, $val) = each($vertical_display['delete'])) {
                if (($foo_counter != 0) && ($repeat_cells != 0) && !($foo_counter % $repeat_cells)) {
                    echo '<td>&nbsp;</td>' . "\n";
                }

                echo $val;
                $foo_counter++;
            } // end while
            echo '</tr>' . "\n";
        }

        return TRUE;
    } // end of the 'PMA_displayVerticalTable' function


    /**
     * Displays a table of results returned by a sql query.
     * This function is called by the "sql.php" script.
     *
     * @param   integer the link id associated to the query which results have
     *                  to be displayed
     * @param   array   the display mode
     * @param   array   the analyzed query
     *
     * @global  string   the current language
     * @global  integer  the server to use (refers to the number in the
     *                   configuration file)
     * @global  array    the current server config
     * @global  string   the database name
     * @global  string   the table name
     * @global  string   the url to go back in case of errors
     * @global  string   the current sql query
     * @global  integer  the total number of rows returned by the sql query
     * @global  integer  the total number of rows returned by the sql query
     *                   without any programmatically appended "LIMIT" clause
     * @global  integer  the current postion of the first record to be
     *                   displayed
     * @global  array    the list of fields properties
     * @global  integer  the total number of fields returned by the sql query
     * @global  array    informations used with vertical display mode
     * @global  string   the display mode (horizontal/vertical/horizontalflipped)
     * @global  integer  the number of row to display between two table headers
     * @global  boolean  whether to limit the number of displayed characters of
     *                   text type fields or not
     * @global  array    the relation settings
     *
     * @access  private
     *
     * @see     PMA_showMessage(), PMA_setDisplayMode(),
     *          PMA_displayTableNavigation(), PMA_displayTableHeaders(),
     *          PMA_displayTableBody()
     */
    function PMA_displayTable(&$dt_result, &$the_disp_mode, $analyzed_sql)
    {
        global $lang, $server, $cfg, $db, $table;
        global $goto;
        global $sql_query, $num_rows, $unlim_num_rows, $pos, $fields_meta, $fields_cnt;
        global $vertical_display, $disp_direction, $repeat_cells, $highlight_columns;
        global $dontlimitchars;
        global $cfgRelation;

        // 1. ----- Prepares the work -----

        // 1.1 Gets the informations about which functionnalities should be
        //     displayed
        $total      = '';
        $is_display = PMA_setDisplayMode($the_disp_mode, $total);
        if ($total == '') {
            unset($total);
        }

        // 1.2 Defines offsets for the next and previous pages
        if ($is_display['nav_bar'] == '1') {
            if (!isset($pos)) {
                $pos          = 0;
            }
            if ($GLOBALS['session_max_rows'] == 'all') {
                $pos_next     = 0;
                $pos_prev     = 0;
            } else {
                $pos_next     = $pos + $GLOBALS['cfg']['MaxRows'];
                $pos_prev     = $pos - $GLOBALS['cfg']['MaxRows'];
                if ($pos_prev < 0) {
                    $pos_prev = 0;
                }
            }
        } // end if

        // 1.3 Urlencodes the query to use in input form fields
        $encoded_sql_query = urlencode($sql_query);

        // 2. ----- Displays the top of the page -----

        // 2.1 Displays a messages with position informations
        if ($is_display['nav_bar'] == '1' && isset($pos_next)) {
            if (isset($unlim_num_rows) && $unlim_num_rows != $total) {
                $selectstring = ', ' . $unlim_num_rows . ' ' . $GLOBALS['strSelectNumRows'];
            } else {
                $selectstring = '';
            }
            $last_shown_rec = ($GLOBALS['session_max_rows'] == 'all' || $pos_next > $total)
                            ? $total - 1
                            : $pos_next - 1;
            PMA_showMessage($GLOBALS['strShowingRecords'] . " $pos - $last_shown_rec ($total " . $GLOBALS['strTotal'] . $selectstring . ', ' . sprintf($GLOBALS['strQueryTime'], $GLOBALS['querytime']) . ')');
        } else if (!isset($GLOBALS['printview']) || $GLOBALS['printview'] != '1') {
            PMA_showMessage($GLOBALS['strSQLQuery']);
        }

        // 2.3 Displays the navigation bars
        if (!isset($table) || strlen(trim($table)) == 0) {
            $table = $fields_meta[0]->table;
        }
        if ($is_display['nav_bar'] == '1') {
            PMA_displayTableNavigation($pos_next, $pos_prev, $encoded_sql_query);
            echo "\n";
        } else if (!isset($GLOBALS['printview']) || $GLOBALS['printview'] != '1') {
            echo "\n" . '<br /><br />' . "\n";
        }

        // 2b ----- Get field references from Database -----
        // (see the 'relation' config variable)
        // loic1, 2002-03-02: extended to php3

        // init map
        $map = array();

        // find tables

        $target=array();
        reset($analyzed_sql[0]['table_ref']);
        while (list ($table_ref_position, $table_ref) = each ($analyzed_sql[0]['table_ref'])) {
           $target[] = $analyzed_sql[0]['table_ref'][$table_ref_position]['table_true_name'];
        }
        $tabs    = '(\'' . join('\',\'', $target) . '\')';

        if ($cfgRelation['displaywork']) {
            if (empty($table)) {
                $exist_rel = FALSE;
            } else {
                $exist_rel = PMA_getForeigners($db, $table, '', 'both');
                if ($exist_rel) {
                    while (list($master_field,$rel) = each($exist_rel)) {
                        $display_field = PMA_getDisplayField($rel['foreign_db'],$rel['foreign_table']);
                        $map[$master_field] = array($rel['foreign_table'],
                                              $rel['foreign_field'],
                                              $display_field,
                                              $rel['foreign_db']);
                    } // end while
                } // end if
            } // end if
        } // end if
        // end 2b

        // 3. ----- Displays the results table -----
        echo '<!-- Results table -->' . "\n"
           . '<table ';
        if (isset($GLOBALS['printview']) && $GLOBALS['printview'] == '1') {
            echo 'border="1" cellpadding="2" cellspacing="0"';
        } else {
            echo 'border="' . $GLOBALS['cfg']['Border'] . '" cellpadding="5"';
        }
        echo '>' . "\n";
        PMA_displayTableHeaders($is_display, $fields_meta, $fields_cnt, $analyzed_sql);
        $url_query='';
        PMA_displayTableBody($dt_result, $is_display, $map, $analyzed_sql);
        // vertical output case
        if ($disp_direction == 'vertical') {
            PMA_displayVerticalTable();
        } // end if
        unset($vertical_display);
        ?>
</table>
        <?php

        echo "\n";

        // 4. ----- Displays the link for multi-fields delete

        if ($is_display['del_lnk'] == 'dr') {

            $propicon = (string)$GLOBALS['cfg']['PropertiesIconic'];

//        echo '&nbsp;&nbsp;&nbsp;<img src="./images/arrow_' . $GLOBALS['text_dir'] . '.gif" border="0" width="38" height="22" alt="' . $GLOBALS['strWithChecked'] . '" />';
              echo '&nbsp;&nbsp;<i>' . $GLOBALS['strWithChecked'] . '</i>'. "\n";

            if ($cfg['PropertiesIconic']) {
                /* Opera has trouble with <input type="image"> */
                /* IE has trouble with <button> */
                if (PMA_USR_BROWSER_AGENT != 'IE') {
                    echo '                    <button class="mult_submit" type="submit" name="submit_mult" value="row_delete" title="' . $GLOBALS['strDelete'] . '">' . "\n"
                       . '<img src="./images/button_drop.png" title="' . $GLOBALS['strDelete'] . '" alt="' . $GLOBALS['strDelete'] . '" width="11" height="13" />' . (($propicon == 'both') ? '&nbsp;' . $GLOBALS['strDelete'] : '') . "\n"
                       . '</button>';
                } else {
                    echo '                    <input type="image" name="submit_mult" value="row_delete" title="' . $GLOBALS['strDelete'] . '" src="./images/button_drop.png" />' . (($propicon == 'both') ? '&nbsp;' . $GLOBALS['strDelete'] : ''); 
                }
                echo "\n";
            } else {
                echo '                    <input type="submit" name="submit_mult" value="row_delete" title="' . $GLOBALS['strDelete'] . '" />' . "\n";
            }
            echo '<input type="hidden" name="sql_query" value="' . $sql_query . '" />' . "\n";
            echo '<input type="hidden" name="pos" value="' . $pos . '" />' . "\n";
            echo '<input type="hidden" name="url_query" value="' . $GLOBALS['url_query'] . '" />' . "\n";
            echo '<br />' . "\n";
            echo '</form>' . "\n";
        }

        // 5. ----- Displays the navigation bar at the bottom if required -----

        if ($is_display['nav_bar'] == '1') {
            echo '<br />' . "\n";
            PMA_displayTableNavigation($pos_next, $pos_prev, $encoded_sql_query);
        } else if (!isset($GLOBALS['printview']) || $GLOBALS['printview'] != '1') {
            echo "\n" . '<br /><br />' . "\n";
        }
    } // end of the 'PMA_displayTable()' function

    function default_function($buffer) {
        $buffer = htmlspecialchars($buffer);
        $buffer = str_replace("\011", ' &nbsp;&nbsp;&nbsp;', str_replace('  ', ' &nbsp;', $buffer));
        $buffer = ereg_replace("((\015\012)|(\015)|(\012))", '<br />', $buffer);
        
        return $buffer;
    }
} // $__PMA_DISPLAY_TBL_LIB__
?>
