<?php
	require("../libs/vodlib.php");
	require("../libs/zip.lib.php");
	Authorize($hash,$user);
	if(!isset($hash)) header("location:error.php?errno=3");
	$link=DBconnect();
	$result = mysql_query("SELECT title,name,surname,priv FROM user WHERE user='$user'", $link);
	if (!$result) die("query failed: ".mysql_error());
	$row = mysql_fetch_array($result);
	
	//echo 'Welcome ',$row["title"],' ',$row["name"],' ',$row["surname"];
	
	if ( phpversion() >= "4.2.0")
   	{
		extract($_POST);
   		extract($_GET);
   		extract($_SERVER);
	   	extract($_FILES);
   		extract($_ENV);
   		extract($_COOKIE);
   	}
	if($dl=="dll")
	{
		$path ="../libs/libmySQL.dll";
		header('Content-type: application/octet-stream'); 
		//header("Content-Type: " . filetype($path));
		header("Content-Disposition: attachment; filename=libmySQL.dll");
		header("Accept-Ranges: bytes");
		header("Content-Length: " . filesize($path));
		//header("Content-Type: " . filetype($path));

		@readfile($path);
	}
	if($dl=="vp")
	{
		$path ="../libs/VoDPlayer.exe";
		//header('Content-type: application/x-download'); 
		header("Content-Type: " . filetype($path));
		$tte = "Content-Disposition: attachment; filename=VoDPlayer.exe";
		header($tte);
		//"Content-Disposition: attachment; filename=VoDPlayer.exe"
		header("Accept-Ranges: bytes");
		header("Content-Length: " . filesize($path));
		//header("Content-Type: " . filetype($path));

		@readfile($path);

	}
	if($dl=="vpp")
	{
		$path ="../libs/VoDPP.exe";
		//header('Content-type: application/x-download'); 
		header("Content-Type: " . filetype($path));
		$tte = "Content-Disposition: attachment; filename=VoDPPlus.exe";
		header($tte);
		//"Content-Disposition: attachment; filename=VoDPlayer.exe"
		header("Accept-Ranges: bytes");
		header("Content-Length: " . filesize($path));
		//header("Content-Type: " . filetype($path));

		@readfile($path);

	}
	if($dl=="vpe")
	{
		$path ="../libs/VoD.exe";
		//header('Content-type: application/x-download'); 
		header("Content-Type: " . filetype($path));
		header("Content-Disposition: attachment; filename=VoD.exe");
		header("Accept-Ranges: bytes");
		header("Content-Length: " . filesize($path));
		//header("Content-Type: " . filetype($path));

		@readfile($path);

	}
?>