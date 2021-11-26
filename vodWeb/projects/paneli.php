<?PHP
print '<meta http-equiv="refresh" content="15;">';
	if ( phpversion() >= "4.2.0")
   	{
		extract($_POST);
   		extract($_GET);
   		extract($_SERVER);
	   	extract($_FILES);
   		extract($_ENV);
   		extract($_COOKIE);
   	}
	function DBparameters(){ //Please Change the Variable Below 
	$dbhost="localhost"; //Hosting address of the MySQL database
	$dbusername="control"; //Username of the Server
	$dbpassword="ankara"; //Password of the server
	$dbname="vod"; //Na
	return (array("host"=>$dbhost,"username"=>$dbusername,"password"=>$dbpassword,"dbname"=>$dbname));
}
//****************************************************************************************************************************

function DBconnect()
{
	$DB=DBparameters();
	$link=mysql_connect($DB["host"],$DB["username"],$DB["password"]) or die ("Could't Connect to the Server");
	mysql_select_db ($DB["dbname"])or die ("The Database cannot be selected");
	return ($link);
}
	$link=DBconnect();
	
?>
<style type="text/css">
<!--
body {
	margin-left: 0px;
	margin-top: 0px;
	margin-right: 0px;
	margin-bottom: 0px;
}
-->
</style>
<table width="100%"  border="1" cellspacing="0" cellpadding="0">
  <tr><td>&nbsp;</td></tr><tr><td bgcolor="#CCCCCC"><b>Active Users:</b></td></tr>
	<?
	$result2 = mysql_query("SELECT  * FROM chatuser, user WHERE chatuser.nick = user.user", $link);
	if (!$result2) die("query failed: ".mysql_error());
	//$row2 = mysql_fetch_array($result2);
	while($row2 = mysql_fetch_array($result2))
	{
		echo "<tr><td>";
		switch($row2["priv"])
		{
			case 0: // student
			echo "<b>Std: </b>";
			break;
			case 1: // Instr
				echo "<b>Inst: </b>";
			break;
			case 2:	// Admin
				echo "<b>Admin: </b>";
			break;
		}
		echo $row2["name"]," ",$row2["surname"];
		echo "</td></tr>";
		
	}
	mysql_close($link);
	?>
	</table>
</html>
