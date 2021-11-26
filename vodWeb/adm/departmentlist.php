<?php
Authorize($hash,$user);
$link=DBconnect();
$result = mysql_query("SELECT id,name FROM department WHERE status<>0", $link);
if (!$result) die("query failed: ".mysql_error());

echo'
<table width="702" border="1" align="center" cellspacing="0">
  <tr bgcolor="#FFFFCC"> 
    <td colspan="4"><div align="center"><strong><font color="#993300" size="4">Departments</font></strong></div></td>
  </tr>
  <tr bgcolor="#FFFFCC"> 
    <td width="69"><div align="center"></div></td>
    <td width="69">&nbsp;</td>
    <td width="46">&nbsp;</td>
    <td width="500"><strong>Name</strong></td>
  </tr>';
  $i=0;
  while($row=mysql_fetch_array($result)){
  $i+=1;
  echo'
  <tr bgcolor="#FFFFCC"> 
    <td><div align="center"><strong><a href="vod.php?action=admin/modifydepartment.php&depid=',$row["id"],'">Modify</a></strong></div></td> 
    <td><div align="center"><a href="vod.php?action=admin/modifydepartment.php&depid=',$row["id"],'"><strong>Delete</strong></a></div></td>
    <td bgcolor="#FFFFFF"><div align="right"><strong>',$i,'.</strong></div></td>
    <td bgcolor="#FFFFFF">',$row["name"],'</td>
  </tr>';
  
  }

	echo'
</table>';
DBdisconnect($link);
?>
