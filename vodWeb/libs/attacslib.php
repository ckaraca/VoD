<?php

function DBparameters(){ //Please Change the Variable Below 
	$dbhost="213.208.49.45"; //Hosting address of the MySQL database
	$dbusername="control"; //Username of the Server
	$dbpassword="ankara"; //Password of the server
	$dbname="attacs_0304fl"; //Na
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
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

function DBdisconnect($link)
{
	mysql_close($link);
}
?>