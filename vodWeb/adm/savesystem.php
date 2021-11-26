<?php
	if(!isset($hash)) header("location:error.php?errno=3");
	$link=DBconnect();
	$result = mysql_query("UPDATE system SET cookiename='$_POST[cookiename]',exptime='$_POST[exptime]',hkey='$_POST[hkey]',myserver='$_POST[myserver]',myuser='$_POST[myuser]',mypass='$_POST[mypass]',mydatabase='$_POST[mydatabase]',cemail='$_POST[cemail]',eemail='$_POST[eemail]',faqemail='$_POST[faqemail]',adminname='$_POST[adminname]' WHERE id=1", $link);
	if (!$result) die("query failed: ".mysql_error());
	DBdisconnect($link);
	echo 'Successfully Saved';
?>