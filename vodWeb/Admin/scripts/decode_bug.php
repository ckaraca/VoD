<?php
/* $Id: decode_bug.php,v 1.7 2002/10/23 04:17:27 robbat2 Exp $ */
// vim: expandtab sw=4 ts=4 sts=4 foldmarker={,} fdm=marker:


/**
 * Parser BUG decoder
 *
 * This is the parser bug decoder system
 * Throw the bug data in teh query box, and hit submit for output.
 *
 * Copyright 2002 Robin Johnson <robbat2@users.sourceforge.net>
 */


/**
 * Displays the form
 */
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN"
    "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en-US" lang="en-US">

<head>
    <meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1" />
    <title>phpMyAdmin - Parser BUG decoder</title>
    <style type="text/css">
    <!--
    body, p {
        font-family: Arial, Helvetica, sans-serif;
        font-size:   medium;
    }
    h1 {
        font-family: Verdana, Arial, Helvetica, sans-serif;
        font-size:   large;
        font-weight: bold;
        color:       #000066;
    }
    //-->
    </style>
</head>


<body bgcolor="#FFFFFF">
<h1>Parser BUG decoder</h1>
<br />

<form method="post" action="./decode_bug.php">
    <input type="hidden" name="bar" value="<?php echo rand(); ?>" />
    Encoded bug report:<br />
    <textarea name="bug_encoded" cols="72" rows="10"></textarea>
    <br /><br />
    <input type="submit" />
</form>
<hr />

<?php
/**
 * If the form has been submitted -> decodes the bug report
 */

/**
 * Display the decoded bug report in ASCII format
 *
 * @param  string  the text data
 *
 * @return string  the text enclosed by "<pre>...</pre>" tags
 *
 * @access public
 */
function PMA_printDecodedBug($textdata)
{
    return '<pre>' . htmlspecialchars($textdata) . '</pre><br />';
} // end of the "PMA_printDecodedBug()" function


if (!empty($_POST) && isset($_POST['bug_encoded'])) {
    $bug_encoded = $_POST['bug_encoded'];
}
else if (!empty($HTTP_POST_VARS) && isset($HTTP_POST_VARS['bug_encoded'])) {
    $bug_encoded = $HTTP_POST_VARS['bug_encoded'];
}

if (!empty($bug_encoded)) {
    if (get_magic_quotes_gpc()) {
        $bug_encoded = stripslashes($bug_encoded);
    }

    $bug_encoded     = ereg_replace('[[:space:]]', '', $bug_encoded);
    $bug_decoded     = base64_decode($bug_encoded);
    if (substr($bug_encoded, 0, 2) == 'eN') {
        if (function_exists('gzuncompress')) {
            $result  = PMA_printDecodedBug(gzuncompress($bug_decoded));
            } else {
            $result  = 'Error: &quot;gzuncompress()&quot; is unavailable!' . "\n";
        }
    }
    else {
        $result  = PMA_printDecodedBug($bug_decoded);
    } // end if... else...

    echo '<p>Decoded:</p>' . "\n"
         . $result . "\n";
} // end if
?>
</body>

</html>
