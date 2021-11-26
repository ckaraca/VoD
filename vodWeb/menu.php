<div align="center">
<?php
	if(!isset($hash)) header("location:error.php?errno=3");
	 	
	$link=DBconnect();
	$result = mysql_query("SELECT title,name,surname,priv FROM user WHERE user='$user'", $link);
	if (!$result) die("query failed: ".mysql_error());
	$row = mysql_fetch_array($result);
	
	echo 'Welcome ',$row["title"],' ',$row["name"],' ',$row["surname"];
	echo "<br>You have ";
	
	switch($row["priv"])
	{
		case 0: // student
		echo "<b>student</b> privileges<br>";
		break;
		case 1: // Instr
				echo "<b>Instructor</b> privileges<br>";
		break;
		case 2:	// Admin
				echo "<b>Administrator</b> privileges<br>";
		break;
	}
	DBdisconnect($link);
?>

</div>
<link href="vod.css" rel="stylesheet" type="text/css">
<div align="center"><br>
  <bR>
</div>
<table width="80%"  border="1" align="center" cellpadding="0" cellspacing="0">
  <tr>
    <th colspan="2" scope="col">Download the latest versions of VoD components </th>
  </tr>
  <tr>
    <td width="52%"><div align="center"><strong>Required libraries for client side applications </strong></div></td>
    <td width="48%"> <div align="center"><a href="dl.php?dl=dll" class="iki">MySQL Library DLL </a><br>
      Last update: <? echo  date ("d F Y H:i", filemtime("../libs/libmySQL.dll"));?> 
</div></td>
  </tr>
  <tr>
    <td rowspan="3"><div align="center"><strong>Client side applications</strong></div></td>
    <td><div align="center"><a href="dl.php?dl=vp" class="iki">VoD Player</a><br>
      Last update: <? echo  date ("d F Y H:i", filemtime("../libs/VoDPlayer.exe"));?></div></td>
  </tr>
  <tr>
    <td><div align="center"><a href="dl.php?dl=vpp" class="iki">VoD Player Plus </a><br>
  Last update: <? echo  date ("d F Y H:i", filemtime("../libs/VoDPP.exe"));?></div></td>
  </tr>
  <tr>
    <td><div align="center" class="iki"><a href="dl.php?dl=vpe" class="iki">VoD Project Editor </a><br>
      Last update:<? echo  date ("d F Y H:i", filemtime("../libs/VoD.exe"));?>
    </div></td>
  </tr>
  <tr>
    <td colspan="2"><div align="center">  <?
	if($row["priv"]==2)
	{
		echo "<a href=\"http://vod.emu.edu.tr/Admin\">PhpMyAdmin</a>";
	}
	?>    </div></td>
  </tr>
</table>


