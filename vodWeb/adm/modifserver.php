<?php
$link=DBconnect();
$result = mysql_query("SELECT * FROM ftp WHERE id='$_GET[uid]'", $link);
if (!$result) die("query failed: ".mysql_error());
$row = mysql_fetch_array($result);

echo'
<table width="550" border="0" align="center" cellspacing="0">
  <tr> 
    <td width="563" bgcolor="#FFFFCC"><div align="center"><font color="#990000" size="+7"><strong>Modify Server Information</strong></font></div></td>
  </tr>
</table>
<form name="form1" method="post" action="vod.php?action=adm/saveserver.php&op=2&uid=',$_GET["uid"],'">
  <table width="550" border="1" align="center" cellspacing="0">
    <tr bgcolor="#FFFFCC"> 
      <td colspan="2"><strong><font color="#993300" size="4">Server Information : </font></strong></td>
    </tr>
    <tr> 
      <td width="196" bgcolor="#FFFFCC"><div align="left"><font color="#996600"><strong>Host IP : </strong></font></div></td>
      <td width="350"><input name="title" type="text" id="title" value="',$row["title"],'"></td>
    </tr>
    <tr> 
      <td bgcolor="#FFFFCC"><div align="left"><font color="#996600"><strong>Port : </strong></font></div></td>
      <td><input name="name" type="text" id="name" value="',$row["name"],'"></td>
    </tr>
    <tr> 
      <td bgcolor="#FFFFCC"><div align="left"><font color="#996600"><strong>User : </strong></font></div></td>
      <td><input name="surname" type="text" id="surname" value="',$row["surname"],'"></td>
    </tr>
    <tr> 
      <td bgcolor="#FFFFCC"><font color="#996600"><strong>Password :</strong></font></td>
      <td><input name="mail" type="text" id="mail" value="',$row["mail"],'"></td>
    </tr>
  </table>
  <br>
  <table width="550" border="1" align="center" cellspacing="0">
    <tr bgcolor="#FFFFCC"> 
      <td colspan="2"><strong><font color="#993300" size="4">Login Information : </font></strong></td>
    </tr>
    <tr> 
      <td width="196" bgcolor="#FFFFCC"><div align="left"><font color="#996600"><strong>Login name :</strong></font></div></td>
      <td width="350"><input name="user" type="text" id="user" value="',$row["user"],'"></td>
    </tr>
    <tr> 
      <td bgcolor="#FFFFCC"><div align="left"><font color="#996600"><strong>Password : </strong></font></div></td>
      <td><input name="pass1" type="password" id="pass1"></td>
    </tr>
    <tr> 
      <td bgcolor="#FFFFCC"><div align="left"><font color="#996600"><strong>Confirm Password :</strong></font></div></td>
      <td><input name="pass2" type="password" id="pass2"></td>
    </tr>
    <tr> 
      <td bgcolor="#FFFFCC"><font color="#996600"><strong>Privilege :</strong></font></td>
      <td><select name="priv" id="priv">
          <option value="0" ';
		  if($row["priv"]==0) echo 'selected';
		  echo'>Student</option>
          <option value="1" ';
		  if($row["priv"]==1) echo 'selected';
		  echo'>Instructor</option>
          <option value="2" ';
		  if($row["priv"]==2) echo 'selected';
		  echo'>Administrator</option>
        </select></td>
    </tr>
  </table>
  <br>
  <div align="center">
    <table width="550" border="0" align="center" cellspacing="0">
      <tr> 
        <td width="563" bgcolor="#FFFFCC"><div align="center">
            <input type="submit" name="Submit" value="Save">
          </div></td>
      </tr>
    </table></div>
</form>
';
?>