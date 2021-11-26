<?php
/* $Id: text_plain__substr.inc.php,v 1.1 2003/03/18 15:30:25 garvinhicking Exp $ */
// vim: expandtab sw=4 ts=4 sts=4:

/**
 * Plugin function TEMPLATE (Garvin Hicking).
 * -----------------------------------------
 *
 * For instructions, read the libraries/transformations/README file.
 *
 * The string ENTER_FILENAME_HERE shall be substituted with the filename without the '.inc.php'
 * extension. For further information regarding naming conventions see the README file.
 */

if (!defined('PMA_TRANSFORMATION_TEXT_PLAIN__SUBSTR')){
    define('PMA_TRANSFORMATION_TEXT_PLAIN__SUBSTR', 1);
    
    function PMA_transformation_text_plain__substr($buffer, $options = array()) {
        // possibly use a global transform and feed it with special options:
        // include('./libraries/transformations/global.inc.php');
        
        // further operations on $buffer using the $options[] array.
        if (!isset($options[0]) ||  $options[0] == '') {
            $options[0] = 0;
        }

        if (!isset($options[1]) ||  $options[1] == '') {
            $options[1] = 'all';
        }

        if (!isset($options[2]) || $options[2] == '') {
            $options[2] = '...';
        }

        $newtext = '';
        if ($options[1] != 'all') {
            $newtext = substr($buffer, $options[0], $options[1]);
        } else {
            $newtext = substr($buffer, $options[0]);
        }
        
        $length = strlen($newtext);
        $baselength = strlen($buffer);
        if ($length != $baselength) {
            if ($options[0] != 0) {
                $newtext = $options[2] . $newtext;
            }
            
            if (($length + $options[0]) != $baselength) {
                $newtext .= $options[2];
            }
        }
        
        return $newtext;
    }
}
