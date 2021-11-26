<?php

if ( phpversion() >= "4.2.0")
   	{
		extract($_POST);
   		extract($_GET);
   		extract($_SERVER);
	   	extract($_FILES);
   		extract($_ENV);
   		extract($_COOKIE);
   	}
//****************************************************************************************************************************
function DBparameters(){ //Please Change the Variable Below 
	$dbhost="213.208.52.183"; //Hosting address of the MySQL database
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
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

function DBdisconnect($link)
{
	mysql_close($link);

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

function Login($loginname,$password) //if the Access information is OK function returns userid, otherwise (-1)
{
	$link=DBconnect();
	$result = mysql_query("SELECT userid from user WHERE user='$loginname' AND pass=PASSWORD('$password')", $link);
	if (!$result) die("query failed: ".mysql_error());

	if(mysql_num_rows($result)<1)
	{
		return(-1);		
	}else{
		$row = mysql_fetch_array($result);
		DBdisconnect($link);
		return($row["userid"]);
	}
}

///////////////////////////////////////////////////////////////////////////

function GetNewHash($user) //Generates a new Session ID and returns it
{	
	$link=DBconnect();
	$result = mysql_query("SELECT hkey FROM system WHERE id=1", $link);
	if (!$result) die("query failed: ".mysql_error());
	$row = mysql_fetch_array($result);
	
	$defhash=$row["hkey"];
	$temp=$defhash.$user;
	
	$newhash=md5($temp);
	DBdisconnect($link);
	return($newhash);
}

///////////////////////////////////////////////////////////////////////////

function CheckHash($hash,$user) //Generates a new Session ID and returns it
{	
	$link=DBconnect();
	$result = mysql_query("SELECT hkey FROM system WHERE id=1", $link);
	if (!$result) die("query failed: ".mysql_error());
	$row = mysql_fetch_array($result);
	
	$defhash=$row["hkey"];
	$temp=$defhash.$user;
	DBdisconnect($link);
	if(md5($temp)==$hash){
		return(1);
	}else{
		return(-1);
	}
}

///////////////////////////////////////////////////////////////////////////

function SendCookie($user)
{
	$link=DBconnect();
	$result = mysql_query("SELECT exptime FROM system", $link);
	if (!$result) die("query failed: ".mysql_error());
	$row = mysql_fetch_array($result);

	$hash=GetNewHash($user);
	$session_expire = 86400; //$row["exptime"];
	$cookie_expire = time()+$session_expire; 
	ini_set("session.cookie_lifetime",$session_expire);
	session_name("VoD");
	session_start();
	setcookie("hash",$hash, $cookie_expire, "/","213.208.52.183");
	setcookie("user",$user, $cookie_expire, "/","213.208.52.183");
	
}

//////////////////////////////////////////////////////////////////////////////////

function DeleteCookie()
{
setcookie ("hash", "", time() - 10000000); 
setcookie ("user", "", time() - 10000000); 

}

////////////////////////////////////////////////////////////////////////////////

function Authorize($hash,$user)
{
	if(isset($hash)) 
	{//There is Cookie
		if(CheckHash($hash,$user)<0) //if the hash information is correct
		{
			header("location:error.php?errno=3");
		}
	}else{ //There is NO Cookie
		header("location:error.php?errno=3");
	}
}


?>
