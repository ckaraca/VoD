<?php
Authorize($hash,$user);
$link=DBconnect();
$result = mysql_query("SELECT * FROM ftp", $link);
if (!$result) die("query failed: ".mysql_error());


if($_GET["op"]==1)
{
?>
<table width="702" border="1" align="center" cellspacing="0">
  <tr bgcolor="#FFFFCC"> 
    <td colspan="8"><div align="center"><strong><font color="#993300" size="4">VoD Content Servers</font></strong></div></td>
  </tr>
  <tr bgcolor="#FFFFCC"> 
    <td ><div align="center">id</div></td>
    <td ><strong>Host IP</strong></td>
	<td ><strong>Port</strong></td>
    <td ><strong>User</strong></td>
    <td ><strong>Password</strong></td>
    <td ><strong>Description</strong></td>
    <td ><strong>Record Date</strong></td>
	<td ><strong>Notes</strong></td>
  </tr><?
  
  $i=1;
  
  while($row = mysql_fetch_array($result))
  {  
  	
  echo'
  <tr bgcolor="#FFFFCC"> 
    <td><div align="center"><strong>',$i,'.</strong></div></td>
    <td bgcolor="#FFFFFF">',$row["Host"],'</td>
	<td bgcolor="#FFFFFF">',$row["Port"],'</td>
    <td bgcolor="#FFFFFF">',$row["User"],'</td>
    <td bgcolor="#FFFFFF">***</td>
    <td bgcolor="#FFFFFF">',$row["dsc"],'</td>
    <td bgcolor="#FFFFFF">',$row["recorddate"],'</td>
	<td bgcolor="#FFFFFF">',$row["Notes"],'</td>
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
    <td colspan="8"><div align="center"><strong><font color="#993300" size="4">VoD Users</font></strong></div></td>
  </tr>
  <tr bgcolor="#FFFFCC"> 
    <td ><div align="center">Action</div></td>
    <td ><strong>Host IP</strong></td>
	<td ><strong>Port</strong></td>
    <td ><strong>User</strong></td>
    <td ><strong>Password</strong></td>
    <td ><strong>Description</strong></td>
    <td ><strong>Record Date</strong></td>
	<td ><strong>Notes</strong></td>
  </tr>';
  $i=1;
  while($row = mysql_fetch_array($result))
  {  
  	
  echo'
  <tr bgcolor="#FFFFCC"> 
    <td><div align="center"><strong><a href="vod.php?action=adm/modifyserver.php&uid=',$row["id"],'">Modify</a></strong></div></td>
    <td bgcolor="#FFFFFF">',$row["Host"],'</td>
	<td bgcolor="#FFFFFF">',$row["Port"],'</td>
    <td bgcolor="#FFFFFF">',$row["User"],'</td>
    <td bgcolor="#FFFFFF">***</td>
    <td bgcolor="#FFFFFF">',$row["dsc"],'</td>
    <td bgcolor="#FFFFFF">',$row["recorddate"],'</td>
	<td bgcolor="#FFFFFF">',$row["Notes"],'</td>
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
    <td colspan="2"><div align="center"><strong><font color="#993300" size="4">VoD Users</font></strong></div></td>
  </tr>
 <tr bgcolor="#FFFFCC"> 
    <td ><div align="center">Action</div></td>
    <td ><strong>Description</strong></td>
  </tr>';
  $i=1;
  while($row = mysql_fetch_array($result))
  {  
  		
  echo'
  <tr bgcolor="#FFFFCC"> 
    <td><div align="center"><strong><a href="vod.php?action=adm/deleteserver.php&uid=',$row["id"],'">Delete</a></strong></div></td>
    <td bgcolor="#FFFFFF">',$row["dsc"],'</td>
  </tr>';
  $i+=1;
  }
  
echo '</table>';

}
?>