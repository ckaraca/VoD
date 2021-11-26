<?php
/* $Id: db_details_common.php,v 1.13 2003/07/19 10:43:22 lem9 Exp $ */
// vim: expandtab sw=4 ts=4 sts=4:

/**
 * Gets some core libraries
 */
if (!defined('PMA_GRAB_GLOBALS_INCLUDED')) {
    include('./libraries/grab_globals.lib.php');
}
if (!defined('PMA_COMMON_LIB_INCLUDED')) {
    include('./libraries/common.lib.php');
}
if (!defined('PMA_BOOKMARK_LIB_INCLUDED')) {
    include('./libraries/bookmark.lib.php');
}

PMA_checkParameters(array('db'));

/**
 * Defines the urls to return to in case of error in a sql statement
 */
$err_url_0 = 'main.php?' . PMA_generate_common_url();
$err_url   = $cfg['DefaultTabDatabase'] . '?' . PMA_generate_common_url($db);


/**
 * Ensures the database exists (else move to the "parent" script) and displays
 * headers
 */
if (!isset($is_db) || !$is_db) {
    // Not a valid db name -> back to the welcome page
    if (!empty($db)) {
        $is_db = @PMA_mysql_select_db($db);
    }
    if (empty($db) || !$is_db) {
        header('Location: ' . $cfg['PmaAbsoluteUri'] . 'main.php?' . PMA_generate_common_url('', '', '&') . (isset($message) ? '&message=' . urlencode($message) : '') . '&reload=1');
        exit();
    }
} // end if (ensures db exists)
// Displays headers
if (!isset($message)) {
    $js_to_run = 'functions.js';
    include('./header.inc.php');
    // Reloads the navigation frame via JavaScript if required
    if (isset($reload) && $reload) {
        echo "\n";
        ?>
<script type="text/javascript" language="javascript1.2">
<!--
window.parent.frames['nav'].location.replace('./left.php?<?php echo PMA_generate_common_url($db, '', '&'); ?>&hash=' + <?php echo (($cfg['QueryFrame'] && $cfg['QueryFrameJS']) ? 'window.parent.frames[\'queryframe\'].document.hashform.hash.value' : "'" . md5($cfg['PmaAbsoluteUri']) . "'"); ?>);
//-->
</script>
        <?php
    }
    echo "\n";
} else {
    PMA_showMessage($message);
}

/**
 * Set parameters for links
 */
$url_query = PMA_generate_common_url($db);

?>
