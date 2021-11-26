<?php
Authorize($hash,$user);
$link=DBconnect();
$result = mysql_query("SELECT * FROM user", $link);
if (!$result) die("query failed: ".mysql_error());


if($_GET["op"]==1)
{
?>
<table width="702" border="1" align="center" cellspacing="0">
  <tr bgcolor="#FFFFCC"> 
    <td colspan="6"><div align="center"><strong><font color="#993300" size="4">VoD Users</font></strong></div></td>
  </tr>
  <tr bgcolor="#FFFFCC"> 
    <td width="33"><div align="center"></div></td>
    <td width="78"><strong>Login name</strong></td>
	<td width="78"><strong>Title</strong></td>
    <td width="86"><strong>Name</strong></td>
    <td width="118"><strong>Surname</strong></td>
    <td width="105"><strong>Privilege</strong></td>
    <td width="150"><strong>E-mail</strong></td>
  </tr><?
  
  $i=1;
  
  while($row = mysql_fetch_array($result))
  {  
  	if($row["priv"]==0) 
  	{
  		$priv="Student";
	}
	elseif($row["priv"]==1)
	{
		$priv="Instructor";
	}
	elseif($row["priv"]==2)
	{
		$priv="Administrator";
	}
	
  echo'
  <tr bgcolor="#FFFFCC"> 
    <td><div align="center"><strong>',$i,'.</strong></div></td>
    <td bgcolor="#FFFFFF">',$row["user"],'</td>
	<td bgcolor="#FFFFFF">',$row["title"],'</td>
    <td bgcolor="#FFFFFF">',$row["name"],'</td>
    <td bgcolor="#FFFFFF">',$row["surname"],'</td>
    <td bgcolor="#FFFFFF">',$priv,'</td>
    <td bgcolor="#FFFFFF"><a href="mailto:',$row["mail"],'">',$row["mail"],'</a></td>
  </tr>';
  $i+=1;
  }
  
echo '</table>';

///////////////////////////////////////////////////////////////////////////////////////////////////////////

}elseif($_GET["op"]==2)
{
echo'
<table width="702" border="1" align="center" cellspacing="0">
  <tr bgcolor="#FFFFCC"> 
    <td colspan="7"><div align="center"><strong><font color="#993300" size="4">VoD Users</font></strong></div></td>
  </tr>
  <tr bgcolor="#FFFFCC"> 
    <td width="49"><div align="center"></div></td>
    <td width="33"><div align="center"></div></td>
	<td width="78"><strong>Login Name</strong></td>
    <td width="78"><strong>Title</strong></td>
    <td width="86"><strong>Name</strong></td>
    <td width="118"><strong>Surname</strong></td>
    <td width="105"><strong>Privilege</strong></td>
    <td width="150"><strong>E-mail</strong></td>
  </tr>';
  $i=1;
  while($row = mysql_fetch_array($result))
  {  
  	if($row["priv"]==0) 
  	{
  		$priv="Student";
	}
	if($row["priv"]==1) 
  	{
  		$priv="Instructor";
	}
	elseif($row["priv"]==2)
	{
		$priv="Administrator";
	}
	
  echo'
  <tr bgcolor="#FFFFCC"> 
    <td><div align="center"><strong><a href="vod.php?action=adm/modifyuser.php&uid=',$row["userid"],'">Modify</a></strong></div></td>
    <td><div align="center"><strong>',$i,'.</strong></div></td>
    <td bgcolor="#FFFFFF">',$row["user"],'</td>
    <td bgcolor="#FFFFFF">',$row["title"],'</td>
    <td bgcolor="#FFFFFF">',$row["name"],'</td>
    <td bgcolor="#FFFFFF">',$row["surname"],'</td>
    <td bgcolor="#FFFFFF">',$priv,'</td>
    <td bgcolor="#FFFFFF"><a href="mailto:',$row["mail"],'">',$row["mail"],'</a></td>
  </tr>';
  $i+=1;
  }
  
echo '</table>';



///////////////////////////////////////////////////////////////////////////////////////////////////////////

}elseif($_GET["op"]==3)
{
echo'
<table width="702" border="1" align="center" cellspacing="0">
  <tr bgcolor="#FFFFCC"> 
    <td colspan="7"><div align="center"><strong><font color="#993300" size="4">VoD Users</font></strong></div></td>
  </tr>
  <tr bgcolor="#FFFFCC"> 
    <td width="49"><div align="center"></div></td>
    <td width="33"><div align="center"></div></td>
    <td width="78"><strong>Title</strong></td>
    <td width="86"><strong>Name</strong></td>
    <td width="118"><strong>Surname</strong></td>
    <td width="105"><strong>Privilege</strong></td>
    <td width="150"><strong>E-mail</strong></td>
  </tr>';
  $i=1;
  while($row = mysql_fetch_array($result))
  {  
  	if($row["priv"]==1) 
  	{
  		$priv="Student";
	}
	if($row["priv"]==1) 
  	{
  		$priv="Instructor";
	}
	elseif($row["priv"]==2)
	{
		$priv="Administrator";
	}
	
  echo'
  <tr bgcolor="#FFFFCC"> 
    <td><div align="center"><strong><a href="vod.php?action=adm/deleteuser.php&uid=',$row["userid"],'">Delete</a></strong></div></td>
    <td><div align="center"><strong>',$i,'.</strong></div></td>
    <td bgcolor="#FFFFFF">',$row["title"],'</td>
    <td bgcolor="#FFFFFF">',$row["name"],'</td>
    <td bgcolor="#FFFFFF">',$row["surname"],'</td>
    <td bgcolor="#FFFFFF">',$priv,'</td>
    <td bgcolor="#FFFFFF"><a href="mailto:',$row["mail"],'">',$row["mail"],'</a></td>
  </tr>';
  $i+=1;
  }
  
echo '</table>';

}
?>