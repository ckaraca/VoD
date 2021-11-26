<?php
/* $Id: transformations.lib.php,v 1.3 2003/03/05 20:04:10 garvinhicking Exp $ */
// vim: expandtab sw=4 ts=4 sts=4:

/**
 * Set of functions used with the relation and pdf feature
 */


if (!defined('PMA_TRANSFORMATION_LIB_INCLUDED')){
    define('PMA_TRANSFORMATION_LIB_INCLUDED', 1);

    function PMA_transformation_getOptions($string) {
        $transform_options = array();
        
        if ($string != '') {
            if (eregi('^\'.*\'$', $string)) {
                $transform_options = explode('\',\'', eregi_replace('^\'(.*)\'$', '\1', $string));
            } else {
                $transform_options = array(0 => $string);
            }
        }
        
        return $transform_options;
    }
    
    /**
     * Gets all available MIME-types
     *
     * @return  array    array[mimetype], array[transformation]
     *
     * @access  public
     *
     * @author  Garvin Hicking <me@supergarv.de>
     */
    function PMA_getAvailableMIMEtypes() {
        $handle = opendir('./libraries/transformations');

        $stack = array();
        $filestack = array();
        
        while (($file = readdir($handle)) != false) {
            $filestack[$file] = $file;
        }  

        closedir($handle);

        if (is_array($filestack)) {
            @ksort($filestack);
            @reset($filestack);
            while(list($key, $file) = each($filestack)) {

                if (eregi('^.*__.*\.inc\.php$', trim($file))) {
                    // File contains transformation functions.
                    $base = explode("__", str_replace(".inc.php", "", $file));
                    
                    $mimetype = str_replace("_", "/", $base[0]);
                    $stack['mimetype'][$mimetype] = $mimetype;
                    
                    $stack['transformation'][] = $mimetype . ': ' . $base[1];
                    $stack['transformation_file'][] = $file;

                } else if (eregi('^.*\.inc\.php$', trim($file))) {
                    // File is a plain mimetype, no functions.
                    $base = str_replace(".inc.php", "", $file);
                    
                    if ($base != 'global') {
                        $mimetype = str_replace("_", "/", $base);
                        $stack['mimetype'][$mimetype] = $mimetype;
                        $stack['empty_mimetype'][$mimetype] = $mimetype;
                    }
                }

            }
        }

        return $stack;
    }
    
    /**
     * Gets the mimetypes for all rows of a table
     *
     * @param   string   the name of the db to check for
     * @param   string   the name of the table to check for
     * @param   string   whether to include only results having a mimetype set
     *
     * @return  array    [field_name][field_key] = field_value
     *
     * @global  array    the list of relations settings
     *
     * @access  public
     *
     * @author  Mike Beck <mikebeck@users.sourceforge.net> / Garvin Hicking <me@supergarv.de>
     */
    function PMA_getMIME($db, $table, $strict = false) {
        global $cfgRelation;

        $com_qry  = 'SELECT column_name, mimetype, transformation, transformation_options FROM ' . PMA_backquote($cfgRelation['column_info'])
                  . ' WHERE db_name = \'' . PMA_sqlAddslashes($db) . '\''
                  . ' AND table_name = \'' . PMA_sqlAddslashes($table) . '\''
                  . ' AND (mimetype != \'\'' . (!$strict ? ' OR transformation != \'\' OR transformation_options != \'\'' : '') . ')';
        $com_rs   = PMA_query_as_cu($com_qry);

        while ($row = @PMA_mysql_fetch_array($com_rs)) {
            $col                                    = $row['column_name'];
            $mime[$col]['mimetype']                 = $row['mimetype'];
            $mime[$col]['transformation']           = $row['transformation'];
            $mime[$col]['transformation_options']   = $row['transformation_options'];
        } // end while

        if (isset($mime) && is_array($mime)) {
            return $mime;
         } else {
            return FALSE;
         }
     } // end of the 'PMA_getMIME()' function

  /**
    * Set a single mimetype to a certain value.
    *
    * @param   string   the name of the db
    * @param   string   the name of the table
    * @param   string   the name of the column
    * @param   string   the mimetype of the column
    * @param   string   the transformation of the column
    * @param   string   the transformation options of the column
    * @param   string   (optional) force delete, will erase any existing comments for this column
    *
    * @return  boolean  true, if comment-query was made.
    *
    * @global  array    the list of relations settings
    *
    * @access  public
    */
    function PMA_setMIME($db, $table, $key, $mimetype, $transformation, $transformation_options, $forcedelete = false) {
        global $cfgRelation;

        $test_qry  = 'SELECT mimetype, ' . PMA_backquote('comment') . ' FROM ' . PMA_backquote($cfgRelation['column_info'])
                    . ' WHERE db_name = \'' . PMA_sqlAddslashes($db) . '\''
                    . ' AND table_name = \'' . PMA_sqlAddslashes($table) . '\''
                    . ' AND column_name = \'' . PMA_sqlAddslashes($key) . '\'';
        $test_rs   = PMA_query_as_cu($test_qry);

        if ($test_rs && mysql_num_rows($test_rs) > 0) {
            $row = @PMA_mysql_fetch_array($test_rs);
            
            if (!$forcedelete && (strlen($mimetype) > 0 || strlen($transformation) > 0 || strlen($transformation_options) > 0 || strlen($row['comment']) > 0)) {
                $upd_query = 'UPDATE ' . PMA_backquote($cfgRelation['column_info'])
                       . ' SET mimetype = \'' . PMA_sqlAddslashes($mimetype) . '\','
                       . '     transformation = \'' . PMA_sqlAddslashes($transformation) . '\','
                       . '     transformation_options = \'' . PMA_sqlAddslashes($transformation_options) . '\''
                       . ' WHERE db_name  = \'' . PMA_sqlAddslashes($db) . '\''
                       . ' AND table_name = \'' . PMA_sqlAddslashes($table) . '\''
                       . ' AND column_name = \'' . PMA_sqlAddslashes($key) . '\'';
            } else {
                $upd_query = 'DELETE FROM ' . PMA_backquote($cfgRelation['column_info'])
                       . ' WHERE db_name  = \'' . PMA_sqlAddslashes($db) . '\''
                       . ' AND table_name = \'' . PMA_sqlAddslashes($table) . '\''
                       . ' AND column_name = \'' . PMA_sqlAddslashes($key) . '\'';
            }
        } else if (strlen($mimetype) > 0 || strlen($transformation) > 0 || strlen($transformation_options) > 0) {
            $upd_query = 'INSERT INTO ' . PMA_backquote($cfgRelation['column_info'])
                       . ' (db_name, table_name, column_name, mimetype, transformation, transformation_options) '
                       . ' VALUES('
                       . '\'' . PMA_sqlAddslashes($db) . '\','
                       . '\'' . PMA_sqlAddslashes($table) . '\','
                       . '\'' . PMA_sqlAddslashes($key) . '\','
                       . '\'' . PMA_sqlAddslashes($mimetype) . '\','
                       . '\'' . PMA_sqlAddslashes($transformation) . '\','
                       . '\'' . PMA_sqlAddslashes($transformation_options) . '\')';
        }

        if (isset($upd_query)){
            $upd_rs    = PMA_query_as_cu($upd_query);
            unset($upd_query);
            return true;
        } else {
            return false;
        }
    } // end of 'PMA_setMIME()' function
} // $__PMA_TRANSFORMATION_LIB__
?>
