<?php
$link=DBconnect();

$result = mysql_query("DELETE from user WHERE userid='$_GET[uid]'", $link);
if (!$result) die("query failed: ".mysql_error());

DBdisconnect($link);
echo '<meta http-equiv="refresh" content="0;URL=vod.php?action=adm/userlist.php&op=3">';
?>